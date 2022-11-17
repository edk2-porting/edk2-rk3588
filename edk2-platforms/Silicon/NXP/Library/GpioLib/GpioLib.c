/** @file

 GPIO controller Library inmplements the functions
 which will be used by other library/drivers to
 get/set GPIO pin direction and get/set data on
 GPIO pins.

 Copyright 2020 NXP

 SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/GpioLib.h>
#include <Library/IoAccessLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>

STATIC MMIO_OPERATIONS *mGpioOps;

/**
   Structure for GPIO Regsters

   GpDir   GPIO direction register
   GpOdr   GPIO open drain register
   GpData  GPIO data register
   GpIer   GPIO interrupt event register
   GpImr   GPIO interrupt mask register
   GpIcr   GPIO interrupt control register

 **/
typedef struct GpioRegs {
  UINT32 GpDir;
  UINT32 GpOdr;
  UINT32 GpData;
  UINT32 GpIer;
  UINT32 GpImr;
  UINT32 GpIcr;
} GPIO_REGS;

/**
   GetBaseAddr GPIO controller Base Address

   @param[in] Id  GPIO controller number

   @retval GPIO controller Base Address, if found
   @retval NULL, if not a valid controller number

 **/
STATIC
VOID *
GetBaseAddr (
  IN  UINT8  Id
  )
{

  UINTN      GpioBaseAddr;
  UINTN      MaxGpioController;

  mGpioOps = GetMmioOperations (FeaturePcdGet (PcdGpioControllerBigEndian));

  MaxGpioController = PcdGet32 (PcdNumGpioController);

  if (Id < MaxGpioController) {
    GpioBaseAddr = PcdGet64 (PcdGpioModuleBaseAddress) +
                     (Id * PcdGet64 (PcdGpioControllerOffset));
    return (VOID *)GpioBaseAddr;
  } else {
    DEBUG((DEBUG_ERROR, "Invalid Gpio Controller Id %d, Allowed Ids are %d-%d",
      Id, GPIO1, MaxGpioController));
      return NULL;
  }
}

/**
   GpioSetDirection: Set GPIO direction as INPUT or OUTPUT

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
  )
{
  GPIO_REGS    *Regs;
  UINT32       DirectionBitMask;

  Regs = GetBaseAddr (Id);
  DirectionBitMask = 1 << Bit;

  if (Dir) {
    mGpioOps->Or32 ((UINTN)&Regs->GpDir, DirectionBitMask);
  }
  else {
    mGpioOps->And32 ((UINTN)&Regs->GpDir, ~DirectionBitMask);
  }

  return EFI_SUCCESS;
}

/**
   GpioGetDiriection:  Retrieve GPIO direction

   @param[in] Id  GPIO controller number
   @param[in] Bit GPIO number

   @retval GPIO Direction as INPUT or OUTPUT
 **/
UINT32
GpioGetDirection (
  IN  UINT8    Id,
  IN  UINT32   Bit
  )
{
  GPIO_REGS    *Regs;
  UINT32       Value;
  UINT32       DirectionBitMask;

  Regs = GetBaseAddr (Id);
  DirectionBitMask = 1 << Bit;

  Value = mGpioOps->Read32 ((UINTN)&Regs->GpDir);

  return (Value & DirectionBitMask);
}

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
  )
{
  GPIO_REGS    *Regs;
  UINT32       Value;
  UINT32       DataBitMask;

  Regs = (VOID *)GetBaseAddr (Id);
  DataBitMask = 1 << Bit;

  Value = mGpioOps->Read32 ((UINTN)&Regs->GpData);

  return (Value & DataBitMask);
}

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
  )
{
  GPIO_REGS    *Regs;
  UINT32       DataBitMask;

  Regs = GetBaseAddr (Id);
  DataBitMask = 1 << Bit;

  if (Data) {
    mGpioOps->Or32 ((UINTN)&Regs->GpData, DataBitMask);
  } else {
    mGpioOps->And32 ((UINTN)&Regs->GpData, ~DataBitMask);
  }

  return EFI_SUCCESS;
}

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
  )
{
  GPIO_REGS    *Regs;
  UINT32       OpenDrainBitMask;

  Regs = GetBaseAddr (Id);
  OpenDrainBitMask = 1 << Bit;

  if (OpenDrain) {
    mGpioOps->Or32 ((UINTN)&Regs->GpOdr, OpenDrainBitMask);
  } else {
    mGpioOps->And32 ((UINTN)&Regs->GpOdr, ~OpenDrainBitMask);
  }

  return EFI_SUCCESS;
}
