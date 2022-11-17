/** @file
  This is part of the implementation of an Intel Graphics drivers OpRegion /
  Software SCI interface between system BIOS, ASL code, and Graphics drivers.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Specification Reference:
  - OpRegion / Software SCI SPEC
**/
#ifndef _IGD_OPREGION_H_
#define _IGD_OPREGION_H_

///
/// Statements that include other header files.
///
#include <Uefi/UefiBaseType.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciLib.h>
#include <Library/IoLib.h>
#include <Protocol/PciIo.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/LegacyBios.h>
#include <Library/MmPciLib.h>
#include <PchAccess.h>
#include <Library/PchCycleDecodingLib.h>
#include <SaAccess.h>
#include <IndustryStandard/Pci22.h>

///
/// Driver Consumed Protocol Prototypes
///
#include <Protocol/SaPolicy.h>


#include <Protocol/SaGlobalNvsArea.h>

///
/// Driver Produced Protocol Prototypes
///
#include <Protocol/IgdOpRegion.h>

///
///
/// OpRegion (Miscellaneous) defines.
///
/// OpRegion Header defines.
///
typedef UINT16  STRING_REF;
#define HEADER_SIGNATURE            "IntelGraphicsMem"
#define HEADER_SIZE                 0x2000
#define HEADER_OPREGION_VER         0x0200
#define HEADER_OPREGION_REV         0x00
#define HEADER_MBOX_SUPPORT         (HD_MBOX5 + HD_MBOX4 + HD_MBOX3 + HD_MBOX2 + HD_MBOX1)
#define HD_MBOX1                    BIT0
#define HD_MBOX2                    BIT1
#define HD_MBOX3                    BIT2
#define HD_MBOX4                    BIT3
#define HD_MBOX5                    BIT4
#define SVER_SIZE                   32

///
/// OpRegion Mailbox 1 EQUates.
///
#define LID_OPEN              3

///
/// OpRegion Mailbox 3 EQUates.
///
#define ALS_ENABLE            BIT0
#define BLC_ENABLE            BIT1
#define BACKLIGHT_BRIGHTNESS  0xFF
#define FIELD_VALID_BIT       BIT31
#define WORD_FIELD_VALID_BIT  BIT15
#define PFIT_ENABLE           BIT2
#define PFIT_OPRN_AUTO        0x00000000
#define PFIT_OPRN_SCALING     0x00000007
#define PFIT_OPRN_OFF         0x00000000
#define PFIT_SETUP_AUTO       0
#define PFIT_SETUP_SCALING    1
#define PFIT_SETUP_OFF        2
#define INIT_BRIGHT_LEVEL     0x64
#define PFIT_STRETCH          6

///
/// Video BIOS / VBT defines
///
#define OPTION_ROM_SIGNATURE    0xAA55
#define VBIOS_LOCATION_PRIMARY  0xC0000

#define VBT_SIGNATURE           SIGNATURE_32 ('$', 'V', 'B', 'T')
///
/// Typedef stuctures
///
#pragma pack(1)
typedef struct {
  UINT16  Signature;  /// 0xAA55
  UINT8   Size512;
  UINT8   Reserved[21];
  UINT16  PcirOffset;
  UINT16  VbtOffset;
} INTEL_VBIOS_OPTION_ROM_HEADER;
#pragma pack()

#pragma pack(1)
typedef struct {
  UINT32  Signature;  /// "PCIR"
  UINT16  VendorId;   /// 0x8086
  UINT16  DeviceId;
  UINT16  Reserved0;
  UINT16  Length;
  UINT8   Revision;
  UINT8   ClassCode[3];
  UINT16  ImageLength;
  UINT16  CodeRevision;
  UINT8   CodeType;
  UINT8   Indicator;
  UINT16  Reserved1;
} INTEL_VBIOS_PCIR_STRUCTURE;
#pragma pack()

#pragma pack(1)
typedef struct {
  UINT8   HeaderSignature[20];
  UINT16  HeaderVersion;
  UINT16  HeaderSize;
  UINT16  HeaderVbtSize;
  UINT8   HeaderVbtCheckSum;
  UINT8   HeaderReserved;
  UINT32  HeaderOffsetVbtDataBlock;
  UINT32  HeaderOffsetAim1;
  UINT32  HeaderOffsetAim2;
  UINT32  HeaderOffsetAim3;
  UINT32  HeaderOffsetAim4;
  UINT8   DataHeaderSignature[16];
  UINT16  DataHeaderVersion;
  UINT16  DataHeaderSize;
  UINT16  DataHeaderDataBlockSize;
  UINT8   CoreBlockId;
  UINT16  CoreBlockSize;
  UINT16  CoreBlockBiosSize;
  UINT8   CoreBlockBiosType;
  UINT8   CoreBlockReleaseStatus;
  UINT8   CoreBlockHWSupported;
  UINT8   CoreBlockIntegratedHW;
  UINT8   CoreBlockBiosBuild[4];
  UINT8   CoreBlockBiosSignOn[155];
} VBIOS_VBT_STRUCTURE;
#pragma pack()
///
/// Driver Private Function definitions
///

/**
  Graphics OpRegion / Software SCI driver installation function.

  @retval EFI_SUCCESS     - The driver installed without error.
  @retval EFI_ABORTED     - The driver encountered an error and could not complete
                            installation of the ACPI tables.
**/
EFI_STATUS
IgdOpRegionInit (
  VOID
  );

/**
  Get Intel video BIOS VBT information (i.e. Pointer to VBT and VBT size).
  The VBT (Video BIOS Table) is a block of customizable data that is built
  within the video BIOS and edited by customers.

  @retval EFI_SUCCESS            - Video BIOS VBT information returned.
  @exception EFI_UNSUPPORTED     - Could not find VBT information (*VBiosVbtPtr = NULL).
**/
EFI_STATUS
GetVBiosVbtEndOfDxe (
  VOID
  );

#endif
