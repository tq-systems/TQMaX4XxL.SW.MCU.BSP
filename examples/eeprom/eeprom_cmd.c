/**
 * @file eeprom_cmd.c
 * @author Copyright (c) 2022, TQ-Systems GmbH
 * @author TODO author of the file <TODO@tq-group.com>
 * @date 2022-05-18
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

/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
/* project */
#include <drivers/i2c.h>
#include <kernel/dpl/DebugP.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
/* own */
#include "eeprom_cmd.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/

#define DECIMAL_BASE      (10U)
#define EEPROM_READ_SIZE  (1U)
#define MAX_INPUT_PARAM   (2U)

/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "ledblink" command. */
const CLI_Command_Definition_t eepromCommandDef =
{
    "eeprom",
    "\r\neeprom [I2c]:\r\n Read or write the EEPROM.\r\n\r\n",
    eepromCommand,
    2
};

/*******************************************************************************
 * local typedefs
 ******************************************************************************/

typedef struct
{
    uint8_t instance;
    uint32_t offset;
    uint8_t data;
} eeprom_t;

/*******************************************************************************
 * local static data
 ******************************************************************************/



/*******************************************************************************
 * forward declarations
 ******************************************************************************/

static bool eepromRead(eeprom_t* eepromData);

/*******************************************************************************
 * local static functions
 ******************************************************************************/

static bool eepromRead(eeprom_t* eepromData)
{
    bool success = false;

    DebugP_log("[EEPROM] Read EEPROM instance: %d.\r\n", eepromData->instance);

    if ((eepromData->instance == TQ_EEPROM) || (eepromData->instance == CUST_EEPROM))
    {
        Drivers_i2cOpen();

        if (Board_driversOpen() != SystemP_SUCCESS)
        {
            DebugP_logError("[EEPROM] Board driver failed for instance %d !!!\r\n", eepromData->instance);
        }
        else if (EEPROM_read(gEepromHandle[TQ_EEPROM], eepromData->offset, &eepromData->data, EEPROM_READ_SIZE) != SystemP_SUCCESS)
        {
            DebugP_logError("[EEPROM] Read failed for instance %d !!!\r\n", eepromData->instance);
        }
        else
        {
            success = true;
        }

        Board_driversClose();
        Drivers_i2cClose();
    }

    return success;
}

/*******************************************************************************
 * global functions
 ******************************************************************************/

BaseType_t eepromCommand( char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString )
{
    uint8_t paramCount  = 0;
    eeprom_t eepromData = {0};
    bool err = false;
    char* pNextNumber                   = NULL;
    BaseType_t xParameterStringLength = 0;

    char* pcParameter[MAX_INPUT_PARAM]  = {NULL};

    /* get all parameter */
    for (paramCount = 0; paramCount < MAX_INPUT_PARAM; paramCount++)
    {
        pcParameter[paramCount] = (char*) FreeRTOS_CLIGetParameter(pcCommandString, paramCount+1, &xParameterStringLength);
    }

    eepromData.instance = strtoul(pcParameter[0], &pNextNumber, DECIMAL_BASE);
    if ((eepromData.instance > 1) || (pcParameter[0] == pNextNumber))
    {
        err = true;
    }

    eepromData.offset = strtoul(pcParameter[1], &pNextNumber, DECIMAL_BASE);
    if (pcParameter[1] == pNextNumber)
    {
        err = true;
    }

    if ((err != true) && (eepromRead(&eepromData) == true))
    {
        sprintf(pcWriteBuffer, "EEPROM data 0x%02X\r\n",eepromData.data);
    }
    else
    {
        sprintf(pcWriteBuffer, "EEPROM failure\r\n");
    }

    return pdFALSE;
}


/*[EOF]************************************************************************/
