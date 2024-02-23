/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file mcan_cmd.c
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2022-06-21
 *
 * -----------------------------------------------------------------------------
 * @brief This function contains the implementation of the mcan bus command.
 *
 */

/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */
#include <stdio.h>
/* project */
#include "ti_drivers_config.h"
#include <drivers/mcan.h>
/* own */
#include "mcan_cmd.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/


/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "mcan" command. */
const CLI_Command_Definition_t mcanCommandDef =
{
    "mcan",
    "\r\nmcan [MCAN]:\r\n Sends or receives CAN messages.\n Usage mcan [c] [v] \n c:\n  0 - CAN 0\n  1 - CAN 1\n v:\n  0 - send 10 CAN msg\n  1 - receive and print one CAN msg \r\r\n\r\n",
    mcanCommand,
    2
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

extern int32_t mcan_rx_interrupt_main(void *args, uint64_t mcanAdd);
extern int32_t mcan_tx_interrupt_main(void *args, uint64_t mcanAdd);

/*******************************************************************************
 * local static functions
 ******************************************************************************/



/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief This function handles the MCAN command
 *
 * @param pcWriteBuffer cli output string buffer
 * @param xWriteBufferLen length of the cli output string
 * @param pcCommandString cli command input string
 * @return pdFALSE = mcan cmd is complete
 */
BaseType_t mcanCommand(char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString)
{
    uint8_t           i                      = 0;
    int32_t           counter                = 0;
    int32_t           status                 = SystemP_SUCCESS;
    BaseType_t        xParameterStringLength = 0;
    uint32_t          usedCanAdd             = 0;
    MCAN_RxBufElement rxMsg                  = {0};
    MCAN_TxBufElement txMsg                  = {0};
    bool              err                    = false;
    const char*       pcParameter1           = FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameterStringLength);
    const char*       pcParameter2           = FreeRTOS_CLIGetParameter(pcCommandString, 2, &xParameterStringLength);

    if (*pcParameter1 == '0')
    {
        usedCanAdd = CONFIG_MCAN0_BASE_ADDR;
    }
    else if (*pcParameter1 == '1')
    {
        usedCanAdd = CONFIG_MCAN1_BASE_ADDR;
    }
    else
    {
        sprintf(pcWriteBuffer, "wrong parameter \r\n");
        err = true;
    }

    if (err == false)
    {
        switch (*pcParameter2)
        {
        case '0':
            status = mcan_tx_interrupt_main(&txMsg, usedCanAdd);

            if (SystemP_SUCCESS == status)
            {
                counter = sprintf(pcWriteBuffer, "send: ");
                for (i = 0; i < txMsg.dlc; i++)
                {
                    counter += sprintf(&pcWriteBuffer[counter], "%d ", txMsg.data[i]);
                }

                counter += sprintf(&pcWriteBuffer[counter], "\r\n");
            }
            else
            {
                counter += sprintf(&pcWriteBuffer[counter], "[MCAN] Failure %i\r\n", status);
            }
            break;

        case '1':
            status = mcan_rx_interrupt_main(&rxMsg, usedCanAdd);
            if (SystemP_SUCCESS == status)
            {
                counter =  sprintf(pcWriteBuffer, "received: ");
                for (i = 0; i < rxMsg.dlc; i++)
                {
                    counter += sprintf(&pcWriteBuffer[counter], "%d ", rxMsg.data[i]);
                }

                counter += sprintf(&pcWriteBuffer[counter], "\r\n");
            }
            else
            {
                counter += sprintf(&pcWriteBuffer[counter], "[MCAN] Failure %i\r\n", status);
            }

            break;

        default:
            sprintf(pcWriteBuffer, "wrong parameter \r\n");
            break;
        }
    }

    return pdFALSE;
}

/*[EOF]************************************************************************/
