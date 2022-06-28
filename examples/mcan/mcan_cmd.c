/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file mcan_cmd.c
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2022-06-21
 *
 * -----------------------------------------------------------------------------
 * @brief TODO
 *
 */

/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */
#include <stdio.h>
#include <string.h>
/* project */
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/TaskP.h>
#include <kernel/dpl/AddrTranslateP.h>
#include <kernel/dpl/SemaphoreP.h>
#include <drivers/mcan.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
/* own */
#include "mcan_cmd.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/

#define APP_MCAN_BASE_ADDR                       (CONFIG_MCAN1_BASE_ADDR)
#define APP_MCAN_INTR_NUM                        (CONFIG_MCAN1_INTR)
#define APP_MCAN_MSG_LOOP_COUNT                  (10U)

/* Allocate Message RAM memory section to filter elements, buffers, FIFO */
/* Maximum STD Filter Element can be configured is 128 */
#define APP_MCAN_STD_ID_FILTER_CNT               (0U)
/* Maximum EXT Filter Element can be configured is 64 */
#define APP_MCAN_EXT_ID_FILTER_CNT               (0U)
/* Maximum TX Buffer + TX FIFO, combined can be configured is 32 */
#define APP_MCAN_TX_BUFF_CNT                     (1U)
#define APP_MCAN_TX_FIFO_CNT                     (0U)
/* Maximum TX Event FIFO can be configured is 32 */
#define APP_MCAN_TX_EVENT_FIFO_CNT               (0U)
/* Maximum RX FIFO 0 can be configured is 64 */
#define APP_MCAN_FIFO_0_CNT                      (0U)
/* Maximum RX FIFO 1 can be configured is 64 and
 * rest of the memory is allocated to RX buffer which is again of max size 64 */
#define APP_MCAN_FIFO_1_CNT                      (0U)

/* Standard Id configured in this app */
#define APP_MCAN_STD_ID                          (0xC0U)
#define APP_MCAN_STD_ID_MASK                     (0x7FFU)
#define APP_MCAN_STD_ID_SHIFT                    (18U)

#define APP_MCAN_EXT_ID_MASK                     (0x1FFFFFFFU)

/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "ledblink" command. */
const CLI_Command_Definition_t mcanCommandDef =
{
    "mcan",
    "\r\nmcan [MCAN]:\r\n TODO\r\n\r\n",
    mcanCommand,
    1
};

/*******************************************************************************
 * local typedefs
 ******************************************************************************/



/*******************************************************************************
 * local static data
 ******************************************************************************/

static SemaphoreP_Object gMcanTxDoneSem;
static uint32_t          gMcanRxBaseAddr;
static HwiP_Object       gMcanTxHwiObject;

/* Semaphore to indicate transfer completion */
static SemaphoreP_Object gMcanRxDoneSem;
static HwiP_Object       gMcanRxHwiObject;
//static uint32_t          gMcanRxBaseAddr;


/* In the CAN FD format, the Data length coding differs from the standard CAN.
 * In case of standard CAN it is 8 bytes */
static const uint8_t gMcanTxDataSize[16U] = {0U,  1U,  2U,  3U,
                                             4U,  5U,  6U,  7U,
                                             8U,  12U, 16U, 20U,
                                             24U, 32U, 48U, 64U};

/*******************************************************************************
 * forward declarations
 ******************************************************************************/

extern void mcan_loopback_interrupt_main(void *args);
extern void mcan_loopback_polling_main(void *args);
extern void mcan_loopback_rx_interrupt_main(void *args);
extern void mcan_loopback_tx_interrupt_main(void *args);

static void App_mcanIntrISR(void *arg);
static void App_mcanConfig(void);
static void App_mcanInitMsgRamConfigParams(MCAN_MsgRAMConfigParams *msgRAMConfigParams);
static void App_mcanEnableIntr(void);
static void App_mcanConfigTxMsg(MCAN_TxBufElement *txMsg);

static int32_t initMcanTx(void);
static void deinitMcanTx(void);

/*******************************************************************************
 * local static functions
 ******************************************************************************/

void test (void)
{
    int32_t                 status = SystemP_SUCCESS;
//    HwiP_Params             hwiPrms;
    MCAN_TxBufElement       txMsg;
    MCAN_ProtocolStatus     protStatus;
    uint32_t                i, bufNum = 0U;

    Board_driversOpen();

    status = initMcanTx();
    DebugP_assert(status == SystemP_SUCCESS);

    /* Transmit And Receive Message */
    for (i = 0U; i < APP_MCAN_MSG_LOOP_COUNT; i++)
    {

        /* Wait for mcan rx application to be ready */
        IpcNotify_syncAll(SystemP_WAIT_FOREVER);

        /* Configure Tx Msg to transmit */
        App_mcanConfigTxMsg(&txMsg);

        /* Select buffer number, 32 buffers available */
        bufNum = 0U;

        /* Enable Transmission interrupt for the selected buf num,
         * If FIFO is used, then need to send FIFO start index until FIFO count */
        status = MCAN_txBufTransIntrEnable(gMcanRxBaseAddr, bufNum, (uint32_t)TRUE);
        DebugP_assert(status == CSL_PASS);

        /* Write message to Msg RAM */
        MCAN_writeMsgRam(gMcanRxBaseAddr, MCAN_MEM_TYPE_BUF, bufNum, &txMsg);

        /* Add request for transmission, This function will trigger transmission */
        status = MCAN_txBufAddReq(gMcanRxBaseAddr, bufNum);
        DebugP_assert(status == CSL_PASS);

        /* Wait for Tx completion */
        SemaphoreP_pend(&gMcanTxDoneSem, 100);

        MCAN_getProtocolStatus(gMcanRxBaseAddr, &protStatus);
        /* Checking for Tx Errors */
        if (((MCAN_ERR_CODE_NO_ERROR != protStatus.lastErrCode) ||
             (MCAN_ERR_CODE_NO_CHANGE != protStatus.lastErrCode)) &&
            ((MCAN_ERR_CODE_NO_ERROR != protStatus.dlec) ||
             (MCAN_ERR_CODE_NO_CHANGE != protStatus.dlec)) &&
            (0U != protStatus.pxe))
        {
             DebugP_assert(FALSE);
        }
    }

    /* De-Construct Tx Semaphore objects */
    HwiP_destruct(&gMcanTxHwiObject);
    SemaphoreP_destruct(&gMcanTxDoneSem);

    deinitMcanTx();

    DebugP_log("[MCAN]TX test passed!!\r\n");

    Board_driversClose();
}

static int32_t initMcanRx(void)
{
    int32_t status = SystemP_SUCCESS;
    HwiP_Params hwiPrms;

    /* Wait for mcan tx application to be ready */
    IpcNotify_syncAll(SystemP_WAIT_FOREVER);

    DebugP_log("[MCAN] RX Application, Interrupt mode started ...\r\n");

    /* Construct Rx Semaphore objects */
    status = SemaphoreP_constructBinary(&gMcanRxDoneSem, 0);
    DebugP_assert(SystemP_SUCCESS == status);

    /* Register interrupt */
    HwiP_Params_init(&hwiPrms);
    hwiPrms.intNum      = APP_MCAN_INTR_NUM;
    hwiPrms.callback    = &App_mcanIntrISR;
    status              = HwiP_construct(&gMcanRxHwiObject, &hwiPrms);
    DebugP_assert(status == SystemP_SUCCESS);

    /* Assign MCAN instance address */
    gMcanRxBaseAddr = (uint32_t) AddrTranslateP_getLocalAddr(APP_MCAN_BASE_ADDR);

    /* Configure MCAN module, Enable LoopBack Mode */
    App_mcanConfig();

    /* Enable Interrupts */
    App_mcanEnableIntr();

    return status;
}

static int32_t initMcanTx(void)
{
    int32_t                 status = SystemP_SUCCESS;
    HwiP_Params             hwiPrms;

    /* Wait for mcan rx application to be ready */
    IpcNotify_syncAll(SystemP_WAIT_FOREVER);

    /* Construct Tx Semaphore objects */
    status = SemaphoreP_constructBinary(&gMcanTxDoneSem, 0);
    DebugP_assert(SystemP_SUCCESS == status);

    /* Register interrupt */
    HwiP_Params_init(&hwiPrms);
    hwiPrms.intNum      = APP_MCAN_INTR_NUM;
    hwiPrms.callback    = &App_mcanIntrISR;
    status              = HwiP_construct(&gMcanTxHwiObject, &hwiPrms);
    DebugP_assert(status == SystemP_SUCCESS);

    /* Assign MCAN instance address */
    gMcanRxBaseAddr = (uint32_t) AddrTranslateP_getLocalAddr(APP_MCAN_BASE_ADDR);

    /* Configure MCAN module, Enable LoopBack Mode */
    App_mcanConfig();

    /* Enable Interrupts */
    App_mcanEnableIntr();

    return status;
}

static void deinitMcanTx(void)
{
    /* De-Construct Tx Semaphore objects */
    HwiP_destruct(&gMcanTxHwiObject);
    SemaphoreP_destruct(&gMcanTxDoneSem);

    /* Wait for mcan tx application to be ready */
    IpcNotify_syncAll(SystemP_WAIT_FOREVER);
}

static void App_mcanConfig(void)
{
    MCAN_InitParams            initParams           = {0U};
    MCAN_ConfigParams          configParams         = {0U};
    MCAN_MsgRAMConfigParams    msgRAMConfigParams   = {0U};
    MCAN_BitTimingParams       bitTimes             = {0U};

    /* Initialize MCAN module initParams */
    MCAN_initOperModeParams(&initParams);
    /* CAN FD Mode and Bit Rate Switch Enabled */
    initParams.fdMode          = FALSE;
    initParams.brsEnable       = FALSE;

    /* Initialize MCAN module Global Filter Params */
    MCAN_initGlobalFilterConfigParams(&configParams);

    /* Initialize MCAN module Bit Time Params */
    /* Configuring default 1Mbps and 5Mbps as nominal and data bit-rate resp */
    MCAN_initSetBitTimeParams(&bitTimes);

    /* Initialize MCAN module Message Ram Params */
    App_mcanInitMsgRamConfigParams(&msgRAMConfigParams);

    /* wait for memory initialization to happen */
    while (FALSE == MCAN_isMemInitDone(gMcanRxBaseAddr))
    {}

    /* Put MCAN in SW initialization mode */
    MCAN_setOpMode(gMcanRxBaseAddr, MCAN_OPERATION_MODE_SW_INIT);
    while (MCAN_OPERATION_MODE_SW_INIT != MCAN_getOpMode(gMcanRxBaseAddr))
    {}

    /* Initialize MCAN module */
    MCAN_init(gMcanRxBaseAddr, &initParams);
    /* Configure MCAN module Gloabal Filter */
    MCAN_config(gMcanRxBaseAddr, &configParams);
    /* Configure Bit timings */
    MCAN_setBitTime(gMcanRxBaseAddr, &bitTimes);
    /* Configure Message RAM Sections */
    MCAN_msgRAMConfig(gMcanRxBaseAddr, &msgRAMConfigParams);
    /* Set Extended ID Mask */
    MCAN_setExtIDAndMask(gMcanRxBaseAddr, APP_MCAN_EXT_ID_MASK);

    /* Take MCAN out of the SW initialization mode */
    MCAN_setOpMode(gMcanRxBaseAddr, MCAN_OPERATION_MODE_NORMAL);
    while (MCAN_OPERATION_MODE_NORMAL != MCAN_getOpMode(gMcanRxBaseAddr))
    {}

    return;
}

static void App_mcanConfigTxMsg(MCAN_TxBufElement *txMsg)
{
    uint32_t i;

    /* Initialize message to transmit */
    MCAN_initTxBufElement(txMsg);
    /* Standard message identifier 11 bit, stored into ID[28-18] */
    txMsg->id  = ((APP_MCAN_STD_ID & MCAN_STD_ID_MASK) << MCAN_STD_ID_SHIFT);
    txMsg->dlc = MCAN_DATA_SIZE_8BYTES; /* Payload size is 64 bytes */
    txMsg->fdf = FALSE; /* CAN FD Frame Format */
    txMsg->xtd = FALSE; /* Extended id not configured */
    for (i = 0U; i < gMcanTxDataSize[MCAN_DATA_SIZE_8BYTES]; i++)
    {
        txMsg->data[i] = i;
    }

    return;
}

static void App_mcanEnableIntr(void)
{
    MCAN_enableIntr(gMcanRxBaseAddr, MCAN_INTR_MASK_ALL, (uint32_t)TRUE);
    MCAN_enableIntr(gMcanRxBaseAddr, MCAN_INTR_SRC_RES_ADDR_ACCESS, (uint32_t)FALSE);
    /* Select Interrupt Line 0 */
    MCAN_selectIntrLine(gMcanRxBaseAddr, MCAN_INTR_MASK_ALL, MCAN_INTR_LINE_NUM_0);
    /* Enable Interrupt Line */
    MCAN_enableIntrLine(gMcanRxBaseAddr, MCAN_INTR_LINE_NUM_0, (uint32_t)TRUE);

    return;
}

static void App_mcanInitMsgRamConfigParams(MCAN_MsgRAMConfigParams* msgRAMConfigParams)
{
    int32_t status;

    MCAN_initMsgRamConfigParams(msgRAMConfigParams);

    /* Configure the user required msg ram params */
    msgRAMConfigParams->lss = APP_MCAN_STD_ID_FILTER_CNT;
    msgRAMConfigParams->lse = APP_MCAN_EXT_ID_FILTER_CNT;
    msgRAMConfigParams->txBufCnt = APP_MCAN_TX_BUFF_CNT;
    msgRAMConfigParams->txFIFOCnt = APP_MCAN_TX_FIFO_CNT;
    /* Buffer/FIFO mode is selected */
    msgRAMConfigParams->txBufMode = MCAN_TX_MEM_TYPE_BUF;
    msgRAMConfigParams->txEventFIFOCnt = APP_MCAN_TX_EVENT_FIFO_CNT;
    msgRAMConfigParams->rxFIFO0Cnt = APP_MCAN_FIFO_0_CNT;
    msgRAMConfigParams->rxFIFO1Cnt = APP_MCAN_FIFO_1_CNT;
    /* FIFO blocking mode is selected */
    msgRAMConfigParams->rxFIFO0OpMode = MCAN_RX_FIFO_OPERATION_MODE_BLOCKING;
    msgRAMConfigParams->rxFIFO1OpMode = MCAN_RX_FIFO_OPERATION_MODE_BLOCKING;

    status = MCAN_calcMsgRamParamsStartAddr(msgRAMConfigParams);
    DebugP_assert(status == CSL_PASS);

    return;
}

static void App_mcanIntrISR(void *arg)
{
    uint32_t intrStatus;

    intrStatus = MCAN_getIntrStatus(gMcanRxBaseAddr);
    MCAN_clearIntrStatus(gMcanRxBaseAddr, intrStatus);

    if (MCAN_INTR_SRC_TRANS_COMPLETE == (intrStatus & MCAN_INTR_SRC_TRANS_COMPLETE))
    {
        SemaphoreP_post(&gMcanTxDoneSem);
    }

    return;
}

/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief This function handles the MCAN command
 *
 * @param pcWriteBuffer cli output string buffer
 * @param xWriteBufferLen length of the cli output string
 * @param pcCommandString cli command input string
 * @return success
 */
BaseType_t mcanCommand( char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString )
{
    BaseType_t xParameterStringLength       = 0;
    const char* pcParameter1 = FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameterStringLength);

//    BaseType_t xReturned;
//    TaskHandle_t xHandle = NULL;

//    /* Create the task, storing the handle. */
//    xReturned = xTaskCreate(
//                            vTaskCode,       /* Function that implements the task. */
//                            "NAME",          /* Text name for the task. */
//                            STACK_SIZE,      /* Stack size in words, not bytes. */
//                            ( void * ) 1,    /* Parameter passed into the task. */
//                            tskIDLE_PRIORITY,/* Priority at which the task is created. */
//                            &xHandle );      /* Used to pass out the created task's handle. */
//
//    vTaskDelay(pdMS_TO_TICKS(100));



    switch (*pcParameter1)
    {
    case '1':
//        mcan_loopback_interrupt_main(NULL);
//        mcan_loopback_tx_interrupt_main(NULL);
        test();
        sprintf(pcWriteBuffer, "success\r\n");
        break;

    case '2':
//        mcan_loopback_polling_main(NULL);
        mcan_loopback_rx_interrupt_main(NULL);
        sprintf(pcWriteBuffer, "success\r\n");
        break;

    default:
        sprintf(pcWriteBuffer, "wrong parameter \r\n");
        break;
    }

    return pdFALSE;
}

/*[EOF]************************************************************************/
