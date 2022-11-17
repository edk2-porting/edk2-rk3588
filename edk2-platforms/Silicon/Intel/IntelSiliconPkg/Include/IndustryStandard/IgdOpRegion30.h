/** @file
  IGD OpRegion definition from Intel Integrated Graphics Device OpRegion
  Specification based on version 3.0.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _IGD_OPREGION_3_0_H_
#define _IGD_OPREGION_3_0_H_

#include "IgdOpRegion.h"

#define IGD_OPREGION_HEADER_MBOX2_VER_3_0 BIT5

#pragma pack(1)
///
/// OpRegion Mailbox 1 - Public ACPI Methods
/// Offset 0x100, Size 0x100
///
typedef struct {
  UINT32 DRDY;          ///< Offset 0x100 Driver Readiness
  UINT32 CSTS;          ///< Offset 0x104 Status
  UINT32 CEVT;          ///< Offset 0x108 Current Event
  UINT8  RM11[0x14];    ///< Offset 0x10C Reserved Must be Zero
  UINT32 DIDL[8];       ///< Offset 0x120 Supported Display Devices ID List
  UINT32 CPDL[8];       ///< Offset 0x140 obsolete
  UINT32 CADL[8];       ///< Offset 0x160 obsolete
  UINT32 NADL[8];       ///< Offset 0x180 obsolete
  UINT32 ASLP;          ///< Offset 0x1A0 ASL Sleep Time Out
  UINT32 TIDX;          ///< Offset 0x1A4 obsolete
  UINT32 CHPD;          ///< Offset 0x1A8 obsolete
  UINT32 CLID;          ///< Offset 0x1AC Current Lid State Indicator
  UINT32 CDCK;          ///< Offset 0x1B0 Current Docking State Indicator
  UINT32 SXSW;          ///< Offset 0x1B4 obsolete
  UINT32 EVTS;          ///< Offset 0x1B8 obsolete
  UINT32 CNOT;          ///< Offset 0x1BC obsolete
  UINT32 NRDY;          ///< Offset 0x1C0 Driver Status
  UINT8  DID2[0x1C];    ///< Offset 0x1C4 Extended Supported Devices ID List (DOD)
  UINT8  CPD2[0x1C];    ///< Offset 0x1E0 obsolete
  UINT8  RM12[4];       ///< Offset 0x1FC - 0x1FF Reserved Must be zero
} IGD_OPREGION_MBOX1_VER_3_0;

///
/// OpRegion Mailbox 2 - Backlight communication
/// Offset 0x200, Size 0x100
///
typedef struct {
  UINT32 BCL1;          ///< Offset 0x200 Backlight Brightness for LFP1
  UINT32 BCL2;          ///< Offset 0x204 Backlight Brightness for LFP2
  UINT32 CBL1;          ///< Offset 0x208 Current User Brightness Level for LFP1
  UINT32 CBL2;          ///< Offset 0x20C Current User Brightness Level for LFP2
  UINT32 BCM1[0x1E];    ///< Offset 0x210 Backlight Brightness Levels Duty Cycle Mapping Table for LFP1
  UINT32 BCM2[0x1E];    ///< Offset 0x288 Backlight Brightness Levels Duty Cycle Mapping Table for LFP2
} IGD_OPREGION_MBOX2_VER_3_0;

///
/// OpRegion Mailbox 3 - BIOS/Driver Notification - ASLE Support
/// Offset 0x300, Size 0x100
///
typedef struct {
  UINT32 ARDY;          ///< Offset 0x300 obsolete
  UINT32 ASLC;          ///< Offset 0x304 obsolete
  UINT32 TCHE;          ///< Offset 0x308 obsolete
  UINT32 ALSI;          ///< Offset 0x30C obsolete
  UINT32 BCLP;          ///< Offset 0x310 obsoleted in ver 3.0, moved to Mailbox 2.
  UINT32 PFIT;          ///< Offset 0x314 obsolete
  UINT32 CBLV;          ///< Offset 0x318 obsoleted in ver 3.0, moved to Mailbox 2.
  UINT16 BCLM[0x14];    ///< Offset 0x31C obsoleted in ver 3.0, moved to Mailbox 2.
  UINT32 CPFM;          ///< Offset 0x344 obsolete
  UINT32 EPFM;          ///< Offset 0x348 obsolete
  UINT8  PLUT[0x4A];    ///< Offset 0x34C obsolete
  UINT32 PFMB;          ///< Offset 0x396 obsolete
  UINT32 CCDV;          ///< Offset 0x39A obsolete
  UINT32 PCFT;          ///< Offset 0x39E obsolete
  UINT32 SROT;          ///< Offset 0x3A2 obsolete
  UINT32 IUER;          ///< Offset 0x3A6 obsolete
  UINT64 FDSS;          ///< Offset 0x3AA obsolete
  UINT32 FDSP;          ///< Offset 0x3B2 obsolete
  UINT32 STAT;          ///< Offset 0x3B6 obsolete
  UINT64 RVDA;          ///< Offset 0x3BA Physical address of Raw VBT data. Added from Spec Version 0.90 to support VBT greater than 6KB.
  UINT32 RVDS;          ///< Offset 0x3C2 Size of Raw VBT data. Added from Spec Version 0.90 to support VBT greater than 6KB.
  UINT8  VRSR;          ///< Offset 0x3C6 Video RAM Self Refresh
  UINT64 DLHP;          ///< Offset 0x3C7 Dual LFP Hinge Alignment Parameters
  UINT8  RM32[0x31];    ///< Offset 0x3CF - 0x3FF  Reserved Must be zero.
} IGD_OPREGION_MBOX3_VER_3_0;

///
/// IGD OpRegion Structure
///
typedef struct {
  IGD_OPREGION_HEADER         Header; ///< OpRegion header (Offset 0x0, Size 0x100)
  IGD_OPREGION_MBOX1_VER_3_0  MBox1;  ///< Mailbox 1: Public ACPI Methods (Offset 0x100, Size 0x100)
  IGD_OPREGION_MBOX2_VER_3_0  MBox2;  ///< Mailbox 2: Backlight communication (Offset 0x200, Size 0x100)
  IGD_OPREGION_MBOX3_VER_3_0  MBox3;  ///< Mailbox 3: BIOS to Driver Notification (Offset 0x300, Size 0x100)
  IGD_OPREGION_MBOX4          MBox4;  ///< Mailbox 4: Video BIOS Table (VBT) (Offset 0x400, Size 0x1800)
  IGD_OPREGION_MBOX5          MBox5;  ///< Mailbox 5: BIOS to Driver Notification Extension (Offset 0x1C00, Size 0x400)
} IGD_OPREGION_STRUCTURE_VER_3_0;
#pragma pack()

#endif
