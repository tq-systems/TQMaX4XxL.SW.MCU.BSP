/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file eth_cmd.c
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2022-07-04
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the implementation of the ethernet commands.
 *
 */

/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */
#include <stdbool.h>
/* project */
#include <board/ethphy.h>
#include "board/ethphy/ethphy_dp83869.h"
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
//#include "lwip/netif.h"
/* own */
#include "eth_cmd.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/

extern int enet_lwip_example(void* args);

/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "eth" command. */
const CLI_Command_Definition_t ethCommandDef =
{
    "eth",
    "\r\neth [eth]:\r\n Returns the current IP address.\r\n\r\n",
    ethCommand,
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



/*******************************************************************************
 * local static functions
 ******************************************************************************/



/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief This function handles the ethernet command.
 *
 * @param pcWriteBuffer cli output string buffer
 * @param xWriteBufferLen length of the cli output string
 * @param pcCommandString cli command input string
 * @return pdFALSE = eth cmd is complete
 */
BaseType_t ethCommand(char* pcWriteBuffer, __size_t xWriteBufferLen, const char* pcCommandString)
{
//    sprintf(pcWriteBuffer, "IP address %s\r\n", ip4addr_ntoa(netif_ip4_addr(netif_list)));

    return pdFALSE;
}

/**
 * @brief This function handles the ethernet task and start the eth example.
 *
 * @param args unused
 */
void ethTask(void* args)
{
    enet_lwip_example(NULL);
}

/*[EOF]************************************************************************/
