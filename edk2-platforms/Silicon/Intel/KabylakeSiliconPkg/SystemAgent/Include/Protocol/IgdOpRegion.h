/** @file
  This file is part of the IGD OpRegion Implementation.  The IGD OpRegion is
  an interface between system BIOS, ASL code, and Graphics drivers.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

@par Specification Reference:
  - IGD OpRegion/Software SCI SPEC
**/
#ifndef _IGD_OPREGION_PROTOCOL_H_
#define _IGD_OPREGION_PROTOCOL_H_

extern EFI_GUID gIgdOpRegionProtocolGuid;

/**
 Protocol data definitions


 OpRegion structures:
 Sub-structures define the different parts of the OpRegion followed by the
 main structure representing the entire OpRegion.

 Note: These structures are packed to 1 byte offsets because the exact
 data location is requred by the supporting design specification due to
 the fact that the data is used by ASL and Graphics driver code compiled
 separatly.
**/
#pragma pack(1)
///
/// OpRegion header (mailbox 0) structure and defines.
///
typedef struct {
  CHAR8   SIGN[0x10]; ///< Offset 0    OpRegion signature
  UINT32  SIZE;       ///< Offset 16   OpRegion size
  UINT32  OVER;       ///< Offset 20   OpRegion structure version
  UINT8   SVER[0x20]; ///< Offset 24   System BIOS build version
  UINT8   VVER[0x10]; ///< Offset 56   Video BIOS build version
  UINT8   GVER[0x10]; ///< Offset 72   Graphic driver build version
  UINT32  MBOX;       ///< Offset 88   Mailboxes supported
  UINT32  DMOD;       ///< Offset 92   Driver Model
  UINT32  PCON;       ///< Offset 96   Platform Capabilities
  CHAR16  DVER[0x10]; ///< Offset 100  GOP Version
  UINT8   RHD1[0x7C]; ///< Offset 132  Reserved
} OPREGION_HEADER;
#pragma pack()
#pragma pack(1)
///
/// OpRegion mailbox 1 (public ACPI Methods).
///
typedef struct {
  UINT32  DRDY;       ///< Offset 0    Driver readiness
  UINT32  CSTS;       ///< Offset 4    Status
  UINT32  CEVT;       ///< Offset 8    Current event
  UINT8   RM11[0x14]; ///< Offset 12   Reserved
  UINT32  DIDL;       ///< Offset 32   Supported display device 1
  UINT32  DDL2;       ///< Offset 36   Supported display device 2
  UINT32  DDL3;       ///< Offset 40   Supported display device 3
  UINT32  DDL4;       ///< Offset 44   Supported display device 4
  UINT32  DDL5;       ///< Offset 48   Supported display device 5
  UINT32  DDL6;       ///< Offset 52   Supported display device 6
  UINT32  DDL7;       ///< Offset 56   Supported display device 7
  UINT32  DDL8;       ///< Offset 60   Supported display device 8
  UINT32  CPDL;       ///< Offset 64   Currently present display device 1
  UINT32  CPL2;       ///< Offset 68   Currently present display device 2
  UINT32  CPL3;       ///< Offset 72   Currently present display device 3
  UINT32  CPL4;       ///< Offset 76   Currently present display device 4
  UINT32  CPL5;       ///< Offset 80   Currently present display device 5
  UINT32  CPL6;       ///< Offset 84   Currently present display device 6
  UINT32  CPL7;       ///< Offset 88   Currently present display device 7
  UINT32  CPL8;       ///< Offset 92   Currently present display device 8
  UINT32  CADL;       ///< Offset 96   Currently active display device 1
  UINT32  CAL2;       ///< Offset 100  Currently active display device 2
  UINT32  CAL3;       ///< Offset 104  Currently active display device 3
  UINT32  CAL4;       ///< Offset 108  Currently active display device 4
  UINT32  CAL5;       ///< Offset 112  Currently active display device 5
  UINT32  CAL6;       ///< Offset 116  Currently active display device 6
  UINT32  CAL7;       ///< Offset 120  Currently active display device 7
  UINT32  CAL8;       ///< Offset 124  Currently active display device 8
  UINT32  NADL;       ///< Offset 128  Next active device 1
  UINT32  NDL2;       ///< Offset 132  Next active device 2
  UINT32  NDL3;       ///< Offset 136  Next active device 3
  UINT32  NDL4;       ///< Offset 140  Next active device 4
  UINT32  NDL5;       ///< Offset 144  Next active device 5
  UINT32  NDL6;       ///< Offset 148  Next active device 6
  UINT32  NDL7;       ///< Offset 152  Next active device 7
  UINT32  NDL8;       ///< Offset 156  Next active device 8
  UINT32  ASLP;       ///< Offset 160  ASL sleep timeout
  UINT32  TIDX;       ///< Offset 164  Toggle table index
  UINT32  CHPD;       ///< Offset 168  Current hot plug enable indicator
  UINT32  CLID;       ///< Offset 172  Current lid state indicator
  UINT32  CDCK;       ///< Offset 176  Current docking state indicator
  UINT32  SXSW;       ///< Offset 180  Display Switch notification on Sx State resume
  UINT32  EVTS;       ///< Offset 184  Events supported by ASL
  UINT32  CNOT;       ///< Offset 188  Current OS Notification
  UINT32  NRDY;       ///< Offset 192  Reasons for DRDY = 0
  UINT8   RM12[0x3C]; ///< Offset 196  Reserved
} OPREGION_MBOX1;
#pragma pack()
#pragma pack(1)
///
/// OpRegion mailbox 2 (Software SCI Interface).
///
typedef struct {
  UINT32  SCIC;       ///< Offset 0    Software SCI function number parameters
  UINT32  PARM;       ///< Offset 4    Software SCI additional parameters
  UINT32  DSLP;       ///< Offset 8    Driver sleep timeout
  UINT8   RM21[0xF4]; ///< Offset 12   Reserved
} OPREGION_MBOX2;
#pragma pack()
#pragma pack(1)
///
/// OpRegion mailbox 3 (Power Conservation).
///
typedef struct {
  UINT32  ARDY;       ///< Offset 0    Driver readiness
  UINT32  ASLC;       ///< Offset 4    ASLE interrupt command / status
  UINT32  TCHE;       ///< Offset 8    Technology enabled indicator
  UINT32  ALSI;       ///< Offset 12   Current ALS illuminance reading
  UINT32  BCLP;       ///< Offset 16   Backlight britness to set
  UINT32  PFIT;       ///< Offset 20   Panel fitting Request
  UINT32  CBLV;       ///< Offset 24   Brightness Current State
  UINT16  BCLM[0x14]; ///< Offset 28   Backlight Brightness Level Duty Cycle Mapping Table
  UINT32  CPFM;       ///< Offset 68   Panel Fitting Current Mode
  UINT32  EPFM;       ///< Offset 72   Enabled Panel Fitting Modes
  UINT8   PLUT[0x4A]; ///< Offset 76   Panel Look Up Table
  UINT32  PFMB;       ///< Offset 150  PWM Frequency and Minimum Brightness
  UINT32  CCDV;       ///< Offset 154  Color Correction Default Values
  UINT32  PCFT;       ///< Offset 158  Power Conservation Features
  UINT32  SROT;       ///< Offset 162  Supported Rotation angle
  UINT32  IUER;       ///< Offset 166  Intel Ultrabook Event Register
  UINT64  FDSP;       ///< Offset 170  FFS Display Physical address
  UINT32  FDSS;       ///< Offset 178  FFS Display Size
  UINT8   RM32[0x4A]; ///< Offset 182  Reserved
} OPREGION_MBOX3;
#pragma pack()
#pragma pack(1)
///
/// OpRegion mailbox 4 (VBT).
///
typedef struct {
  UINT8 GVD1[0x1C00]; ///< Reserved
} OPREGION_VBT;
#pragma pack()
#pragma pack(1)
///
/// IGD OpRegion Structure
///
typedef struct {
  OPREGION_HEADER Header; ///< OpRegion header
  OPREGION_MBOX1  MBox1;  ///< Mailbox 1: Public ACPI Methods
  OPREGION_MBOX2  MBox2;  ///< Mailbox 2: Software SCI Inteface
  OPREGION_MBOX3  MBox3;  ///< Mailbox 3: Power Conservation
  OPREGION_VBT    VBT;    ///< VBT: Video BIOS Table (OEM customizable data)
} IGD_OPREGION_STRUC;
#pragma pack()
///
/// IGD OpRegion Protocol
///
typedef struct {
  IGD_OPREGION_STRUC  *OpRegion; ///< IGD Operation Region Structure
} IGD_OPREGION_PROTOCOL;

#endif
