/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file afe_registers.c
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Bernhard Herz
 * @author Michael Bernhardt
 *
 * @date 2021-10-11
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the implementation of the register access.
 *
 */


/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */

/* project */

/* module */

/* own */
#include "afe_registers.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/


/*******************************************************************************
 * local macros
 ******************************************************************************/

CHANNEL_TypDef_t CHANNEL_REG;

/*******************************************************************************
 * local typedefs
 ******************************************************************************/
/*Init LOGIC Channel Registers with static Register-Address*/
const CH_CONFIG_t LOGIC_CH_CONTROL =
{
        .CH_CONFIG0 = 0x0020,
        .CH_CONFIG1 = 0x0021,
        .CH_CONFIG2 = 0x0022,
        .CH_CONFIG3 = 0x0023,
        .CH_CONFIG4 = 0x0024,
        .CH_STATUS0 = 0x0035,               /*Read only Register*/
        .CH_STATUS1 = 0x0036                /*Read only Register*/
};

/*Init GPIO Registers with static Register-Address*/
const GPIO_REG_t GPIO_CONTROL =
{
        .GPI_DATA     = 0x0029,            /*Read only Register*/
        .GPIO_CONFIG0 = 0x002A,
        .GPIO_CONFIG1 = 0x002B,
        .GPIO_CONFIG2 = 0x002C,
        .GPI_EDGE_POS = 0x002D,             /*Read only Register*/
        .GPI_EDGE_NEG = 0x002E,             /*Read only Register*/
        .GPO_DATA = 0x002F
};

/*Init System Registers with static Register-Address*/
const SYS_REG_t SYS_CONTROL =
{
        .SYS_CONFIG0 = 0x0030,
        .SYS_STATUS0 = 0x0031                 /*Read only Register*/
};

/*Init ALARM and TEMPERATURE Registers with static Register-Address*/
const MONITORING_REG_t ALARM_CONTROL =
{
        .GLOBAL_ALARM_ENABLE = 0x0032,
        .GLOBAL_ALARM_INTERRUPT = 0x0033,
        .DIE_TEMP = 0x0034,                   /*Read only Register*/
        .THRS_TEMP = 0x0037
};


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
 * @brief Initalizes Channel Register Struct with Register-Addresses
 *
 */
void Init_CH_RegisterAdr(void)
{
    uint8_t i = 0;

    for (i = 0; i < 16; i++)
    {
        CHANNEL_REG.DATA[i]        = 0x40 + i;
        CHANNEL_REG.OVR_THRS[i]    = 0x50 + i;
        CHANNEL_REG.UDR_THRS[i]    = 0x60 + i;
        CHANNEL_REG.GAIN_COEF[i]   = 0x80 + i;
        CHANNEL_REG.OFFSET_COEF[i] = 0x90 + i;
        CHANNEL_REG.OPT_COEF[i]    = 0xA0 + i;
    }
}


/*[EOF]************************************************************************/

