/** @file
  Rst policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _RST_CONFIG_H_
#define _RST_CONFIG_H_
#include <PchLimits.h>
#include <ConfigBlock.h>

#define RST_CONFIG_REVISION 1
extern EFI_GUID gRstConfigGuid;

#pragma pack (push,1)

typedef enum {
  SataOromDelay2sec,
  SataOromDelay4sec,
  SataOromDelay6sec,
  SataOromDelay8sec
} SATA_OROM_DELAY;

/**
  This structure describes the details of Intel RST for PCIe Storage remapping
  Note: In order to use this feature, Intel RST Driver is required
**/
typedef struct {
  /**
    This member describes whether or not the Intel RST for PCIe Storage remapping should be enabled. <b>0: Disable</b>; 1: Enable.
    Note 1: If Sata Controller is disabled, PCIe Storage Remapping should be disabled as well
    Note 2: If PCIe Storage remapping is enabled, the PCH integrated AHCI controllers Class Code is configured as RAID
  **/
  UINT32   Enable                 :  1;
  /**
    Intel RST for PCIe Storage remapping - PCIe Port Selection (1-based, <b>0 = autodetect</b>)
    The supported ports for PCIe Storage remapping is different depend on the platform and cycle router
  **/
  UINT32   RstPcieStoragePort     :  5;
  /**
    PCIe Storage Device Reset Delay in milliseconds (ms), which it guarantees such delay gap is fulfilled
    before PCIe Storage Device configuration space is accessed after an reset caused by the link disable and enable step.
    Default value is <b>100ms</b>.
  **/
  UINT32   DeviceResetDelay       :  8;
  UINT32   RsvdBits0              : 18; ///< Reserved bits

} RST_HARDWARE_REMAPPED_STORAGE_CONFIG;

/**
  Rapid Storage Technology settings.

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                             ///< Config Block Header

  UINT32  Raid0              :  1;         ///< 0  : Disable; <b>1  : Enable</b> RAID0
  UINT32  Raid1              :  1;         ///< 0  : Disable; <b>1  : Enable</b> RAID1
  UINT32  Raid10             :  1;         ///< 0  : Disable; <b>1  : Enable</b> RAID10
  UINT32  Raid5              :  1;         ///< 0  : Disable; <b>1  : Enable</b> RAID5
  UINT32  Irrt               :  1;         ///< 0  : Disable; <b>1  : Enable</b> Intel Rapid Recovery Technology
  UINT32  OromUiBanner       :  1;         ///< 0  : Disable; <b>1  : Enable</b> OROM UI and BANNER
  UINT32  OromUiDelay        :  2;         ///< <b>00b  : 2 secs</b>; 01b  : 4 secs; 10b  : 6 secs; 11  : 8 secs (see  : SATA_OROM_DELAY)
  UINT32  HddUnlock          :  1;         ///< 0  : Disable; <b>1  : Enable</b>. Indicates that the HDD password unlock in the OS is enabled
  UINT32  LedLocate          :  1;         ///< 0  : Disable; <b>1  : Enable</b>. Indicates that the LED/SGPIO hardware is attached and ping to locate feature is enabled on the OS
  UINT32  IrrtOnly           :  1;         ///< 0  : Disable; <b>1  : Enable</b>. Allow only IRRT drives to span internal and external ports
  UINT32  SmartStorage       :  1;         ///< 0  : Disable; <b>1  : Enable</b> RST Smart Storage caching Bit
  UINT32  LegacyOrom         :  1;         ///< <b>0  : Disable</b>; 1  : Enable RST Legacy OROM
  UINT32  OptaneMemory       :  1;         ///< 0: Disable; <b>1: Enable</b> RST Optane(TM) Memory
  UINT32  CpuAttachedStorage :  1;         ///< 0: Disable; <b>1: Enable</b> CPU Attached Storage
  UINT32  RsvdBits0          : 17;         ///< Reserved Bits
  /**
    This member describes the details of implementation of Intel RST for PCIe Storage remapping (Intel RST Driver is required)
    Note: RST for PCIe Sorage remapping is supported only for first SATA controller if more controllers are available
  **/
  RST_HARDWARE_REMAPPED_STORAGE_CONFIG   HardwareRemappedStorageConfig[PCH_MAX_RST_PCIE_STORAGE_CR];
} RST_CONFIG;

#pragma pack (pop)
#endif
