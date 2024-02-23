/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file afe_cmd.c
 * @copyright Copyright (c) 2023 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2023-02-17
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the implementation of the CLI AFE command.
 *
 */

/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */
#include "string.h"
#include "stdlib.h"
/* project */
#include "afe.h"
/* own */
#include "afe_cmd.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/

#define MAX_CLI_PARAM  3U

/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "AFE" command. */
const CLI_Command_Definition_t afeCommandDef =
{
    "afe",
    "\r\nafe [afe]:\r\n AFE access.\n"
    " Usage afe [access] [channel] [value]:\n"
    " access:\n"
    "  adc  - read adc\n"
    "  temp - read DIE temperature\n"
    "  gpio - read GPIO state\n"
    "  set  - set GPIO state\n"
    " channel:\n"
    "  0 - 11 ADC channel\n"
    "  0 - 9 GPIO channel\n"
    " state:\n"
    "  0, 1\r\n\r\n",
    afeCommand,
    -1
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



/*******************************************************************************
 * local static functions
 ******************************************************************************/



/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief This function handles the command of the AFE access.
 *
 * @param pcWriteBuffer cli output string buffer
 * @param xWriteBufferLen length of the cli output string
 * @param pcCommandString cli command input string
 * @return pdFALSE = command is finished
 */
BaseType_t afeCommand( char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString )
{
    uint32_t counter                  = 0;
    uint8_t gpio                      = 0;
    float val                         = 0;
    char* pcParameter[3]              = {NULL};
    BaseType_t xParameterStringLength = 0;

    /* get all parameter */
    for (counter = 0; counter < MAX_CLI_PARAM; counter++)
    {
        pcParameter[counter] = (char*) FreeRTOS_CLIGetParameter(pcCommandString, counter + 1, &xParameterStringLength);
    }

    if (!strncmp(pcParameter[0], "adc", strlen("adc")))
    {
        if (pcParameter[1] != NULL)
        {
            val = AFE_ReadAdcVoltage(strtoul(pcParameter[1], NULL, 10));
            sprintf(&pcWriteBuffer[0], "Voltage %.1fV\r\n\n", val);
        }
    }
    else if (!strncmp(pcParameter[0], "temp", strlen("temp")))
    {
        val = AFE_ReadDieTemp();
        sprintf(&pcWriteBuffer[0], "Temperature %.1f%cC\r\n\n", val, (uint8_t)176);
    }
    else if (!strncmp(pcParameter[0], "gpio", strlen("gpio")))
    {
        if (pcParameter[1] != NULL)
        {
            gpio = AFE_GpioRead(strtoul(pcParameter[1], NULL, 10));
            sprintf(&pcWriteBuffer[0], "GPIO %d \r\n\n", gpio);
        }
    }
    else if (!strncmp(pcParameter[0], "set",  strlen("set")))
    {
        if ((pcParameter[1] != NULL) && (pcParameter[2] != NULL))
        {
            AFE_GpioSet(strtoul(pcParameter[1], NULL, 10), strtoul(pcParameter[2], NULL, 10));
            sprintf(&pcWriteBuffer[0], "OK \r\n\n");
        }
    }

    return pdFALSE;
}

/*[EOF]************************************************************************/
