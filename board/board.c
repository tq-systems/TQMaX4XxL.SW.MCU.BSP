/*
 *  Copyright (C) 2021 Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <drivers/pinmux.h>
#include "ti_board_config.h"
#include <networking/enet/core/include/phy/dp83867.h>

#define TMP100_SOC_ADDR         0x4A
/* The delay values are set based on trial and error and not tuned per port of the evm */
#define TX_DELAY  250U
#define RX_DELAY  2000U

static Pinmux_PerCfg_t MDIOPinMuxMainDomainCfg[] = {
    /* MDIO0 pin config */
    /* MDIO0_MDC -> PRG0_PRU1_GPO19 (R2) */
    {
        PIN_PRG0_PRU1_GPO19,
        ( PIN_MODE(4) | PIN_PULL_DISABLE )
    },
    /* MDIO0_MDIO -> PRG0_PRU1_GPO18 (P5) */
    {
        PIN_PRG0_PRU1_GPO18,
        ( PIN_MODE(4) | PIN_INPUT_ENABLE | PIN_PULL_DISABLE )
    },

    {PINMUX_END, PINMUX_END}
};

/**
 * @brief This function returns the I2C temperatur sensor address.
 *
 * @return temperature sensor address.
 */
uint8_t Board_getSocTemperatureSensorAddr(void)
{
    return (TMP100_SOC_ADDR);
}
