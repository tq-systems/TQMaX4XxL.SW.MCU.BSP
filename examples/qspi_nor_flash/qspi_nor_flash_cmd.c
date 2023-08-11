/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file qspi_nor_flash_cmd.c
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2022-05-23
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the implementation of the QSPI NOR flash (MX25U51245G) commands.
 *
 */

/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
/* project */
#include <kernel/dpl/DebugP.h>
#include <drivers/ospi.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
#include "FreeRTOS.h"
#include "task.h"
#include "utilities.h"
/* own */
#include "qspi_nor_flash_cmd.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/



/*******************************************************************************
 * local macros
 ******************************************************************************/

/* The definition of the "qspiflash" command. */
const CLI_Command_Definition_t qspiNorFlashCommandDef =
{
    "qspiflash",
    "\r\nqspiflash:\r\n NOR flash access\r\n r - read data, w - write data, e - erase flash, s - read status register\r\n\r\n",
    qspiNorFlashCommand,
    1
};

/*******************************************************************************
 * local typedefs
 ******************************************************************************/

/* Commands which require tPSL/tESL delay, see data sheet of MX25U51245G */
#define CMD_READ       (0x03)
#define CMD_FAST_READ  (0x0B)
#define CMD_2READ      (0xBB)
#define CMD_DREAD      (0x3B)
#define CMD_4READ      (0xEB)
#define CMD_QREAD      (0x6B)
#define CMD_4READ4B    (0xEC)
#define CMD_4DTRD      (0xED)
#define CMD_4DTRD4B    (0xEE)
#define CMD_FASTREAD4B (0x0C)
#define CMD_2READ4B    (0xBC)
#define CMD_DREAD4B    (0x3C)
#define CMD_RDSFDP     (0x5A)
#define CMD_RDID       (0x9F)
#define CMD_QPIID      (0xAF)
#define CMD_SBL        (0xC0)
#define CMD_ENSO       (0xB1)
#define CMD_EXSO       (0xC1)
#define CMD_WREN       (0x06)
#define CMD_RESUME     (0x30)
#define CMD_RDLR       (0x2D)
#define CMD_RDSPB      (0xE2)
#define CMD_RDFBR      (0x16)
#define CMD_RDDPB      (0xE0)
#define CMD_EQIO       (0x35)
#define CMD_RSTQIO     (0xF5)

/* Commands not required tPSL/tESL delay, see data sheet of MX25U51245G */
#define CMD_WRDI       (0x04)
#define CMD_RDSR       (0x05)
#define CMD_RDCR       (0x15)
#define CMD_RDSCUR     (0x2B)
#define CMD_RES        (0xAB)
#define CMD_REMS       (0x90)
#define CMD_RSTEN      (0x66)
#define CMD_RST        (0x99)
#define CMD_NOP        (0x00)

#define CMD_PP         (0x02)
#define CMD_4PP        (0x38)
#define CMD_SE         (0x20)
#define CMD_BE_32K     (0x52)
#define CMD_BE         (0xD8)
#define CMD_CE         (0x60)

#define OSPI_NOR_WRR_WRITE_TIMEOUT  pdMS_TO_TICKS(1000U)
#define OSPI_NOR_PAGE_PROG_TIMEOUT  pdMS_TO_TICKS(400U)
#define OSPI_NOR_SR_WIP             (1U)
#define FLASH_ADD                   (0U)

/*******************************************************************************
 * local static data
 ******************************************************************************/



/*******************************************************************************
 * forward declarations
 ******************************************************************************/

static void writeFlash(char* const pcWriteBuffer);
static int32_t readStatusReg(uint8_t* const p_norFlashStatus);
static void readFlash(char* const pcWriteBuffer);
static void eraseflash(char* const pcWriteBuffer);
static int32_t norFlashWaitReady(uint32_t timeout);

/*******************************************************************************
 * local static functions
 ******************************************************************************/

/**
 * @brief This function waits until the NOR Flash is ready
 *
 * @param timeOut function time out in ms
 * @return SystemP_SUCCESS: ready, SystemP_FAILURE: failure
 */
static int32_t norFlashWaitReady(uint32_t timeout)
{
    int32_t status = SystemP_SUCCESS;
    uint8_t qspiStatus = 0;

    while((status == SystemP_SUCCESS) && (timeout > 0) && ((qspiStatus & OSPI_NOR_SR_WIP) != 0))
    {
        status = readStatusReg(&qspiStatus);

        vTaskDelay(1);
        timeout--;
    }

    if ((qspiStatus & OSPI_NOR_SR_WIP) == 0)
    {
        status = SystemP_SUCCESS;
    }
    else
    {
        status = SystemP_FAILURE;
    }

    return status;
}

/**
 * @brief This function writes data (0, 1, 2, 3, 4) to the flash address 0.
 *
 * @param pcWriteBuffer FreeRTOS write buffer
 */
static void writeFlash(char* const pcWriteBuffer)
{
    int32_t status               = SystemP_SUCCESS;
    OSPI_Handle qspiHandle       = {0};
    uint8_t buf[5]               = {0, 1, 2, 3, 4};
    OSPI_WriteCmdParams wrParams = {0};

    Drivers_ospiOpen();

    status = Board_driversOpen();

    if (status == SystemP_SUCCESS)
    {
        qspiHandle = OSPI_getHandle(CONFIG_OSPI0);

        OSPI_WriteCmdParams_init(&wrParams);

        wrParams.cmd = CMD_WREN;
        status = OSPI_writeCmd(qspiHandle, &wrParams);
    }

    if (status == SystemP_SUCCESS)
    {
        status = norFlashWaitReady(OSPI_NOR_WRR_WRITE_TIMEOUT);
    }

    if (status == SystemP_SUCCESS)
    {
        wrParams.cmdAddr      = FLASH_ADD;
        wrParams.cmd          = CMD_PP;
        wrParams.txDataBuf    = buf;
        wrParams.txDataLen    = ARRAY_SIZE(buf);
        status = OSPI_writeCmd(qspiHandle, &wrParams);
    }

    if (status == SystemP_SUCCESS)
    {
        status = norFlashWaitReady(OSPI_NOR_PAGE_PROG_TIMEOUT);
    }

    if (status == SystemP_SUCCESS)
    {
        sprintf(pcWriteBuffer, "Data write %02X %02X %02X %02X %02X.\r\n", buf[0], buf[1], buf[2], buf[3] , buf[4]);
        DebugP_log("[QSPI] Data write %02X %02X %02X %02X %02X.\r\n", buf[0], buf[1], buf[2], buf[3] , buf[4]);
    }
    else
    {
        sprintf(pcWriteBuffer, "Error write flash: %d.\r\n", status);
        DebugP_log("[QSPI] Error write flash: %d.\r\n", status);
    }

    Board_driversClose();
    Drivers_ospiClose();
}

/**
 * @brief This function reads the NOR flash status register
 *
 * @param p_norFlashStatus Pointer to NOR flash status data
 * @return QSPI API status
 */
static int32_t readStatusReg(uint8_t* const p_norFlashStatus)
{
    int32_t status               = SystemP_SUCCESS;
    OSPI_Handle qspiHandle       = {0};
    OSPI_ReadCmdParams rdParams  = {0};
    OSPI_WriteCmdParams wrParams = {0};

    Drivers_ospiOpen();

    status = Board_driversOpen();

    if (status == SystemP_SUCCESS)
    {
        qspiHandle = OSPI_getHandle(CONFIG_OSPI0);

        OSPI_WriteCmdParams_init(&wrParams);

        wrParams.cmd = CMD_WREN;
        status = OSPI_writeCmd(qspiHandle, &wrParams);

        qspiHandle = OSPI_getHandle(CONFIG_OSPI0);

        OSPI_ReadCmdParams_init(&rdParams);
        rdParams.cmd       = CMD_RDSR;
        rdParams.rxDataBuf = p_norFlashStatus;
        rdParams.rxDataLen = 1;

        status = OSPI_readCmd(qspiHandle, &rdParams);
    }

    if (status == SystemP_SUCCESS)
    {
        DebugP_log("[QSPI] Status 0x%02X.\r\n", *p_norFlashStatus);
    }
    else
    {
        DebugP_log("[QSPI] Error %d.\r\n", status);
    }

    Board_driversClose();
    Drivers_ospiClose();

    return status;
}

/**
 * @brief This function reads 5 bytes from the NOR flash address 0.
 *
 * @param pcWriteBuffer FreeRTOS write buffer
 */
static void readFlash(char* const pcWriteBuffer)
{
    int32_t status               = SystemP_SUCCESS;
    OSPI_Handle qspiHandle       = {0};
    uint8_t buf[5]               = {0};
    OSPI_Transaction transaction = {0};

    Drivers_ospiOpen();

    status = Board_driversOpen();

    qspiHandle = OSPI_getHandle(CONFIG_OSPI0);

    OSPI_Transaction_init(&transaction);

    transaction.count           = ARRAY_SIZE(buf);
    transaction.addrOffset      = FLASH_ADD;
    transaction.buf             = buf;

    if (status == SystemP_SUCCESS)
    {
        status = OSPI_norFlashRead(qspiHandle, transaction.addrOffset, transaction.buf, transaction.count);
    }

    Board_driversClose();
    Drivers_ospiClose();

    if (status == SystemP_SUCCESS)
    {
        sprintf(pcWriteBuffer, "read flash:  %02X %02X %02X %02X %02X.\r\n", buf[0], buf[1], buf[2], buf[3] , buf[4]);
        DebugP_log("[QSPI] Read data %02X %02X %02X %02X %02X.\r\n", buf[0], buf[1], buf[2], buf[3] , buf[4]);
    }
    else
    {
        sprintf(pcWriteBuffer, "[QSPI] Error %d.\r\n", status);
        DebugP_log("[QSPI] Error %d.\r\n", status);
    }
}

/**
 * @brief This function erases the first nor flash block
 *
 * @param pcWriteBuffer FreeRTOS write buffer
 */
static void eraseflash(char* const pcWriteBuffer)
{
    int32_t status               = SystemP_SUCCESS;
    OSPI_Handle qspiHandle       = {0};

    Drivers_ospiOpen();

    status = Board_driversOpen();

    if (status == SystemP_SUCCESS)
    {
        qspiHandle = OSPI_getHandle(CONFIG_OSPI0);

        status = OSPI_norFlashErase(qspiHandle, FLASH_ADD);
    }

    if (status == SystemP_SUCCESS)
    {
        sprintf(pcWriteBuffer, "Flash erased.\r\n");
        DebugP_log("[QSPI] Flash erased.\r\n");
    }
    else
    {
        sprintf(pcWriteBuffer, "Failure flash erase %d!!!\r\n", status);
        DebugP_log("[QSPI] Failure flash erase %d!!!\r\n", status);
    }

    Board_driversClose();
    Drivers_ospiClose();
}

/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief This function handles the QSPI NOR flash command.
 *
 * @param pcWriteBuffer cli output string buffer
 * @param xWriteBufferLen length of the cli output string
 * @param pcCommandString cli command input string
 * @return pdFALSE = command is finished
 */
BaseType_t qspiNorFlashCommand(char* const pcWriteBuffer, __size_t xWriteBufferLen, const char* pcCommandString )
{
    BaseType_t retVal = pdFALSE;
    BaseType_t xParameter1StringLength = 0;
    uint8_t flashStatus = 0;
    const char* pcParameter1 = FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameter1StringLength);

    ospi_flash_io_main();

    return retVal;
}


/*[EOF]************************************************************************/
