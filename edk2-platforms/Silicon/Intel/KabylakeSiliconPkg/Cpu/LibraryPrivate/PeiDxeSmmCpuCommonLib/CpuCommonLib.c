/** @file
  CPU Common Lib implementation.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/CpuLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciLib.h>
#include <Library/TimerLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/CpuMailboxLib.h>
#include <Register/Cpuid.h>
#include <Register/Msr.h>
#include <CpuAccess.h>

#include <Library/CpuCommonLib.h>

#define INTERRUPT_VECTOR_NUMBER 256
#define END_OF_TIMETABLE        0x3FF


/**
  Set up flags in CR4 for XMM instruction enabling
**/
VOID
EFIAPI
XmmInit (
  VOID
  )
{
  EFI_CPUID_REGISTER Cpuid;
  UINTN              Cr0;
  UINTN              Cr4;

  ///
  /// Read the CPUID information
  ///
  AsmCpuid (CPUID_VERSION_INFO, &Cpuid.RegEax, &Cpuid.RegEbx, &Cpuid.RegEcx, &Cpuid.RegEdx);

  ///
  /// Check whether SSE2 is supported
  ///
  if (Cpuid.RegEdx & BIT26) {
    ///
    /// Enable XMM
    ///
    Cr0 = AsmReadCr0 ();
    Cr0 |= BIT1;
    AsmWriteCr0 (Cr0);

    Cr4 = AsmReadCr4 ();
    Cr4 |= (UINTN) (BIT9 | BIT10);
    AsmWriteCr4 (Cr4);
  }
}

/**
  Enable "Machine Check Enable" bit in Cr4
**/
VOID
EFIAPI
EnableMce (
  VOID
  )
{
  UINTN Cr4;

  ///
  /// Enable MCE
  ///
  Cr4 = AsmReadCr4 ();
  Cr4 |= BIT6;
  AsmWriteCr4 (Cr4);
}

/**
  Mtrr Synch Up Entry
**/
UINTN
EFIAPI
MpMtrrSynchUpEntry (
  VOID
  )
{
  EFI_CPUID_REGISTER Cpuid;
  UINT64 MsrData;
  UINTN  Cr0;
  UINTN  Cr4;

  ///
  /// Read the CPUID and MSR 1Bh information
  ///
  AsmCpuid (CPUID_VERSION_INFO, &Cpuid.RegEax, &Cpuid.RegEbx, &Cpuid.RegEcx, &Cpuid.RegEdx);
  MsrData = AsmReadMsr64 (MSR_IA32_APIC_BASE);

  ///
  /// Set CD(Bit30) bit and clear NW(Bit29) bit of CR0 followed by a WBINVD.
  ///
  if (!(Cpuid.RegEdx & BIT24) || (MsrData & BIT8)) {
    AsmDisableCache ();
  } else {
    ///
    /// We could bypass the wbinvd by
    /// checking MSR 1Bh(MSR_IA32_APIC_BASE) Bit8 (1 = BSP, 0 = AP) to see if we're the BSP?
    /// and checking CPUID if the processor support self-snooping.
    ///
    Cr0 = AsmReadCr0 ();
    Cr0 &= (UINTN) ~BIT29;
    Cr0 |= BIT30;
    AsmWriteCr0 (Cr0);
  }

  ///
  /// Clear PGE flag Bit 7
  ///
  Cr4 = AsmReadCr4 ();
  Cr4 &= (UINTN) ~BIT7;
  AsmWriteCr4 (Cr4);

  ///
  /// Flush all TLBs
  ///
  CpuFlushTlb ();

  return Cr4;
}

/**
  Mtrr Synch Up Exit
**/
VOID
EFIAPI
MpMtrrSynchUpExit (
  UINTN Cr4
  )
{
  UINTN  Cr0;

  ///
  /// Flush all TLBs the second time
  ///
  CpuFlushTlb ();

  ///
  /// Clear both the CD and NW bits of CR0.
  ///
  Cr0 = AsmReadCr0 ();
  Cr0 &= (UINTN) ~(BIT29 | BIT30);
  AsmWriteCr0 (Cr0);

  ///
  /// Set PGE Flag in CR4 if set
  ///
  AsmWriteCr4 (Cr4);
}

/**
  This procedure sends an IPI to the designated processor in
  the requested delivery mode with the requested vector.

  @param[in] ApicID          - APIC ID of processor.
  @param[in] VectorNumber    - Vector number.
  @param[in] DeliveryMode    - I/O APIC Interrupt Deliver Modes

  @retval EFI_INVALID_PARAMETER - Input paramters were not correct.
  @retval EFI_NOT_READY         - There was a pending interrupt
  @retval EFI_SUCCESS           - Interrupt sent successfully
**/
EFI_STATUS
EFIAPI
CpuSendIpi (
  IN UINT32            ApicID,
  IN UINTN             VectorNumber,
  IN UINTN             DeliveryMode
  )
{
  MSR_IA32_APIC_BASE_REGISTER Msr;
  EFI_PHYSICAL_ADDRESS        ApicBase;
  UINT32                      IcrLow;
  UINT32                      IcrHigh;
  BOOLEAN                     XapicEnabled;
  UINT32                      TriggerMode;

  ///
  /// Check for valid input parameters.
  ///
  if (VectorNumber >= INTERRUPT_VECTOR_NUMBER) {
    return EFI_INVALID_PARAMETER;
  }

  if (DeliveryMode >= DELIVERY_MODE_MAX) {
    return EFI_INVALID_PARAMETER;
  }

  ///
  /// Fix the vector number for special interrupts like SMI and INIT.
  ///
  if (DeliveryMode == DELIVERY_MODE_SMI || DeliveryMode == DELIVERY_MODE_INIT) {
    VectorNumber = 0x0;
  }

  ///
  /// Initialze ICR high dword, since P6 family processor needs
  /// the destination field to be 0x0F when it is a broadcast
  ///
  IcrHigh = 0x0f000000;
  IcrLow  = (UINT32) (VectorNumber | (LShiftU64 (DeliveryMode, 8)));

  TriggerMode = TRIGGER_MODE_EDGE;

  ///
  /// Interrupt trigger mode
  ///
  if (TriggerMode == TRIGGER_MODE_LEVEL) {
    IcrLow |= 0x8000;
  }

  ///
  /// Interrupt pin polarity
  ///
  IcrLow |= 0x4000;

  ///
  /// xAPIC Enabled
  ///
  Msr.Uint64 = AsmReadMsr64 (MSR_IA32_APIC_BASE);
  XapicEnabled = (BOOLEAN) ((Msr.Bits.EXTD == 1) && (Msr.Bits.EN == 1));

  if (XapicEnabled) {
    IcrHigh = (UINT32) ApicID;
  } else {
    IcrHigh = (UINT32) LShiftU64 (ApicID, 24);
  }

  ApicBase    = Msr.Uint64 & 0xffffff000;

  /* If Extended XAPIC Mode is enabled,
     legacy xAPIC is no longer working.
     So, previous MMIO offset must be transferred to MSR offset R/W.
     ----------------------------------------------------------------
     MMIO Offset     MSR Offset     Register Name
     ----------------------------------------------------------------
     300h-310h        830h         Interrupt Command Register [63:0]
                      831h         [Reserved]
     ----------------------------------------------------------------
  */
  ///
  /// To write APIC register by MSR or MMIO
  ///
  if (XapicEnabled) {
    AsmWriteMsr64 (MSR_IA32_X2APIC_ICR, (((UINT64) LShiftU64 (IcrHigh, 32)) | (UINT64) IcrLow));
  } else {
    *(volatile UINT32 *) (UINTN) (ApicBase  + APIC_REGISTER_ICR_HIGH_OFFSET) = (UINT32) IcrHigh;
    *(volatile UINT32 *) (UINTN) (ApicBase  + APIC_REGISTER_ICR_LOW_OFFSET) = (UINT32) IcrLow;
  }

  MicroSecondDelay (10);

  ///
  /// To get APIC register from MSR or MMIO
  ///
  if (XapicEnabled) {
    IcrLow = (UINT32) AsmReadMsr64 (MSR_IA32_X2APIC_ICR);
  } else {
    IcrLow = (UINT32) *(volatile UINT32 *) (UINTN) (ApicBase + APIC_REGISTER_ICR_LOW_OFFSET);
  }

  if (IcrLow & BIT12) {
    return EFI_NOT_READY;
  }

  MicroSecondDelay (100);

  return EFI_SUCCESS;
}


/**
  Get APIC ID of processor

  @retval APIC ID of processor
**/
UINT32
GetCpuApicId (
  VOID
  )
{
  EFI_CPUID_REGISTER CpuidRegisters;

  AsmCpuid (
    CPUID_VERSION_INFO,
    &CpuidRegisters.RegEax,
    &CpuidRegisters.RegEbx,
    &CpuidRegisters.RegEcx,
    &CpuidRegisters.RegEdx
    );
  return (UINT32) (CpuidRegisters.RegEbx >> 24);
}

/**
  Programs XAPIC registers.

  @param[in] Bsp             - Is this BSP?
**/
VOID
ProgramXApic (
  BOOLEAN Bsp
  )
{
  UINT64               ApicBaseReg;
  EFI_PHYSICAL_ADDRESS ApicBase;
  volatile UINT32      *EntryAddress;
  UINT32               EntryValue;

  ApicBaseReg = AsmReadMsr64 (MSR_IA32_APIC_BASE);
  ApicBase    = ApicBaseReg & 0xffffff000ULL;

  ///
  /// Program the spurious vector entry
  ///
  EntryAddress  = (UINT32 *) (UINTN) (ApicBase + APIC_REGISTER_SPURIOUS_VECTOR_OFFSET);
  EntryValue    = *EntryAddress;
  EntryValue &= 0xFFFFFD0F;
  EntryValue |= 0x10F;
  *EntryAddress = EntryValue;

  ///
  /// Program the LINT1 vector entry as extINT
  ///
  EntryAddress  = (UINT32 *) (UINTN) (ApicBase + APIC_REGISTER_LINT0_VECTOR_OFFSET);
  EntryValue    = *EntryAddress;

  if (Bsp) {
    EntryValue &= 0xFFFE00FF;
    EntryValue |= 0x700;
  } else {
    EntryValue |= 0x10000;
  }

  *EntryAddress = EntryValue;

  ///
  /// Program the LINT1 vector entry as NMI
  ///
  EntryAddress  = (UINT32 *) (UINTN) (ApicBase + APIC_REGISTER_LINT1_VECTOR_OFFSET);
  EntryValue    = *EntryAddress;
  EntryValue &= 0xFFFE00FF;
  if (Bsp) {
    EntryValue |= 0x400;
  } else {
    EntryValue |= 0x10400;
  }

  *EntryAddress = EntryValue;
}

/**
  This function is to disable BIOS Write Protect in SMM phase.
**/
VOID
EFIAPI
CpuSmmDisableBiosWriteProtect (
  VOID
  )
{
  UINT32      Data32;

  ///
  /// Read memory location FED30880h OR with 00000001h, place the result in EAX,
  /// and write data to lower 32 bits of MSR 1FEh (sample code available)
  ///
  Data32 = MmioRead32 ((UINTN) (0xFED30880)) | (UINT32) (BIT0);
  AsmWriteMsr32 (0x000001FE, Data32);
}

/**
  This function is to enable BIOS Write Protect in SMM phase.
**/
VOID
EFIAPI
CpuSmmEnableBiosWriteProtect (
  VOID
  )
{
  UINT32      Data32;

  ///
  /// Read memory location FED30880h AND with FFFFFFFEh, place the result in EAX,
  /// and write data to lower 32 bits of MSR 1FEh (sample code available)
  ///
  Data32 = MmioRead32 ((UINTN) (0xFED30880)) & (UINT32) (~BIT0);
  AsmWriteMsr32 (0x000001FE, Data32);
}

/**
  This function returns the maximum number of cores supported in this physical processor package.

  @retval Maximum number of supported cores in the package.
**/
UINT8
GetMaxSupportedCoreCount (
  VOID
  )
{
  EFI_CPUID_REGISTER Cpuid;
  AsmCpuidEx (
    4,
    0,
    &Cpuid.RegEax,
    NULL,
    NULL,
    NULL
    );
  return (UINT8) (RShiftU64 (Cpuid.RegEax, 26) & 0x3f) + 1;
}

/**
  This function returns the actual factory-configured number of threads per core,
  and actual factory-configured number of cores in this physical processor package.

  @param[out] *ThreadsPerCore    - variable that will store Maximum enabled threads per core
  @param[out] *NumberOfCores     - variable that will store Maximum enabled cores per die
**/
VOID
GetSupportedCount (
  OUT UINT16 *ThreadsPerCore,  OPTIONAL
  OUT UINT16 *NumberOfCores    OPTIONAL
  )
{
  EFI_CPUID_REGISTER    CpuidRegs;
  UINT16                Threads;

  AsmCpuidEx (CPUID_EXTENDED_TOPOLOGY, 0, NULL, &CpuidRegs.RegEbx, NULL, NULL);
  Threads = (UINT16) CpuidRegs.RegEbx;

  if (ThreadsPerCore != NULL) {
    *ThreadsPerCore = Threads;
  }

  if (NumberOfCores != NULL) {
    AsmCpuidEx (CPUID_EXTENDED_TOPOLOGY, 1, NULL, &CpuidRegs.RegEbx, NULL, NULL);
    *NumberOfCores = (UINT16) (CpuidRegs.RegEbx / Threads);
  }
}


/**
  Check to see if the executing thread is BSP

  @retval TRUE   Executing thread is BSP
  @retval FALSE  Executing thread is AP
**/
BOOLEAN
IsBsp (
  VOID
  )
{
  MSR_IA32_APIC_BASE_REGISTER Msr;

  Msr.Uint64  = AsmReadMsr64 (MSR_IA32_APIC_BASE);
  return (BOOLEAN) (Msr.Bits.BSP == 1);
}


BOOLEAN
IsPrmrrAlreadySet (
  VOID
  )
{
  return FALSE;
}

/**
  Check if this is non-core processor - HT AP thread

  @retval TRUE if this is HT AP thread
  @retval FALSE if this is core thread
**/
BOOLEAN
IsSecondaryThread (
  VOID
  )
{
  UINT32             ApicID;
  EFI_CPUID_REGISTER CpuidRegisters;
  UINT8              CpuCount;
  UINT8              CoreCount;
  UINT8              CpuPerCore;
  UINT32             Mask;

  ApicID = GetCpuApicId ();

  AsmCpuid (
    CPUID_VERSION_INFO,
    &CpuidRegisters.RegEax,
    &CpuidRegisters.RegEbx,
    &CpuidRegisters.RegEcx,
    &CpuidRegisters.RegEdx
    );
  if ((CpuidRegisters.RegEdx & 0x10000000) == 0) {
    return FALSE;
  }

  CpuCount = (UINT8) ((CpuidRegisters.RegEbx >> 16) & 0xff);
  if (CpuCount == 1) {
    return FALSE;
  }

  AsmCpuid (
    CPUID_SIGNATURE,
    &CpuidRegisters.RegEax,
    &CpuidRegisters.RegEbx,
    &CpuidRegisters.RegEcx,
    &CpuidRegisters.RegEdx
    );
  if (CpuidRegisters.RegEax > 3) {

    CoreCount = GetMaxSupportedCoreCount ();
  } else {
    CoreCount = 1;
  }
  ///
  /// Assumes there is symmetry across core boundary, i.e. each core within a package has the same number of logical processors
  ///
  if (CpuCount == CoreCount) {
    return FALSE;
  }

  CpuPerCore = CpuCount / CoreCount;

  ///
  /// Assume 1 Core  has no more than 8 threads
  ///
  if (CpuPerCore == 2) {
    Mask = 0x1;
  } else if (CpuPerCore <= 4) {
    Mask = 0x3;
  } else {
    Mask = 0x7;
  }

  if ((ApicID & Mask) == 0) {
    return FALSE;
  } else {
    return TRUE;
  }
}

