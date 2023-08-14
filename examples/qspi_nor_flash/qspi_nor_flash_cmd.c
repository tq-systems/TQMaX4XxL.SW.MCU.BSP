/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file qspi_nor_flash_cmd.c
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2022-05-23
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the implementation of the QSPI NOR flash (MX25U51245G) commands.
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
#include <kernel/dpl/DebugP.h>
#include <drivers/ospi.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
#include "FreeRTOS.h"
#include "task.h"
#include "utilities.h"
/* own */
#include "qspi_nor_flash_cmd.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/



/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "qspiflash" command. */
const CLI_Command_Definition_t qspiNorFlashCommandDef =
{
    "qspiflash",
    "\r\nqspiflash:\r\n NOR flash test\r\n Write and read data to flash and compare them.\r\n\r\n",
    qspiNorFlashCommand,
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

extern int32_t ospi_flash_io_main(void *args);

/*******************************************************************************
 * local static functions
 ******************************************************************************/



/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief This function handles the QSPI NOR flash command.
 *
 * @param pcWriteBuffer cli output string buffer
 * @param xWriteBufferLen length of the cli output string
 * @param pcCommandString cli command input string
 * @return pdFALSE = command is finished
 */
BaseType_t qspiNorFlashCommand(char* const pcWriteBuffer, __size_t xWriteBufferLen, const char* pcCommandString )
{
    BaseType_t retVal = pdFALSE;

    int32_t status = ospi_flash_io_main(NULL);

    if (status == SystemP_SUCCESS)
    {
        sprintf(pcWriteBuffer, "[QSPI FLASH] All tests have passed.\r\n");
    }
    else
    {
        sprintf(pcWriteBuffer, "[QSPI FLASH] Some tests have failed. %i\r\n", status);
    }

    return retVal;
}


/*[EOF]************************************************************************/
