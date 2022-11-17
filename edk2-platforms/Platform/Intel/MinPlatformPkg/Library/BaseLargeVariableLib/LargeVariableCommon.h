/** @file
  Large Variable Lib Common Header

  These libraries is used to store large data sets using the UEFI Variable
  Services. At time of writing, most UEFI Variable Services implementations do
  not allow more than 64KB of data to be stored in a single UEFI variable. This
  library will split data sets across multiple variables as needed.

  In the case where more than one variable is needed to store the data, an
  integer number will be added to the end of the variable name. This number
  will be incremented for each variable as needed to retrieve the entire data
  set.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _LARGE_VARIABLE_COMMON_H_
#define _LARGE_VARIABLE_COMMON_H_

//
// 1024 was choosen because this is the size of the SMM communication buffer
// used by VariableDxeSmm to transfer the VariableName from DXE to SMM. Choosing
// the same size will prevent this library from limiting variable names any
// more than the MdeModulePkg implementation of UEFI Variable Services does.
//
#define MAX_VARIABLE_NAME_SIZE      1024

//
// The 2012 Windows Hardware Requirements specified a minimum variable size of
// 32KB. By setting the maximum allowed number of variables to 0x20000, this
// allows up to 4GB of data to be stored on most UEFI implementations in
// existence. Older UEFI implementations were known to only provide 8KB per
// variable. In this case, up to 1GB can be stored. Since 1GB vastly exceeds the
// size of any known NvStorage FV, choosing this number should effectively
// enable all available NvStorage space to be used to store the given data.
//
#define MAX_VARIABLE_SPLIT          131072  // 0x20000

//
// There are 6 digits in the number 131072, which means the length of the string
// representation of this number will be at most 6 characters long.
//
#define MAX_VARIABLE_SPLIT_DIGITS   6

//
// On some architectures (Ex. Itanium) there a requirement to
// maintain DWORD alignment of the variable data. Hence the
// maximum possible padding size is 3.
//
#define MAX_VARIABLE_NAME_PAD_SIZE  3

#endif  // _LARGE_VARIABLE_COMMON_H_
