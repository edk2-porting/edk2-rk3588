/** @file
  Intel PCI security data structure definition from
  PCIe* Device Security Enhancements Specification.

  https://www.intel.com/content/www/us/en/io/pci-express/pcie-device-security-enhancements-spec.html

  NOTE: The data structure is not fully match the current specification,
  because it is aligned with the real hardware implementation with minor adjustment
  on INTEL_PCI_DIGEST_CAPABILITY_STRUCTURE, INTEL_PCI_DIGEST_DATA_MODIFIED and
  INTEL_PCI_DIGEST_DATA_VALID.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __INTEL_PCI_SECURITY_H__
#define __INTEL_PCI_SECURITY_H__

#pragma pack(1)

///
/// The PCIE capability structure header for Intel PCI DVSEC extension.
///
typedef struct {
  UINT16  CapId;           // 0x23: DVSEC
  UINT16  CapVersion:4;    // 1
  UINT16  NextOffset:12;
  UINT16  DvSecVendorId;   // 0x8086
  UINT16  DvSecRevision:4; // 1
  UINT16  DvSecLength:12;
  UINT16  DvSecId;         // 0x3E: Measure
} INTEL_PCI_DIGEST_CAPABILITY_HEADER;

#define INTEL_PCI_CAPID_DVSEC                0x23
#define INTEL_PCI_DVSEC_VENDORID_INTEL       0x8086
#define INTEL_PCI_DVSEC_DVSECID_MEASUREMENT  0x3E

///
/// The Intel PCI digest modified macro.
///
#define INTEL_PCI_DIGEST_MODIFIED        BIT0

///
/// The Intel PCI DVSEC digest data modified structure.
///
typedef union {
  struct {
    UINT8   DigestModified:1;         // RW1C
    UINT8   Reserved0:7;
  } Bits;
  UINT8 Data;
} INTEL_PCI_DIGEST_DATA_MODIFIED;

///
/// The Intel PCI digest valid macro.
///
#define INTEL_PCI_DIGEST_0_VALID         BIT0
#define INTEL_PCI_DIGEST_0_LOCKED        BIT1
#define INTEL_PCI_DIGEST_1_VALID         BIT2
#define INTEL_PCI_DIGEST_1_LOCKED        BIT3

///
/// The Intel PCI DVSEC digest data valid structure.
///
typedef union {
  struct {
    UINT8   Digest0Valid:1;          // RO
    UINT8   Digest0Locked:1;         // RO
    UINT8   Digest1Valid:1;          // RO
    UINT8   Digest1Locked:1;         // RO
    UINT8   Reserved1:4;
  } Bits;
  UINT8 Data;
} INTEL_PCI_DIGEST_DATA_VALID;

///
/// The PCIE capability structure for Intel PCI DVSEC extension with digest.
///
typedef struct {
  INTEL_PCI_DIGEST_DATA_MODIFIED   Modified;   // RW1C
  INTEL_PCI_DIGEST_DATA_VALID      Valid;      // RO
  UINT16                           TcgAlgId;   // RO
  UINT8                            FirmwareID; // RO
  UINT8                            Reserved;
//UINT8                            Digest[];
} INTEL_PCI_DIGEST_CAPABILITY_STRUCTURE;

#pragma pack()

#endif

