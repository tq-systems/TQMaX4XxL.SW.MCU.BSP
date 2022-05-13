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

#define I2C_READ_SLAVE_ADDR             (0x51U)
#define I2C_READ_LEN                    (1U)
#define I2C_READ_BIT                    (1U)
#define I2C_WRITE_BIT                   (0U)

/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "i2ctemp" command. */
const CLI_Command_Definition_t i2cRtcCommandDef =
{
    "i2crtc",
    "\r\ni2crtc:\r\n Read the RTC via I2C.\r\n\r\n",
    i2cRtcCommand,
    0
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
} Time_t;

/*******************************************************************************
 * local static data
 ******************************************************************************/



/*******************************************************************************
 * forward declarations
 ******************************************************************************/

static void i2cGetRtcTime(Time_t* time);

/*******************************************************************************
 * local static functions
 ******************************************************************************/

static void i2cGetRtcTime(Time_t* time)
{
    uint8_t         txBuffer[1]    = {0};
    uint8_t         rxBuffer[7]    = {0};
    int32_t         status         = 0;
    I2C_Handle      i2cHandle      = NULL;
    I2C_Transaction i2cTransaction = {0};

    Drivers_i2cOpen();
    Board_driversOpen();

    i2cHandle = gI2cHandle[CONFIG_I2C0];

    DebugP_log("[I2C] Read data ... !!!\r\n");

    /* Set default transaction parameters */
    I2C_Transaction_init(&i2cTransaction);

    /* Override with required transaction parameters */
    i2cTransaction.writeBuf     = txBuffer;
    i2cTransaction.writeCount   = 1;
    i2cTransaction.readBuf      = rxBuffer;
    i2cTransaction.readCount    = I2C_READ_LEN;
    i2cTransaction.slaveAddress = I2C_READ_SLAVE_ADDR;
    txBuffer[0] =  0x04;

    status = I2C_transfer(i2cHandle, &i2cTransaction);

    if(status == I2C_STS_SUCCESS)
    {
        /* read the results */
        I2C_Transaction_init(&i2cTransaction);
        i2cTransaction.readBuf = rxBuffer;
        i2cTransaction.readCount = 7;
        i2cTransaction.slaveAddress = I2C_READ_SLAVE_ADDR;

        /* clear RX buffer every time we read, to make sure it does not have stale data */
        rxBuffer[0] = rxBuffer[1] = 0;
        status = I2C_transfer(i2cHandle, &i2cTransaction);

        if (status == I2C_STS_SUCCESS)
        {
            time->sec   = ((((rxBuffer[0] & 0x7F) >> 4) * 10) + (rxBuffer[0] & 0x0F));
            time->min   = ((((rxBuffer[1] & 0x7F) >> 4) * 10) + (rxBuffer[1] & 0x0F));
            time->h     = ((((rxBuffer[2] & 0x3F) >> 4) * 10) + (rxBuffer[2] & 0x0F));
            time->day   = ((((rxBuffer[4] & 0x3F) >> 4) * 10) + (rxBuffer[4] & 0x0F));
            time->month = ((((rxBuffer[5] & 0x3F) >> 4) * 10) + (rxBuffer[5] & 0x0F));
            time->year  = ((((rxBuffer[6] & 0x3F) >> 4) * 10) + (rxBuffer[6] & 0x0F));
            DebugP_log("[I2C] %02u.%.02u.%.02u,  %02u:%02u:%02u\r\n", time->day, time->month, time->year, time->h, time->min, time->sec);
        }
    }

    Board_driversClose();
    Drivers_i2cClose();

//    return result;
}

/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief TODO
 *
 * @param pcWriteBuffer cli output string buffer
 * @param xWriteBufferLen length of the cli output string
 * @param pcCommandString cli command input string
 * @return success
 */
BaseType_t i2cRtcCommand( char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString )
{
    Time_t time = {0};

    i2cGetRtcTime(&time);

    sprintf(pcWriteBuffer, "Date: %02u.%.02u.%.02u Time: %02u:%02u:%02u\r\n", time.day, time.month, time.year, time.h, time.min, time.sec);

    return pdFALSE;
}

/*[EOF]************************************************************************/
