/*++

Copyright (c) 2006  - 2020, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

  MiscProcessorCacheFunction.c

Abstract:

  BIOS processor cache details.
  Misc. subclass type 7.
  SMBIOS type 7.

--*/
#include "CommonHeader.h"
#include "MiscSubclassDriver.h"
#include <Register/Cpuid.h>

MISC_SMBIOS_TABLE_FUNCTION(MiscProcessorCache)
{
	EFI_SMBIOS_HANDLE     SmbiosHandle;
	SMBIOS_TABLE_TYPE7            *SmbiosRecord;

	EFI_CACHE_SRAM_TYPE_DATA      CacheSramType;
	CHAR16                          *SocketDesignation;
	CHAR8                           *OptionalStrStart;
	UINTN                           SocketStrLen;
	STRING_REF                      TokenToGet;
  UINT32                          SubIndex;
  CPUID_CACHE_PARAMS_EAX          CacheParamsEax;
  CPUID_CACHE_PARAMS_EBX          CacheParamsEbx;
  UINT32                          RegisterEcx;
  CPUID_CACHE_PARAMS_EDX          CacheParamsEdx;
  UINT8                           SystemCacheType;
  UINTN                           Size;

	//
	// Memory Device Locator
	//
	TokenToGet = STRING_TOKEN (STR_SOCKET_DESIGNATION);
	SocketDesignation = SmbiosMiscGetString (TokenToGet);
	SocketStrLen = StrLen(SocketDesignation);
	if (SocketStrLen > SMBIOS_STRING_MAX_LENGTH) {
	  return EFI_UNSUPPORTED;
	}

  //
  // Retrieve cache level information using CPUID
  //
  for (SubIndex = 0; ; SubIndex++) {
    AsmCpuidEx (
      CPUID_CACHE_PARAMS,
      SubIndex,
      &CacheParamsEax.Uint32,
      &CacheParamsEbx.Uint32,
      &RegisterEcx,
      &CacheParamsEdx.Uint32
      );
    //
    // Terminate loop when CacheType is CPUID_CACHE_PARAMS_CACHE_TYPE_NULL
    //
    if (CacheParamsEax.Bits.CacheType == CPUID_CACHE_PARAMS_CACHE_TYPE_NULL) {
      break;
    }

    //
    // Allocate and zero SMBIOS TYPE 7 Record
    //
	  SmbiosRecord = AllocateZeroPool (sizeof (SMBIOS_TABLE_TYPE7) + 7 + 1 + 1);
	  ASSERT (SmbiosRecord != NULL);

    //
    // Compute cache size in bytes
    //
    Size = (CacheParamsEbx.Bits.Ways + 1) *
           (CacheParamsEbx.Bits.LinePartitions + 1) *
           (CacheParamsEbx.Bits.LineSize + 1) *
           (RegisterEcx + 1);
    DEBUG ((DEBUG_INFO, "MiscProcessorCache(): Cache Type = %d  Cache Level = %d  Size = %x\n", CacheParamsEax.Bits.CacheType, CacheParamsEax.Bits.CacheLevel, Size));

    //
    // Determine SMBIOS SystemCacheType
    //
    switch (CacheParamsEax.Bits.CacheType) {
    case 1:
      SystemCacheType = CacheTypeData;
      break;
    case 2:
      SystemCacheType = CacheTypeInstruction;
      break;
    case 3:
      SystemCacheType = CacheTypeUnified;
      break;
    default:
      SystemCacheType = CacheTypeUnknown;
    }

    //
    // Update cache sizes in KB
    //
    switch (CacheParamsEax.Bits.CacheLevel) {
    case 1:
      SmbiosRecord->InstalledSize      = (UINT16)(Size >> 10);
      SmbiosRecord->MaximumCacheSize   = SmbiosRecord->InstalledSize;
      SmbiosRecord->SystemCacheType    = SystemCacheType;
      SmbiosRecord->Associativity      = CacheAssociativity8Way;
      SmbiosRecord->CacheConfiguration = 0x0180;
      break;
    case 2:
      SmbiosRecord->InstalledSize      = (UINT16)(Size >> 10);
      SmbiosRecord->MaximumCacheSize   = SmbiosRecord->InstalledSize;
      SmbiosRecord->SystemCacheType    = SystemCacheType;
      SmbiosRecord->Associativity      = CacheAssociativity16Way;
      SmbiosRecord->CacheConfiguration = 0x0281;
	    //
	    //VLV2 incorporates two SLM modules (quad cores) in the SoC.
      // 2 cores share BIU/L2 cache
	    //
	    SmbiosRecord->InstalledSize    = SmbiosRecord->InstalledSize / 2;
	    SmbiosRecord->MaximumCacheSize = SmbiosRecord->InstalledSize;
      break;
    default:
      DEBUG ((DEBUG_ERROR, "MiscProcessorCache(): Unexpected cache level %d\n", CacheParamsEax.Bits.CacheLevel));
      break;
    }

	  //
	  //Filling SMBIOS type 7 information for different cache levels.
	  //
	  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_CACHE_INFORMATION;
	  SmbiosRecord->Hdr.Length = (UINT8) sizeof (SMBIOS_TABLE_TYPE7);
	  SmbiosRecord->Hdr.Handle = 0;

	  SmbiosRecord->SocketDesignation = 0x01;
	  SmbiosRecord->CacheSpeed = 0;
	  ZeroMem (&CacheSramType, sizeof (EFI_CACHE_SRAM_TYPE_DATA));
	  CacheSramType.Synchronous = 1;
	  CopyMem(&SmbiosRecord->SupportedSRAMType, &CacheSramType, 2);
	  CopyMem(&SmbiosRecord->CurrentSRAMType, &CacheSramType, 2);
	  SmbiosRecord->ErrorCorrectionType = EfiCacheErrorSingleBit;

	  //
	  // Adding SMBIOS type 7 records to SMBIOS table.
	  //
	  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
	  OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
	  UnicodeStrToAsciiStrS (SocketDesignation, OptionalStrStart,  + 7 + 1 + 1);

	  Smbios->Add(
	            Smbios,
	            NULL,
	            &SmbiosHandle,
	            (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord
	            );
  }
	return EFI_SUCCESS;
}
