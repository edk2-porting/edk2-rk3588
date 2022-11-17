/** @file
  Thermal policy

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _THERMAL_CONFIG_H_
#define _THERMAL_CONFIG_H_

#define THERMAL_CONFIG_REVISION 1
extern EFI_GUID gThermalConfigGuid;

#pragma pack (push,1)

/**
  This structure lists PCH supported throttling register setting for custimization.
  When the SuggestedSetting is enabled, the customized values are ignored.
**/
typedef struct {
  UINT32 T0Level                  :  9; ///< Custimized T0Level value. If SuggestedSetting is used, this setting is ignored.
  UINT32 T1Level                  :  9; ///< Custimized T1Level value. If SuggestedSetting is used, this setting is ignored.
  UINT32 T2Level                  :  9; ///< Custimized T2Level value. If SuggestedSetting is used, this setting is ignored.
  UINT32 TTEnable                 :  1; ///< Enable the thermal throttle function. If SuggestedSetting is used, this settings is ignored.
  /**
    When set to 1 and the programmed GPIO pin is a 1, then PMSync state 13 will force at least T2 state.
    If SuggestedSetting is used, this setting is ignored.
  **/
  UINT32 TTState13Enable          :  1;
  /**
    When set to 1, this entire register (TL) is locked and remains locked until the next platform reset.
    If SuggestedSetting is used, this setting is ignored.
  **/
  UINT32 TTLock                   :  1;
  UINT32 SuggestedSetting         :  1; ///< 0: Disable; <b>1: Enable</b> suggested representative values.
  /**
    ULT processors support thermal management and cross thermal throttling between the processor package
    and LP PCH. The PMSYNC message from PCH to CPU includes specific bit fields to update the PCH
    thermal status to the processor which is factored into the processor throttling.
    Enable/Disable PCH Cross Throttling; 0: Disabled, 1: <b>Enabled</b>.
  **/
  UINT32 PchCrossThrottling       :  1;
  UINT32 Rsvd0;                      ///< Reserved bytes
} THERMAL_THROTTLE_LEVELS;

/**
  This structure allows to customize DMI HW Autonomous Width Control for Thermal and Mechanical spec design.
  When the SuggestedSetting is enabled, the customized values are ignored.
  Look at DMI_THERMAL_SENSOR_TARGET_WIDTH for possible values
**/
typedef struct {
  UINT32  DmiTsawEn               :  1; ///< DMI Thermal Sensor Autonomous Width Enable
  UINT32  SuggestedSetting        :  1; ///< 0: Disable; <b>1: Enable</b> suggested representative values
  UINT32  RsvdBits0               :  6; ///< Reserved bits
  UINT32  TS0TW                   :  3; ///< Thermal Sensor 0 Target Width (<b>DmiThermSensWidthx8</b>)
  UINT32  TS1TW                   :  3; ///< Thermal Sensor 1 Target Width (<b>DmiThermSensWidthx4</b>)
  UINT32  TS2TW                   :  3; ///< Thermal Sensor 2 Target Width (<b>DmiThermSensWidthx2</b>)
  UINT32  TS3TW                   :  3; ///< Thermal Sensor 3 Target Width (<b>DmiThermSensWidthx1</b>)
  UINT32  RsvdBits1               : 12; ///< Reserved bits
} DMI_HW_WIDTH_CONTROL;

/**
  This structure configures PCH memory throttling thermal sensor GPIO PIN settings
**/
typedef struct {
  /**
    GPIO PM_SYNC enable, 0:Diabled, 1:<b>Enabled</b>
    When enabled, RC will overrides the selected GPIO native mode.
    For GPIO_C, PinSelection 0: CPU_GP_0 (default) or 1: CPU_GP_1
    For GPIO_D, PinSelection 0: CPU_GP_3 (default) or 1: CPU_GP_2
  **/
  UINT32  PmsyncEnable     :  1;
  UINT32  C0TransmitEnable :  1;        ///< GPIO Transmit enable in C0 state, 0:Disabled, 1:<b>Enabled</b>
  UINT32  PinSelection     :  1;        ///< GPIO Pin assignment selection, <b>0: default</b>, 1: secondary
  UINT32  RsvdBits0        : 29;
} TS_GPIO_PIN_SETTING;

enum PCH_PMSYNC_GPIO_X_SELECTION {
  TsGpioC,
  TsGpioD,
  MaxTsGpioPin
};

/**
  This structure supports an external memory thermal sensor (TS-on-DIMM or TS-on-Board).
**/
typedef struct {
  /**
   This will enable PCH memory throttling.
   While this policy is enabled, must also enable EnableExtts in SA policy.
   <b>0: Disable</b>; 1: Enable
  **/
  UINT32   Enable           :  1;
  UINT32   RsvdBits0        : 31;
  /**
    GPIO_C and GPIO_D selection for memory throttling.
    It's strongly recommended to choose GPIO_C and GPIO_D for memory throttling feature,
    and route EXTTS# accordingly.
  **/
  TS_GPIO_PIN_SETTING     TsGpioPinSetting[2];
} PCH_MEMORY_THROTTLING;

/**
  The PCH_THERMAL_CONFIG block describes the expected configuration of the PCH for Thermal.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  /**
    This locks down "SMI Enable on Alert Thermal Sensor Trip". 0: Disabled, 1: <b>Enabled</b>.
  **/
  UINT32  TsmicLock               :  1;
  UINT32  PchHotEnable            :  1; ///< Enable PCHHOT# pin assertion when temperature is higher than PchHotLevel. 0: <b>Disabled<b>, 1: Enabled.
  UINT32  RsvdBits0               : 30;
  /**
    This field decides the settings of Thermal throttling. When the Suggested Setting
    is enabled, PCH RC will use the suggested representative values.
  **/
  THERMAL_THROTTLE_LEVELS   TTLevels;
  /**
    This field decides the settings of DMI throttling. When the Suggested Setting
    is enabled, PCH RC will use the suggested representative values.
  **/
  DMI_HW_WIDTH_CONTROL      DmiHaAWC;
  /**
    Memory Thermal Management settings
  **/
  PCH_MEMORY_THROTTLING     MemoryThrottling;
  /**
    This field decides the temperature, default is <b>0x154</b>.
    The recommendation is the same as Cat Trip point.
  **/
  UINT16                    PchHotLevel;
  UINT8                     Rsvd0[6];
} PCH_THERMAL_CONFIG;

#pragma pack (pop)

#endif // _THERMAL_CONFIG_H_
