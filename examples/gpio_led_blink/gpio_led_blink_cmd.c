/*
 * gpio_led_blink_cmd.c
 *
 *  Created on: 14.04.2022
 *      Author: schneiderj
 */
#include <stdlib.h>

#include "gpio_led_blink_cmd.h"

void gpio_led_blink_main(void *args);

/* The definition of the "ledblink" command. */
const CLI_Command_Definition_t ledBlinkCommandDef =
{
    "ledblink",
    "\r\nledblink:\r\n Lets the LED blink a fixed number of times\r\n\r\n",
    ledBlinkCommand,
    0
};

BaseType_t ledBlinkCommand( char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString )
{
    gpio_led_blink_main(NULL);

    return pdFALSE;
}
