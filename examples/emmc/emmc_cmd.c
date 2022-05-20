/**
 * @file emmc_cmd.c
 * @author Copyright (c) 2022, TQ-Systems GmbH
 * @author TODO author of the file <TODO@tq-group.com>
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
/* project */
#include <drivers/mmcsd.h>
#include "ti_drivers_config.h"
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
    "\r\nemmc [eMMC]:\r\n TODO\r\n\r\n",
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
    MMCSD_Params mmcsdParams;
    MMCSD_Handle gMmcsdHandle;
    int32_t status = 0;
    uint8_t rBuff[10] = {0};

    MMCSD_Params_init(&mmcsdParams);
    gMmcsdHandle = MMCSD_open(0, &mmcsdParams);
    gMmcsdHandle = MMCSD_getHandle(CONFIG_MMCSD0);

    status = MMCSD_read(gMmcsdHandle, rBuff, 0, 1);

    if (status == SystemP_SUCCESS)
    {
        sprintf(pcWriteBuffer, "Buffer: %u %u %u %u\r\n", rBuff[0], rBuff[1], rBuff[2], rBuff[4]);
    }
    else
    {
        sprintf(pcWriteBuffer, "Error status: %d\r\n", status);
    }


    MMCSD_close(gMmcsdHandle);

    return pdFALSE;
}

/*[EOF]************************************************************************/
