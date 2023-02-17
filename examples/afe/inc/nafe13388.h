/*
 * Mba117xL_AFE.h
 *
 *  Created on: 09.02.2022
 *      Author: forstner
 */

#ifndef AFE_13388_INC_MBA117XL_AFE_H_
#define AFE_13388_INC_MBA117XL_AFE_H_

#include <stdio.h>
#include "fsl_common.h"


/*******************************************************************************
 * global defines
 ******************************************************************************/
#define AFE_CH_AI1P			0
#define AFE_CH_AI1N			1
#define AFE_CH_AI2P			2
#define AFE_CH_AI2N			3
#define AFE_CH_AI3P			4
#define AFE_CH_AI3N			5
#define AFE_CH_AI4P			6
#define AFE_CH_AI4N			7
#define AFE_CH_REFCAL_H		8
#define AFE_CH_REFCAL_L		9
#define AFE_CH_GPIO01		10
#define AFE_CH_HVDD			11


#define AFE_GPIO0		0
#define AFE_GPIO1		1
#define AFE_GPIO2		2
#define AFE_GPIO3		3
#define AFE_GPIO4		4
#define AFE_GPIO5		5
#define AFE_GPIO6		6
#define AFE_GPIO7		7
#define AFE_GPIO8		8
#define AFE_GPIO9		9

#define AFE_GPIO_HIGH	1
#define AFE_GPIO_LOW	0

#define SYS_STATUS0_SINGLE_CH_ACTIVE_MASK     	0x8000
#define SYS_STATUS0_CHIP_READY_MASK     		0x2000
#define RST_GPIO       							GPIO9
#define RST_PIN         						11U

#define ADC_DATA_RDY_GPIO						GPIO9
#define ADC_DATA_RDY_PIN						13U

/* 16-bit User Registers - System Configuration and Status */
#define CRC_CONF_REGS           0x25
#define CRC_COEF_REGS           0x26
#define CRC_TRIM_REGS           0x27
#define CH_STATUS0              0x35
#define CH_STATUS1              0x36
#define GPI_DATA                0x29
#define GPIO_CONFIG0            0x29
#define GPIO_CONFIG1            0x2B
#define GPIO_CONFIG2            0x2C
#define GPI_EDGE_POS            0x2D
#define GPI_EDGE_NEG            0x2E
#define GPO_DATA                0x2F
#define SYS_CONFIG0             0x30
#define SYS_STATUS0             0x31
#define GLOBAL_ALARM_ENABLE     0x32
#define GLOBAL_ALARM_INTERRUPT  0x33
#define DIE_TEMP_REG            0x34
#define THRS_TEMP_REG           0x37
#define SERIAL0_REG             0x7C
#define SERIAL1_REG             0x7D
#define PROD_REV_REG            0x7E

/* 24-bit User Registers */
#define CH_DATA0               0x40
#define CH_DATA1               0x41
#define CH_DATA2               0x42
#define CH_DATA3               0x43
#define CH_DATA4               0x44
#define CH_DATA5               0x45
#define CH_DATA6               0x46
#define CH_DATA7               0x47
#define CH_DATA8               0x48
#define CH_DATA9               0x49
#define CH_DATA10              0x4A
#define CH_DATA11              0x4B
#define CH_DATA12              0x4C
#define CH_DATA13              0x4D
#define CH_DATA14              0x4E
#define CH_DATA14              0x4F

#define CH_CONFIG5_OVR_THRS_0  0x50
#define CH_CONFIG5_OVR_THRS_1  0x51
#define CH_CONFIG5_OVR_THRS_2  0x52
#define CH_CONFIG5_OVR_THRS_3  0x53
#define CH_CONFIG5_OVR_THRS_4  0x54
#define CH_CONFIG5_OVR_THRS_5  0x55
#define CH_CONFIG5_OVR_THRS_6  0x56
#define CH_CONFIG5_OVR_THRS_7  0x57
#define CH_CONFIG5_OVR_THRS_8  0x58
#define CH_CONFIG5_OVR_THRS_9  0x59
#define CH_CONFIG5_OVR_THRS_10 0x5A
#define CH_CONFIG5_OVR_THRS_11 0x5B
#define CH_CONFIG5_OVR_THRS_12 0x5C
#define CH_CONFIG5_OVR_THRS_13 0x5D
#define CH_CONFIG5_OVR_THRS_14 0x5E
#define CH_CONFIG5_OVR_THRS_15 0x5F

#define CH_CONFIG6_UDR_THRS_0  0x60
#define CH_CONFIG6_UDR_THRS_1  0x61
#define CH_CONFIG6_UDR_THRS_2  0x62
#define CH_CONFIG6_UDR_THRS_3  0x63
#define CH_CONFIG6_UDR_THRS_4  0x64
#define CH_CONFIG6_UDR_THRS_5  0x65
#define CH_CONFIG6_UDR_THRS_6  0x66
#define CH_CONFIG6_UDR_THRS_7  0x67
#define CH_CONFIG6_UDR_THRS_8  0x68
#define CH_CONFIG6_UDR_THRS_9  0x69
#define CH_CONFIG6_UDR_THRS_10 0x6A
#define CH_CONFIG6_UDR_THRS_11 0x6B
#define CH_CONFIG6_UDR_THRS_12 0x6C
#define CH_CONFIG6_UDR_THRS_13 0x6D
#define CH_CONFIG6_UDR_THRS_14 0x6E
#define CH_CONFIG6_UDR_THRS_15 0x6F

#define GAIN_COEF_0            0x80
#define GAIN_COEF_1            0x81
#define GAIN_COEF_2            0x82
#define GAIN_COEF_3            0x83
#define GAIN_COEF_4            0x84
#define GAIN_COEF_5            0x85
#define GAIN_COEF_6            0x86
#define GAIN_COEF_7            0x87
#define GAIN_COEF_8            0x88
#define GAIN_COEF_9            0x89
#define GAIN_COEF_10           0x8A
#define GAIN_COEF_11           0x8B
#define GAIN_COEF_12           0x8C
#define GAIN_COEF_13           0x8D
#define GAIN_COEF_14           0x8E
#define GAIN_COEF_15           0x8F

#define OFFSET_COEF_0          0x90
#define OFFSET_COEF_1          0x91
#define OFFSET_COEF_2          0x92
#define OFFSET_COEF_3          0x93
#define OFFSET_COEF_4          0x94
#define OFFSET_COEF_5          0x95
#define OFFSET_COEF_6          0x96
#define OFFSET_COEF_7          0x97
#define OFFSET_COEF_8          0x98
#define OFFSET_COEF_9          0x99
#define OFFSET_COEF_10         0x9A
#define OFFSET_COEF_11         0x9B
#define OFFSET_COEF_12         0x9C
#define OFFSET_COEF_13         0x9D
#define OFFSET_COEF_14         0x9E
#define OFFSET_COEF_15         0x9F

#define OPT_COEF_0_REG         0xA0
#define OPT_COEF_1_REG         0xA1
#define OPT_COEF_2_REG         0xA2
#define OPT_COEF_3_REG         0xA3
#define OPT_COEF_4_REG         0xA4
#define OPT_COEF_5_REG         0xA5
#define OPT_COEF_6_REG         0xA6
#define OPT_COEF_7_REG         0xA7
#define OPT_COEF_8_REG         0xA8
#define OPT_COEF_9_REG         0xA9
#define OPT_COEF_10_REG        0xAA
#define OPT_COEF_11_REG        0xAB
#define OPT_COEF_12_REG        0xAC
#define OPT_COEF_13_REG        0xAD
#define OPT_COEF_14_REG        0xAE
#define OPT_COEF_15_REG        0xAF

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void AFE_GpioInit(void);
void AFE_GpioSet(uint8_t port, bool value);
uint8_t AFE_GpioRead(uint8_t port);
void AFE_Enable(void);
void AFE_Init(void);
void AFE_ReadDieTemp(void);
float AFE_ReadAdcVoltage(uint8_t channel);

BaseType_t AFE_read_all (char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);


#endif /* AFE_13388_INC_MBA117XL_AFE_H_ */
