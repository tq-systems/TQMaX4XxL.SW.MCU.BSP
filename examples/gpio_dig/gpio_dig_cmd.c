/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file gpio_dig_cmd.c
 * @copyright Copyright (c) 2022 TQ-y GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2022-07-15
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the digital gpio implementation.
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

/* The definition of the "digital GPIO" command. */
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
    pinStatus_t pinStatus;
    uint32_t    baseAdd;
    uint32_t    pin;
    uint32_t    direction;
} digGpio_t;

/*******************************************************************************
 * local static data
 ******************************************************************************/

static digGpio_t userButton                = {0};
static digGpio_t digIn[DIG_IN_MAX]         = {0};
static digGpio_t digOut[DIG_OUT_MAX]       = {0};
static digGpio_t digOutStatus[DIG_OUT_MAX] = {0};
static bool      gpioIsInit                = false;

/*******************************************************************************
 * forward declarations
 ******************************************************************************/

static bool gpioSetStatus(const digGpio_t* const pGpio, const pinStatus_t pinStatus);

/*******************************************************************************
 * local static functions
 ******************************************************************************/

/**
 * @brief This function sets the GPIO output to high or low.
 *
 * @param pGpio Pointer to the structure of the GPIO to be set
 * @param pinStatus Status of the GPIO, false = low, true = high
 * @return success
 */
static bool gpioSetStatus(const digGpio_t* const pGpio, const pinStatus_t pinStatus)
{
    bool success = false;

    if ((pGpio != NULL) && (pGpio->baseAdd != 0) && (pGpio->pin != 0))
    {
        if (pinStatus == PIN_STATUS_HIGH)
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

/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief This function sets the GPIO output to high or low.
 *
 * @param gpio GPIO to be set
 * @param pinStatus Status of the GPIO, false = low, true = high
 * @return success
 */
bool gpio_writePinOut(GpioOut_t gpio, pinStatus_t pinStatus)
{
    bool retVal = false;

    if ((gpio < DIG_OUT_MAX) && (pinStatus <= PIN_STATUS_HIGH) && (gpioIsInit == true))
    {
        retVal = gpioSetStatus(&digOut[gpio], pinStatus);
    }

    return retVal;
}

/**
 * @brief This function initializes all digital GPIOs.
 *
 */
void gpioInit(void)
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

    digOut[DIG_OUT_1].baseAdd         = (uint32_t)AddrTranslateP_getLocalAddr(EN_DIG_OUT_1_BASE_ADDR);
    digOut[DIG_OUT_1].pin             = EN_DIG_OUT_1_PIN;
    digOut[DIG_OUT_1].direction       = EN_DIG_OUT_1_DIR;

    digOutStatus[DIG_OUT_1].baseAdd   = (uint32_t)AddrTranslateP_getLocalAddr(STATUS_OUT_1_BASE_ADDR);
    digOutStatus[DIG_OUT_1].pin       = STATUS_OUT_1_PIN;
    digOutStatus[DIG_OUT_1].direction = STATUS_OUT_1_DIR;

    digOut[DIG_OUT_2].baseAdd         = (uint32_t)AddrTranslateP_getLocalAddr(EN_DIG_OUT_2_BASE_ADDR);
    digOut[DIG_OUT_2].pin             = EN_DIG_OUT_2_PIN;
    digOut[DIG_OUT_2].direction       = EN_DIG_OUT_2_DIR;

    digOutStatus[DIG_OUT_2].baseAdd   = (uint32_t)AddrTranslateP_getLocalAddr(STATUS_OUT_2_BASE_ADDR);
    digOutStatus[DIG_OUT_2].pin       = STATUS_OUT_2_PIN;
    digOutStatus[DIG_OUT_2].direction = STATUS_OUT_2_DIR;

    digOut[DIG_OUT_3].baseAdd         = (uint32_t)AddrTranslateP_getLocalAddr(EN_DIG_OUT_3_BASE_ADDR);
    digOut[DIG_OUT_3].pin             = EN_DIG_OUT_3_PIN;
    digOut[DIG_OUT_3].direction       = EN_DIG_OUT_3_DIR;

    digOutStatus[DIG_OUT_3].baseAdd   = (uint32_t)AddrTranslateP_getLocalAddr(STATUS_OUT_3_BASE_ADDR);
    digOutStatus[DIG_OUT_3].pin       = STATUS_OUT_3_PIN;
    digOutStatus[DIG_OUT_3].direction = STATUS_OUT_3_DIR;

    digOut[DIG_OUT_4].baseAdd         = (uint32_t)AddrTranslateP_getLocalAddr(EN_DIG_OUT_4_BASE_ADDR);
    digOut[DIG_OUT_4].pin             = EN_DIG_OUT_4_PIN;
    digOut[DIG_OUT_4].direction       = EN_DIG_OUT_4_DIR;

    digOutStatus[DIG_OUT_4].baseAdd   = (uint32_t)AddrTranslateP_getLocalAddr(STATUS_OUT_4_BASE_ADDR);
    digOutStatus[DIG_OUT_4].pin       = STATUS_OUT_4_PIN;
    digOutStatus[DIG_OUT_4].direction = STATUS_OUT_4_DIR;

    digOut[ADC_RST].baseAdd         = (uint32_t)AddrTranslateP_getLocalAddr(ADC_RST_BASE_ADDR);
    digOut[ADC_RST].pin             = ADC_RST_PIN;
    digOut[ADC_RST].direction       = ADC_RST_DIR;

    digOutStatus[ADC_RST].baseAdd   = (uint32_t)AddrTranslateP_getLocalAddr(ADC_RST_BASE_ADDR);
    digOutStatus[ADC_RST].pin       = ADC_RST_PIN;
    digOutStatus[ADC_RST].direction = ADC_RST_DIR;

    /* init GPIOs */
    GPIO_setDirMode(userButton.baseAdd, userButton.pin, userButton.direction);

    for (counter = 0; counter < DIG_IN_MAX; counter++)
    {
        GPIO_setDirMode(digIn[counter].baseAdd, digIn[counter].pin, digIn[counter].direction);
    }

    for (counter = 0; counter < DIG_OUT_MAX; counter++)
    {
        GPIO_setDirMode(digOut[counter].baseAdd,       digOut[counter].pin,       digOut[counter].direction);
        GPIO_setDirMode(digOutStatus[counter].baseAdd, digOutStatus[counter].pin, digOutStatus[counter].direction);
    }

    gpioIsInit = true;
}

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
    char*       pcParameter[MAX_INPUT_PARAM] = { NULL };
    char*       pNextNumber                  = NULL;
    BaseType_t  xParameterStringLength       = 5;
    uint8_t     outputNumber                 = 0;
    pinStatus_t pinStatus                    = PIN_STATUS_LOW;
    uint8_t     paramCount                   = 0;

    for (paramCount = 0; ((paramCount < MAX_INPUT_PARAM) && (xParameterStringLength != 0)); paramCount++)
    {
        pcParameter[paramCount] = (char*) FreeRTOS_CLIGetParameter(pcCommandString, paramCount + 1, &xParameterStringLength);
    }

    if ((*pcParameter[PARAM_READ_WRITE] == 'w') && (pcParameter[PARAM_OUT_NUM] != NULL) && (pcParameter[PARAM_STATUS] != NULL))
    {
        outputNumber = strtol(pcParameter[PARAM_OUT_NUM], &pNextNumber, DECIMAL_BASE);
        pinStatus    = strtol(pcParameter[PARAM_STATUS],  &pNextNumber, DECIMAL_BASE);

        /* enum DIG_OUT_1 - DIG_OUT_4 is defined as 0 - 3, but input parameter are defined as 1 - 4 because the same definition is in electric diagram. */
        if ((outputNumber > DIG_OUT_1) && (outputNumber <= DIG_OUT_4) && ((pinStatus == PIN_STATUS_LOW) || (pinStatus == PIN_STATUS_HIGH)))
        {
            if (gpioSetStatus(&digOut[outputNumber - 1], pinStatus) == true)
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
                               " GPIO_OUT_4  = %u\r\n",
                               userButton.pinStatus,
                               digIn[DIG_IN_1].pinStatus,
                               digIn[DIG_IN_2].pinStatus,
                               digIn[DIG_IN_3].pinStatus,
                               digIn[DIG_IN_4].pinStatus,
                               digOutStatus[DIG_OUT_1].pinStatus,
                               digOutStatus[DIG_OUT_2].pinStatus,
                               digOutStatus[DIG_OUT_3].pinStatus,
                               digOutStatus[DIG_OUT_4].pinStatus);
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
    uint8_t  counter                       = 0;
    bool     pushButtonOld                 = false;
    bool     digInOld[DIG_IN_MAX]          = {false};
    bool     digOutStatusOld[DIG_OUT_MAX]  = {false};

    gpioInit();

    /* Task loop */
    while(1)
    {
        userButton.pinStatus = GPIO_pinRead(userButton.baseAdd, userButton.pin);
        if (pushButtonOld != userButton.pinStatus)
        {
            pushButtonOld = userButton.pinStatus;
            DebugP_log("[GPIO] User Button state: %u\r\n", !userButton.pinStatus);
        }

        for (counter = 0; counter < DIG_IN_MAX; counter++)
        {
            digIn[counter].pinStatus = GPIO_pinRead(digIn[counter].baseAdd, digIn[counter].pin);
            if (digInOld[counter] != digIn[counter].pinStatus)
            {
                digInOld[counter] = digIn[counter].pinStatus;
                DebugP_log("[GPIO] dig in %u state: %u\r\n", counter+1, !digIn[counter].pinStatus);
            }
        }

        for (counter = 0; counter < DIG_OUT_MAX; counter++)
        {
            digOutStatus[counter].pinStatus = GPIO_pinRead(digOutStatus[counter].baseAdd, digOutStatus[counter].pin);
            if (digOutStatusOld[counter] != digOutStatus[counter].pinStatus)
            {
                digOutStatusOld[counter] = digOutStatus[counter].pinStatus;
                DebugP_log("[GPIO] dig out pinStatus %u state: %u\r\n", counter+1, !digOutStatus[counter].pinStatus);
            }
        }

        vTaskDelay(TASK_DELAY);
    }
}

/*[EOF]************************************************************************/
