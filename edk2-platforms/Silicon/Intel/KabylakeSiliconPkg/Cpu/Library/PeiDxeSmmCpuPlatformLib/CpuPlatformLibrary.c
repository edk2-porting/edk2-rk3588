/** @file
  CPU Platform Lib implementation.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "CpuPlatformLibrary.h"
#include <Library/MmPciLib.h>
#include <SaRegs.h>

#define SKIP_MICROCODE_CHECKSUM_CHECK 1

/**
  Return CPU Family ID

  @retval CPU_FAMILY              CPU Family ID
**/
CPU_FAMILY
EFIAPI
GetCpuFamily (
  VOID
  )
{
  EFI_CPUID_REGISTER Cpuid;
  ///
  /// Read the CPUID information
  ///
  AsmCpuid (CPUID_VERSION_INFO, &Cpuid.RegEax, &Cpuid.RegEbx, &Cpuid.RegEcx, &Cpuid.RegEdx);
  return ((CPU_FAMILY) (Cpuid.RegEax & CPUID_FULL_FAMILY_MODEL));
}

/**
  Return Cpu stepping type

  @retval UINT8                   Cpu stepping type
**/
CPU_STEPPING
EFIAPI
GetCpuStepping (
  VOID
  )
{
  EFI_CPUID_REGISTER Cpuid;
  ///
  /// Read the CPUID information
  ///
  AsmCpuid (CPUID_VERSION_INFO, &Cpuid.RegEax, &Cpuid.RegEbx, &Cpuid.RegEcx, &Cpuid.RegEdx);
  return ((CPU_STEPPING) (Cpuid.RegEax & CPUID_FULL_STEPPING));
}

/**
  Return CPU Sku

  @retval UINT8              CPU Sku
**/
UINT8
EFIAPI
GetCpuSku (
  VOID
  )
{
  UINT8              CpuType;
  UINT16             CpuDid;
  UINT32             CpuFamilyModel;
  EFI_CPUID_REGISTER Cpuid;
  BOOLEAN            SkuFound;

  SkuFound  = TRUE;
  CpuType   = EnumCpuUnknown;

  ///
  /// Read the CPUID & DID information
  ///
  AsmCpuid (CPUID_VERSION_INFO, &Cpuid.RegEax, &Cpuid.RegEbx, &Cpuid.RegEcx, &Cpuid.RegEdx);
  CpuFamilyModel = Cpuid.RegEax & CPUID_FULL_FAMILY_MODEL;
  CpuDid = MmioRead16 (MmPciBase (0, 0, 0) + 2);

  switch (CpuFamilyModel) {
    case CPUID_FULL_FAMILY_MODEL_SKYLAKE_ULT_ULX:
      switch (CpuDid) {
        case V_SA_DEVICE_ID_SKL_MB_ULT_1:    // ULT OPI
        case V_SA_DEVICE_ID_KBL_MB_ULT_1:    // KBL ULT OPI
          CpuType = EnumCpuUlt;
          break;

        case V_SA_DEVICE_ID_SKL_MB_ULX_2:    // ULX OPI
        case V_SA_DEVICE_ID_SKL_MB_ULX_3:    // ULX OPI
        case V_SA_DEVICE_ID_KBL_MB_ULX_1:    // ULX OPI
          CpuType = EnumCpuUlx;
          break;

        default:
          SkuFound = FALSE;
          break;
      }
      break;

    case CPUID_FULL_FAMILY_MODEL_SKYLAKE_DT_HALO:
      switch (CpuDid) {

        case V_SA_DEVICE_ID_SKL_DT_1:      // DT
        case V_SA_DEVICE_ID_SKL_DT_2:      // DT
        case V_SA_DEVICE_ID_KBL_DT_1:      // DT
        case V_SA_DEVICE_ID_KBL_DT_2:      // DT
          CpuType = EnumCpuTrad;
          break;

        case V_SA_DEVICE_ID_SKL_HALO_1:    // Halo
        case V_SA_DEVICE_ID_SKL_HALO_2:    // Halo
        case V_SA_DEVICE_ID_KBL_HALO_1:    // Halo
        case V_SA_DEVICE_ID_KBL_HALO_2:    // Halo
        case V_SA_DEVICE_ID_SKL_SVR_1:     // Server
        case V_SA_DEVICE_ID_SKL_SVR_2:     // Server
        case V_SA_DEVICE_ID_KBL_SVR_1:     // Server
        case V_SA_DEVICE_ID_KBL_SVR_2:     // Server
          CpuType = EnumCpuHalo;
          break;

        default:
          SkuFound = FALSE;
          break;
      }
      break;
    case CPUID_FULL_FAMILY_MODEL_KABYLAKE_ULT_ULX:
      switch (CpuDid) {
        case V_SA_DEVICE_ID_KBL_MB_ULT_1:    // KBL ULT OPI
        case V_SA_DEVICE_ID_KBLR_MB_ULT_1:   // KBL-R ULT
          CpuType = EnumCpuUlt;
          break;

        case V_SA_DEVICE_ID_KBL_MB_ULX_1:    // ULX OPI
          CpuType = EnumCpuUlx;
          break;

        default:
          SkuFound = FALSE;
          break;
      }
      break;

    case CPUID_FULL_FAMILY_MODEL_KABYLAKE_DT_HALO:
      switch (CpuDid) {

        case V_SA_DEVICE_ID_KBL_DT_1:      // DT
        case V_SA_DEVICE_ID_KBL_DT_2:      // DT
        case V_SA_DEVICE_ID_CFL_DT:        // DT
          CpuType = EnumCpuTrad;
          break;

        case V_SA_DEVICE_ID_KBL_HALO_1:    // Halo
        case V_SA_DEVICE_ID_KBL_HALO_2:    // Halo
        case V_SA_DEVICE_ID_KBL_SVR_1:     // Server
        case V_SA_DEVICE_ID_KBL_SVR_2:     // Server
          CpuType = EnumCpuHalo;
          break;

        default:
          SkuFound = FALSE;
          break;
      }
      break;

    default:
      SkuFound = FALSE;
      break;
  }

  if (!SkuFound) {
    DEBUG ((DEBUG_ERROR, "Unsupported CPU SKU, Device ID: 0x%02X, CPUID: 0x%08X!\n", CpuDid, CpuFamilyModel));
    ASSERT (FALSE);
  }

  return CpuType;
}


/**
  Returns the processor microcode revision of the processor installed in the system.

  @retval Processor Microcode Revision
**/
UINT32
GetCpuUcodeRevision (
  VOID
  )
{
  AsmWriteMsr64 (MSR_IA32_BIOS_SIGN_ID, 0);
  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, NULL, NULL);
  return (UINT32) RShiftU64 (AsmReadMsr64 (MSR_IA32_BIOS_SIGN_ID), 32);
}

/**
  Verify the DWORD type checksum

  @param[in] ChecksumAddr  - The start address to be checkumed
  @param[in] ChecksumLen   - The length of data to be checksumed

  @retval EFI_SUCCESS           - Checksum correct
  @retval EFI_CRC_ERROR         - Checksum incorrect
**/
EFI_STATUS
Checksum32Verify (
  IN UINT32 *ChecksumAddr,
  IN UINT32 ChecksumLen
  )
{
#if SKIP_MICROCODE_CHECKSUM_CHECK
  return EFI_SUCCESS;
#else
  UINT32 Checksum;
  UINT32 Index;

  Checksum = 0;

  for (Index = 0; Index < ChecksumLen; Index++) {
    Checksum += ChecksumAddr[Index];
  }

  return (Checksum == 0) ? EFI_SUCCESS : EFI_CRC_ERROR;
#endif
}

/**
  This function checks the MCU revision to decide if BIOS needs to load
  microcode.

  @param[in] MicrocodePointer - Microcode in memory
  @param[in] Revision         - Current CPU microcode revision

  @retval EFI_SUCCESS - BIOS needs to load microcode
  @retval EFI_ABORTED - Don't need to update microcode
**/
EFI_STATUS
CheckMcuRevision (
  IN CPU_MICROCODE_HEADER *MicrocodePointer,
  IN UINT32               Revision
  )
{
  EFI_STATUS Status;
  Status = EFI_ABORTED;

  if ((MicrocodePointer->UpdateRevision & 0x80000000) ||
      (MicrocodePointer->UpdateRevision > Revision) ||
      (Revision == 0)) {
    Status = EFI_SUCCESS;
  }

  return Status;
}

/**
  Check if this microcode is correct one for processor

  @param[in] Cpuid               - processor CPUID
  @param[in] MicrocodeEntryPoint - entry point of microcode
  @param[in] Revision            - revision of microcode

  @retval CorrectMicrocode if this microcode is correct
**/
BOOLEAN
CheckMicrocode (
  IN UINT32               Cpuid,
  IN CPU_MICROCODE_HEADER *MicrocodeEntryPoint,
  IN UINT32               *Revision
  )
{
  EFI_STATUS                          Status;
  UINT8                               ExtendedIndex;
  MSR_IA32_PLATFORM_ID_REGISTER       Msr;
  UINT32                              ExtendedTableLength;
  UINT32                              ExtendedTableCount;
  BOOLEAN                             CorrectMicrocode;
  CPU_MICROCODE_EXTENDED_TABLE        *ExtendedTable;
  CPU_MICROCODE_EXTENDED_TABLE_HEADER *ExtendedTableHeader;

  Status              = EFI_NOT_FOUND;
  ExtendedTableLength = 0;
  CorrectMicrocode    = FALSE;

  if (MicrocodeEntryPoint == NULL) {
    return FALSE;
  }

  Msr.Uint64 = AsmReadMsr64 (MSR_IA32_PLATFORM_ID);

  ///
  /// Check if the microcode is for the Cpu and the version is newer
  /// and the update can be processed on the platform
  ///
  if ((MicrocodeEntryPoint->HeaderVersion == 0x00000001) &&
      !EFI_ERROR (CheckMcuRevision (MicrocodeEntryPoint, *Revision))
      ) {
    if ((MicrocodeEntryPoint->ProcessorId == Cpuid) && (MicrocodeEntryPoint->ProcessorFlags & (1 << (UINT8) Msr.Bits.PlatformId))) {
      if (MicrocodeEntryPoint->DataSize == 0) {
        Status = Checksum32Verify ((UINT32 *) MicrocodeEntryPoint, 2048 / sizeof (UINT32));
      } else {
        Status = Checksum32Verify (
                   (UINT32 *) MicrocodeEntryPoint,
                   (MicrocodeEntryPoint->DataSize + sizeof (CPU_MICROCODE_HEADER)) / sizeof (UINT32)
                   );
      }

      if (!EFI_ERROR (Status)) {
        CorrectMicrocode = TRUE;
      }
    } else if ((MicrocodeEntryPoint->DataSize != 0)) {
      ///
      /// Check the  Extended Signature if the entended signature exist
      /// Only the data size != 0 the extended signature may exist
      ///
      ExtendedTableLength = MicrocodeEntryPoint->TotalSize - (MicrocodeEntryPoint->DataSize + sizeof (CPU_MICROCODE_HEADER));
      if (ExtendedTableLength != 0) {
        ///
        /// Extended Table exist, check if the CPU in support list
        ///
        ExtendedTableHeader = (CPU_MICROCODE_EXTENDED_TABLE_HEADER *) ((UINT8 *) (MicrocodeEntryPoint) + MicrocodeEntryPoint->DataSize + 48);
        ///
        /// Calulate Extended Checksum
        ///
        if ((ExtendedTableLength % 4) == 0) {
          Status = Checksum32Verify ((UINT32 *) ExtendedTableHeader, ExtendedTableLength / sizeof (UINT32));
          if (!EFI_ERROR (Status)) {
            ///
            /// Checksum correct
            ///
            ExtendedTableCount  = ExtendedTableHeader->ExtendedSignatureCount;
            ExtendedTable       = (CPU_MICROCODE_EXTENDED_TABLE *) (ExtendedTableHeader + 1);
            for (ExtendedIndex = 0; ExtendedIndex < ExtendedTableCount; ExtendedIndex++) {
              ///
              /// Verify Header
              ///
              if ((ExtendedTable->ProcessorSignature == Cpuid) && (ExtendedTable->ProcessorFlag & (1 << (UINT8) Msr.Bits.PlatformId))) {
                Status = Checksum32Verify (
                           (UINT32 *) ExtendedTable,
                           sizeof (CPU_MICROCODE_EXTENDED_TABLE) / sizeof (UINT32)
                           );
                if (!EFI_ERROR (Status)) {
                  ///
                  /// Find one
                  ///
                  CorrectMicrocode = TRUE;
                  break;
                }
              }

              ExtendedTable++;
            }
          }
        }
      }
    }
  }

  return CorrectMicrocode;
}


/**
  Check on the processor if SGX is supported.

  @dot
    digraph G {
      subgraph cluster_c0 {
        node [shape = box];
          b1[label="Read CPUID(EAX=7,ECX=0):EBX[2] \nto check SGX feature" fontsize=12 style=filled color=lightblue];
          b2[label="Return TRUE" fontsize=12 style=filled color=lightblue];
          b3[label="Return FALSE" fontsize=12 style=filled color=lightblue];

        node [shape = ellipse];
          e1[label="Start" fontsize=12 style=filled color=lightblue];
          e2[label="End" fontsize=12 style=filled color=lightblue];

        node [shape = diamond,style=filled,color=lightblue];
          d1[label="Are SGX feature supported and \nPRMRR configuration enabled" fontsize=12];

        label = "IsSgxSupported Flow"; fontsize=15; fontcolor=black; color=lightblue;
        e1 -> b1
        b1 -> d1
        d1 -> b2 [label="Yes" fontsize=9]
        d1 -> b3 [label="No" fontsize=9]
        b2 -> e2
        b3 -> e2

      }
    }
  @enddot

  @retval TRUE  if SGX supported
  @retval FALSE if SGX is not supported
**/
BOOLEAN
IsSgxSupported (
  VOID
  )
{
  EFI_CPUID_REGISTER CpuidRegs;

  //
  // Processor support SGX feature by reading CPUID.(EAX=7,ECX=0):EBX[2]
  //
  AsmCpuidEx (CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS, 0, &CpuidRegs.RegEax,&CpuidRegs.RegEbx,&CpuidRegs.RegEcx,&CpuidRegs.RegEdx);

  ///
  /// SGX feature is supported only on SKL and later,
  /// with CPUID.(EAX=7,ECX=0):EBX[2]=1
  /// PRMRR configuration enabled, MSR IA32_MTRRCAP (FEh) [12] == 1
  ///
  if (((CpuidRegs.RegEbx & BIT2)) && (AsmReadMsr64 (MSR_IA32_MTRRCAP) & BIT12)) {
    return TRUE;
  }
  return FALSE;
}

/**
  Get processor generation

  @retval EnumSklCpu    Executing thread is Skylake
  @retval EnumKblCpu    Executing thread is Kabylake
**/
CPU_GENERATION
GetCpuGeneration (
  VOID
  )
{
  EFI_CPUID_REGISTER Cpuid;
  CPU_FAMILY         CpuFamilyModel;
  CPU_STEPPING       CpuStepping;
  CPU_GENERATION     CpuGeneration;

  CpuGeneration = EnumSklCpu;
  ///
  /// Read the CPUID information
  ///
  AsmCpuid (CPUID_VERSION_INFO, &Cpuid.RegEax, &Cpuid.RegEbx, &Cpuid.RegEcx, &Cpuid.RegEdx);
  CpuFamilyModel = (CPU_FAMILY) (Cpuid.RegEax & CPUID_FULL_FAMILY_MODEL);
  CpuStepping    = (CPU_STEPPING) (Cpuid.RegEax & CPUID_FULL_STEPPING);

  switch (CpuFamilyModel) {
    case EnumCpuKblUltUlx:
    case EnumCpuKblDtHalo:
      CpuGeneration = EnumKblCpu;
      break;

    case EnumCpuSklUltUlx:
    case EnumCpuSklDtHalo:
      if (((CpuStepping > EnumSklMaxUltUlxStep) && (CpuStepping <= EnumKblMaxUltUlxStep)) ||
          ((CpuStepping > EnumSklMaxDtHaloStep) && (CpuStepping <= EnumKblMaxDtHaloStep))) {
        CpuGeneration = EnumKblCpu;
      }
      break;

    default:
      CpuGeneration = EnumSklCpu;
      break;
  }

  return CpuGeneration;
}
