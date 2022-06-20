/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file lpddr4_cmd.c
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2022-06-09
 *
 * -----------------------------------------------------------------------------
 * @brief <TODO short description of the file (only one line)>
 */

/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */
#include <stdbool.h>
#include <stdint.h>
/* project */
#include "projdefs.h"
#include <drivers/ddr.h>
//#include "drivers/ddr/v0/soc/am64x_am243x/board_lpddrReginit.h"
#include "drivers/board_ddrReginit.h"
/* own */
#include "lpddr4_cmd.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/



/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "ledblink" command. */
const CLI_Command_Definition_t lpddr4CommandDef =
{
    "lpddr4",
    "\r\nlpddr4 [lpddr4]:\r\n Read or write from/to the LPDDR4.\r\n\r\n",
    lpddr4Command,
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

static bool writeLpddr4(void)
{
    bool    success = false;
    int32_t status  = 0;

    static DDR_Params gDdrParams =
    {
        /* below values are set using the globals defined in drivers/ddr/v0/soc/am64x_am243x/board_ddrReginit.h */
        .clk1Freq              = DDRSS_PLL_FREQUENCY_1,
        .clk2Freq              = DDRSS_PLL_FREQUENCY_2,
        .ddrssCtlReg           = DDRSS_ctlReg,
        .ddrssPhyIndepReg      = DDRSS_phyIndepReg,
        .ddrssPhyReg           = DDRSS_phyReg,
        .ddrssCtlRegNum        = DDRSS_ctlRegNum,
        .ddrssPhyIndepRegNum   = DDRSS_phyIndepRegNum,
        .ddrssPhyRegNum        = DDRSS_phyRegNum,
        .ddrssCtlRegCount      = DDRSS_CTL_REG_INIT_COUNT,
        .ddrssPhyIndepRegCount = DDRSS_PHY_INDEP_REG_INIT_COUNT,
        .ddrssPhyRegCount      = DDRSS_PHY_REG_INIT_COUNT,
        .fshcount              = DDRSS_PLL_FHS_CNT,
    };

    status = DDR_init(&gDdrParams);

    if (status == SystemP_SUCCESS)
    {
        DebugP_log("[LPDDR4] init success.\r\n");
        success = true;
    }
    else
    {
        DebugP_logError("[LPDDR4] Init failure %d!!!\r\n", status);
    }

    return success;
}

/*******************************************************************************
 * global functions
 ******************************************************************************/

BaseType_t lpddr4Command(char* pcWriteBuffer, __size_t xWriteBufferLen, const char* pcCommandString)
{
    writeLpddr4();

    return pdFALSE;
}

/*[EOF]************************************************************************/
