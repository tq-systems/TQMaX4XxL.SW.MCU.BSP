/**
 * @file     spi_driver.c
 * copyright Copyright (c) 2023 TQ-Systems GmbH
 *           <license@tq-group.com>, D-82229 Seefeld, Germany.
 *           All rights reserved.
 * @author   M. Bernhardt
 * @date     2023-02-13
 *
 * This software code is dual licensed under the terms and conditions of
 * TQ-Systems Software License Agreement Version 1.0.3 or any later version
 * You may obtain a copy of the TQ-Systems Software License Agreements in the
 * folder TQS (TQ-Systems Software Licenses) at the following website:
 * https://www.tq-group.com/en/support/downloads/tq-software-license-conditions/
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

/* project */
#include <kernel/dpl/DebugP.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"

/* own */

/*******************************************************************************
 * local defines
 ******************************************************************************/



/*******************************************************************************
 * local macros
 ******************************************************************************/



/*******************************************************************************
 * local typedefs
 ******************************************************************************/

#define APP_MCSPI_MSGSIZE       (100U)

/*******************************************************************************
 * local static data
 ******************************************************************************/

uint8_t gMcspiTxBuffer[APP_MCSPI_MSGSIZE];
uint8_t gMcspiRxBuffer[APP_MCSPI_MSGSIZE];

/*******************************************************************************
 * forward declarations
 ******************************************************************************/



/*******************************************************************************
 * local static functions
 ******************************************************************************/



/*******************************************************************************
 * global functions
 ******************************************************************************/

void testSpi(void)
{
    int32_t             status = SystemP_SUCCESS;
    uint32_t            i;
    int32_t             transferOK;
    MCSPI_Transaction   spiTransaction;

    Drivers_open();
    Board_driversOpen();

    /* Memfill buffers */
    for (i = 0U; i < APP_MCSPI_MSGSIZE; i++)
    {
        gMcspiTxBuffer[i] = i;
        gMcspiRxBuffer[i] = 0U;
    }

    /* Initiate transfer */
    MCSPI_Transaction_init(&spiTransaction);
    spiTransaction.channel = gConfigMcspi0ChCfg[0].chNum;
    spiTransaction.dataSize = 8;
    spiTransaction.csDisable = TRUE;
    spiTransaction.count = APP_MCSPI_MSGSIZE / (spiTransaction.dataSize / 8);
    spiTransaction.txBuf = (void*) gMcspiTxBuffer;
    spiTransaction.rxBuf = (void*) gMcspiRxBuffer;
    spiTransaction.args = NULL;
    transferOK = MCSPI_transfer(gMcspiHandle[CONFIG_MCSPI0], &spiTransaction);
    if ((SystemP_SUCCESS != transferOK)
            || (MCSPI_TRANSFER_COMPLETED != spiTransaction.status))
    {
        DebugP_assert(FALSE); /* MCSPI transfer failed!! */
    }
    else
    {
        /* Compare data */
        for (i = 0U; i < APP_MCSPI_MSGSIZE; i++)
        {
            if (gMcspiTxBuffer[i] != gMcspiRxBuffer[i])
            {
                status = SystemP_FAILURE; /* Data mismatch */
                DebugP_log("Data Mismatch at offset %d\r\n", i);
                break;
            }
        }
    }

    if (SystemP_SUCCESS == status)
    {
        DebugP_log("All tests have passed!!\r\n");
    }
    else
    {
        DebugP_log("Some tests have failed!!\r\n");
    }

    Board_driversClose();
    Drivers_close();
}

/*[EOF]************************************************************************/
