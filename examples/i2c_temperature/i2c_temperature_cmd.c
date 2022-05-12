/** SPDX-License-Identifier: BSD-3-Clause
 *
 * @file gpio_led_blink_cmd.c
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Jeremias Schneider
 *
 * @date 2022-04-14
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the implementation of the I2C temperature command.
 *
 */

/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */
#include <stdlib.h>
#include <stdio.h>
/* project */

/* own */
#include "i2c_temperature_cmd.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/



/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "i2ctemp" command. */
const CLI_Command_Definition_t i2cTempCommandDef =
{
    "i2ctemp",
    "\r\ni2ctemp:\r\n Takes a fixed number of temperature sensor samples and prints them to console\r\n\r\n",
    i2cTempCommand,
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



/*******************************************************************************
 * local static functions
 ******************************************************************************/


/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief This function reads the temperature via i2c.
 *
 * @param pcWriteBuffer cli output string buffer
 * @param xWriteBufferLen length of the cli output string
 * @param pcCommandString cli command input string
 * @return success
 */
BaseType_t i2cTempCommand( char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString )
{
    sprintf(pcWriteBuffer, "Temperature sensor: %.2f deg C\r\n", i2cGetTemperature(NULL));

    return pdFALSE;
}

/*[EOF]************************************************************************/
