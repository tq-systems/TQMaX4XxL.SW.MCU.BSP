/*
 * gpio_led_blink_cmd.h
 *
 *  Created on: 14.04.2022
 *      Author: schneiderj
 */

#ifndef GPIO_LED_BLINK_CMD_H_
#define GPIO_LED_BLINK_CMD_H_

#include "portmacro.h"
#include "projdefs.h"
#include "FreeRTOS_CLI.h"

extern const CLI_Command_Definition_t ledBlinkCommandDef;

BaseType_t ledBlinkCommand(char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString);

#endif /* GPIO_LED_BLINK_CMD_H_ */
