/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file eeprom_cmd.c
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2022-05-18
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the implementation of the EEPROM M24C64.
 *
 */
/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
/* project */
#include "projdefs.h"
#include <drivers/i2c.h>
#include <kernel/dpl/DebugP.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "utilities.h"
/* own */
#include "eeprom_cmd.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/

#define DECIMAL_BASE       (10U)
#define HEX_BASE           (16U)
#define MAX_INPUT_PARAM    (4U)
#define EEPROM_PAGE_SIZE   (256U)
#define EEPROM_WR_BUF_SIZE (2U + EEPROM_PAGE_SIZE)
#define INDEX_INSTANCE     (0U)
#define INDEX_OFFSET       (1U)
#define INDEX_ACCESS       (2U)
#define INDEX_DATA         (3U)

#define TQ_EEPROM_ADD         (0x50)
#define TQ_EEPROM_INSTANCE     (0U)
#define TQ_EEPROM_ADD_SIZE    (1U)
#define CUST_EEPROM_ADD       (0x54)
#define CUST_EEPROM_INSTANCE   (1U)
#define CUST_EEPROM_ADD_SIZE  (2U)
#define MAX_ADDR_SIZE         (CUST_EEPROM_ADD_SIZE)
#define MAX_DATA              (1U)

/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "EEPROM" command. */
const CLI_Command_Definition_t eepromCommandDef =
{
    "eeprom",
    "\r\neeprom [I2c]:\r\n Read or write from/to the EEPROM.\r\n"
    " usage: usage: eeprom [i] [o] [a] [d]\r\n"
    " i - instance: 0 - TQ_EEPROM_ADD, 1 - CUST_EEPROM_ADD\r\n"
    " o - offset in hex\r\n"
    " a - access: w - write, r -read\r\n"
    " d - data in hex\r\n\r\n",
    eepromCommand,
    -1
};

/*******************************************************************************
 * local typedefs
 ******************************************************************************/

typedef struct
{
    uint8_t instance;
    uint32_t offset;
    uint8_t addressSize;
    uint8_t* data;
    uint8_t length;
} eeprom_t;

/*******************************************************************************
 * local static data
 ******************************************************************************/

static const uint8_t eepromInstance[] = {TQ_EEPROM_ADD, CUST_EEPROM_ADD};

/*******************************************************************************
 * forward declarations
 ******************************************************************************/

static bool eepromRead(eeprom_t* p_eepromData);
static bool eepromWrite(eeprom_t* eepromData);

/*******************************************************************************
 * local static functions
 ******************************************************************************/

/**
 * @brief This function reads data from the EEPROM.
 *
 * @param p_eepromData Pointer to EEPROM data struct.
 * @return success
 */
static bool eepromRead(eeprom_t* p_eepromData)
{
    bool            success        = false;
    int32_t         status         = 0;
    I2C_Handle      i2cHandle      = NULL;
    I2C_Transaction i2cTransaction = {0};
    uint8_t         txBuffer[2]    = {0};

    DebugP_log("[I2C] Read EEPROM instance: %d.\r\n", p_eepromData->instance);

    if ((p_eepromData->instance < ARRAY_SIZE(eepromInstance)) && (p_eepromData->data != NULL))
    {
        txBuffer[0U] = (p_eepromData->offset & 0xFF00U) >> 8U;
        txBuffer[1U] = (p_eepromData->offset & 0x00FFU);

        Drivers_i2cOpen();

        i2cHandle = I2C_getHandle(CONFIG_I2C0);

        /* Set default transaction parameters */
        I2C_Transaction_init(&i2cTransaction);

        /* Override with required transaction parameters */

        i2cTransaction.writeCount    = p_eepromData->addressSize;
        i2cTransaction.writeBuf      = &txBuffer[MAX_ADDR_SIZE-p_eepromData->addressSize];

        i2cTransaction.readBuf       = p_eepromData->data;
        i2cTransaction.readCount     = p_eepromData->length;
        i2cTransaction.targetAddress = eepromInstance[p_eepromData->instance];

        status = I2C_transfer(i2cHandle, &i2cTransaction);

        if (status != SystemP_SUCCESS)
        {
            DebugP_logError("[EEPROM] Read failed %d for instance %d !!!\r\n", status, p_eepromData->instance);
            success = false;
        }
        else
        {
            DebugP_log("[EEPROM] Read data 0x%02X.\r\n", *p_eepromData->data);
            success = true;
        }

        Drivers_i2cClose();
    }

    return success;
}

/**
 * @brief This function writes data to the EEPROM.
 *
 * @param p_eepromData Pointer to EEPROM data struct.
 * @return success
 */
static bool eepromWrite(eeprom_t* p_eepromData)
{
    bool            success                      = false;
    int32_t         status                       = 0;
    I2C_Handle      i2cHandle                    = NULL;
    I2C_Transaction i2cTransaction               = {0};
    uint8_t         txBuffer[EEPROM_WR_BUF_SIZE] = {0};


    if ((p_eepromData->instance < ARRAY_SIZE(eepromInstance)) && (p_eepromData->data != NULL))
    {
        txBuffer[0U] = (p_eepromData->offset & 0xFF00U) >> 8U;
        txBuffer[1U] = (p_eepromData->offset & 0x00FFU);

        memcpy(&txBuffer[2U], p_eepromData->data, p_eepromData->length);

        Drivers_i2cOpen();
        if (status == SystemP_SUCCESS)
        {
            i2cHandle = I2C_getHandle(CONFIG_I2C0);

            /* Set default transaction parameters */
            I2C_Transaction_init(&i2cTransaction);

            /* Override with required transaction parameters */
            i2cTransaction.targetAddress = eepromInstance[p_eepromData->instance];
            i2cTransaction.writeBuf     = &txBuffer[MAX_ADDR_SIZE-p_eepromData->addressSize];
            i2cTransaction.writeCount   = p_eepromData->addressSize + p_eepromData->length;

            status = I2C_transfer(i2cHandle, &i2cTransaction);
        }

        if (status != SystemP_SUCCESS)
        {
            DebugP_logError("[EEPROM] Write failure %d for instance %d !!!\r\n", status, i2cTransaction.targetAddress);
        }
        else
        {
            DebugP_log("[EEPROM] write data 0x%02X.\r\n", *p_eepromData->data);
            success = true;
        }

        Drivers_i2cClose();
    }

    return success;
}

/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief This function handles the command of the EEPROM access. One byte can be read or written to the EEPROM.
 *
 * @param pcWriteBuffer cli output string buffer
 * @param xWriteBufferLen length of the cli output string
 * @param pcCommandString cli command input string
 * @return pdFALSE = command is finished
 */
BaseType_t eepromCommand(char* pcWriteBuffer, __size_t xWriteBufferLen, const char* pcCommandString)
{
    uint8_t    paramCount                   = 0;
    eeprom_t   eepromData                   = {0};
    bool       err                          = false;
    char*      pNextNumber                  = NULL;
    char*      pcParameter[MAX_INPUT_PARAM] = {NULL};
    BaseType_t xParameterStringLength       = 0;
    uint8_t    dataBuf                      = 0;

    /* default EEPROM data setting */
    eepromData.data   = &dataBuf;
    eepromData.length = MAX_DATA;

    /* get all parameter */
    for (paramCount = 0; paramCount < MAX_INPUT_PARAM; paramCount++)
    {
        pcParameter[paramCount] = (char*) FreeRTOS_CLIGetParameter(pcCommandString, paramCount+1, &xParameterStringLength);
    }

    eepromData.instance = strtoul(pcParameter[INDEX_INSTANCE], &pNextNumber, DECIMAL_BASE);
    if ((pcParameter[INDEX_INSTANCE] == pNextNumber) || (eepromData.instance >= ARRAY_SIZE(eepromInstance)))
    {
        err = true;
    }

    eepromData.offset = strtoul(pcParameter[INDEX_OFFSET], &pNextNumber, HEX_BASE);
    if (pcParameter[INDEX_OFFSET] == pNextNumber)
    {
        err = true;
    }

    switch(eepromData.instance){
    case TQ_EEPROM_INSTANCE:
         eepromData.addressSize = TQ_EEPROM_ADD_SIZE;
         break;
    case CUST_EEPROM_INSTANCE:
        eepromData.addressSize = CUST_EEPROM_ADD_SIZE;
        break;
    default:
        err = true;
        break;
    }

    if (err == false)
    {
        switch (*pcParameter[INDEX_ACCESS])
        {
        case 'r':
            if (eepromRead(&eepromData) == true)
            {
                sprintf(pcWriteBuffer, "EEPROM data 0x%02X\r\n", *eepromData.data);
            }
            else
            {
                sprintf(pcWriteBuffer, "EEPROM read failure\r\n");
            }
            break;

        case 'w':
            *eepromData.data   = strtoul(pcParameter[INDEX_DATA], &pNextNumber, HEX_BASE);

            if ((pcParameter[INDEX_OFFSET] == pNextNumber) || (eepromWrite(&eepromData) == false))
            {
                sprintf(pcWriteBuffer, "EEPROM write failure\r\n");
            }
            else
            {
                sprintf(pcWriteBuffer, "EEPROM write 0x%02X\r\n", *eepromData.data);
            }
            break;

        default:
            sprintf(pcWriteBuffer, "EEPROM wrong parameter\r\n");
            break;
        }
    }
    else
    {
        sprintf(pcWriteBuffer, "EEPROM wrong parameter\r\n");
    }

    return pdFALSE;
}


/*[EOF]************************************************************************/
