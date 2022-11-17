/** @file
  PCH cycle decoding configuration and query library.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchDmiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Register/PchRegs.h>
#include <Register/PchRegsLpc.h>
#include <Register/SpiRegs.h>
#include <Register/SmbusRegs.h>
#include <Library/EspiLib.h>
#include <Library/PchPciBdfLib.h>

typedef enum {
  SlaveLpcEspiCS0,
  SlaveEspiCS1,
  SlaveId_Max
} SLAVE_ID_INDEX;

/**
  Get PCH TCO base address.

  @param[out] Address                   Address of TCO base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid pointer passed.
**/
EFI_STATUS
PchTcoBaseGet (
  OUT UINT16                            *Address
  )
{
  if (Address == NULL) {
    DEBUG ((DEBUG_ERROR, "PchTcoBaseGet Error. Invalid pointer.\n"));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  //
  // Read "TCO Base Address" from DMI
  // Don't read TCO base address from SMBUS PCI register since SMBUS might be disabled.
  //
  *Address = PchDmiGetTcoBase ();

  return EFI_SUCCESS;
}

/**
  Set PCH LPC/eSPI and eSPI CS1# generic IO range decoding.

  Steps of programming generic IO range:
  1. Program LPC/eSPI PCI Offset 84h ~ 93h (LPC, eSPI CS0#) or A4h (eSPI CS1#) of Mask, Address, and Enable.
  2. Program LPC/eSPI Generic IO Range in DMI

  @param[in] Address                    Address for generic IO range decoding.
  @param[in] Length                     Length of generic IO range.
  @param[in] SlaveId                    Slave ID (refer to SLAVE_ID_INDEX)

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address or length passed.
  @retval EFI_OUT_OF_RESOURCES          No more generic range available.
  @retval EFI_UNSUPPORTED               DMI configuration is locked,
                                        GenIO range conflicting with other eSPI CS
**/
STATIC
EFI_STATUS
LpcEspiGenIoRangeSetHelper (
  IN  UINT32                            Address,
  IN  UINT32                            Length,
  IN  SLAVE_ID_INDEX                    SlaveId
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Set PCH LPC/eSPI generic IO range.
  For generic IO range, the base address must align to 4 and less than 0xFFFF, and the length must be power of 2
  and less than or equal to 256. Moreover, the address must be length aligned.
  This function basically checks the address and length, which should not overlap with all other generic ranges.
  If no more generic range register available, it returns out of resource error.
  This cycle decoding is also required on DMI side
  Some IO ranges below 0x100 have fixed target. The target might be ITSS,RTC,LPC,PMC or terminated inside P2SB
  but all predefined and can't be changed. IO range below 0x100 will be rejected in this function except below ranges:
    0x00-0x1F,
    0x44-0x4B,
    0x54-0x5F,
    0x68-0x6F,
    0x80-0x8F,
    0xC0-0xFF
  Steps of programming generic IO range:
  1. Program LPC/eSPI PCI Offset 84h ~ 93h of Mask, Address, and Enable.
  2. Program LPC/eSPI Generic IO Range in DMI

  @param[in] Address                    Address for generic IO range base address.
  @param[in] Length                     Length of generic IO range.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address or length passed.
  @retval EFI_OUT_OF_RESOURCES          No more generic range available.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchLpcGenIoRangeSet (
  IN  UINT16                            Address,
  IN  UINTN                             Length
  )
{
  return LpcEspiGenIoRangeSetHelper ((UINT32)Address, (UINT32)Length, SlaveLpcEspiCS0);
}


/**
  Set PCH LPC/eSPI and eSPI CS1# memory range decoding.
  This cycle decoding is required to be set on DMI side
  Programming steps:
  1. Program LPC/eSPI PCI Offset 98h (LPC, eSPI CS0#) or A8h (eSPI CS1#) [0] to [0] to disable memory decoding first before changing base address.
  2. Program LPC/eSPI PCI Offset 98h (LPC, eSPI CS0#) or A8h (eSPI CS1#) [31:16, 0] to [Address, 1].
  3. Program LPC/eSPI Memory Range in DMI

  @param[in] Address                    Address for memory for decoding.
  @param[in] RangeIndex                 Slave ID (refer to SLAVE_ID_INDEX)

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address or length passed.
**/
STATIC
EFI_STATUS
LpcEspiMemRangeSetHelper (
  IN  UINT32                            Address,
  IN  SLAVE_ID_INDEX                    SlaveId
  )
{
  UINT64                                LpcBase;
  EFI_STATUS                            Status;
  UINT32                                GenMemReg;
  UINT32                                MemRangeAddr;

  if (((Address & (~B_LPC_CFG_LGMR_MA)) != 0) || (SlaveId >= SlaveId_Max)) {
    DEBUG ((DEBUG_ERROR, "%a Error. Invalid Address: %x or invalid SlaveId\n", __FUNCTION__, Address));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  LpcBase = LpcPciCfgBase ();

  MemRangeAddr = ~Address;
  if (SlaveId == SlaveEspiCS1) {
    GenMemReg = R_ESPI_CFG_CS1GMR1;
    // Memory Range already decoded for LPC/eSPI?
    Status = PchLpcMemRangeGet (&MemRangeAddr);
    if (MemRangeAddr != Address) {
      Status = PchDmiSetEspiCs1MemRange (Address);
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
        return Status;
      }
    }
  } else {
    GenMemReg = R_LPC_CFG_LGMR;
    // Memory Range already decoded for eSPI CS1?
    Status = PchEspiCs1MemRangeGet (&MemRangeAddr);
    if (MemRangeAddr != Address) {
      Status = PchDmiSetLpcMemRange (Address);
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
        return Status;
      }
    }
  }

  //
  // Program LPC/eSPI PCI Offset 98h (LPC, eSPI CS0#) or A8h (eSPI CS1#) [0] to [0] to disable memory decoding first before changing base address.
  //
  PciSegmentAnd32 (
    LpcBase + GenMemReg,
    (UINT32) ~B_LPC_CFG_LGMR_LMRD_EN
    );
  //
  // Program LPC/eSPI PCI Offset 98h (LPC, eSPI CS0#) or A8h (eSPI CS1#) [31:16, 0] to [Address, 1].
  //
  PciSegmentWrite32 (
    LpcBase + GenMemReg,
    (Address | B_LPC_CFG_LGMR_LMRD_EN)
    );

  return Status;
}

/**
  Set PCH LPC/eSPI memory range decoding.
  This cycle decoding is required to be set on DMI side
  Programming steps:
  1. Program LPC PCI Offset 98h [0] to [0] to disable memory decoding first before changing base address.
  2. Program LPC PCI Offset 98h [31:16, 0] to [Address, 1].
  3. Program LPC Memory Range in DMI

  @param[in] Address                    Address for memory base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address or length passed.
  @retval EFI_OUT_OF_RESOURCES          No more generic range available.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchLpcMemRangeSet (
  IN  UINT32                            Address
  )
{
  return LpcEspiMemRangeSetHelper (Address, SlaveLpcEspiCS0);
}

/**
  Set PCH eSPI CS1# memory range decoding.
  This cycle decoding is required to be set on DMI side
  Programming steps:
  1. Program eSPI PCI Offset A8h (eSPI CS1#) [0] to [0] to disable memory decoding first before changing base address.
  2. Program eSPI PCI Offset A8h (eSPI CS1#) [31:16, 0] to [Address, 1].
  3. Program eSPI Memory Range in DMI

  @param[in] Address                    Address for memory for decoding.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address or length passed.
  @retval EFI_UNSUPPORTED               eSPI secondary slave not supported
**/
EFI_STATUS
PchEspiCs1MemRangeSet (
  IN  UINT32                            Address
  )
{
  if (!IsEspiSecondSlaveSupported ()) {
    return EFI_UNSUPPORTED;
  }

  return LpcEspiMemRangeSetHelper (Address, SlaveEspiCS1);
}

/**
  Get PCH LPC/eSPI and eSPI CS1# memory range decoding address.

  @param[in]  SlaveId                   Slave ID (refer to SLAVE_ID_INDEX)
  @param[out] Address                   Address of LPC/eSPI or eSPI CS1# memory decoding base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               eSPI secondary slave not supported
**/
STATIC
EFI_STATUS
LpcEspiMemRangeGetHelper (
  IN  SLAVE_ID_INDEX                    SlaveId,
  OUT UINT32                            *Address
  )
{
  UINT32                                GenMemReg;

  if ((Address == NULL) || (SlaveId >= SlaveId_Max)) {
    DEBUG ((DEBUG_ERROR, "%a Error. Invalid pointer or SlaveId.\n", __FUNCTION__));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (SlaveId == SlaveEspiCS1) {
    GenMemReg = R_ESPI_CFG_CS1GMR1;
  } else {
    GenMemReg = R_LPC_CFG_LGMR;
  }
  *Address = PciSegmentRead32 (LpcPciCfgBase () + GenMemReg) & B_LPC_CFG_LGMR_MA;
  return EFI_SUCCESS;
}

/**
  Get PCH LPC/eSPI memory range decoding address.

  @param[out] Address                   Address of LPC/eSPI memory decoding base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
**/
EFI_STATUS
PchLpcMemRangeGet (
  OUT UINT32                            *Address
  )
{
  return LpcEspiMemRangeGetHelper (SlaveLpcEspiCS0, Address);
}

/**
  Get PCH eSPI CS1# memory range decoding address.

  @param[out] Address                   Address of eSPI CS1# memory decoding base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               eSPI secondary slave not supported
**/
EFI_STATUS
PchEspiCs1MemRangeGet (
  OUT UINT32                            *Address
  )
{
  if (!IsEspiSecondSlaveSupported ()) {
    return EFI_UNSUPPORTED;
  }

  return LpcEspiMemRangeGetHelper (SlaveEspiCS1, Address);
}

/**
  Set PCH BIOS range deocding.
  This will check General Control and Status bit 10 (GCS.BBS) to identify SPI or LPC/eSPI and program BDE register accordingly.
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
  This cycle decoding is also required in DMI
  Programming steps:
  1. if GCS.BBS is 0 (SPI), program SPI offset D8h to BiosDecodeEnable.
     if GCS.BBS is 1 (LPC/eSPi), program LPC offset D8h to BiosDecodeEnable.
  2. program LPC BIOS Decode Enable in DMI

  @param[in] BiosDecodeEnable           Bios decode enable setting.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchBiosDecodeEnableSet (
  IN  UINT16                            BiosDecodeEnable
  )
{
  UINT64                                BaseAddr;
  EFI_STATUS                            Status;

  Status = PchDmiSetBiosDecodeEnable (BiosDecodeEnable);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Check Boot BIOS Strap in DMI
  //
  if (PchDmiIsBootBiosStrapSetForSpi ()) {
    BaseAddr = SpiPciCfgBase ();
    //
    // If SPI, Program SPI offset D8h to BiosDecodeEnable.
    //
    PciSegmentWrite16 (BaseAddr + R_SPI_CFG_BDE, BiosDecodeEnable);
  } else {
    BaseAddr = LpcPciCfgBase ();
    //
    // If LPC/eSPi, program LPC offset D8h to BiosDecodeEnable.
    //
    PciSegmentWrite16 (BaseAddr + R_LPC_CFG_BDE, BiosDecodeEnable);
  }

  return Status;
}

/**
  Set PCH LPC/eSPI IO decode ranges.
  Program LPC/eSPI I/O Decode Ranges in DMI to the same value programmed in LPC/eSPI PCI offset 80h.
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
PchLpcIoDecodeRangesSet (
  IN  UINT16                            LpcIoDecodeRanges
  )
{
  UINT64                                LpcBaseAddr;
  EFI_STATUS                            Status;

  //
  // Note: Inside this function, don't use debug print since it's could used before debug print ready.
  //

  LpcBaseAddr = LpcPciCfgBase ();

  //
  // check if setting is identical
  //
  if (LpcIoDecodeRanges == PciSegmentRead16 (LpcBaseAddr + R_LPC_CFG_IOD)) {
    return EFI_SUCCESS;
  }

  Status = PchDmiSetLpcIoDecodeRanges (LpcIoDecodeRanges);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // program LPC/eSPI PCI offset 80h.
  //
  PciSegmentWrite16 (LpcBaseAddr + R_LPC_CFG_IOD, LpcIoDecodeRanges);

  return Status;
}

/**
  Set PCH LPC/eSPI and eSPI CS1# IO enable decoding.
  Setup I/O Enables in DMI to the same value program in LPC/eSPI PCI offset 82h (LPC, eSPI CS0#) or A0h (eSPI CS1#).
  Note: Bit[15:10] of the source decode register is Read-Only. The IO range indicated by the Enables field
  in LPC/eSPI PCI offset 82h[13:10] or A0h[13:10] is always forwarded by DMI to subtractive agent for handling.
  Please check EDS for detail of Lpc/eSPI IO decode ranges bit definition.

  @param[in] IoEnableDecoding           LPC/eSPI IO enable decoding bit settings.
  @param[in] SlaveId                    Slave ID (refer to SLAVE_ID_INDEX)

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMI configuration is locked
**/
EFI_STATUS
LpcEspiIoEnableDecodingSetHelper (
  IN  UINT16                            IoEnableDecoding,
  IN  SLAVE_ID_INDEX                    SlaveId
  )
{
  UINT64      LpcBaseAddr;
  EFI_STATUS  Status;
  UINT16      Cs1IoEnableDecodingOrg;
  UINT16      Cs0IoEnableDecodingOrg;
  UINT16      IoEnableDecodingMerged;

  LpcBaseAddr = LpcPciCfgBase ();

  Cs0IoEnableDecodingOrg = PciSegmentRead16 (LpcBaseAddr + R_LPC_CFG_IOE);

  if (IsEspiSecondSlaveSupported ()) {
    Cs1IoEnableDecodingOrg = PciSegmentRead16 (LpcBaseAddr + R_ESPI_CFG_CS1IORE);
  } else {
    Cs1IoEnableDecodingOrg = 0;
  }

  if (SlaveId == SlaveEspiCS1) {
    if (IoEnableDecoding == Cs1IoEnableDecodingOrg) {
      return EFI_SUCCESS;
    } else {
      IoEnableDecodingMerged = (Cs0IoEnableDecodingOrg | IoEnableDecoding);
    }
  } else {
    if ((IoEnableDecoding | Cs1IoEnableDecodingOrg) == Cs0IoEnableDecodingOrg) {
      return EFI_SUCCESS;
    } else {
      IoEnableDecodingMerged = (Cs1IoEnableDecodingOrg | IoEnableDecoding);
    }
  }

  Status = PchDmiSetLpcIoEnable (IoEnableDecodingMerged);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // program PCI offset 82h for LPC/eSPI.
  //
  PciSegmentWrite16 (LpcBaseAddr + R_LPC_CFG_IOE, IoEnableDecodingMerged);

  if (SlaveId == SlaveEspiCS1) {
    //
    // For eSPI CS1# device program eSPI PCI offset A0h.
    //
    PciSegmentWrite16 (LpcBaseAddr + R_ESPI_CFG_CS1IORE, IoEnableDecoding);
  }

  return Status;
}

/**
  Set PCH LPC and eSPI CS0# IO enable decoding.
  Setup I/O Enables in DMI to the same value program in LPC/eSPI PCI offset 82h.
  Note: Bit[15:10] of the source decode register is Read-Only. The IO range indicated by the Enables field
  in LPC/eSPI PCI offset 82h[13:10] is always forwarded by DMI to subtractive agent for handling.
  Please check EDS for detail of LPC/eSPI IO decode ranges bit definition.

  @param[in] LpcIoEnableDecoding        LPC IO enable decoding bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchLpcIoEnableDecodingSet (
  IN  UINT16                            LpcIoEnableDecoding
  )
{
  return LpcEspiIoEnableDecodingSetHelper (LpcIoEnableDecoding, SlaveLpcEspiCS0);
}

/**
  Set PCH eSPI CS1# IO enable decoding.
  Setup I/O Enables in DMI to the same value program in eSPI PCI offset A0h (eSPI CS1#).
  Note: Bit[15:10] of the source decode register is Read-Only. The IO range indicated by the Enables field
  in eSPI PCI offset A0h[13:10] is always forwarded by DMI to subtractive agent for handling.
  Please check EDS for detail of eSPI IO decode ranges bit definition.

  @param[in] IoEnableDecoding           eSPI IO enable decoding bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMI configuration is locked
**/
EFI_STATUS
PchEspiCs1IoEnableDecodingSet (
  IN  UINT16                            IoEnableDecoding
  )
{
  if (!IsEspiSecondSlaveSupported ()) {
    return EFI_UNSUPPORTED;
  }

  return LpcEspiIoEnableDecodingSetHelper (IoEnableDecoding, SlaveEspiCS1);
}


/**
  Get IO APIC regsiters base address.

  @param[out] IoApicBase                Buffer of IO APIC regsiter address

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
PchIoApicBaseGet (
  OUT UINT32                            *IoApicBase
  )
{
  *IoApicBase = PcdGet32 (PcdSiIoApicBaseAddress);
  return EFI_SUCCESS;
}

/**
  Get HPET base address.

  @param[out] HpetBase                  Buffer of HPET base address

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid offset passed.
**/
EFI_STATUS
PchHpetBaseGet (
  OUT UINT32                            *HpetBase
  )
{
  if (HpetBase == NULL) {
    DEBUG ((DEBUG_ERROR, "PchHpetBaseGet Error. Invalid pointer.\n"));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  *HpetBase = PcdGet32 (PcdSiHpetBaseAddress);
  return EFI_SUCCESS;
}

