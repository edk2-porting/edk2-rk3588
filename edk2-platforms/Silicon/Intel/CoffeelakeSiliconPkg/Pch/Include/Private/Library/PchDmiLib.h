/** @file
  Header file for PchDmiLib.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_DMI_LIB_H_
#define _PCH_DMI_LIB_H_

/**
  This function checks if DMI Secured Register Lock (SRL) is set

  @retval SRL state
**/
BOOLEAN
IsPchDmiLocked (
  VOID
  );

/**
  Set ACPI base address decoding in DMI

  @param[in] Address                    Address for ACPI base.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiSetAcpiBase (
  IN  UINT16                            Address
  );

/**
  Set PWRM base address decoding in DMI

  @param[in] Address                    Address for PWRM base.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiSetPwrmBase (
  IN  UINT32                            Address
  );

/**
  Set PCH TCO base address decoding in DMI

  @param[in] Address                    Address for TCO base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiSetTcoBase (
  IN  UINT16                            Address
  );

/**
  Get PCH TCO base address.

  @retval Address                   Address of TCO base address.
**/
UINT16
PchDmiGetTcoBase (
  VOID
  );

/**
  Set PCH LPC/eSPI generic IO range decoding in DMI

  @param[in] Address                    Address for generic IO range base address.
  @param[in] Length                     Length of generic IO range.
  @param[in] RangeIndex                 Index of choosen range

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiSetLpcGenIoRange (
  IN  UINT32                            Address,
  IN  UINT32                            Length,
  IN  UINT32                            RangeIndex
  );

/**
  Set PCH eSPI eSPI CS1# generic IO range decoding in DMI

  @param[in] Address                    Address for generic IO range base address.
  @param[in] Length                     Length of generic IO range.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiSetEspiCs1GenIoRange (
  IN  UINT32                            Address,
  IN  UINT32                            Length
  );

/**
  Clear PCH LPC/eSPI generic IO range decoding in DMI

  @param[in] RangeIndex                 Index of chosen range

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiClearLpcGenIoRange (
  IN  UINTN                             RangeIndex
  );

/**
  Clear PCH eSPI CS1# generic IO range decoding in DMI

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiClearEspiCs1GenIoRange (
  VOID
  );

/**
  Set PCH LPC/eSPI memory range decoding in DMI

  @param[in] Address                    Address for memory base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiSetLpcMemRange (
  IN  UINT32                            Address
  );

/**
  Set PCH eSPI CS1# memory range decoding in DMI

  @param[in] Address                    Address for memory base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiSetEspiCs1MemRange (
  IN  UINT32                            Address
  );

/**
  Check if Boot BIOS Strap is set for SPI.

  @retval TRUE                Boot BIOS Strap set for SPI
  @retval FALSE               Boot BIOS Strap set for LPC/eSPI
**/
BOOLEAN
PchDmiIsBootBiosStrapSetForSpi (
  VOID
  );

/**
  Set PCH BIOS range decoding in DMI
  Please check EDS for detail of BiosDecodeEnable bit definition.
    bit 15: F8-FF Enable
    bit 14: F0-F8 Enable
    bit 13: E8-EF Enable
    bit 12: E0-E8 Enable
    bit 11: D8-DF Enable
    bit 10: D0-D7 Enable
    bit  9: C8-CF Enable
    bit  8: C0-C7 Enable
    bit  7: Legacy F Segment Enable
    bit  6: Legacy E Segment Enable
    bit  5: Reserved
    bit  4: Reserved
    bit  3: 70-7F Enable
    bit  2: 60-6F Enable
    bit  1: 50-5F Enable
    bit  0: 40-4F Enable

  @param[in] BiosDecodeEnable           Bios decode enable setting.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiSetBiosDecodeEnable (
  IN  UINT16                            BiosDecodeEnable
  );

/**
  Set PCH LPC/eSPI IO decode ranges in DMI
  Please check EDS for detail of LPC/eSPI IO decode ranges bit definition.
  Bit  12: FDD range
  Bit 9:8: LPT range
  Bit 6:4: ComB range
  Bit 2:0: ComA range

  @param[in] LpcIoDecodeRanges          LPC/eSPI IO decode ranges bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiSetLpcIoDecodeRanges (
  IN  UINT16                            LpcIoDecodeRanges
  );

/**
  Set PCH LPC/eSPI IO enable decoding in DMI

  @param[in] LpcIoEnableDecoding        LPC/eSPI IO enable decoding bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiSetLpcIoEnable (
  IN  UINT16                            LpcIoEnableDecoding
  );

/**
  Set PCH IO port 80h cycle decoding to PCIE root port in DMI

  @param[in] RpNumber                   PCIE root port physical number.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
PchDmiSetIoPort80Decode (
  IN  UINTN                             RpNumber
  );

/**
  Set DMI thermal throttling to recommended configuration
**/
VOID
PchDmiSetRecommendedThermalThrottling (
  VOID
  );

//
// Thermal Sensor Target Width structure
// Look at DMI_THERMAL_SENSOR_TARGET_WIDTH for possible values
//
typedef struct {
  UINT32  ThermalSensor0TargetWidth :3;
  UINT32  ThermalSensor1TargetWidth :3;
  UINT32  ThermalSensor2TargetWidth :3;
  UINT32  ThermalSensor3TargetWidth :3;
  UINT32  Rsvd                      :20;
} DMI_THERMAL_THROTTLING;

/**
  Set DMI thermal throttling to custom configuration.
  This function will configure Thermal Sensor 0/1/2/3 TargetWidth and set
  DMI Thermal Sensor Autonomous Width Enable.

  @param[in] DmiThermalThrottling        DMI Thermal Throttling structure.
**/
VOID
PchDmiSetCustomThermalThrottling (
  IN DMI_THERMAL_THROTTLING      DmiThermalThrottling
  );

/**
  Determines where to send the reserved page registers
  Accesses to the I/O ranges 80h - 8Fh will be forwarded to PCIe Root Port
  with the destination ID specified in GCS.RPRDID using DMI source decode.
**/
VOID
PchDmiSetReservedPageRegToPcieRootPort (
  VOID
  );

/**
  Determines where to send the reserved page registers
  DMI will not perform source decode on the I/O ranges 80h - 8Fh. The cycles hitting these ranges will
  end up in P2SB which will then forward the cycle to LPC or eSPI through IOSF Sideband.
**/
VOID
PchDmiSetReservedPageRegToLpc (
  VOID
  );

/**
  uCode Patch Region Enable (UPRE). Enables memory access targeting the uCode patch region (0xFEF00000 to 0xFEFFFFFF)
  to be forwarded to SPI Flash. This can only be set if the boot flash is on SPI.
**/
VOID
PchDmiEnableUCodePatchRegion (
  VOID
  );

/**
  Enable PCIe Relaxed Order
**/
VOID
PchDmiEnablePcieRelaxedOrder (
  VOID
  );

/**
  This function will switch SAI value to be driven to IOSF Primary Fabric
  for cycles with Core BDF from HOSTIA_BOOT_SAI to HOSTIA_POSTBOOT_SAI.
  To be used when PCH is paired with CFL CPU.
**/
VOID
PchDmiEnablePostBootSai (
  VOID
  );

/**
  This function will do necessary configuration after platform
  should have switched to POSTBOOT_SAI. It needs to be called even if
  POSTBOOT_SAI was not set.
**/
VOID
PchDmiConfigAfterPostBootSai (
  VOID
  );

/**
  Configure PCH DMI Lock
**/
VOID
PchDmiSetLockWithS3BootScript (
  VOID
  );

/**
  Set BIOS interface Lock-Down
**/
VOID
PchDmiSetBiosLockDownWithS3BootScript (
  VOID
  );
#endif // _PCH_DMI_LIB_H_
