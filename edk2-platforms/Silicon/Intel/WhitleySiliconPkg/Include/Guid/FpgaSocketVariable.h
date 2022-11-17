/** @file
  Data format for Universal Data Structure

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef   __FPGA_SOCKET_CONFIGURATION_DATA_H__
#define   __FPGA_SOCKET_CONFIGURATION_DATA_H__

#include <Fpga.h>

extern EFI_GUID gFpgaSocketVariableGuid;
#define FPGA_SOCKET_CONFIGURATION_NAME L"FpgaSocketConfig"

#pragma pack(1)

typedef struct {

  // User Bitmap to enable the FPGA socket.
  UINT8    FpgaSetupEnabled;

  // for each socket enabled, use this Bit stream GUID Index
  // Note: variable is Index+ 1 for unused default to be 0
  UINT8    FpgaSocketGuid[FPGA_MAX_SOCKET];
  // FPGA Temperature Threshold 1/2: Max value clamped at 100 C;
  // i.e. if the SW tries to write value greater than 100 C, HW will automatically default to 100 C.
  UINT8    FpgaThermalTH1[FPGA_MAX_SOCKET];
  UINT8    FpgaThermalTH2[FPGA_MAX_SOCKET];

  // FPGA reserved data
  UINT8    FpgaReserved[14];
} FPGA_SOCKET_CONFIGURATION;
#pragma pack()

#endif

