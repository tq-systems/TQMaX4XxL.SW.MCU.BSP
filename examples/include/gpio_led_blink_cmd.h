/** SPDX-License-Identifier: BSD-3-Clause
 *
 * @file gpio_led_blink_cmd.h
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Jeremias Schneider
 * @author Michael Bernhardt
 *
 * @date 2022-04-14
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the declaration of the gpio LEDs.
 *
 */

#ifndef GPIO_LED_BLINK_CMD_H_
#define GPIO_LED_BLINK_CMD_H_

/*******************************************************************************
 * includes
 ******************************************************************************/

#include "portmacro.h"
#include "projdefs.h"
#include "FreeRTOS_CLI.h"

/*******************************************************************************
 * defines
 ******************************************************************************/



/*******************************************************************************
 * macros
 ******************************************************************************/



/*******************************************************************************
 * typedefs
 ******************************************************************************/



/*******************************************************************************
 * prototypes
 ******************************************************************************/

void gpio_led_blink_main(void *args);
BaseType_t ledBlinkCommand(char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString);

/*******************************************************************************
 * global extern data
 ******************************************************************************/

extern const CLI_Command_Definition_t ledBlinkCommandDef;

/******************************************************************************/

#endif /* GPIO_LED_BLINK_CMD_H_ */
