/** @file
Header file for CpuPcieInitCommonLib.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _CPU_PCIE_INIT_COMMON_H_
#define _CPU_PCIE_INIT_COMMON_H_

#include <Library/CpuRegbarAccessLib.h>

/**
  Print registers value

  @param[in] PrintMmioBase       Mmio base address
  @param[in] PrintSize           Number of registers
  @param[in] OffsetFromBase      Offset from mmio base address

  @retval None
**/
VOID
SaPrintRegisters (
  IN  UINTN        PrintMmioBase,
  IN  UINT32       PrintSize,
  IN  UINT32       OffsetFromBase
  );

/**
  Print registers value

  @param[in] PrintPciSegmentBase Pci segment base address
  @param[in] PrintSize           Number of registers
  @param[in] OffsetFromBase      Offset from mmio base address

  @retval None
**/
VOID
SaPrintPciRegisters (
  IN  UINT64       PrintPciSegmentBase,
  IN  UINT32       PrintSize,
  IN  UINT32       OffsetFromBase
  );

//
// 2LM: PegPcie APIs for Sideband Access Mechanism in 2LM mode
//
/**
Reads an 8-bit PCI configuration register.

Its a wrapper library function. This function uses side band access for PEG60 when 2LM mode is enabled.
Other calls to this function will be routed to core PciSegmentRead8 function.

@param  Address   Address that encodes the PCI Segment, Bus, Device, Function, and Register.

@return The 8-bit PCI configuration register specified by Address.

**/
UINT8
EFIAPI
PegPciSegmentRead8 (
  IN UINT64                    Address
  );

/**
Writes an 8-bit PCI configuration register.

Its a wrapper library function. This function uses side band access for PEG60 when 2LM mode is enabled.
Other calls to this function will be routed to core PciSegmentWrite8 function.

@param  Address     Address that encodes the PCI Segment, Bus, Device, Function, and Register.
@param  Value       The value to write.

@return The value written to the PCI configuration register.

**/
UINT8
EFIAPI
PegPciSegmentWrite8 (
  IN UINT64                    Address,
  IN UINT8                     Value
  );

/**
Reads a 16-bit PCI configuration register.

Its a wrapper library function. This function uses side band access for PEG60 when 2LM mode is enabled.
Other calls to this function will be routed to core PciSegmentRead16 function.

@param  Address   Address that encodes the PCI Segment, Bus, Device, Function, and Register.

@return The 16-bit PCI configuration register specified by Address.

**/
UINT16
EFIAPI
PegPciSegmentRead16 (
  IN UINT64                    Address
  );

/**
Writes a 16-bit PCI configuration register.

Its a wrapper library function. This function uses side band access for PEG60 when 2LM mode is enabled.
Other calls to this function will be routed to core PciSegmentWrite16 function.

@param  Address     Address that encodes the PCI Segment, Bus, Device, Function, and Register.
@param  Value       The value to write.

@return The parameter of Value.

**/
UINT16
EFIAPI
PegPciSegmentWrite16  (
  IN UINT64                    Address,
  IN UINT16                    Value
  );

/**
Reads a 32-bit PCI configuration register.

Its a wrapper library function. This function uses side band access for PEG60 when 2LM mode is enabled.
Other calls to this function will be routed to core PciSegmentRead32 function.

@param  Address   Address that encodes the PCI Segment, Bus, Device, Function, and Register.

@return The 32-bit PCI configuration register specified by Address.

**/
UINT32
EFIAPI
PegPciSegmentRead32 (
  IN UINT64                    Address
  );

/**
Writes a 32-bit PCI configuration register.

Its a wrapper library function. This function uses side band access for PEG60 when 2LM mode is enabled.
Other calls to this function will be routed to core PciSegmentWrite32 function.

@param  Address     Address that encodes the PCI Segment, Bus, Device, Function, and Register.
@param  Value       The value to write.

@return The parameter of Value.

**/
UINT32
EFIAPI
PegPciSegmentWrite32 (
  IN UINT64                    Address,
  IN UINT32                    Value
  );

/**
Performs a bitwise OR of a 16-bit PCI configuration register with a 16-bit value.

Its a wrapper library function. This function uses side band access for PEG60 when 2LM mode is enabled.
Other calls to this function will be routed to core PciSegmentOr16 function.

@param  Address   Address that encodes the PCI Segment, Bus, Device, Function, and Register.
@param  OrData    The value to OR with the PCI configuration register.

@return The value written to the PCI configuration register.
**/
UINT16
EFIAPI
PegPciSegmentOr16 (
  IN UINT64                    Address,
  IN UINT16                    OrData
  );

/**
Performs a bitwise AND of a 16-bit PCI configuration register with a 16-bit value.

Its a wrapper library function. This function uses side band access for PEG60 when 2LM mode is enabled.
Other calls to this function will be routed to core PciSegmentAnd32 function.

@param  Address   Address that encodes the PCI Segment, Bus, Device, Function, and Register.
@param  AndData   The value to AND with the PCI configuration register.

@return The value written to the PCI configuration register.

**/
UINT16
EFIAPI
PegPciSegmentAnd16 (
  IN UINT64                    Address,
  IN UINT16                    AndData
  );

/**
Performs a bitwise AND of a 8-bit PCI configuration register with a 8-bit value.

Its a wrapper library function. This function uses side band access for PEG60 when 2LM mode is enabled.
Other calls to this function will be routed to core PciSegmentAnd8 function.

@param  Address   Address that encodes the PCI Segment, Bus, Device, Function, and Register.
@param  AndData   The value to AND with the PCI configuration register.

@return The value written to the PCI configuration register.

**/
UINT8
EFIAPI
PegPciSegmentAnd8 (
  IN UINT64                    Address,
  IN UINT8                     AndData
  );

/**
Performs a bitwise OR of a 32-bit PCI configuration register with a 32-bit value.

Its a wrapper library function. This function uses side band access for PEG60 when 2LM mode is enabled.
Other calls to this function will be routed to core PciSegmentOr32 function.

@param  Address   Address that encodes the PCI Segment, Bus, Device, Function, and Register.
@param  OrData    The value to OR with the PCI configuration register.

@return The value written to the PCI configuration register.

**/
UINT32
EFIAPI
PegPciSegmentOr32 (
  IN UINT64                    Address,
  IN UINT32                    OrData
  );

/**
Performs a bitwise OR of a 8-bit PCI configuration register with a 8-bit value.

Its a wrapper library function. This function uses side band access for PEG60 when 2LM mode is enabled.
Other calls to this function will be routed to core PciSegmentOr8 function.

@param  Address   Address that encodes the PCI Segment, Bus, Device, Function, and Register.
@param  OrData    The value to OR with the PCI configuration register.

@return The value written to the PCI configuration register.

**/
UINT8
EFIAPI
PegPciSegmentOr8 (
  IN UINT64                    Address,
  IN UINT8                     OrData
  );

/**
Performs a bitwise AND of a 32-bit PCI configuration register with a 32-bit value.

Its a wrapper library function. This function uses side band access for PEG60 when 2LM mode is enabled.
Other calls to this function will be routed to core PciSegmentAnd32 function.

@param  Address   Address that encodes the PCI Segment, Bus, Device, Function, and Register.
@param  AndData   The value to AND with the PCI configuration register.

@return The value written to the PCI configuration register.

**/
UINT32
EFIAPI
PegPciSegmentAnd32 (
  IN UINT64                    Address,
  IN UINT32                    AndData
  );

/**
Performs a bitwise AND of a 32-bit PCI configuration register with a 32-bit value,
followed a  bitwise OR with another 32-bit value.

Its a wrapper library function. This function uses side band access for PEG60 when 2LM mode is enabled.
Other calls to this function will be routed to core PciSegmentAndThenOr32 function.

@param  Address   Address that encodes the PCI Segment, Bus, Device, Function, and Register.
@param  AndData   The value to AND with the PCI configuration register.
@param  OrData    The value to OR with the PCI configuration register.

@return The value written to the PCI configuration register.

**/
UINT32
EFIAPI
PegPciSegmentAndThenOr32 (
  IN UINT64                    Address,
  IN UINT32                    AndData,
  IN UINT32                    OrData
  );

/**
Performs a bitwise AND of a 16-bit PCI configuration register with a 16-bit value,
followed a  bitwise OR with another 16-bit value.

Its a wrapper library function. This function uses side band access for PEG60 when 2LM mode is enabled.
Other calls to this function will be routed to core PciSegmentAndThenOr16 function.

@param  Address   Address that encodes the PCI Segment, Bus, Device, Function, and Register.
@param  AndData   The value to AND with the PCI configuration register.
@param  OrData    The value to OR with the PCI configuration register.

@return The value written to the PCI configuration register.

**/
UINT16
EFIAPI
PegPciSegmentAndThenOr16 (
  IN UINT64                    Address,
  IN UINT16                    AndData,
  IN UINT16                    OrData
  );

/**
Performs a bitwise AND of a 8-bit PCI configuration register with a 8-bit value,
followed a  bitwise OR with another 8-bit value.

Its a wrapper library function. This function uses side band access for PEG60 when 2LM mode is enabled.
Other calls to this function will be routed to core PciSegmentAndThenOr8 function.

@param  Address   Address that encodes the PCI Segment, Bus, Device, Function, and Register.
@param  AndData   The value to AND with the PCI configuration register.
@param  OrData    The value to OR with the PCI configuration register.

@return The value written to the PCI configuration register.

**/
UINT8
EFIAPI
PegPciSegmentAndThenOr8 (
  IN UINT64                    Address,
  IN UINT8                     AndData,
  IN UINT8                     OrData
  );

/**
Find the Offset to a Capabilities ID
@param[in] Segment              Pci Segment Number
@param[in] Bus                  Pci Bus Number
@param[in] Device               Pci Device Number
@param[in] Function             Pci Function Number
@param[in] CapId                CAPID to search for

@retval 0                       CAPID not found
@retval Other                   CAPID found, Offset of desired CAPID
**/
UINT8
PegPcieFindCapId (
  IN UINT8   Segment,
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function,
  IN UINT8   CapId
  );
#endif // _CPU_PCIE_INIT_COMMON_H_
