/** @file
  Provides data structure information used by SiliconIp SGX-TEM

  @copyright
  Copyright 2020 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

// NAK - Not a knob, used just for indication
UINT8  IsSgxCapable;
UINT8  IsHwCongifSupportedBySgx;                // ## PRODUCED by SgxPreMemInit
UINT8  CrDimmsPresent;
UINT64 ValidPrmrrBitMap;
UINT64 SprspOrLaterPrmSizeBitmap;               // ## PRODUCED by SgxPreMemInit
UINT8  ShowEpoch;
UINT8  SkipSignalPpmDone;                       // ## PRODUCED by SgxEarlyInit

UINT8  SprspOrLaterIsPrmSizeInvalidated;        // ## PRODUCED by SgxPreMemInit
UINT8  SprspOrLaterAreHardwarePreconditionsMet; // ## PRODUCED by SgxPreMemInit
UINT8  SprspOrLaterAreMemoryPreconditionsMet;   // ## PRODUCED by SgxPreMeminit
UINT8  SprspOrLaterAreSetupPreconditionsMet;    // ## PRODUCED by SgxPreMemInit
