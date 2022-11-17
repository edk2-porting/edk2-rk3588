/** @file
  Copyright (c) 2006 - 2019, Intel Corporation. All rights reserved.<BR>
  Copyright (c) 2011, Andrei Warkentin <andreiw@motorola.com>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//
// The package level header files this module uses
//
#include <PiPei.h>

//
// The Library classes this module consumes
//
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PciLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/ResourcePublicationLib.h>
#include <Library/CmosAccessLib.h>
#include <Guid/MemoryTypeInformation.h>
#include <Ppi/MasterBootMode.h>
#include <IndustryStandard/Pci22.h>
#include <SimicsPlatforms.h>

#include "Platform.h"

EFI_MEMORY_TYPE_INFORMATION mDefaultMemoryTypeInformation[] = {
  { EfiACPIMemoryNVS,       0x004 },
  { EfiACPIReclaimMemory,   0x008 },
  { EfiReservedMemoryType,  0x004 },
  { EfiRuntimeServicesData, 0x024 },
  { EfiRuntimeServicesCode, 0x030 },
  { EfiBootServicesCode,    0x180 },
  { EfiBootServicesData,    0xF00 },
  { EfiMaxMemoryType,       0x000 }
};


EFI_PEI_PPI_DESCRIPTOR   mPpiBootMode[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiPeiMasterBootModePpiGuid,
    NULL
  }
};


UINT16 mHostBridgeDevId;

EFI_BOOT_MODE mBootMode = BOOT_WITH_FULL_CONFIGURATION;

BOOLEAN mS3Supported = FALSE;

UINT32 mMaxCpuCount;

VOID
AddIoMemoryBaseSizeHob (
  EFI_PHYSICAL_ADDRESS        MemoryBase,
  UINT64                      MemorySize
  )
{
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
      EFI_RESOURCE_ATTRIBUTE_PRESENT     |
      EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
      EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
      EFI_RESOURCE_ATTRIBUTE_TESTED,
    MemoryBase,
    MemorySize
    );
}

VOID
AddReservedMemoryBaseSizeHob (
  EFI_PHYSICAL_ADDRESS        MemoryBase,
  UINT64                      MemorySize,
  BOOLEAN                     Cacheable
  )
{
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_RESERVED,
      EFI_RESOURCE_ATTRIBUTE_PRESENT     |
      EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
      EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
      (Cacheable ?
       EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
       EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
       EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE :
       0
       ) |
      EFI_RESOURCE_ATTRIBUTE_TESTED,
    MemoryBase,
    MemorySize
    );
}

VOID
AddIoMemoryRangeHob (
  EFI_PHYSICAL_ADDRESS        MemoryBase,
  EFI_PHYSICAL_ADDRESS        MemoryLimit
  )
{
  AddIoMemoryBaseSizeHob (MemoryBase, (UINT64)(MemoryLimit - MemoryBase));
}


VOID
AddMemoryBaseSizeHob (
  EFI_PHYSICAL_ADDRESS        MemoryBase,
  UINT64                      MemorySize
  )
{
  BuildResourceDescriptorHob (
    EFI_RESOURCE_SYSTEM_MEMORY,
      EFI_RESOURCE_ATTRIBUTE_PRESENT |
      EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
      EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
      EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
      EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
      EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |
      EFI_RESOURCE_ATTRIBUTE_TESTED,
    MemoryBase,
    MemorySize
    );
}


VOID
AddMemoryRangeHob (
  EFI_PHYSICAL_ADDRESS        MemoryBase,
  EFI_PHYSICAL_ADDRESS        MemoryLimit
  )
{
  AddMemoryBaseSizeHob (MemoryBase, (UINT64)(MemoryLimit - MemoryBase));
}


VOID
AddUntestedMemoryBaseSizeHob (
  EFI_PHYSICAL_ADDRESS        MemoryBase,
  UINT64                      MemorySize
  )
{
  BuildResourceDescriptorHob (
    EFI_RESOURCE_SYSTEM_MEMORY,
      EFI_RESOURCE_ATTRIBUTE_PRESENT |
      EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
      EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
      EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
      EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
      EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE,
    MemoryBase,
    MemorySize
    );
}


VOID
AddUntestedMemoryRangeHob (
  EFI_PHYSICAL_ADDRESS        MemoryBase,
  EFI_PHYSICAL_ADDRESS        MemoryLimit
  )
{
  AddUntestedMemoryBaseSizeHob (MemoryBase, (UINT64)(MemoryLimit - MemoryBase));
}

VOID
AddFlashDeviceRange (
  EFI_PHYSICAL_ADDRESS        MemoryBase,
  UINT64                      MemorySize
  )
{
  BuildResourceDescriptorHob (
    EFI_RESOURCE_FIRMWARE_DEVICE,
    (EFI_RESOURCE_ATTRIBUTE_PRESENT    |
      EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
      EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
    MemoryBase,
    MemorySize
    );

  BuildMemoryAllocationHob (
    MemoryBase,
    MemorySize,
    EfiMemoryMappedIO
    );
}

VOID
MemMapInitialization (
  VOID
  )
{
  UINT64 PciIoBase;
  UINT64 PciIoSize;

  UINT32  TopOfLowRam;
  UINT64  PciExBarBase;
  UINT32  PciBase;
  UINT32  PciSize;

  PciIoBase = 0xC000;
  PciIoSize = 0x4000;

  //
  // Create Memory Type Information HOB
  //
  BuildGuidDataHob (
    &gEfiMemoryTypeInformationGuid,
    mDefaultMemoryTypeInformation,
    sizeof(mDefaultMemoryTypeInformation)
    );

  //
  // Video memory + Legacy BIOS region
  //
  AddIoMemoryRangeHob (0x0A0000, BASE_1MB);

  TopOfLowRam = GetSystemMemorySizeBelow4gb ();
  PciExBarBase = 0;
  if (mHostBridgeDevId == INTEL_ICH10_DEVICE_ID) {
    //
    // The MMCONFIG area is expected to fall between the top of low RAM and
    // the base of the 32-bit PCI host aperture.
    //
    PciExBarBase = FixedPcdGet64 (PcdPciExpressBaseAddress);
    ASSERT (TopOfLowRam <= PciExBarBase);
    ASSERT (PciExBarBase <= MAX_UINT32 - SIZE_256MB);
    PciBase = (UINT32)(PciExBarBase + SIZE_256MB);
  } else {
    PciBase = (TopOfLowRam < BASE_2GB) ? BASE_2GB : TopOfLowRam;
  }

  //
  // address       purpose   size
  // ------------  --------  -------------------------
  // 0x00000000    TopOfLowRam   0xDF000000
  // 0xDF000000    Tseg+UMA      0x01000000
  // 0xE0000000    PciExBarBase  0x10000000
  // 0xF0000000    PciBase       0x0C000000
  // -------------------------------------------------
  // max(top, 2g)  PCI MMIO  0xFC000000 - max(top, 2g)
  // 0xFC000000    gap                           44 MB
  // 0xFEC00000    IO-APIC                        4 KB
  // 0xFEC01000    gap                         1020 KB
  // 0xFED00000    HPET                           1 KB
  // 0xFED00400    gap                          111 KB
  // 0xFED1C000    gap (PIIX4) / RCRB (ICH9)     16 KB
  // 0xFED20000    gap                          896 KB
  // 0xFEE00000    LAPIC                          1 MB
  //
  PciSize = 0xFC000000 - PciBase;
  AddIoMemoryBaseSizeHob (PciBase, PciSize);
  PcdSet64S (PcdPciMmio32Base, PciBase);
  PcdSet64S (PcdPciMmio32Size, PciSize);
  AddIoMemoryBaseSizeHob (0xFEC00000, SIZE_4KB);
  AddIoMemoryBaseSizeHob (0xFED00000, SIZE_1KB);
  if (mHostBridgeDevId == INTEL_ICH10_DEVICE_ID) {
    AddIoMemoryBaseSizeHob (ICH10_ROOT_COMPLEX_BASE, SIZE_16KB);
    //
    // Note: there should be an
    //
    //   AddIoMemoryBaseSizeHob (PciExBarBase, SIZE_256MB);
    //   BuildMemoryAllocationHob (PciExBarBase, SIZE_256MB, EfiMemoryMappedIO);
    //
    // call below, just like the one above for RCBA. However, Linux insists
    // that the MMCONFIG area be marked in the E820 or UEFI memory map as
    // "reserved memory" -- Linux does not content itself with a simple gap
    // in the memory map wherever the MCFG ACPI table points to.
    //
    // This appears to be a safety measure. The PCI Firmware Specification
    // (rev 3.1) says in 4.1.2. "MCFG Table Description": "The resources can
    // *optionally* be returned in [...] EFIGetMemoryMap as reserved memory
    // [...]". (Emphasis added here.)
    //
    // Normally we add memory resource descriptor HOBs in
    // QemuInitializeRam(), and pre-allocate from those with memory
    // allocation HOBs in InitializeRamRegions(). However, the MMCONFIG area
    // is most definitely not RAM; so, as an exception, cover it with
    // uncacheable reserved memory right here.
    //
      AddReservedMemoryBaseSizeHob (PciExBarBase, SIZE_256MB, FALSE);
      BuildMemoryAllocationHob (PciExBarBase, SIZE_256MB,
        EfiReservedMemoryType);
  }
  AddIoMemoryBaseSizeHob (PcdGet32(PcdCpuLocalApicBaseAddress), SIZE_1MB);

  // Add PCI IO Port space available for PCI resource allocations.
  //
  BuildResourceDescriptorHob (
    EFI_RESOURCE_IO,
    EFI_RESOURCE_ATTRIBUTE_PRESENT     |
    EFI_RESOURCE_ATTRIBUTE_INITIALIZED,
    PciIoBase,
    PciIoSize
    );
  PcdSet64S (PcdPciIoBase, PciIoBase);
  PcdSet64S (PcdPciIoSize, PciIoSize);

  //
  // Add flash range.
  //
  AddFlashDeviceRange (PcdGet32(PcdFlashAreaBaseAddress), PcdGet32(PcdFlashAreaSize));
  //
  // Video memory / ABSEG
  //
  AddIoMemoryBaseSizeHob (0x0A0000, 0x20000);
  //
  // Legacy BIOS region.
  //
  AddReservedMemoryBaseSizeHob (0xC0000, 0x40000, TRUE);
}

VOID
MiscInitialization (
  VOID
  )
{
  UINTN         PmCmd;
  UINTN         Pmba;
  UINT32        PmbaAndVal;
  UINT32        PmbaOrVal;
  UINTN         AcpiCtlReg;
  UINT8         AcpiEnBit;

  //
  // Disable A20 Mask
  //
  IoOr8 (0x92, BIT1);

  //
  // Build the CPU HOB with guest RAM size dependent address width and 16-bits
  // of IO space. (Side note: unlike other HOBs, the CPU HOB is needed during
  // S3 resume as well, so we build it unconditionally.)
  //
  BuildCpuHob (mPhysMemAddressWidth, 16);

  //
  // Determine platform type and save Host Bridge DID to PCD
  //
  switch (mHostBridgeDevId) {
    case INTEL_82441_DEVICE_ID:
      PmCmd      = POWER_MGMT_REGISTER_PIIX4 (PCI_COMMAND_OFFSET);
      Pmba       = POWER_MGMT_REGISTER_PIIX4 (PIIX4_PMBA);
      PmbaAndVal = ~(UINT32)PIIX4_PMBA_MASK;
      PmbaOrVal  = PIIX4_PMBA_VALUE;
      AcpiCtlReg = POWER_MGMT_REGISTER_PIIX4 (PIIX4_PMREGMISC);
      AcpiEnBit  = PIIX4_PMREGMISC_PMIOSE;
      break;
    case INTEL_ICH10_DEVICE_ID:
      PmCmd      = POWER_MGMT_REGISTER_ICH10 (PCI_COMMAND_OFFSET);
      Pmba       = POWER_MGMT_REGISTER_ICH10 (ICH10_PMBASE);
      PmbaAndVal = ~(UINT32)ICH10_PMBASE_MASK;
      PmbaOrVal  = ICH10_PMBASE_VALUE;
      AcpiCtlReg = POWER_MGMT_REGISTER_ICH10 (ICH10_ACPI_CNTL);
      AcpiEnBit  = ICH10_ACPI_CNTL_ACPI_EN;
      break;
    default:
      DEBUG ((EFI_D_ERROR, "%a: Unknown Host Bridge Device ID: 0x%04x\n",
        __FUNCTION__, mHostBridgeDevId));
      ASSERT (FALSE);
      return;
  }
  PcdSet16S (PcdSimicsX58HostBridgePciDevId, mHostBridgeDevId);

  //
  // If the appropriate IOspace enable bit is set, assume the ACPI PMBA
  // has been configured and skip the setup here.
  // This matches the logic in AcpiTimerLibConstructor ().
  //
  if ((PciRead8 (AcpiCtlReg) & AcpiEnBit) == 0) {
    //
    // The PEI phase should be exited with fully accessibe ACPI PM IO space:
    // 1. set PMBA
    //
    PciAndThenOr32 (Pmba, PmbaAndVal, PmbaOrVal);

    //
    // 2. set PCICMD/IOSE
    //
    PciOr8 (PmCmd, EFI_PCI_COMMAND_IO_SPACE);

    //
    // 3. set ACPI PM IO enable bit (PMREGMISC:PMIOSE or ACPI_CNTL:ACPI_EN)
    //
    PciOr8 (AcpiCtlReg, AcpiEnBit);
  }

  if (mHostBridgeDevId == INTEL_ICH10_DEVICE_ID) {
    //
    // Set Root Complex Register Block BAR
    //
    PciWrite32 (
      POWER_MGMT_REGISTER_ICH10 (ICH10_RCBA),
      ICH10_ROOT_COMPLEX_BASE | ICH10_RCBA_EN
      );

  }
  //
  // Set the PM I/O base address to 0x400
  //
  PciAndThenOr32 (
    PCI_LIB_ADDRESS (
      0,
      31,
      0,
      0x40
      ),
    (UINT32) ~0xfc0, ICH10_PMBASE_VALUE
    );
  //
  // Enable AHCI and all ports on the SATA controller.
  //
  // Address MAP Reg, setting AHCI mode
  //
  PciOr16 (PCI_LIB_ADDRESS (0, 31, 2, 0x90), 0x0060);
  //
  //Enabling Ports 0-5
  //
  PciOr16 (PCI_LIB_ADDRESS (0, 31, 2, 0x92), 0x003F);
  //
  //Disabling Sata Controller 2, bit 25 = 1, bit 0 = 1
  //
  MmioWrite32(0xFED1F418, 0x02000001);
  //
  //Enable HPET at FED0_0000h ? FED0_03FFh
  //
  MmioWrite8(ICH10_ROOT_COMPLEX_BASE + 0x3404, 0x80);
  //
  //Config and enable APIC
  //
  MmioWrite32(0xFEC00000 + 0X0, 0);
  MmioWrite32(0xFEC00000 + 0X10, PcdGet8(PcdIoApicId)<<24);
  MmioWrite8(ICH10_ROOT_COMPLEX_BASE + 0x31FF, 0x01);
}


VOID
BootModeInitialization (
  VOID
  )
{
  EFI_STATUS    Status;

 DEBUG((EFI_D_INFO, "modeValue = %x\n", IoBitFieldRead16(ICH10_PMBASE_IO + 4, 10, 12)));
  if (IoBitFieldRead16(ICH10_PMBASE_IO + 4, 10, 12) == 0x5) {
    mBootMode = BOOT_ON_S3_RESUME;
  }

  Status = PeiServicesSetBootMode (mBootMode);
  ASSERT_EFI_ERROR (Status);

  Status = PeiServicesInstallPpi (mPpiBootMode);
  ASSERT_EFI_ERROR (Status);
}


VOID
ReserveEmuVariableNvStore (
  )
{
  EFI_PHYSICAL_ADDRESS VariableStore;

  //
  // Allocate storage for NV variables early on so it will be
  // at a consistent address.  Since VM memory is preserved
  // across reboots, this allows the NV variable storage to survive
  // a VM reboot.
  //
  VariableStore =
    (EFI_PHYSICAL_ADDRESS)(UINTN)
      AllocateRuntimePages (
        EFI_SIZE_TO_PAGES (2 * PcdGet32 (PcdFlashNvStorageFtwSpareSize))
        );
  DEBUG ((EFI_D_INFO,
          "Reserved variable store memory: 0x%lX; size: %dkb\n",
          VariableStore,
          (2 * PcdGet32 (PcdFlashNvStorageFtwSpareSize)) / 1024
        ));
  PcdSet64S (PcdEmuVariableNvStoreReserved, VariableStore);
}


VOID
SimicsVersionCheck(
  VOID
  )
{

  UINTN           PciAddrPtr;
  UINT8           CapOffset;
  STATIC CHAR8    SimicsStr[0x100];
  UINTN           i;
  UINT32          MajorVersion;
  UINT32          MinorVersion;
  UINT32          ModelNumber;

  PciAddrPtr = PCI_LIB_ADDRESS(0, SIMICS_SIDEBANDPCI_DEV, SIMICS_SIDEBANDPCI_FUNC, 0);
  CapOffset = PciRead8(PciAddrPtr + PCI_CAPBILITY_POINTER_OFFSET);
  if (CapOffset != 0xFF) {
    ModelNumber = PciRead32(PciAddrPtr + CapOffset + 4);
    MajorVersion = PciRead32(PciAddrPtr + CapOffset + 8);
    MinorVersion = PciRead32(PciAddrPtr + CapOffset + 0xc);
    for (i = 0; i < 0x80; i++) {
      SimicsStr[i] = PciRead8(PciAddrPtr + CapOffset + 0x10 + i);
    }
    DEBUG((EFI_D_INFO, "=============SIMICS Version info=============\n"));
    DEBUG((EFI_D_INFO, "Model number = %d\n", ModelNumber));
    DEBUG((EFI_D_INFO, "Major version = %d\n", MajorVersion));
    DEBUG((EFI_D_INFO, "Minor version = %d\n", MinorVersion));
    DEBUG((EFI_D_INFO, "%a\n", SimicsStr));
    DEBUG((EFI_D_INFO, "=============================================\n"));
  }
}

VOID
DebugDumpCmos (
  VOID
  )
{
  UINT8 Loop;

  DEBUG ((EFI_D_INFO, "CMOS:\n"));

  for (Loop = 0; Loop < 0x80; Loop++) {
    if ((Loop % 0x10) == 0) {
      DEBUG ((EFI_D_INFO, "%02x:", Loop));
    }
    DEBUG ((EFI_D_INFO, " %02x", CmosRead8 (Loop)));
    if ((Loop % 0x10) == 0xf) {
      DEBUG ((EFI_D_INFO, "\n"));
    }
  }
}


/**
  Fetch the number of boot CPUs from QEMU and expose it to UefiCpuPkg modules.
  Set the mMaxCpuCount variable.
**/
VOID
MaxCpuCountInitialization (
  VOID
  )
{
  mMaxCpuCount = PcdGet32 (PcdCpuMaxLogicalProcessorNumber);
  return;
}

/**
  Determine if S3 support is explicitly enabled.

  @retval  TRUE   If S3 support is explicitly enabled. Other functions in this
  library may be called (subject to their individual
  restrictions).

  FALSE  Otherwise. This includes unavailability of the firmware
  configuration interface. No other function in this library
  must be called.
**/
BOOLEAN
EFIAPI
QemuFwCfgS3Enabled(
  VOID
)
{
  //TO DO IF NEEDED
  return TRUE;
}

/**
  Perform Platform PEI initialization.

  @param  FileHandle      Handle of the file being invoked.
  @param  PeiServices     Describes the list of possible PEI Services.

  @return EFI_SUCCESS     The PEIM initialized successfully.

**/
EFI_STATUS
EFIAPI
InitializePlatform (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS    Status;

  DEBUG ((EFI_D_ERROR, "Platform PEIM Loaded\n"));

  SimicsVersionCheck ();
  DebugDumpCmos ();

  if (QemuFwCfgS3Enabled ()) {
    DEBUG ((EFI_D_INFO, "S3 support was detected on SIMICS\n"));
    mS3Supported = TRUE;
    Status = PcdSetBoolS (PcdAcpiS3Enable, TRUE);
    ASSERT_EFI_ERROR (Status);
  }
  AddressWidthInitialization ();
  MaxCpuCountInitialization ();

  mHostBridgeDevId = PciRead16(SIMICS_HOSTBRIDGE_DID);

  if (FeaturePcdGet (PcdSmmSmramRequire)) {
    X58TsegMbytesInitialization ();
  }

  PublishPeiMemory ();

  InitializeRamRegions ();

  if (mBootMode != BOOT_ON_S3_RESUME) {
    if (!FeaturePcdGet (PcdSmmSmramRequire)) {
      ReserveEmuVariableNvStore ();
    }
    MemMapInitialization ();
  }

  MiscInitialization ();
  InstallFeatureControlCallback ();

  return EFI_SUCCESS;
}
