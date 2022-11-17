/** @file
  Header file for GpioMemLib. This library provides GpioLib with static memory to hold GpioName.
  Static memory is handled differently in PEI and DXE phase. For PEI pre mem we use private HOB to store
  gpio name since .data section is read only. For PEI post mem and DXE simple static buffer is used.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _GPIO_NAME_BUFFER_LIB_H_
#define _GPIO_NAME_BUFFER_LIB_H_

#define GPIO_NAME_LENGTH_MAX  32

/**
  Returns pointer to the global buffer to be used by GpioNamesLib

  @retval CHAR8*  Pointer to the buffer
**/
CHAR8*
GpioGetStaticNameBuffer (
  VOID
  );
#endif
