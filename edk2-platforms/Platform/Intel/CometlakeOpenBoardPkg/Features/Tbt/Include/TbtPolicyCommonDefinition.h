/** @file
TBT Policy Common definition.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _TBT_POLICY_COMMON_H_
#define _TBT_POLICY_COMMON_H_

#include <Library/GpioLib.h>
#include <IndustryStandard/Pci22.h>

#define MAX_DTBT_CONTROLLER_NUMBER 2

#define TYPE_PCIE           0x01
#define TYPE_PEG            0x02

#pragma pack(push, 1)

//
// dTBT Force Power GPIO Data Structure
//
typedef struct _DTBT_FORCE_POWER_GPIO_CONFIG {
  GPIO_PAD       GpioPad;                 ///< GPIO Pad Number
  BOOLEAN        GpioLevel;               ///< 0 = Active Low; 1 = Active High
  UINT8          Reserved[3];             ///< Reserved for DWORD alignment
} DTBT_FORCE_POWER_GPIO_CONFIG;

//
// dTBT CIO Plug Event GPIO Data Structure
//
typedef struct _DTBT_CIO_PLUG_EVENT_GPIO_CONFIG {
  GPIO_PAD       GpioPad;                 ///< GPIO Pad Number
  UINT32         AcpiGpeSignature;        ///< AcpiPlatform driver will change the XTBT method to the _Lxx or _Exx that we assign in this item.
  BOOLEAN        AcpiGpeSignaturePorting; ///< 0 = No porting required(for 2-tier GPI GPE event architecture), 1 = Porting required(for 1-tier GPI GPE event architecture)
  UINT8          Reserved[3];             ///< Reserved for DWORD alignment
} DTBT_CIO_PLUG_EVENT_GPIO_CONFIG;

//
// dTBT PCIE Reset GPIO Data Structure
//
typedef struct _DTBT_PCIE_RESET_GPIO_CONFIG {
  GPIO_PAD       GpioPad;                 ///< GPIO Pad Number
  BOOLEAN        GpioLevel;               ///< 0 = Active Low; 1 = Active High
  UINT8          Reserved[3];             ///< Reserved for DWORD alignment
} DTBT_PCIE_RESET_GPIO_CONFIG;

//
// dTBT Controller Data Structure
//
typedef struct _DTBT_CONTROLLER_CONFIG {
  UINT8                           DTbtControllerEn; ///< Enable/Disable DTbtController.
  UINT8                           Type;             ///< 01-Pcie RP, 02- PEG,Reserved. <Specific according to Board Design>
  UINT8                           PcieRpNumber;     ///< RP Number/ PEG Port (0,1,2) that connecet to dTBT controller. <Specific according to Board Design>
  DTBT_FORCE_POWER_GPIO_CONFIG    ForcePwrGpio;     ///< The GPIO pin that can force dTBT Power On. <Specific according to Board Design>
  DTBT_CIO_PLUG_EVENT_GPIO_CONFIG CioPlugEventGpio; ///< The GPIO pin that can generate Hot-Plug event. <Specific according to Board Design>
  DTBT_PCIE_RESET_GPIO_CONFIG     PcieRstGpio;      ///< The GPIO pin that is use to perform Reset when platform enters to Sx, it is required for platforms where PCI_RST pin connected to Tbt is controlled with GPIO <Specific according to Board Design>
  GPIO_PAD                        PdResetGpioPad;   ///< PD HRESET GPIO Pad Number
  GPIO_PAD                        PdSxEntryGpioPad; ///< PD SX Entry GPIO Pad Number
  GPIO_PAD                        PdSxAckGpioPad;   ///< PD SX Ack GPIO Pad Number
  UINT8                           Reserved[1];      ///< Reserved for DWORD alignment
} DTBT_CONTROLLER_CONFIG;

//
// dTBT Controller Data Structure
//
typedef struct _DTBT_COMMON_CONFIG {
  UINT8            TbtBootOn;                    ///< Send BootOn Mailbox command when TbtBootOn is enabled.
  UINT8            TbtUsbOn;                     ///< Send UsbOn Mailbox command when TbtBootOn is enabled.
  UINT8            Gpio3ForcePwr;                ///< Force GPIO to power on or not
  UINT16           Gpio3ForcePwrDly;             ///< The delay time after do ForcePwr
  BOOLEAN          DTbtSharedGpioConfiguration;  ///< Multiple DTBT controllers share the same GPIO pin <Specific according to Board Design>
  BOOLEAN          PcieRstSupport;               ///< 0 = Not Support, 1 = Supported. it is required for platforms where PCI_RST pin connected to Tbt is controlled with GPIO
  UINT8            SecurityMode;                 ///< 0: SL0 No Security, 1: SL1 User Authorization, 2: SL2 Secure Connect, 3: SL3 Display Port and USB
  UINT8            ControlIommu;                 ///< Control Iommu behavior in pre-boot, 0: Disabled Iommu, 1: Enable Iommu, Disable exception list, 2: Enable Iommu, Enable exception list
  UINT8            Reserved[3];                  ///< Reserved for DWORD alignment
} DTBT_COMMON_CONFIG;

#pragma pack(pop)

#endif

