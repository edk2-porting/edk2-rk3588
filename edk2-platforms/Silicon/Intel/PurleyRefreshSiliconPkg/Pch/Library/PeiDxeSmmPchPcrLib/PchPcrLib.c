/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
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

/**
  Read PCR register. (This is internal function)
  It returns PCR register and size in 1byte/2bytes/4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid                       Port ID
  @param[in]  Offset                    Register offset of this Port ID
  @param[in]  Size                      Size for read. Must be 1 or 2 or 4.
  @param[out] OutData                   Buffer of Output Data. Must be the same size as Size parameter.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid offset passed.
**/
STATIC
EFI_STATUS
PchPcrRead (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT16                            Offset,
  IN  UINTN                             Size,
  OUT UINT32                            *OutData
  )
{
  if ((Offset & (Size - 1)) != 0) {
    DEBUG ((DEBUG_ERROR, "PchPcrRead error. Invalid Offset: %x Size: %x", Offset, Size));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  //
  // @todo SKL PCH: check PID that not expected to use this routine, such as CAM_FLIS, CSME0
  //

  switch (Size) {
  case 4:
    *(UINT32*)OutData = MmioRead32 (PCH_PCR_ADDRESS (Pid, Offset));
    break;
  case 2:
    *(UINT16*)OutData = MmioRead16 (PCH_PCR_ADDRESS (Pid, Offset));
    break;
  case 1:
    *(UINT8*) OutData = MmioRead8  (PCH_PCR_ADDRESS (Pid, Offset));
    break;
  default:
    break;
  }
  return EFI_SUCCESS;
}

/**
  Read PCR register.
  It returns PCR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid                       Port ID
  @param[in]  Offset                    Register offset of this Port ID
  @param[out] OutData                   Buffer of Output Data. Must be the same size as Size parameter.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid offset passed.
**/
EFI_STATUS
PchPcrRead32 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT16                            Offset,
  OUT UINT32                            *OutData
  )
{
  return PchPcrRead (Pid, Offset, 4, (UINT32*) OutData);
}

/**
  Read PCR register.
  It returns PCR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid                       Port ID
  @param[in]  Offset                    Register offset of this Port ID
  @param[out] OutData                   Buffer of Output Data. Must be the same size as Size parameter.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid offset passed.
**/
EFI_STATUS
PchPcrRead16 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT16                            Offset,
  OUT UINT16                            *OutData
  )
{
  return PchPcrRead (Pid, Offset, 2, (UINT32*) OutData);
}

/**
  Read PCR register.
  It returns PCR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid                       Port ID
  @param[in]  Offset                    Register offset of this Port ID
  @param[out] OutData                   Buffer of Output Data. Must be the same size as Size parameter.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid offset passed.
**/
EFI_STATUS
PchPcrRead8 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT16                            Offset,
  OUT UINT8                             *OutData
  )
{
  return PchPcrRead (Pid, Offset, 1, (UINT32*) OutData);
}

BOOLEAN
PchPcrWriteMmioCheck (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT16                            Offset
  )
{
DEBUG_CODE_BEGIN();
  PCH_SERIES  PchSeries;

  PchSeries = GetPchSeries ();
  //
  // 1. USB2 AFE register must use SBI method
  //

  //
  // 2. GPIO unlock register field must use SBI method
  //
  if (Pid == PID_GPIOCOM0) {
    if (((PchSeries == PchLp) &&
      ((Offset == R_PCH_LP_PCR_GPIO_GPP_A_PADCFGLOCK)   ||
       (Offset == R_PCH_LP_PCR_GPIO_GPP_A_PADCFGLOCKTX) ||
       (Offset == R_PCH_LP_PCR_GPIO_GPP_B_PADCFGLOCK)   ||
       (Offset == R_PCH_LP_PCR_GPIO_GPP_B_PADCFGLOCKTX))) ||
       ((PchSeries == PchH) &&
      ((Offset == R_PCH_H_PCR_GPIO_GPP_A_PADCFGLOCK)   ||
       (Offset == R_PCH_H_PCR_GPIO_GPP_A_PADCFGLOCKTX) ||
       (Offset == R_PCH_H_PCR_GPIO_GPP_B_PADCFGLOCK)   ||
       (Offset == R_PCH_H_PCR_GPIO_GPP_B_PADCFGLOCKTX))))
    {
      return FALSE;
    }
  }
  if (Pid == PID_GPIOCOM1) {
    if (((PchSeries == PchLp) &&
      ((Offset == R_PCH_LP_PCR_GPIO_GPP_C_PADCFGLOCK)   ||
       (Offset == R_PCH_LP_PCR_GPIO_GPP_C_PADCFGLOCKTX) ||
       (Offset == R_PCH_LP_PCR_GPIO_GPP_D_PADCFGLOCK)   ||
       (Offset == R_PCH_LP_PCR_GPIO_GPP_D_PADCFGLOCKTX) ||
       (Offset == R_PCH_LP_PCR_GPIO_GPP_E_PADCFGLOCK)   ||
       (Offset == R_PCH_LP_PCR_GPIO_GPP_E_PADCFGLOCKTX))) ||
       ((PchSeries == PchH) &&
      ((Offset == R_PCH_H_PCR_GPIO_GPP_C_PADCFGLOCK)   ||
       (Offset == R_PCH_H_PCR_GPIO_GPP_C_PADCFGLOCKTX) ||
       (Offset == R_PCH_H_PCR_GPIO_GPP_D_PADCFGLOCK)   ||
       (Offset == R_PCH_H_PCR_GPIO_GPP_D_PADCFGLOCKTX) ||
       (Offset == R_PCH_H_PCR_GPIO_GPP_E_PADCFGLOCK)   ||
       (Offset == R_PCH_H_PCR_GPIO_GPP_E_PADCFGLOCKTX) ||
       (Offset == R_PCH_H_PCR_GPIO_GPP_F_PADCFGLOCK)   ||
       (Offset == R_PCH_H_PCR_GPIO_GPP_F_PADCFGLOCKTX) ||
       (Offset == R_PCH_H_PCR_GPIO_GPP_G_PADCFGLOCK)   ||
       (Offset == R_PCH_H_PCR_GPIO_GPP_G_PADCFGLOCKTX) ||
       (Offset == R_PCH_H_PCR_GPIO_GPP_H_PADCFGLOCK)   ||
       (Offset == R_PCH_H_PCR_GPIO_GPP_H_PADCFGLOCKTX))))
    {
      return FALSE;
    }
  }
  if (Pid == PID_GPIOCOM2) {
    if (((PchSeries == PchLp) &&
      ((Offset == R_PCH_LP_PCR_GPIO_GPD_PADCFGLOCK)   ||
       (Offset == R_PCH_LP_PCR_GPIO_GPD_PADCFGLOCKTX))) ||
       ((PchSeries == PchH) &&
      ((Offset == R_PCH_H_PCR_GPIO_GPD_PADCFGLOCK)   ||
       (Offset == R_PCH_H_PCR_GPIO_GPD_PADCFGLOCKTX))))
    {
      return FALSE;
    }
  }
  if (Pid == PID_GPIOCOM3) {
    if (((PchSeries == PchLp) &&
      ((Offset == R_PCH_LP_PCR_GPIO_GPP_F_PADCFGLOCK)   ||
       (Offset == R_PCH_LP_PCR_GPIO_GPP_F_PADCFGLOCKTX) ||
       (Offset == R_PCH_LP_PCR_GPIO_GPP_G_PADCFGLOCK)   ||
       (Offset == R_PCH_LP_PCR_GPIO_GPP_G_PADCFGLOCKTX))) ||
       ((PchSeries == PchH) &&
      ((Offset == R_PCH_H_PCR_GPIO_GPP_I_PADCFGLOCK)   ||
       (Offset == R_PCH_H_PCR_GPIO_GPP_I_PADCFGLOCKTX))))
    {
      return FALSE;
    }
  }
  //
  // 3. CIO2 FLIS regsiter must use SBI method
  //

  //
  // 4. CSME0 based PCR should use the SBI method due to the FID requirement
  //
  if (Pid == PID_CSME0) {
    return FALSE;
  }
DEBUG_CODE_END();
  return TRUE;

}

/**
  Write PCR register. (This is internal function)
  It programs PCR register and size in 1byte/2bytes/4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid                       Port ID
  @param[in]  Offset                    Register offset of Port ID.
  @param[in]  Size                      Size for read. Must be 1 or 2 or 4.
  @param[in]  AndData                   AND Data. Must be the same size as Size parameter.
  @param[in]  OrData                    OR Data. Must be the same size as Size parameter.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid offset passed.
**/
STATIC
EFI_STATUS
PchPcrWrite (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT16                            Offset,
  IN  UINTN                             Size,
  IN  UINT32                            InData
  )
{
  if ((Offset & (Size - 1)) != 0) {
    DEBUG ((DEBUG_ERROR, "PchPcrWrite error. Invalid Offset: %x Size: %x", Offset, Size));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
DEBUG_CODE_BEGIN();
  if (!PchPcrWriteMmioCheck (Pid, Offset)) {
    DEBUG ((DEBUG_ERROR, "PchPcrWrite error. Pid: %x Offset: %x should access through SBI interface", Pid, Offset));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
DEBUG_CODE_END();

  //
  // Write the PCR register with provided data
  // Then read back PCR register to prevent from back to back write.
  //
  switch (Size) {
    case 4:
      MmioWrite32 (PCH_PCR_ADDRESS (Pid, Offset), (UINT32)InData);
      break;
    case 2:
      MmioWrite16 (PCH_PCR_ADDRESS (Pid, Offset), (UINT16)InData);
      break;
    case 1:
      MmioWrite8  (PCH_PCR_ADDRESS (Pid, Offset), (UINT8) InData);
      break;
    default:
      break;
  }
  MmioRead32  (PCH_PCR_ADDRESS (PID_LPC, R_PCH_PCR_LPC_GCFD));

  return EFI_SUCCESS;
}

/**
  Write PCR register.
  It programs PCR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid                       Port ID
  @param[in]  Offset                    Register offset of Port ID.
  @param[in]  InData                    Input Data. Must be the same size as Size parameter.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid offset passed.
**/
EFI_STATUS
PchPcrWrite32 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT16                            Offset,
  IN  UINT32                            InData
  )
{
  return PchPcrWrite (Pid, Offset, 4, InData);
}

/**
  Write PCR register.
  It programs PCR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid                       Port ID
  @param[in]  Offset                    Register offset of Port ID.
  @param[in]  InData                    Input Data. Must be the same size as Size parameter.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid offset passed.
**/
EFI_STATUS
PchPcrWrite16 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT16                            Offset,
  IN  UINT16                            InData
  )
{
  return PchPcrWrite (Pid, Offset, 2, InData);
}

/**
  Write PCR register.
  It programs PCR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid                       Port ID
  @param[in]  Offset                    Register offset of Port ID.
  @param[in]  InData                    Input Data. Must be the same size as Size parameter.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid offset passed.
**/
EFI_STATUS
PchPcrWrite8 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT16                            Offset,
  IN  UINT8                             InData
  )
{
  return PchPcrWrite (Pid, Offset, 1, InData);
}

/**
  Write PCR register.
  It programs PCR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid                       Port ID
  @param[in]  Offset                    Register offset of Port ID.
  @param[in]  AndData                   AND Data. Must be the same size as Size parameter.
  @param[in]  OrData                    OR Data. Must be the same size as Size parameter.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid offset passed.
**/
EFI_STATUS
PchPcrAndThenOr32 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT16                            Offset,
  IN  UINT32                            AndData,
  IN  UINT32                            OrData
  )
{
  EFI_STATUS                            Status;
  UINT32                                Data32;

  Data32 = 0x00;
  Status  = PchPcrRead (Pid, Offset, 4, &Data32);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Data32 &= AndData;
  Data32 |= OrData;
  Status  = PchPcrWrite (Pid, Offset, 4, Data32);
  return Status;
}

/**
  Write PCR register.
  It programs PCR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid                       Port ID
  @param[in]  Offset                    Register offset of Port ID.
  @param[in]  AndData                   AND Data. Must be the same size as Size parameter.
  @param[in]  OrData                    OR Data. Must be the same size as Size parameter.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid offset passed.
**/
EFI_STATUS
PchPcrAndThenOr16 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT16                            Offset,
  IN  UINT16                            AndData,
  IN  UINT16                            OrData
  )
{
  EFI_STATUS                            Status;
  UINT16                                Data16;

  Data16 = 0x00;
  Status  = PchPcrRead (Pid, Offset, 2, (UINT32*) &Data16);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Data16 &= AndData;
  Data16 |= OrData;
  Status  = PchPcrWrite (Pid, Offset, 2, Data16);
  return Status;
}

/**
  Write PCR register.
  It programs PCR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid                       Port ID
  @param[in]  Offset                    Register offset of Port ID.
  @param[in]  AndData                   AND Data. Must be the same size as Size parameter.
  @param[in]  OrData                    OR Data. Must be the same size as Size parameter.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid offset passed.
**/
EFI_STATUS
PchPcrAndThenOr8 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT16                            Offset,
  IN  UINT8                             AndData,
  IN  UINT8                             OrData
  )
{
  EFI_STATUS                            Status;
  UINT8                                 Data8;

  Status  = PchPcrRead (Pid, Offset, 1, (UINT32*) &Data8);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Data8 &= AndData;
  Data8 |= OrData;
  Status  = PchPcrWrite (Pid, Offset, 1, Data8);
  return Status;
}

