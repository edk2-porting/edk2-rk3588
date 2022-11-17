/** @file
  Interface definition for GopPolicy Protocol.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _GOP_POLICY_PROTOCOL_H_
#define _GOP_POLICY_PROTOCOL_H_


#define GOP_POLICY_PROTOCOL_REVISION_01  0x01
#define GOP_POLICY_PROTOCOL_REVISION_03  0x03

typedef enum {
  LidClosed,
  LidOpen,
  LidStatusMax
} LID_STATUS;

typedef enum {
  Docked,
  UnDocked,
  DockStatusMax
} DOCK_STATUS;

///
/// Function to retrieve LID status
///
typedef
EFI_STATUS
(EFIAPI *GET_PLATFORM_LID_STATUS) (
  OUT LID_STATUS * CurrentLidStatus
  );

///
/// Function to retrieve Dock status
///
typedef
EFI_STATUS
(EFIAPI *GET_PLATFORM_DOCK_STATUS) (
 OUT DOCK_STATUS  CurrentDockStatus
);

///
/// Function to retrieve VBT table address and size
///
typedef
EFI_STATUS
(EFIAPI *GET_VBT_DATA) (
  OUT EFI_PHYSICAL_ADDRESS * VbtAddress,
  OUT UINT32               *VbtSize
  );

/**
  System Agent Graphics Output Protocol (GOP) - Policy Protocol\n
  Graphics Output Protocol (GOP) is a UEFI API replacing legacy Video ROMs for EFI boot\n
  When GOP Driver is used this protocol can be consumed by GOP driver or platform code for GOP relevant initialization\n
  All functions in this protocol should be initialized by platform code basing on platform implementation\n
**/
typedef struct {
  UINT32                    Revision;              ///< Protocol revision
  GET_PLATFORM_LID_STATUS   GetPlatformLidStatus;  ///< Protocol function to get Lid Status. Platform code should provide this function basing on design.
  GET_VBT_DATA              GetVbtData;            ///< Protocol function to get Vbt Data address and size. Platform code should provide this function basing on design.
  GET_PLATFORM_DOCK_STATUS  GetPlatformDockStatus;  ///< Function pointer for get platform dock status.
  EFI_GUID                  GopOverrideGuid;        ///< A GUID provided by BIOS in case GOP is to be overridden.
} GOP_POLICY_PROTOCOL;

extern EFI_GUID gGopPolicyProtocolGuid;
extern EFI_GUID gGen12PolicyProtocolGuid;
extern EFI_GUID gGen9PolicyProtocolGuid;
extern EFI_GUID gIntelGraphicsVbtGuid;

#endif
