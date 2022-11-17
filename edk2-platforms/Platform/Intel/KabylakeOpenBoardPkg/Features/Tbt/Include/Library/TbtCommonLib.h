/**@file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _TBT_COMMON_LIB_H_
#define _TBT_COMMON_LIB_H_

#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/GpioExpanderLib.h>

#define DTBT_CONTROLLER                   0x00
#define DTBT_TYPE_PCH                     0x01
#define DTBT_TYPE_PEG                     0x02
#define TBT2PCIE_DTBT_R                   0x548
#define PCIE2TBT_DTBT_R                   0x54C

//
//  Thunderbolt FW OS capability
//
#define NO_OS_NATIVE_SUPPORT    0
#define OS_NATIVE_SUPPORT_ONLY  1
#define OS_NATIVE_SUPPORT_RTD3  2

#define DTBT_SAVE_STATE_OFFSET  BIT0 // Bits 0-3 is for DTBT (only bit 0 is in use)
/**
Get Tbt2Pcie Register Offset

@retval     Register  Register Variable
**/

#define GET_TBT2PCIE_REGISTER_ADDRESS(Segment, Bus, Device, Function, RegisterAddress) \
RegisterAddress = PCI_SEGMENT_LIB_ADDRESS(Segment, Bus, Device, Function, TBT2PCIE_DTBT_R); \

/**
Get Pcie2Tbt Register Offset

@retval     Register  Register Variable
**/

#define GET_PCIE2TBT_REGISTER_ADDRESS(Segment, Bus, Device, Function, RegisterAddress) \
RegisterAddress = PCI_SEGMENT_LIB_ADDRESS(Segment, Bus, Device, Function, PCIE2TBT_DTBT_R); \

#define PCIE2TBT_VLD_B                    BIT0
#define TBT2PCIE_DON_R                    BIT0
#define TBT_MAIL_BOX_DELAY                (100*1000)
#define TBT_5S_TIMEOUT                    50
#define TBT_1S_TIMEOUT                    10
#define TBT_3S_TIMEOUT                    30

#define PCIE2TBT_GO2SX                    (0x02 << 1)
#define PCIE2TBT_GO2SX_NO_WAKE            (0x03 << 1)
#define PCIE2TBT_SX_EXIT_TBT_CONNECTED    (0x04 << 1)
#define PCIE2TBT_SX_EXIT_NO_TBT_CONNECTED (0x05 << 1)
#define PCIE2TBT_OS_UP                    (0x06 << 1)
#define PCIE2TBT_SET_SECURITY_LEVEL       (0x08 << 1)
#define PCIE2TBT_GET_SECURITY_LEVEL       (0x09 << 1)
#define PCIE2TBT_CM_AUTH_MODE_ENTER       (0x10 << 1)
#define PCIE2TBT_CM_AUTH_MODE_EXIT        (0x11 << 1)
#define PCIE2TBT_BOOT_ON                  (0x18 << 1)
#define PCIE2TBT_BOOT_OFF                 (0x19 << 1)
#define PCIE2TBT_USB_ON                   (0x19 << 1)
#define PCIE2TBT_GET_ENUMERATION_METHOD   (0x1A << 1)
#define PCIE2TBT_SET_ENUMERATION_METHOD   (0x1B << 1)
#define PCIE2TBT_POWER_CYCLE              (0x1C << 1)
#define PCIE2TBT_PREBOOTACL               (0x1E << 1)
#define CONNECT_TOPOLOGY_COMMAND          (0x1F << 1)

#define RESET_HR_BIT                      BIT0
#define ENUMERATE_HR_BIT                  BIT1
#define AUTO                              0x0

//
//Thunder Bolt Device IDs
//

//
// Alpine Ridge HR device IDs
//
#define AR_HR_2C  0x1576
#define AR_HR_4C  0x1578
#define AR_XHC    0x15B5
#define AR_XHC_4C 0x15B6
#define AR_HR_LP  0x15C0
//
// Alpine Ridge C0 HR device IDs
//
#define AR_HR_C0_2C  0x15DA
#define AR_HR_C0_4C  0x15D3
//
// Titan Ridge HR device IDs
//
#define TR_HR_2C  0x15E7
#define TR_HR_4C  0x15EA
//
//End of Thunderbolt(TM) Device IDs
//

typedef struct _DEV_ID {
  UINT8 Segment;
  UINT8 Bus;
  UINT8 Dev;
  UINT8 Fun;
} DEV_ID;

//@todo Seems to only be used by Platform/TBT/Smm/TbtSmm.inf
//@todo should refactor this to only be present in that driver
//@todo also definitions like this should never be in a .h file anyway
//@todo this is a quick hack to get things compiling for now
#ifdef __GNUC__
#pragma GCC diagnostic warning "-Wunused-variable"
#endif

/**
Based on the Security Mode Selection, BIOS drives FORCE_PWR.

@param[in]  GpioNumber
@param[in]  Value
**/
VOID
ForceDtbtPower(
  IN  UINT8          GpioAccessType,
  IN  UINT8          Expander,
  IN  UINT32         GpioNumber,
  IN  BOOLEAN        Value
);

/**
  Get Security Level.
  @param[in]  Bus       Bus number for Host Router (DTBT)
  @param[in]  Device    Device number for Host Router (DTBT)
  @param[in]  Function  Function number for Host Router (DTBT)
  @param[in]  Timeout   Time out with 100 ms garnularity
**/
UINT8
GetSecLevel (
  IN    UINT8                   Bus,
  IN    UINT8                   Device,
  IN    UINT8                   Function,
  IN    UINT8                   Command,
  IN    UINT32                  Timeout
  );

/**
  Set Security Level.
  @param[in]  Data      Security State
  @param[in]  Bus       Bus number for Host Router (DTBT)
  @param[in]  Device    Device number for Host Router (DTBT)
  @param[in]  Function  Function number for Host Router (DTBT)
  @param[in]  Timeout   Time out with 100 ms garnularity
**/
BOOLEAN
SetSecLevel (
  IN    UINT8                   Data,
  IN    UINT8                   Bus,
  IN    UINT8                   Device,
  IN    UINT8                   Function,
  IN    UINT8                   Command,
  IN    UINT32                  Timeout
  );

/**
Execute TBT Mail Box Command

@param[in]  Command   TBT Command
@param[in]  Bus       Bus number for Host Router (DTBT)
@param[in]  Device    Device number for Host Router (DTBT)
@param[in]  Function  Function number for Host Router (DTBT)
@param[in]  Timeout   Time out with 100 ms garnularity
@Retval     true      if command executes succesfully
**/
BOOLEAN
TbtSetPcie2TbtCommand(
  IN    UINT8                   Command,
  IN    UINT8                   Bus,
  IN    UINT8                   Device,
  IN    UINT8                   Function,
  IN    UINT32                  Timeout
);
/**
  Check connected TBT controller is supported or not by DeviceID

  @param[in]  DeviceID              DeviceID of of TBT controller


  @retval     TRUE                  Valid DeviceID
  @retval     FALSE                 Invalid DeviceID
**/

BOOLEAN
IsTbtHostRouter (
  IN    UINT16  DeviceID
  );

/**
  Get Pch/Peg Pcie Root Port Device and Function Number for TBT by Root Port physical Number

  @param[in]  RpNumber              Root port physical number. (0-based)
  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.

  @retval     EFI_SUCCESS           Root port device and function is retrieved
**/
EFI_STATUS
EFIAPI
GetDTbtRpDevFun(
  IN  BOOLEAN Type,
  IN  UINTN   RpNumber,
  OUT UINTN   *RpDev,
  OUT UINTN   *RpFunc
  );

/**
  Internal function to Wait for Tbt2PcieDone Bit.to Set or clear
  @param[in]  CommandOffsetAddress      Tbt2Pcie Register Address
  @param[in]  TimeOut                   Time out with 100 ms garnularity
  @param[in]  Tbt2PcieDone              Wait condition (wait for Bit to Clear/Set)
  @param[out] *Tbt2PcieValue Function   Register value
**/
BOOLEAN
InternalWaitforCommandCompletion (
  IN  UINT64   CommandOffsetAddress,
  IN  UINT32   TimeOut,
  IN  BOOLEAN  Tbt2PcieDone,
  OUT UINT32   *Tbt2PcieValue
  );

VOID
GetRootporttoSetResourcesforTbt (
  IN UINTN                              RpIndex,
  OUT UINT8                             *RsvdExtraBusNum,
  OUT UINT16                            *RsvdPcieMegaMem,
  OUT UINT8                             *PcieMemAddrRngMax,
  OUT UINT16                            *RsvdPciePMegaMem,
  OUT UINT8                             *PciePMemAddrRngMax,
  OUT BOOLEAN                           *SetResourceforTbt
  );

#endif