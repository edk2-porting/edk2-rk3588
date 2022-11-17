/** @file
  Header file for the ChaosKey hardware random number generator.

  Copyright (c) 2016 - 2017, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _CHAOSKEY_USB_HWRNG_DRIVER_H_
#define _CHAOSKEY_USB_HWRNG_DRIVER_H_

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <Protocol/Rng.h>
#include <Protocol/UsbIo.h>

#define CHAOSKEY_VENDOR_ID      0x1d50  /* OpenMoko */
#define CHAOSKEY_PRODUCT_ID     0x60c6  /* ChaosKey */

#define CHAOSKEY_TIMEOUT        10 // ms
#define CHAOSKEY_MAX_EP_SIZE    64 // max EP size for full-speed devices

#define CHAOSKEY_DEV_SIGNATURE  SIGNATURE_32('c','h','k','e')

typedef struct {
  UINT32                        Signature;
  UINT16                        EndpointAddress;
  UINT16                        EndpointSize;
  EFI_USB_IO_PROTOCOL           *UsbIo;
  EFI_RNG_PROTOCOL              Rng;
} CHAOSKEY_DEV;

#define CHAOSKEY_DEV_FROM_THIS(a) \
  CR(a, CHAOSKEY_DEV, Rng, CHAOSKEY_DEV_SIGNATURE)

extern EFI_COMPONENT_NAME2_PROTOCOL gChaosKeyDriverComponentName2;

EFI_STATUS
ChaosKeyInit (
  IN  EFI_HANDLE        DriverBindingHandle,
  IN  EFI_HANDLE        ControllerHandle
  );

EFI_STATUS
ChaosKeyRelease (
  IN  EFI_HANDLE        DriverBindingHandle,
  IN  EFI_HANDLE        ControllerHandle
  );

#endif // _CHAOSKEY_USB_HWRNG_DRIVER_H_
