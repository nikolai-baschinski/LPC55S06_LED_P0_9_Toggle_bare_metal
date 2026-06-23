/*
** ###################################################################
**     Processors:          LPC55S06JBD64
**                          LPC55S06JHI48
**
**     Compilers:           GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**                          Keil ARM C/C++ Compiler
**                          MCUXpresso Compiler
**
**     Reference manual:    LPC55S0x/LPC550x User manual Rev.0.3  14 August 2020
**     Version:             rev. 2.0, 2024-10-29
**     Build:               b250520
**
**     Abstract:
**         CMSIS Peripheral Access Layer for LPC55S06
**
**     Copyright 1997-2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2025 NXP
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 1.0 (2020-04-09)
**         Initial version based on Niobe4mini
**     - rev. 2.0 (2024-10-29)
**         Change the device header file from single flat file to multiple files based on peripherals,
**         each peripheral with dedicated header file located in periphN folder.
**
** ###################################################################
*/

/*!
 * @file LPC55S06.h
 * @version 2.0
 * @date 2024-10-29
 * @brief CMSIS Peripheral Access Layer for LPC55S06
 *
 * CMSIS Peripheral Access Layer for LPC55S06
 */

#if !defined(LPC55S06_H_)  /* Check if memory map has not been already included */
#define LPC55S06_H_

/* IP Header Files List */
#include "PERI_ANACTRL.h"
#include "PERI_CTIMER.h"
#include "PERI_FLASH.h"
#include "PERI_GPIO.h"
#include "PERI_IOCON.h"
#include "PERI_PMC.h"
#include "PERI_PUF_SRAM_CTRL.h"
#include "PERI_SYSCON.h"

#endif  /* #if !defined(LPC55S06_H_) */
