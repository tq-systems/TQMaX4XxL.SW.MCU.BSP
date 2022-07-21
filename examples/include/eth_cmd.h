/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file eth_cmd.h
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2022-07-04
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the declaration of the ethernet commands.
 *
 */

#ifndef EXAMPLES_INCLUDE_ETH_CMD_H_
#define EXAMPLES_INCLUDE_ETH_CMD_H_

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

BaseType_t ethCommand( char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString );
void ethTask(void *args);

/*******************************************************************************
 * global extern data
 ******************************************************************************/

extern const CLI_Command_Definition_t ethCommandDef;

/******************************************************************************/

#endif /* EXAMPLES_INCLUDE_ETH_CMD_H_ */

/*[EOF]************************************************************************/
