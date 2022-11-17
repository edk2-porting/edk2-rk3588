/** @file
*  OemMiscLib.c
*
*  Copyright (c) 2021, NUVIA Inc. All rights reserved.
*  Copyright (c) 2020, Linaro Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/FdtHelperLib.h>
#include <Library/HiiLib.h>
#include <Library/IoLib.h>
#include <Library/OemMiscLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimerLib.h>
#include <libfdt.h>

/** Returns whether the specified processor is present or not.

  @param ProcessorIndex The processor index to check.

  @return TRUE if the processor is present, FALSE otherwise.
**/
BOOLEAN
EFIAPI
OemIsProcessorPresent (
  UINTN ProcessorIndex
  )
{
  if (ProcessorIndex < FdtHelperCountCpus ()) {
    return TRUE;
  }

  return FALSE;
}

/** Gets the CPU frequency of the specified processor.

  @param ProcessorIndex Index of the processor to get the frequency for.

  @return               CPU frequency in Hz
**/
UINTN
EFIAPI
OemGetCpuFreq (
  UINT8 ProcessorIndex
  )
{
  return 2000000000; // 2 GHz
}

/** Gets information about the specified processor and stores it in
    the structures provided.

  @param ProcessorIndex  Index of the processor to get the information for.
  @param ProcessorStatus Processor status.
  @param ProcessorCharacteristics Processor characteritics.
  @param MiscProcessorData        Miscellaneous processor information.

  @return  TRUE on success, FALSE on failure.
**/
BOOLEAN
EFIAPI
OemGetProcessorInformation (
  IN  UINTN                             ProcessorIndex,
  IN OUT PROCESSOR_STATUS_DATA          *ProcessorStatus,
  IN OUT PROCESSOR_CHARACTERISTIC_FLAGS *ProcessorCharacteristics,
  IN OUT OEM_MISC_PROCESSOR_DATA        *MiscProcessorData
  )
{
  UINT16 ProcessorCount;

  ProcessorCount = FdtHelperCountCpus ();

  if (ProcessorIndex < ProcessorCount) {
    ProcessorStatus->Bits.CpuStatus       = 1; // CPU enabled
    ProcessorStatus->Bits.Reserved1       = 0;
    ProcessorStatus->Bits.SocketPopulated = 1;
    ProcessorStatus->Bits.Reserved2       = 0;
  } else {
    ProcessorStatus->Bits.CpuStatus       = 0; // CPU disabled
    ProcessorStatus->Bits.Reserved1       = 0;
    ProcessorStatus->Bits.SocketPopulated = 0;
    ProcessorStatus->Bits.Reserved2       = 0;
  }

  ProcessorCharacteristics->ProcessorReserved1      = 0;
  ProcessorCharacteristics->ProcessorUnknown        = 0;
  ProcessorCharacteristics->Processor64BitCapable   = 1;
  ProcessorCharacteristics->ProcessorMultiCore      = 0;
  ProcessorCharacteristics->ProcessorHardwareThread = 0;
  ProcessorCharacteristics->ProcessorExecuteProtection      = 1;
  ProcessorCharacteristics->ProcessorEnhancedVirtualization = 0;
  ProcessorCharacteristics->ProcessorPowerPerformanceCtrl   = 0;
  ProcessorCharacteristics->Processor128BitCapable = 0;
  ProcessorCharacteristics->ProcessorArm64SocId = 1;
  ProcessorCharacteristics->ProcessorReserved2  = 0;

  MiscProcessorData->CurrentSpeed = 2000;
  MiscProcessorData->MaxSpeed     = 2000;
  MiscProcessorData->CoreCount    = 1;
  MiscProcessorData->CoresEnabled = 1;
  MiscProcessorData->ThreadCount  = 1;

  return TRUE;
}

/** Gets the maximum number of processors supported by the platform.

  @return The maximum number of processors.
**/
UINT8
EFIAPI
OemGetMaxProcessors (
  VOID
  )
{
  return FdtHelperCountCpus ();
}

/** Gets information about the cache at the specified cache level.

  @param ProcessorIndex The processor to get information for.
  @param CacheLevel     The cache level to get information for.
  @param DataCache      Whether the cache is a data cache.
  @param UnifiedCache   Whether the cache is a unified cache.
  @param SmbiosCacheTable The SMBIOS Type7 cache information structure.

  @return TRUE on success, FALSE on failure.
**/
BOOLEAN
EFIAPI
OemGetCacheInformation (
  IN UINT8     ProcessorIndex,
  IN UINT8     CacheLevel,
  IN BOOLEAN   DataCache,
  IN BOOLEAN   UnifiedCache,
  IN OUT SMBIOS_TABLE_TYPE7 *SmbiosCacheTable
  )
{
  SmbiosCacheTable->CacheConfiguration = CacheLevel - 1;

  if (CacheLevel == 1 && !DataCache && !UnifiedCache) {
    // Unknown operational mode
    SmbiosCacheTable->CacheConfiguration |= (3 << 8);
  } else {
    // Write back operational mode
    SmbiosCacheTable->CacheConfiguration |= (1 << 8);
  }

  return TRUE;
}

/** Gets the type of chassis for the system.

  @retval The type of the chassis.
**/
MISC_CHASSIS_TYPE
EFIAPI
OemGetChassisType (
  VOID
  )
{
  return MiscChassisTypeMainServerChassis;
}

/** Updates the HII string for the specified field.

  @param HiiHandle     The HII handle.
  @param TokenToUpdate The string to update.
  @param Field         The field to get information about.
**/
VOID
EFIAPI
OemUpdateSmbiosInfo (
  IN EFI_HII_HANDLE HiiHandle,
  IN EFI_STRING_ID TokenToUpdate,
  IN OEM_MISC_SMBIOS_HII_STRING_FIELD Field
  )
{
  CHAR16 *String;

  // These values are fixed for now, but should be configurable via
  // something like an emulated SCP.
  switch (Field) {
    case SystemManufacturerType01:
      String = (CHAR16*)PcdGetPtr (PcdSystemManufacturer);
      break;
    case SerialNumType01:
      String = (CHAR16*)PcdGetPtr (PcdSystemSerialNumber);
      break;
    case SkuNumberType01:
      String = (CHAR16*)PcdGetPtr (PcdSystemSKU);
      break;
    case FamilyType01:
      String = (CHAR16*)PcdGetPtr (PcdSystemFamily);
      break;
    case AssertTagType02:
      String = (CHAR16*)PcdGetPtr (PcdBaseBoardAssetTag);
      break;
    case SerialNumberType02:
      String = (CHAR16*)PcdGetPtr (PcdBaseBoardSerialNumber);
      break;
    case BoardManufacturerType02:
      String = (CHAR16*)PcdGetPtr (PcdBaseBoardManufacturer);
      break;
    case SkuNumberType02:
      String = (CHAR16*)PcdGetPtr (PcdBaseBoardSKU);
      break;
    case ChassisLocationType02:
      String = (CHAR16*)PcdGetPtr (PcdBaseBoardLocation);
      break;
    case SerialNumberType03:
      String = (CHAR16*)PcdGetPtr (PcdChassisSerialNumber);
      break;
    case VersionType03:
      String = (CHAR16*)PcdGetPtr (PcdChassisVersion);
      break;
    case ManufacturerType03:
      String = (CHAR16*)PcdGetPtr (PcdChassisManufacturer);
      break;
    case AssetTagType03:
      String = (CHAR16*)PcdGetPtr (PcdChassisAssetTag);
      break;
    case SkuNumberType03:
      String = (CHAR16*)PcdGetPtr (PcdChassisSKU);
      break;
    default:
      String = NULL;
      break;
  }

  if (String != NULL) {
    HiiSetString (HiiHandle, TokenToUpdate, String, NULL);
  }
}

/** Fetches the Type 32 boot information status.

  @return Boot status.
**/
MISC_BOOT_INFORMATION_STATUS_DATA_TYPE
EFIAPI
OemGetBootStatus (
  VOID
  )
{
  return BootInformationStatusNoError;
}

/** Fetches the chassis status when it was last booted.

 @return Chassis status.
**/
MISC_CHASSIS_STATE
EFIAPI
OemGetChassisBootupState (
  VOID
  )
{
  return ChassisStateSafe;
}

/** Fetches the chassis power supply/supplies status when last booted.

 @return Chassis power supply/supplies status.
**/
MISC_CHASSIS_STATE
EFIAPI
OemGetChassisPowerSupplyState (
  VOID
  )
{
  return ChassisStateSafe;
}

/** Fetches the chassis thermal status when last booted.

 @return Chassis thermal status.
**/
MISC_CHASSIS_STATE
EFIAPI
OemGetChassisThermalState (
  VOID
  )
{
  return ChassisStateSafe;
}

/** Fetches the chassis security status when last booted.

 @return Chassis security status.
**/
MISC_CHASSIS_SECURITY_STATE
EFIAPI
OemGetChassisSecurityStatus (
  VOID
  )
{
  return ChassisSecurityStatusNone;
}

/** Fetches the chassis height in RMUs (Rack Mount Units).

  @return The height of the chassis.
**/
UINT8
EFIAPI
OemGetChassisHeight (
  VOID
  )
{
  return 1U;
}

/** Fetches the number of power cords.

  @return The number of power cords.
**/
UINT8
EFIAPI
OemGetChassisNumPowerCords (
  VOID
  )
{
  return 1;
}
