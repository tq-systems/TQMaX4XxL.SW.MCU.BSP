/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file lpddr4_cmd.c
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2022-06-09
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the implementation of the LPDDR4.
 */

/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */
#include <stdlib.h>
#include <stdio.h>
/* project */

/* own */
#include "lpddr4_cmd.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/

#define HEXA_BASE       (16U)
#define DDR4_SIZE       (0x80000000U)

/*******************************************************************************
 * local macros
 ******************************************************************************/

enum
{
    LPDDR4_ACCESS,
    LPDDR4_OFFSET,
    LPDDR4_VALUE,
    LPDDR4_MAX
};

/* The definition of the "LPDDR4" command. */
const CLI_Command_Definition_t lpddr4CommandDef =
{
    "lpddr4",
    "\r\nlpddr4 [lpddr4]:\r\n Read or write from/to the LPDDR4.\n "
    " Usage lpddr4 [access] [offset] [value]\n"
    " access:\n"
    "  w - write\n"
    "  r - read\n"
    " offset:\n"
    "  address offset in hex\n"
    " value:\n"
    "  value to write in LPDDR4, only used in the write option.\r\n\r\n",
    lpddr4Command,
    -1
};

/*******************************************************************************
 * local typedefs
 ******************************************************************************/



/*******************************************************************************
 * local static data
 ******************************************************************************/

static const uint32_t LPDDR4_START_ADD = 0x80000000;

/*******************************************************************************
 * forward declarations
 ******************************************************************************/

static void writeLpddr4(uint32_t offset, uint32_t value);
static uint32_t readLpddr4(uint32_t offset);

/*******************************************************************************
 * local static functions
 ******************************************************************************/

/**
 * @brief This function writes data to the LPDDR4.
 *
 * @param offset Offset of the LPDDR4 address to be written.
 * @param value Value to write to LPDDR4.
 */
static void writeLpddr4(uint32_t offset, uint32_t value)
{
    uint32_t* ddr4Add  = (uint32_t*) (LPDDR4_START_ADD + offset);

    *ddr4Add = value;

    DebugP_log("[LPDDR4] Write value 0x%02X to add 0x%02X.\r\n", value, ddr4Add);
}

/**
 * @brief This function reads data from the LPDDR4.
 *
 * @param offset Offset of the LPDDR4 address to be read.
 * @return Value of the read address.
 */
static uint32_t readLpddr4(uint32_t offset)
{
    uint32_t* ddr4Add  = NULL;
    uint32_t value     = 0;

    ddr4Add = (uint32_t*) (LPDDR4_START_ADD + offset);
    value   = *(uint32_t*)ddr4Add;
    DebugP_log("[LPDDR4] Read from add: 0x%02X: 0x%02X\r\n", ddr4Add, value);

    return value;
}

/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief This function handles the command of the LPDDR4 access.
 *
 * @param pcWriteBuffer cli output string buffer
 * @param xWriteBufferLen length of the cli output string
 * @param pcCommandString cli command input string
 * @return pdFALSE = command is finished
 */
BaseType_t lpddr4Command(char* pcWriteBuffer, __size_t xWriteBufferLen, const char* pcCommandString)
{
    uint8_t    paramCount              = 0;
    char*      pcParameter[LPDDR4_MAX] = {NULL};
    BaseType_t xParameterStringLength  = 0;
    char*      pNextNumber             = NULL;
    uint32_t   offset                  = 0;
    uint32_t   value                   = 0;

    /* get all parameter */
    for (paramCount = 0; paramCount < LPDDR4_MAX; paramCount++)
    {
        pcParameter[paramCount] = (char*) FreeRTOS_CLIGetParameter(pcCommandString, paramCount+1, &xParameterStringLength);
    }

    offset = strtoul(pcParameter[LPDDR4_OFFSET], &pNextNumber, HEXA_BASE);

    if ((*pcParameter[LPDDR4_ACCESS] == 'w') && (offset < (DDR4_SIZE - 4)) && (pcParameter[LPDDR4_VALUE] != NULL))
    {
        value  = strtoul(pcParameter[LPDDR4_VALUE], &pNextNumber, HEXA_BASE);
        writeLpddr4(offset, value);
        sprintf(pcWriteBuffer, "[LPDDR4] Write value 0x%02X to add 0x%02X\r\n", value, (offset + LPDDR4_START_ADD));
    }
    else if ((*pcParameter[LPDDR4_ACCESS] == 'r') && (offset < (DDR4_SIZE - 4)) && (pcParameter[LPDDR4_VALUE] == NULL))
    {
        value = readLpddr4(offset);
        sprintf(pcWriteBuffer, "[LPDDR4] Read from add 0x%02X: 0x%02X\r\n", (offset + LPDDR4_START_ADD), value);
    }
    else
    {
        sprintf(pcWriteBuffer, "[LPDDR4] Wrong parameter.\r\n");
    }

    return pdFALSE;
}

/*[EOF]************************************************************************/
