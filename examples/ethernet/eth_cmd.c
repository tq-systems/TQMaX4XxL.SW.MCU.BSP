/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file eth_cmd.c
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2022-07-04
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the implementation of the ethernet commands.
 *
 */

/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */

/* project */

/* own */
#include "eth_cmd.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/



/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "eth" command. */
const CLI_Command_Definition_t ethCommandDef =
{
    "eth",
    "\r\neth [eth]:\r\n TODO\r\n\r\n",
    ethCommand,
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
 * @brief This function handles the ethernet command
 *
 * @param pcWriteBuffer cli output string buffer
 * @param xWriteBufferLen length of the cli output string
 * @param pcCommandString cli command input string
 * @return pdFALSE = eth cmd is complete
 */
BaseType_t ethCommand( char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString )
{
    return pdFALSE;
}


/*[EOF]************************************************************************/
