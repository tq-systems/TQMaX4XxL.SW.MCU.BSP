/**
 * @file     crcDriver.h
 * copyright Copyright (c) 2023 TQ-Systems GmbH
 *           <license@tq-group.com>, D-82229 Seefeld, Germany.
 *           All rights reserved.
 * @author   <TODO author of the file>
 * @date     2023-02-17
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

#ifndef EXAMPLES_AFE_DRIVER_INC_CRCDRIVER_H_
#define EXAMPLES_AFE_DRIVER_INC_CRCDRIVER_H_

/*******************************************************************************
 * includes
 ******************************************************************************/

/* runtime */
#include <stdint.h>
/* project */

/*******************************************************************************
 * defines
 ******************************************************************************/



/*******************************************************************************
 * macros
 ******************************************************************************/



/*******************************************************************************
 * typedefs
 ******************************************************************************/



/*******************************************************************************
 * prototypes
 ******************************************************************************/

uint32_t crc_compute(uint8_t* p_buf, uint8_t size);

/*******************************************************************************
 * global extern data
 ******************************************************************************/



/******************************************************************************/

#endif /* EXAMPLES_AFE_DRIVER_INC_CRCDRIVER_H_ */

/*[EOF]************************************************************************/
