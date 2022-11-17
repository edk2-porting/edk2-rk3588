/** @file

 Copyright 2020 NXP

 SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef GPIO_H__
#define GPIO_H__

#include <Uefi.h>

/* enum for GPIO number */
typedef enum _GPIO_BLOCK {
  GPIO1,
  GPIO2,
  GPIO3,
  GPIO4,
  GPIO_MAX
} GPIO_BLOCK;

/* enum for GPIO direction */
typedef enum _GPIO_DIRECTION {
  INPUT,
  OUTPUT
} GPIO_DIRECTION;

/* enum for GPIO state */
typedef enum _GPIO_STATE {
  LOW,
  HIGH
} GPIO_VAL;

/**
   GpioSetDiriection: Set GPIO direction as INPUT or OUTPUT

   @param[in] Id  GPIO controller number
   @param[in] Bit GPIO number
   @param[in] Dir GPIO Direction as INPUT or OUTPUT

   @retval EFI_SUCCESS
 **/
EFI_STATUS
GpioSetDirection (
  IN  UINT8    Id,
  IN  UINT32   Bit,
  IN  BOOLEAN  Dir
  );

/**
   GpioGetDirection:  Retrieve GPIO direction

   @param[in] Id  GPIO controller number
   @param[in] Bit GPIO number

   @retval GPIO Direction as INPUT or OUTPUT
 **/
UINT32
GpioGetDirection (
  IN  UINT8    Id,
  IN  UINT32   Bit
  );

 /**
   GpioGetData:  Retrieve GPIO Value

   @param[in] Id  GPIO controller number
   @param[in] Bit GPIO number

   @retval GPIO value  as HIGH or LOW
 **/
UINT32
GpioGetData (
  IN  UINT8    Id,
  IN  UINT32   Bit
  );

/**
   GpioSetData:  Set GPIO data Value

   @param[in] Id  GPIO controller number
   @param[in] Bit GPIO number
   @param[in] Data GPIO data value to set

   @retval GPIO value  as HIGH or LOW
 **/
EFI_STATUS
GpioSetData (
  IN  UINT8    Id,
  IN  UINT32   Bit,
  IN  BOOLEAN  Data
  );

/**
   GpioSetOpenDrain:  Set GPIO as Open drain

   @param[in] Id  GPIO controller number
   @param[in] Bit GPIO number
   @param[in] OpenDrain Set as open drain

   @retval EFI_SUCCESS
 **/
EFI_STATUS
GpioSetOpenDrain (
  IN  UINT8    Id,
  IN  UINT32   Bit,
  IN  BOOLEAN  OpenDrain
  );

#endif
