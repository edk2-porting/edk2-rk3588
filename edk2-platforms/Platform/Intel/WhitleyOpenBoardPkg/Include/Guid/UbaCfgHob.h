/** @file
  uba config database head file

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _UBA_CONFIG_DATABASE_HOB_H_
#define _UBA_CONFIG_DATABASE_HOB_H_


#define UBA_CONFIG_HOB_SIGNATURE    SIGNATURE_32('U', 'B', 'A', 'H')
#define UBA_CONFIG_HOB_VERSION      0x01

#define UBA_BOARD_SIGNATURE         SIGNATURE_32('S', 'K', 'U', 'D')
#define UBA_BOARD_VERSION           0x01

//
// Interface data between PEI & DXE
// Should keep same align
//
#pragma pack (1)

typedef struct _UBA_CONFIG_HOB_FIELD {
  UINT32                  Signature;
  UINT32                  Version;
  EFI_GUID                ResId;
  UINT64                  DataOffset;
  UINT32                  Size;
} UBA_CONFIG_HOB_FIELD;

typedef struct _UBA_CONFIG_HOB_HEADER {
  UINT32                  Signature;
  UINT32                  Version;
  EFI_GUID                DataGuid;
  UINT32                  HobLength;
  UINT32                  BoardId;
  EFI_GUID                BoardGuid;
  CHAR8                   BoardName[16];
  UINT32                  DataCount;
  UBA_CONFIG_HOB_FIELD    HobField[1];
} UBA_CONFIG_HOB_HEADER;


typedef struct _UBA_BOARD_NODE {
  UINT32                  Signature;
  UINT32                  Version;
  LIST_ENTRY              DataLinkHead;

  UINT32                  BoardId;
  EFI_GUID                BoardGuid;
  CHAR8                   BoardName[16];
  UINT32                  DataCount;
} UBA_BOARD_NODE;

typedef struct _UBA_CONFIG_NODE {
  UINT32                  Signature;
  UINT32                  Version;
  LIST_ENTRY              DataLink;

  EFI_HANDLE              Handle;
  EFI_GUID                ResId;
  UINT32                  Size;
  VOID                    *Data;
} UBA_CONFIG_NODE;

#pragma pack ()

#define BOARD_NODE_INSTANCE_FROM_THIS(p)       CR(p, UBA_BOARD_NODE, DataLinkHead, UBA_BOARD_SIGNATURE)
#define CONFIG_NODE_INSTANCE_FROM_THIS(p)      CR(p, UBA_CONFIG_NODE, DataLink, UBA_BOARD_SIGNATURE)

#endif // _UBA_CONFIG_DATABASE_HOB_H_
