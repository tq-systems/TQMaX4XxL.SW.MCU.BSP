
/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file afe.h
 * @copyright Copyright (c) 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author forstner
 *
 * @date 2022-02-09
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the declaration of the AFE.
 *
 */

#ifndef AFE_13388_INC_MBA117XL_AFE_H_
#define AFE_13388_INC_MBA117XL_AFE_H_

#include <stdio.h>
#include "stdint.h"
#include <stdbool.h>

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
#define CRC_CONF_REGS_ADD           0x25
#define CRC_COEF_REGS_ADD           0x26
#define CRC_TRIM_REGS_ADD           0x27
#define CH_STATUS0_ADD              0x35
#define CH_STATUS1_ADD              0x36
#define GPI_DATA_ADD                0x29
#define GPIO_CONFIG0_ADD            0x29
#define GPIO_CONFIG1_ADD            0x2B
#define GPIO_CONFIG2_ADD            0x2C
#define GPI_EDGE_POS_ADD            0x2D
#define GPI_EDGE_NEG_ADD            0x2E
#define GPO_DATA_ADD                0x2F
#define SYS_CONFIG0_ADD             0x30
#define SYS_STATUS0_ADD             0x31
#define GLOBAL_ALARM_ENABLE_ADD     0x32
#define GLOBAL_ALARM_INTERRUPT_ADD  0x33
#define DIE_TEMP_REG_ADD            0x34
#define THRS_TEMP_REG_ADD           0x37
#define SERIAL0_REG_ADD             0x7C
#define SERIAL1_REG_ADD             0x7D
#define PROD_REV_REG_ADD            0x7E

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void AFE_GpioInit(void);
void AFE_GpioSet(uint8_t port, bool value);
uint8_t AFE_GpioRead(uint8_t port);
void AFE_Enable(void);
void AFE_Init(void);
float AFE_ReadDieTemp(void);
float AFE_ReadAdcVoltage(uint8_t channel);

//BaseType_t AFE_read_all (char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);
//BaseType_t afeCommand( char *pcWriteBuffer, __size_t xWriteBufferLen, const char *pcCommandString );

#endif /* AFE_13388_INC_MBA117XL_AFE_H_ */
