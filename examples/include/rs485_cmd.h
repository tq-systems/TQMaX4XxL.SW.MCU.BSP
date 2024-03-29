/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file rs485_cmd.h
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2023-01-04
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the declaration of the RS485 command.
 *
 */

#ifndef EXAMPLES_INCLUDE_RS485_CMD_H_
#define EXAMPLES_INCLUDE_RS485_CMD_H_

/*******************************************************************************
 * includes
 ******************************************************************************/

/* runtime */

/* project */
#include "portmacro.h"
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

BaseType_t rs485Command(char* pcWriteBuffer, __size_t xWriteBufferLen, const char* pcCommandString);

/*******************************************************************************
 * global extern data
 ******************************************************************************/

extern const CLI_Command_Definition_t rs485CommandDef;

/******************************************************************************/

#endif /* EXAMPLES_INCLUDE_RS485_CMD_H_ */

/*[EOF]************************************************************************/
