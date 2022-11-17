/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Guid/Cper.h>
#include "Ghes.h"
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#define READ_ACK_PRESERVE 0xFFFFFFFE
#define READ_ACK_WRITE 0x1

EFI_ACPI_6_1_GENERIC_ERROR_STATUS_STRUCTURE*
ErrorBlockInitial (
  VOID   *Block,
  UINT32 Severity
)
{
  EFI_ACPI_6_1_GENERIC_ERROR_STATUS_STRUCTURE* BlockHeader = Block;
  BlockHeader->BlockStatus = (EFI_ACPI_6_1_ERROR_BLOCK_STATUS) {0, 0, 0, 0, 0};
  BlockHeader->RawDataOffset = 0;
  BlockHeader->RawDataLength = 0;
  BlockHeader->DataLength = 0;
  BlockHeader->ErrorSeverity = Severity;
  return BlockHeader;
}


BOOLEAN
ErrorBlockUpdateStatusStructure (
  VOID *ErrorBlock
)
{
  if (ErrorBlock == NULL) {
    return FALSE;
  }
  IN EFI_ACPI_6_1_GENERIC_ERROR_STATUS_STRUCTURE *BlockHeader =  ErrorBlock;
  VOID *EntriesBegin = ErrorBlock + sizeof (EFI_ACPI_6_1_GENERIC_ERROR_STATUS_STRUCTURE);
  if (BlockHeader->BlockStatus.ErrorDataEntryCount == 0) {
    gBS->SetMem (EntriesBegin, BlockHeader->DataLength, 0);
    BlockHeader->RawDataLength = 0;
    BlockHeader->RawDataOffset = 0;
    BlockHeader->DataLength = 0;
  }
  return TRUE;
}


BOOLEAN
ErrorBlockAddErrorData (
  IN VOID                  *ErrorBlock,
  IN UINT32                MaxBlockLength,
  IN EFI_CPER_SECTION_TYPE TypeOfErrorData,
  IN VOID                  *GenericErrorData,
  IN UINT32                SizeOfGenericErrorData,
  IN ERROR_SEVERITY        ErrorSeverity,
  IN BOOLEAN               Correctable
)
{
  if (ErrorBlock == NULL || GenericErrorData == NULL) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL]Invalid Param \n", __FUNCTION__, __LINE__));
    return FALSE;
  }
  EFI_ACPI_6_1_GENERIC_ERROR_DATA_ENTRY_STRUCTURE*  Entry;
  EFI_ACPI_6_1_GENERIC_ERROR_STATUS_STRUCTURE* BlockHeader = ErrorBlock;
  EFI_ACPI_6_1_ERROR_BLOCK_STATUS* BlockStatus = &BlockHeader->BlockStatus;
  (VOID)ErrorBlockUpdateStatusStructure (ErrorBlock);
  UINT32 ExpectedNewDataLength = BlockHeader->DataLength +
           sizeof (EFI_ACPI_6_1_GENERIC_ERROR_DATA_ENTRY_STRUCTURE) +
           SizeOfGenericErrorData;
  if (sizeof (EFI_ACPI_6_1_GENERIC_ERROR_STATUS_STRUCTURE) + ExpectedNewDataLength >
      MaxBlockLength) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL]Out of BlockSize \n", __FUNCTION__, __LINE__));
    return FALSE;
  }
  // guid
  EFI_GUID Guid;
  switch (TypeOfErrorData) {
    case PROCESSOR_GENERIC:
      Guid = (EFI_GUID)EFI_ERROR_SECTION_PROCESSOR_GENERIC_GUID;
      break;
    case PROCESSOR_ARM:
      Guid = (EFI_GUID)EFI_ERROR_SECTION_PROCESSOR_SPECIFIC_ARM_GUID;
      break;
    case PLATFORM_MEMORY:
      Guid = (EFI_GUID)EFI_ERROR_SECTION_PLATFORM_MEMORY_GUID;
      break;
    case PLATFORM_MEMORY2:
      Guid = (EFI_GUID)EFI_ERROR_SECTION_PLATFORM_MEMORY2_GUID;
      break;
    case PCIE_EXPRESS:
      Guid = (EFI_GUID)EFI_ERROR_SECTION_PCIE_GUID;
      break;
    case FIRMWARE_ERROR:
      Guid = (EFI_GUID)EFI_ERROR_SECTION_FW_ERROR_RECORD_GUID;
      break;
    case PCI_BUS:
      Guid = (EFI_GUID)EFI_ERROR_SECTION_PCI_PCIX_BUS_GUID;
      break;
    case PCI_COMPONENT:
      Guid = (EFI_GUID)EFI_ERROR_SECTION_PCI_DEVICE_GUID;
      break;
    default:
      return FALSE;
  }
  //Block Status
  if (Correctable == TRUE) {
    if (BlockStatus->CorrectableErrorValid == 0) {
      BlockStatus->CorrectableErrorValid = 1;
    } else {
      BlockStatus->MultipleCorrectableErrors = 1;
    }
  } else {
    if (BlockStatus->UncorrectableErrorValid == 0) {
      BlockStatus->UncorrectableErrorValid = 1;
    } else {
      BlockStatus->MultipleUncorrectableErrors = 1;
    }
  }
  BlockStatus->ErrorDataEntryCount++;
  // Entry
  Entry = (EFI_ACPI_6_1_GENERIC_ERROR_DATA_ENTRY_STRUCTURE*)(ErrorBlock +
           sizeof (EFI_ACPI_6_1_GENERIC_ERROR_STATUS_STRUCTURE) +
           BlockHeader->DataLength);
  gBS->SetMem (Entry, sizeof (EFI_ACPI_6_1_GENERIC_ERROR_DATA_ENTRY_STRUCTURE), 0);
  gBS->CopyMem (&Entry->SectionType, &Guid, sizeof (EFI_GUID));
  Entry->ErrorSeverity = ErrorSeverity;
  Entry->Revision = EFI_ACPI_6_1_GENERIC_ERROR_DATA_ENTRY_REVISION;
  Entry->ErrorDataLength = SizeOfGenericErrorData;
  VOID*  GenericErrorDataFollowEntry = (VOID*)Entry +
           sizeof (EFI_ACPI_6_1_GENERIC_ERROR_DATA_ENTRY_STRUCTURE);
  gBS->CopyMem (
         GenericErrorDataFollowEntry,
         GenericErrorData,
         SizeOfGenericErrorData);
  // BlockHeader
  BlockHeader->RawDataOffset = 0;
  BlockHeader->RawDataLength = 0;
  BlockHeader->DataLength = ExpectedNewDataLength;
  return TRUE;
}

VOID
GhesV2Initial (
  EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_VERSION_2_STRUCTURE *GhesV2,
  UINT32                                                         BlockLength
)
{
  if (GhesV2 == NULL) {
    return;
  }
  *GhesV2 = (EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_VERSION_2_STRUCTURE) {
    .Type = EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_VERSION_2,
    .SourceId = 0,
    .RelatedSourceId = 0xFFFF,
    .Flags = 0,
    .Enabled = 1,
    .NumberOfRecordsToPreAllocate = 1,//ERROR BLOCK
    .MaxSectionsPerRecord = 1,// Num Entries(section)
    .MaxRawDataLength = BlockLength, // Max Size Of a Raw Data
    .ErrorStatusAddress = {
      .AddressSpaceId = EFI_ACPI_6_1_SYSTEM_MEMORY,
      .RegisterBitWidth = 64,
      .RegisterBitOffset = 0,
      .AccessSize = EFI_ACPI_6_1_QWORD,
      .Address = 0
    },
    .NotificationStructure = {
      .Type = EFI_ACPI_6_1_HARDWARE_ERROR_NOTIFICATION_GSIV,
      .Length = sizeof (EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_VERSION_2_STRUCTURE),
      .ConfigurationWriteEnable = {0, 0, 0, 0, 0, 0, 0} ,
      .PollInterval = 0,
      .Vector = 0,
      .SwitchToPollingThresholdValue = 0,
      .SwitchToPollingThresholdWindow = 0,
      .ErrorThresholdValue = 0,
      .ErrorThresholdWindow = 0
    },
    .ErrorStatusBlockLength = BlockLength,
    .ReadAckRegister = {
      .AddressSpaceId = EFI_ACPI_6_1_SYSTEM_MEMORY,
      .RegisterBitWidth = 64,
      .RegisterBitOffset = 0,
      .AccessSize = EFI_ACPI_6_1_QWORD,
      .Address = 0
    },
    .ReadAckPreserve = READ_ACK_PRESERVE,
    .ReadAckWrite = READ_ACK_WRITE
  };
  return;
}

VOID
GhesV2AddNotification (
  EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_VERSION_2_STRUCTURE  *This,
  UINT8                                                           Type
)
{
  This->NotificationStructure = (EFI_ACPI_6_1_HARDWARE_ERROR_NOTIFICATION_STRUCTURE) {
    .Type = Type,
    .Length = sizeof (EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_VERSION_2_STRUCTURE),
    .ConfigurationWriteEnable = {
      .Type = 0,
      .PollInterval = 1,
      .SwitchToPollingThresholdValue = 1,
      .SwitchToPollingThresholdWindow = 1,
      .ErrorThresholdValue = 1,
      .ErrorThresholdWindow = 1
    },
    .PollInterval = 20,
    .Vector = 0,
    .SwitchToPollingThresholdValue = 0,
    .SwitchToPollingThresholdWindow = 0,
    .ErrorThresholdValue = 0,
    .ErrorThresholdWindow = 0
  };
  return;
}

EFI_STATUS
GhesV2LinkErrorBlock (
  EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_VERSION_2_STRUCTURE *GhesV2,
  GHES_REGISTER                                                  *Register,
  VOID                                                           *ErrorBlock
)
{
  if (ErrorBlock == NULL || Register == NULL || GhesV2 == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Register->ErrorStatusBlockAddress = (UINTN)ErrorBlock;
  GhesV2->ErrorStatusAddress.Address = (UINTN)&(Register->ErrorStatusBlockAddress);
  Register->AckRegister = READ_ACK_WRITE;
  GhesV2->ReadAckRegister.Address = (UINT64)&(Register->AckRegister);
  return EFI_SUCCESS;
}


VOID GhesV1Initial (
  EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE *GhesV1,
  UINT32 BlockLength
)
{
  if (GhesV1 == NULL) {
    return;
  }
  *GhesV1 = (EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE) {
    .Type = EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR,
    .SourceId = 0,
    .RelatedSourceId = 0xFFFF,
    .Flags = 0,
    .Enabled = 1,
    .NumberOfRecordsToPreAllocate = 1,//ERROR BLOCK
    .MaxSectionsPerRecord = 1,// Num Entries(section)
    .MaxRawDataLength = BlockLength, // Max Size Of a Raw Data
    .ErrorStatusAddress = {
      .AddressSpaceId = EFI_ACPI_6_1_SYSTEM_MEMORY,
      .RegisterBitWidth = 64,
      .RegisterBitOffset = 0,
      .AccessSize = EFI_ACPI_6_1_QWORD,
      .Address = 0
    },
    .NotificationStructure = {
      .Type = EFI_ACPI_6_1_HARDWARE_ERROR_NOTIFICATION_GSIV,
      .Length = sizeof (EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE),
      .ConfigurationWriteEnable = {0, 0, 0, 0, 0, 0, 0},
      .PollInterval = 0,
      .Vector = 0,
      .SwitchToPollingThresholdValue = 0,
      .SwitchToPollingThresholdWindow = 0,
      .ErrorThresholdValue = 0,
      .ErrorThresholdWindow = 0
    },
    .ErrorStatusBlockLength = BlockLength,
  };
  return;
}

VOID
GhesV1AddNotification (
  EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE *This,
  UINT8                                                Type
)
{
  This->NotificationStructure = (EFI_ACPI_6_1_HARDWARE_ERROR_NOTIFICATION_STRUCTURE) {
    .Type = Type,
    .Length = sizeof (EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE),
    .ConfigurationWriteEnable = {
      .Type = 0,
      .PollInterval = 1,
      .SwitchToPollingThresholdValue = 1,
      .SwitchToPollingThresholdWindow = 1,
      .ErrorThresholdValue = 1,
      .ErrorThresholdWindow = 1
    },
    .PollInterval = 20,
    .Vector = 0,
    .SwitchToPollingThresholdValue = 0,
    .SwitchToPollingThresholdWindow = 0,
    .ErrorThresholdValue = 0,
    .ErrorThresholdWindow = 0
  };
  return;
}

EFI_STATUS GhesV1LinkErrorBlock (
  EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE *This,
  UINT64                                               *ptrBlockAddress,
  VOID                                                 *ErrorBlock
)
{
  if (ErrorBlock == NULL || ptrBlockAddress == NULL || This == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  *ptrBlockAddress = (UINTN)ErrorBlock;
  This->ErrorStatusAddress.Address = (UINTN) ptrBlockAddress;
  return EFI_SUCCESS;
}

