/** @file
  Pch information string library.

  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/MmPciLib.h>
#include <Library/PchInfoLib.h>
#include <PchAccess.h>

/**
  Structure for PCH stepping string mapping
**/
struct PCH_STEPPING_STRING {
  PCH_STEPPING  Stepping;
  CHAR8         *String;
};

GLOBAL_REMOVE_IF_UNREFERENCED
struct PCH_STEPPING_STRING mSteppingStrs[] = {
  {PchHB0,    "B0"},
  {PchHC0,    "C0"},
  {PchHD0,    "D0"},
  {PchHD1,    "D1"},
  {PchLpB0,   "B0"},
  {PchLpB1,   "B1"},
  {PchLpC0,   "C0"},
  {PchLpC1,   "C1"},
  {KblPchHA0, "A0"},
  {PchSteppingMax, NULL}
};

/**
  Structure for PCH series string mapping
**/
typedef struct {
  PCH_SERIES    Series;
  CHAR8         *String;
} PCH_SERIES_STRING ;

GLOBAL_REMOVE_IF_UNREFERENCED
PCH_SERIES_STRING mSklPchSeriesStrs[] = {
  {PchH,  "SKL PCH-H"},
  {PchLp, "SKL PCH-LP"},
  {PchUnknownSeries, NULL}
};

GLOBAL_REMOVE_IF_UNREFERENCED
PCH_SERIES_STRING mKblPchSeriesStrs[] = {
  {PchH,  "KBL PCH-H"},
  {PchUnknownSeries, NULL}
};

/**
  Structure for PCH sku string mapping
**/
struct PCH_SKU_STRING {
  UINT16        Id;
  CHAR8         *String;
};

GLOBAL_REMOVE_IF_UNREFERENCED
struct PCH_SKU_STRING mSkuStrs[] = {
  //
  // SKL PCH H Desktop LPC Device IDs
  //
  {V_SKL_PCH_H_LPC_DEVICE_ID_DT_SUPER_SKU, "Super SKU"},
  {V_KBL_PCH_H_LPC_DEVICE_ID_DT_ES_SUPER_SKU, "ES Super SKU"},
  {V_SKL_PCH_H_LPC_DEVICE_ID_DT_0, "Super SKU (locked)"},
  {V_SKL_PCH_H_LPC_DEVICE_ID_DT_1, "H110"},
  {V_SKL_PCH_H_LPC_DEVICE_ID_DT_2, "H170"},
  {V_SKL_PCH_H_LPC_DEVICE_ID_DT_3, "Z170"},
  {V_SKL_PCH_H_LPC_DEVICE_ID_DT_4, "Q170"},
  {V_SKL_PCH_H_LPC_DEVICE_ID_DT_5, "Q150"},
  {V_SKL_PCH_H_LPC_DEVICE_ID_DT_6, "B150"},
  //
  // SKL PCH H Server/WS LPC Device IDs
  //
  {V_SKL_PCH_H_LPC_DEVICE_ID_SVR_0, "C236"},
  {V_SKL_PCH_H_LPC_DEVICE_ID_SVR_1, "C232"},
  {V_SKL_PCH_H_LPC_DEVICE_ID_SVR_2, "CM236"},
  {V_SKL_PCH_H_LPC_DEVICE_ID_A14B, "Super SKU (Unlocked)"},
  {V_KBL_PCH_H_LPC_DEVICE_ID_SVR_ES_SUPER_SKU, "ES Super SKU (Unlocked)"},
  //
  // SKL PCH H Mobile LPC Device IDs
  //
  {V_SKL_PCH_H_LPC_DEVICE_ID_MB_0, "QM170"},
  {V_SKL_PCH_H_LPC_DEVICE_ID_MB_1, "HM170"},
  {V_SKL_PCH_H_LPC_DEVICE_ID_MB_3, "QMS180"},
  {V_SKL_PCH_H_LPC_DEVICE_ID_MB_4, "HM175"},
  {V_SKL_PCH_H_LPC_DEVICE_ID_MB_5, "QM175"},
  {V_SKL_PCH_H_LPC_DEVICE_ID_MB_6, "CM238"},
  {V_SKL_PCH_H_LPC_DEVICE_ID_MB_SUPER_SKU, "Super SKU"},
  {V_SKL_PCH_H_LPC_DEVICE_ID_MB_8, "QMS185"},
  //
  // SKL PCH LP Mobile LPC Device IDs
  //
  {V_PCH_LP_LPC_DEVICE_ID_MB_SUPER_SKU, "Super SKU"},
  {V_PCH_LP_LPC_DEVICE_ID_MB_0, "Super SKU (locked)"},
  {V_PCH_LP_LPC_DEVICE_ID_MB_1, "(U) Base SKU"},
  {V_PCH_LP_LPC_DEVICE_ID_MB_2, "(Y) Premium SKU"},
  {V_PCH_LP_LPC_DEVICE_ID_MB_3, "(U) Premium  SKU"},
  {V_PCH_LP_LPC_DEVICE_ID_MB_SUPER_SKU_1, "Super SKU"},
  {V_PCH_LP_LPC_DEVICE_ID_MB_4, "Super SKU (locked)"},
  {V_PCH_LP_LPC_DEVICE_ID_MB_5, "(U) Base SKU"},
  {V_PCH_LP_LPC_DEVICE_ID_MB_6, "(Y) Premium SKU"},
  {V_PCH_LP_LPC_DEVICE_ID_MB_7, "(U) Premium  SKU"},
  {V_PCH_LP_LPC_DEVICE_ID_MB_8, "(Y) iHDCP 2.2 Premium"},
  {V_PCH_LP_LPC_DEVICE_ID_MB_9, "(U) iHDCP 2.2 Premium"},
  {V_PCH_LP_LPC_DEVICE_ID_MB_10, "(U) iHDCP 2.2 Base"},
  //
  // KBL PCH H Desktop LPC Device IDs
  //
  {V_KBL_PCH_H_LPC_DEVICE_ID_DT_SUPER_SKU, "Super SKU"},
  {V_KBL_PCH_H_LPC_DEVICE_ID_DT_H270, "H270"},
  {V_KBL_PCH_H_LPC_DEVICE_ID_DT_Z270, "Z270"},
  {V_KBL_PCH_H_LPC_DEVICE_ID_DT_Q270, "Q270"},
  {V_KBL_PCH_H_LPC_DEVICE_ID_DT_Q250, "Q250"},
  {V_KBL_PCH_H_LPC_DEVICE_ID_DT_B250, "B250"},
  {V_KBL_PCH_H_LPC_DEVICE_ID_DT_H215, "H215"},
  {V_KBL_PCH_H_LPC_DEVICE_ID_DT_Z370, "Z370"},

  //
  // KBL PCH H Server/WS LPC Device IDs
  //
  {V_KBL_PCH_H_LPC_DEVICE_ID_SVR_SUPER_SKU, "Super SKU (Unlocked)"},
  {V_KBL_PCH_H_LPC_DEVICE_ID_SVR_0, "X290"},
  {V_KBL_PCH_H_LPC_DEVICE_ID_SVR_1, "C6xx"},
  {V_KBL_PCH_H_LPC_DEVICE_ID_SVR_2, "ES Super SKU Server (Interposer)"},
  {V_KBL_PCH_H_LPC_DEVICE_ID_SVR_3, "Super SKU Server"},
  {V_KBL_PCH_H_LPC_DEVICE_ID_SVR_4, "C422B"},
  {0xFFFF, NULL}
};

/**
  Get PCH stepping ASCII string
  The return string is zero terminated.

  @param [in]      PchStep              Pch stepping
  @param [out]     Buffer               Output buffer of string
  @param [in,out]  BufferSize           Size of input buffer,
                                        and return required string size when buffer is too small.

  @retval EFI_SUCCESS                   String copy successfully
  @retval EFI_INVALID_PARAMETER         The stepping is not supported, or parameters are NULL
  @retval EFI_BUFFER_TOO_SMALL          Input buffer size is too small
**/
EFI_STATUS
PchGetSteppingStr (
  IN     PCH_STEPPING                   PchStep,
  OUT    CHAR8                          *Buffer,
  IN OUT UINT32                         *BufferSize
  )
{
  UINTN  Index;
  UINT32 StrLength;
  CHAR8  *Str;

  if ((Buffer == NULL) || (BufferSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (*BufferSize > 0) {
    Buffer[0] = 0;
  }

  Str = NULL;
  StrLength = 0;
  for (Index = 0; mSteppingStrs[Index].Stepping != PchSteppingMax; Index++) {
    if (PchStep == mSteppingStrs[Index].Stepping) {
      StrLength = (UINT32) AsciiStrLen (mSteppingStrs[Index].String);
      Str       = mSteppingStrs[Index].String;
      break;
    }
  }
  if (StrLength == 0) {
    // Unsupported Stepping
    // ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  if (*BufferSize <= StrLength) {
    *BufferSize = StrLength + 1;
    return EFI_BUFFER_TOO_SMALL;
  }
  AsciiStrCpyS (Buffer, *BufferSize, Str);
  return EFI_SUCCESS;
}

/**
  Get PCH series ASCII string
  The return string is zero terminated.

  @param [in]      PchSeries            Pch series
  @param [out]     Buffer               Output buffer of string
  @param [in,out]  BufferSize           Size of input buffer,
                                        and return required string size when buffer is too small.

  @retval EFI_SUCCESS                   String copy successfully
  @retval EFI_INVALID_PARAMETER         The series is not supported, or parameters are NULL
  @retval EFI_BUFFER_TOO_SMALL          Input buffer size is too small
**/
EFI_STATUS
PchGetSeriesStr (
  IN     PCH_SERIES                     PchSeries,
  OUT    CHAR8                          *Buffer,
  IN OUT UINT32                         *BufferSize
  )
{
  UINTN              Index;
  UINT32             StrLength;
  CHAR8              *Str;
  PCH_GENERATION     PchGen;
  PCH_SERIES_STRING  *SeriesStrs;
  UINT16             LpcDeviceId;

  if ((Buffer == NULL) || (BufferSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  PchGen = GetPchGeneration ();

  if (PchGen == SklPch) {
    //
    // Check if Client/Server ES Super SKU
    //

    LpcDeviceId = GetLpcDid ();

    if ((LpcDeviceId == V_KBL_PCH_H_LPC_DEVICE_ID_DT_ES_SUPER_SKU) ||
        (LpcDeviceId == V_KBL_PCH_H_LPC_DEVICE_ID_SVR_ES_SUPER_SKU)) {
      SeriesStrs = mKblPchSeriesStrs;
    } else {
      SeriesStrs = mSklPchSeriesStrs;
    }
  } else if (PchGen == KblPch){
    SeriesStrs = mKblPchSeriesStrs;
  } else {
    ASSERT(FALSE);
    return EFI_UNSUPPORTED;
  }

  if (*BufferSize > 0) {
    Buffer[0] = 0;
  }

  Str = NULL;
  StrLength = 0;
  for (Index = 0; SeriesStrs[Index].Series != PchUnknownSeries; Index++) {
    if (PchSeries == SeriesStrs[Index].Series) {
      StrLength = (UINT32) AsciiStrLen (SeriesStrs[Index].String);
      Str       = SeriesStrs[Index].String;
      break;
    }
  }
  if (StrLength == 0) {
    // Unsupported Series
    // ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  if (*BufferSize <= StrLength) {
    *BufferSize = StrLength + 1;
    return EFI_BUFFER_TOO_SMALL;
  }
  AsciiStrCpyS (Buffer, *BufferSize, Str);
  return EFI_SUCCESS;
}

/**
  Get PCH Sku ASCII string
  The return string is zero terminated.

  @param [in]      LpcDid               LPC device id
  @param [out]     Buffer               Output buffer of string
  @param [in,out]  BufferSize           Size of input buffer,
                                        and return required string size when buffer is too small.

  @retval EFI_SUCCESS                   String copy successfully
  @retval EFI_INVALID_PARAMETER         The series is not supported, or parameters are NULL
  @retval EFI_BUFFER_TOO_SMALL          Input buffer size is too small
**/
EFI_STATUS
PchGetSkuStr (
  IN     UINT16                         LpcDid,
  OUT    CHAR8                          *Buffer,
  IN OUT UINT32                         *BufferSize
  )
{
  UINTN  Index;
  UINT32 StrLength;
  CHAR8  *Str;

  if ((Buffer == NULL) || (BufferSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (*BufferSize > 0) {
    Buffer[0] = 0;
  }

  Str = NULL;
  StrLength = 0;
  for (Index = 0; mSkuStrs[Index].Id != 0xFFFF; Index++) {
    if (LpcDid == mSkuStrs[Index].Id) {
      StrLength = (UINT32) AsciiStrLen (mSkuStrs[Index].String);
      Str       = mSkuStrs[Index].String;
    }
  }
  if (StrLength == 0) {
    // Unsupported Sku
    // ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  if (*BufferSize <= StrLength) {
    *BufferSize = StrLength + 1;
    return EFI_BUFFER_TOO_SMALL;
  }
  AsciiStrCpyS (Buffer, *BufferSize, Str);
  return EFI_SUCCESS;
}
