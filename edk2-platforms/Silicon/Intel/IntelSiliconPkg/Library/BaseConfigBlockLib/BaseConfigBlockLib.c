/** @file
  Library functions for Config Block management.

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <ConfigBlock.h>
#include <Library/ConfigBlockLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>

/**
  Create config block table.

  @param[in]     TotalSize                    - Max size to be allocated for the Config Block Table
  @param[out]    ConfigBlockTableAddress      - On return, points to a pointer to the beginning of Config Block Table Address

  @retval EFI_INVALID_PARAMETER - Invalid Parameter
  @retval EFI_OUT_OF_RESOURCES  - Out of resources
  @retval EFI_SUCCESS           - Successfully created Config Block Table at ConfigBlockTableAddress
**/
EFI_STATUS
EFIAPI
CreateConfigBlockTable (
  IN     UINT16    TotalSize,
  OUT    VOID      **ConfigBlockTableAddress
  )
{
  CONFIG_BLOCK_TABLE_HEADER *ConfigBlkTblAddrPtr;
  UINT32                    ConfigBlkTblHdrSize;

  ConfigBlkTblHdrSize = (UINT32)(sizeof (CONFIG_BLOCK_TABLE_HEADER));

  if (TotalSize <= (ConfigBlkTblHdrSize + sizeof (CONFIG_BLOCK_HEADER))) {
    return EFI_INVALID_PARAMETER;
  }

  ConfigBlkTblAddrPtr = (CONFIG_BLOCK_TABLE_HEADER *)AllocateZeroPool (TotalSize);
  if (ConfigBlkTblAddrPtr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  ConfigBlkTblAddrPtr->NumberOfBlocks = 0;
  ConfigBlkTblAddrPtr->Header.GuidHob.Header.HobLength = TotalSize;
  ConfigBlkTblAddrPtr->AvailableSize = TotalSize - ConfigBlkTblHdrSize;

  *ConfigBlockTableAddress = (VOID *)ConfigBlkTblAddrPtr;

  return EFI_SUCCESS;
}

/**
  Add config block into config block table structure.

  @param[in]     ConfigBlockTableAddress      - A pointer to the beginning of Config Block Table Address
  @param[out]    ConfigBlockAddress           - On return, points to a pointer to the beginning of Config Block Address

  @retval EFI_OUT_OF_RESOURCES - Config Block Table is full and cannot add new Config Block or
                                 Config Block Offset Table is full and cannot add new Config Block.
  @retval EFI_SUCCESS          - Successfully added Config Block
**/
EFI_STATUS
EFIAPI
AddConfigBlock (
  IN     VOID      *ConfigBlockTableAddress,
  OUT    VOID      **ConfigBlockAddress
  )
{
  CONFIG_BLOCK              *TempConfigBlk;
  CONFIG_BLOCK_TABLE_HEADER *ConfigBlkTblAddrPtr;
  CONFIG_BLOCK              *ConfigBlkAddrPtr;
  UINT16                    ConfigBlkSize;

  ConfigBlkTblAddrPtr = (CONFIG_BLOCK_TABLE_HEADER *)ConfigBlockTableAddress;
  ConfigBlkAddrPtr = (CONFIG_BLOCK *)(*ConfigBlockAddress);
  ConfigBlkSize = ConfigBlkAddrPtr->Header.GuidHob.Header.HobLength;

  if ((ConfigBlkSize % 4) != 0) {
    return EFI_INVALID_PARAMETER;
  }
  if (ConfigBlkTblAddrPtr->AvailableSize < ConfigBlkSize) {
    return EFI_OUT_OF_RESOURCES;
  }

  TempConfigBlk = (CONFIG_BLOCK *)((UINTN)ConfigBlkTblAddrPtr + (UINTN)(ConfigBlkTblAddrPtr->Header.GuidHob.Header.HobLength - ConfigBlkTblAddrPtr->AvailableSize));
  CopyMem (&TempConfigBlk->Header, &ConfigBlkAddrPtr->Header, sizeof(CONFIG_BLOCK_HEADER));

  ConfigBlkTblAddrPtr->NumberOfBlocks++;
  ConfigBlkTblAddrPtr->AvailableSize = ConfigBlkTblAddrPtr->AvailableSize - ConfigBlkSize;

  *ConfigBlockAddress = (VOID *) TempConfigBlk;
  return EFI_SUCCESS;
}

/**
  Retrieve a specific Config Block data by GUID.

  @param[in]      ConfigBlockTableAddress      - A pointer to the beginning of Config Block Table Address
  @param[in]      ConfigBlockGuid              - A pointer to the GUID uses to search specific Config Block
  @param[out]     ConfigBlockAddress           - On return, points to a pointer to the beginning of Config Block Address

  @retval EFI_NOT_FOUND         - Could not find the Config Block
  @retval EFI_SUCCESS           - Config Block found and return
**/
EFI_STATUS
EFIAPI
GetConfigBlock (
  IN     VOID      *ConfigBlockTableAddress,
  IN     EFI_GUID  *ConfigBlockGuid,
  OUT    VOID      **ConfigBlockAddress
  )
{
  UINT16                    OffsetIndex;
  CONFIG_BLOCK              *TempConfigBlk;
  CONFIG_BLOCK_TABLE_HEADER *ConfigBlkTblAddrPtr;
  UINT32                    ConfigBlkTblHdrSize;
  UINT32                    ConfigBlkOffset;
  UINT16                    NumOfBlocks;

  ConfigBlkTblHdrSize = (UINT32)(sizeof (CONFIG_BLOCK_TABLE_HEADER));
  ConfigBlkTblAddrPtr = (CONFIG_BLOCK_TABLE_HEADER *)ConfigBlockTableAddress;
  NumOfBlocks = ConfigBlkTblAddrPtr->NumberOfBlocks;

  ConfigBlkOffset = 0;
  for (OffsetIndex = 0; OffsetIndex < NumOfBlocks; OffsetIndex++) {
    if ((ConfigBlkTblHdrSize + ConfigBlkOffset) > (ConfigBlkTblAddrPtr->Header.GuidHob.Header.HobLength)) {
      break;
    }
    TempConfigBlk = (CONFIG_BLOCK *)((UINTN)ConfigBlkTblAddrPtr + (UINTN)ConfigBlkTblHdrSize + (UINTN)ConfigBlkOffset);
    if (CompareGuid (&(TempConfigBlk->Header.GuidHob.Name), ConfigBlockGuid)) {
      *ConfigBlockAddress = (VOID *)TempConfigBlk;
      return EFI_SUCCESS;
    }
    ConfigBlkOffset = ConfigBlkOffset + TempConfigBlk->Header.GuidHob.Header.HobLength;
  }

  return EFI_NOT_FOUND;
}
