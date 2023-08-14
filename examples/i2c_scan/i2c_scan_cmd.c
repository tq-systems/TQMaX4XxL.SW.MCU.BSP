/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file i2c_scan_cmd.c
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2022-05-12
 *
 * -----------------------------------------------------------------------------
 * @brief This function contains the implementation of the i2c bus scan.
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
#include <drivers/i2c.h>
#include <i2c_scan_cmd.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
/* own */

/*******************************************************************************
 * local defines
 ******************************************************************************/



/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "i2cBusScan" command. */
const CLI_Command_Definition_t i2cBusScanCommandDef =
{
    "i2cbusscan",
    "\r\ni2cbusscan:\r\n Scans the I2C bus from address 0x03 until 0x77 \r\n\r\n",
    i2cBusScanCommand,
    0
};


/*******************************************************************************
 * local typedefs
 ******************************************************************************/



/*******************************************************************************
 * local static data
 ******************************************************************************/



/*******************************************************************************
 * forward declarations
 ******************************************************************************/

static bool i2c_busScan(uint8_t i2cAddress);

/*******************************************************************************
 * local static functions
 ******************************************************************************/

/**
 * @brief This function scans an i2c address and returns whether there is a device on the address.
 *
 * @param i2cAddress address to be scanned
 * @return true = device found, false = no device found
 */
static bool i2c_busScan(uint8_t i2cAddress)
{
    I2C_Handle i2cHandle = NULL;
    bool retVal = false;

    Drivers_i2cOpen();

    i2cHandle = gI2cHandle[CONFIG_I2C0];

    if (I2C_probe(i2cHandle, i2cAddress) == SystemP_SUCCESS)
    {
        retVal = true;
    }
    else
    {
        retVal = false;
    }

    Drivers_i2cClose();

    return retVal;
}

/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief This function scans the entire address range of the i2c bus.
 *
 * @param pcWriteBuffer cli output string buffer
 * @param xWriteBufferLen length of the cli output string
 * @param pcCommandString cli command input string
 * @return pdFALSE = i2c scan is complete, pdTRUE = i2c scan is not complete
 */
BaseType_t i2cBusScanCommand( char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString )
{
    typedef enum
    {
        PRINT_HEADER,
        PRINT_I2C_SCAN,
        PRINT_NEXT_LINE,
        END_OF_SCAN,
    } I2cScanState_t;

    const uint8_t FIRST_I2C_ADD     = 0x03;
    const uint8_t LAST_I2C_ADD      = 0x77;

    BaseType_t retVal               = pdTRUE;
    static uint16_t addrCounter     = FIRST_I2C_ADD;
    static I2cScanState_t i2cState  = PRINT_HEADER;

    switch (i2cState)
    {
    case PRINT_HEADER:
        sprintf(pcWriteBuffer, "    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\r\n%02X          ", addrCounter);
        i2cState = PRINT_I2C_SCAN;
        break;

    case PRINT_I2C_SCAN:

        /* write CLI output */
        if (i2c_busScan(addrCounter) == true)
        {
            sprintf(pcWriteBuffer, "%02X ", addrCounter);
        }
        else
        {
            sprintf(pcWriteBuffer, "-- ");
        }

        /* Increase the counter to scan the next I2C address next time. */
        addrCounter++;

        /* Is the line completely printed on the CLI? */
        if ((addrCounter % 0x10) == 0)
        {
            i2cState = PRINT_NEXT_LINE;
        }

        /* Was the entire address range scanned? */
        if (addrCounter > LAST_I2C_ADD)
        {
            i2cState = END_OF_SCAN;
        }
        break;

    case PRINT_NEXT_LINE:
        sprintf(pcWriteBuffer, "\r\n%02X ", addrCounter);
        i2cState = PRINT_I2C_SCAN;
        break;

    case END_OF_SCAN:
        sprintf(pcWriteBuffer, "\r\n");
        addrCounter = 0;
        i2cState    = PRINT_HEADER;
        retVal      = pdFALSE;
        break;

    default:
        sprintf(pcWriteBuffer, "Error while scanning \r\n");
        addrCounter = 0;
        i2cState    = PRINT_HEADER;
        retVal      = pdFALSE;
        break;
    }

    return retVal;
}


/*[EOF]************************************************************************/
