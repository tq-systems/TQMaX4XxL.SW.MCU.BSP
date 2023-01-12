/**
 * @file rs485_cmd.h
 * @author Copyright (c) 2023, TQ-Systems GmbH
 * @author Michael Bernhardt
 * @date 2023-01-04
 *
 * This software code contained herein is licensed under the terms and
 * conditions of the TQ-Systems Software License Agreement Version 1.0.2.
 * You may obtain a copy of the TQ-Systems Software License Agreement in the
 * folder TQS (TQ-Systems Software Licenses) at the following website:
 * https://www.tq-group.com/Software-Lizenzbedingungen
 * In case of any license issues please contact license@tq-group.com.
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
