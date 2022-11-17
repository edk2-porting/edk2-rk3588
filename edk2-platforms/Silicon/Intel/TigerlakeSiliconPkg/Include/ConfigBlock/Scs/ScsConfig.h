/** @file
  Scs policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _SCS_CONFIG_H_
#define _SCS_CONFIG_H_

#include <ConfigBlock.h>
#include <PchLimits.h>

#define SCS_SDCARD_CONFIG_REVISION     1
#define SCS_EMMC_CONFIG_REVISION       1
#define SCS_EMMC_DXE_CONFIG_REVISION   1
#define SCS_SDCARD_MAX_DATA_GPIOS      4
#define SCS_EMMC_MAX_DATA_GPIOS        8
extern EFI_GUID gSdCardConfigGuid;
extern EFI_GUID gEmmcConfigGuid;
extern EFI_GUID gUfsConfigGuid;
extern EFI_GUID gEmmcDxeConfigGuid;

#pragma pack (push,1)

/**
  This structre holds the DLL configuration
  register values that will be programmed by RC
  if EnableCustomDlls field is set to TRUE. Those
  policies should be used by platform if default values
  provided by RC are not sufficient to provide stable operation
  at all supported spped modes. RC will blindly set the DLL values
  as provided in this structre.

  For help with obtaining valid DLL values for your platform please
  contact enabling support.
**/
typedef struct {
  UINT32  TxCmdDelayControl;      // Offset 820h: Tx CMD Delay Control
  UINT32  TxDataDelayControl1;    // Offset 824h: Tx Data Delay Control 1
  UINT32  TxDataDelayControl2;    // Offset 828h: Tx Data Delay Control 2
  UINT32  RxCmdDataDelayControl1; // Offset 82Ch: Rx CMD + Data Delay Control 1
  UINT32  RxCmdDataDelayControl2; // Offset 834h: Rx CMD + Data Delay Control 2
  UINT32  RxStrobeDelayControl;   // Offset 830h: Rx Strobe Delay Control, valid only for eMMC
} SCS_SD_DLL;

/**
  SD GPIO settings
**/
typedef struct {
  /**
    GPIO signals pin muxing settings. If signal can be enable only on a single pin
    then this parameter should be set to 0. Refer to GPIO_*_MUXING_SDCARD_*x_* in GpioPins*.h
    for supported settings on a given platform
  **/
  UINT32   PinMux;
  /**
    GPIO Pads Internal Termination.
    For more information please see Platform Design Guide.
    Check GPIO_ELECTRICAL_CONFIG for reference
  **/
  UINT32   PadTermination;
} MUX_GPIO_PARAM;

typedef struct {
  MUX_GPIO_PARAM PowerEnable;
  MUX_GPIO_PARAM Cmd;
  MUX_GPIO_PARAM Data[SCS_SDCARD_MAX_DATA_GPIOS];
  MUX_GPIO_PARAM Cdb;
  MUX_GPIO_PARAM Clk;
  MUX_GPIO_PARAM Wp;
} SCS_SDCARD_GPIO_CONFIG;

typedef struct {
  MUX_GPIO_PARAM Cmd;
  MUX_GPIO_PARAM Data[SCS_EMMC_MAX_DATA_GPIOS];
  MUX_GPIO_PARAM Rclk;
  MUX_GPIO_PARAM Clk;
  MUX_GPIO_PARAM Resetb;
} SCS_EMMC_GPIO_CONFIG;

typedef struct {
  CONFIG_BLOCK_HEADER  Header;

  UINT32      Enable                : 1;  ///<  Enable/Disable SdCard 0: Disabled, <b>1: Enabled</b>
  UINT32      PowerEnableActiveHigh : 1;  ///<  Determine SD_PWREN# polarity 0: Active low, <b>1: Active high</b>
  UINT32      UseCustomDlls         : 1;  ///<  Use tuned DLL values from policy <b>0: Use default DLL</b>, 1: Use values from TunedDllValues field
  UINT32      Reserved              : 29;
  SCS_SD_DLL  CustomDllValues;            ///<  Structure containing custom DLL values for SD card
  SCS_SDCARD_GPIO_CONFIG GpioConfig;
} SCS_SDCARD_CONFIG;

typedef struct {
  UINT32  Hs400RxValue : 7;  ///< Value of the tuned HS400 Rx value
  UINT32  Hs400TxValue : 7;  ///< Value of the tuned HS400 Tx value
  UINT32  Reserved     : 18;
} SCS_EMMC_TUNED_DLL;

typedef struct {
  CONFIG_BLOCK_HEADER  Header;

  UINT32      Enable         : 1;  ///<  Enable/Disable eMMC 0: Disabled, <b>1: Enabled</b>
  UINT32      Hs400Supported : 1;  ///<  Enable/Disable eMMC HS400 support 0: Disabled, <b>1: Enabled</b>
  UINT32      UseCustomDlls  : 1;  ///<  Use custom DLL values from policy <b>0: Use default DLL</b>, 1: Use values from TunedDllValues field
  UINT32      Reserved       : 29;
  SCS_SD_DLL  CustomDllValues;     ///<  Structure containing custom DLL values for eMMC                    ///< Structure containing tuned DLL settings for eMMC
  SCS_EMMC_GPIO_CONFIG GpioConfig;
} SCS_EMMC_CONFIG;

typedef enum {
  DriverStrength33Ohm = 0,
  DriverStrength40Ohm,
  DriverStrength50Ohm
} SCS_EMMC_DRIVER_STRENGTH;

typedef struct {
  UINT32  TuningSuccessful  : 1;  ///< Informs software tuning module about previous software tuning status.
  UINT32  Hs400RxValue      : 7;  ///< Value of the tuned HS400 Rx value returned from software tuning module
  UINT32  Hs400TxValue      : 7;  ///< Value of the tuned HS400 Tx value returned from software tuning module
  UINT32  Reserved          : 17;
} SCS_EMMC_SOFTWARE_TUNING_RESULTS;

typedef struct {
  CONFIG_BLOCK_HEADER  Header;

  UINT32   EnableSoftwareHs400Tuning         : 1;           ///< Enable/Disable software eMMC HS400 tuning: <b>0 - Disable</b>, 1 - Enable
  UINT32   DriverStrength                    : 2;           ///< I/O driver strength: 0 - 33 Ohm, <b>1 - 40 Ohm</b>, 2 - 50 Ohm
  UINT32   Reserved                          : 29;
  EFI_LBA  TuningLba;                                       ///< Specifies LBA which will be used during software tuning process.
  SCS_EMMC_SOFTWARE_TUNING_RESULTS  PreviousTuningResults;  ///< Informes software tuning module about previous software tuning results.} SCS_EMMC_DXE_CONFIG;
} SCS_EMMC_DXE_CONFIG;

typedef struct {
  UINT32  Enable   : 1;  ///< Enable/Disable UFS controller 0: Disabled, <b>1: Enabled</b>
  UINT32  Reserved : 31;
} SCS_UFS_CONTROLLER_CONFIG;

#pragma pack (pop)

#endif // _SCS_CONFIG_H_
