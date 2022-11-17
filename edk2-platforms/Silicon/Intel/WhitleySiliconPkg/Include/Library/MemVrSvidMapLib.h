/** @file

  MemVrSvidMapLib.h

  API Header for VrSvid Mapping

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MEM_VR_SVID_MAP_LIB_H_
#define _MEM_VR_SVID_MAP_LIB_H_

#define MAX_SVID_MC     16
#define MAX_SVID_SOCKET 16

typedef struct {
    UINT8 Mc[MAX_SVID_MC];  // Store SVID for Socket\MC Pair
} SVID_SOCKET;

typedef struct {
  SVID_SOCKET Socket[MAX_SVID_SOCKET];
} MEM_SVID_MAP;


/**
  Get SVID Mapping from Socket and MCID

  @param[in] Socket    - Socket Id - 0 based
  @param[in] McId      - Memory controller 0 based
  @param[in] SvidValue - SVID Value

  @retval EFI_SUCCESS   - Value found
  @retval EFI_NOT_FOUND - Value not found

**/
EFI_STATUS
EFIAPI
GetSvidMap (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT8 *SvidValue
  );

/**
  Set SVID Mapping for given Socket and MCID

  @param[in] Socket    - Socket Id - 0 based
  @param[in] McId      - Memory controller 0 based
  @param[in] SvidValue - SVID Value

  @retval EFI_SUCCESS   - Value set successfully
  @retval EFI_NOT_FOUND - Value not set

**/
EFI_STATUS
EFIAPI
SetSvidMap (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT8 SvidValue
  );

#endif //_MEM_VR_SVID_MAP_LIB_H_
