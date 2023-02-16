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
