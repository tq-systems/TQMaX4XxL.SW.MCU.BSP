/**
 * @file emmc_cmd.h
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

#ifndef EXAMPLES_INCLUDE_EMMC_CMD_H_
#define EXAMPLES_INCLUDE_EMMC_CMD_H_

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

BaseType_t emmcCommand( char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString );

/*******************************************************************************
 * global extern data
 ******************************************************************************/

extern const CLI_Command_Definition_t emmcCommandDef;

/******************************************************************************/

#endif /* EXAMPLES_INCLUDE_EMMC_CMD_H_ */

/*[EOF]************************************************************************/
