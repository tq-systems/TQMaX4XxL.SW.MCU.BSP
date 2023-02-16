/**
 * @file     spi_func.c
 * copyright Copyright (c) 2023 TQ-Systems GmbH
 *           <license@tq-group.com>, D-82229 Seefeld, Germany.
 *           All rights reserved.
 * @author   <TODO author of the file>
 * @date     2023-02-14
 *
 * This software code is dual licensed under the terms and conditions of
 * (TQ-Systems Software License Agreement Version 1.0.3 or any later version) OR
 * (TQ-Systems Product Software License Agreement Version 1.0.1 or any later version) OR
 * (TQ-Systems Customer Software License Agreement Version 1.0.0 or any later version).
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
#include "fsl_afe_registers.h"
/* own */
#include "spi_func.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/

#define APP_MCSPI_MSGSIZE       (100U)

/*******************************************************************************
 * local macros
 ******************************************************************************/



/*******************************************************************************
 * local typedefs
 ******************************************************************************/

/**
 * @struct CRC_config
 * struct for CRC configurations
 * 16-bit Register/Address: CRC-2 generated of 16 bit crcSize = 1 is needed --> 1 Byte CRC
 * 24-bit Data/Register: crcSize =3 is needed + AND(0x0FFF) --> 3Byte CRC
 *
 * If Host writes to 16bit or 24 bit register with CRC, there must be extra
 * empty byte (8 extra SCLK) after the CRC of the DATA
 * --> Chip needs extra clocks to check/verify with the onchip CRC
 */
//hal_crc_config_t CRC_config = {
//        .crcSize                = 1,
//        .crcStartByte           = 0,
//        .crcRefIn               = KHAL_CrcInputNoRef,
//        .crcRefOut              = KHAL_CrcOutputNoRef,
//        .crcByteOrder           = KHAL_CrcMSByteFirst,
//        .complementChecksum     = false,    //true,
//        .crcSeed                = 0, //0xFFFFFFFF,
//        .crcPoly                = 0x107,  //0x7,  //KHAL_CrcPolynomial_CRC_8_CCITT,   /*!< x^8+x^2+x^1+1 */
//        .crcXorOut              = 0,    //0xFFFFFFFF,
//};

MCSPI_Transaction   spiTransaction;

/*******************************************************************************
 * local static data
 ******************************************************************************/

/*Flag that represent CRC_EN bit of SYS_CONFIG0 Register*/
bool crc_enabled = false;
uint8_t LPSPI1_txBuffer[LPSPI1_BUFFER_SIZE];
uint8_t LPSPI1_rxBuffer[LPSPI1_BUFFER_SIZE];

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

void spiCallback(MCSPI_Handle handle, MCSPI_Transaction *transaction)
{
    if (transaction->status == MCSPI_TRANSFER_COMPLETED)
    {
        spi_transfer_completed = true;
    }
    else
    {
        spi_transfer_completed = false;
        DebugP_log("Error occurred during SPI transfer!\r\n");
    }
}

status_t AFE_SPI_Write(uint16_t reg_addr, uint32_t reg_data, afe_reg_typ_t reg_width)
{
    status_t result = kStatus_NoTransferInProgress;
    uint8_t reg_addr_1, reg_addr_2;
//    MCSPI_Transaction   spiTransaction;
    int32_t             transferOK;

    /*DEV_AD-bit + RW_L-bit + RA12-RA7*/
    reg_addr_1 = (uint8_t) (((reg_addr >> 8) | WRITE_REG_OFFSET) >> 8);
    /*RA6-RA0 + Unused X-bit*/
    reg_addr_2 = (uint8_t) (reg_addr << 1);

    /*Assemble of first SPI frame for AFE*/
    LPSPI_AFE_txbuffer[0] = reg_addr_1;
    LPSPI_AFE_txbuffer[1] = reg_addr_2;

//    Drivers_open();
//    Board_driversOpen();

//    MCSPI_Transaction_init(&spiTransaction);
    spiTransaction.channel = gConfigMcspi0ChCfg[0].chNum;
    spiTransaction.csDisable = TRUE;
    spiTransaction.txBuf = (void*) LPSPI_AFE_txbuffer;
    spiTransaction.rxBuf = (void*) LPSPI_AFE_rxbuffer;
    spiTransaction.args = NULL;

    switch (reg_width)
    {
        case reg_16bit:
        {
            /*check crc_enabled flag (Is active if CRC_EN bit is set to 1)*/
            if (crc_enabled == true)
            {
//                /*Calculate CRC-2 (1Byte) checksum with first 16bit SPI payload*/
//                checksumCRC_TX[0] = HAL_CrcCompute(&CRC_config, LPSPI_AFE_txbuffer, 2);
//
//                LPSPI_AFE_txbuffer[2] = (uint8_t) checksumCRC_TX[0];
//                LPSPI_AFE_txbuffer[3] = (uint8_t) (reg_data >> 8);
//                LPSPI_AFE_txbuffer[4] = (uint8_t) (reg_data);
//
//                /*Calculate CRC-2 (1Byte) 16bit checksum with second 16bit SPI payload*/
//                checksumCRC_TX[1] = HAL_CrcCompute(&CRC_config, &(LPSPI_AFE_txbuffer[3]), 2);
//
//                LPSPI_AFE_txbuffer[5] = (uint8_t) checksumCRC_TX[1];
//                /*Provide 8 more SCLK*/
//                LPSPI_AFE_txbuffer[6] = SPI_DUMMY_DATA;
//                LPSPI_AFE_transfer.dataSize = 7;
            }
            else
            {
                LPSPI_AFE_txbuffer[2] = (uint8_t) (reg_data >>8);
                LPSPI_AFE_txbuffer[3] = (uint8_t) reg_data;
                spiTransaction.dataSize = 4;
            }

            break;
        }
        case reg_24bit:
        {
            /*check crc_enabled flag (Is active if CRC_EN bit is set to 1)*/
            if (crc_enabled == true)
            {
//                /*Calculate CRC-2 (1Byte) checksum with first 16bit SPI payload*/
//                checksumCRC_TX[0] = HAL_CrcCompute(&CRC_config, LPSPI_AFE_txbuffer, 2);
//
//
//                LPSPI_AFE_txbuffer[2] = (uint8_t) checksumCRC_TX[0];
//                LPSPI_AFE_txbuffer[3] = (uint8_t) (reg_data >> 16);
//                LPSPI_AFE_txbuffer[4] = (uint8_t) (reg_data >> 8);
//                LPSPI_AFE_txbuffer[5] = (uint8_t) reg_data;
//
//                /*Calculate CRC 24bit checksum with second 24bit SPI payload*/
//                checksumCRC_TX[1] = HAL_CrcCompute(&CRC_config, LPSPI_AFE_txbuffer[4], 3);
//
//                LPSPI_AFE_txbuffer[6] = (uint8_t) checksumCRC_TX[1];
//                /*Provide 8 more SCLK*/
//                LPSPI_AFE_txbuffer[7] = SPI_DUMMY_DATA;
//                spiTransaction.dataSize = 8;
            }
            else
            {
                LPSPI_AFE_txbuffer[2] = (uint8_t) (reg_data >> 16);
                LPSPI_AFE_txbuffer[3] = (uint8_t) (reg_data >> 8);
                LPSPI_AFE_txbuffer[4] = (uint8_t) reg_data;
                spiTransaction.dataSize = 5;
            }
            break;
        }
        default:
        {
            DebugP_log("Invalid Register typ entered!\r\n");
            result = kStatus_InvalidArgument;
            return result;
            break;
        }
    }

    spi_transfer_completed = false;

    if(spiTransaction.dataSize < 8)
    {
        spiTransaction.count = APP_MCSPI_MSGSIZE;
    }
    else
    {
        spiTransaction.count = APP_MCSPI_MSGSIZE / (spiTransaction.dataSize / 8);
    }

    /*Write Data via SPI*/
    transferOK = MCSPI_transfer(gMcspiHandle[CONFIG_MCSPI0], &spiTransaction);

    if((SystemP_SUCCESS != transferOK) || (MCSPI_TRANSFER_COMPLETED != spiTransaction.status))
    {
        DebugP_assert(FALSE); /* MCSPI transfer failed!! */
    }

    // Wait for transmission compled
//    while (!spi_transfer_completed)
//    { }

    spi_transfer_completed = false;

//    Board_driversClose();
//    Drivers_close();

    return result;
}

/**
 * @brief AFE_SPI_Read
 *
 * <Reads Register data via SPI Interrupt based>
 *
 * @param reg_addr[in]          Register to read from
 * @param data_len[in]          Amount of uint16_t data to read
 * @return status of status_t
 */
status_t AFE_SPI_Read(uint16_t reg_addr, afe_reg_typ_t reg_width)
{
    status_t result = 0;
    uint8_t reg_addr_1, reg_addr_2;
    uint16_t reg_addrShifted;
//    MCSPI_Transaction   spiTransaction;
    int32_t             transferOK;

    // AFE Communication Protocol
    // Shift left by one (Bit 25 = LSB, Bit 24 = 0, Bit 33 - 39 = 0)
    reg_addrShifted = (reg_addr << 1) & 0x01FE;
    // Bit 38 RW# = 1 for read access
    reg_addrShifted |= 0x4000,

    /*Assemble of first SPI frame for AFE*/
    LPSPI_AFE_txbuffer[0] = (uint8_t)(reg_addrShifted >> 8);
    LPSPI_AFE_txbuffer[1] = (uint8_t)reg_addrShifted;

//    Drivers_open();
//    Board_driversOpen();

    spiTransaction.channel = gConfigMcspi0ChCfg[0].chNum;
    spiTransaction.csDisable = TRUE;
    spiTransaction.txBuf = (void*) LPSPI_AFE_txbuffer;
    spiTransaction.rxBuf = (void*) LPSPI_AFE_rxbuffer;
    spiTransaction.args = NULL;

    /*Check Register Typ of AFE*/
    switch (reg_width)
    {
        case reg_16bit:
        {
            /*check crc_enabled flag (Is active if CRC_EN bit is set to 1)*/
            if (crc_enabled == true)
            {
//               /*Calculate CRC 16bit checksum with first 16bit SPI payload*/
//                checksumCRC_TX[0] = HAL_CrcCompute(&CRC_config, LPSPI_AFE_txbuffer, 2);
//
//               /*Fill CRC checksum into tx buffer*/
//               LPSPI_AFE_txbuffer[2] = (uint8_t) checksumCRC_TX[0];
//               /*Provide SCLK*/
//               memset(&(LPSPI_AFE_txbuffer[3]), SPI_DUMMY_DATA, 4);
//               LPSPI_AFE_transfer.dataSize = 7;
            }
            else
            {
               LPSPI_AFE_txbuffer[2] = SPI_DUMMY_DATA;
               LPSPI_AFE_txbuffer[3] = SPI_DUMMY_DATA;
               spiTransaction.dataSize = 4;
            }
            break;
        }

        case reg_24bit:
        {
            /*check crc_enabled flag (Is active if CRC_EN bit is set to 1)*/
            if (crc_enabled == true)
            {
//               /*Calculate CRC-2 (1Byte) checksum with first 16bit SPI payload*/
//                checksumCRC_TX[0] = HAL_CrcCompute(&CRC_config, LPSPI_AFE_txbuffer, 2);
//
//               /*Fill CRC checksum and reg_data into tx buffer*/
//               LPSPI_AFE_txbuffer[2] = (uint8_t) checksumCRC_TX[0];
//               /*Provide SCLK*/
//               memset(&(LPSPI_AFE_txbuffer[3]), SPI_DUMMY_DATA, 5);
//               LPSPI_AFE_transfer.dataSize = 7;
            }
            else
            {
               LPSPI_AFE_txbuffer[2] = SPI_DUMMY_DATA;
               LPSPI_AFE_txbuffer[3] = SPI_DUMMY_DATA;
               LPSPI_AFE_txbuffer[4] = SPI_DUMMY_DATA;
               spiTransaction.dataSize = 5;
            }
            break;
        }
        default:
        {
            DebugP_log("Invalid Register typ entered!\r\n");
            result = kStatus_InvalidArgument;
            return result;
            break;
        }
    }

    spi_transfer_completed = false;

    /*Read Data via SPI*/
    /*Write Data via SPI*/
    if(spiTransaction.dataSize < 8)
    {
        spiTransaction.count = APP_MCSPI_MSGSIZE ;
    }
    else
    {
        spiTransaction.count = APP_MCSPI_MSGSIZE / (spiTransaction.dataSize / 8);
    }

    transferOK = MCSPI_transfer(gMcspiHandle[CONFIG_MCSPI0], &spiTransaction);

    if((SystemP_SUCCESS != transferOK) || (MCSPI_TRANSFER_COMPLETED != spiTransaction.status))
    {
        DebugP_assert(FALSE); /* MCSPI transfer failed!! */
    }

    // Wait for transmission compled
//    while (!spi_transfer_completed)
//    { }
    spi_transfer_completed = false;

//    Board_driversClose();
//    Drivers_close();

    return result;
}

/**
 * @brief AFE_SPI_RW
 *
 * <Reads Data form Register and sends or'ed (data | old_value) to Register>
 *
 * @param *reg_data[in;out]     Data to write into Registers at reg_addr
 * @param reg_addr[in]          Register to write on
 * @param data_len[in]          Amount of uint16_t data to send (Buffer length)
 * @return status of status_t
 */
status_t AFE_SPI_RW(uint16_t reg_addr, uint32_t reg_data, afe_reg_typ_t reg_width)
{
    uint16_t reg_read_value = 0;
    status_t transfer_result = kStatus_Fail;

    transfer_result = AFE_SPI_Read(reg_addr, reg_width);

    /*Check SPI Transfer Configuration*/
    if (crc_enabled == true)
    {
        /*Fill variabel with actual register value*/
//        reg_read_value =  ((uint16_t)LPSPI_AFE_rxbuffer[4] << 8) | (uint16_t)LPSPI_AFE_rxbuffer[5];
    }
    else
    {
        /*Fill variabel with actual register value*/
        reg_read_value =  ((uint16_t)LPSPI_AFE_rxbuffer[2] << 8) | (uint16_t)LPSPI_AFE_rxbuffer[3];
    }

    /*Fill required bit-value with and consider old value*/
    reg_data = reg_data | (uint32_t)reg_read_value;

    /*Write new system configuration into register*/
    transfer_result = AFE_SPI_Write(reg_addr, reg_data, reg_16bit);

    return transfer_result;
}

/**
 * @brief AFE_SPI_Send_InstCMD
 *
 * <Sends Instructions CMD via SPI to the AFE>
 *
 * @param[in;out] <TODO parameter name> <TODO description of the parameter>
 *                                      <TODO description of the parameter>
 * @param[in]     <TODO parameter name> <TODO description of the parameter>
 * @param[out]    <TODO parameter name> <TODO description of the parameter>
 *
 * @return <specification of the return parameter>
 */
status_t AFE_SPI_Send_InstCMD(uint16_t cmd_value)
{
    status_t result = 0;
    MCSPI_Transaction   spiTransaction;
    int32_t             transferOK;

    LPSPI_AFE_txbuffer[0] = (uint8_t) ((cmd_value >> 8) << 1);
    LPSPI_AFE_txbuffer[1] = (uint8_t) (cmd_value << 1);

//    Board_driversOpen();
//    Drivers_open();

    /*Check Typ of Instruction CMD*/
    if (cmd_value <= NON_RETURN_CMD_BORDER)
    {
        /*Non-return Data CMD*/         /*RW bit = 0*/

        /*check crc_enabled flag (Is active if CRC_EN bit is set to 1)*/
        if (crc_enabled == true)
        {
            /*Calculate CRC-2 (1Byte) checksum with first 16bit SPI payload*/
//            checksumCRC_TX[0] = HAL_CrcCompute(&CRC_config, LPSPI_AFE_txbuffer, 2);
//            LPSPI_AFE_txbuffer[2] = (uint8_t) checksumCRC_TX[0];
//            LPSPI_AFE_txbuffer[3] = SPI_DUMMY_DATA;
//            LPSPI_AFE_transfer.dataSize = 4;
        }
        else
        {
            spiTransaction.dataSize = 2;
        }
    }
    else
    {
        /*Return ADC Data CMD*/         /*RW bit = 1*/

        /*check crc_enabled flag (Is active if CRC_EN bit is set to 1)*/
        if (crc_enabled == true)
        {
//            /*Calculate CRC-2 (1Byte) checksum with first 16bit SPI payload*/
//            checksumCRC_TX[0] = HAL_CrcCompute(&CRC_config, LPSPI_AFE_txbuffer, 2);
//            LPSPI_AFE_txbuffer[2] = (uint8_t) checksumCRC_TX[0];
//
//            /*Check of STATUS_EN bit*/
//            if (status_en_enabled)
//            {
//                /*Provide SCLK*/
//                memset(&(LPSPI_AFE_txbuffer[3]), SPI_DUMMY_DATA, 6);
//                spiTransaction.dataSize = 9;
//            }
//            else
//            {
//                /*Provide SCLK*/
//                memset(&(LPSPI_AFE_txbuffer[3]), SPI_DUMMY_DATA, 5);
//                spiTransaction.dataSize = 8;
//            }

        }
        else
        {
            spiTransaction.dataSize = 2;
        }
    }

    spi_transfer_completed = false;

    if(spiTransaction.dataSize < 8)
    {
        spiTransaction.count = APP_MCSPI_MSGSIZE ;
    }
    else
    {
        spiTransaction.count = APP_MCSPI_MSGSIZE / (spiTransaction.dataSize / 8);
    }

    /*Write Data via SPI*/
    transferOK = MCSPI_transfer(gMcspiHandle[CONFIG_MCSPI0], &spiTransaction);;

    // Wait for transmission compled
    while (!spi_transfer_completed)
    { }
    spi_transfer_completed = false;

//    Board_driversClose();
//    Drivers_close();

    return result;
}

void spi_init(void)
{
    Board_driversOpen();
    Drivers_open();

    MCSPI_Transaction_init(&spiTransaction);
}

/*[EOF]************************************************************************/
