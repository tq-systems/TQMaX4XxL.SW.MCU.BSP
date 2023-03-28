/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file rs485_cmd.c
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt

 * @date 2023-01-04
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the implementation of the RS485 command.
 *
 */

/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
/* project */
#include "projdefs.h"
#include "utilities.h"
/* own */
#include "rs485_cmd.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/



/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "ledblink" command. */
const CLI_Command_Definition_t rs485CommandDef =
{
    "rs485",
    "\r\nrs485 [rs485]:\r\n Read or write data via RS485.\r\n\r\n",
    rs485Command,
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

void uart_echo(void *args);

/*******************************************************************************
 * local static functions
 ******************************************************************************/



/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief This function handles the command of the RS485.
 *
 * @param pcWriteBuffer cli output string buffer
 * @param xWriteBufferLen length of the cli output string
 * @param pcCommandString cli command input string
 * @return pdFALSE = command is finished
 */
BaseType_t rs485Command(char* pcWriteBuffer, __size_t xWriteBufferLen, const char* pcCommandString)
{
    UNUSED_PARAM(pcWriteBuffer);
    UNUSED_PARAM(xWriteBufferLen);
    UNUSED_PARAM(pcCommandString);

    uart_echo(NULL);

    return pdFALSE;
}

/*[EOF]************************************************************************/
