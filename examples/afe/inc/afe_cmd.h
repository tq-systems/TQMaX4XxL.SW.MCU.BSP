/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file afe_cmd.h
 * @copyright Copyright (c) 2023 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2023-02-17
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the declaration of the AFE command.
 *
 */

#ifndef EXAMPLES_AFE_INC_AFE_CMD_H_
#define EXAMPLES_AFE_INC_AFE_CMD_H_

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

BaseType_t afeCommand(char* pcWriteBuffer, __size_t xWriteBufferLen, const char* pcCommandString);

/*******************************************************************************
 * global extern data
 ******************************************************************************/

extern const CLI_Command_Definition_t afeCommandDef;

/******************************************************************************/

#endif /* EXAMPLES_AFE_INC_AFE_CMD_H_ */

/*[EOF]************************************************************************/
