/** @file
  Prototype of the MultiPch library - Base definitions.

  @copyright
  Copyright 2020 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_MULTI_PCH_BASE_H_
#define _PCH_MULTI_PCH_BASE_H_


#define PCH_LEGACY_ID                     0
#define PCH_MAX                           4

/**
  Function returns PchId for devices of passed Segment and Bus.
  PchId is an index in PCH_IP_INFO table

  @param [in]      Segment  Segment to look for
  @param [in]      Bus      Bas to look for
  @param [in, out] PchId    pointer to variable to return PchId

  @retval EFI_SUCCESS           Function returned valid PchId
  @retval EFI_INVALID_PARAMETER Passed pointer is invalid
  @retval EFI_DEVICE_ERROR      Not found valid PchInfo
  @retval EFI_NOT_FOUND         For given input parameters not found valid PchId
**/
EFI_STATUS
FindMultiPchInfo (
  IN     UINTN       HeciSegment,
  IN     UINTN       HeciBus,
  IN OUT UINTN       *PchId
  );

#endif // _PCH_MULTI_PCH_BASE_H_
