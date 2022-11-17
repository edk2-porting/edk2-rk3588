/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _EFI_SYSTEM_BOARD_PPI_H_
#define _EFI_SYSTEM_BOARD_PPI_H_

#include <Platform.h>
#include <Library/MmPciBaseLib.h>
#include <Pi/PiHob.h>


// GUID
#include <Guid/SetupVariable.h>
#include <Guid/SocketIioVariable.h>

// PPI
#include <Ppi/PchPolicy.h>
#include <Ppi/SystemBoard.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/Smbus2.h>


// Library
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseLib.h>
#include <Library/PciLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/PciExpressLib.h>

#include <Library/GpioLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/PchInfoLib.h>
#include <Platform.h>
#include <GpioPinsSklH.h>
#include <Library/GpioLib.h>

#include <IioBifurcationSlotTable.h>

// CMOS access Port address
#define LAST_CMOS_BYTE          0x7F
#define NMI_OFF                 0x80
#define B_PCH_RTC_REGB_SRBRST   0x02  // Value to be reset to during POST
#define R_PCH_RTC_REGD          0x0D  // CMOS Register D Status
#define R_PCH_RTC_REGE          0x0E  // CMOS Register E Status
#define B_PCH_RTC_REGE_INVTIM   0x04  // CMOS invalid time found

#define TIMER1_CONTROL_PORT 0x43
#define TIMER1_COUNT_PORT   0x41
#define LOAD_COUNTER1_LSB   0x54
#define COUNTER1_COUNT      0x12
//
// Reset Generator I/O Port
//
#define RESET_GENERATOR_PORT  0xCF9

//-----------------------------------------------------------------------;
// PCH: Chipset Configuration Register Equates
//-----------------------------------------------------------------------;
#define ICH_RCRB_IRQ0                       0
#define ICH_RCRB_IRQA                       1
#define ICH_RCRB_IRQB                       2
#define ICH_RCRB_IRQC                       3
#define ICH_RCRB_IRQD                       4
#define ICH_RCRB_PIRQA                      0
#define ICH_RCRB_PIRQB                      1
#define ICH_RCRB_PIRQC                      2
#define ICH_RCRB_PIRQD                      3
#define ICH_RCRB_PIRQE                      4
#define ICH_RCRB_PIRQF                      5
#define ICH_RCRB_PIRQG                      6
#define ICH_RCRB_PIRQH                      7

//
// From WBG Soft Straps WIP.xlsx
//
#define WBG_DOWNSKU_STRAP_DSKU      0x80046000
#define WBG_DOWNSKU_STRAP_BSKU      0x8004E003
#define WBG_DOWNSKU_STRAP_TSKU      0x00044000

#define PCHSTRAP_9   9
#define PCHSTRAP_10  10
#define PCHSTRAP_16  16
#define PCHSTRAP_17  17

#define RESET_PORT                0x0CF9
#define CLEAR_RESET_BITS          0x0F1
#define COLD_RESET                0x02  // Set bit 1 for cold reset
#define RST_CPU                   0x04  // Setting this bit triggers a reset of the CPU
#define FULL_RESET                0x08  // Set bit 4 with bit 1 for full reset

//
// PPI functions
//

VOID
SetBifurcations(
    IN OUT IIO_GLOBALS *IioGlobalData,
    IN     IIO_BIFURCATION_ENTRY *BifurcationTable,
    IN     UINT8                  BifurcationEntries
);

VOID
EnableHotPlug (
    IN OUT IIO_GLOBALS *IioGlobalData,
    IN UINT8 Port,
    IN UINT8 VppPort,
    IN UINT8 VppAddress,
    IN UINT8 PortOwnership
  );


VOID
ConfigSlots (
    IN OUT IIO_GLOBALS        *IioGlobalData,
    IN IIO_SLOT_CONFIG_ENTRY  *Slot,
    IN UINT8                  SlotEntries
  );

VOID
OverrideConfigSlots (
    IN OUT IIO_GLOBALS        *IioGlobalData,
    IN IIO_SLOT_CONFIG_ENTRY  *Slot,
    IN UINT8                  SlotEntries
  );
  
VOID
CalculatePEXPHideFromIouBif (
    IN UINT8 Iou,
    IN UINT8 IioIndex,
    IN OUT IIO_GLOBALS *IioGlobalData
);

VOID
DumpIioConfiguration(
    IN UINT8 iio,
    IN IIO_GLOBALS *IioGlobalData
);

VOID
OverrideDefaultBifSlots(
    IN IIO_GLOBALS *IioGlobalData
);

UINT8
GetUplinkPortInformationCommon (
    IN UINT8 IioIndex
);

VOID
SystemIioPortBifurcationInitCommon (
    IIO_GLOBALS *IioGlobalData,
    IIO_BIFURCATION_ENTRY         **BifurcationTable,
    UINT8                         *BifurcationEntries,
    IIO_SLOT_CONFIG_ENTRY         **SlotTable,
    UINT8                         *SlotEntries
);

VOID
SystemHideIioPortsCommon(
    IIO_GLOBALS *IioGlobalData,
    UINT8       IioIndex
);

UINT8
GetUplinkPortInformation (
    IN UINT8 IioIndex
);

VOID
SystemIioPortBifurcationInit (
  IN IIO_GLOBALS  *IioGlobalData
  );

#endif
