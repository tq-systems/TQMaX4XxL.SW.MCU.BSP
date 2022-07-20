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
#include "FreeRTOS.h"
#include "task.h"
/* own */
#include "gpio_dig_cmd.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/

#define TASK_DELAY pdMS_TO_TICKS(10)

/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "digial GPIO" command. */
const CLI_Command_Definition_t gpioDigCommandDef =
{
    "gpio",
    "\r\ngpio [GPIO]:\r\n This function sets the digital outputs or prints the current digital input state.\r\n\r\n",
    gpioDigCommand,
    1
};

/*******************************************************************************
 * local typedefs
 ******************************************************************************/

enum
{
    DIG_IN_1 = 0,
    DIG_IN_2,
    DIG_IN_3,
    DIG_IN_4,
    DIG_IN_MAX
};

enum
{
    OUT_1 = 0,
    OUT_2,
    OUT_3,
    OUT_4,
    OUT_MAX,
};

enum
{
    DIG_OUTPUT = 0,
    DIG_OUT_STATUS,
    DIG_OUT_MAX,
};

typedef struct
{
    bool enDigOut;
    bool status;
} digOut_t;

/*******************************************************************************
 * local static data
 ******************************************************************************/

static bool     digIn[DIG_IN_MAX]   = {false};
static digOut_t digOut[OUT_MAX]     = {false};
static bool     pushButton          = false;

/*******************************************************************************
 * forward declarations
 ******************************************************************************/


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
    char*      pcParameter                  = {NULL};
    BaseType_t xParameterStringLength       = 0;

    pcParameter = (char*) FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameterStringLength);

    switch (*pcParameter)
    {
    case '1':
        GPIO_pinWriteHigh((uint32_t) AddrTranslateP_getLocalAddr(EN_DIG_OUT_1_BASE_ADDR), EN_DIG_OUT_1_PIN);
        break;

    default:
        break;
    }

    return pdFALSE;
}

void gpioPollingTask(void *pvParameters)
{
    uint8_t  i                                    = 0;
    bool     pushButtonOld                        = pushButton;
    bool     digInOld[DIG_IN_MAX]                 = {false};
    bool     digOutStatusOld[OUT_MAX]             = {false};

    uint32_t userButtonAddr                       = 0;
    uint32_t digInBaseAddr[DIG_IN_MAX]            = {0};
    uint32_t digInPin[DIG_IN_MAX]                 = {DIG_IN_1_PIN, DIG_IN_2_PIN, DIG_IN_3_PIN, DIG_IN_4_PIN};
    uint32_t digInDir[DIG_IN_MAX]                 = {DIG_IN_1_DIR, DIG_IN_2_DIR, DIG_IN_3_DIR, DIG_IN_4_DIR};
    uint32_t digOutBaseAddr[OUT_MAX][DIG_OUT_MAX] = {0};

    uint32_t digOutPin[OUT_MAX][DIG_OUT_MAX]      = {{EN_DIG_OUT_1_PIN, STATUS_OUT_1_PIN},
                                                     {EN_DIG_OUT_2_PIN, STATUS_OUT_2_PIN},
                                                     {EN_DIG_OUT_3_PIN, STATUS_OUT_3_PIN},
                                                     {EN_DIG_OUT_4_PIN, STATUS_OUT_4_PIN}};

    uint32_t digOutDir[OUT_MAX][DIG_OUT_MAX]      = {{EN_DIG_OUT_1_DIR, STATUS_OUT_1_DIR},
                                                     {EN_DIG_OUT_2_DIR, STATUS_OUT_2_DIR},
                                                     {EN_DIG_OUT_3_DIR, STATUS_OUT_3_DIR},
                                                     {EN_DIG_OUT_4_DIR, STATUS_OUT_4_DIR}};

    userButtonAddr          = (uint32_t)AddrTranslateP_getLocalAddr(USER_BUTTON_BASE_ADDR);

    digInBaseAddr[DIG_IN_1] = (uint32_t)AddrTranslateP_getLocalAddr(DIG_IN_1_BASE_ADDR);
    digInBaseAddr[DIG_IN_2] = (uint32_t)AddrTranslateP_getLocalAddr(DIG_IN_2_BASE_ADDR);
    digInBaseAddr[DIG_IN_3] = (uint32_t)AddrTranslateP_getLocalAddr(DIG_IN_3_BASE_ADDR);
    digInBaseAddr[DIG_IN_4] = (uint32_t)AddrTranslateP_getLocalAddr(DIG_IN_4_BASE_ADDR);

    digOutBaseAddr[OUT_1][DIG_OUTPUT]     = (uint32_t)AddrTranslateP_getLocalAddr(EN_DIG_OUT_1_BASE_ADDR);
    digOutBaseAddr[OUT_1][DIG_OUT_STATUS] = (uint32_t)AddrTranslateP_getLocalAddr(STATUS_OUT_1_BASE_ADDR);
    digOutBaseAddr[OUT_2][DIG_OUTPUT]     = (uint32_t)AddrTranslateP_getLocalAddr(EN_DIG_OUT_2_BASE_ADDR);
    digOutBaseAddr[OUT_2][DIG_OUT_STATUS] = (uint32_t)AddrTranslateP_getLocalAddr(STATUS_OUT_2_BASE_ADDR);
    digOutBaseAddr[OUT_3][DIG_OUTPUT]     = (uint32_t)AddrTranslateP_getLocalAddr(EN_DIG_OUT_3_BASE_ADDR);
    digOutBaseAddr[OUT_3][DIG_OUT_STATUS] = (uint32_t)AddrTranslateP_getLocalAddr(STATUS_OUT_3_BASE_ADDR);
    digOutBaseAddr[OUT_4][DIG_OUTPUT]     = (uint32_t)AddrTranslateP_getLocalAddr(EN_DIG_OUT_4_BASE_ADDR);
    digOutBaseAddr[OUT_4][DIG_OUT_STATUS] = (uint32_t)AddrTranslateP_getLocalAddr(STATUS_OUT_4_BASE_ADDR);

    /* GPIO init */
    GPIO_setDirMode(userButtonAddr, USER_BUTTON_PIN, USER_BUTTON_DIR);

    for (i = 0; i < DIG_IN_MAX; i++)
    {
        GPIO_setDirMode(digInBaseAddr[i], digInPin[i], digInDir[i]);
    }

    for (i = 0; i < OUT_MAX; i++)
    {
        GPIO_setDirMode(digOutBaseAddr[i][DIG_OUTPUT], digOutPin[i][DIG_OUTPUT], digOutDir[i][DIG_OUTPUT]);
        GPIO_setDirMode(digOutBaseAddr[i][DIG_OUT_STATUS], digOutPin[i][DIG_OUT_STATUS], digOutDir[i][DIG_OUT_STATUS]);
    }

    while(1)
    {
        pushButton = GPIO_pinRead((uint32_t)AddrTranslateP_getLocalAddr(USER_BUTTON_BASE_ADDR), USER_BUTTON_PIN);

        if (pushButtonOld != pushButton)
        {
            pushButtonOld = pushButton;
            DebugP_log("[GPIO] User Button state: %u\r\n", pushButton);
        }

        for (i = 0; i < DIG_IN_MAX; i++)
        {
            digIn[i] = GPIO_pinRead(digInBaseAddr[i], digInPin[i]);
            if (digInOld[i] != digIn[i])
            {
                digInOld[i] = digIn[i];
                DebugP_log("[GPIO] dig in %u state: %u\r\n", i+1, digIn[i]);
            }
        }

        for (i = 0; i < OUT_MAX; i++)
        {
            digOut[i].status = GPIO_pinRead(digOutBaseAddr[i][DIG_OUT_STATUS], digOutPin[i][DIG_OUT_STATUS]);
            if (digOutStatusOld[i] != digOut[i].status)
            {
                digOutStatusOld[i] = digOut[i].status;
                DebugP_log("[GPIO] dig out status %u state: %u\r\n", i+1, digOut[i].status);
            }
        }

        vTaskDelay(TASK_DELAY);
    }
}

/*[EOF]************************************************************************/
