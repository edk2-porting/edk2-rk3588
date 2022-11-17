/** @file
  HSIO policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _HSIO_CONFIG_H_
#define _HSIO_CONFIG_H_
#define HSIO_PREMEM_CONFIG_REVISION 1 //@deprecated
extern EFI_GUID gHsioPreMemConfigGuid; //@deprecated

#define HSIO_CONFIG_REVISION 1
extern EFI_GUID gHsioConfigGuid;

#pragma pack (push,1)

/**
  The PCH_HSIO_PREMEM_CONFIG block provides HSIO message related settings.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header

  /**
  <b>(Test)</b>
  0- Disable, disable will prevent the HSIO version check and ChipsetInit HECI message from being sent
  <b>1- Enable</b> ChipsetInit HECI message
  **/
  UINT8     ChipsetInitMessage;
  /**
  <b>(Test)</b>
  <b>0- Disable</b>
  1- Enable When enabled, this is used to bypass the reset after ChipsetInit HECI message.
  **/
  UINT8     BypassPhySyncReset;
  UINT8     RsvdBytes[2];

} PCH_HSIO_PREMEM_CONFIG;


/**
  The PCH_HSIO_CONFIG block provides HSIO message related settings.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  /**
    Policy used to point to the Base (+ OEM) ChipsetInit binary used to sync between BIOS and CSME
  **/
  UINT32    ChipsetInitBinPtr;
  /**
    Policy used to indicate the size of the Base (+ OEM) ChipsetInit binary used to sync between BIOS and CSME
  **/
  UINT32    ChipsetInitBinLen;
} PCH_HSIO_CONFIG;

#pragma pack (pop)

#endif // _HSIO_CONFIG_H_
