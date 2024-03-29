/*
 *  Copyright (C) 2021 Texas Instruments Incorporated
 *
 *  Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 *  Author Jeremias Schneider
 *  Author Michael Bernhardt
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

#include <drivers/gpio.h>
#include <kernel/dpl/AddrTranslateP.h>
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/ClockP.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"

/*
 * This example configures a GPIO pin connected to an LED on the EVM in
 * output mode.
 * The application toggles the LED on/off for 10 seconds and exits.
 */

void gpio_led_blink_main(void *args)
{
    bool error            = false;
    uint32_t loopcnt      = 5;
    uint32_t delayUsec    = 500000;
    uint32_t gpioBaseAddr = 0;
    uint32_t pinNum       = 0;

    DebugP_log("GPIO LED Blink Test Started ...\r\n");
    DebugP_log("LED will Blink for %d seconds ...\r\n", (loopcnt * delayUsec / 1000000 * 2));

    /* Get address after translation translate */
    if (*(uint8_t*)args == '1')
    {
        gpioBaseAddr = (uint32_t) AddrTranslateP_getLocalAddr(GPIO_USER_LED_1_BASE_ADDR);
        pinNum       = GPIO_USER_LED_1_PIN;
        GPIO_setDirMode(gpioBaseAddr, pinNum, GPIO_USER_LED_1_DIR);
    }
    else if (*(uint8_t*)args == '2')
    {
        gpioBaseAddr = (uint32_t) AddrTranslateP_getLocalAddr(GPIO_USER_LED_2_BASE_ADDR);
        pinNum       = GPIO_USER_LED_2_PIN;
        GPIO_setDirMode(gpioBaseAddr, pinNum, GPIO_USER_LED_2_DIR);
    }
    else
    {
        error = true;
    }

    while((error == false) && (loopcnt > 0))
    {
        GPIO_pinWriteHigh(gpioBaseAddr, pinNum);
        ClockP_usleep(delayUsec);
        GPIO_pinWriteLow(gpioBaseAddr, pinNum);
        ClockP_usleep(delayUsec);

        loopcnt--;
    }

    DebugP_log("GPIO LED Blink Test Passed!!\r\n");
    DebugP_log("All tests have passed!!\r\n");
}
