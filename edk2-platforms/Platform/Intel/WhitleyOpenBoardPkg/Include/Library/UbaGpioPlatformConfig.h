/** @file
  UBA GPIO Platform Specific functions Library Header File.

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _UBA_GPIO_PLATFORM_CONFIG_LIB_H
#define _UBA_GPIO_PLATFORM_CONFIG_LIB_H

#include <Base.h>
#include <Uefi.h>
#include <Library/GpioLib.h>

#define PLATFORM_GPIO_CONFIG_SIGNATURE  SIGNATURE_32 ('P', 'G', 'P', 'O')
#define PLATFORM_GPIO_CONFIG_VERSION    01


// {d2c2adab-80c0-4a13-a0f8-adede1a51740}
#define   PLATFORM_GPIO_PLATFORM_CONFIG_DATA_GUID \
{ 0xd2c2adab, 0x80c0, 0x4a13, { 0xa0, 0xf8, 0xad, 0xed, 0xe1, 0xa5, 0x17, 0x40 } }

//Use this define to skip the usage of a gpio in PLATFORM_GPIO_CONFIG_TABLE
#define UNUSED_GPIO             0x0

typedef struct _PLATFORM_GPIO_CONFIG_TABLE{
  //
  // Header information
  //
  INT32                    Signature;
  INT32                    Version;

  GPIO_INIT_CONFIG         GpioMfgPad;
  GPIO_PAD                 ReservedM;
  GPIO_PAD                 RcvJumper;

  //
  // ADR pads
  //
  GPIO_PAD                 FmAdrTrigger;
  GPIO_PAD                 AdrEnable;

  //
  // OemProcMemInit pad
  //
  GPIO_PAD                 ForceTo1SConfigModePad;

  //
  // Used by PC Platforms
  //
  GPIO_PAD                 QATGpio;

  //
  // Used by PC platforms. This is the first GPIO pad of the pad series to indicate Board ID
  //
  GPIO_PAD                 BoardID0Gpio;

  //
  // Used to indicate proper pin to for WHEA SCI detection
  //
  GPIO_PAD                 WheaSciPad;

  //
  // Used to generate CPU HP SMI
  //
  GPIO_PAD                 CpuHpSmiPad;

  //
  // Used to signal FPGA error
  //
  GPIO_PAD                 FpgaErrorSingnalPad1;

  //
  // Used to signal FPGA error
  //
  GPIO_PAD                 FpgaErrorSingnalPad2;

  // Flash Security override
  GPIO_PAD                 FlashSecOverride;
} PLATFORM_GPIO_CONFIG_TABLE;


/**

    Reads GPIO pin to get Flash Security Override jumper status

    @param[out] Jumper - The pointer to the jumper output

    @retval Status - Success if GPIO's are read properly

**/
EFI_STATUS
GpioGetFlashSecOvrdVal (
  OUT UINT32 *Jumper
);
/**

    Reads GPIO pin to get recovery jumper status

    @param[out] RcvJumper - The pointer to the Recovery jumper input

    @retval Status - Success if GPIO's are read properly

**/
EFI_STATUS
GpioGetRcvPadVal (
  OUT UINT32 *RcvJumper
);


/**

    Reads GPIO pin to get FM ADR trigger pin

    @param[out] FmAdrTrigger - The pointer to the ADR trigger input

    @retval Status - Success if GPIO's are read properly

**/
EFI_STATUS
GpioGetFmAdrTriggerPadVal (
  OUT UINT32 *FmAdrTrigger
);

/**

    Sets GPIO pin to enable ADR on the board

    @param Set[in] - If TRUE means the pas should go 'high', otherwise 'low'

    @retval Status - Success if GPIO set properly

**/
EFI_STATUS
GpioSetAdrEnablePadOutVal (
  IN BOOLEAN Set
);

/**

    Reads GPIO pin to Force to S1 config mode pad

    @param[out] ForceS1ConfigPad - Input value of the Froce S1 Config pad

    @retval Status - Success if GPIO's are read properly

**/
EFI_STATUS
GpioGetForcetoS1ConfigModePadVal (
  OUT UINT32 *ForceS1ConfigPad
);

/**

    Reads GPIO pin related to QAT

    @param[out] QATPad - Input value of the QAT pad

    @retval Status - Success if GPIO's are read properly

**/
EFI_STATUS
GpioGetQATPadVal (
  OUT UINT32 *QATPad
);


/**

    Get GPIO pin for SCI detection for WHEA RAS functionality

    @param[out] WheaSciPad - Input value of the Whea SCI pad

    @retval Status - Success if GPIO's pad read properly

**/
EFI_STATUS
GpioGetWheaSciPad (
  OUT UINT32 *WheaSciPad
);

/**

    Get GPIO pin for FPGA error detection RAS functionality

    @param[out] FpgaErrorPad -The input value of the FPGA error 1 pad

    @retval Status - Success if GPIO's pad read properly

**/
EFI_STATUS
GpioGetFpgaErrorPad1 (
  OUT UINT32 *FpgaErrorPad
);


/**

    Get GPIO pin for FPGA error detection RAS functionality

    @param[out] FpgaErrorPad -The input value of the FPGA error 2 pad

    @retval Status - Success if GPIO's pad read properly

**/
EFI_STATUS
GpioGetFpgaErrorPad2 (
  OUT UINT32 *FpgaErrorPad
);

/**

    Get GPIO pin for CPU HP SMI detection for RAS functionality

    @retval Status - Success if GPIO's pad read properly

**/
EFI_STATUS
GpioGetCpuHpSmiPad (
  OUT UINT32 *CpuHpSmiPad
);


/**

    Reads GPIO pin that is first bit of the Board ID indication word

    @param[out] BoardID0Gpio - Input value of the first Board ID pad

    @retval Status - Success if GPIO's are read properly

**/
EFI_STATUS
GpioGetBoardId0PadVal (
  OUT UINT32 *BoardID0Gpio
);


/**

    Checks whether the MDF jumper has been set

    @param None

    @retval ManufacturingMode - TRUE when MFG jumper is on, FALSE otherwise

**/
BOOLEAN
IsManufacturingMode (
  VOID
);


STATIC  EFI_GUID gPlatformGpioPlatformConfigDataGuid = PLATFORM_GPIO_PLATFORM_CONFIG_DATA_GUID;

#endif //_UBA_GPIO_PLATFORM_CONFIG_LIB_H

