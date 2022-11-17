/** @file
  Platform Type Driver for Harwich.

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PLATFORM_TYPES_H_
#define _PLATFORM_TYPES_H_

#include <PiDxe.h>
#include <Protocol/PlatformType.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/VariableWrite.h>
#include <Protocol/Spi.h>
#include <Protocol/IioUds.h>
#include <SystemBoard.h>
#include <Library/MemoryAllocationLib.h>

#include <Guid/HobList.h>
#include <Register/PchRegsSpi.h>
#include <Register/PchRegsLpc.h>
#include <PchAccess.h>
#include <Platform.h>
#include <Register/Cpuid.h>




#define EFI_PLATFORM_TYPE_DRIVER_PRIVATE_SIGNATURE  SIGNATURE_32 ('T', 'Y', 'P', 'P')
#define EFI_IIO_UDS_DRIVER_PRIVATE_SIGNATURE  SIGNATURE_32 ('S', 'D', 'U', 'I')


typedef unsigned char BYTE;     //!<  8-bit quantities
typedef unsigned short WORD;    //!< 16-bit quantities
typedef unsigned long DWORD;    //!< 32-bit quantities

typedef struct {
  UINTN                               Signature;
  EFI_HANDLE                          Handle;               // Handle for protocol this driver installs on
  EFI_PLATFORM_TYPE_PROTOCOL          PlatformType;         // Policy protocol this driver installs
} EFI_PLATFORM_DATA_DRIVER_PRIVATE;

typedef struct {
  UINTN                               Signature;
  EFI_HANDLE                          Handle;         // Handle for protocol this driver installs on
  EFI_IIO_UDS_PROTOCOL                IioUds;         // Policy protocol this driver installs
} EFI_IIO_UDS_DRIVER_PRIVATE;

#endif
