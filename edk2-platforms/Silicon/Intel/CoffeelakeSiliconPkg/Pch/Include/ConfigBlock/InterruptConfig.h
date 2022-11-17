/** @file
  Interrupt policy

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _INTERRUPT_CONFIG_H_
#define _INTERRUPT_CONFIG_H_

#define INTERRUPT_CONFIG_REVISION 1
extern EFI_GUID gInterruptConfigGuid;

#pragma pack (push,1)

//
// --------------------- Interrupts Config ------------------------------
//
typedef enum {
  PchNoInt,        ///< No Interrupt Pin
  PchIntA,
  PchIntB,
  PchIntC,
  PchIntD
} PCH_INT_PIN;

///
/// The PCH_DEVICE_INTERRUPT_CONFIG block describes interrupt pin, IRQ and interrupt mode for PCH device.
///
typedef struct {
  UINT8        Device;                  ///< Device number
  UINT8        Function;                ///< Device function
  UINT8        IntX;                    ///< Interrupt pin: INTA-INTD (see PCH_INT_PIN)
  UINT8        Irq;                     ///< IRQ to be set for device.
} PCH_DEVICE_INTERRUPT_CONFIG;

#define PCH_MAX_DEVICE_INTERRUPT_CONFIG  64       ///< Number of all PCH devices
#define PCH_MAX_PXRC_CONFIG              8        ///< Number of PXRC registers in ITSS

///
/// The PCH_INTERRUPT_CONFIG block describes interrupt settings for PCH.
///
typedef struct {
  CONFIG_BLOCK_HEADER          Header;                                          ///< Config Block Header
  UINT8                        NumOfDevIntConfig;                               ///< Number of entries in DevIntConfig table
  UINT8                        Rsvd0[3];                                        ///< Reserved bytes, align to multiple 4.
  PCH_DEVICE_INTERRUPT_CONFIG  DevIntConfig[PCH_MAX_DEVICE_INTERRUPT_CONFIG];   ///< Array which stores PCH devices interrupts settings
  UINT8                        PxRcConfig[PCH_MAX_PXRC_CONFIG];                 ///< Array which stores interrupt routing for 8259 controller
  UINT8                        GpioIrqRoute;                                    ///< Interrupt routing for GPIO. Default is <b>14</b>.
  UINT8                        SciIrqSelect;                                    ///< Interrupt select for SCI. Default is <b>9</b>.
  UINT8                        TcoIrqSelect;                                    ///< Interrupt select for TCO. Default is <b>9</b>.
  UINT8                        TcoIrqEnable;                                    ///< Enable IRQ generation for TCO. <b>0: Disable</b>; 1: Enable.
} PCH_INTERRUPT_CONFIG;

#pragma pack (pop)

#endif // _INTERRUPT_CONFIG_H_
