/** @file
  PCH cycle deocding configuration and query library.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

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
#include <Private/Library/PchDmiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PchEspiLib.h>
#include <Register/PchRegs.h>
#include <Register/PchRegsLpc.h>
#include <Register/PchRegsSpi.h>
#include <Register/PchRegsSmbus.h>

typedef enum {
  SlaveLpcEspiCS0,
  SlaveEspiCS1,
  SlaveId_Max
} SLAVE_ID_INDEX;

/**
  Set PCH TCO base address.
  This cycle decoding is required also on DMI side
  Programming steps:
  1. set Smbus PCI offset 54h [8] to enable TCO base address.
  2. program Smbus PCI offset 50h [15:5] to TCO base address.
  3. set Smbus PCI offset 54h [8] to enable TCO base address.
  4. program "TCO Base Address" in DMI

  @param[in] Address                    Address for TCO base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchTcoBaseSet (
  IN  UINT16                            Address
  )
{
  UINT64                                SmbusBase;
  EFI_STATUS                            Status;

  if ((Address & ~B_SMBUS_CFG_TCOBASE_BAR) != 0) {
    DEBUG ((DEBUG_ERROR, "PchTcoBaseSet Error. Invalid Address: %x.\n", Address));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Status = PchDmiSetTcoBase (Address);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  SmbusBase = PCI_SEGMENT_LIB_ADDRESS (
                DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                DEFAULT_PCI_BUS_NUMBER_PCH,
                PCI_DEVICE_NUMBER_PCH_SMBUS,
                PCI_FUNCTION_NUMBER_PCH_SMBUS,
                0
                );
  if (PciSegmentRead16 (SmbusBase) == 0xFFFF) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }
  //
  // Verify TCO base is not locked.
  //
  if ((PciSegmentRead8 (SmbusBase + R_SMBUS_CFG_TCOCTL) & B_SMBUS_CFG_TCOCTL_TCO_BASE_LOCK) != 0) {
    ASSERT (FALSE);
    return EFI_DEVICE_ERROR;
  }
  //
  // Disable TCO in SMBUS Device first before changing base address.
  // Byte access to not touch the TCO_BASE_LOCK bit
  //
  PciSegmentAnd8 (
    SmbusBase + R_SMBUS_CFG_TCOCTL + 1,
    (UINT8) ~(B_SMBUS_CFG_TCOCTL_TCO_BASE_EN >> 8)
    );
  //
  // Program TCO in SMBUS Device
  //
  PciSegmentAndThenOr16 (
    SmbusBase + R_SMBUS_CFG_TCOBASE,
    (UINT16) (~B_SMBUS_CFG_TCOBASE_BAR),
    Address
    );
  //
  // Enable TCO in SMBUS Device and lock TCO BASE
  //
  PciSegmentOr16 (
    SmbusBase + R_SMBUS_CFG_TCOCTL,
    B_SMBUS_CFG_TCOCTL_TCO_BASE_EN | B_SMBUS_CFG_TCOCTL_TCO_BASE_LOCK
    );

  return Status;
}

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
  Returns PCH LPC device PCI base address.

  @retval                   PCH LPC PCI base address.
**/
STATIC
UINT64
LpcPciBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           PCI_DEVICE_NUMBER_PCH_LPC,
           PCI_FUNCTION_NUMBER_PCH_LPC,
           0
           );
}

/**
  Function checks if passed Generic LPC IO Address and Length meets requirements.

  @param[in] Address                    Address for generic IO range decoding.
  @param[in] Length                     Length of generic IO range.

  @retval TRUE                          Passed IO range meets requirements
  @retval FALSE                         Passed IO range does not meets requirements.
**/
STATIC
BOOLEAN
IsLpcIoRangeValid (
  IN  UINT32                            Address,
  IN  UINT32                            Length
  )
{
  UINT32            Index;
  UINT32            NumRanges;

  STATIC struct EXCEPT_RANGE {
    UINT8 Start;
    UINT8 Length;
  } ExceptRanges[] = { {0x00, 0x20}, {0x44, 0x08}, {0x54, 0x0C}, {0x68, 0x08}, {0x80, 0x10}, {0xC0, 0x40} };

  NumRanges = ARRAY_SIZE (ExceptRanges);
  //
  // For generic IO range, the base address must align to 4 and less than 0xFFFF,
  // the length must be power of 2 and less than or equal to 256, and the address must be length aligned.
  // IO range below 0x100 will be rejected in this function except below ranges:
  //   0x00-0x1F,
  //   0x44-0x4B,
  //   0x54-0x5F,
  //   0x68-0x6F,
  //   0x80-0x8F,
  //   0xC0-0xFF
  //
  if (((Length & (Length - 1)) != 0)  ||
      ((Address & (UINT16) ~B_LPC_CFG_GENX_DEC_IOBAR) != 0) ||
      (Length > 256)) {
    return FALSE;
  }
  if (Address < 0x100) {
    for (Index = 0; Index < NumRanges; Index++) {
      if ((Address >= ExceptRanges[Index].Start) &&
          ((Address + Length) <= ((UINTN) ExceptRanges[Index].Start + (UINTN) ExceptRanges[Index].Length))) {
        break;
      }
    }
    if (Index >= NumRanges) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
  Function checks if passed Generic LPC IO Range is already in Gen IO Range list

  @param[in] Address                    Address for generic IO range decoding.
  @param[in] Length                     Length of generic IO range.
  @param[in] GenIoRangeList             Pointer to Generic IO Ranges List

  @retval TRUE                          Passed IO range alredy covered
  @retval FALSE                         Passed IO range NOT covered
**/
STATIC
BOOLEAN
IsRangeInList (
  IN  UINT32                      Address,
  IN  UINT32                      Length,
  IN  PCH_LPC_GEN_IO_RANGE_LIST   *GenIoRangeList
  )
{
  UINT32                                CurrentBaseAddr;
  UINT32                                CurrentLength;
  UINT32                                Index;

  for (Index = 0; Index < PCH_LPC_GEN_IO_RANGE_MAX; Index++) {
    CurrentBaseAddr = GenIoRangeList->Range[Index].BaseAddr;
    CurrentLength   = GenIoRangeList->Range[Index].Length;
    if (GenIoRangeList->Range[Index].Enable == 0) {
      continue;
    }
    if ((Address >= CurrentBaseAddr) && ((Address + Length) <= (CurrentBaseAddr + CurrentLength))) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Function checks if passed Generic LPC IO Range overlaps with existing range

  @param[in] Address                    Address for generic IO range base address.
  @param[in] Length                     Length of generic IO range.
  @param[in] GenIoRangeList             Pointer to Generic IO Ranges List

  @retval TRUE                          Passed LPC IO range overlaps with existing range
  @retval FALSE                         Passed LPC IO range NOT overlaps
**/
STATIC
BOOLEAN
FindOverlappingGenIoRange (
  IN  UINT32                          Address,
  IN  UINT32                          Length,
  IN  PCH_LPC_GEN_IO_RANGE_LIST       *GenIoRangeList
  )
{
  UINT32                              Index;
  UINT32                              CurrentBaseAddr;
  UINT32                              CurrentLength;

  for (Index = 0; Index < PCH_LPC_GEN_IO_RANGE_MAX; Index++) {
    CurrentBaseAddr = GenIoRangeList->Range[Index].BaseAddr;
    CurrentLength = GenIoRangeList->Range[Index].Length;
    if (GenIoRangeList->Range[Index].Enable == 0) {
      continue;
    }

    if ((Address >= CurrentBaseAddr) &&
        (Address <= (CurrentBaseAddr + CurrentLength))) {
      return TRUE;
    } else if (((Address + Length) >= CurrentBaseAddr) &&
              ((Address + Length) <= (CurrentBaseAddr + CurrentLength))) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Function look for empty Generic IO range register.
  If found return range index.

  @param[in]      GenIoRangeList        Pointer to Generic IO Ranges List
  @param[in]      ListLength            Length of passed list
  @param[out]     RangeIndex            Generic IO Range Index

  @retval TRUE                          Empty range found
  @retval FALSE                         NOT found empty range
**/
STATIC
BOOLEAN
FindEmptyGenIoRange (
  IN  PCH_LPC_GEN_IO_RANGE_LIST   *GenIoRangeList,
  IN  UINT32                      ListLength,
  OUT UINT32                      *RangeIndex
  )
{
  UINT32                          Index;

  for (Index = 0; Index < ListLength; Index++) {
    if (GenIoRangeList->Range[Index].Enable == 0) {
      *RangeIndex = Index;
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Get PCH LPC/eSPI and eSPI CS1# generic IO range list.
  This function returns a list of base address, length, and enable for all LPC/eSPI or eSPI CS1# generic IO range registers.

  @param[in]  RangeIndex                Slave ID (refer to SLAVE_ID_INDEX)
  @param[out] GenIoRangeList            LPC/eSPI or eSPI CS1# generic IO range registers.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
**/
EFI_STATUS
LpcEspiGenIoRangeGetHelper (
  IN  SLAVE_ID_INDEX                    SlaveId,
  OUT PCH_LPC_GEN_IO_RANGE_LIST         *GenIoRangeList
  )
{
  UINT32                                Index;
  UINT64                                LpcBase;
  UINT32                                Data32;
  UINT32                                GenIoReg;

  if ((GenIoRangeList == NULL) || (SlaveId >= SlaveId_Max)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  LpcBase = LpcPciBase ();

  if (SlaveId == SlaveEspiCS1) {
    GenIoReg = R_ESPI_CFG_CS1GIR1;
  } else {
    GenIoReg = R_LPC_CFG_GEN1_DEC;
  }

  for (Index = 0; Index < PCH_LPC_GEN_IO_RANGE_MAX; Index++) {
    if ((SlaveId == SlaveEspiCS1) &&
        (Index > 0)) {
      // For eSPI CS1# we have only one range. Reset remaining entries to zero.
      GenIoRangeList->Range[Index].BaseAddr = 0;
      GenIoRangeList->Range[Index].Enable = 0;
      GenIoRangeList->Range[Index].Length = 0;
      continue;
    }
    Data32 = PciSegmentRead32 (LpcBase + GenIoReg + Index * 4);
    GenIoRangeList->Range[Index].BaseAddr = Data32 & B_LPC_CFG_GENX_DEC_IOBAR;
    GenIoRangeList->Range[Index].Length   = ((Data32 & B_LPC_CFG_GENX_DEC_IODRA) >> 16) + 4;
    GenIoRangeList->Range[Index].Enable   = Data32 & B_LPC_CFG_GENX_DEC_EN;
  }

  return EFI_SUCCESS;
}


/**
  Function checks if passed Generic LPC IO Range colliding
  with range alredy defined for other eSPI chiselect (CS)

  @param[in] Address                    Address for generic IO range base address.
  @param[in] Length                     Length of generic IO range.
  @param[in] SlaveId                    Slave ID (refer to SLAVE_ID_INDEX)

  @retval TRUE                          Passed IO range conflicting
  @retval FALSE                         There is no conflict
**/
STATIC
BOOLEAN
IsRangeColliding (
  IN  UINT32                      Address,
  IN  UINT32                      Length,
  IN  SLAVE_ID_INDEX              SlaveId
  )
{
  EFI_STATUS                      Status;
  PCH_LPC_GEN_IO_RANGE_LIST       GenIoRangeList;

  if (SlaveId == SlaveEspiCS1) {
    Status  = LpcEspiGenIoRangeGetHelper (SlaveLpcEspiCS0, &GenIoRangeList);
    if (!EFI_ERROR (Status)) {
      if (FindOverlappingGenIoRange (Address, Length, &GenIoRangeList) ||
          IsRangeInList (Address, Length, &GenIoRangeList)) {
        return TRUE;
      }
    }
  } else {
    Status  = LpcEspiGenIoRangeGetHelper (SlaveEspiCS1, &GenIoRangeList);
    if (!EFI_ERROR (Status)) {
      if (FindOverlappingGenIoRange (Address, Length, &GenIoRangeList) ||
          IsRangeInList (Address, Length, &GenIoRangeList)) {
        return TRUE;
      }
    }
  }

  return FALSE;
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
  EFI_STATUS                            Status;
  PCH_LPC_GEN_IO_RANGE_LIST             GenIoRangeList;
  UINT32                                RangeIndex;
  UINT32                                Data32;
  UINT32                                GenIoReg;
  UINT32                                ListLength;

  //
  // Check if pasesed Address and Length meets all requirements
  //
  if(!IsLpcIoRangeValid (Address, Length) || (SlaveId >= SlaveId_Max)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Read current Generic IO configuration
  //
  Status  = LpcEspiGenIoRangeGetHelper (SlaveId, &GenIoRangeList);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Check if passed Generic IO range is already covered in current configuration
  //
  if (IsRangeInList (Address, Length, &GenIoRangeList)) {
    return EFI_SUCCESS;
  }

  //
  // Check if passed Generic IO range conflicting with other eSPI CS decoding
  //
  if (IsRangeColliding (Address, Length, SlaveId)) {
    return EFI_UNSUPPORTED;
  }

  if (SlaveId == SlaveEspiCS1) {
    GenIoReg = R_ESPI_CFG_CS1GIR1;
    ListLength = ESPI_CS1_GEN_IO_RANGE_MAX;
  } else {
    GenIoReg = R_LPC_CFG_GEN1_DEC;
    ListLength = PCH_LPC_GEN_IO_RANGE_MAX;
  }

  RangeIndex = ListLength;
  //
  // Check if there is an empty Generic IO range register
  //
  if (FindEmptyGenIoRange (&GenIoRangeList, ListLength, &RangeIndex) == FALSE) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Program decoding in DMI and LPC/eSPI registers
  //
  if (SlaveId == SlaveEspiCS1) {
    ASSERT (RangeIndex == 0);
    Status = PchDmiSetEspiCs1GenIoRange (Address, Length);
  } else {
    Status = PchDmiSetLpcGenIoRange (Address, Length, RangeIndex);
  }
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Program LPC/eSPI generic IO range register accordingly.
  //
  Data32 =  (UINT32) (((Length - 1) << 16) & B_LPC_CFG_GENX_DEC_IODRA);
  Data32 |= (UINT32) Address;
  Data32 |= B_LPC_CFG_GENX_DEC_EN;

  //
  // Program LPC/eSPI PCI Offset 84h ~ 93h (LPC, eSPI CS0#) or A4h (eSPI CS1#) of Mask, Address, and Enable.
  //
  PciSegmentWrite32 (
    LpcPciBase () + GenIoReg + RangeIndex * 4,
    Data32
    );

  return Status;
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
  Set PCH eSPI CS1# generic IO range decoding.

  Steps of programming generic IO range:
  1. Program eSPI PCI Offset A4h (eSPI CS1#) of Mask, Address, and Enable.
  2. Program eSPI Generic IO Range in DMI

  @param[in] Address                    Address for generic IO range decoding.
  @param[in] Length                     Length of generic IO range.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address or length passed.
  @retval EFI_OUT_OF_RESOURCES          No more generic range available.
  @retval EFI_UNSUPPORTED               eSPI secondary slave not supported
**/
EFI_STATUS
PchEspiCs1GenIoRangeSet (
  IN  UINT16                            Address,
  IN  UINTN                             Length
  )
{
  if (!IsEspiSecondSlaveSupported ()) {
    return EFI_UNSUPPORTED;
  }

  return LpcEspiGenIoRangeSetHelper ((UINT32)Address, (UINT32)Length, SlaveEspiCS1);
}

/**
  Get PCH LPC/eSPI generic IO range list.
  This function returns a list of base address, length, and enable for all LPC/eSPI generic IO range registers.

  @param[out] LpcGenIoRangeList         Return all LPC/eSPI generic IO range register status.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
**/
EFI_STATUS
PchLpcGenIoRangeGet (
  OUT PCH_LPC_GEN_IO_RANGE_LIST         *LpcGenIoRangeList
  )
{
  return LpcEspiGenIoRangeGetHelper (SlaveLpcEspiCS0, LpcGenIoRangeList);
}

/**
  Get PCH eSPI CS1# generic IO range list.
  This function returns a list of base address, length, and enable for all eSPI CS1# generic IO range registers.

  @param[out] GenIoRangeList            eSPI generic IO range registers.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               eSPI secondary slave not supported
**/
EFI_STATUS
PchEspiCs1GenIoRangeGet (
  OUT PCH_LPC_GEN_IO_RANGE_LIST         *GenIoRangeList
  )
{
  if (!IsEspiSecondSlaveSupported ()) {
    return EFI_UNSUPPORTED;
  }

  return LpcEspiGenIoRangeGetHelper (SlaveEspiCS1, GenIoRangeList);
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
    DEBUG ((DEBUG_ERROR, "PchLpcEspiMemRangeSet Error. Invalid Address: %x or invalid SlaveId\n", Address));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  LpcBase = LpcPciBase ();

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
  @deprecated. Keep this for backward compatibility.
  It's replaced by PchEspiCs1MemRangeSet.
**/
EFI_STATUS
PchEspiMemRange2Set (
  IN  UINT32                            Address
  )
{
  return PchEspiCs1MemRangeSet (Address);
}

/**
  Get PCH LPC/eSPI and eSPI CS1# memory range decoding address.

  @param[in]  SlaveId                   Slave ID (refer to SLAVE_ID_INDEX)
  @param[out] Address                   Address of LPC/eSPI or eSPI CS1# memory decoding base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               eSPI secondary slave not supported
**/
EFI_STATUS
LpcEspiMemRangeGetHelper (
  IN  SLAVE_ID_INDEX                    SlaveId,
  OUT UINT32                            *Address
  )
{
  UINT32                                GenMemReg;

  if ((Address == NULL) || (SlaveId >= SlaveId_Max)) {
    DEBUG ((DEBUG_ERROR, "PchLpcEspiMemRangeGet Error. Invalid pointer or SlaveId.\n"));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (SlaveId == SlaveEspiCS1) {
    GenMemReg = R_ESPI_CFG_CS1GMR1;
  } else {
    GenMemReg = R_LPC_CFG_LGMR;
  }
  *Address = PciSegmentRead32 (LpcPciBase () + GenMemReg) & B_LPC_CFG_LGMR_MA;
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
    BaseAddr = PCI_SEGMENT_LIB_ADDRESS (
                 DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                 DEFAULT_PCI_BUS_NUMBER_PCH,
                 PCI_DEVICE_NUMBER_PCH_SPI,
                 PCI_FUNCTION_NUMBER_PCH_SPI,
                 0
                 );
    //
    // Read SPI CFG cycle before write SPI CFG cycle
    PciSegmentRead16 (BaseAddr + R_SPI_CFG_BDE);
    //
    // If SPI, Program SPI offset D8h to BiosDecodeEnable.
    //
    PciSegmentWrite16 (BaseAddr + R_SPI_CFG_BDE, BiosDecodeEnable);
  } else {
    BaseAddr = LpcPciBase ();
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

  LpcBaseAddr  = LpcPciBase ();

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

  LpcBaseAddr = LpcPciBase ();

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
  Set PCH IO port 80h cycle decoding to PCIE root port.
  System BIOS is likely to do this very soon after reset before PCI bus enumeration.
  This cycle decoding is allowed to set when DMI is unlocked

  @param[in] RpNumber                PCIE root port physical number.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
PchIoPort80DecodeSet (
  IN  UINTN                             RpNumber
  )
{
  EFI_STATUS  Status;

  Status = PchDmiSetIoPort80Decode (RpNumber);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Get IO APIC registers base address.

  @param[out] IoApicBase                Buffer of IO APIC register address

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
PchIoApicBaseGet (
  OUT UINT32                            *IoApicBase
  )
{
  *IoApicBase = PcdGet32 (PcdIoApicBaseAddress);
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

