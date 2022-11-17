/** @file
  Definitions for DisableBmeProtocol


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DISABLE_TBT_BME_PROTOCOL_H_
#define _DISABLE_TBT_BME_PROTOCOL_H_

typedef struct EFI_DISABLE_BME_PROTOCOL EFI_DISABLE_TBT_BME_PROTOCOL;

/**
  This is for disable TBT BME bit under shell environment

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
typedef
VOID
(EFIAPI *DISABLE_BUS_MASTER_ENABLE) (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );


struct EFI_DISABLE_BME_PROTOCOL {
  DISABLE_BUS_MASTER_ENABLE DisableBme;
};

extern EFI_GUID gDxeDisableTbtBmeProtocolGuid;


#endif

