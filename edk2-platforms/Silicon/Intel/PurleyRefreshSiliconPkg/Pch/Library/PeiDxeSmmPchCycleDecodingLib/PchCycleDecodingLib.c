/** @file

Copyright (c) 2018 - 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/MmPciBaseLib.h>
#include <PchAccess.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchP2sbLib.h>
#include <Library/PchCycleDecodingLib.h>

/**
  Set PCH ACPI base address.
  The Address should not be 0 and should be 256 bytes alignment, and it is IO space, so must not exceed 0xFFFF.
  This cycle decoding is allowed to set when DMIC.SRL is 0.
  Programming steps:
  1. clear PMC PCI offset 44h [7] to diable ACPI base address first before changing base address.
  2. program PMC PCI offset 40h [15:2] to ACPI base address.
  3. set PMC PCI offset 44h [7] to enable ACPI base address.
  4. program "ACPI Base Address" PCR[DMI] + 27B4h[23:18, 15:2, 0] to [0x3F, PMC PCI Offset 40h bit[15:2],  1].
  5. Program "ACPI Base Destination ID"
      For SPT-LP:  Program PCR[DMI] + 27B8h[31:0] to 0x23A0
      For SPT-H:   Program PCR[DMI] + 27B8h[31:0] to 0x23A8

  @param[in] Address                    Address for ACPI base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
EFIAPI
PchAcpiBaseSet (
  IN  UINT16                            Address
  )
{
  UINTN                                 PmcBase;
  UINT32                                Dmic;
  UINT32                                Data32;
  PCH_SERIES                            PchSeries;

  PchSeries = GetPchSeries ();

  if (((Address & 0x00FF) != 0) ||
      (Address == 0))
  {
    DEBUG((DEBUG_ERROR, "PchAcpiBaseSet Error. Invalid Address: %x.\n", Address));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  PchPcrRead32 (PID_DMI, R_PCH_PCR_DMI_DMIC, &Dmic);
  if ((Dmic & B_PCH_PCR_DMI_DMIC_SRL) != 0) {
    DEBUG((DEBUG_ERROR, "PchAcpiBaseSet Error. DMIC.SRL is set.\n"));
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  PmcBase      = MmPciBase (
                   DEFAULT_PCI_BUS_NUMBER_PCH,
                   PCI_DEVICE_NUMBER_PCH_PMC,
                   PCI_FUNCTION_NUMBER_PCH_PMC
                   );
  if (MmioRead16 (PmcBase) == 0xFFFF) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }
  //
  // Disable ABASE in PMC Device first before changing base address.
  //
  MmioAnd8 (
    PmcBase + R_PCH_PMC_ACPI_CNT,
    (UINT8) ~B_PCH_PMC_ACPI_CNT_ACPI_EN
    );
  //
  // Program ABASE in PMC Device
  //
  MmioAndThenOr16 (
    PmcBase + R_PCH_PMC_ACPI_BASE,
    (UINT16) (~B_PCH_PMC_ACPI_BASE_BAR),
    Address
    );
  //
  // Enable ABASE in PMC Device
  //
  MmioOr8 (
    PmcBase + R_PCH_PMC_ACPI_CNT,
    B_PCH_PMC_ACPI_CNT_ACPI_EN
    );
  //
  // Program "ACPI Base Address" PCR[DMI] + 27B4h[23:18, 15:2, 0] to [0x3F, PMC PCI Offset 40h bit[15:2],  1]
  //
  PchPcrWrite32 (
    PID_DMI, R_PCH_PCR_DMI_ACPIBA,
    (0x00FC0001 + Address)
    );
  //
  // Program "ACPI Base Destination ID"
  // For SPT-LP:
  //   Program PCR[DMI] + 27B8h[31:0] to 0x23A0
  // For SPT-H:
  //   Program PCR[DMI] + 27B8h[31:0] to 0x23A8
  //
  if(PchSeries == PchLp){
    Data32 = 0x23A0;
  } else {
    Data32 = 0x23A8;
  }
  PchPcrWrite32 (
    PID_DMI, R_PCH_PCR_DMI_ACPIBDID,
    Data32
    );
  return EFI_SUCCESS;
}

/**
  Get PCH ACPI base address.

  @param[out] Address                   Address of ACPI base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid pointer passed.
**/
EFI_STATUS
EFIAPI
PchAcpiBaseGet (
  OUT UINT16                            *Address
  )
{
  UINTN                                 PmcBase;

  if (Address == NULL) {
    DEBUG((DEBUG_ERROR, "PchAcpiBaseGet Error. Invalid pointer.\n"));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  PmcBase      = MmPciBase (
                   DEFAULT_PCI_BUS_NUMBER_PCH,
                   PCI_DEVICE_NUMBER_PCH_PMC,
                   PCI_FUNCTION_NUMBER_PCH_PMC
                   );
  if (MmioRead16 (PmcBase) == 0xFFFF) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }
  *Address = MmioRead16 (PmcBase + R_PCH_PMC_ACPI_BASE) & B_PCH_PMC_ACPI_BASE_BAR;
  return EFI_SUCCESS;
}

/**
  Set PCH PWRM base address.
  This cycle decoding is allowed to set when DMIC.SRL is 0.
  Programming steps:
  1. clear PMC PCI offset 44h [8] to diable PWRM base address first before changing PWRM base address.
  2. program PMC PCI offset 48h [31:16] to PM base address.
  3. set PMC PCI offset 44h [8] to enable PWRM base address.
  4. program "PM Base Address Memory Range Base" PCR[DMI] + 27ACh[15:0] to the same value programmed in PMC PCI Offset 48h bit[31:16], this has an implication of making sure the PWRMBASE to be 64KB aligned.
     program "PM Base Address Memory Range Limit" PCR[DMI] + 27ACh[31:16] to the value programmed in PMC PCI Offset 48h bit[31:16], this has an implication of making sure the memory allocated to PWRMBASE to be 64KB in size.
  5. Program "PM Base Control"
      For SPT-LP: Program PCR[DMI] + 27B0h[31, 30:0] to [1, 0x23A0]
      For SPT-H:  Program PCR[DMI] + 27B0h[31, 30:0] to [1, 0x23A8]

  @param[in] Address                    Address for PWRM base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
EFIAPI
PchPwrmBaseSet (
  IN  UINT32                            Address
  )
{
  UINTN                                 PmcBase;
  UINT32                                Dmic;
  UINT32                                Data32;
  PCH_SERIES                            PchSeries;

  PchSeries = GetPchSeries ();

  if (((Address & (~B_PCH_PMC_PWRM_BASE_BAR)) != 0) ||
      (Address == 0))
  {
    DEBUG((DEBUG_ERROR, "PchPwrmBaseSet Error. Invalid Address: %x.\n", Address));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  PchPcrRead32 (PID_DMI, R_PCH_PCR_DMI_DMIC, &Dmic);
  if ((Dmic & B_PCH_PCR_DMI_DMIC_SRL) != 0) {
    DEBUG((DEBUG_ERROR, "PchPwrmBaseSet Error. DMIC.SRL is set.\n"));
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  PmcBase      = MmPciBase (
                   DEFAULT_PCI_BUS_NUMBER_PCH,
                   PCI_DEVICE_NUMBER_PCH_PMC,
                   PCI_FUNCTION_NUMBER_PCH_PMC
                   );
  if (MmioRead16 (PmcBase) == 0xFFFF) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }
  //
  // Disable PWRMBASE in PMC Device first before changing PWRM base address.
  //
  MmioAnd16 (
    PmcBase + R_PCH_PMC_ACPI_CNT,
    (UINT16) ~B_PCH_PMC_ACPI_CNT_PWRM_EN
    );
  //
  // Program PWRMBASE in PMC Device
  //
  MmioAndThenOr32 (
    PmcBase + R_PCH_PMC_PWRM_BASE,
    (UINT32) (~B_PCH_PMC_PWRM_BASE_BAR),
    Address
    );
  //
  // Enable PWRMBASE in PMC Device
  //
  MmioOr16 (
    PmcBase + R_PCH_PMC_ACPI_CNT,
    B_PCH_PMC_ACPI_CNT_PWRM_EN
    );
  //
  // Program "PM Base Address Memory Range Base" PCR[DMI] + 27ACh[15:0] to the same value programmed in PMC PCI Offset 48h bit[31:16], this has an implication of making sure the PWRMBASE to be 64KB aligned.
  // Program "PM Base Address Memory Range Limit" PCR[DMI] + 27ACh[31:16] to the value programmed in PMC PCI Offset 48h bit[31:16], this has an implication of making sure the memory allocated to PWRMBASE to be 64KB in size.
  //
  PchPcrWrite32 (
    PID_DMI, R_PCH_PCR_DMI_PMBASEA,
    ((Address & 0xFFFF0000) | (Address >> 16))
    );
  //
  // Program "PM Base Control"
  // For SPT-LP:
  //   Program PCR[DMI] + 27B0h[31, 30:0] to [1, 0x23A0]
  // For SPT-H:
  //   Program PCR[DMI] + 27B0h[31, 30:0] to [1, 0x23A8]
  //
  if(PchSeries == PchLp){
    Data32 = 0x800023A0;
  } else {
    Data32 = 0x800023A8;
  }
  PchPcrWrite32 (
    PID_DMI, R_PCH_PCR_DMI_PMBASEC,
    Data32
    );
  return EFI_SUCCESS;
}

/**
  Get PCH PWRM base address.

  @param[out] Address                   Address of PWRM base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid pointer passed.
**/
EFI_STATUS
EFIAPI
PchPwrmBaseGet (
  OUT UINT32                            *Address
  )
{
  UINTN                                 PmcBase;

  if (Address == NULL) {
    DEBUG((DEBUG_ERROR, "PchPwrmBaseGet Error. Invalid pointer.\n"));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  PmcBase      = MmPciBase (
                   DEFAULT_PCI_BUS_NUMBER_PCH,
                   PCI_DEVICE_NUMBER_PCH_PMC,
                   PCI_FUNCTION_NUMBER_PCH_PMC
                   );
  if (MmioRead16 (PmcBase) == 0xFFFF) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }
  *Address = MmioRead32 (PmcBase + R_PCH_PMC_PWRM_BASE) & B_PCH_PMC_PWRM_BASE_BAR;
  return EFI_SUCCESS;
}

/**
  Set PCH TCO base address.
  This cycle decoding is allowed to set when DMIC.SRL is 0.
  Programming steps:
  1. set Smbus PCI offset 54h [8] to enable TCO base address.
  2. program Smbus PCI offset 50h [15:5] to TCO base address.
  3. set Smbus PCI offset 54h [8] to enable TCO base address.
  4. program "TCO Base Address" PCR[DMI] + 2778h[15:5, 1] to [Smbus PCI offset 50h[15:5], 1].

  @param[in] Address                    Address for TCO base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
EFIAPI
PchTcoBaseSet (
  IN  UINT16                            Address
  )
{
  UINTN                                 SmbusBase;
  UINT32                                Dmic;

  if ((Address & ~B_PCH_SMBUS_TCOBASE_BAR) != 0) {
    DEBUG((DEBUG_ERROR, "PchTcoBaseSet Error. Invalid Address: %x.\n", Address));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  PchPcrRead32 (PID_DMI, R_PCH_PCR_DMI_DMIC, &Dmic);
  if ((Dmic & B_PCH_PCR_DMI_DMIC_SRL) != 0) {
    DEBUG((DEBUG_ERROR, "PchTcoBaseSet Error. DMIC.SRL is set.\n"));
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  SmbusBase = MmPciBase (
                DEFAULT_PCI_BUS_NUMBER_PCH,
                PCI_DEVICE_NUMBER_PCH_SMBUS,
                PCI_FUNCTION_NUMBER_PCH_SMBUS
                );
  if (MmioRead16 (SmbusBase) == 0xFFFF) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }
  //
  // Verify TCO base is not locked.
  //
  if ((MmioRead8 (SmbusBase + R_PCH_SMBUS_TCOCTL) & B_PCH_SMBUS_TCOCTL_TCO_BASE_LOCK) != 0) {
    ASSERT (FALSE);
    return EFI_DEVICE_ERROR;
  }
  //
  // Disable TCO in SMBUS Device first before changing base address.
  //
  MmioAnd8 (
    SmbusBase + R_PCH_SMBUS_TCOCTL + 1,
    (UINT8) ~(B_PCH_SMBUS_TCOCTL_TCO_BASE_EN >> 8)
    );
  //
  // Program TCO in SMBUS Device
  //
  MmioAndThenOr16 (
    SmbusBase + R_PCH_SMBUS_TCOBASE,
    (UINT16) (~B_PCH_SMBUS_TCOBASE_BAR),
    Address
    );
  //
  // Enable TCO in SMBUS Device
  //
  MmioOr8 (
    SmbusBase + R_PCH_SMBUS_TCOCTL + 1,
    (B_PCH_SMBUS_TCOCTL_TCO_BASE_EN >> 8)
    );
  //
  // Program "TCO Base Address" PCR[DMI] + 2778h[15:5, 1] to [SMBUS PCI offset 50h[15:5], 1].
  //
  PchPcrWrite16 (
    PID_DMI, R_PCH_PCR_DMI_TCOBASE,
    (Address | BIT1)
    );

  return EFI_SUCCESS;
}

/**
  Get PCH TCO base address.

  @param[out] Address                   Address of TCO base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid pointer passed.
**/
EFI_STATUS
EFIAPI
PchTcoBaseGet (
  OUT UINT16                            *Address
  )
{
  if (Address == NULL) {
    DEBUG((DEBUG_ERROR, "PchTcoBaseGet Error. Invalid pointer.\n"));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  //
  // Read "TCO Base Address" PCR[DMI] + 2778h[15:5]
  // Don't read TCO base address from SMBUS PCI register since SMBUS might be disabled.
  //
  PchPcrRead16 (
    PID_DMI, R_PCH_PCR_DMI_TCOBASE,
    Address
    );
  *Address &= B_PCH_PCR_DMI_TCOBASE_TCOBA;

  return EFI_SUCCESS;
}

/**
  Set PCH LPC/eSPI generic IO range.
  For generic IO range, the base address must align to 4 and less than 0xFFFF, and the length must be power of 2
  and less than or equal to 256. Moreover, the address must be length aligned.
  This function basically checks the address and length, which should not overlap with all other generic ranges.
  If no more generic range register available, it returns out of resource error.
  This cycle decoding is allowed to set when DMIC.SRL is 0.
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
  2. Program LPC/eSPI Generic IO Range #, PCR[DMI] + 2730h ~ 273Fh to the same value programmed in LPC/eSPI PCI Offset 84h~93h.

  @param[in] Address                    Address for generic IO range base address.
  @param[in] Length                     Length of generic IO range.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address or length passed.
  @retval EFI_OUT_OF_RESOURCES          No more generic range available.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
EFIAPI
PchLpcGenIoRangeSet (
  IN  UINT16                            Address,
  IN  UINTN                             Length
  , IN  UINT8                           SlaveDevice
  )
{
  EFI_STATUS                            Status;
  PCH_LPC_GEN_IO_RANGE_LIST             LpcGenIoRangeList;
  UINTN                                 LpcBase;
  UINTN                                 Index;
  UINTN                                 BaseAddr;
  UINTN                                 MaskLength;
  UINTN                                 TempMaxAddr;
  UINT32                                Data32;
  UINTN                                 ArraySize;
  static struct EXCEPT_RANGE {
    UINT8 Start;
    UINT8 Length;
  } ExceptRanges[] = { {0x00, 0x20}, {0x44, 0x08}, {0x54, 0x0C}, {0x68, 0x08}, {0x80, 0x10}, {0xC0, 0x40} };

  Index = 0;
  //
  // Note: Inside this function, don't use debug print since it's could used before debug print ready.
  //

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
      ((Address & (UINT16)~B_PCH_LPC_GENX_DEC_IOBAR) != 0) ||
      (Length > 256))
  {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  if (Address < 0x100) {
    ArraySize = sizeof (ExceptRanges) / sizeof (struct EXCEPT_RANGE);
    for (Index = 0; Index < ArraySize; Index++) {
      if ((Address >= ExceptRanges[Index].Start) &&
          ((Address + Length) <= ((UINTN)ExceptRanges[Index].Start + (UINTN)ExceptRanges[Index].Length)))
      {
        break;
      }
    }
    if (Index >= ArraySize) {
      ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
    }
  }

  //
  // check if range overlap
  //
  Status  = PchLpcGenIoRangeGet (&LpcGenIoRangeList, SlaveDevice);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return Status;
  }
  if (SlaveDevice == LPC_ESPI_FIRST_SLAVE) {
    for (Index = 0; Index < PCH_LPC_GEN_IO_RANGE_MAX; Index++) {
      BaseAddr = LpcGenIoRangeList.Range[Index].BaseAddr;
      MaskLength   = LpcGenIoRangeList.Range[Index].Length;
      if (BaseAddr == 0) {
        continue;
      }
      if (((Address >= BaseAddr) && (Address < (BaseAddr + MaskLength))) ||
          (((Address + Length) > BaseAddr) && ((Address + Length) <= (BaseAddr + MaskLength))))
      {
        if ((Address >= BaseAddr) && (Length <= MaskLength)) {
          //
          // return SUCCESS while range is covered.
          //
          return EFI_SUCCESS;
        }

        if ((Address + Length) > (BaseAddr + MaskLength)) {
          TempMaxAddr = Address + Length;
        } else {
          TempMaxAddr = BaseAddr + MaskLength;
        }
        if (Address > BaseAddr) {
          Address = (UINT16) BaseAddr;
        }
        Length = TempMaxAddr - Address;
        break;
      }
    }
    //
    // If no range overlap
    //
    if (Index >= PCH_LPC_GEN_IO_RANGE_MAX) {
      //
      // Find a empty register
      //
      for (Index = 0; Index < PCH_LPC_GEN_IO_RANGE_MAX; Index++) {
        BaseAddr = LpcGenIoRangeList.Range[Index].BaseAddr;
        if (BaseAddr == 0) {
          break;
        }
      }
      if (Index >= PCH_LPC_GEN_IO_RANGE_MAX) {
        return EFI_OUT_OF_RESOURCES;
      }
    }
  } else {
    BaseAddr = LpcGenIoRangeList.Range[0].BaseAddr;
    MaskLength   = LpcGenIoRangeList.Range[0].Length;
    if (BaseAddr != 0) {
      if (((Address >= BaseAddr) && (Address < (BaseAddr + MaskLength))) ||
          (((Address + Length) > BaseAddr) && ((Address + Length) <= (BaseAddr + MaskLength))))
      {
        if ((Address >= BaseAddr) && (Length <= MaskLength)) {
          //
          // return SUCCESS while range is covered.
          //
          return EFI_SUCCESS;
        } else {
          return EFI_OUT_OF_RESOURCES;
        }
      }
    }
  }
  //
  // This cycle decoding is only allowed to set when DMIC.SRL is 0.
  //
  PchPcrRead32 (PID_DMI, R_PCH_PCR_DMI_DMIC, &Data32);
  if ((Data32 & B_PCH_PCR_DMI_DMIC_SRL) != 0) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  //
  // Program LPC/eSPI generic IO range register accordingly.
  //
  LpcBase = MmPciBase (
              DEFAULT_PCI_BUS_NUMBER_PCH,
              PCI_DEVICE_NUMBER_PCH_LPC,
              PCI_FUNCTION_NUMBER_PCH_LPC
              );
  Data32 =  (UINT32) (((Length - 1) << 16) & B_PCH_LPC_GENX_DEC_IODRA);
  Data32 |= (UINT32) Address;
  Data32 |= B_PCH_LPC_GENX_DEC_EN;

  if (SlaveDevice == LPC_ESPI_FIRST_SLAVE) {
    //
    // Program LPC/eSPI PCI Offset 84h ~ 93h of Mask, Address, and Enable.
    //
    MmioWrite32 (
      LpcBase + R_PCH_LPC_GEN1_DEC + Index * 4,
      Data32
      );
    //
    // Program LPC Generic IO Range #, PCR[DMI] + 2730h ~ 273Fh to the same value programmed in LPC/eSPI PCI Offset 84h~93h.
    //
    PchPcrWrite32 (
      PID_DMI, (UINT16) (R_PCH_PCR_DMI_LPCLGIR1 + Index * 4),
      Data32
      );
  } else {
    ASSERT(FALSE);
  }
  return EFI_SUCCESS;
}

/**
  Get PCH LPC/eSPI generic IO range list.
  This function returns a list of base address, length, and enable for all LPC/eSPI generic IO range regsiters.

  @param[out] LpcGenIoRangeList         Return all LPC/eSPI generic IO range register status.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
**/
EFI_STATUS
EFIAPI
PchLpcGenIoRangeGet (
  OUT PCH_LPC_GEN_IO_RANGE_LIST         *LpcGenIoRangeList
  , IN  UINT8                           SlaveDevice
  )
{
  UINTN                                 Index;
  UINTN                                 LpcBase;
  UINT32                                Data32;

  //
  // Note: Inside this function, don't use debug print since it's could used before debug print ready.
  //

  if (LpcGenIoRangeList == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  LpcBase      = MmPciBase (
                   DEFAULT_PCI_BUS_NUMBER_PCH,
                   PCI_DEVICE_NUMBER_PCH_LPC,
                   PCI_FUNCTION_NUMBER_PCH_LPC
                   );

  if (SlaveDevice == LPC_ESPI_FIRST_SLAVE) {
    for (Index = 0; Index < PCH_LPC_GEN_IO_RANGE_MAX; Index++) {
      Data32 = MmioRead32 (LpcBase + R_PCH_LPC_GEN1_DEC + Index * 4);
      LpcGenIoRangeList->Range[Index].BaseAddr = Data32 & B_PCH_LPC_GENX_DEC_IOBAR;
      LpcGenIoRangeList->Range[Index].Length   = ((Data32 & B_PCH_LPC_GENX_DEC_IODRA) >> 16) + 4;
      LpcGenIoRangeList->Range[Index].Enable   = Data32 & B_PCH_LPC_GENX_DEC_EN;
    }
  } else {
    ASSERT(FALSE);
  }
  return EFI_SUCCESS;
}

/**
  Set PCH LPC/eSPI memory range decoding.
  This cycle decoding is allowed to set when DMIC.SRL is 0.
  Programming steps:
  1. Program LPC/eSPI PCI Offset 98h [0] to [0] to disable memory decoding first before changing base address.
  2. Program LPC/eSPI PCI Offset 98h [31:16, 0] to [Address, 1].
  3. Program LPC/eSPI Memory Range, PCR[DMI] + 2740h to the same value programmed in LPC/eSPI PCI Offset 98h.

  @param[in] Address                    Address for memory base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address or length passed.
  @retval EFI_OUT_OF_RESOURCES          No more generic range available.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
EFIAPI
PchLpcMemRangeSet (
  IN  UINT32                            Address
  , IN  UINT8                           SlaveDevice
  )
{
  UINTN                                 LpcBase;
  UINT32                                Dmic;
  UINTN                                 LpcReg;
  UINT16                                DmiReg;

  if ((Address & (~B_PCH_LPC_LGMR_MA)) != 0) {
    DEBUG((DEBUG_ERROR, "PchLpcMemRangeSet Error. Invalid Address: %x.\n", Address));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  PchPcrRead32 (PID_DMI, R_PCH_PCR_DMI_DMIC, &Dmic);
  if ((Dmic & B_PCH_PCR_DMI_DMIC_SRL) != 0) {
    DEBUG((DEBUG_ERROR, "PchLpcMemRangeSet Error. DMIC.SRL is set.\n"));
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  LpcBase      = MmPciBase (
                   DEFAULT_PCI_BUS_NUMBER_PCH,
                   PCI_DEVICE_NUMBER_PCH_LPC,
                   PCI_FUNCTION_NUMBER_PCH_LPC
                   );
  if (SlaveDevice == ESPI_SECONDARY_SLAVE) {
    ASSERT(FALSE);
  } else {
    LpcReg = LpcBase + R_PCH_LPC_LGMR;
    DmiReg = R_PCH_PCR_DMI_LPCGMR;
  }
  //
  // Program LPC/eSPI PCI Offset 98h [0] (LPC/ePSI first slave) or A8h [0] (eSPI secondary slave) to [0] to disable memory decoding first before changing base address.
  //
  MmioAnd32 (
    LpcReg,
    (UINT32) ~B_PCH_LPC_LGMR_LMRD_EN
    );
  //
  // Program LPC/eSPI PCI Offset 98h [31:16, 0] (LPC/ eSPI first slave) or A8h [31:16, 0] (eSPI secondary slave) to [Address, 1].
  //
  MmioWrite32 (
    LpcReg,
    (Address | B_PCH_LPC_LGMR_LMRD_EN)
    );
  //
  // Program LPC Memory Range, PCR[DMI] + 2740h (LPC/eSPI first slave) or 27C0h (eSPI secondary slave) to the same value programmed in LPC/eSPI PCI Offset 98h.
  //
  PchPcrWrite32 (
    PID_DMI, DmiReg,
    (Address | B_PCH_LPC_LGMR_LMRD_EN)
    );
  return EFI_SUCCESS;
}

/**
  Get PCH LPC/eSPI memory range decoding address.

  @param[out] Address                   Address of LPC/eSPI memory decoding base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
**/
EFI_STATUS
EFIAPI
PchLpcMemRangeGet (
  OUT UINT32                            *Address
    , IN  UINT8                           SlaveDevice
  )
{
  UINTN                                 LpcBase;

  if (Address == NULL) {
    DEBUG((DEBUG_ERROR, "PchLpcMemRangeGet Error. Invalid pointer.\n"));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  LpcBase      = MmPciBase (
                   DEFAULT_PCI_BUS_NUMBER_PCH,
                   PCI_DEVICE_NUMBER_PCH_LPC,
                   PCI_FUNCTION_NUMBER_PCH_LPC
                   );
  if (SlaveDevice == LPC_ESPI_FIRST_SLAVE) {
    *Address = MmioRead32 (LpcBase + R_PCH_LPC_LGMR) & B_PCH_LPC_LGMR_MA;
  } else {
    ASSERT(FALSE);
  }
  return EFI_SUCCESS;
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
  This cycle decoding is allowed to set when DMIC.SRL is 0.
  Programming steps:
  1. if GCS.BBS is 0 (SPI), program SPI offset D8h to BiosDecodeEnable.
     if GCS.BBS is 1 (LPC/eSPi), program LPC offset D8h to BiosDecodeEnable.
  2. program LPC BIOS Decode Enable, PCR[DMI] + 2744h to the same value programmed in LPC or SPI Offset D8h.

  @param[in] BiosDecodeEnable           Bios decode enable setting.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
EFIAPI
PchBiosDecodeEnableSet (
  IN  UINT16                            BiosDecodeEnable
  )
{
  UINTN                                 BaseAddr;
  UINT32                                DmiGcsBbs;
  UINT32                                Dmic;

  PchPcrRead32 (PID_DMI, R_PCH_PCR_DMI_DMIC, &Dmic);
  if ((Dmic & B_PCH_PCR_DMI_DMIC_SRL) != 0) {
    DEBUG((DEBUG_ERROR, "PchBiosDecodeEnableSet Error. DMIC.SRL is set.\n"));
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  PchPcrRead32 (PID_DMI, R_PCH_PCR_DMI_GCS, &DmiGcsBbs);
  DmiGcsBbs &= B_PCH_PCR_DMI_BBS;
  //
  // Check General Control and Status (GCS) [10]
  // '0': SPI
  // '1': LPC/eSPI
  //
  if (DmiGcsBbs == 0) {
    BaseAddr = MmPciBase (
                 DEFAULT_PCI_BUS_NUMBER_PCH,
                 PCI_DEVICE_NUMBER_PCH_SPI,
                 PCI_FUNCTION_NUMBER_PCH_SPI
                 );
    //
    // if GCS.BBS is 0 (SPI), program SPI offset D8h to BiosDecodeEnable.
    //
    MmioWrite16 (BaseAddr + R_PCH_SPI_BDE, BiosDecodeEnable);
  } else {
    BaseAddr = MmPciBase (
                 DEFAULT_PCI_BUS_NUMBER_PCH,
                 PCI_DEVICE_NUMBER_PCH_LPC,
                 PCI_FUNCTION_NUMBER_PCH_LPC
                 );
    //
    // if GCS.BBS is 1 (LPC/eSPi), program LPC offset D8h to BiosDecodeEnable.
    //
    MmioWrite16 (BaseAddr + R_PCH_LPC_BDE, BiosDecodeEnable);
  }

  //
  // program LPC BIOS Decode Enable, PCR[DMI] + 2744h to the same value programmed in LPC or SPI Offset D8h.
  //
  PchPcrWrite16 (PID_DMI, R_PCH_PCR_DMI_LPCBDE, BiosDecodeEnable);
  return EFI_SUCCESS;
}

/**
  Set PCH LPC/eSPI IO decode ranges.
  Program LPC/eSPI I/O Decode Ranges, PCR[DMI] + 2770h[15:0] to the same value programmed in LPC/eSPI PCI offset 80h.
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
EFIAPI
PchLpcIoDecodeRangesSet (
  IN  UINT16                            LpcIoDecodeRanges
  )
{
  UINTN                                 LpcBaseAddr;
  UINT32                                Dmic;

  //
  // Note: Inside this function, don't use debug print since it's could used before debug print ready.
  //

  LpcBaseAddr  = MmPciBase (
                   DEFAULT_PCI_BUS_NUMBER_PCH,
                   PCI_DEVICE_NUMBER_PCH_LPC,
                   PCI_FUNCTION_NUMBER_PCH_LPC
                   );
  //
  // check if setting is identical
  //
  if (LpcIoDecodeRanges == MmioRead16 (LpcBaseAddr + R_PCH_LPC_IOD)) {
    return EFI_SUCCESS;
  }

  //
  // This cycle decoding is only allowed to set when DMIC.SRL is 0.
  //
  PchPcrRead32 (PID_DMI, R_PCH_PCR_DMI_DMIC, &Dmic);
  if ((Dmic & B_PCH_PCR_DMI_DMIC_SRL) != 0) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  //
  // program LPC/eSPI PCI offset 80h.
  //
  MmioWrite16 (LpcBaseAddr + R_PCH_LPC_IOD, LpcIoDecodeRanges);

  //
  // program LPC I/O Decode Ranges, PCR[DMI] + 2770h[15:0] to the same value programmed in LPC/eSPI PCI offset 80h.
  //
  PchPcrWrite16 (PID_DMI, R_PCH_PCR_DMI_LPCIOD, LpcIoDecodeRanges);
  return EFI_SUCCESS;
}

/**
  Set PCH LPC/eSPI IO enable decoding.
  Setup LPC/eSPI I/O Enables, PCR[DMI] + 2774h[15:0] to the same value program in LPC/eSPI PCI offset 82h.
  Note: Bit[15:10] of the source decode register is Read-Only. The IO range indicated by the Enables field
  in LPC/eSPI PCI offset 82h[13:10] is always forwarded by DMI to subtractive agent for handling.
  Please check EDS for detail of Lpc/eSPI IO decode ranges bit definition.

  @param[in] LpcIoEnableDecoding        LPC/eSPI IO enable decoding bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
EFIAPI
PchLpcIoEnableDecodingSet (
  IN  UINT16                            LpcIoEnableDecoding
  , IN  UINT8                           SlaveDevice
  )
{
  UINTN                                 LpcBaseAddr;
  UINT32                                Dmic;
  UINTN                                 LpcReg;

  if (SlaveDevice == LPC_ESPI_FIRST_SLAVE) {
    LpcReg = R_PCH_LPC_IOE;
  } else {
    ASSERT(FALSE);
  }

  //
  // Note: Inside this function, don't use debug print since it's could used before debug print ready.
  //

  LpcBaseAddr  = MmPciBase (
                   DEFAULT_PCI_BUS_NUMBER_PCH,
                   PCI_DEVICE_NUMBER_PCH_LPC,
                   PCI_FUNCTION_NUMBER_PCH_LPC
                   );
  if (LpcIoEnableDecoding == MmioRead16 (LpcBaseAddr + LpcReg)) {
    return EFI_SUCCESS;
  }
  //
  // This cycle decoding is only allowed to set when DMIC.SRL is 0.
  //
  PchPcrRead32 (PID_DMI, R_PCH_PCR_DMI_DMIC, &Dmic);
  if ((Dmic & B_PCH_PCR_DMI_DMIC_SRL) != 0) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  //
  // program PCI offset 82h for LPC/eSPI CS#0 or offset A0h for eSPI CS#1.
  //
  MmioWrite16 (LpcBaseAddr + LpcReg, LpcIoEnableDecoding);

  if (SlaveDevice == ESPI_SECONDARY_SLAVE) {
    //
    // For eSPI CS#1 device program PCI offset 82h respectively
    //
    MmioWrite16 (LpcBaseAddr + R_PCH_LPC_IOE, (LpcIoEnableDecoding | MmioRead16(LpcBaseAddr + R_PCH_LPC_IOE)));
  }

  //
  // program LPC I/O Decode Ranges, PCR[DMI] + 2774h[15:0] to the same value programmed in LPC/eSPI PCI offset 82h.
  //
  PchPcrWrite16 (PID_DMI, R_PCH_PCR_DMI_LPCIOE, LpcIoEnableDecoding);
  return EFI_SUCCESS;
}


//
// PCH-LP RPR destination ID table
//
UINT16 PchLpRprDidTable[] = {
  0x2188, // Dest ID of RP1
  0x2189, // Dest ID of RP2
  0x218A, // Dest ID of RP3
  0x218B, // Dest ID of RP4
  0x2198, // Dest ID of RP5
  0x2199, // Dest ID of RP6
  0x219A, // Dest ID of RP7
  0x219B, // Dest ID of RP8
  0x21A8, // Dest ID of RP9
  0x21A9, // Dest ID of RP10
  0x21AA, // Dest ID of RP11
  0x21AB  // Dest ID of RP12
};

//
// PCH-H RPR destination ID table
//
UINT16 PchHRprDidTable[] = {
  0x2180, // Dest ID of RP1
  0x2181, // Dest ID of RP2
  0x2182, // Dest ID of RP3
  0x2183, // Dest ID of RP4
  0x2188, // Dest ID of RP5
  0x2189, // Dest ID of RP6
  0x218A, // Dest ID of RP7
  0x218B, // Dest ID of RP8
  0x2198, // Dest ID of RP9
  0x2199, // Dest ID of RP10
  0x219A, // Dest ID of RP11
  0x219B, // Dest ID of RP12
  0x21A8, // Dest ID of RP13
  0x21A9, // Dest ID of RP14
  0x21AA, // Dest ID of RP15
  0x21AB, // Dest ID of RP16
  0x21B8, // Dest ID of RP17
  0x21B9, // Dest ID of RP18
  0x21BA, // Dest ID of RP19
  0x21BB, // Dest ID of RP20
};

/**
  Set PCH IO port 80h cycle decoding to PCIE root port.
  System BIOS is likely to do this very soon after reset before PCI bus enumeration.
  This cycle decoding is allowed to set when DMIC.SRL is 0.
  Programming steps:
  1. Program "RPR Destination ID", PCR[DMI] + 274Ch[31:16] to the Dest ID of RP.
  2. Program "Reserved Page Route", PCR[DMI] + 274Ch[11] to '1'. Use byte write on GCS+1 and leave the BILD bit which is RWO.

  @param[in] RpPhyNumber                PCIE root port physical number.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
EFIAPI
PchIoPort80DecodeSet (
  IN  UINTN                             RpPhyNumber
  )
{
  UINT32                                Dmic;
  UINT16                                *PchRprDidTable;

  PchPcrRead32 (PID_DMI, R_PCH_PCR_DMI_DMIC, &Dmic);
  if ((Dmic & B_PCH_PCR_DMI_DMIC_SRL) != 0) {
    DEBUG((DEBUG_ERROR, "PchIoPort80DecodeSet Error. DMIC.SRL is set.\n"));
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  ///
  /// IO port 80h is typically used by decoder/LED hardware for debug purposes.
  /// By default PCH will forward IO port 80h cycles to LPC bus. The Reserved Page Route (RPR) bit
  /// of General Control and Status register, located at PCR[DMI] + 274Ch[11] , allows software to
  /// re-direct IO port 80h cycles to PCIe bus so that a target (for example, a debug card) on
  /// PCIe bus can receive and claim these cycles.
  /// The "RPR Destination ID", PCR[DMI] + 274Ch[31:16] need to be set accordingly to point
  /// to the root port that decode this range. Reading from Port 80h may not return valid values
  /// if the POST-card itself do not shadow the writes. Unlike LPC, PCIe does not shadow the Port 80 writes.
  ///

  if (GetPchSeries () == PchLp) {
    PchRprDidTable = PchLpRprDidTable;
  } else {
    PchRprDidTable = PchHRprDidTable;
  }

  //
  // Program "RPR Destination ID", PCR[DMI] + 274Ch[31:16] to the Dest ID of RP.
  //
  PchPcrWrite16 (PID_DMI, R_PCH_PCR_DMI_GCS + 2, PchRprDidTable[RpPhyNumber]);
  //
  // Program "Reserved Page Route", PCR[DMI] + 274Ch[11] to '1'.
  // Use byte write on GCS+1 and leave the BILD bit which is RWO.
  //
  PchPcrAndThenOr8 (PID_DMI, R_PCH_PCR_DMI_GCS + 1, 0xFF, (B_PCH_PCR_DMI_RPR >> 8));

  return EFI_SUCCESS;
}

/**
  Get IO APIC regsiters base address.
  It returns IO APIC INDEX, DATA, and EOI regsiter address once the parameter is not NULL.
  This function will be unavailable after P2SB is hidden by PSF.

  @param[out] IoApicIndex               Buffer of IO APIC INDEX regsiter address
  @param[out] IoApicData                Buffer of IO APIC DATA regsiter address

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
PchIoApicBaseGet (
  OPTIONAL OUT UINT32                   *IoApicIndex,
  OPTIONAL OUT UINT32                   *IoApicData
  )
{
  EFI_STATUS                            Status;
  UINT16                                RegIoac;
  UINT32                                RangeSelect;

  Status = PchP2sbCfgGet16 (R_PCH_P2SB_IOAC, &RegIoac);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  RangeSelect = (RegIoac & B_PCH_P2SB_IOAC_ASEL) << N_PCH_IO_APIC_ASEL;

  if (IoApicIndex != NULL) {
    *IoApicIndex = R_PCH_IO_APIC_INDEX + RangeSelect;
  }
  if (IoApicData != NULL) {
    *IoApicData  = R_PCH_IO_APIC_DATA + RangeSelect;
  }

  return EFI_SUCCESS;
}

/**
  Get HPET base address.
  This function will be unavailable after P2SB is hidden by PSF.

  @param[out] HpetBase                  Buffer of HPET base address

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid offset passed.
**/
EFI_STATUS
PchHpetBaseGet (
  OUT UINT32                            *HpetBase
  )
{
  EFI_STATUS                            Status;
  UINT8                                 RegHptc;

  if (HpetBase == NULL) {
    DEBUG((DEBUG_ERROR, "PchHpetBaseGet Error. Invalid pointer.\n"));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Status = PchP2sbCfgGet8 (R_PCH_P2SB_HPTC, &RegHptc);

  switch (RegHptc & B_PCH_P2SB_HPTC_AS) {
  case 0:
    *HpetBase = V_PCH_HPET_BASE0;
    break;
  case 1:
    *HpetBase = V_PCH_HPET_BASE1;
    break;
  case 2:
    *HpetBase = V_PCH_HPET_BASE2;
    break;
  case 3:
    *HpetBase = V_PCH_HPET_BASE3;
    break;
  default:
    break;
 }

 return Status;
}
