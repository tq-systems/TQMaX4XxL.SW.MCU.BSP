/*
 * i2c_temperature_cmd.c
 *
 *  Created on: 14.04.2022
 *      Author: schneiderj
 */
#include <stdlib.h>

#include "i2c_temperature_cmd.h"

void i2c_temperature_main(void *args);

/* The definition of the "i2ctemp" command. */
const CLI_Command_Definition_t i2cTempCommandDef =
{
    "i2ctemp",
    "\r\ni2ctemp:\r\n Takes a fixed number of temperature sensor samples and prints them to console\r\n\r\n",
    i2cTempCommand,
    0
};

BaseType_t i2cTempCommand( char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString )
{
    i2c_temperature_main(NULL);

    return pdFALSE;
}
