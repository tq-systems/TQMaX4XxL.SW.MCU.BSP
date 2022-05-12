/** SPDX-License-Identifier: BSD-3-Clause
 *
 * @file i2c_temperature_cmd.h
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Jeremias Schneider
 *
 * @date 2022-04-14
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the declaration of the I2C temperature command.
 *
 */

#ifndef I2C_TEMPERATURE_CMD_H_
#define I2C_TEMPERATURE_CMD_H_

/*******************************************************************************
 * includes
 ******************************************************************************/

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

float i2cGetTemperature(void *args);
BaseType_t i2cTempCommand(char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString);

/*******************************************************************************
 * global extern data
 ******************************************************************************/

extern const CLI_Command_Definition_t i2cTempCommandDef;

/******************************************************************************/

#endif /* I2C_TEMPERATURE_CMD_H_ */
