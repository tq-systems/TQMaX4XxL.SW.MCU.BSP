/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file crcDriver.c
 * @copyright Copyright (c) 2018 NXP
 * @author Michael Bernhardt
 *
 * @date 2023-02-14
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the implementation of the CRC calculation.
 *
 */

/*******************************************************************************
 * local includes
 ******************************************************************************/

/* runtime */
#include "stdbool.h"
/* project */

/* own */
#include "crcDriver.h"

/*******************************************************************************
 * local defines
 ******************************************************************************/



/*******************************************************************************
 * local macros
 ******************************************************************************/

/*! @brief crcRefIn definitions. */
typedef enum crc_cfg_refin
{
    CrcInputNoRef = 0U, /*!< Do not manipulate input data stream. */
    CrcRefInput   = 1U  /*!< Reflect each byte in the input stream bitwise. */
} crc_cfg_refin_t;

/*! @brief crcRefOut definitions. */
typedef enum crc_cfg_refout
{
    CrcOutputNoRef = 0U, /*!< Do not manipulate CRC result. */
    CrcRefOutput   = 1U  /*!< CRC result is to be reflected bitwise (operated on entire word). */
} crc_cfg_refout_t;

/*! @brief crcByteOrder definitions. */
typedef enum crc_cfg_byteord
{
    CrcLSByteFirst = 0U, /*!< Byte order of the CRC LS Byte first. */
    CrcMSByteFirst = 1U  /*!< Bit order of the CRC  MS Byte first. */
} crc_cfg_byteord_t;

/*! @brief CRC polynomials to use. */
typedef enum crc_polynomial
{
    CrcPolynomial_CRC_8_CCITT = 0x103,      /*!< x^8+x^2+x^1+1 */
    CrcPolynomial_CRC_16      = 0x1021,     /*!< x^16+x^12+x^5+1 */
    CrcPolynomial_CRC_32      = 0x4C11DB7U, /*!< x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x+1 */
} crc_polynomial_t;

/*******************************************************************************
 * local typedefs
 ******************************************************************************/

/*! @brief CRC configuration structure. */
typedef struct
{
    crc_cfg_refin_t crcRefIn;       /*!< CRC reflect input. See "hal_crc_cfg_refin_t". */
    crc_cfg_refout_t crcRefOut;     /*!< CRC reflect output. See "hal_crc_cfg_refout_t". */
    crc_cfg_byteord_t crcByteOrder; /*!< CRC byte order. See "hal_crc_cfg_byteord_t". */
    uint32_t crcSeed;                   /*!< CRC Seed value. Initial value for CRC LFSR. */
    uint32_t crcPoly;                   /*!< CRC Polynomial value. */
    uint32_t crcXorOut;                 /*!< XOR mask for CRC result (for no mask, should be 0). */
    uint8_t complementChecksum;         /*!< wether output the complement checksum. */
    uint8_t crcSize;                    /*!< Number of CRC octets, 2 mean use CRC16, 4 mean use CRC32. */
    uint8_t crcStartByte;               /*!< Start CRC with this byte position. Byte #0 is the first byte of Sync Address. */
} crc_config_t;

/*******************************************************************************
 * local static data
 ******************************************************************************/

crc_config_t crcConfig =
{
        .crcSize                = 1,
        .crcStartByte           = 0,
        .crcRefIn               = CrcInputNoRef,
        .crcRefOut              = CrcOutputNoRef,
        .crcByteOrder           = CrcMSByteFirst,
        .complementChecksum     = false,    //true,
        .crcSeed                = 0, //0xFFFFFFFF,
        .crcPoly                = 0x107,  //0x7,  //KHAL_CrcPolynomial_CRC_8_CCITT,   /*!< x^8+x^2+x^1+1 */
        .crcXorOut              = 0,    //0xFFFFFFFF,
};

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
 * @brief This function computes the CRC
 *
 * @param p_buf pointer to buffer
 * @param size buffer size
 * @return crc
 */
uint32_t crc_compute(uint8_t* p_buf, uint8_t length)
{
    uint32_t shiftReg    = crcConfig.crcSeed << ((4U - crcConfig.crcSize) << 3U);
    uint32_t crcPoly     = crcConfig.crcPoly << ((4U - crcConfig.crcSize) << 3U);
    uint32_t crcXorOut   = crcConfig.crcXorOut << ((4U - crcConfig.crcSize) << 3U);
    uint16_t startOffset = crcConfig.crcStartByte;
    uint8_t crcBits      = 8U * crcConfig.crcSize;
    uint32_t computedCRC = 0;
    uint32_t i, j;
    uint8_t data = 0;
    uint8_t bit;

    /* Size 0 will bypass CRC calculation. */
    if (crcConfig.crcSize != 0U)
    {
        for (i = 0UL + startOffset; i < length; i++)
        {
            data = p_buf[i];

            if (crcConfig.crcRefIn == CrcRefInput)
            {
                bit = 0U;
                for (j = 0U; j < 8U; j++)
                {
                    bit = (bit << 1);
                    bit |= ((data & 1U) != 0U) ? 1U : 0U;
                    data = (data >> 1);
                }
                data = bit;
            }

            for (j = 0; j < 8U; j++)
            {
                bit  = ((data & 0x80U) != 0U) ? 1U : 0U;
                data = (data << 1);

                if ((shiftReg & 1UL << 31) != 0U)
                {
                    bit = (bit != 0U) ? 0U : 1U;
                }

                shiftReg = (shiftReg << 1);

                if (bit != 0U)
                {
                    shiftReg ^= crcPoly;
                }

                if ((bool)bit && ((crcPoly & (1UL << (32U - crcBits))) != 0U))
                {
                    shiftReg |= (1UL << (32U - crcBits));
                }
                else
                {
                    shiftReg &= ~(1UL << (32U - crcBits));
                }
            }
        }

        shiftReg ^= crcXorOut;

        if (crcConfig.crcByteOrder == CrcMSByteFirst)
        {
            computedCRC = (shiftReg >> (32U - crcBits));
        }
        else
        {
            computedCRC = 0;
            j = 1U;
            for (i = 0; i < 32U; i++)
            {
                computedCRC = (computedCRC << 1);
                computedCRC |= ((shiftReg & j) != 0U) ? 1U : 0U;
                j = (j << 1);
            }
        }
    }

    return computedCRC;
}

/*[EOF]************************************************************************/
