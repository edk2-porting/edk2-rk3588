/** @file
  Configurations for HostBridge

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _HOST_BRIDGE_CONFIG_H_
#define _HOST_BRIDGE_CONFIG_H_

#include <ConfigBlock.h>

#define HOST_BRIDGE_PREMEM_CONFIG_REVISION 1
#define HOST_BRIDGE_PEI_CONFIG_REVISION    1

extern EFI_GUID gHostBridgePeiPreMemConfigGuid;
extern EFI_GUID gHostBridgePeiConfigGuid;

#pragma pack (push,1)

/**
  This configuration block describes HostBridge settings in PreMem.\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;               ///< Offset 0-27 Config Block Header
  UINT32  MchBar;                            ///< Offset 28 Address of System Agent MCHBAR: <b>0xFEDC0000(TGL)/0xFED10000(RKL)/0xFEA80000(JSL)<b>
  UINT32  DmiBar;                            ///< Offset 32 Address of System Agent DMIBAR: <b>0xFEDA0000</b>
  UINT32  EpBar;                             ///< Offset 36 Address of System Agent EPBAR: <b>0xFEDA1000</b>
  UINT32  GdxcBar;                           ///< Offset 40 Address of System Agent GDXCBAR: <b>0xFED84000</b>
  UINT32  RegBar;                            ///< Offset 44 Address of System Agent REGBAR: <b>0xFB000000</b>
  UINT32  EdramBar;                          ///< Offset 48 Address of System Agent EDRAMBAR: <b>0xFED80000</b>
  /**
    Offset 52 :
    Size of reserved MMIO space for PCI devices\n
    <b>0=AUTO</b>, 512=512MB, 768=768MB, 1024=1024MB, 1280=1280MB, 1536=1536MB, 1792=1792MB,
    2048=2048MB, 2304=2304MB, 2560=2560MB, 2816=2816MB, 3072=3072MB\n
    When AUTO mode selected, the MMIO size will be calculated by required MMIO size from PCIe devices detected.
  **/
  UINT32  MmioSize;
  UINT32  MmioSizeAdjustment;                ///< Offset 56 Increase (given positive value) or Decrease (given negative value) the Reserved MMIO size when Dynamic Tolud/AUTO mode enabled (in MBs): <b>0=no adjustment</b>
  UINT8   EnableAbove4GBMmio;                ///< Offset 60 Enable/disable above 4GB MMIO resource support: 0=Disable, <b>1=Enable</b>
  UINT8   Reserved[3];                       ///< Offset 61 Reserved for future use.
} HOST_BRIDGE_PREMEM_CONFIG;


/**
  This configuration block describes HostBridge settings in Post-Mem.\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;               ///< Offset 0-27 Config Block Header
  UINT8  Device4Enable;                      ///< Offser 28 :This policy is used to control enable or disable System Agent Thermal device (0,4,0). <b>0=FALSE</b>, 1=TRUE.
  UINT8  ChapDeviceEnable;                   ///< Offset 29 :<b>(Test)</b>This policy is used to control enable or disable System Agent Chap device (0,7,0). <b>0=FALSE</b>, 1=TRUE.
  UINT8  SkipPamLock;                        ///< Offset 30 :To skip PAM register locking. @note It is still recommended to set PCI Config space B0: D0: F0: Offset 80h[0]=1 in platform code even Silicon code skipped this.\n <b>0=All PAM registers will be locked in Silicon code</b>, 1=Skip lock PAM registers in Silicon code.
  UINT8  EdramTestMode;                      ///< Offset 28 :EDRAM Test Mode. For EDRAM stepping - 0- EDRAM SW Disable, 1- EDRAM SW Enable, <b> 2- EDRAM HW Mode</b>
} HOST_BRIDGE_PEI_CONFIG;

#pragma pack (pop)

#endif
