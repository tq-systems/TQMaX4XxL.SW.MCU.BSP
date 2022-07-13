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
/* own */
#include "eth_cmd.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/

extern int enet_lwip_example(void *args);

/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "eth" command. */
const CLI_Command_Definition_t ethCommandDef =
{
    "eth",
    "\r\neth [eth]:\r\n TODO\r\n\r\n",
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

static bool test(void)
{
    bool success = false;
    ETHPHY_DP83869_LedSourceConfig  ledConfig = {0};
    int32_t status = SystemP_SUCCESS;

//    Board_ethPhyOpen();
//
//    /*Use CONFIG_ETHPHY0 macro as an index for the handle array*/
//    /* Select MII mode */
//    status = ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY0], ETHPHY_CMD_ENABLE_MII, NULL, 0);
//
//    /* PHY pin LED_0 as link for fast link detection */
//    ledConfig.ledNum = ETHPHY_DP83869_LED0;
//    ledConfig.mode = ETHPHY_DP83869_LED_MODE_LINK_OK;
//    status = ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY0], ETHPHY_CMD_CONFIGURE_LED_SOURCE, (void *)&ledConfig, sizeof(ledConfig));
//
//    /* PHY pin LED_1 as 1G link established */
//    ledConfig.ledNum = ETHPHY_DP83869_LED1;
//    ledConfig.mode = ETHPHY_DP83869_LED_MODE_1000BT_LINK_UP;
//    status = ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY0], ETHPHY_CMD_CONFIGURE_LED_SOURCE, (void *)&ledConfig, sizeof(ledConfig));
//
//    /* PHY pin LED_2 as Rx/Tx Activity */
//    ledConfig.ledNum = ETHPHY_DP83869_LED2;
//    ledConfig.mode = ETHPHY_DP83869_LED_MODE_LINK_OK_AND_BLINK_ON_RX_TX;
//    status = ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY0], ETHPHY_CMD_CONFIGURE_LED_SOURCE, (void *)&ledConfig, sizeof(ledConfig));
//
//    /* PHY pin LED_3 as 100M link established */
//    ledConfig.ledNum = ETHPHY_DP83869_LED_GPIO;
//    ledConfig.mode = ETHPHY_DP83869_LED_MODE_10_OR_100BT_LINK_UP;
//    status = ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY0], ETHPHY_CMD_CONFIGURE_LED_SOURCE, (void *)&ledConfig, sizeof(ledConfig));
//
//    Board_ethPhyClose();

    return success;
}

/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief This function handles the ethernet command
 *
 * @param pcWriteBuffer cli output string buffer
 * @param xWriteBufferLen length of the cli output string
 * @param pcCommandString cli command input string
 * @return pdFALSE = eth cmd is complete
 */
BaseType_t ethCommand( char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString )
{
//    test();
    enet_lwip_example(NULL);

    return pdFALSE;
}


/*[EOF]************************************************************************/
