/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <SaPolicyCommon.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PciLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>

#include <Library/PeiSaPolicyLib.h>
#include <Library/BoardInitLib.h>
#include <PchAccess.h>
#include <Library/GpioNativeLib.h>
#include <Library/GpioLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PcdLib.h>
#include <Library/GpioExpanderLib.h>


#include <GpioPinsSklLp.h>
#include <GpioPinsSklH.h>
#include <SioRegs.h>
#include <GpioPinsCnlLp.h>
#include <CpuRegs.h>
//#include <CpuDataStruct.h>
#include <CpuAccess.h>
#include <Register/Cpuid.h>
#include "UpXtremeInit.h"
#include <ConfigBlock.h>
#include <ConfigBlock/MemoryConfig.h>




CONST UINT32 mUpxGpioBomPad[]  = {
  GPIO_CNL_LP_GPP_C10, // BRD_ID2
  GPIO_CNL_LP_GPP_C9,  // BRD_ID1
  GPIO_CNL_LP_GPP_C8,  // BRD_ID0
  GPIO_CNL_LP_GPP_A23, // DDR_ID2
  GPIO_CNL_LP_GPP_A18, // DDR_ID1
  GPIO_CNL_LP_GPP_C11, // DDR_ID0
};

CONST GPIO_INIT_CONFIG mUpxBomGpioTemplate = {
   GPIO_CNL_LP_GPP_C10, {GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}
};

/**
  Prints the  processor information
**/
VOID
PrintCpuInformation (
  VOID
  )
{
  UINT32  MaximumExtendedFunction;
  UINT16              CpuDid;
  UINT32              CpuFamilyModel;
  UINT8               CpuStepping;
  EFI_CPUID_REGISTER  Cpuid;
  //
  // Array to store brand string from 3 brand string leafs with
  // 4 32-bit brand string values per leaf and an extra value to
  // null terminate the string.
  //
  UINT32  BrandString[3 * 4 + 1];
  CHAR8   *AsciiBrandString;
  CHAR16  *UnicodeBrandString;
  UINTN   Length;

  AsmCpuid (CPUID_EXTENDED_FUNCTION, &MaximumExtendedFunction, NULL, NULL, NULL);

  ZeroMem (&BrandString, sizeof (BrandString));
  if (CPUID_BRAND_STRING1 <= MaximumExtendedFunction) {
    AsmCpuid (
      CPUID_BRAND_STRING1,
      &BrandString[0],
      &BrandString[1],
      &BrandString[2],
      &BrandString[3]
      );
  }
  if (CPUID_BRAND_STRING2 <= MaximumExtendedFunction) {
    AsmCpuid (
      CPUID_BRAND_STRING2,
      &BrandString[4],
      &BrandString[5],
      &BrandString[6],
      &BrandString[7]
      );
  }
  if (CPUID_BRAND_STRING3 <= MaximumExtendedFunction) {
    AsmCpuid (
      CPUID_BRAND_STRING3,
      &BrandString[8],
      &BrandString[9],
      &BrandString[10],
      &BrandString[11]
      );
  }

  //
  // Skip spaces at the beginning of the brand string
  //
  for (AsciiBrandString = (CHAR8 *)BrandString; *AsciiBrandString == ' '; AsciiBrandString++);

  DEBUG ((DEBUG_INFO, "Processor Brand String = %a\n", AsciiBrandString));

  //
  // Convert ASCII brand string to an allocated Unicode brand string
  //
  Length = AsciiStrLen (AsciiBrandString) + 1;
  UnicodeBrandString = AllocatePool (Length * sizeof (CHAR16));
  AsciiStrToUnicodeStrS (AsciiBrandString, UnicodeBrandString, Length);

  DEBUG ((DEBUG_INFO, "Processor Unicode Brand String = %s\n", UnicodeBrandString));

  ///
  /// Read the CPUID & DID information
  ///
  AsmCpuid (CPUID_VERSION_INFO, &Cpuid.RegEax, &Cpuid.RegEbx, &Cpuid.RegEcx, &Cpuid.RegEdx);
  CpuFamilyModel = Cpuid.RegEax & CPUID_FULL_FAMILY_MODEL;
  CpuStepping = (Cpuid.RegEax & 0xF);
  CpuDid = PciRead16 (PCI_LIB_ADDRESS (SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, R_SA_MC_DEVICE_ID));

  DEBUG ((DEBUG_ERROR, "CpuFamilyModel 0x%X, CpuStepping 0x%X, CpuDid 0x%X\n", CpuFamilyModel, CpuStepping, CpuDid));
}


EFI_STATUS
EFIAPI
UpXtremeBoardDetect (
  VOID
  )
{
  UINT32              GpioData;
  GPIO_INIT_CONFIG    UpxBomGpioTemplate;
  UINT8               BomId;
  UINT8               Index;
  UINTN               NumberOfGpios;
  EFI_STATUS          Status;

  DEBUG ((DEBUG_INFO, "UpXtremeDetectionCallback\n"));

  PrintCpuInformation ();

  if (LibPcdGetSku () != 0) {
    return EFI_SUCCESS;
  }

  BomId               = 0;
  NumberOfGpios       = ARRAY_SIZE (mUpxGpioBomPad);

  LibPcdSetSku (BoardIdUpXtreme);

  DEBUG ((DEBUG_INFO, "SKU_ID: 0x%x\n", LibPcdGetSku ()));
  ASSERT (LibPcdGetSku () == BoardIdUpXtreme);

  CopyMem (&UpxBomGpioTemplate, &mUpxBomGpioTemplate, sizeof(UpxBomGpioTemplate));

  // Initialize all GPIO pins to input
  for (Index = 0; Index < NumberOfGpios; Index++) {
    UpxBomGpioTemplate.GpioPad = mUpxGpioBomPad[Index];
    GpioConfigurePads (1, &UpxBomGpioTemplate);
  }

  // Sample the GPIO pin level
  for (Index = 0; Index < NumberOfGpios; Index++) {
    Status = GpioGetInputValue (mUpxGpioBomPad[Index], &GpioData);
    if (EFI_ERROR(Status)) {
      break;
    }
    BomId = (BomId << 1) + (GpioData & 1);
  }

  if (Index == NumberOfGpios) {
    PcdSet8S(PcdBoardBomId, BomId);
  }

  DEBUG ((DEBUG_INFO, "BOM_ID 0x%x\n", PcdGet8(PcdBoardBomId)));
  return EFI_SUCCESS;
}

