/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file afe_registers.c
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Klaus Forstner
 *
 * @date 2022-02-09
 * -----------------------------------------------------------------------------
 * @brief This file contains the implementation of the AFE.
 *
 */


/*******************************************************************************
 * local includes
 ******************************************************************************/
/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* runtime */

/* project */
#include <string.h>
/* module */
#include "SystemP.h"
#include <afe.h>
#include <afe_comms.h>
#include <afe_registers.h>
#include "math.h"
#include "ti_drivers_config.h"


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
CH_CONFIG_t AFE_ChannelInit[16];
SYS_REG_t AFE_SystemInit;
/*******************************************************************************
 * local static functions
 ******************************************************************************/



/*******************************************************************************
 * global functions
 ******************************************************************************/

/**
 * @brief Enable AFE with POR
 *
 */
void AFE_Enable(void)
{
    uint16_t reg_read_value = 0;

    /*System Power On Reset*/
    GPIO_pinWriteLow(ADC_RST_BASE_ADDR, ADC_RST_PIN);
    vTaskDelay(pdMS_TO_TICKS(1));
    GPIO_pinWriteHigh(ADC_RST_BASE_ADDR, ADC_RST_PIN);
    vTaskDelay(pdMS_TO_TICKS(20));

    /*Read of STATUS0 until Chip is ready*/
    while(reg_read_value != SYS_STATUS0_CHIP_READY_MASK)
    {
        if(AFE_SPI_Read(SYS_CONTROL.SYS_STATUS0, reg_16bit) != SystemP_SUCCESS)
        {
            DebugP_log("[ ERROR ] \t AFE13388 Initialization failed!\r\n");
            return;
        }
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

        /*Masking Chip ready bit*/
        reg_read_value &= SYS_STATUS0_CHIP_READY_MASK;
    }

    vTaskDelay(pdMS_TO_TICKS(25));

    DebugP_log("[ OK ]\tAFE13388 Chip Ready\r\n");
}

/********************************************************************************/

/**
 * @brief Initialise AFE
 *
 */
void AFE_Init(void)
{
	uint16_t serial0 = 0;
	uint16_t serial1 = 0;

	Init_CH_RegisterAdr();

	// Read NAFE Informations
	// Serial0
    AFE_SPI_Read(0x7C, reg_16bit);
    serial0 =  ((uint16_t)AFE_rxBuffer[2] << 8) | (uint16_t)AFE_rxBuffer[3];
	// Serial1
    AFE_SPI_Read(0x7D, reg_16bit);
    serial1 =  ((uint16_t)AFE_rxBuffer[2] << 8) | (uint16_t)AFE_rxBuffer[3];

    DebugP_log("\t - Serial %X%X\r\n", serial0, serial1);

    AFE_SystemInit.SYS_CONFIG0 = SYS_CONFIG0_REF_SEL(0)    |
                                 SYS_CONFIG0_CK_SRC_SEL(0) |
                                 SYS_CONFIG0_CRC_EN(0)     |
                                 SYS_CONFIG0_ADC_SYNC(0)   |
                                 SYS_CONFIG0_DRDY_PWDT(1);

    DebugP_log("[ OK ]\tAFE13388 Init\r\n");

    DebugP_log("\t - AFE_CONFIG0 0x%x\r\n", AFE_SystemInit.SYS_CONFIG0);

    // SPI Write
    AFE_SPI_Write(SYS_CONTROL.SYS_CONFIG0, AFE_SystemInit.SYS_CONFIG0, reg_16bit);

    //------------------------------------------------------------------------------------------------------
    // Channel 0
    //------------------------------------------------------------------------------------------------------
    /*
     * Channel Configuration
     *             Type                 Gain             Nominal Input Range
     *    AI1P:    Unipolar SE          0.2V/V                +10V
     */
    AFE_ChannelInit[0].CH_CONFIG0 = CH_CONFIG0_HV_AIP(1)   |
                                    CH_CONFIG0_HV_AIN(0)   |
                                    CH_CONFIG0_CH_GAIN(0)  |
                                    CH_CONFIG0_LVSIG_IN(0) |
                                    CH_CONFIG0_HV_SEL(1);
    AFE_ChannelInit[0].CH_CONFIG1 = 0x38;       // DATA_RATE 7
    AFE_ChannelInit[0].CH_CONFIG2 = 0x0;
    AFE_ChannelInit[0].CH_CONFIG3 = 0x0;
    AFE_ChannelInit[0].CH_CONFIG4 = 0x0;        // Multichannel Disabled
    //------------------------------------------------------------------------------------------------------
    // Channel 1
    //------------------------------------------------------------------------------------------------------
    /*
     * Channel Configuration
     *             Type                 Gain              Nominal Input Range
     *    AI1N:    Unipolar SE          0.2V/V             +10V
     */
    AFE_ChannelInit[1].CH_CONFIG0 = CH_CONFIG0_HV_AIP(0)   |
                                    CH_CONFIG0_HV_AIN(1)   |
                                    CH_CONFIG0_CH_GAIN(0)  |
                                    CH_CONFIG0_LVSIG_IN(0) |
                                    CH_CONFIG0_HV_SEL(1);
    AFE_ChannelInit[1].CH_CONFIG1 = 0x38;   // DATA_RATE 7
    AFE_ChannelInit[1].CH_CONFIG2 = 0x0;
    AFE_ChannelInit[1].CH_CONFIG3 = 0x0;
    AFE_ChannelInit[1].CH_CONFIG4 = 0x0;    // Multichannel Disabled
    //------------------------------------------------------------------------------------------------------
    // Channel 2
    //------------------------------------------------------------------------------------------------------
    /*
     * Channel Configuration
     *             Type                 Gain               Nominal Input Range
     *    AI2P:    Unipolar SE          0.2V/V              +10V
     */
    AFE_ChannelInit[2].CH_CONFIG0 = CH_CONFIG0_HV_AIP(2)   |
                                    CH_CONFIG0_HV_AIN(0)   |
                                    CH_CONFIG0_CH_GAIN(0)  |
                                    CH_CONFIG0_LVSIG_IN(0) |
                                    CH_CONFIG0_HV_SEL(1);
    AFE_ChannelInit[2].CH_CONFIG1 = 0x38;   // DATA_RATE 7
    AFE_ChannelInit[2].CH_CONFIG2 = 0x0;
    AFE_ChannelInit[2].CH_CONFIG3 = 0x0;
    AFE_ChannelInit[2].CH_CONFIG4 = 0x0;    // Multichannel Disabled
    //------------------------------------------------------------------------------------------------------
    // Channel 3
    //------------------------------------------------------------------------------------------------------
    /*
     * Channel Configuration
     *             Type                 Gain                Nominal Input Range
     *    AI2N:    Unipolar SE          0.2V/V              +10V
     */
    AFE_ChannelInit[3].CH_CONFIG0 = CH_CONFIG0_HV_AIP(0)   |
                                    CH_CONFIG0_HV_AIN(2)   |
                                    CH_CONFIG0_CH_GAIN(0)  |
                                    CH_CONFIG0_LVSIG_IN(0) |
                                    CH_CONFIG0_HV_SEL(1);
    AFE_ChannelInit[3].CH_CONFIG1 = 0x38;    // DATA_RATE 7
    AFE_ChannelInit[3].CH_CONFIG2 = 0x0;
    AFE_ChannelInit[3].CH_CONFIG3 = 0x0;
    AFE_ChannelInit[3].CH_CONFIG4 = 0x0;     // Multichannel Disabled
    //------------------------------------------------------------------------------------------------------
    // Channel 4
    //------------------------------------------------------------------------------------------------------
    /*
     * Channel Configuration
     *              Type                Gain                Nominal Input Range
     *    AI3P:     Unipolar SE         1V/V                +2.0V
     */
    AFE_ChannelInit[4].CH_CONFIG0 = CH_CONFIG0_HV_AIP(3)   |
                                    CH_CONFIG0_HV_AIN(0)   |
                                    CH_CONFIG0_CH_GAIN(3)  |
                                    CH_CONFIG0_LVSIG_IN(0) |
                                    CH_CONFIG0_HV_SEL(1);
    AFE_ChannelInit[4].CH_CONFIG1 = 0x38;   // DATA_RATE 7
    AFE_ChannelInit[4].CH_CONFIG2 = 0x0;
    AFE_ChannelInit[4].CH_CONFIG3 = 0x0;
    AFE_ChannelInit[4].CH_CONFIG4 = 0x0;    // Multichannel Disabled
    //------------------------------------------------------------------------------------------------------
    // Channel 5
    //------------------------------------------------------------------------------------------------------
    /*
     * Channel Configuration
     *              Type                Gain                Nominal Input Range
     *    AI3N:    Unipolar SE          1V/V                +2.0V
     */
    AFE_ChannelInit[5].CH_CONFIG0 = CH_CONFIG0_HV_AIP(0)   |
                                    CH_CONFIG0_HV_AIN(3)   |
                                    CH_CONFIG0_CH_GAIN(3)  |
                                    CH_CONFIG0_LVSIG_IN(0) |
                                    CH_CONFIG0_HV_SEL(1);
    AFE_ChannelInit[5].CH_CONFIG1 = 0x38;   // DATA_RATE 7
    AFE_ChannelInit[5].CH_CONFIG2 = 0x0;
    AFE_ChannelInit[5].CH_CONFIG3 = 0x0;
    AFE_ChannelInit[5].CH_CONFIG4 = 0x0;    // Multichannel Disabled

    //------------------------------------------------------------------------------------------------------
    // Channel 6
    //------------------------------------------------------------------------------------------------------
    /*
     * Channel Configuration
     *              Type                Gain                Nominal Input Range
     *    AI4P:    Unipolar SE          1V/V                +2.0V
     */
    AFE_ChannelInit[6].CH_CONFIG0 = CH_CONFIG0_HV_AIP(4)   |
                                    CH_CONFIG0_HV_AIN(0)   |
                                    CH_CONFIG0_CH_GAIN(3)  |
                                    CH_CONFIG0_LVSIG_IN(0) |
                                    CH_CONFIG0_HV_SEL(1);
    AFE_ChannelInit[6].CH_CONFIG1 = 0x38;   // DATA_RATE 7
    AFE_ChannelInit[6].CH_CONFIG2 = 0x0;
    AFE_ChannelInit[6].CH_CONFIG3 = 0x0;
    AFE_ChannelInit[6].CH_CONFIG4 = 0x0;    // Multichannel Disabled

    //------------------------------------------------------------------------------------------------------
    // Channel 7
    //------------------------------------------------------------------------------------------------------
    /*
     * Channel Configuration
     *              Type                Gain                Nominal Input Range
     *    AI4N:    Unipolar SE          1V/V                +2.0V
     */
    AFE_ChannelInit[7].CH_CONFIG0 = CH_CONFIG0_HV_AIP(0)   |
                                    CH_CONFIG0_HV_AIN(4)   |
                                    CH_CONFIG0_CH_GAIN(3)  |
                                    CH_CONFIG0_LVSIG_IN(0) |
                                    CH_CONFIG0_HV_SEL(1);
    AFE_ChannelInit[7].CH_CONFIG1 = 0x38;   // DATA_RATE 7
    AFE_ChannelInit[7].CH_CONFIG2 = 0x0;
    AFE_ChannelInit[7].CH_CONFIG3 = 0x0;
    AFE_ChannelInit[7].CH_CONFIG4 = 0x0;    // Multichannel Disabled
    //------------------------------------------------------------------------------------------------------
    // Channel 8
    //------------------------------------------------------------------------------------------------------
    /*
     * Channel Configuration
     *                  Type
     *    REFCAL_H:    Internal
     */
    AFE_ChannelInit[8].CH_CONFIG0 = CH_CONFIG0_HV_AIP(5)   |
                                    CH_CONFIG0_HV_AIN(0)   |
                                    CH_CONFIG0_CH_GAIN(3)  |
                                    CH_CONFIG0_LVSIG_IN(0) |
                                    CH_CONFIG0_HV_SEL(1);
    AFE_ChannelInit[8].CH_CONFIG1 = 0x38;   // DATA_RATE 7
    AFE_ChannelInit[8].CH_CONFIG2 = 0x0;
    AFE_ChannelInit[8].CH_CONFIG3 = 0x0;
    AFE_ChannelInit[8].CH_CONFIG4 = 0x0;    // Multichannel Disabled
    //------------------------------------------------------------------------------------------------------
    // Channel 9
    //------------------------------------------------------------------------------------------------------
    /*
     * Channel Configuration
     *                  Type
     *    REFCAL_L:     Internal
     */
    AFE_ChannelInit[9].CH_CONFIG0 = CH_CONFIG0_HV_AIP(6)   |
                                    CH_CONFIG0_HV_AIN(0)   |
                                    CH_CONFIG0_CH_GAIN(3)  |
                                    CH_CONFIG0_LVSIG_IN(0) |
                                    CH_CONFIG0_HV_SEL(1);
    AFE_ChannelInit[9].CH_CONFIG1 = 0x38;   // DATA_RATE 7
    AFE_ChannelInit[9].CH_CONFIG2 = 0x0;
    AFE_ChannelInit[9].CH_CONFIG3 = 0x0;
    AFE_ChannelInit[9].CH_CONFIG4 = 0x0;    // Multichannel Disabled
    //------------------------------------------------------------------------------------------------------
    // Channel 10
    //------------------------------------------------------------------------------------------------------
    /*
     * Channel Configuration
     *                  Type
     *    GPIO 0-1:     Internal
     */
    AFE_ChannelInit[10].CH_CONFIG0 = CH_CONFIG0_HV_AIP(0)   |
                                     CH_CONFIG0_HV_AIN(0)   |
                                     CH_CONFIG0_CH_GAIN(3)  |
                                     CH_CONFIG0_LVSIG_IN(1) |
                                     CH_CONFIG0_HV_SEL(0);
    AFE_ChannelInit[10].CH_CONFIG1 = 0x38;  // DATA_RATE 7
    AFE_ChannelInit[10].CH_CONFIG2 = 0x0;
    AFE_ChannelInit[10].CH_CONFIG3 = 0x0;
    AFE_ChannelInit[10].CH_CONFIG4 = 0x0;   // Multichannel Disabled
    //------------------------------------------------------------------------------------------------------
    // Channel 11
    //------------------------------------------------------------------------------------------------------
    /*
     * Channel Configuration
     *              Type
     *   HVDD:      Internal
     */
    AFE_ChannelInit[11].CH_CONFIG0 = CH_CONFIG0_HV_AIP(0)   |
                                     CH_CONFIG0_HV_AIN(0)   |
                                     CH_CONFIG0_CH_GAIN(3)  |
                                     CH_CONFIG0_LVSIG_IN(4) |
                                     CH_CONFIG0_HV_SEL(0);
    AFE_ChannelInit[11].CH_CONFIG1 = 0x38;  // DATA_RATE 7
    AFE_ChannelInit[11].CH_CONFIG2 = 0x0;
    AFE_ChannelInit[11].CH_CONFIG3 = 0x0;
    AFE_ChannelInit[11].CH_CONFIG4 = 0x0;   // Multichannel Disabled

    AFE_GpioInit();
}

/********************************************************************************/

/**
 * @brief Read ADC Voltages
 *
 * @param channel AFE channel AI
 * @return adc voltage
 */
float AFE_ReadAdcVoltage(uint8_t channel)
{
    uint16_t status0_regValue = 0;
    uint32_t result = 0;
    float voltage = 0;
    float gain = 0;

    switch (channel)
    {
    case AFE_CH_AI1P: /*CMD_Pointer set to CH0*/
        AFE_SPI_Send_InstCMD(CMD_CH0);
        gain = 0.2;
        break;
    case AFE_CH_AI1N: /*CMD_Pointer set to CH1*/
        AFE_SPI_Send_InstCMD(CMD_CH1);
        gain = 0.2;
        break;
    case AFE_CH_AI2P: /*CMD_Pointer set to CH2*/
        AFE_SPI_Send_InstCMD(CMD_CH2);
        gain = 0.2;
        break;
    case AFE_CH_AI2N: /*CMD_Pointer set to CH3*/
        AFE_SPI_Send_InstCMD(CMD_CH3);
        gain = 0.2;
        break;
    case AFE_CH_AI3P: /*CMD_Pointer set to CH4*/
        AFE_SPI_Send_InstCMD(CMD_CH4);
        gain = 1;
        break;
    case AFE_CH_AI3N: /*CMD_Pointer set to CH5*/
        AFE_SPI_Send_InstCMD(CMD_CH5);
        gain = 1;
        break;
    case AFE_CH_AI4P: /*CMD_Pointer set to CH6*/
        AFE_SPI_Send_InstCMD(CMD_CH6);
        gain = 1;
        break;
    case AFE_CH_AI4N: /*CMD_Pointer set to CH7*/
        AFE_SPI_Send_InstCMD(CMD_CH7);
        gain = 1;
        break;
    case AFE_CH_REFCAL_H: /*CMD_Pointer set to CH8*/
        AFE_SPI_Send_InstCMD(CMD_CH8);
        gain = 1;
        break;
    case AFE_CH_REFCAL_L: /*CMD_Pointer set to CH9*/
        AFE_SPI_Send_InstCMD(CMD_CH9);
        gain = 1;
        break;
    case AFE_CH_GPIO01: /*CMD_Pointer set to CH10*/
        AFE_SPI_Send_InstCMD(CMD_CH10);
        gain = 2.5;  // 10 / 4V
        break;
    case AFE_CH_HVDD: /*CMD_Pointer set to CH11*/
        AFE_SPI_Send_InstCMD(CMD_CH11);
        break;
    default: /*CMD_Pointer set to CH0*/
        AFE_SPI_Send_InstCMD(CMD_CH0);
        gain = 0.2;
        break;
    }

    // Write Channel Config
    AFE_SPI_Write(LOGIC_CH_CONTROL.CH_CONFIG0, AFE_ChannelInit[channel].CH_CONFIG0, reg_16bit);
    AFE_SPI_Write(LOGIC_CH_CONTROL.CH_CONFIG1, AFE_ChannelInit[channel].CH_CONFIG1, reg_16bit);
    AFE_SPI_Write(LOGIC_CH_CONTROL.CH_CONFIG2, AFE_ChannelInit[channel].CH_CONFIG2, reg_16bit);
    AFE_SPI_Write(LOGIC_CH_CONTROL.CH_CONFIG3, AFE_ChannelInit[channel].CH_CONFIG3, reg_16bit);

    // Start OneShot Conversion of Single Channel
    AFE_SPI_Send_InstCMD(CMD_SS);

    // SPI Polling
    // Wait until SINGLE_CH_ACTIVE = 0 -> Idle Mode / ADC finished
    vTaskDelay(pdMS_TO_TICKS(1));
    status0_regValue = 0xFFFF;

    while( (status0_regValue & SYS_STATUS0_SINGLE_CH_ACTIVE_MASK) != 0x0)
    {
        AFE_SPI_Read(SYS_CONTROL.SYS_STATUS0, reg_16bit);
        status0_regValue =  ((uint16_t)AFE_rxBuffer[2] << 8) | (uint16_t)AFE_rxBuffer[3];
    }

    // Read Data from correspondig DATA Register
    AFE_SPI_Read((0x40 + channel), reg_24bit);
    result = ((AFE_rxBuffer[2] << 16) | (AFE_rxBuffer[3] << 8) | AFE_rxBuffer[4]);

    if (channel != AFE_CH_HVDD)
    {
        // Check Sign
        if ((result & 0x800000) == 0)
        {
            voltage = ((float) result * 10) / (pow(2, 24) * gain);
        }
        else
        {
            voltage = 0;
        }
    }
    else
    {
        voltage = (((result * 4) / pow(2, 24)) + 0.25) * 32;
    }

    return voltage;

}

/********************************************************************************/

/**
 * @brief Read Die Temperature
 *
 * @return DIE temperature
 */
float AFE_ReadDieTemp(void)
{
    float temperature = 0;
    uint16_t reg_read_value = 0;

    AFE_SPI_Read(ALARM_CONTROL.DIE_TEMP, reg_16bit);
    reg_read_value =  ((uint16_t)AFE_rxBuffer[2] << 8) | (uint16_t)AFE_rxBuffer[3];

   // Check Sign
    if ((reg_read_value & 0x8000) == 0)
    {
    	temperature = (float)reg_read_value / 64;
    	DebugP_log("\t - Temperature %.1f%cC\r\n", temperature, (uint8_t)176);
    }
    else
    {
        reg_read_value = (~reg_read_value);
        reg_read_value += 1;
        temperature = (float) reg_read_value / 64;
        DebugP_log("\t - Temperature -%.1f%cC\r\n", temperature, (uint8_t)176);
    }

    return temperature;
}

/********************************************************************************/

/**
 * @brief Initialisation AFE GPIOs
 *
 */
void AFE_GpioInit(void)
{

    /* GPIO0 = analog Input
     * GPIO1 = analog Input
     * GPIO2 = not used / configured as Output
     * GPIO3 = not used / configured as Output
     * GPIO4 = not used / configured as Output
     * GPIO5 = not used / configured as Output
     * GPIO6 = not used / configured as Input
     * GPIO7 = not used / configured as Input
     * GPIO8 = not used / configured as Input
     * GPIO9 = not used / configured as Input
     */

    // Default all GPO Data 0
    AFE_SPI_Write(GPIO_CONTROL.GPO_DATA, 0, reg_16bit);

    // Connect all Pin to Pads
    AFE_SPI_Write(GPIO_CONTROL.GPIO_CONFIG1, 0xFFC0, reg_16bit);

    // GPO Driving Enable for GPO2...GPO5
    AFE_SPI_Write(GPIO_CONTROL.GPIO_CONFIG0, 0x0F00, reg_16bit);

    // GPI Read Enable for GPI06...GPI09
    AFE_SPI_Write(GPIO_CONTROL.GPIO_CONFIG2, 0xF000, reg_16bit);
}


/*******************************************************************************/

/**
 * @brief Set AFE GPIOs
 *
 * @param port AFE GPIO Port
 * @param value value to be set
 */
void AFE_GpioSet(uint8_t port, bool value)
{
    uint16_t regValue = 0;

    // read actual Data Register Value
    AFE_SPI_Read(GPIO_CONTROL.GPO_DATA, reg_16bit);

    if (crc_enabled)
    {
        regValue = ((AFE_rxBuffer[5] << 8) | AFE_rxBuffer[5]);
    }
    else
    {
        regValue = ((AFE_rxBuffer[2] << 8) | AFE_rxBuffer[3]);
    }

    switch (port)
    {
    case AFE_GPIO0:
        if (value == AFE_GPIO_HIGH)
        {
            regValue |= 0x40;
        }
        else
        {
            regValue &= 0xFFBF;
        }
        break;

    case AFE_GPIO1:
        if (value == AFE_GPIO_HIGH)
        {
            regValue |= 0x80;
        }
        else
        {
            regValue &= 0xFF7F;
        }
        break;

    case AFE_GPIO2:
        if (value == AFE_GPIO_HIGH)
        {
            regValue |= 0x100;
        }
        else
        {
            regValue &= 0xFEFF;
        }
        break;

    case AFE_GPIO3:
        if (value == AFE_GPIO_HIGH)
        {
            regValue |= 0x200;
        }
        else
        {
            regValue &= 0xFDFF;
        }
        break;

    case AFE_GPIO4:
        if (value == AFE_GPIO_HIGH)
        {
            regValue |= 0x400;
        }
        else
        {
            regValue &= 0xFBFF;
        }
        break;

    case AFE_GPIO5:
        if (value == AFE_GPIO_HIGH)
        {
            regValue |= 0x800;
        }
        else
        {
            regValue &= 0xF7FF;
        }
        break;

    case AFE_GPIO6:
        if (value == AFE_GPIO_HIGH)
        {
            regValue |= 0x1000;
        }
        else
        {
            regValue &= 0xEFFF;
        }
        break;

    case AFE_GPIO7:
        if (value == AFE_GPIO_HIGH)
        {
            regValue |= 0x2000;
        }
        else
        {
            regValue &= 0xDFFF;
        }
        break;

    case AFE_GPIO8:
        if (value == AFE_GPIO_HIGH)
        {
            regValue |= 0x4000;
        }
        else
        {
            regValue &= 0xBFFF;
        }
        break;

    case AFE_GPIO9:
        if (value == AFE_GPIO_HIGH)
        {
            regValue |= 0x8000;
        }
        else
        {
            regValue &= 0x7FFF;
        }
        break;

    default:
        /* do nothing */
        break;

    }

    AFE_SPI_Write(GPIO_CONTROL.GPO_DATA, regValue, reg_16bit);
}

/*******************************************************************************/

/**
 * @brief Read AFE GPIO
 *
 * @param port AFE GPIO Port
 * @return GPIO value
 */
uint8_t AFE_GpioRead(uint8_t port)
{
    uint16_t regValue = 0;
    uint8_t returnValue = 0;

    // read Data Register Value
    AFE_SPI_Read(GPIO_CONTROL.GPI_DATA, reg_16bit);

    if (crc_enabled)
    {
    	regValue = ((AFE_rxBuffer[4] << 8) |  AFE_rxBuffer[5]);
    }
    else
    {
        regValue = ((AFE_rxBuffer[2] << 8) |  AFE_rxBuffer[3]);
    }

    returnValue = AFE_GPIO_LOW;

    switch (port)
    {
    case AFE_GPIO0:
        if ((regValue & 0x40) > 0)
        {
            returnValue = AFE_GPIO_HIGH;
        }
        break;

    case AFE_GPIO1:
        if ((regValue & 0x80) > 0)
        {
            returnValue = AFE_GPIO_HIGH;
        }
        break;

    case AFE_GPIO2:
        if ((regValue & 0x100) > 0)
        {
            returnValue = AFE_GPIO_HIGH;
        }
        break;

    case AFE_GPIO3:
        if ((regValue & 0x200) > 0)
        {
            returnValue = AFE_GPIO_HIGH;
        }
        break;

    case AFE_GPIO4:
        if ((regValue & 0x400) > 0)
        {
            returnValue = AFE_GPIO_HIGH;
        }
        break;

    case AFE_GPIO5:
        if ((regValue & 0x800) > 0)
        {
            returnValue = AFE_GPIO_HIGH;
        }
        break;

    case AFE_GPIO6:
        if ((regValue & 0x1000) > 0)
        {
            returnValue = AFE_GPIO_HIGH;
        }
        break;

    case AFE_GPIO7:
        if ((regValue & 0x2000) > 0)
        {
            returnValue = AFE_GPIO_HIGH;
        }
        break;

    case AFE_GPIO8:
        if ((regValue & 0x4000) > 0)
        {
            returnValue = AFE_GPIO_HIGH;
        }
        break;

    case AFE_GPIO9:
        if ((regValue & 0x8000) > 0)
        {
            returnValue = AFE_GPIO_HIGH;
        }
        break;

    default:
        /* do nothing */
        break;
    }

    return returnValue;
}
