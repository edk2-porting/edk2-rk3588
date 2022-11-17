/** @file
  Defines and prototypes for the System Agent PCIe library module
  This library is expected to share between DXE and SMM drivers.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SA_PCIE_LIB_H_
#define _SA_PCIE_LIB_H_

#include <Library/S3BootScriptLib.h>
#include <Protocol/SaPolicy.h>

#define MAX_SUPPORTED_ROOT_BRIDGE_NUMBER  3
#define MAX_SUPPORTED_DEVICE_NUMBER       192

/**
  Enumerate all end point devices connected to root bridge ports and record their MMIO base address

  @exception EFI_UNSUPPORTED      PCIe capability structure not found
  @retval    EFI_SUCCESS          All done successfully
**/
EFI_STATUS
EnumerateAllPcieDevices (
  VOID
  );

/**
  Sets Common Clock, TCx-VC0 mapping, and Max Payload for PCIe
**/
VOID
SaPcieConfigBeforeOpRom (
  VOID
  );

/**
  This function does all SA ASPM initialization
**/
VOID
SaAspm (
  VOID
  );

/**
  This function checks PEG end point device for extended tag capability and enables them if they are.
**/
VOID
EnableExtendedTag (
  VOID
  );

/**
  This function handles SA S3 resume
**/
VOID
SaS3Resume (
  VOID
  );

/**
  Wrapper function for all SA S3 resume tasks which can be a callback function.
**/
VOID
SaS3ResumeCallback (
  VOID
  );

#endif
