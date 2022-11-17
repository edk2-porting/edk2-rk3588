/** @file
  Pch information library.

  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/MmPciLib.h>
#include <Library/PchInfoLib.h>
#include <PchAccess.h>

#define PCH_DO_STRINGIFY(x) #x
#define PCH_STRINGIFY(x) PCH_DO_STRINGIFY(x)

//
// This module variables are used for cache the static result.
// @note: please pay attention to the PEI phase, the module variables on ROM
//       and can't be modified.
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_STEPPING     mPchStepping      = PchSteppingMax;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8            mIsPchSupported   = 0xFF;
GLOBAL_REMOVE_IF_UNREFERENCED PCH_SERIES       mPchSeries        = PchUnknownSeries;
GLOBAL_REMOVE_IF_UNREFERENCED PCH_GENERATION   mPchGeneration    = PchUnknownGeneration;

/**
  Return Pch stepping type

  @retval PCH_STEPPING            Pch stepping type
**/
PCH_STEPPING
EFIAPI
PchStepping (
  VOID
  )
{
  UINT8         RevId;
  UINT16        LpcDeviceId;
  UINTN         LpcBaseAddress;
#ifndef MDEPKG_NDEBUG
  static CHAR8  *UnsupportedPchMsg = "Unsupported PCH. Supported stepping starting from";
#endif

  if (mPchStepping != PchSteppingMax) {
    return mPchStepping;
  }

  LpcBaseAddress = MmPciBase (
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_LPC,
                     PCI_FUNCTION_NUMBER_PCH_LPC
                     );
  RevId = MmioRead8 (LpcBaseAddress + PCI_REVISION_ID_OFFSET);

  LpcDeviceId = MmioRead16 (LpcBaseAddress + PCI_DEVICE_ID_OFFSET);

  if (IS_SKL_PCH_H_LPC_DEVICE_ID (LpcDeviceId)) {
    switch (RevId) {
      case V_PCH_LPC_RID_10:
        mPchStepping = PchHB0;
        return PchHB0;

      case V_PCH_LPC_RID_20:
        mPchStepping = PchHC0;
        return PchHC0;

      case V_PCH_LPC_RID_30:
        mPchStepping = PchHD0;
        return PchHD0;

      case V_PCH_LPC_RID_31:
        mPchStepping = PchHD1;
        return PchHD1;

      default:
        DEBUG ((DEBUG_ERROR, "%a %a\n", UnsupportedPchMsg, PCH_STRINGIFY (SKL_PCH_H_MIN_SUPPORTED_STEPPING)));
        return PchSteppingMax;
    }
  }

  if (IS_KBL_PCH_H_LPC_DEVICE_ID (LpcDeviceId)) {
    switch (RevId) {
      case V_PCH_LPC_RID_0:
        mPchStepping = KblPchHA0;
        return KblPchHA0;

      default:
        DEBUG ((DEBUG_ERROR, "%a %a\n", UnsupportedPchMsg, PCH_STRINGIFY (KBL_PCH_H_MIN_SUPPORTED_STEPPING)));
        return PchSteppingMax;
    }
  }

  if (IS_PCH_LP_LPC_DEVICE_ID (LpcDeviceId)) {
    switch (RevId) {
      case V_PCH_LPC_RID_10:
        mPchStepping = PchLpB0;
        return PchLpB0;

      case V_PCH_LPC_RID_11:
        mPchStepping = PchLpB1;
        return PchLpB1;

      case V_PCH_LPC_RID_20:
        mPchStepping = PchLpC0;
        return PchLpC0;

      case V_PCH_LPC_RID_21:
        mPchStepping = PchLpC1;
        return PchLpC1;

      default:
        DEBUG ((DEBUG_ERROR, "%a %a\n", UnsupportedPchMsg, PCH_STRINGIFY (PCH_LP_MIN_SUPPORTED_STEPPING)));
        return PchSteppingMax;
    }
  }
  return PchSteppingMax;
}

/**
  Determine if PCH is supported

  @retval TRUE                    PCH is supported
  @retval FALSE                   PCH is not supported
**/
BOOLEAN
IsPchSupported (
  VOID
  )
{
  UINT16  LpcDeviceId;
  UINT16  LpcVendorId;
  UINTN   LpcBaseAddress;

  if (mIsPchSupported != 0xFF) {
    return (BOOLEAN) mIsPchSupported;
  }

  LpcBaseAddress = MmPciBase (
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_LPC,
                     PCI_FUNCTION_NUMBER_PCH_LPC
                     );

  LpcDeviceId = MmioRead16 (LpcBaseAddress + PCI_DEVICE_ID_OFFSET);
  LpcVendorId = MmioRead16 (LpcBaseAddress + PCI_VENDOR_ID_OFFSET);

  ///
  /// Verify that this is a supported chipset
  ///
  if ((LpcVendorId == V_PCH_LPC_VENDOR_ID) &&
      (IS_PCH_LPC_DEVICE_ID (LpcDeviceId))) {
    mIsPchSupported = TRUE;
    return TRUE;
  } else {
    DEBUG ((DEBUG_ERROR, "PCH code doesn't support the LpcDeviceId: 0x%04x!\n", LpcDeviceId));
    mIsPchSupported = FALSE;
    return FALSE;
  }
}

/**
  Return Pch Series

  @retval PCH_SERIES            Pch Series
**/
PCH_SERIES
EFIAPI
GetPchSeries (
  VOID
  )
{
  UINT16  LpcDeviceId;
  UINT32  PchSeries;

  if (mPchSeries != PchUnknownSeries) {
    return mPchSeries;
  }

  LpcDeviceId = GetLpcDid ();

  if (IS_PCH_H_LPC_DEVICE_ID (LpcDeviceId)) {
    PchSeries = PchH;
  } else if (IS_PCH_LP_LPC_DEVICE_ID (LpcDeviceId)) {
    PchSeries = PchLp;
  } else {
    PchSeries = PchUnknownSeries;
    DEBUG ((DEBUG_ERROR, "Unsupported PCH SKU, LpcDeviceId: 0x%04x!\n", LpcDeviceId));
    ASSERT (FALSE);
  }
  mPchSeries = PchSeries;

  return PchSeries;
}

/**
  Return Pch Generation

  @retval PCH_GENERATION            Pch Generation
**/
PCH_GENERATION
EFIAPI
GetPchGeneration (
  VOID
  )
{
  UINT16          LpcDeviceId;
  PCH_GENERATION  PchGen;

  if (mPchGeneration != PchUnknownGeneration) {
    return mPchGeneration;
  }

  LpcDeviceId = GetLpcDid ();

  if (IS_SKL_PCH_LPC_DEVICE_ID (LpcDeviceId)) {
    PchGen = SklPch;
  } else if (IS_KBL_PCH_LPC_DEVICE_ID (LpcDeviceId)) {
    PchGen = KblPch;
  } else {
    PchGen = PchUnknownGeneration;
    DEBUG ((DEBUG_ERROR, "Unsupported PCH SKU, LpcDeviceId: 0x%04x!\n", LpcDeviceId));
    ASSERT (FALSE);
  }
  mPchGeneration = PchGen;

  return PchGen;
}

/**
  Get PCH SKU type

  @retval PCH_SKU_TYPE  Type of PCH SKU
**/
PCH_SKU_TYPE
GetPchSkuType (
  VOID
  )
{
  UINT16  LpcDeviceId;

  LpcDeviceId = GetLpcDid ();

  if (IS_PCH_LPC_DEVICE_ID_MOBILE (LpcDeviceId)) {
    return PchMobileSku;
  } else if (IS_PCH_LPC_DEVICE_ID_DESKTOP (LpcDeviceId)) {
    return PchDesktopSku;
  } else if (IS_PCH_LPC_DEVICE_ID_SERVER (LpcDeviceId)) {
    return PchServerSku;
  } else {
    return PchUnknownSku;
  }
}

/**
  Get Pch Maximum Pcie Root Port Number

  @retval Pch Maximum Pcie Root Port Number
**/
UINT8
EFIAPI
GetPchMaxPciePortNum (
  VOID
  )
{
  PCH_SERIES  PchSeries;

  PchSeries = GetPchSeries ();
  switch (PchSeries) {
    case PchLp:
      return PCH_LP_PCIE_MAX_ROOT_PORTS;

    case PchH:
      switch (GetPchGeneration ()) {
        case SklPch:
          return SKL_PCH_H_PCIE_MAX_ROOT_PORTS;
        case KblPch:
          return KBL_PCH_H_PCIE_MAX_ROOT_PORTS;
        default:
          return 0;
      }
    default:
      return 0;
  }
}

/**
  Get Pch Maximum Sata Port Number

  @retval Pch Maximum Sata Port Number
**/
UINT8
EFIAPI
GetPchMaxSataPortNum (
  VOID
  )
{
  PCH_SERIES  PchSeries;

  PchSeries = GetPchSeries ();
  switch (PchSeries) {
    case PchLp:
      return PCH_LP_AHCI_MAX_PORTS;

    case PchH:
      return PCH_H_AHCI_MAX_PORTS;

    default:
      return 0;
  }
}

/**
  Get Pch Usb Maximum Physical Port Number

  @retval Pch Usb Maximum Physical Port Number
**/
UINT8
EFIAPI
GetPchUsbMaxPhysicalPortNum (
  VOID
  )
{
  PCH_SERIES  PchSeries;

  PchSeries = GetPchSeries ();
  switch (PchSeries) {
    case PchLp:
      return PCH_LP_XHCI_MAX_USB2_PHYSICAL_PORTS;

    case PchH:
      return PCH_H_XHCI_MAX_USB2_PHYSICAL_PORTS;

    default:
      return 0;
  }
}

/**
  Get Pch Maximum Usb2 Port Number of XHCI Controller

  @retval Pch Maximum Usb2 Port Number of XHCI Controller
**/
UINT8
EFIAPI
GetPchXhciMaxUsb2PortNum (
  VOID
  )
{
  PCH_SERIES  PchSeries;

  PchSeries = GetPchSeries ();
  switch (PchSeries) {
    case PchLp:
      return PCH_LP_XHCI_MAX_USB2_PORTS;

    case PchH:
      return PCH_H_XHCI_MAX_USB2_PORTS;

    default:
      return 0;
  }
}

/**
  Get Pch Maximum Usb3 Port Number of XHCI Controller

  @retval Pch Maximum Usb3 Port Number of XHCI Controller
**/
UINT8
EFIAPI
GetPchXhciMaxUsb3PortNum (
  VOID
  )
{
  PCH_SERIES  PchSeries;

  PchSeries = GetPchSeries ();
  switch (PchSeries) {
    case PchLp:
      return PCH_LP_XHCI_MAX_USB3_PORTS;

    case PchH:
      return PCH_H_XHCI_MAX_USB3_PORTS;

    default:
      return 0;
  }
}

/**
  Get Lpc Did

  @retval  UINT16   Lpc Did
**/
UINT16
EFIAPI
GetLpcDid (
  VOID
  )
{
  UINT16  LpcDeviceId;
  UINTN   LpcBaseAddress;

  LpcBaseAddress = MmPciBase (
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_LPC,
                     PCI_FUNCTION_NUMBER_PCH_LPC
                     );

  LpcDeviceId = MmioRead16 (LpcBaseAddress + PCI_DEVICE_ID_OFFSET);

  return LpcDeviceId;
}

/**
  Get RST mode supported by the silicon

  @retval RST_MODE               RST mode supported by silicon
**/
RST_MODE
EFIAPI
GetSupportedRstMode (
  VOID
  )
{
  if (IsPchRaidSupported ()) {
    return RstPremium;
  } else if (IsOptaneModeSupported ()) {
    return RstOptane;
  } else {
    return RstUnsupported;
  }
}

/**
  Check if current SKU supports Optane mode

  @retval TRUE                   This SKU supports Optane mode
  @retval FALSE                  This SKU doesn't support Optane mode
**/
BOOLEAN
EFIAPI
IsOptaneModeSupported (
  VOID
  )
{
  UINT16   LpcDeviceId;
  BOOLEAN  OptaneSupported;

  LpcDeviceId = GetLpcDid ();

  switch (LpcDeviceId) {
    case V_KBL_PCH_H_LPC_DEVICE_ID_DT_B250:
    case V_KBL_PCH_H_LPC_DEVICE_ID_DT_Q250:
      OptaneSupported = TRUE;
      break;
    default:
      OptaneSupported = FALSE;
      break;
  }

  return OptaneSupported;
}

/**
  Check if current SKU supports RAID feature

  @retval TRUE                   This SKU supports RAID
  @retval FALSE                  This SKU doesn't support RAID
**/
BOOLEAN
EFIAPI
IsPchRaidSupported (
  VOID
  )
{
  UINT16          LpcDeviceId;
  BOOLEAN         RaidSupported;

  LpcDeviceId = GetLpcDid ();


  switch (LpcDeviceId) {
    case V_PCH_LP_LPC_DEVICE_ID_MB_1:       // SPT-LP Base-U (SPT-LP Mobile)
    case V_PCH_LP_LPC_DEVICE_ID_MB_5:       // KBL-LP Base-U (KBL-LP Mobile)
    case V_PCH_LP_LPC_DEVICE_ID_MB_10:      // KBL-LP Mobile (U) iHDCP 2.2 Base
    case V_SKL_PCH_H_LPC_DEVICE_ID_DT_1:    // H110 (SPT-H Desktop)
    case V_SKL_PCH_H_LPC_DEVICE_ID_DT_6:    // B150 (SPT-H Desktop)
    case V_SKL_PCH_H_LPC_DEVICE_ID_DT_5:    // Q150 (SPT-H Desktop)
    case V_KBL_PCH_H_LPC_DEVICE_ID_DT_B250: // B250 (KBL-H Desktop)
    case V_KBL_PCH_H_LPC_DEVICE_ID_DT_Q250: // Q250 (KBL-H Desktop)
    case V_SKL_PCH_H_LPC_DEVICE_ID_SVR_1:   // C232 (SPT-H Server)
      RaidSupported = FALSE;
      break;
    default:
      RaidSupported = TRUE;
      break;
  }

  return RaidSupported;
}
