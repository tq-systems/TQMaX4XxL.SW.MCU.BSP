/**
 * @file emmc_cmd.c
 * @author Copyright (c) 2022, TQ-Systems GmbH
 * @author Michael Bernhardt
 * @date 2022-05-19
 *
 * This software code contained herein is licensed under the terms and
 * conditions of the TQ-Systems Software License Agreement Version 1.0.2.
 * You may obtain a copy of the TQ-Systems Software License Agreement in the
 * folder TQS (TQ-Systems Software Licenses) at the following website:
 * https://www.tq-group.com/Software-Lizenzbedingungen
 * In case of any license issues please contact license@tq-group.com.
 *
 * -----------------------------------------------------------------------------
 * @brief This file includes the eMMC implementation.
 *
 */

/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
/* project */

/* own */
#include "emmc_cmd.h"
/*******************************************************************************
 * local defines
 ******************************************************************************/



/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "ledblink" command. */
const CLI_Command_Definition_t emmcCommandDef =
{
    "emmc",
    "\r\nemmc [eMMC]:\r\n This function starts an eMMC raw io test.\r\n\r\n",
    emmcCommand,
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

extern bool mmcsd_raw_io_main(void *args);

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
BaseType_t emmcCommand( char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString )
{
    if (mmcsd_raw_io_main(NULL) == true)
    {
        sprintf(pcWriteBuffer, "eMMC all tests have passed \r\n");
    }
    else
    {
        sprintf(pcWriteBuffer, "eMMC tests failed\r\n");
    }

    return pdFALSE;
}

/*[EOF]************************************************************************/
