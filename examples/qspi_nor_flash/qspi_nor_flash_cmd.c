/**
 * @file qspi_nor_flash_cmd.c
 * @author Copyright (c) 2022, TQ-Systems GmbH
 * @author TODO author of the file <TODO@tq-group.com>
 * @date 2022-05-23
 *
 * This software code contained herein is licensed under the terms and
 * conditions of the TQ-Systems Software License Agreement Version 1.0.2.
 * You may obtain a copy of the TQ-Systems Software License Agreement in the
 * folder TQS (TQ-Systems Software Licenses) at the following website:
 * https://www.tq-group.com/Software-Lizenzbedingungen
 * In case of any license issues please contact license@tq-group.com.
 *
 * -----------------------------------------------------------------------------
 * @brief <TODO short description of the file (only one line)>
 *
 * <TODO Detailed description of the file>
 */

/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
/* project */
#include <kernel/dpl/DebugP.h>
#include <drivers/ospi.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
/* own */
#include "qspi_nor_flash_cmd.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/



/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "i2cBusScan" command. */
const CLI_Command_Definition_t qspiNorFlashCommandDef =
{
    "qspiflash",
    "\r\nqspiflash:\r\n Read, write in NOR flash \r\n\r\n",
    qspiNorFlashCommand,
    1
};

/*******************************************************************************
 * local typedefs
 ******************************************************************************/

/* Commands which require tPSL/tESL delay */
#define CMD_READ       (0x03)
#define CMD_FAST_READ  (0x0B)
#define CMD_2READ      (0xBB)
#define CMD_DREAD      (0x3B)
#define CMD_4READ      (0xEB)
#define CMD_QREAD      (0x6B)
#define CMD_4READ4B    (0xEC)
#define CMD_4DTRD      (0xED)
#define CMD_4DTRD4B    (0xEE)
#define CMD_FASTREAD4B (0x0C)
#define CMD_2READ4B    (0xBC)
#define CMD_DREAD4B    (0x3C)
#define CMD_RDSFDP     (0x5A)
#define CMD_RDID       (0x9F)
#define CMD_QPIID      (0xAF)
#define CMD_SBL        (0xC0)
#define CMD_ENSO       (0xB1)
#define CMD_EXSO       (0xC1)
#define CMD_WREN       (0x06)
#define CMD_RESUME     (0x30)
#define CMD_RDLR       (0x2D)
#define CMD_RDSPB      (0xE2)
#define CMD_RDFBR      (0x16)
#define CMD_RDDPB      (0xE0)
#define CMD_EQIO       (0x35)
#define CMD_RSTQIO     (0xF5)

/* Commands not required tPSL/tESL delay */
#define CMD_WRDI       (0x04)
#define CMD_RDSR       (0x05)
#define CMD_RDCR       (0x15)
#define CMD_RDSCUR     (0x2B)
#define CMD_RES        (0xAB)
#define CMD_REMS       (0x90)
#define CMD_RSTEN      (0x66)
#define CMD_RST        (0x99)
#define CMD_NOP        (0x00)

#define CMD_PP         (0x02)
#define CMD_4PP        (0x38)
#define CMD_SE         (0x20)
#define CMD_BE_32K     (0x52)
#define CMD_BE         (0xD8)
#define CMD_CE         (0x60)

#define OSPI_NOR_WRR_WRITE_TIMEOUT  (600U * 1000U)
#define OSPI_NOR_PAGE_PROG_TIMEOUT  (400U)
/*******************************************************************************
 * local static data
 ******************************************************************************/



/*******************************************************************************
 * forward declarations
 ******************************************************************************/



/*******************************************************************************
 * local static functions
 ******************************************************************************/

//static bool writeFlash(void)
//{
//    int32_t status = SystemP_SUCCESS;
//    bool success   = false;
//    OSPI_Handle qspiHanlde = {0};
//    uint8_t buf[10]        = {0};
//    OSPI_Transaction transaction = {0};
//
//    OSPI_Transaction_init(&transaction);
//
//    transaction.count      = 1;
//    transaction.addrOffset = 0;
//    transaction.buf        = buf;
//    transaction.transferTimeout = 50;
//
//    buf[0] = CMD_WRDI;
//
//    /* Open OSPI Driver, among others */
//    Drivers_ospiOpen();
//
//    /* Open Flash drivers with OSPI instance as input */
//    status = Board_driversOpen();
//
//    qspiHanlde = OSPI_getHandle(CONFIG_OSPI0);
//
//    status = OSPI_writeDirect(qspiHanlde, &transaction);
////    status = OSPI_norFlashWrite(qspiHanlde, 0, buf, 1);
//
//    if (status == SystemP_SUCCESS)
//    {
//        buf[0] = CMD_PP;
//        status = OSPI_writeDirect(qspiHanlde, &transaction);
//    }
//
//    if (status == SystemP_SUCCESS)
//    {
//        transaction.count = 4;
//        buf[0] = 0x0;
//        buf[1] = 0x0;
//        buf[2] = 0x0;
//        buf[3] = 0x1;
//        status = OSPI_writeDirect(qspiHanlde, &transaction);
//    }
//
//    if (status == SystemP_SUCCESS)
//    {
//        transaction.count = 1;
//        buf[0] = 0xAA;
//        status = OSPI_writeDirect(qspiHanlde, &transaction);
//    }
//
//    Board_driversClose();
//    Drivers_ospiClose();
//
//    if (status == SystemP_SUCCESS)
//    {
////        DebugP_log("[QSPI] Read data %d %d %d %d %d.\r\n", buf[0], buf[1], buf[2], buf[3] , buf[4]);
//    }
//    else
//    {
//        DebugP_log("[QSPI] Error %d.\r\n", status);
//    }
//
//    return success;
//}

//static bool writeFlash(void)
//{
//    bool success   = false;
//    int32_t status = SystemP_SUCCESS;
//    OSPI_Handle qspiHanlde = {0};
//    uint8_t buf[10]        = {0};
//
//    /* Open OSPI Driver, among others */
//    Drivers_ospiOpen();
//
//    /* Open Flash drivers with OSPI instance as input */
//    status = Board_driversOpen();
//
//    qspiHanlde = OSPI_getHandle(CONFIG_OSPI0);
//
//    for (uint8_t i = 0; i < sizeof(buf); i++)
//    {
//        buf[i] = i;
//    }
//
//    status = OSPI_norFlashWrite(qspiHanlde, 2, buf, 4);
//
//    if (status == SystemP_SUCCESS)
//    {
//        DebugP_log("[QSPI] Data write %02X %02X %02X %02X %02X.\r\n", buf[0], buf[1], buf[2], buf[3] , buf[4]);
//    }
//    else
//    {
//        DebugP_log("[QSPI] Error write flash: %d.\r\n", status);
//    }
//
//    Board_driversClose();
//    Drivers_ospiClose();
//
//    return success;
//}

static bool writeFlash(void)
{
    bool success   = false;
    int32_t status = SystemP_SUCCESS;
    OSPI_Handle qspiHanlde = {0};
    uint8_t buf[10]        = {0};
    OSPI_WriteCmdParams wrParams = {0};
    OSPI_Transaction transaction = {0};

    /* Open OSPI Driver, among others */
    Drivers_ospiOpen();

    /* Open Flash drivers with OSPI instance as input */
    status = Board_driversOpen();

    qspiHanlde = OSPI_getHandle(CONFIG_OSPI0);

    for (uint8_t i = 0; i < sizeof(buf); i++)
    {
        buf[i] = i;
    }

    OSPI_WriteCmdParams_init(&wrParams);

    wrParams.cmd = CMD_WREN;
    status = OSPI_writeCmd(qspiHanlde, &wrParams);

    if( status == SystemP_SUCCESS)
    {
        wrParams.cmdAddr = 0;
        wrParams.cmd = CMD_PP;
        status = OSPI_writeCmd(qspiHanlde, &wrParams);
    }

    if (status == SystemP_SUCCESS)
    {
        status = OSPI_norFlashWaitReady(qspiHanlde, OSPI_NOR_WRR_WRITE_TIMEOUT);

        if (status == SystemP_SUCCESS)
        {
            OSPI_Transaction_init(&transaction);
            transaction.addrOffset = 0x0;
            transaction.buf = buf;
            transaction.count = 5;
            status = OSPI_writeDirect(qspiHanlde, &transaction);
        }
    }

    if(status == SystemP_SUCCESS)
    {
        status = OSPI_norFlashWaitReady(qspiHanlde, OSPI_NOR_PAGE_PROG_TIMEOUT);
    }

    if (status == SystemP_SUCCESS)
    {
        DebugP_log("[QSPI] Data write %02X %02X %02X %02X %02X.\r\n", buf[0], buf[1], buf[2], buf[3] , buf[4]);
    }
    else
    {
        DebugP_log("[QSPI] Error write flash: %d.\r\n", status);
    }

    Board_driversClose();
    Drivers_ospiClose();

    return success;
}

static void readStatusReg(void)
{
    OSPI_Handle qspiHanlde = {0};
    int32_t status = SystemP_SUCCESS;
    uint8_t readStatus = 0U;
    OSPI_ReadCmdParams rdParams = {0};

    /* Open OSPI Driver, among others */
    Drivers_ospiOpen();

    /* Open Flash drivers with OSPI instance as input */
    status = Board_driversOpen();

    qspiHanlde = OSPI_getHandle(CONFIG_OSPI0);

    OSPI_ReadCmdParams_init(&rdParams);
    rdParams.cmd       = CMD_RDSR;
    rdParams.rxDataBuf = &readStatus;
    rdParams.rxDataLen = 1;

    status = OSPI_readCmd(qspiHanlde, &rdParams);

    if (status == SystemP_SUCCESS)
    {
        DebugP_log("[QSPI] Status %02X.\r\n", readStatus);
    }
    else
    {
        DebugP_log("[QSPI] Error %d.\r\n", status);
    }

    Board_driversClose();
    Drivers_ospiClose();
}

static bool readFlash(void)
{
    int32_t status = SystemP_SUCCESS;
    bool success   = false;
    OSPI_Handle qspiHanlde = {0};
    OSPI_WriteCmdParams wrParams = {0};
    uint8_t buf[10]        = {0};
    OSPI_Transaction transaction = {0};

    OSPI_WriteCmdParams_init(&wrParams);

    wrParams.cmd = CMD_READ;
    status = OSPI_writeCmd(qspiHanlde, &wrParams);

    OSPI_Transaction_init(&transaction);

    transaction.count      = 5;
    transaction.addrOffset = 0x0;
    transaction.buf        = buf;
    transaction.transferTimeout = 50;

    /* Open OSPI Driver, among others */
    Drivers_ospiOpen();

    /* Open Flash drivers with OSPI instance as input */
    status = Board_driversOpen();

    qspiHanlde = OSPI_getHandle(CONFIG_OSPI0);
//
    if (status == SystemP_SUCCESS)
    {
//        status = OSPI_readDirect(qspiHanlde, &transaction);
        status = OSPI_norFlashRead(qspiHanlde, transaction.addrOffset, buf, 5);
    }

    Board_driversClose();
    Drivers_ospiClose();

    if (status == SystemP_SUCCESS)
    {
        DebugP_log("[QSPI] Read data %02X %02X %02X %02X %02X.\r\n", buf[0], buf[1], buf[2], buf[3] , buf[4]);
    }
    else
    {
        DebugP_log("[QSPI] Error %d.\r\n", status);
    }

    return success;
}

static bool eraseflash(void)
{
    bool success                 = false;
    int32_t status               = SystemP_SUCCESS;
    OSPI_Handle qspiHanlde       = {0};
    uint8_t buf[10]              = {0};
    OSPI_WriteCmdParams wrParams = {0};

    /* Open OSPI Driver, among others */
    Drivers_ospiOpen();

    /* Open Flash drivers with OSPI instance as input */
    status = Board_driversOpen();

    OSPI_WriteCmdParams_init(&wrParams);

    wrParams.cmd          = CMD_BE;
//    wrParams.cmdAddr      = 0;
//    wrParams.numAddrBytes = numAddrBytes;
    wrParams.txDataBuf    = buf;
    wrParams.txDataLen    = 0;


//    status = OSPI_norFlashErase(qspiHanlde, 1);
    status = OSPI_writeCmd(qspiHanlde, &wrParams);
    if (status == SystemP_SUCCESS)
    {
        DebugP_log("[QSPI] Flash erased.\r\n");
    }
    else
    {
        DebugP_log("[QSPI] Failure flash erase %d!!!\r\n", status);
    }

    Board_driversClose();
    Drivers_ospiClose();

    return success;
}

/*******************************************************************************
 * global functions
 ******************************************************************************/

BaseType_t qspiNorFlashCommand( char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString )
{
    BaseType_t retVal = pdFALSE;
    BaseType_t xParameter1StringLength = 0;
    const char* pcParameter1 = FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameter1StringLength);

    switch (*pcParameter1)
    {
    case 'r':
        readFlash();
        sprintf(pcWriteBuffer, "read flash\r\n");
        break;

    case 'w':
        writeFlash();
        sprintf(pcWriteBuffer, "write flash\r\n");
        break;

    case 'e':
        eraseflash();
        sprintf(pcWriteBuffer, "erase flash\r\n");
        break;

    case 's':
        readStatusReg();
        sprintf(pcWriteBuffer, "read flash status\r\n");
        break;

    default:
        sprintf(pcWriteBuffer, "Wrong parameter\r\n");
        break;
    }

    return retVal;
}


/*[EOF]************************************************************************/
