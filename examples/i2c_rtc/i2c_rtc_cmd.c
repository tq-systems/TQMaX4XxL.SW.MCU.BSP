/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file i2c_rtc_cmd.c
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2022-05-13
 *
 * -----------------------------------------------------------------------------
 * @brief This file includes the i2c rtc implementation.
 *
 */

/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* project */
#include <drivers/i2c.h>
#include <kernel/dpl/DebugP.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
/* own */
#include "i2c_rtc_cmd.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/

#define I2C_READ_SLAVE_ADDR  (0x51U)

#define MAX_BUFFER_SIZE      (7U)
#define I2C_READ_LEN         (1U)
#define I2C_READ_BIT         (1U)
#define I2C_WRITE_BIT        (0U)
#define DECIMAL_BASE         (10U)

#define MAX_INPUT_PARAM      (5U)
#define PARAM_INPUT_ACCESS   (0U)
#define PARAM_INPUT_OPTION   (1U)
#define PARAM_INPUT_HOUR     (2U)
#define PARAM_INPUT_MIN      (3U)
#define PARAM_INPUT_SEC      (4U)
#define PARAM_INPUT_DAY      (2U)
#define PARAM_INPUT_MONTH    (3U)
#define PARAM_INPUT_YEAR     (4U)

#define RTC_REG_SECOND       (0x04)
#define RTC_REG_DAY          (0x07)

/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "i2crtc" command. */
const CLI_Command_Definition_t i2cRtcCommandDef =
{
    "i2crtc",
    "\r\ni2crtc:\r\n Read or set the RTC via I2C. \r\n Usage: i2crtc [access] [option] [val1] [val2] [val3]. \r\n For more information use -h\r\n\r\n",
    i2cRtcCommand,
    -1
};

/*******************************************************************************
 * local typedefs
 ******************************************************************************/

typedef struct
{
    uint8_t sec;
    uint8_t min;
    uint8_t h;
    uint8_t day;
    uint8_t month;
    uint8_t year;
} rtcTime_t;

typedef enum
{
    TIME,
    DATE
} rtcOption_t;

/*******************************************************************************
 * local static data
 ******************************************************************************/



/*******************************************************************************
 * forward declarations
 ******************************************************************************/

static bool i2cGetRtcTime(rtcTime_t* p_time);
static bool i2cWriteRtcTime(rtcTime_t* time, rtcOption_t option);
static uint8_t decToBcd(uint8_t decVal);
static bool inputParamToSettingParam(rtcTime_t* p_time, rtcOption_t* p_rtcOption, const char *pcCommandString);

/*******************************************************************************
 * local static functions
 ******************************************************************************/

/**
 * @brief This function converts a decimal integer number to a bcd code.
 *
 * @param decVal decimal number to be convert
 * @return bcd cpde
 */
static uint8_t decToBcd(uint8_t decVal)
{
    uint8_t bcdResult = 0;
    uint8_t shift = 0;

    while (decVal > 0)
    {
        bcdResult |= (decVal % 10) << (shift++ << 2);
        decVal /= 10;
    }

    return bcdResult;
}

/**
 * @brief This function converts the input parameter to time or date and returns the setting parameter.
 *
 * @param p_time pointer to rtcTime struct
 * @param rtcOption pointer to rtc option parameter
 * @param pcCommandString cli command input string
 * @return true = success, false = failure
 */
static bool inputParamToSettingParam(rtcTime_t* p_time, rtcOption_t* p_rtcOption, const char *pcCommandString )
{
    bool success                        = true;
    uint8_t paramCount                  = 0;
    BaseType_t xParameterStringLength   = 0;
    char* pNextNumber                   = NULL;
    char* pcParameter[MAX_INPUT_PARAM]  = {NULL};

    /* get all parameter */
    for (paramCount = 0; paramCount < MAX_INPUT_PARAM; paramCount++)
    {
        pcParameter[paramCount] = (char*) FreeRTOS_CLIGetParameter(pcCommandString, paramCount+1, &xParameterStringLength);
    }

    /* set data */
    if (*pcParameter[PARAM_INPUT_OPTION] == 't')
    {
        *p_rtcOption = TIME;

        p_time->h = strtoul(pcParameter[PARAM_INPUT_HOUR], &pNextNumber, DECIMAL_BASE);
        if ((p_time->h > 23) || (pcParameter[PARAM_INPUT_HOUR] == pNextNumber))
        {
            success = false;
        }

        p_time->min = strtoul(pcParameter[PARAM_INPUT_MIN], &pNextNumber, DECIMAL_BASE);
        if ((p_time->min > 59) || (pcParameter[PARAM_INPUT_MIN] == pNextNumber))
        {
            success = false;
        }

        p_time->sec = strtoul(pcParameter[PARAM_INPUT_SEC], &pNextNumber, DECIMAL_BASE);
        if ((p_time->sec > 59) || (pcParameter[PARAM_INPUT_SEC] == pNextNumber))
        {
            success = false;
        }
    }
    else if (*pcParameter[PARAM_INPUT_OPTION] == 'd')
    {
        *p_rtcOption = DATE;
        p_time->day = strtoul(pcParameter[PARAM_INPUT_DAY], &pNextNumber, DECIMAL_BASE);
        if ((p_time->day < 1) || (p_time->day > 31) || (pcParameter[PARAM_INPUT_DAY] == pNextNumber))
        {
            success = false;
        }

        p_time->month = strtoul(pcParameter[PARAM_INPUT_MONTH], &pNextNumber, DECIMAL_BASE);
        if ((p_time->month < 1) || (p_time->month > 12) || (pcParameter[PARAM_INPUT_MONTH] == pNextNumber))
        {
            success = false;
        }

        p_time->year = strtoul(pcParameter[PARAM_INPUT_YEAR], &pNextNumber, DECIMAL_BASE);
        if ((p_time->year > 99) || (pcParameter[PARAM_INPUT_YEAR] == pNextNumber))
        {
            success = false;
        }
    }
    else
    {
        success = false;
    }

    return success;
}

/**
 * @brief This function gets the current RTC time and date.
 *
 * @param p_time pointer to rtcTime struct
 * @return true = success, false = failure
 */
static bool i2cGetRtcTime(rtcTime_t* p_time)
{
    bool            retVal                     = false;
    uint8_t         rxBuffer[MAX_BUFFER_SIZE]  = {0};
    uint8_t         txBuffer[1]                = {0};
    int32_t         status                     = 0;
    I2C_Handle      i2cHandle                  = NULL;
    I2C_Transaction i2cTransaction             = {0};

    Drivers_i2cOpen();
    Board_driversOpen();
    i2cHandle = I2C_getHandle(CONFIG_I2C0);

//    i2cHandle = gI2cHandle[CONFIG_I2C0];

    DebugP_log("[I2C] Read RTC data.\r\n");

    /* Set default transaction parameters */
    I2C_Transaction_init(&i2cTransaction);

    /* Override with required transaction parameters */
    i2cTransaction.writeBuf     = txBuffer;
    i2cTransaction.writeCount   = 1;
    i2cTransaction.readBuf      = rxBuffer;
    i2cTransaction.readCount    = I2C_READ_LEN;
    i2cTransaction.slaveAddress = I2C_READ_SLAVE_ADDR;
    txBuffer[0] =  RTC_REG_SECOND;

    status = I2C_transfer(i2cHandle, &i2cTransaction);

    if(status == I2C_STS_SUCCESS)
    {
        /* read the results */
        i2cTransaction.readCount = sizeof(rxBuffer);

        /* clear RX buffer every p_time we read, to make sure it does not have stale data */
        memset(rxBuffer, 0, sizeof(rxBuffer));

        status = I2C_transfer(i2cHandle, &i2cTransaction);

        if (status == I2C_STS_SUCCESS)
        {
            p_time->sec   = ((((rxBuffer[0] & 0x7F) >> 4) * 10) + (rxBuffer[0] & 0x0F));
            p_time->min   = ((((rxBuffer[1] & 0x7F) >> 4) * 10) + (rxBuffer[1] & 0x0F));
            p_time->h     = ((((rxBuffer[2] & 0x3F) >> 4) * 10) + (rxBuffer[2] & 0x0F));
            p_time->day   = ((((rxBuffer[3] & 0x3F) >> 4) * 10) + (rxBuffer[3] & 0x0F));
            p_time->month = ((((rxBuffer[5] & 0x3F) >> 4) * 10) + (rxBuffer[5] & 0x0F));
            p_time->year  = ((((rxBuffer[6] & 0x3F) >> 4) * 10) + (rxBuffer[6] & 0x0F));
            retVal        = true;
        }
    }

    Board_driversClose();
    Drivers_i2cClose();

    return retVal;
}

/**
 * @brief This function writes the time or date to the RTC.
 *
 * @param p_time pointer to rtcTimer struct
 * @param option Option whether the date or the time should be set.
 * @return true = success, false = failure
 */
static bool i2cWriteRtcTime(rtcTime_t* p_time, rtcOption_t option)
{
    const uint8_t WRITE_TIME_WRITE_CONT = 4;
    const uint8_t WRITE_DATE_WRITE_CONT = 5;

    bool retVal                    = false;
    uint8_t         txBuffer[5]    = {0};
    uint8_t         rxBuffer[1]    = {0};
    int32_t         status         = I2C_STS_SUCCESS;
    I2C_Handle      i2cHandle      = NULL;
    I2C_Transaction i2cTransaction = {0};

    /* Set default transaction parameters */
    I2C_Transaction_init(&i2cTransaction);

    /* Override with required transaction parameters */
    i2cTransaction.writeBuf     = txBuffer;
    i2cTransaction.readBuf      = rxBuffer;
    i2cTransaction.writeCount   = 1;
    i2cTransaction.readCount    = I2C_READ_LEN;
    i2cTransaction.slaveAddress = I2C_READ_SLAVE_ADDR;

    Drivers_i2cOpen();
    Board_driversOpen();

    i2cHandle = gI2cHandle[CONFIG_I2C0];

    DebugP_log("[I2C] Write RTC data.\r\n");

    if (option == TIME)
    {
        i2cTransaction.writeCount = WRITE_TIME_WRITE_CONT;
        txBuffer[0]               = RTC_REG_SECOND;
        txBuffer[1]               = decToBcd(p_time->sec);
        txBuffer[2]               = decToBcd(p_time->min);
        txBuffer[3]               = decToBcd(p_time->h);
    }
    else if (option == DATE)
    {
        i2cTransaction.writeCount = WRITE_DATE_WRITE_CONT;
        txBuffer[0]               = RTC_REG_DAY;
        txBuffer[1]               = decToBcd(p_time->day);
        txBuffer[3]               = decToBcd(p_time->month);
        txBuffer[4]               = decToBcd(p_time->year);
    }

    status = I2C_transfer(i2cHandle, &i2cTransaction);
    if(status != I2C_STS_SUCCESS)
    {
        DebugP_log("[I2C] RTC write error %d.\r\n", status);
    }
    else
    {
        retVal = true;
    }

    Board_driversClose();
    Drivers_i2cClose();

    return retVal;
}

/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief This function handles the i2c RTC command.
 *
 * @param pcWriteBuffer cli output string buffer
 * @param xWriteBufferLen length of the cli output string
 * @param pcCommandString cli command input string
 * @return pdFALSE = command is finished
 */
BaseType_t i2cRtcCommand( char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString )
{
    rtcTime_t time = {0};
    rtcOption_t rtcOption = TIME;
    BaseType_t xParameterStringLength = 0;

    const char* pcParameter1 = FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameterStringLength);

    if (strncmp(pcParameter1, "-h", 2) == 0)
    {
        sprintf(pcWriteBuffer,
        " Usage: i2crtc [access] [option] [val1] [val2] [val3]\r\n"
        " access: r = read rtc, w = write\r\n"
        " option: t = set time, d = set date\r\n"
        " val1:   hour [0 - 23] or day [1 - 31]\r\n"
        " val2:   minute [0 - 59] or month [ 1 - 12]\r\n"
        " val3:   second [0 - 59] or year [0 - 99]\r\n\r\n"
        );
    }
    else if (*pcParameter1 == 'w')
    {
        if (inputParamToSettingParam(&time, &rtcOption, pcCommandString) == true)
        {
            if(i2cWriteRtcTime(&time, rtcOption) == true)
            {
                sprintf(pcWriteBuffer, "RTC successfully set \r\n");
            }
            else
            {
                sprintf(pcWriteBuffer, "RTC write failure \r\n");
            }
        }
        else
        {
            sprintf(pcWriteBuffer, "Incorrect setting parameter\r\n");
        }
    }
    else if (*pcParameter1 == 'r')
    {
        if (i2cGetRtcTime(&time) == true)
        {
            sprintf(pcWriteBuffer, "Date: %02u.%.02u.%.02u Time: %02u:%02u:%02u\r\n", time.day, time.month, time.year, time.h, time.min, time.sec);
        }
        else
        {
            sprintf(pcWriteBuffer, "RTC read failure \r\n");
        }
    }
    else
    {
        sprintf(pcWriteBuffer, "Incorrect command parameter(s).  Enter \"help\" to view a list of available commands.\r\n");
    }

    return pdFALSE;
}

/*[EOF]************************************************************************/
