/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file mcan_cmd.c
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2022-06-21
 *
 * -----------------------------------------------------------------------------
 * @brief TODO
 *
 */

/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */
#include <stdio.h>
#include <string.h>
/* project */
#include <drivers/mcan.h>
/* own */
#include "mcan_cmd.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/


/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "ledblink" command. */
const CLI_Command_Definition_t mcanCommandDef =
{
    "mcan",
    "\r\nmcan [MCAN]:\r\n TODO\r\n\r\n",
    mcanCommand,
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


/*******************************************************************************
 * local static functions
 ******************************************************************************/



/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief This function handles the MCAN command
 *
 * @param pcWriteBuffer cli output string buffer
 * @param xWriteBufferLen length of the cli output string
 * @param pcCommandString cli command input string
 * @return success
 */
BaseType_t mcanCommand( char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString )
{
    BaseType_t xParameterStringLength       = 0;
    const char* pcParameter1 = FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameterStringLength);


    switch (*pcParameter1)
    {
    case '1':
//        mcan_loopback_interrupt_main(NULL);
//        mcan_loopback_tx_interrupt_main(NULL);
        test();
        sprintf(pcWriteBuffer, "success\r\n");
        break;

    case '2':
//        mcan_loopback_polling_main(NULL);
        mcan_loopback_rx_interrupt_main(NULL);
        sprintf(pcWriteBuffer, "success\r\n");
        break;

    default:
        sprintf(pcWriteBuffer, "wrong parameter \r\n");
        break;
    }

    return pdFALSE;
}

/*[EOF]************************************************************************/
