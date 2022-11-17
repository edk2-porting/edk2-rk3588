/** @file
  Header file for SaPlatformLib.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SA_PLATFORM_LIB_H_
#define _SA_PLATFORM_LIB_H_


/**
  Checks if SKU is Mobile

  @retval FALSE  SKU is not Mobile
  @retval TRUE   SKU is Mobile
**/
BOOLEAN
EFIAPI
IsMobileSku (
  VOID
  );

/**
  Checks if SKU is Desktop

  @retval FALSE  SKU is not Desktop
  @retval TRUE   SKU is Desktop
**/
BOOLEAN
EFIAPI
IsDesktopSku (
  VOID
  );

/**
  Checks if SKU is Server

  @retval FALSE  SKU is not Server
  @retval TRUE   SKU is Server
**/
BOOLEAN
EFIAPI
IsServerSku (
  VOID
  );

#endif
