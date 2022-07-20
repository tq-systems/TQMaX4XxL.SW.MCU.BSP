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
#include "stdlib.h"
#include <stdio.h>
/* project */
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/AddrTranslateP.h>
//#include <kernel/dpl/HwiP.h>
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

#define DECIMAL_BASE               (10U)
#define TASK_DELAY  pdMS_TO_TICKS  (10U)
#define MAX_INPUT_PARAM             (3U)

/*******************************************************************************
 * local macros
 ******************************************************************************/

enum
{
    PARAM_READ_WRITE = 0,
    PARAM_OUT_NUM,
    PARAM_STATUS
};

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

/* The definition of the "digial GPIO" command. */
const CLI_Command_Definition_t gpioDigCommandDef =
{
    "gpio",
    "\r\ngpio [GPIO]:\r\n Read all dig in or set the dig out.\n"
    " Usage gpio [access] [output num] [state]:\n"
    " access:\n"
    "  r - read all dig in\n"
    "  w - write a dig out\n"
    " output num: (only used in write option)\n"
    "  1 - 4 dig out\n"
    " state:\n"
    "  0 - low, 1 - high\r\n\r\n",
    gpioDigCommand,
    -1
};



/*******************************************************************************
 * local typedefs
 ******************************************************************************/

typedef struct
{
    bool status;
    uint32_t baseAdd;
    uint32_t pin;
    uint32_t direction;
} digGpio_t;

/*******************************************************************************
 * local static data
 ******************************************************************************/

static digGpio_t userButton            = {0};
static digGpio_t digIn[DIG_IN_MAX]     = {0};
static digGpio_t digOut[OUT_MAX]       = {0};
static digGpio_t digOutStatus[OUT_MAX] = {0};

/*******************************************************************************
 * forward declarations
 ******************************************************************************/

static void gpioInit(void);
static bool gpioSetStatus(const digGpio_t* const pGpio, const bool status);

/*******************************************************************************
 * local static functions
 ******************************************************************************/

/**
 * @brief This function set the GPIO output to high or low.
 *
 * @param pGpio Pointer to the structure of the GPIO to be set
 * @param status Status of the GPIO, false = low, true = high
 * @return success
 */
static bool gpioSetStatus(const digGpio_t* const pGpio, const bool status)
{
    bool success = false;

    if ((pGpio != NULL) && (pGpio->baseAdd != 0) && (pGpio->pin != 0))
    {
        if (status == true)
        {
            GPIO_pinWriteHigh(pGpio->baseAdd, pGpio->pin);
            success = true;
        }
        else
        {
            GPIO_pinWriteLow(pGpio->baseAdd, pGpio->pin);
            success = true;
        }
    }

    return success;
}

/**
 * @brief This function initializes all digital GPIOs.
 *
 */
static void gpioInit(void)
{
    uint8_t  counter = 0;

    userButton.baseAdd            = (uint32_t)AddrTranslateP_getLocalAddr(USER_BUTTON_BASE_ADDR);
    userButton.pin                = USER_BUTTON_PIN;
    userButton.direction          = USER_BUTTON_DIR;

    digIn[DIG_IN_1].baseAdd       = (uint32_t)AddrTranslateP_getLocalAddr(DIG_IN_1_BASE_ADDR);
    digIn[DIG_IN_1].pin           = DIG_IN_1_PIN;
    digIn[DIG_IN_1].direction     = DIG_IN_1_DIR;

    digIn[DIG_IN_2].baseAdd       = (uint32_t)AddrTranslateP_getLocalAddr(DIG_IN_2_BASE_ADDR);
    digIn[DIG_IN_2].pin           = DIG_IN_2_PIN;
    digIn[DIG_IN_2].direction     = DIG_IN_2_DIR;

    digIn[DIG_IN_3].baseAdd       = (uint32_t)AddrTranslateP_getLocalAddr(DIG_IN_3_BASE_ADDR);
    digIn[DIG_IN_3].pin           = DIG_IN_3_PIN;
    digIn[DIG_IN_3].direction     = DIG_IN_3_DIR;

    digIn[DIG_IN_4].baseAdd       = (uint32_t)AddrTranslateP_getLocalAddr(DIG_IN_4_BASE_ADDR);
    digIn[DIG_IN_4].pin           = DIG_IN_4_PIN;
    digIn[DIG_IN_4].direction     = DIG_IN_4_DIR;

    digOut[OUT_1].baseAdd         = (uint32_t)AddrTranslateP_getLocalAddr(EN_DIG_OUT_1_BASE_ADDR);
    digOut[OUT_1].pin             = EN_DIG_OUT_1_PIN;
    digOut[OUT_1].direction       = EN_DIG_OUT_1_DIR;

    digOutStatus[OUT_1].baseAdd   = (uint32_t)AddrTranslateP_getLocalAddr(STATUS_OUT_1_BASE_ADDR);
    digOutStatus[OUT_1].pin       = STATUS_OUT_1_PIN;
    digOutStatus[OUT_1].direction = STATUS_OUT_1_DIR;

    digOut[OUT_2].baseAdd         = (uint32_t)AddrTranslateP_getLocalAddr(EN_DIG_OUT_2_BASE_ADDR);
    digOut[OUT_2].pin             = EN_DIG_OUT_2_PIN;
    digOut[OUT_2].direction       = EN_DIG_OUT_2_DIR;

    digOutStatus[OUT_2].baseAdd   = (uint32_t)AddrTranslateP_getLocalAddr(STATUS_OUT_2_BASE_ADDR);
    digOutStatus[OUT_2].pin       = STATUS_OUT_2_PIN;
    digOutStatus[OUT_2].direction = STATUS_OUT_2_DIR;

    digOut[OUT_3].baseAdd         = (uint32_t)AddrTranslateP_getLocalAddr(EN_DIG_OUT_3_BASE_ADDR);
    digOut[OUT_3].pin             = EN_DIG_OUT_3_PIN;
    digOut[OUT_3].direction       = EN_DIG_OUT_3_DIR;

    digOutStatus[OUT_3].baseAdd   = (uint32_t)AddrTranslateP_getLocalAddr(STATUS_OUT_3_BASE_ADDR);
    digOutStatus[OUT_3].pin       = STATUS_OUT_3_PIN;
    digOutStatus[OUT_3].direction = STATUS_OUT_3_DIR;

    digOut[OUT_4].baseAdd         = (uint32_t)AddrTranslateP_getLocalAddr(EN_DIG_OUT_4_BASE_ADDR);
    digOut[OUT_4].pin             = EN_DIG_OUT_4_PIN;
    digOut[OUT_4].direction       = EN_DIG_OUT_4_DIR;

    digOutStatus[OUT_4].baseAdd   = (uint32_t)AddrTranslateP_getLocalAddr(STATUS_OUT_4_BASE_ADDR);
    digOutStatus[OUT_4].pin       = STATUS_OUT_4_PIN;
    digOutStatus[OUT_4].direction = STATUS_OUT_4_DIR;

    /* init GPIOs */
    GPIO_setDirMode(userButton.baseAdd, userButton.pin, userButton.direction);

    for (counter = 0; counter < DIG_IN_MAX; counter++)
    {
        GPIO_setDirMode(digIn[counter].baseAdd, digIn[counter].pin, digIn[counter].direction);
    }

    for (counter = 0; counter < OUT_MAX; counter++)
    {
        GPIO_setDirMode(digOut[counter].baseAdd,       digOut[counter].pin,       digOut[counter].direction);
        GPIO_setDirMode(digOutStatus[counter].baseAdd, digOutStatus[counter].pin, digOutStatus[counter].direction);
    }
}

/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief This function handles the digital GPIO command.
 *
 * @param pcWriteBuffer cli output string buffer
 * @param xWriteBufferLen length of the cli output string
 * @param pcCommandString cli command input string
 * @return pdFALSE = command is finished
 */
BaseType_t gpioDigCommand(char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString)
{
    char*      pcParameter[MAX_INPUT_PARAM] = { NULL };
    char*      pNextNumber                  = NULL;
    BaseType_t xParameterStringLength       = 5;
    uint8_t    outputNumber                 = 0;
    uint8_t    status                       = 0;
    uint8_t    paramCount                   = 0;

    for (paramCount = 0; (paramCount < MAX_INPUT_PARAM) && (xParameterStringLength != 0); paramCount++)
    {
        pcParameter[paramCount] = (char*) FreeRTOS_CLIGetParameter(pcCommandString, paramCount + 1, &xParameterStringLength);
    }

    if ((*pcParameter[PARAM_READ_WRITE] == 'w') && (pcParameter[PARAM_OUT_NUM] != NULL) && (pcParameter[PARAM_STATUS] != NULL))
    {
        outputNumber = strtol(pcParameter[PARAM_OUT_NUM], &pNextNumber, DECIMAL_BASE);
        status       = strtol(pcParameter[PARAM_STATUS],  &pNextNumber, DECIMAL_BASE);

        if ((outputNumber > OUT_1) && (outputNumber <= OUT_4) && ((status == 0) || (status == 1)))
        {
            if (gpioSetStatus(&digOut[outputNumber - 1], status) == true)
            {
                sprintf(pcWriteBuffer, "[GPIO] Set output successfully.\r\n");
            }
            else
            {
                sprintf(pcWriteBuffer, "[GPIO] set GPIO failure.\r\n");
            }
        }
        else
        {
            sprintf(pcWriteBuffer, "[GPIO] Wrong parameter input.\r\n");
        }
    }

    else if (*pcParameter[PARAM_READ_WRITE] == 'r')
    {
        sprintf(pcWriteBuffer, " User Button = %u\n"
                               " GPIO_IN_1   = %u\n"
                               " GPIO_IN_2   = %u\n"
                               " GPIO_IN_3   = %u\n"
                               " GPIO_IN_4   = %u\n"
                               " GPIO_OUT_1  = %u\n"
                               " GPIO_OUT_2  = %u\n"
                               " GPIO_OUT_3  = %u\n"
                               " GPIO_OUT_4  = %u\r\n", userButton.status,
                                                        digIn[DIG_IN_1].status,     digIn[DIG_IN_2].status,
                                                        digIn[DIG_IN_3].status,     digIn[DIG_IN_4].status,
                                                        digOutStatus[OUT_1].status, digOutStatus[OUT_2].status,
                                                        digOutStatus[OUT_3].status, digOutStatus[OUT_4].status);
    }
    else
    {
        sprintf(pcWriteBuffer, "[GPIO] Wrong parameter input.\r\n");
    }

    return pdFALSE;
}

/**
 * @brief Task for GPIO polling.
 *
 * @param pvParameters unused
 */
void gpioPollingTask(void *pvParameters)
{

    uint8_t  counter                   = 0;
    bool     pushButtonOld             = false;
    bool     digInOld[DIG_IN_MAX]      = {false};
    bool     digOutStatusOld[OUT_MAX]  = {false};

    gpioInit();

    /* Task loop */
    while(1)
    {
        userButton.status = GPIO_pinRead(userButton.baseAdd, userButton.pin);
        if (pushButtonOld != userButton.status)
        {
            pushButtonOld = userButton.status;
            DebugP_log("[GPIO] User Button state: %u\r\n", !userButton.status);
        }

        for (counter = 0; counter < DIG_IN_MAX; counter++)
        {
            digIn[counter].status = GPIO_pinRead(digIn[counter].baseAdd, digIn[counter].pin);
            if (digInOld[counter] != digIn[counter].status)
            {
                digInOld[counter] = digIn[counter].status;
                DebugP_log("[GPIO] dig in %u state: %u\r\n", counter+1, !digIn[counter].status);
            }
        }

        for (counter = 0; counter < OUT_MAX; counter++)
        {
            digOutStatus[counter].status = GPIO_pinRead(digOutStatus[counter].baseAdd, digOutStatus[counter].pin);
            if (digOutStatusOld[counter] != digOutStatus[counter].status)
            {
                digOutStatusOld[counter] = digOutStatus[counter].status;
                DebugP_log("[GPIO] dig out status %u state: %u\r\n", counter+1, !digOutStatus[counter].status);
            }
        }

        vTaskDelay(TASK_DELAY);
    }
}

/*[EOF]************************************************************************/
