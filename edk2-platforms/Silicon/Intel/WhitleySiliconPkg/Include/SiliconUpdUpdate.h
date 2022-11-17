/** @file
  Header file for the SiliconUpdUpdate.h Library.

  @copyright
  Copyright 2020 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SILICON_UPD_UPDATE_H_
#define _SILICON_UPD_UPDATE_H_

#pragma pack(1)
typedef struct {
  //For IioPcieConfig
  UINT8   Socket;
  UINT16  PortIndex;
  UINT8   HidePort;
  UINT8   DeEmphasis;
  UINT8   PortLinkSpeed;
  UINT8   MaxPayload;
  UINT8   ReservedE;
  UINT8   ReservedF;
  UINT8   ReservedG;
  UINT8   Sris;
  UINT8   PcieCommonClock;
  //For IIO Ntb
  UINT8   NtbIndex;
  UINT8   NtbPpd;
  UINT8   NtbBarSizeOverride;
  UINT8   NtbSplitBar;
  UINT8   NtbBarSizeImBar1;
  UINT8   NtbBarSizeImBar2;
  UINT8   NtbBarSizeImBar2_0;
  UINT8   NtbBarSizeImBar2_1;
  UINT8   NtbBarSizeEmBarSZ1;
  UINT8   NtbBarSizeEmBarSZ2;
  UINT8   NtbBarSizeEmBarSZ2_0;
  UINT8   NtbBarSizeEmBarSZ2_1;
  UINT8   NtbXlinkCtlOverride;
} UPD_IIO_PCIE_PORT_CONFIG;

// IIO_PCIE_PORT_CONFIG:
// PciePortConfiguration - Pointer to an array of PCIe port configuration structures as declared above
// NumberOfEntries       - Number of elements in the PciePortConfiguration Array

typedef struct {
  UPD_IIO_PCIE_PORT_CONFIG  *ConfigurationTable;
  UINT16                     NumberOfEntries;
} IIO_PCIE_PORT_CONFIG;
#pragma pack()

#endif
