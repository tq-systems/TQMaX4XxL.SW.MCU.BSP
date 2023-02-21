/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file afe_registers.c
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2023-02-17
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the SPI driver implementation.
 *
 */

/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */

/* project */
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
/* own */
#include "spiDriver.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/



/*******************************************************************************
 * local macros
 ******************************************************************************/



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
 * @brief This function transmit and receive data from SPI bus.
 *
 * @param p_txBuf Pointer to the transmit buffer, must be large enough for tx and rx data
 * @param p_rxBuf Pointer to the receive buffer, must be large enough for tx and rx data
 * @param size data size in byte
 * @return true = success
 */
bool spi_transmit(uint8_t* p_txBuf, uint8_t* p_rxBuf, uint8_t size)
{
    MCSPI_Transaction   spiTransaction = {0};
    int32_t             transferOK     = SystemP_SUCCESS;
    bool                retVal         = true;

    Drivers_mcspiOpen();

    MCSPI_Transaction_init(&spiTransaction);
    spiTransaction.channel   = gConfigMcspi0ChCfg[0].chNum;
    spiTransaction.csDisable = TRUE;
    spiTransaction.txBuf     = (void*) p_txBuf;
    spiTransaction.rxBuf     = (void*) p_rxBuf;
    spiTransaction.args      = NULL;
    spiTransaction.dataSize  = 8;       /* transmit 8 bits per frame count */
    spiTransaction.count     = size;    /* frame counter */

    transferOK = MCSPI_transfer(gMcspiHandle[CONFIG_MCSPI0], &spiTransaction);

    if ((SystemP_SUCCESS != transferOK) || (MCSPI_TRANSFER_COMPLETED != spiTransaction.status))
    {
        retVal = false;
    }

    Drivers_mcspiClose();

    return retVal;
}

/*[EOF]************************************************************************/
