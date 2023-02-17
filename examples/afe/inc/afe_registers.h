/**
 * @file fsl_afe_registers.h
 * @author Copyright (c) 2021, TQ-Systems GmbH
 * @author Bernhard Herz author of the file <bernhard.herz@tq-group.com>
 * @date 2021-10-11
 *
 * This software code contained herein is licensed under the terms and
 * conditions of the TQ-Systems Software License Agreement Version 1.0.2.
 * You may obtain a copy of the TQ-Systems Software License Agreement in the
 * folder TQS (TQ-Systems Software Licenses) at the following website:
 * https://www.tq-group.com/Software-Lizenzbedingungen
 * In case of any license issues please contact license@tq-group.com.
 *
 * -----------------------------------------------------------------------------
 * @brief <Includes Commands and Registers for Driver NAFE13388>
 *
 * <TODO Detailed description of the file>
 */


#ifndef AFE_DRIVER_INC_FSL_AFE_REGISTERS_H_
#define AFE_DRIVER_INC_FSL_AFE_REGISTERS_H_

/*******************************************************************************
 * includes
 ******************************************************************************/
/* runtime */
//#include <stdio.h>
#include <stdint.h>
//#include "fsl_common.h"
/* project */

/* module */
//#include "MIMXRT1176_cm7.h"
//#include "fsl_debug_console.h"

/*******************************************************************************
 * defines
 ******************************************************************************/
/*Revision of NAFE13388*/
#ifndef NAFE13388_REVA
#define NAFE13388_REVB      /*rev.B*/
#endif
// NAFE13388 Evalkit is rev.B LP
/*----------------------------------------------------------------------------*/

/*Instruction commands for NAFE13388*/

/*Commands with non-return ADC Data at SPI Transfer*/

/*Selection of logical channel*/
#define CMD_CH0              0x0000        /*CMD_Pointer set to CH0*/
#define CMD_CH1              0x0001        /*CMD_Pointer set to CH1*/
#define CMD_CH2              0x0002        /*CMD_Pointer set to CH2*/
#define CMD_CH3              0x0003        /*CMD_Pointer set to CH3*/
#define CMD_CH4              0x0004        /*CMD_Pointer set to CH4*/
#define CMD_CH5              0x0005        /*CMD_Pointer set to CH5*/
#define CMD_CH6              0x0006        /*CMD_Pointer set to CH6*/
#define CMD_CH7              0x0007        /*CMD_Pointer set to CH7*/
#define CMD_CH8              0x0008        /*CMD_Pointer set to CH8*/
#define CMD_CH9              0x0009        /*CMD_Pointer set to CH9*/
#define CMD_CH10             0x000A        /*CMD_Pointer set to CH10*/
#define CMD_CH11             0x000B        /*CMD_Pointer set to CH11*/
#define CMD_CH12             0x000C        /*CMD_Pointer set to CH12*/
#define CMD_CH13             0x000D        /*CMD_Pointer set to CH13*/
#define CMD_CH14             0x000E        /*CMD_Pointer set to CH14*/
#define CMD_CH15             0x000F        /*CMD_Pointer set to CH15*/

/*
 * The instructions for system configuration
 * are encoded in 16bit SPI transaction
 */
#define CMD_ABORT            0x0010        /*Abort current channel conversion*/         /* + rev.B: Return to inital waiting state*/
#define CMD_END              0x0011        /*Complete current channel conversion*/      /* + rev.B: Return to inital waiting state*/
#define CMD_CLEAR_ALRM       0x0012        /*Clear global Alarm*/                       //ToDo //Bits at 0x33
#define CMD_CLEAR_DATA       0x0013        /*Clear CH_DATA0-15 Registers*/
#define CMD_RESET            0x0014        /*HW reset = RSTB pulled Low(*/
#define CMD_CLEAR_REG        0x0015        /*Clear user accessible registers without clock source selection*/
#define CMD_RELOAD           0x0016        /*Reload stored contents from on-chip non-volatile memory*/

/*Macro to differ between Return ADC Data and non return ADC Data Commands*/
#define NON_RETURN_CMD_BORDER   CMD_RELOAD

/*Commands with possible return ADC Data at SPI Transfer*/

#define CMD_SS               0x2000        /*Set Single Channel Single reading*/
#define CMD_SC               0x2001        /*Set Single Channel Continuous reading*/

/*
 * Start Sequencer for multichannel conversions and will start from
 * MCH_EN[0] to MCH_EN[15]. Every active Channel will be executed once.
 * Point stops at the last active channel of SSEQ sequencer.
 */
#define CMD_MM               0x2002        /*Set Multi Channel Multi reading*/

/*
 * Is similar to CMD_MM, but is in autonomous mode. So it will loop forever
 * until it is interrupted or restarted due to CMD_ABORT or CMD_END
 */
#define CMD_MC               0x2003        /*Set Multi Channel Continuous reading*/

#ifdef NAFE13388_REVB
/*
 * Auto incremention of ADC logic channel pointer.
 * After completion of last active channel ADC loops back to 1st enabled
 * channel. Termination through CMD_END or CMD_ABORT possible.
 * If SYNC Impuls or same command is used during ADC channel processing, the
 * ADC will start immediately from the same position again!!
 */
#define CMD_MS               0x2004        /*Set Multi Channel Single reading*/

#define CMD_CALC_CRC_CONFG   0x2006        /*Calculate CRC sum for all 16-bit user config. registers and save result to 0x25\h */
#endif

#define CMD_BURST_DATA       0x2005        /*Burst read of all channel CH0-CH15*/

/*Register Read via SPI*/
#define CMD_CALC_CRC_COEF    0x2007        /*Calculate CRC sum for all 24-bit CAL coefficientand save result to 0x26\h*/

/*Register Write via SPI*/
#define CMD_CALC_CRC_FAC     0x2008        /*Calculate all factory OTP trim registers*/


/* --------------------------------------------------------------------------
   -- User Registers for NAFE13388 Part1
   -------------------------------------------------------------------------- */
/*Offset for Register RW Operations*/
#define WRITE_REG_OFFSET    0x0000
#define READ_REG_OFFSET     0x4000//0x2000

/**
 * @def SERIAL0_REGISTER
 * <TODO short description of the purpose of this define>
 */
#define SERIAL0             0x007C          /*Read only Register*/

/**
 * @def SERIAL1_REGISTER
 * <TODO short description of the purpose of this define>
 */
#define SERIAL1             0x007D          /*Read only Register*/

/**
 * @def PROD_REV_REGISTER
 * <TODO short description of the purpose of this define>
 */
#define PROD_REV            0x007E          /*Read only Register*/

/*
 * CH_CONFIG0,1,2,3 are logical channel configuration channel registers. Send
 * SPI CMD_CHi (i=0..15)! CONFIG_CH_PTR[3:0] shows selected channel.
 */
/*******************************************************************************
 * CH_CONFIG0 Register Masks
 ******************************************************************************/
/*Bit Masking HV_AIP_MUX*/
#define CH_CONFIG0_HV_AIP_MASK                                         (0xF000U)
#define CH_CONFIG0_HV_AIP_SHIFT                                        (12)
/*! HV_AIP
 *  0b0000..Writing 0 to this field results in an HV_AIP connect to GND
 *  0b0001..Write 1 --> HV_AIP connect to AI1P
 *  0b0010..Write 2 --> HV_AIP connect to AI2P
 *  0b0011..Write 3 --> HV_AIP connect to AI3P
 *  0b0100..Write 4 --> HV_AIP connect to AI4P
 *  0b0101..Write 5 --> HV_AIP connect to REFCAL_H
 *  0b0110..Write 6 --> HV_AIP connect to REFCAL_L
 *  0b0111..Write 7 --> HV_AIP connect to AICOM
 *  0b1000..Write 8 --> HV_AIP connect to VEXC
 */
#define CH_CONFIG0_HV_AIP(x)                (((uint16_t)(((uint16_t)(x)) << CH_CONFIG0_HV_AIP_SHIFT)) & CH_CONFIG0_HV_AIP_MASK)


/*Bit Masking HV_AIN_MUX*/
#define CH_CONFIG0_HV_AIN_MASK                                         (0x0F00U)
#define CH_CONFIG0_HV_AIN_SHIFT                                        (8)
/*! HV_AIN
 *  0b0000..Writing 0 to this field results in an HV_AIN connect to GND
 *  0b0001..Write 1 --> HV_AIN connect to AI1N
 *  0b0010..Write 2 --> HV_AIN connect to AI2N
 *  0b0011..Write 3 --> HV_AIN connect to AI3N
 *  0b0100..Write 4 --> HV_AIN connect to AI4N
 *  0b0101..Write 5 --> HV_AIN connect to REFCAL_H
 *  0b0110..Write 6 --> HV_AIN connect to REFCAL_L
 *  0b0111..Write 7 --> HV_AIN connect to AICOM
 *  0b1000..Write 8 --> HV_AIN connect to VEXC
 */
#define CH_CONFIG0_HV_AIN(x)                (((uint16_t)(((uint16_t)(x)) << CH_CONFIG0_HV_AIN_SHIFT)) & CH_CONFIG0_HV_AIN_MASK)


/*Bit Masking CH_GAIN*/
#define CH_CONFIG0_CH_GAIN_MASK                                        (0x00E0U)
#define CH_CONFIG0_CH_GAIN_SHIFT                                       (5)
/*! Channel Gain
 *  0b000..Writing 0 to this field results in an CH_GAIN of 0.2 V/V
 *  0b001..Write 1 --> Channel Gain of 0.4 V/V
 *  0b010..Write 2 --> Channel Gain of 0.8 V/V
 *  0b011..Write 3 --> Channel Gain of 1 V/V
 *  0b100..Write 4 --> Channel Gain of 2 V/V
 *  0b101..Write 5 --> Channel Gain of 4 V/V
 *  0b110..Write 6 --> Channel Gain of 8 V/V
 *  0b111..Write 7 --> Channel Gain of 16 V/V
 */
#define CH_CONFIG0_CH_GAIN(x)               (((uint16_t)(((uint16_t)(x)) << CH_CONFIG0_CH_GAIN_SHIFT)) & CH_CONFIG0_CH_GAIN_MASK)

/*Bit Masking HV_Selection_MUX*/
#define CH_CONFIG0_HV_SEL_MASK                                         (0x0010U)
#define CH_CONFIG0_HV_SEL_SHIFT                                        (4)
/*! HV_Selection_MUX
 *  0b0..Writing 0 to this field results in an HV_SEL connect to LVSIG_IN
 *  0b1..Write 1 --> HV_SEL connect to HV_AIP-HV_AIN
 */
#define CH_CONFIG0_HV_SEL(x)                (((uint16_t)(((uint16_t)(x)) << CH_CONFIG0_HV_SEL_SHIFT)) & CH_CONFIG0_HV_SEL_MASK)


/*Bit Masking LVSIG_IN_MUX*/
#define CH_CONFIG0_LVSIG_IN_MASK                                       (0x000EU)
#define CH_CONFIG0_LVSIG_IN_SHIFT                                      (1)
/*! Channel Gain
 *  0b0000..Writing 0 to this field results in an LVSIG_IN(pos,neg) connect to (REF/2,REF/2)
 *  0b0010..Write 1 --> LVSIG_IN(pos,neg) connect to (GPIO0,GPIO1)
 *  0b0100..Write 2 --> LVSIG_IN(pos,neg) connect to (REF_Coarse,REF/2)
 *  0b0110..Write 3 --> LVSIG_IN(pos,neg) connect to (VADD,REF/2)
 *  0b1000..Write 4 --> LVSIG_IN(pos,neg) connect to (GPIO0,GPIO1)
 *  0b1010..Write 5 --> LVSIG_IN(pos,neg) connect to (REF/2,VHSS)
 */
#define CH_CONFIG0_LVSIG_IN(x)              (((uint16_t)(((uint16_t)(x)) << CH_CONFIG0_LVSIG_IN_SHIFT)) & CH_CONFIG0_LVSIG_IN_MASK)


#ifdef NAFE13388_REVB
/*Bit Masking Proprietary Channel Temperature Coefficient Compensation Bypass*/
#define CH_CONFIG0_TCC_BYP_MASK                                        (0x0001U)
#define CH_CONFIG0_TCC_BYP_SHIFT                                       (0)
/*! TCC_BYP
 *  0b0..Writing 0 to this field results in an TCC_BYP = enable,ON
 *  0b1..Write 1 --> TCC_BYP = disable,OFF
 */
#define CH_CONFIG0_TCC_BYP(x)               (((uint16_t)(((uint16_t)(x)) << CH_CONFIG0_TCC_BYP_SHIFT)) & CH_CONFIG0_TCC_BYP_MASK)


#endif

/*******************************************************************************
 * CH_CONFIG1 Register Masks
 ******************************************************************************/
/*Bit Masking CH_CAL_GAIN_OFFSET*/
#define CH_CONFIG1_CH_CAL_GAIN_OFFSET_MASK                             (0xF000U)
#define CH_CONFIG1_CH_CAL_GAIN_OFFSET_SHIFT                            (12)
/*! CH_CAL_GAIN_OFFSET
 *  Selection 1 of 16 calibrated gain and offset coefficient pairs in the Calibrated Channel
 */ //ToDO
#define CH_CONFIG1_CH_CAL_GAIN_OFFSET(x)    (((uint16_t)(((uint16_t)(x)) << CH_CONFIG1_CH_CAL_GAIN_OFFSET_SHIFT)) & CH_CONFIG1_CH_CAL_GAIN_OFFSET_MASK)


/*Bit Masking Channel Over-/Under-range Threshold*/
#define CH_CONFIG1_CH_THRS_MASK                                        (0x0F00U)
#define CH_CONFIG1_CH_THRS_SHIFT                                       (8)
//ToDO Comments with TABLE_ADC_RANGE_THRS
#define CH_CONFIG1_CH_THRS(x)               (((uint16_t)(((uint16_t)(x)) << CH_CONFIG1_CH_THRS_SHIFT)) & CH_CONFIG1_CH_THRS_MASK)


/*Bit Masking ADC_DATA_RATE*/
#define CH_CONFIG1_ADC_DATA_RAT_MASK                                   (0x00F8U)
#define CH_CONFIG1_ADC_DATA_RAT_SHIFT                                  (2)
//ToDO Comments with TABLE_ADC_RATE
#define CH_CONFIG1_CH_THRS_OFFSET(x)        (((uint16_t)(((uint16_t)(x)) << CH_CONFIG1_CH_CAL_GAIN_OFFSET_SHIFT)) & CH_CONFIG1_CH_CAL_GAIN_OFFSET_MASK)


/*Bit Masking ADC_SINC*/
#define CH_CONFIG1_ADC_SINC_MASK                                       (0x0007U)
#define CH_CONFIG1_ADC_SINC_STIFT                                      (0)
/*! ADC_SINC
 *  0b000..Writing 0 to this field results in an ADC_SINC = SINC4
 *  0b001..Write 1 --> ADC_SINC = SINC4+1
 *  0b010..Write 2 --> ADC_SINC = SINC4+2
 *  0b011..Write 3 --> ADC_SINC = SINC4+3
 *  0b100..Write 4 --> ADC_SINC = SINC4+4
 *  0b101..Write 5 --> ADC_SINC = SINC4
 *  0b110..Write 6 --> ADC_SINC = SINC4
 *  0b111..Write 5 --> ADC_SINC = SINC4
 */
#define CH_CONFIG1_ADC_SINC(x)              (((uint16_t)(((uint16_t)(x)) << CH_CONFIG1_ADC_SINC_STIFT)) & CH_CONFIG1_ADC_SINC_MASK)


/*******************************************************************************
 * CH_CONFIG2 Register Masks
 ******************************************************************************/
#ifdef NAFE13388_REVA
/*Bit Masking Channel Delay*/
#define CH_CONFIG2_CH_DELAY_MASK                                       (0xF000U)
#define CH_CONFIG2_CH_DELAY_SHIFT                                      (12)
/*! ToDO Programmable Delay Table
 *  0b0000..Writing 0 to this field results in an
 *  0b0001..
 */
#define CH_CONFIG2_CH_DELAY(x)              (((uint16_t)(((uint16_t)(x)) << CH_CONFIG2_CH_DELAY_SHIFT)) & CH_CONFIG2_CH_DELAY_MASK)


/*Bit Masking ADC Settling Mode*/
#define CH_CONFIG2_ADC_NORMAL_SETTLING_MASK                            (0x0800U)
#define CH_CONFIG2_ADC_NORMAL_SETTLING_SHIFT                           (11)
/*! ADC Settling Mode
 *  0b0..Writing 0 to this field results in an Single-Sycle Settling (SCS)
 *  0b1..Write 1 --> Normal Settling (NS)
 */
#define CH_CONFIG2_ADC_NORMAL_SETTLING(x)       (((uint16_t)(((uint16_t)(x)) << CH_CONFIG2_ADC_NORMAL_SETTLING_SHIFT)) & CH_CONFIG2_ADC_NORMAL_SETTLING_MASK)

/*Bit Masking ADC Filter Reset at Start of Conversion*/
#define CH_CONFIG2_ADC_FILTER_RESET_MASK                               (0x0400U)
#define CH_CONFIG2_ADC_FILTER_RESET_SHIFT                              (10)
/*! ADC Filter Reset
 *  0b0..Writing 0 to this field results in an "hold of digital filters data from previous conversion"
 *  0b1..Write 1 --> Reset digital filter
 */
#define CH_CONFIG2_ADC_FILTER_RESET(x)         (((uint16_t)(((uint16_t)(x)) << CH_CONFIG2_ADC_FILTER_RESET_SHIFT)) & CH_CONFIG2_ADC_FILTER_RESET_MASK)


/*Bit Masking Enable Input Channel Level chopping with 2 ADC Conversions*/
#define CH_CONFIG2_CH_CHOP_MASK                                        (0x0040U)
#define CH_CONFIG2_CH_CHOP_SHIFT                                       (7)
/*! CH_CHOP
 *  0b0..Writing 0 to this field results in Normal mode
 *  0b1..Write 1 --> Precision mode with 2 conversions chopping
 */
#define CH_CONFIG2_CH_CHOP(x)                  (((uint16_t)(((uint16_t)(x)) << CH_CONFIG2_CH_CHOP_SHIFT)) & CH_CONFIG2_CH_CHOP_MASK)


/*Bit Masking Excitation V/I Source Polarity chopping with 2 ADC Conversions*/
#define CH_CONFIG2_VIEX_CHOP_MASK                                      (0x0020U)
#define CH_CONFIG2_VIEX_CHOP_SHIFT                                     (6)
/*! VIEX_CHOP
 *  0b0..Writing 0 to this field results in Normal mode
 *  0b1..Write 1 --> Conversions with polarity chopping
 */
#define CH_CONFIG2_VIEX_CHOP(x)                  (((uint16_t)(((uint16_t)(x)) << CH_CONFIG2_VIEX_CHOP_SHIFT)) & CH_CONFIG2_VIEX_CHOP_MASK)
#endif

#ifdef NAFE13388_REVB
/*Bit Masking Channel Delay*/
#define CH_CONFIG2_CH_DELAY_MASK                                       (0xFC00U)
#define CH_CONFIG2_CH_DELAY_SHIFT                                      (10)
/*! ToDO Programmable Delay Table
 *  0b000000..Writing 0 to this field results in an
 *  0b000001..
 */
#define CH_CONFIG2_CH_DELAY(x)              (((uint16_t)(((uint16_t)(x)) << CH_CONFIG2_CH_DELAY_SHIFT)) & CH_CONFIG2_CH_DELAY_MASK)


/*Bit Masking ADC Settling Mode*/
#define CH_CONFIG2_ADC_NORMAL_SETTLING_MASK                            (0x0100U)
#define CH_CONFIG2_ADC_NORMAL_SETTLING_SHIFT                           (8)
/*! ADC Settling Mode
 *  0b0..Writing 0 to this field results in an Single-Sycle Settling (SCS)
 *  0b1..Write 1 --> Normal Settling (NS)
 */
#define CH_CONFIG2_ADC_NORMAL_SETTLING(x)       (((uint16_t)(((uint16_t)(x)) << CH_CONFIG2_ADC_NORMAL_SETTLING_SHIFT)) & CH_CONFIG2_ADC_NORMAL_SETTLING_MASK)


/*Bit Masking ADC Filter Reset at Start of Conversion*/
#define CH_CONFIG2_ADC_FILTER_RESET_MASK                               (0x0080U)
#define CH_CONFIG2_ADC_FILTER_RESET_SHIFT                              (7)
/*! ADC Filter Reset
 *  0b0..Writing 0 to this field results in an "hold of digital filters data from previous conversion"
 *  0b1..Write 1 --> Reset digital filter
 */
#define CH_CONFIG2_ADC_FILTER_RESET(x)         (((uint16_t)(((uint16_t)(x)) << CH_CONFIG2_ADC_FILTER_RESET_SHIFT)) & CH_CONFIG2_ADC_FILTER_RESET_MASK)

#endif


/*******************************************************************************
 *CH_CONFIG3 Register Masks
 ******************************************************************************/
/*Bit Masking Excitation V/I Source Selection Voltage/Current*/
#define CH_CONFIG3_VIEX_VI_MASK                                        (0x8000U)
#define CH_CONFIG3_VIEX_VI_SHIFT                                       (14)
/*! VIEX_VI
 *  0b0..Writing 0 to this field selects Voltage Source
 *  0b1..Write 1 --> Current Source
 */
#define CH_CONFIG3_VIEX_VI(x)                (((uint16_t)(((uint16_t)(x)) << CH_CONFIG3_VIEX_VI_SHIFT)) & CH_CONFIG3_VIEX_VI_MASK)


/*Bit Masking Excitation V/I Source Polarity*/
#define CH_CONFIG3_VIEX_POL_MASK                                       (0x4000U)
#define CH_CONFIG3_VIEX_POL_SHIFT                                      (13)
/*! VIEX_VI
 *  0b0..Writing 0 to this field selects Positive Polarity
 *  0b1..Write 1 --> Negative Polarity
 */
#define CH_CONFIG3_VIEX_POL(x)                (((uint16_t)(((uint16_t)(x)) << CH_CONFIG3_VIEX_POL_SHIFT)) & CH_CONFIG3_VIEX_POL_MASK)


/*Bit Masking Excitation V/I Source Magnitude*/
#define CH_CONFIG3_VIEX_MAG_MASK                                       (0x3C00U)
#define CH_CONFIG3_VIEX_MAG_SHIFT                                      (9)
/*! VIEX_VI ToDO
 *  0b0..Writing 0 to this field
 *  0b1..Write 1 -->
 */
#define CH_CONFIG3_VIEX_MAG(x)                (((uint16_t)(((uint16_t)(x)) << CH_CONFIG3_VIEX_MAG_SHIFT)) & CH_CONFIG3_VIEX_MAG_MASK)


/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/*ToDO NAFE13388 rev.A Mistake at Datasheet Register Map p.119????*/
/*
 * If VIEX_CHOP and CH_CHOP are both set,this particular bit is ignored
 * Refer to CH_CONFIG2 Register!
 */
/*Bit Masking Enable Excitation V/I Source Polarity chopping*/
#define CH_CONFIG3_VIEX_CHOP_MASK                                      (0x0040U)
#define CH_CONFIG3_VIEX_CHOP_SHIFT                                     (5)
/*! VIEX_VI
 *  0b0..Writing 0 to this field select Normal
 *  0b1..Write 1 --> Select Conversion with polarity chopping
 */
#define CH_CONFIG3_VIEX_CHOP(x)               (((uint16_t)(((uint16_t)(x)) << CH_CONFIG3_VIEX_CHOP_SHIFT)) & CH_CONFIG3_VIEX_CHOP_MASK)


/*Bit Masking Excitation V/I Source applied at AIxP Pin*/
#define CH_CONFIG3_VIEX_AIP_EN_MASK                                    (0x0038U)
#define CH_CONFIG3_VIEX_AIP_EN_SHIFT                                   (3)
/*! VIEX_AIP_EN
 *  0b000..Writing 0 to this field selects input pin none
 *  0b001..Write 1 --> Input Pin AI1P
 *  0b010..Write 2 --> Input Pin AI2P
 *  0b011..Write 3 --> Input Pin AI3P
 *  0b100..Write 4 --> Input Pin AI4P
 */
#define CH_CONFIG3_VIEX_AIP_EN(x)               (((uint16_t)(((uint16_t)(x)) << CH_CONFIG3_VIEX_AIP_EN_SHIFT)) & CH_CONFIG3_VIEX_AIP_EN_MASK)


/*Bit Masking Excitation V/I Source applied at AIxN Pin*/
#define CH_CONFIG3_VIEX_AIN_EN_MASK                                    (0x0007U)
#define CH_CONFIG3_VIEX_AIN_EN_SHIFT                                   (0)
/*! VIEX_AIN_EN
 *  0b000..Writing 0 to this field selects input pin none
 *  0b001..Write 1 --> Input Pin AI1N
 *  0b010..Write 2 --> Input Pin AI2N
 *  0b011..Write 3 --> Input Pin AI3N
 *  0b100..Write 4 --> Input Pin AI4N
 */
#define CH_CONFIG3_VIEX_AIN_EN(x)              (((uint16_t)(((uint16_t)(x)) << CH_CONFIG3_VIEX_AIN_EN_SHIFT)) & CH_CONFIG3_VIEX_AIN_EN_MASK)

#ifdef NAFE13388_REVB
/*Bit Masking Excitation V/I Source Enable VEXC Voltage*/
#define CH_CONFIG3_VIEX_EN_MASK                                        (0x0200U)
#define CH_CONFIG3_VIEX_EN_SHIFT                                       (9)
/*! VIEX_EN
 *  0b0..Writing 0 to this field disables VEXC voltage output
 *  0b1..Write 1 --> Enable VEXC voltage output
 */
#define CH_CONFIG3_VIEX_AIN_EN(x)              (((uint16_t)(((uint16_t)(x)) << CH_CONFIG3_VIEX_AIN_EN_SHIFT)) & CH_CONFIG3_VIEX_AIN_EN_MASK)
#endif

/*******************************************************************************
 *CH_CONFIG4 Register Masks
 ******************************************************************************/
/*Bit Masking for Logic Channel x for ADC conversion in MC Mode*/
#define CH_CONFIG4_MCH_EN_MASK                                         (0xFFFFU)
#define CH_CONFIG4_MCH_EN_SHIFT                                        (0)
/*! VIEX_AIN_EN
 *  Bit settings 0=disable, 1=enable --> CH15 is bit15, CH0 is bit0
 *  Example:
 *  0b1000000000000000 -> CH15 is enable for MC Mode
 *  0b0000000000000010 -> CH1 is enabled for MC Mode
 */
#define CH_CONFIG4_MCH_EN(x)              (((uint16_t)(((uint16_t)(x)) << CH_CONFIG4_MCH_EN_SHIFT)) & CH_CONFIG4_MCH_EN_MASK)


/*******************************************************************************
 *SYS_CONFIG0 Register Masks
 ******************************************************************************/
#ifdef NAFE13388_REVA
/*Bit Masking for Current Pointer of 16 Data Channels*/
#define SYS_CONFIG0_CURR_POINTER_MASK                                  (0xF000U)
#define SYS_CONFIG0_CURR_POINTER_SHIFT                                 (12)
/*! CURRENT_POINTER
 *  After reset the Pointer is at Channel0
 *  0b0000..Writing 0 to this field selects Channel0
 *  0b0001..Write 1 --> Channel1
 *  0b0010..Write 2 --> Channel2
 *  0b0011..Write 3 --> Channel3
 *  0b0100..Write 4 --> Channel4
 *  ...........
 */
#define SYS_CONFIG0_CURR_POINTER(x)              (((uint16_t)(((uint16_t)(x)) << SYS_CONFIG0_CURR_POINTER_SHIFT)) & SYS_CONFIG0_CURR_POINTER_MASK)


/*Bit Masking DRDYB pin behavior*/
#define SYS_CONFIG0_DRDYB_PIN_SEQ_MASK                                 (0x0010U)
#define SYS_CONFIG0_DRDYB_PIN_SEQ_SHIFT                                (4)
/*! DRDYB_PIN_SEQ (Especially for sequencer Mode)
 *  0b0..Write 0 --> Produce falling on every channel conversion done
 *  0b1..Write 1 --> Produce falling edge only when the sequ. done with last enabled channel conversion
 */
#define SYS_CONFIG0_DRDYB_PIN_SEQ(x)              (((uint16_t)(((uint16_t)(x)) << SYS_CONFIG0_DRDYB_PIN_SEQ_SHIFT)) & SYS_CONFIG0_DRDYB_PIN_SEQ_MASK)
#endif


/*Bit Masking Int./External Voltage Reference*/
#define SYS_CONFIG0_REF_SEL_MASK                                       (0x0C00U)
#define SYS_CONFIG0_REF_SEL_SHIFT                                      (10)
/*! REF_SEL
 *  After reset the Pointer is at Channel0
 *  0b00..Writing 0 to this field select both REF_BYP and REF_ADC (Internal 2.5V REF_INT)
 *  0b01..Write 1 --> REF_BYP = 2.5V(REF_INT) / REF_ADC = 2.5V(REF_EXT)
 *  0b10..Write 2 --> REF_BYP = 2.5V(REF_EXT) / REF_ADC = 2.5V(REF_INT)
 *  0b11..Write 3 --> REF_BYP = 2.5V(REF_EXT) / REF_ADC = 2.5V(REF_EXT)
 */
#define SYS_CONFIG0_REF_SEL(x)                  (((uint16_t)(((uint16_t)(x)) << SYS_CONFIG0_REF_SEL_SHIFT)) & SYS_CONFIG0_REF_SEL_MASK)


/*Bit Masking Clock source Selection*/
#define SYS_CONFIG0_CK_SRC_SEL_MASK                                    (0x0300U)
#define SYS_CONFIG0_CK_SRC_SEL_SHIFT                                   (8)
/*! CK_SRC_SEL
 *  0b00..Write 0 --> Internal Clock
 *  0b01..Write 1 --> Internal Clock, disable crystal oscillator circuit
 *  0b10..Write 2 --> Ext. 18.432MHz clock at XI pin, disable crystal oscillator circuit
 *  0b11..Write 3 --> 18.432MHz crystal is installed at XI/XO pins
 */
#define SYS_CONFIG0_CK_SRC_SEL(x)              (((uint16_t)(((uint16_t)(x)) << SYS_CONFIG0_CK_SRC_SEL_SHIFT)) & SYS_CONFIG0_CK_SRC_SEL_MASK)


/*Bit Masking CRC Activation*/
#define SYS_CONFIG0_CRC_EN_MASK                                        (0x0080U)
#define SYS_CONFIG0_CRC_EN_SHIFT                                       (7)
/*! CRC_EN
 *  0b0..Write 0 --> CRC disabled
 *  0b1..Write 1 --> CRC enabled
 */
#define SYS_CONFIG0_CRC_EN(x)                   (((uint16_t)(((uint16_t)(x)) << SYS_CONFIG0_CRC_EN_SHIFT)) & SYS_CONFIG0_CRC_EN_MASK)


/*Bit Masking Status to ADC data*/
#define SYS_CONFIG0_STATUS_EN_MASK                                     (0x0040U)
#define SYS_CONFIG0_STATUS_EN_SHIFT                                    (6)
/*! STATUS_EN
 *  Prepend 8-bit status bits to ADC data of enabled channels, MCH_EN[i] = 1
 *  In MC-Mode-Reading 8 bits status is OR'd of enabled channels when in
 *  data ouput burst
 *  status_bits(MSB to LSB):
 *  over-load, under-load, over-range, under-range, over-temp, global_alarm,
 *  over-voltage, CRC error
 */
#define SYS_CONFIG0_STATUS_EN(x)                (((uint16_t)(((uint16_t)(x)) << SYS_CONFIG0_STATUS_EN_SHIFT)) & SYS_CONFIG0_STATUS_EN_MASK)


/*Bit Masking ADC synchronization mode with SYNC pulse*/
#define SYS_CONFIG0_ADC_SYNC_MASK                                      (0x0020U)
#define SYS_CONFIG0_ADC_SYNC_SHIFT                                     (5)
/*! ADC_SYNC
 *
 * NAFE REV.A:
 *  !!Only with Single conversion mode!!
 *  --> in MC-Mode SYNC pulse is ignored
 *  0b0..Write 0 --> Internal Timing
 *  0b1..Write 1 --> ADC synchronizated via SYNC pulse at rising edge in SC-Mode
 *
 *  NAFE REV.B:
 *  0b0..Write 0 --> ADC Conversation triggerd by SPI CMD
 *  0b1..Write 1 --> ADC Conversation triggerd by SYNC impulse and SPI_CMD only decides which Conversion Mode is executed
 */
#define SYS_CONFIG0_ADC_SYNC(x)                 (((uint16_t)(((uint16_t)(x)) << SYS_CONFIG0_ADC_SYNC_SHIFT)) & SYS_CONFIG0_ADC_SYNC_MASK)


/*Bit Masking Global alarm Interrupt*/
#define SYS_CONFIG0_GLOBAL_ALRM_STICKY_MASK                            (0x0008U)
#define SYS_CONFIG0_GLOBAL_ALRM_STICKY_SHIFT                           (3)
/*! GLOBAL_ALRM_STICKY
 *  0b0..Write 0 --> Non-sticky, read live status
 *  0b1..Write 1 --> Sticky, write 1 to clear a secific bit
 */
#define SYS_CONFIG0_GLOBAL_ALRM_STICKY(x)       (((uint16_t)(((uint16_t)(x)) << SYS_CONFIG0_GLOBAL_ALRM_STICKY_SHIFT)) & SYS_CONFIG0_GLOBAL_ALRM_STICKY_MASK)


/*Bit Masking SPI_DOUT_DRIVE*/
#define SYS_CONFIG0_SPI_DOUT_DRIVE_MASK                                (0x0004U)
#define SYS_CONFIG0_SPI_DOUT_DRIVE_SHIFT                               (2)
/*! SPI_DOUT_DRIVE
 *  Increase DOUT output drive if high capacitance loading
 */
#define SYS_CONFIG0_SPI_DOUT_DRIVE(x)           (((uint16_t)(((uint16_t)(x)) << SYS_CONFIG0_SPI_DOUT_DRIVE_SHIFT)) & SYS_CONFIG0_SPI_DOUT_DRIVE_MASK)


/*Bit Masking INTB_DRIVER_TYPE*/
#define SYS_CONFIG0_INTB_DRIVER_TYPE_MASK                              (0x0002U)
#define SYS_CONFIG0_INTB_DRIVER_TYPE_SHIFT                             (1)
/*! INTB_DRIVER_TYPE
 *  INTB pin driver type:
 *  0b0..Write 0 --> 100kOhm pull up with open drain
 *  0b1..Write 1 --> CMOS push pull
 */
#define SYS_CONFIG0_INTB_DRIVER_TYPE(x)         (((uint16_t)(((uint16_t)(x)) << SYS_CONFIG0_INTB_DRIVER_TYPE_SHIFT)) & SYS_CONFIG0_INTB_DRIVER_TYPE_MASK)


/*Bit Masking Rout of CRC_ERROR*/
#define SYS_CONFIG0_CRC_ERROR_ON_GPIO2_MASK                            (0x0001U)
#define SYS_CONFIG0_CRC_ERROR_ON_GPIO2_SHIFT                           (0)
/*! CRC_ERROR_ON_GPIO2
 *  Routing of CRC_ERROR interrupt to GPIO2
 *  0b0..Write 0 --> Normal GPIO function
 *  0b1..Write 1 --> Output CRC_ERROR to GPIO2 pin, active high
 */
#define SYS_CONFIG0_CRC_ERROR_ON_GPIO2(x)       (((uint16_t)(((uint16_t)(x)) << SYS_CONFIG0_CRC_ERROR_ON_GPIO2_SHIFT)) & SYS_CONFIG0_CRC_ERROR_ON_GPIO2_MASK)

#ifdef NAFE13388_REVB
/*Bit Masking DRDY pulse with duration (# of SYSCLCK cycle)*/
#define SYS_CONFIG0_DRDY_PWDT_MASK                                     (0x8000U)
#define SYS_CONFIG0_DRDY_PWDT_SHIFT                                    (15)
/*! DRDY_PWDT
 *  0b0..Write 0 --> Value 2
 *  0b1..Write 1 --> Value 8
 */
#define SYS_CONFIG0_DRDY_PWDT(x)                (((uint16_t)(((uint16_t)(x)) << SYS_CONFIG0_DRDY_PWDT_SHIFT)) & SYS_CONFIG0_DRDY_PWDT_MASK)


/*Bit Masking ADC Data register wide*/
#define SYS_CONFIG0_ADC_DATA_OUT_16BIT_MASK                            (0x4000U)
#define SYS_CONFIG0_ADC_DATA_OUT_16BIT_SHIFT                           (14)
/*! ADC_DATA_OUT_16BIT
 *  0b0..Write 0 --> ADC Data register 24bit
 *  0b1..Write 1 --> ADC Data register 16bit
 */
#define SYS_CONFIG0_ADC_DATA_OUT_16BIT(x)       (((uint16_t)(((uint16_t)(x)) << SYS_CONFIG0_ADC_DATA_OUT_16BIT_SHIFT)) & SYS_CONFIG0_ADC_DATA_OUT_16BIT_MASK)


/*Bit Masking DRDYB pin edge behavior*/
#define SYS_CONFIG0_DRDYB_PIN_EDGE_MASK                                (0x0010U)
#define SYS_CONFIG0_DRDYB_PIN_EDGE_SHIFT                               (4)
/*! DRDYB_PIN_EDGE
 *  DRDY pin set for CMD_MS, CMD_MM, CMD_MC reading modes:
 *  0b0..Write 0 --> Produce rising edge on every channel conversion done
 *  0b1..Write 1 --> Produce rising edge only when the last enabled channel conversion is done
 */
#define SYS_CONFIG0_DRDYB_PIN_EDGE(x)           (((uint16_t)(((uint16_t)(x)) << SYS_CONFIG0_DRDYB_PIN_EDGE_SHIFT)) & SYS_CONFIG0_DRDYB_PIN_EDGE_MASK)
#endif

/*******************************************************************************
 *SYS_STATUS0 Register Masks
 ******************************************************************************/
#ifdef NAFE13388_REVA
#endif

/*******************************************************************************
 *GLOBAL_ALRM_ENABLE Register Masks
 ******************************************************************************/
/*
 * 0b0..Write 0 --> ALRM is disabled
 * 0b1..Write 1 --> ALRm is enabled
 */
#define OVER_TEMP_ALRM(x)         (((uint16_t)(((uint16_t)(x)) << 15)) & 0x8000)  // Warning >145°C
#define HVDD_ALRM(x)              (((uint16_t)(((uint16_t)(x)) << 14)) & 0x4000)  // HVDD supply detect below preset threshold
#define HVSS_ALRM(x)              (((uint16_t)(((uint16_t)(x)) << 13)) & 0x2000)  // HVSS supply detect below preset threshold
#define DVDD_ALRM(x)              (((uint16_t)(((uint16_t)(x)) << 12)) & 0x1000)  // DVDD supply detect below preset threshold
/*bit 11 is reserved*/
#define GPI_POS_ALRM(x)           (((uint16_t)(((uint16_t)(x)) << 10)) & 0x0400)  // Rising edge detected at an of GPI pins
#define GPI_NEG_ALRM(x)           (((uint16_t)(((uint16_t)(x)) <<  9)) & 0x0200)  // Falling edge detected at an of GPI pins
#define CONFIG_ERROR_ALRM(x)      (((uint16_t)(((uint16_t)(x)) <<  8)) & 0x0100)  // Register configuration error
#define OVRRING_ALRM(x)           (((uint16_t)(((uint16_t)(x)) <<  7)) & 0x0080)  // On or more data channel in over-range
#define UNDRING_ALRM(x)           (((uint16_t)(((uint16_t)(x)) <<  6)) & 0x0040)  // On or more data channel in under-range
#define OVRLOAD_ALRM(x)           (((uint16_t)(((uint16_t)(x)) <<  5)) & 0x0020)  // On or more data channel is over-loaded or under-loaded
#define EXTCLK_FRQ_ALRM(x)        (((uint16_t)(((uint16_t)(x)) <<  4)) & 0x0010)  // XTAL or EXTCLK freq. varies with internal CLK
#define PGA_OV_ALRM(x)            (((uint16_t)(((uint16_t)(x)) <<  3)) & 0x0008)  // On or more data channel is over-voltage stressing PGA

#define TEMP_ALRM(x)              (((uint16_t)(((uint16_t)(x)) <<  0)) & 0x0001)  // Programmable Temp. alarm (Threshold set at THRES_TEMP register bits)

#ifdef NAFE13388_REVA
#define VIEX_OV_ALRM(x)           (((uint16_t)(((uint16_t)(x)) <<  2)) & 0x0004)  // Excitation voltage source is over loaded
#define VIEX_OI_ALRM(x)           (((uint16_t)(((uint16_t)(x)) <<  1)) & 0x0002)  // Excitation current source is over loaded
#endif

#ifdef NAFE13388_REVB
#define VIEX_OVLD_ALRM(x)         (((uint16_t)(((uint16_t)(x)) <<  2)) & 0x0004)  // Excitation source is over loaded
/*bit 1 is reserved*/
#endif

/*******************************************************************************
 *GLOBAL_ALRM_INT Register Masks
 ******************************************************************************/
/*
 * Bit clear behavior controlled by GLOBAL_ALRM_STICKY bit at SYS_CONFIG0 Reg.
 * Default values are: 0x0000
 */
#define OVER_TEMP_INT(x)          (((uint16_t)(((uint16_t)(x)) << 15)) & 0x8000)
#define HVDD_INT(x)               (((uint16_t)(((uint16_t)(x)) << 14)) & 0x4000)
#define HVSS_INT(x)               (((uint16_t)(((uint16_t)(x)) << 13)) & 0x2000)
#define DVDD_INT(x)               (((uint16_t)(((uint16_t)(x)) << 12)) & 0x1000)
/*bit 11 is reserved*/
#define GPI_POS_INT(x)            (((uint16_t)(((uint16_t)(x)) << 10)) & 0x0400)
#define GPI_NEG_INT(x)            (((uint16_t)(((uint16_t)(x)) <<  9)) & 0x0200)
#define CONFIG_ERROR_INT(x)       (((uint16_t)(((uint16_t)(x)) <<  8)) & 0x0100)
#define OVRRING_INT(x)            (((uint16_t)(((uint16_t)(x)) <<  7)) & 0x0080)
#define UNDRING_INT(x)            (((uint16_t)(((uint16_t)(x)) <<  6)) & 0x0040)
#define OVRLOAD_INT(x)            (((uint16_t)(((uint16_t)(x)) <<  5)) & 0x0020)
#define EXTCLK_FRQ_INT(x)         (((uint16_t)(((uint16_t)(x)) <<  4)) & 0x0010)
#define PGA_OV_INT(x)             (((uint16_t)(((uint16_t)(x)) <<  3)) & 0x0008)

#define TEMP_INT(x)               (((uint16_t)(((uint16_t)(x)) <<  0)) & 0x0001)

#ifdef NAFE13388_REVA
#define VIEX_OV_INT(x)            (((uint16_t)(((uint16_t)(x)) <<  2)) & 0x0004)
#define VIEX_OI_INT(x)            (((uint16_t)(((uint16_t)(x)) <<  1)) & 0x0002)
#endif

#ifdef NAFE13388_REVB
#define VIEX_OVLD_INT(x)          (((uint16_t)(((uint16_t)(x)) <<  2)) & 0x0004)
/*bit 1 is reserved*/
#endif

/*******************************************************************************
 * typedefs
 ******************************************************************************/
/*User Register NAFE13388 Part2*/
/*For detail Discription refer to Register Map NAFE13388.v43 p.118*/

/**
 * @struct __CH_CHONFIG_t
 * <Configuration/ Status Register for MUX/ PGA/ SINC/ ADC / OVR| UDR CH Range>
 */
typedef struct __CH_CONFIG_t
{
    uint16_t CH_CONFIG0;                /*Config HV/LV-MUX + PGA*/
    uint16_t CH_CONFIG1;                /*Config ADC + SINC + CAL_GAIN_OFFSET*/
    uint16_t CH_CONFIG2;                /*Config ADC + Delay*/
    uint16_t CH_CONFIG3;                /*Config Excitation V/I Source*/
    uint16_t CH_CONFIG4;                /*Enable CH Config ADC Multi Channel*/

    uint16_t CH_STATUS0;                /*Channel over range register*/
    uint16_t CH_STATUS1;                /*Channel under range register*/

} CH_CONFIG_t;


/**
 * @struct __CRC_REG_t
 * <cyclic redundancy check registers>
 */
typedef struct __CRC_REG_t
{
    uint16_t CRC_CONF_REGS;             /*CRC result of all configuration*/
    uint16_t CRC_COEF_REGS;             /*CRC result of all user coeffiecient*/
    uint16_t CRC_TRIM_REGS;             /*CRC result of factroy trim OTP */

} CRC_REG_t;

/**
 * @struct __GPIO_REG_t
 * <GPIO Control-, Data-, Edge-registers>
 */
typedef struct __GPIO_REG_t
{
    uint16_t GPI_DATA;                  /*GPI Data detection Register*/
    uint16_t GPIO_CONFIG0;              /*GPIO Driving Enable GPIO9-15 is bit6*/
    uint16_t GPIO_CONFIG1;              /*GPIO Connect to Pin GPIO9-15 is bit6*/
    uint16_t GPIO_CONFIG2;              /*GPI Read Enable GPIO9-15 is bit6*/
    uint16_t GPI_EDGE_POS;              /*GPI Pos. Edge -> Clear after readback or CMD_CLEAR_ALARM*/
    uint16_t GPI_EDGE_NEG;              /*GPI Neg. Edge -> Clear after readback or CMD_CLEAR_ALARM*/
    uint16_t GPO_DATA;                  /*GPIO output data GPIO9-15 is bit6*/

} GPIO_REG_t;

/**
 * @struct __SYS_REG_t
 * <SYS Control/Status Registers>
 */
typedef struct __SYS_REG_t
{
    uint16_t SYS_CONFIG0;               /*System configuration Register*/
    uint16_t SYS_STATUS0;               /*System status Register*/

} SYS_REG_t;

/**
 * @struct __MONITORING_REG
 * <TODO short explanation of the purpose of this struct>
 */
typedef struct __MONITORING_REG_tag
{
    uint16_t GLOBAL_ALARM_ENABLE;       /*ALARM Enable Register*/
    uint16_t GLOBAL_ALARM_INTERRUPT;    /*ALARM INT Register*/
    uint16_t DIE_TEMP;                  /*16bit Temp read only 2's complement*/
    uint16_t THRS_TEMP;                 /*Threshold Temperature Register*/

} MONITORING_REG_t;


/**
 * @struct __CHANNEL_TypDef_t
 * <Channel Registers 0...15>
 */
typedef struct __CHANNEL_TypDef_t
{
    uint16_t DATA[16];                  /*CH output Data 0..15 [23:0] Register*/
    uint16_t OVR_THRS[16];              /*CH over-range threshold 0..15 [23:0] Register*/
    uint16_t UDR_THRS[16];              /*CH under-range threshold 0..15 [23:0] Register*/
    uint16_t GAIN_COEF[16];             /*Calibrated gain coeffient 0..15 [23:0] Register*/
    uint16_t OFFSET_COEF[16];           /*Calibrated offset coeffient 0..15 [23:0] Register*/
    uint16_t OPT_COEF[16];              /*Extra Register for Temp storing coefficients*/

} CHANNEL_TypDef_t;

/**
 * @struct afe_ch_config
 * <TODO short explanation of the purpose of this struct>
 */
typedef struct afe_ch_config_tag
{
    uint8_t hv_aip;
    uint8_t test2;


} afe_ch_config_t;

/**
 * @struct afe_sys_config
 * <TODO short explanation of the purpose of this struct>
 */
typedef struct afe_sys_config_tag
{
    uint8_t value1;
    uint8_t value2;
    uint8_t value3;
    uint8_t value4;

} afe_sys_config_t;

/*******************************************************************************
 * prototypes
 ******************************************************************************/
/**
 * @brief Init_CH_RegisterAdr
 *
 * <Initalizes Channel Register Struct with Register-Addresses>
 *
 */
void Init_CH_RegisterAdr(void);

/*******************************************************************************
 * global extern data
 ******************************************************************************/
extern const CH_CONFIG_t LOGIC_CH_CONTROL;
extern const GPIO_REG_t GPIO_CONTROL;
extern const SYS_REG_t SYS_CONTROL;
extern const MONITORING_REG_t ALARM_CONTROL;
extern CHANNEL_TypDef_t CHANNEL_REG;


#endif /* AFE_DRIVER_INC_FSL_AFE_REGISTERS_H_ */

/*[EOF]************************************************************************/
