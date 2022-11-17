/** @file
  Dci policy

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _DCI_CONFIG_H_
#define _DCI_CONFIG_H_

#define DCI_PREMEM_CONFIG_REVISION 2
extern EFI_GUID gDciPreMemConfigGuid;

#pragma pack (push,1)

/**
  This structure contains the policies which are related to Direct Connection Interface (DCI).

  <b>Revision 1:</b>
  - Inital version.
  <b>Revision 2:</b>
  - Deprecate DciAutoDetect
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  /**
    DCI enable (HDCIEN bit)
    when Enabled, allow DCI to be enabled. When Disabled, the host control is not enabling DCI feature.
    BIOS provides policy to enable or disable DCI, and user would be able to use BIOS option to change this policy.
    The user changing the setting from disable to enable, is taken as a consent from the user to enable this DCI feature.
    <b>0:Disabled</b>; 1:Enabled
  **/
  UINT32    DciEn           :  1;
  /**
    @deprecated from revision 2.
    <b>(Test)</b> When set to Auto detect mode, it detects CCA being connected during BIOS post time.
    This policy only applies when DciEn is disabled.
    NOTE: this policy should not be visible to end customer.
    0: Disable AUTO mode, <b>1: Enable AUTO mode</b>
  **/
  UINT32    DciAutoDetect   :  1;
  UINT32    RsvdBits        : 30;       ///< Reserved bits
} PCH_DCI_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _DCI_CONFIG_H_
