/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file afe_comms.c
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2023-02-14
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the implementation of the AFE communication protocol.
 *
 */

/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */
#include <string.h>
/* project */
#include <kernel/dpl/DebugP.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
#include "afe_registers.h"
#include "crcDriver.h"
#include "spiDriver.h"
/* own */
#include "afe_comms.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/

#define APP_MCSPI_MSGSIZE       (100U)

/*******************************************************************************
 * local macros
 ******************************************************************************/

/*Flag that represent CRC_EN bit of SYS_CONFIG0 Register*/
bool crc_enabled = false;
uint8_t SPI_txBuffer[BUFFER_SIZE];
uint8_t SPI_rxBuffer[BUFFER_SIZE];

/*******************************************************************************
 * local typedefs
 ******************************************************************************/



/*******************************************************************************
 * local static data
 ******************************************************************************/


/*Flag that represent STATUS_EN bit of SYS_CONFIG0 Register*/
static bool status_en_enabled     = false;
static uint32_t checksumCRC_TX[2] = {0};
static uint32_t checksumCRC_RX[2] = {0};

/*******************************************************************************
 * forward declarations
 ******************************************************************************/

/*Global SPI Transfer Flag*/
volatile bool spi_transfer_completed = false;

/*******************************************************************************
 * local static functions
 ******************************************************************************/



/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief This function writs data to the AFE via SPI
 *
 * @param reg_addr AFE register address
 * @param reg_data AFE data to be write
 * @param reg_width AFE Register width typs
 * @return status of status_t
 */
status_t AFE_SPI_Write(uint16_t reg_addr, uint32_t reg_data, afe_reg_typ_t reg_width)
{
    status_t result = NO_TRANSFER_IN_PROGRESS;
    uint8_t reg_addr_1, reg_addr_2;
    uint8_t dataSize = 0;

    /*DEV_AD-bit + RW_L-bit + RA12-RA7*/
    reg_addr_1 = (uint8_t) (((reg_addr >> 8) | WRITE_REG_OFFSET) >> 8);
    /*RA6-RA0 + Unused X-bit*/
    reg_addr_2 = (uint8_t) (reg_addr << 1);

    /*Assemble of first SPI frame for AFE*/
    AFE_txBuffer[0] = reg_addr_1;
    AFE_txBuffer[1] = reg_addr_2;

    switch (reg_width)
    {
        case reg_16bit:
        {
            /*check crc_enabled flag (Is active if CRC_EN bit is set to 1)*/
            if (crc_enabled == true)
            {
                /*Calculate CRC-2 (1Byte) checksum with first 16bit SPI payload*/
                checksumCRC_TX[0] = crc_compute(AFE_txBuffer, 2);

                AFE_txBuffer[2] = (uint8_t) checksumCRC_TX[0];
                AFE_txBuffer[3] = (uint8_t) (reg_data >> 8);
                AFE_txBuffer[4] = (uint8_t) (reg_data);

                /*Calculate CRC-2 (1Byte) 16bit checksum with second 16bit SPI payload*/
                checksumCRC_TX[1] = crc_compute(&AFE_txBuffer[3], 2);

                AFE_txBuffer[5] = (uint8_t) checksumCRC_TX[1];
                /*Provide 8 more SCLK*/
                AFE_txBuffer[6] = SPI_DUMMY_DATA;
                dataSize = 7;
            }
            else
            {
                AFE_txBuffer[2] = (uint8_t) (reg_data >>8);
                AFE_txBuffer[3] = (uint8_t) reg_data;
                dataSize = 4;   /* transmit 4 Bytes */
            }

            break;
        }
        case reg_24bit:
        {
            /*check crc_enabled flag (Is active if CRC_EN bit is set to 1)*/
            if (crc_enabled == true)
            {
                /*Calculate CRC-2 (1Byte) checksum with first 16bit SPI payload*/
                checksumCRC_TX[0] = crc_compute(AFE_txBuffer, 2);


                AFE_txBuffer[2] = (uint8_t) checksumCRC_TX[0];
                AFE_txBuffer[3] = (uint8_t) (reg_data >> 16);
                AFE_txBuffer[4] = (uint8_t) (reg_data >> 8);
                AFE_txBuffer[5] = (uint8_t) reg_data;

                /*Calculate CRC 24bit checksum with second 24bit SPI payload*/
                checksumCRC_TX[1] = crc_compute(&AFE_txBuffer[4], 3);

                AFE_txBuffer[6] = (uint8_t) checksumCRC_TX[1];
                /*Provide 8 more SCLK*/
                AFE_txBuffer[7] = SPI_DUMMY_DATA;
                dataSize = 8;
            }
            else
            {
                AFE_txBuffer[2] = (uint8_t) (reg_data >> 16);
                AFE_txBuffer[3] = (uint8_t) (reg_data >> 8);
                AFE_txBuffer[4] = (uint8_t) reg_data;
                dataSize = 5;
            }
            break;
        }
        default:
        {
            DebugP_log("Invalid Register typ entered!\r\n");
            result = INVALID_ARGUMENT;
            return result;
            break;
        }
    }

    if (spi_transmit(AFE_txBuffer, AFE_rxBuffer, dataSize))
    {
        result = SUCCESS;
    }
    else
    {
        result = FAIL;
    }

    return result;
}

/**
 * @brief Reads Register data via SPI Interrupt based
 *
 * @param reg_addr[in]          Register to read from
 * @param data_len[in]          Amount of uint16_t data to read
 * @return status of status_t
 */
status_t AFE_SPI_Read(uint16_t reg_addr, afe_reg_typ_t reg_width)
{
    status_t result    = 0;
    uint16_t reg_addrShifted;
    uint8_t dataSize = 0;

    // AFE Communication Protocol
    // Shift left by one (Bit 25 = LSB, Bit 24 = 0, Bit 33 - 39 = 0)
    reg_addrShifted = (reg_addr << 1) & 0x01FE;
    // Bit 38 RW# = 1 for read access
    reg_addrShifted |= 0x4000,

    /*Assemble of first SPI frame for AFE*/
    AFE_txBuffer[0] = (uint8_t)(reg_addrShifted >> 8);
    AFE_txBuffer[1] = (uint8_t)reg_addrShifted;


    /*Check Register Typ of AFE*/
    switch (reg_width)
    {
        case reg_16bit:
        {
            /*check crc_enabled flag (Is active if CRC_EN bit is set to 1)*/
            if (crc_enabled == true)
            {
               /*Calculate CRC 16bit checksum with first 16bit SPI payload*/
                checksumCRC_TX[0] = crc_compute(AFE_txBuffer, 2);

               /*Fill CRC checksum into tx buffer*/
               AFE_txBuffer[2] = (uint8_t) checksumCRC_TX[0];
               /*Provide SCLK*/
               memset(&(AFE_txBuffer[3]), SPI_DUMMY_DATA, 4);
               dataSize = 7;
            }
            else
            {
               AFE_txBuffer[2] = SPI_DUMMY_DATA;
               AFE_txBuffer[3] = SPI_DUMMY_DATA;
               dataSize = 4;  // transmit 4 Bytes
            }
            break;
        }

        case reg_24bit:
        {
            /*check crc_enabled flag (Is active if CRC_EN bit is set to 1)*/
            if (crc_enabled == true)
            {
               /*Calculate CRC-2 (1Byte) checksum with first 16bit SPI payload*/
                checksumCRC_TX[0] = crc_compute(AFE_txBuffer, 2);

               /*Fill CRC checksum and reg_data into tx buffer*/
               AFE_txBuffer[2] = (uint8_t) checksumCRC_TX[0];
               /*Provide SCLK*/
               memset(&(AFE_txBuffer[3]), SPI_DUMMY_DATA, 5);
               dataSize = 7;
            }
            else
            {
               AFE_txBuffer[2] = SPI_DUMMY_DATA;
               AFE_txBuffer[3] = SPI_DUMMY_DATA;
               AFE_txBuffer[4] = SPI_DUMMY_DATA;
               dataSize = 5;
            }
            break;
        }
        default:
        {
            DebugP_log("Invalid Register typ entered!\r\n");
            result = INVALID_ARGUMENT;
            return result;
            break;
        }
    }

    if (spi_transmit(AFE_txBuffer, AFE_rxBuffer, dataSize))
    {
        result = SUCCESS;
    }
    else
    {
        result = FAIL;
    };

    return result;
}

/**
 * @brief Reads Data form Register and sends or'ed (data | old_value) to Register
 *
 * @param *reg_data[in;out]     Data to write into Registers at reg_addr
 * @param reg_addr[in]          Register to write on
 * @param data_len[in]          Amount of uint16_t data to send (Buffer length)
 * @return status of status_t
 */
status_t AFE_SPI_RW(uint16_t reg_addr, uint32_t reg_data, afe_reg_typ_t reg_width)
{
    uint16_t reg_read_value = 0;
    status_t transfer_result = FAIL;

    transfer_result = AFE_SPI_Read(reg_addr, reg_width);

    /*Check SPI Transfer Configuration*/
    if (crc_enabled == true)
    {
        /*Fill variabel with actual register value*/
        reg_read_value =  ((uint16_t)AFE_rxBuffer[4] << 8) | (uint16_t)AFE_rxBuffer[5];
    }
    else
    {
        /*Fill variabel with actual register value*/
        reg_read_value =  ((uint16_t)AFE_rxBuffer[2] << 8) | (uint16_t)AFE_rxBuffer[3];
    }

    /*Fill required bit-value with and consider old value*/
    reg_data = reg_data | (uint32_t)reg_read_value;

    /*Write new system configuration into register*/
    transfer_result = AFE_SPI_Write(reg_addr, reg_data, reg_16bit);

    return transfer_result;
}

/**
 * @brief Sends Instructions CMD via SPI to the AFE
 *
 * @param cmd_value AFE command
 * @return status of status_t
 */
status_t AFE_SPI_Send_InstCMD(uint16_t cmd_value)
{
    status_t result  = 0;
    uint8_t dataSize = 0;

    AFE_txBuffer[0] = (uint8_t) ((cmd_value >> 8) << 1);
    AFE_txBuffer[1] = (uint8_t) (cmd_value << 1);

    /*Check Typ of Instruction CMD*/
    if (cmd_value <= NON_RETURN_CMD_BORDER)
    {
        /*Non-return Data CMD*/         /*RW bit = 0*/

        /*check crc_enabled flag (Is active if CRC_EN bit is set to 1)*/
        if (crc_enabled == true)
        {
            /*Calculate CRC-2 (1Byte) checksum with first 16bit SPI payload*/
            checksumCRC_TX[0] = crc_compute(AFE_txBuffer, 2);
            AFE_txBuffer[2] = (uint8_t) checksumCRC_TX[0];
            AFE_txBuffer[3] = SPI_DUMMY_DATA;
            dataSize = 4;
        }
        else
        {
            dataSize = 2;
        }
    }
    else
    {
        /*Return ADC Data CMD*/         /*RW bit = 1*/

        /*check crc_enabled flag (Is active if CRC_EN bit is set to 1)*/
        if (crc_enabled == true)
        {
            /*Calculate CRC-2 (1Byte) checksum with first 16bit SPI payload*/
            checksumCRC_TX[0] = crc_compute( AFE_txBuffer, 2);
            AFE_txBuffer[2] = (uint8_t) checksumCRC_TX[0];

            /*Check of STATUS_EN bit*/
            if (status_en_enabled)
            {
                /*Provide SCLK*/
                memset(&(AFE_txBuffer[3]), SPI_DUMMY_DATA, 6);
                dataSize = 9;
            }
            else
            {
                /*Provide SCLK*/
                memset(&(AFE_txBuffer[3]), SPI_DUMMY_DATA, 5);
                dataSize = 8;
            }

        }
        else
        {
            dataSize = 0;
        }
    }

    if (spi_transmit(AFE_txBuffer, AFE_rxBuffer, dataSize))
    {
        result = SUCCESS;
    }
    else
    {
        result = FAIL;
    }

    return result;
}

/*[EOF]************************************************************************/
