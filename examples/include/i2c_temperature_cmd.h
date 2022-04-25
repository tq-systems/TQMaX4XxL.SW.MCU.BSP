/*
 * gpio_led_blink_cmd.h
 *
 *  Created on: 14.04.2022
 *      Author: schneiderj
 */

#ifndef I2C_TEMPERATURE_CMD_H_
#define I2C_TEMPERATURE_CMD_H_

#include "portmacro.h"
#include "projdefs.h"
#include "FreeRTOS_CLI.h"

extern const CLI_Command_Definition_t i2cTempCommandDef;

BaseType_t i2cTempCommand(char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString);

#endif /* I2C_TEMPERATURE_CMD_H_ */
