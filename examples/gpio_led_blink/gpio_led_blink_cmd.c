/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file gpio_led_blink_cmd.c
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Jeremias Schneider
 * @author Michael Bernhardt
 *
 * @date 2022-04-14
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the implementation of the gpio LEDs.
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
#include "gpio_led_blink_cmd.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/



/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "ledblink" command. */
const CLI_Command_Definition_t ledBlinkCommandDef =
{
    "ledblink",
    "\r\nledblink [LED]:\r\n Lets the LED blink a fixed number of times, use 1 or 2 for user LED 1 or 2.\r\n\r\n",
    ledBlinkCommand,
    1
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

extern void gpio_led_blink_main(void *args);

/*******************************************************************************
 * local static functions
 ******************************************************************************/


/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief This function handles the led blink command
 *
 * @param pcWriteBuffer cli output string buffer
 * @param xWriteBufferLen length of the cli output string
 * @param pcCommandString cli command input string
 * @return success
 */
BaseType_t ledBlinkCommand( char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString )
{
    BaseType_t xParameter1StringLength = 0;
    const char* pcParameter1 = FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameter1StringLength);

    if ((*pcParameter1 == '1') || (*pcParameter1 == '2'))
    {
        gpio_led_blink_main((char*)pcParameter1);
        sprintf(pcWriteBuffer, "Command executed successfully\r\n");
    }
    else
    {
        sprintf(pcWriteBuffer, "Wrong input parameter\r\n");
    }

    return pdFALSE;
}

/*[EOF]************************************************************************/
