/**
 * @file rs485_cmd.c
 * @author Copyright (c) 2023, TQ-Systems GmbH
 * @author TODO author of the file <TODO@tq-group.com>
 * @date 2023-01-04
 *
 * This software code contained herein is licensed under the terms and
 * conditions of the TQ-Systems Software License Agreement Version 1.0.2.
 * You may obtain a copy of the TQ-Systems Software License Agreement in the
 * folder TQS (TQ-Systems Software Licenses) at the following website:
 * https://www.tq-group.com/Software-Lizenzbedingungen
 * In case of any license issues please contact license@tq-group.com.
 *
 * -----------------------------------------------------------------------------
 * @brief <TODO short description of the file (only one line)>
 *
 * <TODO Detailed description of the file>
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
    "RS485",
    "\r\nrs485 [rs485]:\r\n Read or write data via RS485.\r\n\r\n",
    rs485Command,
    -1
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

BaseType_t rs485Command(char* pcWriteBuffer, __size_t xWriteBufferLen, const char* pcCommandString)
{
    bool       err                          = false;
    char*      pNextNumber                  = NULL;
    char*      pcParameter                  = {NULL};
    BaseType_t xParameterStringLength       = 0;
    uint8_t    dataBuf                      = 0;

    /* get all parameter */
    pcParameter = (char*) FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameterStringLength);

    uart_echo(NULL);

    return pdFALSE;
}

/*[EOF]************************************************************************/
