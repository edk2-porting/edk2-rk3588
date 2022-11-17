/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _HISI_BOARD_NIC_PROTOCOL_H_
#define _HISI_BOARD_NIC_PROTOCOL_H_

#define HISI_BOARD_NIC_PROTOCOL_GUID   \
    { 0xb5903955, 0x31e9, 0x4aaf, { 0xb2, 0x83, 0x7, 0x9f, 0x3c, 0xc4, 0x71, 0x66 } }

#define HISI_BOARD_XGE_STATUS_PROTOCOL_GUID   \
        { 0xa6b8ed0e, 0xd8cc, 0x4853, { 0xaa, 0x39, 0x2c, 0x3e, 0xcd, 0x7c, 0xa5, 0x97 } }

typedef
EFI_STATUS
(EFIAPI *HISI_BOARD_NIC_GET_MAC_ADDRESS) (
  IN OUT EFI_MAC_ADDRESS *Mac,
  IN UINTN Port
  );

typedef
EFI_STATUS
(EFIAPI *HISI_BOARD_NIC_SET_MAC_ADDRESS) (
  IN EFI_MAC_ADDRESS *Mac,
  IN UINTN Port
  );

typedef struct {
  HISI_BOARD_NIC_GET_MAC_ADDRESS GetMac;
  HISI_BOARD_NIC_SET_MAC_ADDRESS SetMac;
} HISI_BOARD_NIC_PROTOCOL;

typedef
VOID
(*HISI_BOARD_FEEDBACK_XGE_STATUS) (
  BOOLEAN IsLinkup,
  BOOLEAN IsActOK,
  UINT32 port
  );

typedef struct {
  HISI_BOARD_FEEDBACK_XGE_STATUS FeedbackXgeStatus;
} HISI_BOARD_XGE_STATUS_PROTOCOL;


extern EFI_GUID gHisiBoardNicProtocolGuid;
extern EFI_GUID gHisiBoardXgeStatusProtocolGuid;


#endif
