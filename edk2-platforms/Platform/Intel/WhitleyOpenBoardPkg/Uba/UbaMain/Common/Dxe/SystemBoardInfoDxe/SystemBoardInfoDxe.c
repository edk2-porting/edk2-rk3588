/** @file
  System Board Info.

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "SystemBoardInfoDxe.h"


//
// Describes Local APICs' connections.
//
STATIC DEVICE_DATA_HW_LOCAL_INT      DeviceDataHwLocalInt1[] = {
  {
    {{0},{{0xFF,0},{0xFF,0},{0xFF,0}}},
    0x00,
    0xff,
    0x00,
    EfiLegacyMpTableEntryLocalIntTypeExtInt,
    EfiLegacyMpTableEntryLocalIntFlagsPolaritySpec,
    EfiLegacyMpTableEntryLocalIntFlagsTriggerSpec
  },
  {
    {{0},{{0xFF,0},{0xFF,0},{0xFF,0}}},
    0x00,
    0xff,
    0x01,
    EfiLegacyMpTableEntryLocalIntTypeInt,
    EfiLegacyMpTableEntryLocalIntFlagsPolaritySpec,
    EfiLegacyMpTableEntryLocalIntFlagsTriggerSpec
  },
};

//
// Describes system's address space mapping, specific to the system.
//
STATIC DEVICE_DATA_HW_ADDR_SPACE_MAPPING DeviceDataHwAddrSpace1[] = {
  //
  // Legacy IO addresses.
  //
  { {0}, EfiLegacyMpTableEntryExtSysAddrSpaceMappingIo,       0x0000,     0x1000    },
  /*
  { {0}, EfiLegacyMpTableEntryExtSysAddrSpaceMappingMemory,   0xXXXXXXXX, 0xXXXXXXXX},
  { {0}, EfiLegacyMpTableEntryExtSysAddrSpaceMappingPrefetch, 0xXXXXXXXX, 0xXXXXXXXX},
  */
};

//
// IRQ priority
//
STATIC EFI_LEGACY_IRQ_PRIORITY_TABLE_ENTRY  IrqPriorityTable1[] = {
  {11, 0},
  {10, 0},
  {9, 0},
  {5, 0},
  {0, 0},
  {0, 0},
  {0, 0}
};

//
// Note : UpdateBusNumbers updates the bus numeber
//
STATIC EFI_LEGACY_PIRQ_TABLE  PirqTableHead1 [] = {
  {
    {
      EFI_PIRQ_TABLE_SIGNATURE,
      00,
      01,
      0000,
      00,
      00,
      0000,
      V_INTEL_VID,
      30,
      00000000,
      {
        00,
        00,
        00,
        00,
        00,
        00,
        00,
        00,
        00,
        00,
        00
      },
      00
    }
  }
};

//
// Instantiation of the system device data.
//
DEVICE_DATA           mDeviceData = {
  DeviceDataHwLocalInt1,   sizeof (DeviceDataHwLocalInt1) / sizeof (DeviceDataHwLocalInt1[0]),
  DeviceDataHwAddrSpace1,  sizeof (DeviceDataHwAddrSpace1)/ sizeof (DeviceDataHwAddrSpace1[0])
};

//
// Instantiation of platform PIRQ data.
//
PLATFORM_PIRQ_DATA    mPlatformPirqData = {
  IrqPriorityTable1,    sizeof(IrqPriorityTable1) / sizeof(IrqPriorityTable1[0]),
  PirqTableHead1,       sizeof(PirqTableHead1) / sizeof(PirqTableHead1[0])
};


PCI_OPTION_ROM_TABLE      mPciOptionRomTable[] = {
  //
  // End of OptionROM Entries
  //
  {
    NULL_ROM_FILE_GUID, // Guid
    0,                  // Segment
    0,                  // Bus Number
    0,                  // Device Number
    0,                  // Function Number
    0xffff,             // Vendor ID
    0xffff              // Device ID
  }
};

//
// system board information structure
//
DXE_SYSTEM_BOARD_INFO SystemBoardInfoTable = {
  //
  // System board configuration data
  //
  mPciOptionRomTable,

  //
  // System board CPU data
  //
  2,                                      // Cpu socket count

  //
  // System board legacy data
  //
  &mDeviceData,
  &mPlatformPirqData
};

DXE_SYSTEM_BOARD_INFO *SystemBoardInfoCallback ()
{
  return (DXE_SYSTEM_BOARD_INFO*) &SystemBoardInfoTable;
}

SYSTEM_BOARD_INFO_DATA SystemBoardInfoData = {
  SYSTEM_SYSTEM_BOARD_INFO_SIGNATURE,
  SYSTEM_SYSTEM_BOARD_INFO_VERSION,
  SystemBoardInfoCallback
};
/**
  The Driver Entry Point.

  The function is the driver Entry point.

  @param ImageHandle   A handle for the image that is initializing this driver
  @param SystemTable   A pointer to the EFI system table

  @retval EFI_SUCCESS:              Driver initialized successfully
  @retval EFI_LOAD_ERROR:           Failed to Initialize or has been loaded
  @retval EFI_OUT_OF_RESOURCES      Could not allocate needed resources

**/
EFI_STATUS
EFIAPI
SystemBoardInfoEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
)
{
  EFI_STATUS                                Status;
  UBA_CONFIG_DATABASE_PROTOCOL             *UbaConfigProtocol = NULL;

  DEBUG((EFI_D_INFO, "UBA:System Board Info Table.\n"));
  Status = gBS->LocateProtocol (
                  &gUbaConfigDatabaseProtocolGuid,
                  NULL,
                  &UbaConfigProtocol
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = UbaConfigProtocol->AddData (
                                     UbaConfigProtocol,
                                     &gSystemBoardInfoConfigDataGuid,
                                     &SystemBoardInfoData,
                                     sizeof(SystemBoardInfoData)
                                     );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  return Status;
}
