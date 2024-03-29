/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file lpddr4_cmd.h
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2022-06-09
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the declaration of the LPDDR4.
 */

#ifndef EXAMPLES_INCLUDE_LPDDR4_CMD_H_
#define EXAMPLES_INCLUDE_LPDDR4_CMD_H_

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

BaseType_t lpddr4Command(char* pcWriteBuffer, __size_t xWriteBufferLen, const char* pcCommandString);

/*******************************************************************************
 * global extern data
 ******************************************************************************/

extern const CLI_Command_Definition_t lpddr4CommandDef;

/******************************************************************************/

#endif /* EXAMPLES_INCLUDE_LPDDR4_CMD_H_ */

/*[EOF]************************************************************************/
