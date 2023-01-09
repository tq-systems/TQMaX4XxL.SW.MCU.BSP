/*
 *  Copyright (C) 2021 Texas Instruments Incorporated
 *
 *  Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
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

/* This example demonstrates the UART RX and TX operation by echoing char
 * that it recieves in blocking, interrupt mode of operation.
 * When user types 'quit', the application ends.
 */

#include <string.h>
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/AddrTranslateP.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
#include "ti_drivers_config.h"
#include "FreeRTOS.h"
#include "task.h"

#define APP_UART_BUFSIZE              (200U)
#define APP_UART_RECEIVE_BUFSIZE      (8U)
#define DELAY_TIME                    (pdMS_TO_TICKS(6))

uint8_t gUartBuffer[APP_UART_BUFSIZE];
uint8_t gUartReceiveBuffer[APP_UART_RECEIVE_BUFSIZE];
volatile uint32_t gNumBytesRead = 0U, gNumBytesWritten = 0U;

/* Semaphore to indicate Write/Read completion used in callback api's */
static SemaphoreP_Object gUartWriteDoneSem;
static SemaphoreP_Object gUartReadDoneSem;


typedef struct
{
    uint32_t    baseAdd;
    uint32_t    pin;
    uint32_t    direction;
} gpio_t;

gpio_t rs485Rts;
bool rtsFlag = false;

#define APP_UART_ASSERT_ON_FAILURE(transferOK, transaction) \
    do { \
        if((SystemP_SUCCESS != (transferOK)) || (UART_TRANSFER_STATUS_SUCCESS != transaction.status)) \
        { \
            DebugP_assert(FALSE); /* UART TX/RX failed!! */ \
        } \
    } while(0) \

void uart_echo(void *args)
{
    int32_t          transferOK;
    int32_t          status;
    UART_Transaction trans;

    rs485Rts.baseAdd   = (uint32_t) AddrTranslateP_getLocalAddr(RS485_RTS_BASE_ADDR);
    rs485Rts.pin       = RS485_RTS_PIN;
    rs485Rts.direction = RS485_RTS_DIR;

    status = SemaphoreP_constructBinary(&gUartWriteDoneSem, 0);
    DebugP_assert(SystemP_SUCCESS == status);

    status = SemaphoreP_constructBinary(&gUartReadDoneSem, 0);
    DebugP_assert(SystemP_SUCCESS == status);

    Board_driversOpen();

    DebugP_log("[UART] Echo example started ...\r\n");

    UART_Transaction_init(&trans);
    GPIO_setDirMode(rs485Rts.baseAdd, rs485Rts.pin, rs485Rts.direction);

    /* Send entry string */
    gNumBytesWritten = 0U;
    trans.buf   = &gUartBuffer[0U];
    strncpy(trans.buf,"This is uart echo test callback mode\r\nReceives 8 characters then echo's back. Please input..\r\n", APP_UART_BUFSIZE);
    trans.count = strlen(trans.buf);

    GPIO_pinWriteHigh(rs485Rts.baseAdd, rs485Rts.pin);
    rtsFlag = true;
    transferOK = UART_write(gUartHandle[RS485], &trans);
    APP_UART_ASSERT_ON_FAILURE(transferOK, trans);

    /* Wait for write completion */
    SemaphoreP_pend(&gUartWriteDoneSem, SystemP_WAIT_FOREVER);
    DebugP_assert(gNumBytesWritten == strlen(trans.buf));
    vTaskDelay(DELAY_TIME);
    GPIO_pinWriteLow(rs485Rts.baseAdd, rs485Rts.pin);


    /* Read 8 chars */
    gNumBytesRead = 0U;
    trans.buf   = &gUartReceiveBuffer[0U];
    trans.count = APP_UART_RECEIVE_BUFSIZE;
    transferOK = UART_read(gUartHandle[RS485], &trans);
    APP_UART_ASSERT_ON_FAILURE(transferOK, trans);

    /* Wait for read completion */
    SemaphoreP_pend(&gUartReadDoneSem, SystemP_WAIT_FOREVER);
    DebugP_assert(gNumBytesRead == APP_UART_RECEIVE_BUFSIZE);

    /* Echo chars entered */
    gNumBytesWritten = 0U;
    trans.buf   = &gUartReceiveBuffer[0U];
    trans.count = APP_UART_RECEIVE_BUFSIZE;
    GPIO_pinWriteHigh(rs485Rts.baseAdd, rs485Rts.pin);
    transferOK = UART_write(gUartHandle[RS485], &trans);
    APP_UART_ASSERT_ON_FAILURE(transferOK, trans);

    /* Wait for write completion */
    SemaphoreP_pend(&gUartWriteDoneSem, SystemP_WAIT_FOREVER);
    DebugP_assert(gNumBytesWritten == APP_UART_RECEIVE_BUFSIZE);
    vTaskDelay(DELAY_TIME);
    GPIO_pinWriteLow(rs485Rts.baseAdd, rs485Rts.pin);

    /* Send exit string */
    gNumBytesWritten = 0U;
    trans.buf   = &gUartBuffer[0U];
    strncpy(trans.buf, "\r\nAll tests have passed!!\r\n", APP_UART_BUFSIZE);
    trans.count = strlen(trans.buf);
    GPIO_pinWriteHigh(rs485Rts.baseAdd, rs485Rts.pin);
    transferOK = UART_write(gUartHandle[RS485], &trans);
    APP_UART_ASSERT_ON_FAILURE(transferOK, trans);

    /* Wait for write completion */
    SemaphoreP_pend(&gUartWriteDoneSem, SystemP_WAIT_FOREVER);
    DebugP_assert(gNumBytesWritten == strlen(trans.buf));
    vTaskDelay(6);
    GPIO_pinWriteLow(rs485Rts.baseAdd, rs485Rts.pin);

    DebugP_log("All tests have passed!!\r\n");

    SemaphoreP_destruct(&gUartWriteDoneSem);
    SemaphoreP_destruct(&gUartReadDoneSem);

    Board_driversClose();

    return;
}

void uart_echo_write_callback(UART_Handle handle, UART_Transaction* trans)
{
    DebugP_assertNoLog(UART_TRANSFER_STATUS_SUCCESS == trans->status);
    gNumBytesWritten = trans->count;
    SemaphoreP_post(&gUartWriteDoneSem);

    return;
}

void uart_echo_read_callback(UART_Handle handle, UART_Transaction *trans)
{
    DebugP_assertNoLog(UART_TRANSFER_STATUS_SUCCESS == trans->status);
    gNumBytesRead = trans->count;
    SemaphoreP_post(&gUartReadDoneSem);

    return;
}
