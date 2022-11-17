/** @file
  PCH FIVR policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _FIVR_CONFIG_H_
#define _FIVR_CONFIG_H_

#define FIVR_CONFIG_REVISION 1
extern EFI_GUID gFivrConfigGuid;

#pragma pack (push,1)

/**
  Rail support in S0ix and Sx
  Settings other than FivrRailDisabled can be OR'ed
**/
typedef enum {
  FivrRailDisabled   = 0,
  FivrRailInS0i1S0i2 = BIT0,
  FivrRailInS0i3     = BIT1,
  FivrRailInS3       = BIT2,
  FivrRailInS4       = BIT3,
  FivrRailInS5       = BIT4,
  FivrRailInS0ix     = FivrRailInS0i1S0i2 | FivrRailInS0i3,
  FivrRailInSx       = FivrRailInS3 | FivrRailInS4 | FivrRailInS5,
  FivrRailAlwaysOn   = FivrRailInS0ix | FivrRailInSx
} FIVR_RAIL_SX_STATE;

typedef enum {
  FivrRetentionActive = BIT0,
  FivrNormActive      = BIT1,
  FivrMinActive       = BIT2,
  FivrMinRetention    = BIT3
} FIVR_RAIL_SUPPORTED_VOLTAGE;

/**
  Structure for V1p05/Vnn VR rail configuration
**/
typedef struct {
  /**
    Mask to enable the usage of external VR rail in specific S0ix or Sx states
    Use values from FIVR_RAIL_SX_STATE
    The default is <b>FivrRailDisabled</b>.
  **/
  UINT32  EnabledStates   : 5;

  /**
    VR rail voltage value that will be used in S0i2/S0i3 states.
    This value is given in 2.5mV increments (0=0mV, 1=2.5mV, 2=5mV...)
    The default for Vnn is set to <b>420 - 1050 mV</b>.
  **/
  UINT32  Voltage         : 11;
  /**
    @deprecated
    THIS POLICY IS DEPRECATED, PLEASE USE IccMaximum INSTEAD
    VR rail Icc Max Value
    Granularity of this setting is 1mA and maximal possible value is 500mA
    The default is <b> 0mA </b>.
  **/
  UINT32  IccMax          : 8;

  /**
  This register holds the control hold off values to be used when
  changing the rail control for external bypass value in us
  **/
  UINT32   CtrlRampTmr    : 8;

  /**
    Mask to set the supported configuration in VR rail.
    Use values from FIVR_RAIL_SUPPORTED_VOLTAGE
  **/
  UINT32  SupportedVoltageStates   : 4;

  /**
    VR rail Icc Maximum Value
    Granularity of this setting is 1mA and maximal possible value is 500mA
    The default is <b> 0mA </b>.
  **/
  UINT32  IccMaximum                : 16;

  UINT32  RsvdBits1                 : 12;

} FIVR_EXT_RAIL_CONFIG;


/**
  Structure for VCCIN_AUX voltage rail configuration
**/
typedef struct {
  /**
  Transition time in microseconds from Low Current Mode Voltage to High Current Mode Voltage.
  Voltage transition time required by motherboard voltage regulator when PCH changes
  the VCCIN_AUX regulator set point from the low current mode voltage and high current mode voltage.
  This field has 1us resolution.
  When value is 0 PCH will not transition VCCIN_AUX to low current mode voltage.
  The default is <b> 0xC </b>.
  **/
  UINT8  LowToHighCurModeVolTranTime;

  /**
  Transition time in microseconds from Retention Mode Voltage to High Current Mode Voltage.
  Voltage transition time required by motherboard voltage regulator when PCH changes
  the VCCIN_AUX regulator set point from the retention mode voltage to high current mode voltage.
  This field has 1us resolution.
  When value is 0 PCH will not transition VCCIN_AUX to retention voltage.
  The default is <b> 0x36 </b>.
  **/
  UINT8  RetToHighCurModeVolTranTime;

  /**
  Transition time in microseconds from Retention Mode Voltage to Low Current Mode Voltage.
  Voltage transition time required by motherboard voltage regulator when PCH changes
  the VCCIN_AUX regulator set point from the retention mode voltage to low current mode voltage.
  This field has 1us resolution.
  When value is 0 PCH will not transition VCCIN_AUX to retention voltage.
  The default is <b> 0x2B </b>.
  **/
  UINT8  RetToLowCurModeVolTranTime;
  UINT8  RsvdByte1;
  /**
  Transition time in microseconds from Off (0V) to High Current Mode Voltage.
  Voltage transition time required by motherboard voltage regulator when PCH changes
  the VCCIN_AUX regulator set point from 0V to the high current mode voltage.
  This field has 1us resolution.
  0 = Transition to 0V is disabled
  Setting this field to 0 sets VCCIN_AUX as a fixed rail that stays on
  in all S0 & Sx power states after initial start up on G3 exit
  The default is <b> 0x96 </b>.
  **/
  UINT32  OffToHighCurModeVolTranTime : 11;
  UINT32  RsvdBits1                   : 21;
} FIVR_VCCIN_AUX_CONFIG;

/**
  The PCH_FIVR_CONFIG block describes FIVR settings.
**/
typedef struct {
  CONFIG_BLOCK_HEADER      Header;  ///< Config Block Header
  /**
    External V1P05 VR rail configuration.
  **/
  FIVR_EXT_RAIL_CONFIG     ExtV1p05Rail;
  /**
    External Vnn VR rail configuration.
  **/
  FIVR_EXT_RAIL_CONFIG     ExtVnnRail;
  /**
    Additional External Vnn VR rail configuration that will get applied
    in Sx entry SMI callback. Required only if External Vnn VR
    needs different settings for Sx than those specified in ExtVnnRail.
  **/
  FIVR_EXT_RAIL_CONFIG     ExtVnnRailSx;
  /**
    VCCIN_AUX voltage rail configuration.
  **/
  FIVR_VCCIN_AUX_CONFIG    VccinAux;

  /**
    Enable/Disable FIVR Dynamic Power Management
    Default is <b> 1 </b>.
  **/
  UINT32                   FivrDynPm : 1;
  UINT32                   RsvdBits2 : 31;
} PCH_FIVR_CONFIG;

#pragma pack (pop)

#endif // _FIVR_CONFIG_H_
