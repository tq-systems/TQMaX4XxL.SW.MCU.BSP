/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file qspi_nor_flash_cmd.h
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2022-05-13
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the declaration of the ospi NOR flash commands.
 *
 */

#ifndef EXAMPLES_INCLUDE_QSPI_NOR_FLASH_CMD_H_
#define EXAMPLES_INCLUDE_QSPI_NOR_FLASH_CMD_H_

/*******************************************************************************
 * includes
 ******************************************************************************/

/* runtime */

/* project */
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

BaseType_t qspiNorFlashCommand( char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString );

/*******************************************************************************
 * global extern data
 ******************************************************************************/

extern const CLI_Command_Definition_t qspiNorFlashCommandDef;

/******************************************************************************/

#endif /* EXAMPLES_INCLUDE_QSPI_NOR_FLASH_CMD_H_ */

/*[EOF]************************************************************************/
