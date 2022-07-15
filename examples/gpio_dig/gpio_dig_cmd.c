/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file emmc_cmd.c
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2022-07-15
 *
 * -----------------------------------------------------------------------------
 * @brief This file includes the digital gpio implementation.
 *
 */
/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */
#include "stdbool.h"
/* project */
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/AddrTranslateP.h>
#include <kernel/dpl/HwiP.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
/* own */
#include "gpio_dig_cmd.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/



/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "digial GPIO" command. */
const CLI_Command_Definition_t gpioDigCommandDef =
{
    "gpio",
    "\r\ngpio [GPIO]:\r\n This function sets the digital outputs or prints the current digital input state.\r\n\r\n",
    gpioDigCommand,
    0
};

/*******************************************************************************
 * local typedefs
 ******************************************************************************/



/*******************************************************************************
 * local static data
 ******************************************************************************/

uint32_t            gGpioBaseAddr = USER_BUTTON_BASE_ADDR;
HwiP_Object         gGpioHwiObject;

volatile uint32_t   gGpioIntrDone = 0;

/*******************************************************************************
 * forward declarations
 ******************************************************************************/

static void GPIO_bankIsrFxn(void *args);
extern void Board_gpioInit(void);
extern void Board_gpioDeinit(void);
extern uint32_t Board_getGpioButtonIntrNum(void);
extern uint32_t Board_getGpioButtonSwitchNum(void);

/*******************************************************************************
 * local static functions
 ******************************************************************************/



/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief This function handles the digital GPIO command
 *
 * @param pcWriteBuffer cli output string buffer
 * @param xWriteBufferLen length of the cli output string
 * @param pcCommandString cli command input string
 * @return pdFALSE = command is finished
 */
BaseType_t gpioDigCommand( char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString )
{
//    if (mmcsd_raw_io_main(NULL) == true)
//    {
//        sprintf(pcWriteBuffer, "eMMC all tests have passed \r\n");
//    }
//    else
//    {
//        sprintf(pcWriteBuffer, "eMMC tests failed\r\n");
//    }

    return pdFALSE;
}

bool gpioDigInit(void)
{
    bool        success   = false;
    int32_t     retVal    = 0;
    uint32_t    pinNum    = 0;
    uint32_t    intrNum   = 0;
    uint32_t    bankNum   = 0;
    uint32_t    waitCount = 5;
    HwiP_Params hwiPrms   = {0};
    uint32_t test = 0;

    /* Open drivers to open the UART driver for console */
    Drivers_open();
    Board_driversOpen();
    Board_gpioInit();

    pinNum          = USER_BUTTON_PIN;
    intrNum         = Board_getGpioButtonIntrNum();
    bankNum         = GPIO_GET_BANK_INDEX(pinNum);

    /* Address translate */
    gGpioBaseAddr = (uint32_t) AddrTranslateP_getLocalAddr(gGpioBaseAddr);

    /* Setup GPIO for interrupt generation */
    GPIO_setDirMode(gGpioBaseAddr, pinNum, USER_BUTTON_DIR);
    GPIO_setTrigType(gGpioBaseAddr, pinNum, USER_BUTTON_TRIG_TYPE);
    GPIO_bankIntrEnable(gGpioBaseAddr, bankNum);

    /* Register pin interrupt */
    HwiP_Params_init(&hwiPrms);
    hwiPrms.intNum   = intrNum;
    hwiPrms.callback = &GPIO_bankIsrFxn;
    hwiPrms.args     = (void *) pinNum;
    retVal = HwiP_construct(&gGpioHwiObject, &hwiPrms);
    DebugP_assert(retVal == SystemP_SUCCESS );

    DebugP_log("Press and release SW%d button on EVM to trigger GPIO interrupt ...\r\n", Board_getGpioButtonSwitchNum());
    while(gGpioIntrDone < waitCount)
    {
        test = GPIO_pinRead(gGpioBaseAddr, pinNum);
        /* Keep printing the current GPIO value */
        DebugP_log("Key is pressed %d %d times\r\n", gGpioIntrDone, test);
        ClockP_sleep(1);
    }
    DebugP_log("Key is pressed %d times\r\n", gGpioIntrDone);

    /* Unregister interrupt */
    GPIO_bankIntrDisable(gGpioBaseAddr, bankNum);
    GPIO_setTrigType(gGpioBaseAddr, pinNum, GPIO_TRIG_TYPE_NONE);
    GPIO_clearIntrStatus(gGpioBaseAddr, pinNum);
    HwiP_destruct(&gGpioHwiObject);

    return success;
}

static void GPIO_bankIsrFxn(void *args)
{
    uint32_t    pinNum = (uint32_t) args;
    uint32_t    bankNum =  GPIO_GET_BANK_INDEX(pinNum);
    uint32_t    intrStatus, pinMask = GPIO_GET_BANK_BIT_MASK(pinNum);

    /* Get and clear bank interrupt status */
    intrStatus = GPIO_getBankIntrStatus(gGpioBaseAddr, bankNum);
    GPIO_clearBankIntrStatus(gGpioBaseAddr, bankNum, intrStatus);

    /* Per pin interrupt handling */
    if(intrStatus & pinMask)
    {
        gGpioIntrDone++;
        DebugP_log("GPIO\r\n");
    }
}
/*[EOF]************************************************************************/
