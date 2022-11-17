/** @file
  This file contains routines that support PCI Express initialization

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include "PciExpressHelpersLibrary.h"
#include <Library/BaseMemoryLib.h>
#include <Library/PcieHelperLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PcieRpLib.h>
#include <PchPcieRpInfo.h>
#include <CpuPcieInfo.h>
#include <CpuPcieHob.h>
#include <Library/HobLib.h>
#include <Register/PchPcieRpRegs.h>

#define ASPM_L1_NO_LIMIT 0xFF
#define ASPM_L0s_NO_LIMIT 0x7
#define LINK_RETRAIN_WAIT_TIME 1000 // microseconds

typedef union {
  struct {
    UINT32  RequesterCapable       : 1;
    UINT32  ResponderCapable       : 1;
    UINT32  RootCapable            : 1;
    UINT32  Reserved               : 5;
    UINT32  LocalClockGranularity  : 8;
    UINT32  Reserved2              : 16;
  } Bits;
  UINT32  Uint32;
} PTM_CAPS;

typedef union {
  struct {
    UINT32 Enable                 : 1;
    UINT32 RootSelect             : 1;
    UINT32 Reserved               : 6;
    UINT32 EffectiveGranularity   : 8;
    UINT32 Reserved2              : 16;
  } Bits;
  UINT32  Uint32;
} PTM_CTRL;

typedef struct {
  UINT32                          Size;
  const PCH_PCIE_DEVICE_OVERRIDE* Table;
} OVERRIDE_TABLE;

typedef enum {
  DevTypePci,
  DevTypePcieEndpoint,
  DevTypePcieUpstream,
  DevTypePcieDownstream,
  DevTypeMax
} PCI_DEV_TYPE;

//
// This structure keeps in one place all data relevant to enabling L0s and L1.
// L0s latencies are encoded in the same way as in hardware registers. The only operation
// that will be performed on them is comparison
// L1 latencies are decoded to microseconds, because they will be used in subtractions and additions
//
typedef struct {
  UINT32  L0sSupported          : 1;
  UINT32  L1Supported           : 1;
  UINT32  L0sAcceptableLatency  : 3; // encoded as in hardware register
  UINT32  L1AcceptableLatencyUs : 8; // decoded to microseconds
  UINT32  LinkL0sExitLatency    : 3; // encoded as in hardware register
  UINT32  LinkL1ExitLatencyUs   : 8; // decoded to microseconds
} ASPM_CAPS;

typedef struct {
  UINT32     AspmL11  : 1;
  UINT32     AspmL12  : 1;
  UINT32     PmL11    : 1;
  UINT32     PmL12    : 1;
  UINT32     Cmrt     : 8; // Common Mode Restore Time
  UINT32     TpoScale : 2; // T power_on scale
  UINT32     TpoValue : 6; // T power_on value
} L1SS_CAPS;

#define MAX_SBDF_TABLE_SIZE 50 //arbitrary table size; big enough to accomodate even full length TBT chain.

typedef struct {
  UINT32 Count;
  SBDF   Entry [MAX_SBDF_TABLE_SIZE];
} SBDF_TABLE;

/**
  Converts device's segment:bus:device:function coordinates to flat address

  @param[in] Sbdf   device's segment:bus:device:function coordinates

  @retval    address of device's PCI cfg space
**/
STATIC
UINT64
SbdfToBase (
  SBDF Sbdf
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (Sbdf.Seg, Sbdf.Bus, Sbdf.Dev, Sbdf.Func, 0);
}

/*
  Converts Tpower_on from value:scale notation to microseconds

  @param[in] TpoScale   T power on scale
  @param[in] TpoValue   T power on value

  @retval    number of microseconds
*/
UINT32
TpoToUs (
  UINT32 TpoScale,
  UINT32 TpoValue
  )
{
  static const UINT8 TpoScaleMultiplier[] = {2, 10, 100};

  ASSERT (TpoScale < TpoScaleMax);
  if (TpoScale >= TpoScaleMax) {
    return 0;
  }
  return (TpoScaleMultiplier[TpoScale] * TpoValue);
}

/**
  Get max payload size supported by device.

  @param[in] Sbdf   device's segment:bus:device:function coordinates

  @retval    Max payload size, encoded in the same way as in register (0=128b, 1=256b, etc)
**/
STATIC
UINT8
GetMps (
  SBDF Sbdf
  )
{
  return (PciSegmentRead16 (SbdfToBase (Sbdf) + Sbdf.PcieCap + R_PCIE_DCAP_OFFSET) & B_PCIE_DCAP_MPS);
}

/**
  Sets Maximum Payload Size to be used by device

  @param[in] Sbdf   device's segment:bus:device:function coordinates
  @param[in] Mps    Max payload size, encoded in the same way as in register (0=128b, 1=256b, etc)
**/
STATIC
VOID
SetMps (
  SBDF  Sbdf,
  UINT8  Mps
  )
{
  PciSegmentAndThenOr16 (SbdfToBase (Sbdf) + Sbdf.PcieCap + R_PCIE_DCTL_OFFSET, (UINT16) ~B_PCIE_DCTL_MPS, Mps << N_PCIE_DCTL_MPS);
}

/**
  Enables LTR feature in given device

  @param[in] Sbdf            device's segment:bus:device:function coordinates
**/
STATIC
VOID
EnableLtr (
  SBDF Sbdf
  )
{
  if (Sbdf.PcieCap == 0) {
    return;
  }
  PciSegmentOr32 (SbdfToBase (Sbdf) + Sbdf.PcieCap + R_PCIE_DCTL2_OFFSET, B_PCIE_DCTL2_LTREN);
}

/**
  Returns information about type of device.

  @param[out] Sbdf            device's segment:bus:device:function coordinates

  @retval     one of: not a PCIe device (legacy PCI), PCIe endpoint, PCIe upstream port or PCIe downstream port (including rootport)
**/
STATIC
PCI_DEV_TYPE
GetDeviceType (
  SBDF Sbdf
  )
{
  UINT8 DeviceType;

  if (Sbdf.PcieCap == 0) {
    return DevTypePci;
  }
  DeviceType = (UINT8) ((PciSegmentRead16 (SbdfToBase (Sbdf) + Sbdf.PcieCap + R_PCIE_XCAP_OFFSET) & B_PCIE_XCAP_DT) >> N_PCIE_XCAP_DT);
  if (DeviceType == PCIE_DEVICE_PORT_TYPE_UPSTREAM_PORT) {
    return DevTypePcieUpstream;
  } else if (DeviceType == PCIE_DEVICE_PORT_TYPE_DOWNSTREAM_PORT || DeviceType == PCIE_DEVICE_PORT_TYPE_ROOT_PORT) {
    return DevTypePcieDownstream;
  } else {
    return DevTypePcieEndpoint;
  }
}

/**
  Initializes Dev:Func numbers for use in FindNextPcieChild or FindNextLegalSbdf functions.

  @param[out] Sbdf            device's segment:bus:device:function coordinates
**/
STATIC
VOID
InitChildFinder (
  OUT SBDF *Sbdf
  )
{
  //
  // Initialize Dev/Func to maximum values, so that when FindNextLegalSbdf ()
  // is called on those input parameters, it will return 1st legal address (Dev 0 Func 0).
  //
  Sbdf->Dev = PCI_MAX_DEVICE;
  Sbdf->Func = PCI_MAX_FUNC;
}

/**
  Checks the device is a bridge and has non-zero secondary bus number assigned.
  If so, it returns TRUE and initializes ChildSbdf with such values that
  allow searching for devices on the secondary bus.
  ChildSbdf will be mangled even if this function returns FALSE.

  Legal bus assignment is assumed. This function doesn't check subordinate bus numbers of
  the the device it was called on or any bridges between it and root complex

  @param[in]  Sbdf       device's segment:bus:device:function coordinates
  @param[out] ChildSbdf  SBDF initialized in such way that calling FindNextPcieChild( ) on it will find all children devices

  @retval TRUE if device is a bridge and has a bus behind it; FALSE otherwise
**/
STATIC
BOOLEAN
HasChildBus (
  SBDF   Sbdf,
  SBDF   *ChildSbdf
  )
{
  UINT32 Data32;
  UINT64 Base;
  UINT8  SecondaryBus;

  ChildSbdf->Seg = Sbdf.Seg;
  InitChildFinder (ChildSbdf);

  Base = SbdfToBase (Sbdf);

  if (PciSegmentRead8 (Base + R_PCI_BCC_OFFSET) != PCI_CLASS_BRIDGE) {
    DEBUG ((DEBUG_INFO, "HasChildBus%02:%02:%02: no\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));
    return FALSE;
  }
  Data32 = PciSegmentRead32 (Base + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET);
  SecondaryBus = (UINT8)((Data32 & B_PCI_BRIDGE_BNUM_SCBN) >> 8);
  ChildSbdf->Bus = SecondaryBus;
  if (SecondaryBus == 0) {
    DEBUG ((DEBUG_INFO, "HasChildBus%02x:%02x:%02x: no\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));
    return FALSE;
  } else {
    DEBUG ((DEBUG_INFO, "HasChildBus%02x:%02x:%02x: yes, %x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func, SecondaryBus));
    return TRUE;
  }
}

/**
  Sets LTR limit in a device.

  @param[in] Base            device's base address
  @param[in] Ltr             LTR limit
**/
STATIC
VOID
SetLtrLimit (
  UINT64    Base,
  LTR_LIMIT Ltr
  )
{
  UINT16 LtrCapOffset;
  UINT16 Data16;

  LtrCapOffset = PcieBaseFindExtendedCapId (Base, R_PCIE_LTRECH_CID);
  if (LtrCapOffset == 0) {
    return;
  }
  Data16 = (UINT16)((Ltr.MaxSnoopLatencyValue << N_PCIE_LTRECH_MSLR_VALUE) | (Ltr.MaxSnoopLatencyScale << N_PCIE_LTRECH_MSLR_SCALE));
  PciSegmentWrite16(Base + LtrCapOffset + R_PCIE_LTRECH_MSLR_OFFSET, Data16);

  Data16 = (UINT16)((Ltr.MaxNoSnoopLatencyValue << N_PCIE_LTRECH_MNSLR_VALUE) | (Ltr.MaxNoSnoopLatencyScale << N_PCIE_LTRECH_MNSLR_SCALE));
  PciSegmentWrite16(Base + LtrCapOffset + R_PCIE_LTRECH_MNSLR_OFFSET, Data16);
}

/**
  Checks if device at given address exists and is a PCI Express device.
  PCI express devices are distinguished from PCI by having Capability ID 0x10
  If the device is PCI express then its SDBF structure gets updated with pointer to
  the PCIe Capability. This is an optimization feature. It greatly decreases the number
  of bus accesses, since most features configured by this library depend on registers
  whose location is relative to PCIe capability.

  @param[in,out] Sbdf   on entry, segment:bus:device:function coordinates
                        on exit, PcieCap offset is updated

  @retval               TRUE when PCIe device exists; FALSE if it's not PCIe or there's no device at all
**/
STATIC
BOOLEAN
IsPcieDevice (
  SBDF *Sbdf
  )
{
  UINT8 PcieCapOffset;
  UINT64         Base;

  Base = SbdfToBase(*Sbdf);

  if (PciSegmentRead16 (Base) == 0xFFFF) {
    return FALSE;
  }


  PcieCapOffset = PcieBaseFindCapId (Base, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (PcieCapOffset == 0) {
    DEBUG ((DEBUG_INFO, "IsPcieDevice %02x:%02x:%02x - legacy\n", Sbdf->Bus, Sbdf->Dev, Sbdf->Func));
    return FALSE;
  } else {
    Sbdf->PcieCap = PcieCapOffset;
    DEBUG ((DEBUG_INFO, "IsPcieDevice %02x:%02x:%02x - yes\n", Sbdf->Bus, Sbdf->Dev, Sbdf->Func));
    return TRUE;
  }
}

/**
  Returns TRUE and Dev:Func numbers where a PCIe device could legally be located, or FALSE if there
  no such coordinates left.

  Segment and Bus fields of SBDF structure are input only and determine which bus will be scanned.
  This function should be called in a while() loop. It replaces the less efficient method of
  using nested FOR loops that iterate over all device and function numbers. It is optimized for
  the amount of bus access. If function0 doesn't exist or doesn't have Multifunction bit set,
  then higher function numbers are skipped. If parent of this bus is a downstream port, then
  Device numbers 1-31 get skipped too (there can be only Dev0 behind downstream ports)
  If device/function number == 0x1F/0x7, this function returns first possible address, that is 0:0
  Any other device number means Dev:Func contain address of last found child device
  and this function should search for next one

  @param[in]     ParentDevType  type of bridge who's partent of this bus
  @param[in,out] Sbdf           On entry: location returned previously from this function
                                          Dev:Func value of 1F:07 means search should start from the beginning
                                On exit:  if legal Dev:Func combination was found, that Dev:Func is returned
                                          otherwise, Dev:Func are initialized to 1F:07 for convenience

  @retval TRUE when next legal Dev:Func address was found; FALSE otherwise
**/
STATIC
BOOLEAN
FindNextLegalSbdf (
  IN     PCI_DEV_TYPE ParentDevType,
  IN OUT SBDF         *Sbdf
  )
{
  UINT8  MaxDev;
  UINT64 Func0Base;

  if (ParentDevType == DevTypePcieEndpoint) {
    return FALSE;
  }
  if (ParentDevType == DevTypePcieUpstream) {
    MaxDev = PCI_MAX_DEVICE;
  } else {
    MaxDev = 0;
  }
  Func0Base = PCI_SEGMENT_LIB_ADDRESS (Sbdf->Seg, Sbdf->Bus, Sbdf->Dev, 0, 0);
  if ((Sbdf->Dev == PCI_MAX_DEVICE) && Sbdf->Func == PCI_MAX_FUNC) {
    Sbdf->Dev = 0;
    Sbdf->Func = 0;
    return TRUE;
  } else if ((Sbdf->Func == PCI_MAX_FUNC) || (Sbdf->Func == 0 && !IsMultifunctionDevice (Func0Base))) {
  //
  // if it's the last function of a device, then return Func0 of new device or FALSE in case there are no more devices
  //
    if (Sbdf->Dev == MaxDev) {
      InitChildFinder (Sbdf);
      return FALSE;
    }
    (Sbdf->Dev)++;
    Sbdf->Func = 0;
    return TRUE;
  } else {
    (Sbdf->Func)++;
    return TRUE;
  }
}

/**
  Finds next PCIe (not legacy PCI) device behind given device
  If device/function number == 0x1F/0x7, this function searches for children from scratch
  Any other device number means Dev:Func contain address of last found child device
  and this function should search for next one

  @param[in]     ParentDevType  type of bridge who's partent of this bus
  @param[in,out] Sbdf           On entry: location returned previously from this function
                                          Dev:Func value of 0x1F:0x07 means search should start from the beginning
                                On exit:  if PCIe device was found, its SBDF coordinates are returned
                                          otherwise, Dev:Func are initialized to 0x1F:0x07 for convenience
  @retval TRUE when next PCIe device was found; FALSE otherwise
**/
BOOLEAN
FindNextPcieChild (
  IN     PCI_DEV_TYPE ParentDevType,
  IN OUT SBDF   *Sbdf
  )
{
  while ( FindNextLegalSbdf (ParentDevType, Sbdf)) {
    if (IsPcieDevice (Sbdf)) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Checks if given device supports Clock Power Management

  @param[in] Sbdf     segment:bus:device:function coordinates of a device

  @retval TRUE when device supports it, FALSE otherwise
**/
STATIC
BOOLEAN
IsCpmSupported (
  SBDF Sbdf
  )
{
  return !!(PciSegmentRead32 (SbdfToBase (Sbdf) + Sbdf.PcieCap + R_PCIE_LCAP_OFFSET) & B_PCIE_LCAP_CPM);
}

/**
  Sets Enable Clock Power Management bit for given device

  @param[in] Sbdf            segment:bus:device:function coordinates of a device
**/
STATIC
VOID
EnableCpm (
  SBDF Sbdf
  )
{
  PciSegmentOr16 (SbdfToBase (Sbdf) + Sbdf.PcieCap + R_PCIE_LCTL_OFFSET, B_PCIE_LCTL_ECPM);
}

/**
  Checks if given device is an IoAPIC

  @param[in] Base            device's base address

  @retval TRUE if it's an IoAPIC
**/
BOOLEAN
IsIoApicDevice (
  UINT64 Base
  )
{
  UINT8 BaseClassCode;
  UINT8 SubClassCode;
  UINT8 ProgInterface;

  BaseClassCode = PciSegmentRead8 (Base + PCI_CLASSCODE_OFFSET + 2);
  SubClassCode  = PciSegmentRead8 (Base + PCI_CLASSCODE_OFFSET + 1);
  ProgInterface = PciSegmentRead8 (Base + PCI_CLASSCODE_OFFSET);
  if ((BaseClassCode == PCI_CLASS_SYSTEM_PERIPHERAL) &&
      (SubClassCode == PCI_SUBCLASS_PIC) &&
      ((ProgInterface == PCI_IF_APIC_CONTROLLER) ||
       (ProgInterface == PCI_IF_APIC_CONTROLLER2))) {
    return TRUE;
  }
  return FALSE;
}

/**
  There are some devices which support L1 substates, but due to silicon bugs the corresponding register
  cannot be found by scanning PCIe capabilities. This function checks list of such devices and if one
  is found, returns its L1ss capability register offset

  @param[in] Base       base address of device
  @param[in] Override   table of devices that need override

  @retval               offset to L1ss capability register
**/
UINT16
GetOverrideL1ssCapsOffset (
  UINT64         Base,
  OVERRIDE_TABLE *Override
  )
{
  UINT16 DeviceId;
  UINT16 VendorId;
  UINT8  Revision;
  UINT32 Index;

  VendorId = PciSegmentRead16 (Base + PCI_VENDOR_ID_OFFSET);
  DeviceId = PciSegmentRead16 (Base + PCI_DEVICE_ID_OFFSET);
  Revision = PciSegmentRead8  (Base + PCI_REVISION_ID_OFFSET);

  for (Index = 0; Index < Override->Size; Index++) {
    if (((Override->Table[Index].OverrideConfig & PchPcieL1SubstatesOverride) == PchPcieL1SubstatesOverride) &&
        (Override->Table[Index].VendorId == VendorId) &&
        (Override->Table[Index].DeviceId == DeviceId) &&
        (Override->Table[Index].RevId == Revision || Override->Table[Index].RevId == 0xFFFF)) {
      return Override->Table[Index].L1SubstatesCapOffset;
    }
  }
  return 0;
}

/**
  There are some devices whose implementation of L1 substates is partially broken. This function checks
  list of such devices and if one is found, overrides their L1ss-related capabilities

  @param[in]     Base       base address of device
  @param[in]     Override   table of devices that need override
  @param[in,out] L1ss       on entry, capabilities read from register; on exit, capabilities modified according ot override table
**/
STATIC
VOID
OverrideL1ssCaps (
  UINT64         Base,
  OVERRIDE_TABLE *Override,
  L1SS_CAPS      *L1ss
  )
{
  UINT16 DeviceId;
  UINT16 VendorId;
  UINT8  Revision;
  UINT32 Index;

  VendorId = PciSegmentRead16 (Base + PCI_VENDOR_ID_OFFSET);
  DeviceId = PciSegmentRead16 (Base + PCI_DEVICE_ID_OFFSET);
  Revision = PciSegmentRead8  (Base + PCI_REVISION_ID_OFFSET);

  for (Index = 0; Index < Override->Size; Index++) {
    if (((Override->Table[Index].OverrideConfig & PchPcieL1SubstatesOverride) == PchPcieL1SubstatesOverride) &&
        (Override->Table[Index].VendorId == VendorId) &&
        (Override->Table[Index].DeviceId == DeviceId) &&
        (Override->Table[Index].RevId == Revision || Override->Table[Index].RevId == 0xFF)) {

      L1ss->PmL12   &= !!(Override->Table[Index].L1SubstatesCapMask & B_PCIE_EX_L1SCAP_PPL12S);
      L1ss->PmL11   &= !!(Override->Table[Index].L1SubstatesCapMask & B_PCIE_EX_L1SCAP_PPL11S);
      L1ss->AspmL12 &= !!(Override->Table[Index].L1SubstatesCapMask & B_PCIE_EX_L1SCAP_AL12S);
      L1ss->AspmL11 &= !!(Override->Table[Index].L1SubstatesCapMask & B_PCIE_EX_L1SCAP_AL1SS);
      if (Override->Table[Index].L1sTpowerOnValue != 0) {
        L1ss->Cmrt = Override->Table[Index].L1sCommonModeRestoreTime;
        L1ss->TpoScale = Override->Table[Index].L1sTpowerOnScale;
        L1ss->TpoValue = Override->Table[Index].L1sTpowerOnValue;
      }
      return;
    }
  }
}

/**
  Returns L1 sub states capabilities of a device

  @param[in] Base   base address of a device

  @retval L1SS_CAPS structure filled with device's capabilities
**/
STATIC
L1SS_CAPS
GetL1ssCaps (
  UINT64         Base,
  OVERRIDE_TABLE *Override
  )
{
  L1SS_CAPS Capabilities = {0};
  UINT16    PcieCapOffset;
  UINT32    CapsRegister;

  PcieCapOffset = GetOverrideL1ssCapsOffset (Base, Override);
  if (PcieCapOffset == 0) {
    PcieCapOffset = PcieBaseFindExtendedCapId (Base, V_PCIE_EX_L1S_CID);
  }
  if (PcieCapOffset == 0) {
    return Capabilities;
  }
  CapsRegister = PciSegmentRead32 (Base + PcieCapOffset + R_PCIE_EX_L1SCAP_OFFSET);
  if (CapsRegister & B_PCIE_EX_L1SCAP_L1PSS) {
    //
    // Skip L1.1 checking since some device only indecate L1.2 support.
    // [1604452805]
    //
    Capabilities.PmL11 = !!(CapsRegister & B_PCIE_EX_L1SCAP_PPL11S);
    Capabilities.PmL12 = !!(CapsRegister & B_PCIE_EX_L1SCAP_PPL12S);
    Capabilities.AspmL12 = !!(CapsRegister & B_PCIE_EX_L1SCAP_AL12S);
    Capabilities.AspmL11 = !!(CapsRegister & B_PCIE_EX_L1SCAP_AL1SS);
    Capabilities.Cmrt = (CapsRegister & B_PCIE_EX_L1SCAP_CMRT) >> N_PCIE_EX_L1SCAP_CMRT;
    Capabilities.TpoValue = (CapsRegister & B_PCIE_EX_L1SCAP_PTV) >> N_PCIE_EX_L1SCAP_PTV;
    Capabilities.TpoScale = (CapsRegister & B_PCIE_EX_L1SCAP_PTPOS) >> N_PCIE_EX_L1SCAP_PTPOS;
  }
  OverrideL1ssCaps (Base, Override, &Capabilities);
  return Capabilities;
}

/**
  Returns combination of two sets of L1 substate capabilities
  Given feature is supported by the link only if both sides support it
  Time parameters for link (Cmrt and Tpo) depend on the bigger value between two sides

  @param[in] L1ssA      L1 substate capabilities of first device
  @param[in] L1ssB      L1 substate capabilities of second device

  @retval Link's L1 substate capabilities
**/
STATIC
L1SS_CAPS
CombineL1ss (
  L1SS_CAPS L1ssA,
  L1SS_CAPS L1ssB
  )
{
  L1SS_CAPS Combined;

  Combined.PmL12 = L1ssA.PmL12 && L1ssB.PmL12;
  Combined.PmL11 = L1ssA.PmL11 && L1ssB.PmL11;
  Combined.AspmL12 = L1ssA.AspmL12 && L1ssB.AspmL12;
  Combined.AspmL11 = L1ssA.AspmL11 && L1ssB.AspmL11;
  Combined.Cmrt = MAX (L1ssA.Cmrt, L1ssB.Cmrt);
  if (TpoToUs (L1ssA.TpoScale, L1ssA.TpoValue) > TpoToUs (L1ssB.TpoScale, L1ssB.TpoValue)) {
    Combined.TpoScale = L1ssA.TpoScale;
    Combined.TpoValue = L1ssA.TpoValue;
  } else {
    Combined.TpoScale = L1ssB.TpoScale;
    Combined.TpoValue = L1ssB.TpoValue;
  }
  return Combined;
}

/**
  Configures L1 substate feature in a device

  @param[in] Sbdf     segment:bus:device:function coordinates of a device
  @param[in] L1ss     configuration to be programmed
  @param[in] Override table of devices that require special handling
**/
STATIC
VOID
SetL1ss (
  SBDF           Sbdf,
  L1SS_CAPS      L1ss,
  OVERRIDE_TABLE *Override,
  BOOLEAN   IsCpuPcie

  )
{
  UINT16    PcieCapOffset;
  UINT32    Ctrl1Register;
  UINT32    Ctrl2Register;
  UINT64    Base;

  Base = SbdfToBase(Sbdf);
  Ctrl1Register = 0;
  Ctrl2Register = 0;

  PcieCapOffset = GetOverrideL1ssCapsOffset (Base, Override);
  if (PcieCapOffset == 0) {
    PcieCapOffset = PcieBaseFindExtendedCapId (Base, V_PCIE_EX_L1S_CID);
  }
  if (PcieCapOffset == 0) {
    return;
  }

  Ctrl1Register |= (L1ss.PmL12 ? B_PCIE_EX_L1SCAP_PPL12S : 0);
  Ctrl1Register |= (L1ss.PmL11 ? B_PCIE_EX_L1SCAP_PPL11S : 0);
  Ctrl1Register |= (L1ss.AspmL12 ? B_PCIE_EX_L1SCAP_AL12S : 0);
  Ctrl1Register |= (L1ss.AspmL11 ? B_PCIE_EX_L1SCAP_AL1SS : 0);
  if ((GetDeviceType (Sbdf) == DevTypePcieDownstream)) {
    Ctrl1Register |= (L1ss.Cmrt << N_PCIE_EX_L1SCAP_CMRT);
  }
  ///
  /// BWG 1.3 Section 5.5.7.6 LTR Threshold Latency
  /// Set L1.2 LTR threshold using formula (TpoToUs (L1ss.TpoScale, L1ss.TpoValue) + L1ss.Cmrt + 10)
  ///
  Ctrl1Register |= ((TpoToUs (L1ss.TpoScale, L1ss.TpoValue) + L1ss.Cmrt + 10) << N_PCIE_EX_L1SCTL1_L12LTRTLV);
  Ctrl1Register |= (2 << N_PCIE_EX_L1SCTL1_L12LTRTLSV);

  Ctrl2Register |= (L1ss.TpoScale);
  Ctrl2Register |= (L1ss.TpoValue << N_PCIE_EX_L1SCTL2_POWT);
  //
  // Set CLKREQ Acceleration Interrupt Enable
  //
  Ctrl1Register |= B_PCIE_EX_L1SCTL1_L1SSEIE;
  PciSegmentWrite32 (Base + PcieCapOffset + R_PCIE_EX_L1SCTL1_OFFSET, 0);
  PciSegmentWrite32 (Base + PcieCapOffset + R_PCIE_EX_L1SCTL2_OFFSET, Ctrl2Register);
  PciSegmentWrite32 (Base + PcieCapOffset + R_PCIE_EX_L1SCTL1_OFFSET, Ctrl1Register);
}

/**
  Converts L1 latency from enumerated register value to microseconds

  @param[in] L1Latency     latency value retrieved from register; see PCIE specification for encoding

  @retval    L1 latency converted to microseconds
**/
UINT32
L1LatencyToUs (
  UINT32 L1Latency
  )
{
  if (L1Latency < 7) {
    return 1 * (BIT0 << L1Latency);
  } else {
    return ASPM_L1_NO_LIMIT;
  }
}

/**
  Modifies L1 latency by provided value

  @param[in] Aspm     Structure that contains ASPM capabilities of a link, including L1 acceptable latency
  @param[in] Value    Value, in microseconds, to be added to L1 acceptable latency. Can be negative.

  @retval             Aspm structure with modified L1 acceptable latency
**/
STATIC
ASPM_CAPS
PatchL1AcceptableLatency (
  ASPM_CAPS Aspm,
  INT8      Value
  )
{
  if (Aspm.L1AcceptableLatencyUs != ASPM_L1_NO_LIMIT) {
    if (Value > 0) {
      Aspm.L1AcceptableLatencyUs += Value;
    } else {
      if (Aspm.L1AcceptableLatencyUs > (UINT32)(-1*Value)) {
        Aspm.L1AcceptableLatencyUs = Aspm.L1AcceptableLatencyUs + Value;
      } else {
        Aspm.L1AcceptableLatencyUs = 0;
      }
    }
  }
  return Aspm;
}

/**
  Reads ASPM capabilities of a device

  @param[in] Sbdf segment:bus:device:function coordinates of a device

  @retval           structure containing device's ASPM capabilities
**/
STATIC
ASPM_CAPS
GetAspmCaps (
  SBDF   Sbdf
  )
{

  UINT32    LinkCapRegister;
  UINT32    DevCapRegister;
  UINT64    Base;
  ASPM_CAPS Aspm = {0};

  Base = SbdfToBase (Sbdf);

  LinkCapRegister = PciSegmentRead32 (Base + Sbdf.PcieCap + R_PCIE_LCAP_OFFSET);
  DevCapRegister = PciSegmentRead32 (Base + Sbdf.PcieCap + R_PCIE_DCAP_OFFSET);

  ///
  /// Check endpoint for pre-1.1 devices based on the Role based Error Reporting Capability bit. Don't report L0s support for old devices
  ///
  if (DevCapRegister & B_PCIE_DCAP_RBER) {
    Aspm.L0sSupported = !!(LinkCapRegister & B_PCIE_LCAP_APMS_L0S);
  }
  Aspm.L1Supported = !!(LinkCapRegister & B_PCIE_LCAP_APMS_L1);

  Aspm.LinkL0sExitLatency = (LinkCapRegister & B_PCIE_LCAP_EL0) >> N_PCIE_LCAP_EL0;
  Aspm.LinkL1ExitLatencyUs = L1LatencyToUs( (LinkCapRegister & B_PCIE_LCAP_EL1) >> N_PCIE_LCAP_EL1);

  if (GetDeviceType (Sbdf) == DevTypePcieEndpoint) {
    Aspm.L0sAcceptableLatency = (DevCapRegister & B_PCIE_DCAP_E0AL) >> N_PCIE_DCAP_E0AL;
    Aspm.L1AcceptableLatencyUs = L1LatencyToUs( (DevCapRegister & B_PCIE_DCAP_E1AL) >> N_PCIE_DCAP_E1AL);
    DEBUG ((DEBUG_INFO, "GetAspmCaps %02x:%02x:%02x L0s%c %d:%d L1%c %d:%d\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func,
                                                                           Aspm.L0sSupported?'+':'-', Aspm.LinkL0sExitLatency, Aspm.L0sAcceptableLatency,
                                                                           Aspm.L1Supported?'+':'-', Aspm.LinkL1ExitLatencyUs, Aspm.L1AcceptableLatencyUs));
  } else {
    Aspm.L0sAcceptableLatency = ASPM_L0s_NO_LIMIT;
    Aspm.L1AcceptableLatencyUs = ASPM_L1_NO_LIMIT;
    DEBUG ((DEBUG_INFO, "GetAspmCaps %02x:%02x:%02x L0s%c %d:x L1%c %d:x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func,
                                                                           Aspm.L0sSupported?'+':'-', Aspm.LinkL0sExitLatency,
                                                                           Aspm.L1Supported?'+':'-', Aspm.LinkL1ExitLatencyUs));
  }
  return Aspm;
}

/**
  Get ASPM L0s and L1 override of given device.

  @param[in] Sbdf                Segment,Bus,Device,Function address of currently visited PCIe device
  @param[in,out] MyAspm          Current device's ASPM capabilities structure
  @param[in] Override            Pch Pcie devices OverrideTable
**/
STATIC
VOID
GetOverrideAspm (
  SBDF           Sbdf,
  ASPM_CAPS      *MyAspm,
  OVERRIDE_TABLE *Override
  )
{
  UINT16      DeviceId;
  UINT16      VendorId;
  UINT8       Revision;
  UINT32      Index;
  UINT64      Base;

  Base = SbdfToBase (Sbdf);

  VendorId = PciSegmentRead16 (Base + PCI_VENDOR_ID_OFFSET);
  DeviceId = PciSegmentRead16 (Base + PCI_DEVICE_ID_OFFSET);
  Revision = PciSegmentRead8  (Base + PCI_REVISION_ID_OFFSET);

  for (Index = 0; Index < Override->Size; Index++) {
    if (((Override->Table[Index].OverrideConfig & PchPcieL1L2Override) == PchPcieL1L2Override) &&
        (Override->Table[Index].VendorId == VendorId) &&
        (Override->Table[Index].DeviceId == DeviceId) &&
        (Override->Table[Index].RevId == Revision || Override->Table[Index].RevId == 0xFF)) {
      DEBUG ((DEBUG_INFO, "GetOverrideAspm %02x:%02x:%02x, original L0sSupported = 0x%x, L1Supported = 0x%x\n",
              Sbdf.Bus, Sbdf.Dev, Sbdf.Func, MyAspm->L0sSupported, MyAspm->L1Supported));
      if (MyAspm->L0sSupported) {
        //
        // If L0s is supported in capability, apply platform override.
        //
        MyAspm->L0sSupported = Override->Table[Index].EndPointAspm & BIT0;
      }
      if (MyAspm->L1Supported) {
        //
        // If L1 is supported in capability, apply platform override.
        //
        MyAspm->L1Supported = (Override->Table[Index].EndPointAspm & BIT1) >> 1;
      }
      DEBUG ((DEBUG_INFO, "GetOverrideAspm %02x:%02x:%02x, override L0sSupported = 0x%x, L1Supported = 0x%x\n",
              Sbdf.Bus, Sbdf.Dev, Sbdf.Func, MyAspm->L0sSupported, MyAspm->L1Supported));
    }
  }
}

/**
  Combines ASPM capabilities of two devices on both ends of a link to determine link's ASPM capabilities

  @param[in] AspmA, AspmB  ASPM capabilities of two devices

  @retval    ASPM_CAPS structure containing combined ASPM capabilities
**/
STATIC
ASPM_CAPS
CombineAspm (
  ASPM_CAPS AspmA,
  ASPM_CAPS AspmB,
  BOOLEAN   DownstreamPort
  )
{
  ASPM_CAPS Combined;

  if (DownstreamPort) {
    //
    // When combining ASPM in downstream ports, combination must reflect state of link just below
    // and consider all acceptable latencies of all endpoints anywhere down below that port
    //
    Combined.L0sSupported = AspmA.L0sSupported & AspmB.L0sSupported;
    Combined.L1Supported = AspmA.L1Supported & AspmB.L1Supported;
    Combined.LinkL0sExitLatency = MAX (AspmA.LinkL0sExitLatency, AspmB.LinkL0sExitLatency);
    Combined.LinkL1ExitLatencyUs = MAX (AspmA.LinkL1ExitLatencyUs, AspmB.LinkL1ExitLatencyUs);
    Combined.L0sAcceptableLatency = MIN (AspmA.L0sAcceptableLatency, AspmB.L0sAcceptableLatency);
    Combined.L1AcceptableLatencyUs = MIN (AspmA.L1AcceptableLatencyUs, AspmB.L1AcceptableLatencyUs);
  } else {
    //
    // When combining ASPM in switch upstream ports,
    // Supported and ExitLatency must only reflect capabilities of upstream port itself
    // But acceptable latencies must consider all endpoints anywhere below
    //
    Combined.L0sSupported = AspmA.L0sSupported;
    Combined.L1Supported = AspmA.L1Supported;
    Combined.LinkL0sExitLatency = AspmA.LinkL0sExitLatency;
    Combined.LinkL1ExitLatencyUs = AspmA.LinkL1ExitLatencyUs;
    Combined.L0sAcceptableLatency = MIN (AspmA.L0sAcceptableLatency, AspmB.L0sAcceptableLatency);
    Combined.L1AcceptableLatencyUs = MIN (AspmA.L1AcceptableLatencyUs, AspmB.L1AcceptableLatencyUs);
  }
  DEBUG ((DEBUG_INFO, "CombineAspm %x:%x -> %x\n", AspmA.L1AcceptableLatencyUs, AspmB.L1AcceptableLatencyUs, Combined.L1AcceptableLatencyUs));
  return Combined;
}

/**
  Checks if L1 can be enabled on given link, according to ASPM parameters of that link

  @param[in] Aspm            set of parameters describing this link and endpoint devices below it

  @retval    TRUE if L1 can be enabled
**/
STATIC
BOOLEAN
IsL1Allowed (
  ASPM_CAPS Aspm
  )
{
  return (Aspm.L1Supported && (Aspm.L1AcceptableLatencyUs >= Aspm.LinkL1ExitLatencyUs));
}

/**
  Checks if L0s can be enabled on given link, according to ASPM parameters of that link

  @param[in] Aspm            set of parameters describing this link and endpoint devices below it

  @retval    TRUE if L0s can be enabled
**/
STATIC
BOOLEAN
IsL0sAllowed (
  ASPM_CAPS Aspm
  )
{
  return (Aspm.L0sSupported && (Aspm.L0sAcceptableLatency >= Aspm.LinkL0sExitLatency));
}

/**
  Enables L0s and L1 for given port, if possible.
  L0s/L1 can be enabled if it's supported on both sides of a link and if link's latency doesn't exceed
  acceptable latency of any endpoint below this link

  @param[in] Base            device's base address
  @param[in] Aspm            set of parameters describing this link and endpoint devices below it
**/
STATIC
VOID
SetAspm (
  SBDF      Sbdf,
  ASPM_CAPS Aspm
  )
{
  UINT16 DataOr;

  DataOr = 0;
  if (IsL0sAllowed (Aspm)) {
    DataOr |= V_PCIE_LCTL_ASPM_L0S;
  }
  if (IsL1Allowed (Aspm)) {
    DataOr |= V_PCIE_LCTL_ASPM_L1;
  }
  DEBUG ((DEBUG_INFO, "SetAspm on %02x:%02x:%02x to %d\n", Sbdf.Bus,Sbdf.Dev,Sbdf.Func, DataOr));
  PciSegmentAndThenOr16 (SbdfToBase (Sbdf) + Sbdf.PcieCap + R_PCIE_LCTL_OFFSET, (UINT16)~B_PCIE_LCTL_ASPM, DataOr);
}

/**
  Adds device entry to a list of devices.

  @param[in,out] Table    array of devices
  @param[in]     Sbdf     segment:bus:device:function coordinates of device to be added to table
**/
STATIC
VOID
AddToDeviceTable (
  SBDF_TABLE *Table,
  SBDF       Sbdf
  )
{
  if (Table->Count < MAX_SBDF_TABLE_SIZE) {
    Table->Entry[Table->Count++] = Sbdf;
  } else {
    ASSERT (FALSE);
  }
}

/**
  Remove device entry from a list and clear its bus assignment

  @param[in,out] Table    array of devices
**/
STATIC
VOID
ClearBusFromTable (
  SBDF_TABLE *Table
  )
{
  while (Table->Count > 0) {
    PciSegmentWrite32 (SbdfToBase (Table->Entry[Table->Count - 1]) + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, 0);
    Table->Count--;
  }
}

/**
  Attempts to assign secondary and subordinate bus numbers to uninitialized bridges in PCIe tree
  If the device is a bridge and already has bus numbers assigned, they won't be changed
  Otherwise new bus number will be assigned below this bridge.
  This function can be called from SMM, where BIOS must not modify bus numbers to prevent
  conflict with OS enumerator. To prevent this, this function returns list of bridges whose
  bus numbers were changed. All devices from that list must have buses cleared afterwards.

  @param[in] Sbdf                segment:bus:device:function coordinates of device to be added to table
  @param[in] MinBus              minimum Bus number that can be assigned below this port
  @param[in] MaxBus              maximum Bus number that can be assigned below this port
  @param[in] BridgeCleanupList   list of bridges where bus numbers were modified

  @retval    maximum bus number assigned anywhere below this device
**/
STATIC
UINT8
RecursiveBusAssignment (
  SBDF       Sbdf,
  UINT8      MinBus,
  UINT8      MaxBus,
  SBDF_TABLE *BridgeCleanupList
  )
{
  UINT64  Base;
  SBDF    ChildSbdf;
  PCI_DEV_TYPE DevType;
  UINT32  Data32;
  UINT8   BelowBus;
  UINT8   SecondaryBus;
  UINT8   SubordinateBus;

  ChildSbdf.Seg = Sbdf.Seg;
  InitChildFinder (&ChildSbdf);
  Base = SbdfToBase (Sbdf);

  //
  // On way down:
  //   assign secondary bus, then increase it by one before stepping down; temporarily assign max subordinate bus
  // On way up:
  //   fix subordinate bus assignment to equal max bus number assigned anywhere below; return that number
  //
  DevType = GetDeviceType (Sbdf);
  if ((Sbdf.Bus >= MaxBus) || (DevType == DevTypePcieEndpoint) || (DevType == DevTypePci)) {
    return (UINT8) Sbdf.Bus;
  } else  {
    Data32 = PciSegmentRead32 (Base + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET);
    SecondaryBus = (UINT8)((Data32 & B_PCI_BRIDGE_BNUM_SCBN) >> 8);
    SubordinateBus = (UINT8)((Data32 & B_PCI_BRIDGE_BNUM_SBBN) >> 16);
    if (SecondaryBus != 0) {
      ChildSbdf.Bus = SecondaryBus;
      MinBus = SecondaryBus + 1;
      DEBUG ((DEBUG_INFO, "RecursiveBusAssignmentP %x:%x:%x -> %x,%x,%x \n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func, Sbdf.Bus, MinBus, SubordinateBus));
      while (FindNextPcieChild (DevType, &ChildSbdf)) {
        BelowBus = RecursiveBusAssignment (ChildSbdf, MinBus, SubordinateBus, BridgeCleanupList);
        MinBus = BelowBus + 1;
      }
      return SubordinateBus;
    } else {
      Data32 = Sbdf.Bus + (MinBus << 8) + (MaxBus << 16);
      PciSegmentWrite32(Base + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, Data32);
      AddToDeviceTable (BridgeCleanupList, Sbdf);
      DEBUG ((DEBUG_INFO, "RecursiveBusAssignmentE %x:%x:%x -> %x,%x,%x \n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func, Sbdf.Bus, MinBus, MaxBus));
      BelowBus = MinBus;
      ChildSbdf.Bus = MinBus;
      MinBus++;
      while (FindNextPcieChild (DevType, &ChildSbdf)) {
        BelowBus = RecursiveBusAssignment (ChildSbdf, MinBus, MaxBus, BridgeCleanupList);
        MinBus = BelowBus + 1;
      }
      Data32  &= ~B_PCI_BRIDGE_BNUM_SBBN;
      Data32 |= (BelowBus << 16);
      PciSegmentWrite32 (Base + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, Data32);
      DEBUG ((DEBUG_INFO, "RecursiveBusAssignmentL %x:%x:%x -> %x,%x,%x \n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func, Sbdf.Bus, (Data32&0xFF00)>>8, BelowBus));
      return BelowBus;
    }
  }
}

/**
  Enables L0s and/or L1 for PCIE links in the hierarchy below
  L0s/L1 can be enabled when both sides of a link support it and link latency is smaller than acceptable latency
  ASPM of a given link is independend from any other link (except 1ms L1 adjustment, read below), so it's possible to
  have a hierarchy when RP link has no ASPM but links below do.

  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device
  @param[in] Depth                          How many links there are between this port and root complex
  @param[in] Override                       Pch Pcie devices OverrideTable

  @retval structure that describes acceptable latencies of all endpoints below plus ASPM parameters of last link
**/
STATIC
ASPM_CAPS
RecursiveAspmConfiguration (
  SBDF           Sbdf,
  UINT8          Depth,
  OVERRIDE_TABLE *Override
  )
{
  SBDF         ChildSbdf;
  ASPM_CAPS    MyAspm;
  ASPM_CAPS    ChildAspm;
  PCI_DEV_TYPE DevType;

  DEBUG ((DEBUG_INFO, "RecursiveAspmConfiguration %x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  //
  // On way down:
  //   pass number of links traversed; increase it per upstream port visited (not endpoint)
  // On way up:
  //   EndPoint: read Acceptable Latencies; subtract Depth From L1AcceptableLat to account for "1us per switch additional delay"
  //   Downstreamport: AND L0s/L1 caps; calculate LinkLatency; enable L0s/L1 if supported and if acceptable latency is bigger than link latency;
  //     if L1 not enabled, add back 1us to Acceptable Latency to cancel earlier Depth subtraction
  //   UpstreamPort: calculate minimum of below Acceptable Latencies; return that, with upper link's Latency and L0s/L1 support
  //
  DevType = GetDeviceType(Sbdf);
  if (DevType == DevTypePcieUpstream) {
    Depth++;
  }
  MyAspm = GetAspmCaps (Sbdf);
  //
  // Get ASPM L0s and L1 override
  //
  if (Override != NULL) {
    GetOverrideAspm (Sbdf, &MyAspm, Override);
  }
  if (DevType == DevTypePcieEndpoint) {
    //
    // Every switch between endpoint and CPU introduces 1us additional latency on L1 exit. This is reflected by
    // subtracting 1us per switch from endpoint's acceptable L1 latency.
    // In case L1 doesn't get enabled in one of switches, that 1us will be added back.
    // This calculation is not precise. It ignores that some switches' added delay may be shadowed by
    // other links' exit latency. But it guarantees that acceptable latency won't be exceeded and is simple
    // enough to perform in a single iteration without backtracking.
    //
    return PatchL1AcceptableLatency (MyAspm, (-1 * Depth));
  }
  if (HasChildBus (Sbdf, &ChildSbdf)) {
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      ChildAspm = RecursiveAspmConfiguration (ChildSbdf, Depth, Override);
      MyAspm = CombineAspm (MyAspm, ChildAspm, (DevType == DevTypePcieDownstream));
    }
    if (DevType == DevTypePcieDownstream) {
      SetAspm (Sbdf, MyAspm);
      //
      // ASPM config must be consistent across all functions of a device. That's why there's while loop.
      //
      while (FindNextPcieChild (DevType, &ChildSbdf)) {
        SetAspm (ChildSbdf, MyAspm);
      }
      if (!IsL1Allowed (MyAspm)) {
        MyAspm = PatchL1AcceptableLatency (MyAspm, 1);
      }
    }
  }
  return MyAspm;
}

/**
  Enables L1 substates for PCIE links in the hierarchy below
  L1.1 / L1.2 can be enabled if both sides of a link support it.

  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device

  @retval  structure that describes L1ss capabilities of the device
**/
STATIC
L1SS_CAPS
RecursiveL1ssConfiguration (
  SBDF           Sbdf,
  OVERRIDE_TABLE *Override
  )
{
  UINT64  Base;
  SBDF    ChildSbdf;
  L1SS_CAPS CombinedCaps;
  L1SS_CAPS ChildCaps;
  PCI_DEV_TYPE DevType;
  BOOLEAN   IsCpuPcie;
  UINT32    DevNum;

  IsCpuPcie = FALSE;

  DEBUG ((DEBUG_INFO, "RecursiveL1ssConfiguration %x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  Base = SbdfToBase (Sbdf);
  DevNum = Sbdf.Dev;
  if (DevNum == SA_PEG0_DEV_NUM || DevNum == SA_PEG3_DEV_NUM) {
    IsCpuPcie = TRUE;
  }
  //
  // On way down:
  //   do nothing
  // On way up:
  //   In downstream ports, combine L1ss capabilities of that port and device behind it, then enable L1.1 and/or L1.2 if possible
  //   Return L1ss capabilities
  //
  if (HasChildBus (Sbdf, &ChildSbdf)) {
    DevType = GetDeviceType (Sbdf);
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      ChildCaps = RecursiveL1ssConfiguration (ChildSbdf, Override);
      if (DevType == DevTypePcieDownstream && ChildSbdf.Func == 0) {
        CombinedCaps = CombineL1ss (GetL1ssCaps (Base, Override), ChildCaps);
        SetL1ss (Sbdf, CombinedCaps, Override, IsCpuPcie);
        SetL1ss (ChildSbdf, CombinedCaps, Override, IsCpuPcie);
      }
    }
  }
  return GetL1ssCaps (Base, Override);
}

/**
  Checks if there is an IoAPIC device in the PCIe hierarchy.
  If one is found, this function doesn't check for more and returns

  @param[in] BusLimit                       maximum Bus number that can be assigned below this port
  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device

  @retval  TRUE if IoAPIC device was found
**/
STATIC
BOOLEAN
RecursiveIoApicCheck (
  SBDF       Sbdf
  )
{
  SBDF         ChildSbdf;
  UINT8        IoApicPresent;
  PCI_DEV_TYPE DevType;

  DEBUG ((DEBUG_INFO, "RecursiveIoApicCheck %x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  IoApicPresent = FALSE;

  if (IsIoApicDevice (SbdfToBase (Sbdf))) {
    DEBUG ((DEBUG_INFO, "IoApicFound @%x:%x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));
    return TRUE;
  }
  if (HasChildBus (Sbdf, &ChildSbdf)) {
    DevType = GetDeviceType (Sbdf);
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      IoApicPresent = RecursiveIoApicCheck (ChildSbdf);
      if (IoApicPresent) {
        break;
      }
    }
  }
  DEBUG ((DEBUG_INFO, "IoApic status %d @%x:%x:%x:%x\n", IoApicPresent, Sbdf.Seg, Sbdf.Bus, Sbdf.Dev, Sbdf.Func));
  return IoApicPresent;
}

/**
  Calculates Maximum Payload Size supported by PCIe hierarchy.
  Starting from a device, it finds the minimum MPS supported by devices below it.
  There are many valid strategies for setting MPS. This implementation chooses
  one that is safest, but doesn't guarantee maximum performance:
    Find minimum MPS under given rootport, then program that minimum value everywhere below that rootport

  @param[in] BusLimit                       maximum Bus number that can be assigned below this port
  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device

  @retval  MPS supported by PCIe hierarchy, calculated as MIN(MPS of all devices below)
**/
STATIC
UINT8
RecursiveMpsCheck (
  SBDF       Sbdf
  )
{
  SBDF         ChildSbdf;
  UINT8        MyMps;
  UINT8        SubtreeMps;
  PCI_DEV_TYPE DevType;

  DEBUG ((DEBUG_INFO, "RecursiveMpsCheck %x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  MyMps = GetMps (Sbdf);
  if (MyMps == 0) {
    return MyMps;
  }
  if (HasChildBus (Sbdf, &ChildSbdf)) {
    DevType = GetDeviceType (Sbdf);
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      SubtreeMps = RecursiveMpsCheck (ChildSbdf);
      MyMps = MIN(MyMps, SubtreeMps);
    }
  }
  return MyMps;
}

/**
  Sets Maximum Payload Size in PCIe hierarchy.
  Starting from a device, it programs the same MPS value to it and all devices below it.
  There are many valid strategies for setting MPS. This implementation chooses
  one that is safest, but doesn't guarantee maximum performance:
    Find minimum MPS under given rootport, then program that minimum value everywhere below that rootport

  @param[in] BusLimit                       maximum Bus number that can be assigned below this port
  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device
  @param[in] Mps                            Maximum Payload Size to be programmed
**/
STATIC
VOID
RecursiveMpsConfiguration (
  SBDF       Sbdf,
  UINT8      Mps
  )
{
  SBDF    ChildSbdf;
  PCI_DEV_TYPE DevType;

  DEBUG ((DEBUG_INFO, "RecursiveMpsConfiguration %x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  if (HasChildBus (Sbdf, &ChildSbdf)) {
    DevType = GetDeviceType (Sbdf);
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      RecursiveMpsConfiguration (ChildSbdf, Mps);
    }
  }
  SetMps (Sbdf, Mps);
}

/**
  Sets Enable Clock Power Management bit for devices that support it.
  A device supports CPM only if all function of this device report CPM support.
  Downstream ports never report CPM capability, so it's only relevant for upstream ports.
  When this function executes on upstream component, it will check CPM & set ECPM of downstream component
  When this function executes on downstream component, all devices below it are guaranteed to
  return CPM=0 so it will do nothing

  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device

  @retval TRUE = this device supports CPM, FALSE = it doesn't
**/
STATIC
BOOLEAN
RecursiveCpmConfiguration (
  SBDF       Sbdf
  )
{
  SBDF         ChildSbdf;
  BOOLEAN      ChildCpm;
  PCI_DEV_TYPE DevType;

  DEBUG ((DEBUG_INFO, "RecursiveCpmConfiguration %x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  ChildCpm = FALSE;

  if (HasChildBus (Sbdf, &ChildSbdf)) {
    ChildCpm = TRUE;
    DevType = GetDeviceType (Sbdf);
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      ChildCpm &= RecursiveCpmConfiguration (ChildSbdf);
    }
    if (ChildCpm) {
      while (FindNextPcieChild (DevType, &ChildSbdf)) {
        EnableCpm (ChildSbdf);
      }
    }
  }
  return IsCpmSupported (Sbdf);
}

/**
  Sets Common Clock Configuration bit for devices that share common clock across link
  Devices on both sides of a PCIE link share common clock if both upstream component
  and function 0 of downstream component report Slot Clock Configuration bit = 1.
  When this function executes on upstream component, it checks SCC of both sides of the link
  If they both support it, sets CCC for both sides (this means all functions of downstream component)
  When this function executes on downstream component, it only returns SCC capability

  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device
  @param[in] WaitForRetrain                 decides if this function should busy-wait for link retrain

  @retval TRUE = this device supports SCC, FALSE = it doesn't
**/
STATIC
BOOLEAN
RecursiveCccConfiguration (
  SBDF       Sbdf,
  BOOLEAN    WaitForRetrain
  )
{
  UINT64       Base;
  SBDF         ChildSbdf;
  BOOLEAN      MyScc;
  BOOLEAN      ChildScc;
  BOOLEAN      LinkScc;
  PCI_DEV_TYPE DevType;

  DEBUG ((DEBUG_INFO, "RecursiveCccConfiguration %x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  ChildScc = 0;
  Base = SbdfToBase(Sbdf);
  MyScc = GetScc (SbdfToBase(Sbdf), (UINT8)Sbdf.PcieCap);
  if (HasChildBus (Sbdf, &ChildSbdf)) {
    DevType = GetDeviceType (Sbdf);
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      ChildScc |= RecursiveCccConfiguration (ChildSbdf, WaitForRetrain);
    }
    if (DevType == DevTypePcieDownstream) {
      LinkScc = MyScc & ChildScc;
      if (LinkScc) {
        EnableCcc (SbdfToBase(Sbdf), (UINT8)Sbdf.PcieCap);
        while (FindNextPcieChild (DevType, &ChildSbdf)) {
          EnableCcc (SbdfToBase(ChildSbdf), (UINT8)ChildSbdf.PcieCap);
        }
        RetrainLink(Base, (UINT8)Sbdf.PcieCap, WaitForRetrain);
      }
    }
  }
  return MyScc;
}

/**
  Configures Latency Tolerance Reporting in given device and in PCIe tree below it.
  This function configures Maximum LTR and enables LTR mechanism. It visits devices using depth-first search
  and skips branches behind devices which do not support LTR.
  Maximum LTR:
    This function will set LTR's upper bound for every visited device. Max LTR value is provided as a parameter
  Enable LTR:
    LTR should be enabled top-to-bottom in every visited device that supports LTR. This function does not
    iterate down behind devices with no LTR support. In effect, LTR will be enabled in given device if that device
    and all devices above it on the way to RootComplex do support LTR.

  This function expects that bridges have bus numbers already configured

  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device
  @param[in] LtrLimit                       Ltr to be programmed to every endpoint

  @retval MaxLTR programmed in this device
**/
STATIC
VOID
RecursiveLtrConfiguration (
  SBDF       Sbdf,
  LTR_LIMIT  LtrLimit
  )
{
  UINT64  Base;
  SBDF    ChildSbdf;
  PCI_DEV_TYPE DevType;

  DEBUG ((DEBUG_INFO, "RecursiveLtrConfiguration %x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  Base = SbdfToBase(Sbdf);

  if (!IsLtrCapable (Sbdf)) {
    DEBUG ((DEBUG_INFO, "Not LtrCapable %02x:%02x:%02x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));
    return;
  }
  EnableLtr (Sbdf);
  if (HasChildBus (Sbdf, &ChildSbdf)) {
    DevType = GetDeviceType (Sbdf);
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      RecursiveLtrConfiguration (ChildSbdf, LtrLimit);
    }
  }
  SetLtrLimit (Base, LtrLimit);
}

/**
  Checks to see device is PTM Capable

  @param[in] Sbdf    device's segment:bus:device:function coordinates

  @retval TRUE = PTM Capability found, FALSE = Not PTM capable
**/
STATIC
BOOLEAN
IsPtmCapable (
  SBDF       Sbdf
  )
{
  UINT16     CapHeaderOffset;

  CapHeaderOffset = PcieFindExtendedCapId ((UINT8) Sbdf.Seg, (UINT8) Sbdf.Bus, (UINT8) Sbdf.Dev, (UINT8) Sbdf.Func, V_PCIE_EX_PTM_CID);

  return (CapHeaderOffset != 0);
}

/**
  Get PTM Capability register from PCIe Extended Capability Space.

  @param[in] Sbdf                device's segment:bus:device:function coordinates
  @param[in] PtmCapHeaderOffset  PTM Capability Header Offset

  @retval LocalPtm       Returns PTM Capability.
                         If Device is not PTM capable then PTM Capability is zeroed out.
**/
STATIC
PTM_CAPS
GetPtmCapability (
  SBDF      Sbdf,
  UINT16    PtmCapHeaderOffset
  )
{
  PTM_CAPS PtmCaps;

  PtmCaps.Uint32 = 0;

  if (PtmCapHeaderOffset != 0) {
    PtmCaps.Uint32 = PciSegmentRead32 (SbdfToBase (Sbdf) + PtmCapHeaderOffset + R_PCIE_EX_PTMCAP_OFFSET);
  }

  return PtmCaps;
}

/**
  Get PTM Control register from PCIe Extended Capability Space.

  @param[in] Sbdf                device's segment:bus:device:function coordinates
  @param[in] PtmCapHeaderOffset  PTM Capability Header Offset

  @retval LocalPtm       Returns PTM Control.
                         If Device is not PTM capable then PTM Control is zeroed out.
**/
STATIC
PTM_CTRL
GetPtmControl (
  SBDF      Sbdf,
  UINT16    PtmCapHeaderOffset
  )
{
  PTM_CTRL PtmCtrl;

  PtmCtrl.Uint32 = 0;

  if (PtmCapHeaderOffset != 0) {
    PtmCtrl.Uint32 = PciSegmentRead32 (SbdfToBase (Sbdf) + PtmCapHeaderOffset + R_PCIE_EX_PTMCTL_OFFSET);
  }

  return PtmCtrl;
}

/**
  Set PTM Control register in the PCIe Extended Capability Space.

  @param[in] Sbdf                device's segment:bus:device:function coordinates
  @param[in] PtmCapHeaderOffset  PTM Capability Header Offset
  @param[in] PtmCtrl             PTM Control Register
**/
STATIC
VOID
SetPtmControl (
  SBDF      Sbdf,
  UINT16    PtmCapHeaderOffset,
  PTM_CTRL  PtmCtrl
  )
{
  if (PtmCapHeaderOffset != 0) {
    PciSegmentWrite32 (SbdfToBase (Sbdf) + PtmCapHeaderOffset + R_PCIE_EX_PTMCTL_OFFSET, PtmCtrl.Uint32);
  }
}

/**
  Enable PTM on device's control register. Set the Effective Clock Granularity.

  @param[in]     Sbdf                 device's segment:bus:device:function coordinates
  @param[in out] EffectiveGranularity Effective Clock Granularity of the PTM hierarchy
  @param[in out] PtmHierarchy         Indicates if the current device is within a PTM hierarchy
**/
STATIC
VOID
SetPtm (
  IN     SBDF       Sbdf,
  IN OUT UINT8      *EffectiveGranularity,
  IN OUT BOOLEAN    *PtmHierarchy
  )
{
  PTM_CTRL CurrentPtmCtrl;
  PTM_CAPS CurrentPtmCaps;
  PTM_CTRL OrigPtmCtrl;
  UINT16   PtmCapHeaderOffset;

  PtmCapHeaderOffset = PcieFindExtendedCapId ((UINT8) Sbdf.Seg, (UINT8) Sbdf.Bus, (UINT8) Sbdf.Dev, (UINT8) Sbdf.Func, V_PCIE_EX_PTM_CID);
  CurrentPtmCtrl = GetPtmControl (Sbdf, PtmCapHeaderOffset);
  CurrentPtmCaps = GetPtmCapability (Sbdf, PtmCapHeaderOffset);

  OrigPtmCtrl.Uint32 = CurrentPtmCtrl.Uint32;

  if ( (*PtmHierarchy == FALSE) && CurrentPtmCaps.Bits.RootCapable) {
    // Select device as PTM Root if PTM Root is not selected.
    CurrentPtmCtrl.Bits.RootSelect = TRUE;
    *EffectiveGranularity = (UINT8) CurrentPtmCaps.Bits.LocalClockGranularity;
    *PtmHierarchy = TRUE;
  }

  if (*PtmHierarchy == TRUE) {
    // Enable PTM if device is part of a ptm hierarchy
    CurrentPtmCtrl.Bits.Enable = TRUE;

    if (CurrentPtmCaps.Bits.RequesterCapable) {
      // Set Effective Granularity if PTM device is Requester roles.
      CurrentPtmCtrl.Bits.EffectiveGranularity = *EffectiveGranularity;
    }
  }

  if (OrigPtmCtrl.Uint32 != CurrentPtmCtrl.Uint32) {
    SetPtmControl (Sbdf, PtmCapHeaderOffset, CurrentPtmCtrl);
  }

  // Update EffectiveGranularity.
  // Set to zero if 1 or more switches between the root and endpoint report local granularity = 0.
  // Otherwise set to the max local granularity of the hierarchy.
  if ( ((CurrentPtmCaps.Bits.LocalClockGranularity == 0) && CurrentPtmCaps.Bits.RequesterCapable) ||
       (*EffectiveGranularity  == 0) ) {
    *EffectiveGranularity = 0;
  } else {
    *EffectiveGranularity = MAX (*EffectiveGranularity, (UINT8) CurrentPtmCaps.Bits.LocalClockGranularity);
  }
}

/**
  Configures PTM hierarchies by searching for Endpoints and Upstream Switches
  that are PTM capable. Each PTM device role is identified and configured accordingly.
  PTM root capable devices are selected as PTM root if the device is not already in a
  PTM hierarchy.
  PTM capable Root Ports must be configured before calling this function.
  @note: This function has not been tested with switches.

  @param[in] Sbdf                 Address of curretly visited Pcie device
  @param[in] EffectiveGranularity The largest Clock Granularity from Upstream Devices
  @param[in] PtmHierarchy         TRUE = Device in a PTM Hierarchy, FALSE = Not in PTM Hierarchy
**/
STATIC
VOID
RecursivePtmConfiguration (
  SBDF          Sbdf,
  UINT8         EffectiveGranularity,
  BOOLEAN       PtmHierarchy
  )
{
  SBDF         ChildSbdf;
  PCI_DEV_TYPE DevType;

  DEBUG ((DEBUG_INFO, "RecursivePtmConfiguration %x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  DevType = GetDeviceType (Sbdf);

  if (IsPtmCapable (Sbdf)) {
    //
    // Enable PTM for PTM Capable devices (Endpoints, Upstream Switch, and Root Ports).
    // @Note: Switches have not been tested.
    //
    SetPtm (Sbdf, &EffectiveGranularity, &PtmHierarchy);
  } else if (!IsPtmCapable (Sbdf) && (DevType == DevTypePcieUpstream) ) {
    //
    // Non-PTM UpStream Switch Ports breaks the PTM Hierarchy.
    // No other downstream PTM devices should be PTM enabled until a PTM Root capable device is selected.
    //
    PtmHierarchy = FALSE;
    EffectiveGranularity = 0;
  }

  if (HasChildBus (Sbdf, &ChildSbdf)) {
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      RecursivePtmConfiguration (ChildSbdf, EffectiveGranularity, PtmHierarchy);
    }
  }
}

/**
  Initializes the following features in rootport and devices behind it:
  Maximum Payload Size (generic)
  Rootport packet split (proprietary)
  EonOfInterrupt forwarding (proprietary)
  Common Clock Configuration (generic)

  Generic: any code written according to PCIE Express base specification can do that.
  Proprietary: code uses registers and features that are specific to Intel silicon
  and probably only this Reference Code knows how to handle that.

  If OEM implemented generic feature enabling in his platform code or trusts Operating System
  to do it, then those features can be deleted from here.

  CCC requires link retrain, which takes a while. CCC must happen before L0s/L1 programming.
  If there was guarantee no code would access PCI while links retrain, it would be possible to skip this waiting

  @param[in] RpSegment  address of rootport on PCIe
  @param[in] RpBus      address of rootport on PCIe
  @param[in] RpDevice   address of rootport on PCIe
  @param[in] RpFunction address of rootport on PCIe
  @param[in] BusMin     minimum Bus number that can be assigned below this rootport
  @param[in] BusMax     maximum Bus number that can be assigned below this rootport
**/
VOID
RootportDownstreamConfiguration (
  UINT8                     RpSegment,
  UINT8                     RpBus,
  UINT8                     RpDevice,
  UINT8                     RpFunction,
  UINT8                     BusMin,
  UINT8                     BusMax,
  PCI_SKU                   PciSku
  )
{
  UINT8           Mps;
  BOOLEAN         IoApicPresent;
  UINT64          RpBase;
  SBDF            RpSbdf;
  SBDF_TABLE      BridgeCleanupList;

  IoApicPresent = FALSE;
  RpBase = PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, 0);
  if (!(IsDevicePresent (RpBase))) {
    return;
  }
  RpSbdf.Seg = RpSegment;
  RpSbdf.Bus = RpBus;
  RpSbdf.Dev = RpDevice;
  RpSbdf.Func = RpFunction;
  RpSbdf.PcieCap = PcieBaseFindCapId (RpBase, EFI_PCI_CAPABILITY_ID_PCIEXP);

  DEBUG ((DEBUG_INFO, "RootportDownstreamConfiguration %x:%x\n", RpDevice, RpFunction));
  BridgeCleanupList.Count = 0;
  RecursiveBusAssignment (RpSbdf, BusMin, BusMax, &BridgeCleanupList);

  Mps = RecursiveMpsCheck (RpSbdf);
  RecursiveMpsConfiguration (RpSbdf, Mps);
  if (PciSku == EnumPchPcie) {
    RpBase = SbdfToBase (RpSbdf);
    ConfigureRpPacketSplit(RpBase, Mps);
    IoApicPresent = RecursiveIoApicCheck(RpSbdf);
  }
  if ((PciSku == EnumPchPcie) || (PciSku == EnumCpuPcie)) {
    ConfigureEoiForwarding (RpBase, IoApicPresent);
  }
  RecursiveCccConfiguration (RpSbdf, TRUE);

  if (IsPtmCapable (RpSbdf)) {
    RecursivePtmConfiguration (RpSbdf, 0, FALSE);
  }

  ClearBusFromTable (&BridgeCleanupList);
}

/**
  Checks if given PCI device is capable of Latency Tolerance Reporting

  @param[in] Sbdf            device's segment:bus:device:function coordinates

  @retval TRUE if yes
**/
BOOLEAN
IsLtrCapable (
  SBDF Sbdf
  )
{
  if (Sbdf.PcieCap == 0) {
    return FALSE;
  }
  return !!(PciSegmentRead32 (SbdfToBase (Sbdf) + Sbdf.PcieCap + R_PCIE_DCAP2_OFFSET) & B_PCIE_DCAP2_LTRMS);
}

/**
  Returns combination of two LTR override values
  The resulting LTR override separately chooses stricter limits for snoop and nosnoop

  @param[in] LtrA      LTR override values to be combined
  @param[in] LtrB      LTR override values to be combined

  @retval LTR override value
**/
LTR_OVERRIDE
CombineLtr (
  LTR_OVERRIDE LtrA,
  LTR_OVERRIDE LtrB
  )
{
  UINT64        DecodedLatencyA;
  UINT64        DecodedLatencyB;
  LTR_OVERRIDE  Result;
  static UINT32 ScaleEncoding [8] = {1, 32, 1024, 32768, 1048576, 33554432, 0, 0};

  ZeroMem (&Result, sizeof (LTR_OVERRIDE));
  DecodedLatencyA = ScaleEncoding[LtrA.MaxSnoopLatencyScale] * LtrA.MaxSnoopLatencyValue;
  DecodedLatencyB = ScaleEncoding[LtrB.MaxSnoopLatencyScale] * LtrB.MaxSnoopLatencyValue;
  if ((!LtrB.MaxSnoopLatencyRequirement) || ((DecodedLatencyA < DecodedLatencyB) && LtrA.MaxSnoopLatencyRequirement)) {
    Result.MaxSnoopLatencyValue       = LtrA.MaxSnoopLatencyValue;
    Result.MaxSnoopLatencyScale       = LtrA.MaxSnoopLatencyScale;
    Result.MaxSnoopLatencyRequirement = LtrA.MaxSnoopLatencyRequirement;
  } else {
    Result.MaxSnoopLatencyValue       = LtrB.MaxSnoopLatencyValue;
    Result.MaxSnoopLatencyScale       = LtrB.MaxSnoopLatencyScale;
    Result.MaxSnoopLatencyRequirement = LtrB.MaxSnoopLatencyRequirement;
  }
  DecodedLatencyA = ScaleEncoding[LtrA.MaxNoSnoopLatencyScale] * LtrA.MaxNoSnoopLatencyValue;
  DecodedLatencyB = ScaleEncoding[LtrB.MaxNoSnoopLatencyScale] * LtrB.MaxNoSnoopLatencyValue;
  if ((!LtrB.MaxNoSnoopLatencyRequirement) || ((DecodedLatencyA < DecodedLatencyB) && LtrA.MaxNoSnoopLatencyRequirement)) {
    Result.MaxNoSnoopLatencyValue       = LtrA.MaxNoSnoopLatencyValue;
    Result.MaxNoSnoopLatencyScale       = LtrA.MaxNoSnoopLatencyScale;
    Result.MaxNoSnoopLatencyRequirement = LtrA.MaxNoSnoopLatencyRequirement;
  } else {
    Result.MaxNoSnoopLatencyValue       = LtrB.MaxNoSnoopLatencyValue;
    Result.MaxNoSnoopLatencyScale       = LtrB.MaxNoSnoopLatencyScale;
    Result.MaxNoSnoopLatencyRequirement = LtrB.MaxNoSnoopLatencyRequirement;
  }
  if (LtrA.ForceOverride || LtrB.ForceOverride) {
    Result.ForceOverride = TRUE;
  }
  DEBUG ((DEBUG_INFO, "CombineLtr: A(V%d S%d E%d : V%d S%d E%d, F%d)\n",
    LtrA.MaxSnoopLatencyValue, LtrA.MaxSnoopLatencyScale, LtrA.MaxSnoopLatencyRequirement,
    LtrA.MaxNoSnoopLatencyValue, LtrA.MaxNoSnoopLatencyScale, LtrA.MaxNoSnoopLatencyRequirement,
    LtrA.ForceOverride
    ));
  DEBUG ((DEBUG_INFO, "          : B(V%d S%d E%d : V%d S%d E%d, F%d)\n",
    LtrB.MaxSnoopLatencyValue, LtrB.MaxSnoopLatencyScale, LtrB.MaxSnoopLatencyRequirement,
    LtrB.MaxNoSnoopLatencyValue, LtrB.MaxNoSnoopLatencyScale, LtrB.MaxNoSnoopLatencyRequirement,
    LtrB.ForceOverride
    ));
  DEBUG ((DEBUG_INFO, "          : R(V%d S%d E%d : V%d S%d E%d, F%d)\n",
    Result.MaxSnoopLatencyValue, Result.MaxSnoopLatencyScale, Result.MaxSnoopLatencyRequirement,
    Result.MaxNoSnoopLatencyValue, Result.MaxNoSnoopLatencyScale, Result.MaxNoSnoopLatencyRequirement,
    Result.ForceOverride
    ));
  return Result;
}

/**
  Returns LTR override value for given device
  The value is extracted from Device Override table. If the device is not found,
  the returned value will have Requirement bits clear

  @param[in] Base            device's base address
  @param[in] Override        device override table

  @retval LTR override value
**/
LTR_OVERRIDE
GetOverrideLtr (
  UINT64         Base,
  OVERRIDE_TABLE *Override
  )
{
  UINT16       DevId;
  UINT16       VenId;
  UINT16       RevId;
  UINT32       Index;
  LTR_OVERRIDE ReturnValue = {0};

  VenId = PciSegmentRead16 (Base + PCI_VENDOR_ID_OFFSET);
  DevId = PciSegmentRead16 (Base + PCI_DEVICE_ID_OFFSET);
  RevId = PciSegmentRead16 (Base + PCI_REVISION_ID_OFFSET);

  for (Index = 0; Index < Override->Size; Index++) {
    if (((Override->Table[Index].OverrideConfig & PchPcieLtrOverride) == PchPcieLtrOverride) &&
        (Override->Table[Index].VendorId == VenId) &&
        ((Override->Table[Index].DeviceId == DevId) || (Override->Table[Index].DeviceId == 0xFFFF)) &&
        ((Override->Table[Index].RevId == RevId) || (Override->Table[Index].RevId == 0xFF))) {
      if (Override->Table[Index].SnoopLatency & 0x8000) {
        ReturnValue.MaxSnoopLatencyRequirement = 1;
        ReturnValue.MaxSnoopLatencyValue = Override->Table[Index].SnoopLatency & 0x3FF;
        ReturnValue.MaxSnoopLatencyScale = (Override->Table[Index].SnoopLatency & 0x1C00) >> 10;
      }
      if (Override->Table[Index].NonSnoopLatency & 0x8000) {
        ReturnValue.MaxNoSnoopLatencyRequirement = 1;
        ReturnValue.MaxNoSnoopLatencyValue = Override->Table[Index].NonSnoopLatency & 0x3FF;
        ReturnValue.MaxNoSnoopLatencyScale = (Override->Table[Index].NonSnoopLatency & 0x1C00) >> 10;
      }
      ReturnValue.ForceOverride = Override->Table[Index].ForceLtrOverride;
      break;
    }
  }
  return ReturnValue;
}

/**
  In accordance with PCIe spec, devices with no LTR support are considered to have no LTR requirements
  which means infinite latency tolerance. This was found to cause problems with HID and Audio devices without LTR
  support placed behind PCIe switches with LTR support, as Switch's upstream link would be allowed to enter L1.2
  and cause large latency downstream. To work around such issues and to fix some devices with broken
  LTR reporting, Device Override table was introduced.
  This function scans PCIe tree for devices mentioned in override table and calculates the strictest
  LTR requirement between them. That value will be programmed into rootport's LTR override register

  This function expects that bridges have bus numbers already configured

  @param[in] BusLimit                       maximum Bus number that can be assigned below this port
  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device
  @param[in] AspmOverride                   Device specific ASPM policy override items

  @retval MaxLTR programmed in this device
**/
LTR_OVERRIDE
RecursiveLtrOverrideCheck (
  SBDF           Sbdf,
  OVERRIDE_TABLE *AspmOverride
  )
{
  UINT64       Base;
  SBDF         ChildSbdf;
  LTR_OVERRIDE MyLtrOverride;
  LTR_OVERRIDE ChildLtr;
  PCI_DEV_TYPE DevType;

  DEBUG ((DEBUG_INFO, "RecursiveLtrOverrideCheck %x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  Base = SbdfToBase(Sbdf);

  MyLtrOverride = GetOverrideLtr (Base, AspmOverride);
  if (HasChildBus (Sbdf, &ChildSbdf)) {
    DevType = GetDeviceType (Sbdf);
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      ChildLtr = RecursiveLtrOverrideCheck (ChildSbdf, AspmOverride);
      MyLtrOverride = CombineLtr (MyLtrOverride, ChildLtr);
    }
  }
  return MyLtrOverride;
}

/**
  Configures the following power-management related features in rootport and devices behind it:
  LTR limit (generic)
  LTR override (proprietary)
  Clock Power Management (generic)
  L1 substates (generic except for the override table)
  L1.LOW substate (proprietary)
  L0s and L1 (generic)

  Generic: any code written according to PCIE Express base specification can do that.
  Proprietary: code uses registers and features that are specific to Intel silicon
  and probably only this Reference Code knows how to handle that.

  If OEM implemented generic feature enabling in his platform code or trusts Operating System
  to do it, then those features can be deleted from here.

  @param[in] RpSegment                address of rootport on PCIe
  @param[in] RpBus                    address of rootport on PCIe
  @param[in] RpDevice                 address of rootport on PCIe
  @param[in] RpFunction               address of rootport on PCIe
  @param[in] BusLimit                 maximum Bus number that can be assigned below this rootport
  @param[in] PcieRpLtrConfig          address of LTR Policy struct
  @param[in] PcieRpCommonConfig       address of Common PCIe Policy struct
  @param[in] AspmOverrideTableSize    size of override array
  @param[in] AspmOverrideTable        array of device that need exceptions in configuration
**/
VOID
RootportDownstreamPmConfiguration (
  UINT8                               RpSegment,
  UINT8                               RpBus,
  UINT8                               RpDevice,
  UINT8                               RpFunction,
  UINT8                               BusMin,
  UINT8                               BusMax,
  PCIE_ROOT_PORT_COMMON_CONFIG        *PcieRpCommonConfig,
  UINT32                              AspmOverrideTableSize,
  PCH_PCIE_DEVICE_OVERRIDE            *AspmOverrideTable
  )
{
  LTR_LIMIT      PolicyLtr;
  LTR_OVERRIDE   TreeLtr;
  OVERRIDE_TABLE PmOverrideTable;
  UINT64         RpBase;
  SBDF           RpSbdf;
  SBDF_TABLE     BridgeCleanupList;

  RpBase = PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, 0);
  if (!(IsDevicePresent (RpBase))) {
    return;
  }
  PmOverrideTable.Size = AspmOverrideTableSize;
  PmOverrideTable.Table = AspmOverrideTable;

  DEBUG ((DEBUG_INFO, "RootportDownstreamPmConfiguration %x:%x\n", RpDevice, RpFunction));
  PolicyLtr.MaxNoSnoopLatencyScale = (PcieRpCommonConfig->PcieRpLtrConfig.LtrMaxNoSnoopLatency & 0x1c00) >> 10;
  PolicyLtr.MaxNoSnoopLatencyValue = PcieRpCommonConfig->PcieRpLtrConfig.LtrMaxNoSnoopLatency & 0x3FF;
  PolicyLtr.MaxSnoopLatencyScale   = (PcieRpCommonConfig->PcieRpLtrConfig.LtrMaxSnoopLatency & 0x1c00) >> 10;
  PolicyLtr.MaxSnoopLatencyValue   = PcieRpCommonConfig->PcieRpLtrConfig.LtrMaxSnoopLatency & 0x3FF;

  RpSbdf.Seg = RpSegment;
  RpSbdf.Bus = RpBus;
  RpSbdf.Dev = RpDevice;
  RpSbdf.Func = RpFunction;
  RpSbdf.PcieCap = PcieBaseFindCapId (RpBase, EFI_PCI_CAPABILITY_ID_PCIEXP);
  //
  // This code could execute either before or after enumeration. If before, then buses would not yet be assigned to bridges,
  // making devices deeper in the hierarchy inaccessible.
  // RecursiveBusAssignment will scan whole PCie tree and assign bus numbers to uninitialized bridges, if there are any
  // List of such bridges will be kept in CleanupList, so that after PM programming is done, bus numbers can brought to original state
  //
  BridgeCleanupList.Count = 0;
  RecursiveBusAssignment(RpSbdf, BusMin, BusMax, &BridgeCleanupList);
  //
  // The 'Recursive...' functions below expect bus numbers to be already assigned
  //
  RecursiveLtrConfiguration (RpSbdf, PolicyLtr);
  TreeLtr = RecursiveLtrOverrideCheck (RpSbdf, &PmOverrideTable);
  ConfigureRpLtrOverride (RpBase, RpSbdf.Dev, &TreeLtr, &(PcieRpCommonConfig->PcieRpLtrConfig));
  DEBUG ((DEBUG_INFO, "ConfigureRpLtrOverride %x:%x\n", RpSbdf.Dev, RpSbdf.Func));
  if (PcieRpCommonConfig->EnableCpm) {
    RecursiveCpmConfiguration (RpSbdf);
  }
  //
  // L1 substates can be modified only when L1 is disabled, so this function must execute
  // before Aspm configuration which enables L1
  //
  RecursiveL1ssConfiguration (RpSbdf, &PmOverrideTable);
  L1ssProprietaryConfiguration (RpBase, IsLtrCapable (RpSbdf));
  RecursiveAspmConfiguration (RpSbdf, 0, &PmOverrideTable);

  ClearBusFromTable (&BridgeCleanupList);
}
