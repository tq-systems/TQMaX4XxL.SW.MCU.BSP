/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * @file afe_comms.h
 * @copyright Copyright (c) 2023 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * @author Michael Bernhardt
 *
 * @date 2023-02-14
 *
 * -----------------------------------------------------------------------------
 * @brief This file contains the declaration of the AFE communication protocol.
 *
 */

#ifndef EXAMPLES_AFE_SPI_INC_SPI_FUNC_H_
#define EXAMPLES_AFE_SPI_INC_SPI_FUNC_H_

/*******************************************************************************
 * includes
 ******************************************************************************/

/* runtime */
#include "stdbool.h"
/* project */
/*******************************************************************************
 * defines
 ******************************************************************************/

/*SPI defines for ADC NAFE13388*/
#define BUFFER_SIZE          18
#define AFE_txBuffer         SPI_txBuffer
#define AFE_rxBuffer         SPI_rxBuffer

#define SPI_DUMMY_DATA       0x00

/*******************************************************************************
 * macros
 ******************************************************************************/



/*******************************************************************************
 * typedefs
 ******************************************************************************/

typedef enum afe_reg_typ_tag
{
    reg_16bit,
    reg_24bit,
} afe_reg_typ_t;

typedef enum
{
    SUCCESS                 = 0,
    INVALID_ARGUMENT        = 1,
    NO_TRANSFER_IN_PROGRESS = 2,
    FAIL                    = 3,
} status_t;

/*******************************************************************************
 * prototypes
 ******************************************************************************/

status_t AFE_SPI_Write(uint16_t reg_addr, uint32_t reg_data, afe_reg_typ_t reg_width);
status_t AFE_SPI_Read(uint16_t reg_addr, afe_reg_typ_t reg_width);
status_t AFE_SPI_RW(uint16_t reg_addr, uint32_t reg_data, afe_reg_typ_t reg_width);
status_t AFE_SPI_Send_InstCMD(uint16_t cmd_value);

/*******************************************************************************
 * global extern data
 ******************************************************************************/

extern volatile bool spi_transfer_completed;
extern bool crc_enabled;
extern uint8_t SPI_txBuffer[BUFFER_SIZE];
extern uint8_t SPI_rxBuffer[BUFFER_SIZE];

/******************************************************************************/

#endif /* EXAMPLES_AFE_SPI_INC_SPI_FUNC_H_ */

/*[EOF]************************************************************************/
