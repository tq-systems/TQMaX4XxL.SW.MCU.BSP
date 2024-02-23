/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file gpio_dig_cmd.h
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2022-07-15
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the digital gpio declaration.
 *
 */


#ifndef EXAMPLES_INCLUDE_GPIO_DIG_CMD_H_
#define EXAMPLES_INCLUDE_GPIO_DIG_CMD_H_

/*******************************************************************************
 * includes
 ******************************************************************************/

/* runtime */
#include "stdbool.h"
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

typedef enum
{
    PIN_STATUS_LOW  = 0,
    PIN_STATUS_HIGH = 1,
} pinStatus_t;

typedef enum
{
    DIG_OUT_1 = 0,
    DIG_OUT_2,
    DIG_OUT_3,
    DIG_OUT_4,
    ADC_RST,
    DIG_OUT_MAX,
} GpioOut_t;


/*******************************************************************************
 * prototypes
 ******************************************************************************/

BaseType_t gpioDigCommand(char* pcWriteBuffer, __size_t xWriteBufferLen, const char* pcCommandString);
bool gpio_writePinOut(GpioOut_t gpio, pinStatus_t state);
void gpioInit(void);

/*******************************************************************************
 * global extern data
 ******************************************************************************/

extern const CLI_Command_Definition_t gpioDigCommandDef;

/******************************************************************************/

#endif /* EXAMPLES_INCLUDE_GPIO_DIG_CMD_H_ */

/*[EOF]************************************************************************/
