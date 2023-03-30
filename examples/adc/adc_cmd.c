/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file adc_cmd.c
 * @author Copyright (c) 2023, TQ-Systems GmbH
 * @author Michael Bernhardt
 *
 * @date 2023-01-10
 *
 * -----------------------------------------------------------------------------
 * @brief This function contains the implementation of the ADC command.
 *
 */

/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
/* project */

/* own */
#include "adc_cmd.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/

#define APP_ADC_NUM_CH              (8U)

/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "eMMC" command. */
const CLI_Command_Definition_t adcCommandDef =
{
    "adc",
    "\r\nadc [adc]:\r\n This function reads the ADCs.\r\n\r\n",
    adcCommand,
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

int32_t adc_singleshot_main(void *args);

/*******************************************************************************
 * local static functions
 ******************************************************************************/



/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief This function handles the ADC command
 *
 * @param pcWriteBuffer cli output string buffer
 * @param xWriteBufferLen length of the cli output string
 * @param pcCommandString cli command input string
 * @return pdFALSE = command is finished
 */
BaseType_t adcCommand( char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString )
{
    uint32_t adcValue[APP_ADC_NUM_CH] = {0};
    uint32_t counter                  = 0;
    uint8_t  i                        = 0;
    int32_t status                    = SystemP_SUCCESS;

    status = adc_singleshot_main(adcValue);

    if (SystemP_SUCCESS == status)
    {
        counter += sprintf(&pcWriteBuffer[counter], "Step ID\tVoltage Level\r\n");
        counter += sprintf(&pcWriteBuffer[counter], "-------\t-------------\r\n");
        for (i = 0; i < APP_ADC_NUM_CH; i++)
        {
            counter += sprintf(&pcWriteBuffer[counter], "%d\t%d mV\r\n", i, adcValue[i]);
        }
    }
    else
    {
        counter += sprintf(&pcWriteBuffer[counter], "[ADC] Failure %i\r\n", status);
    }


    return pdFALSE;
}

/*[EOF]************************************************************************/
