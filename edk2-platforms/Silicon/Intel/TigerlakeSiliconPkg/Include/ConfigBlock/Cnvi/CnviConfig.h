/** @file
  CNVi policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _CNVI_CONFIG_H_
#define _CNVI_CONFIG_H_

#define CNVI_CONFIG_REVISION 1
extern EFI_GUID gCnviConfigGuid;

#pragma pack (push,1)

/**
  CNVi Mode options
**/
typedef enum {
  CnviModeDisabled = 0,
  CnviModeAuto
} CNVI_MODE;


/**
  CNVi signals pin muxing settings. If signal can be enable only on a single pin
  then this parameter is ignored by RC. Refer to GPIO_*_MUXING_CNVI_* in GpioPins*.h
  for supported settings on a given platform
**/
typedef struct {
  UINT32 RfReset; ///< RF_RESET# Pin mux configuration. Refer to GPIO_*_MUXING_CNVI_RF_RESET_*
  UINT32 Clkreq;  ///< CLKREQ Pin mux configuration. Refer to GPIO_*_MUXING_CNVI_*_CLKREQ_*
} CNVI_PIN_MUX;

/**
  The CNVI_CONFIG block describes the expected configuration of the CNVi IP.

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;      ///< Config Block Header
  /**
    This option allows for automatic detection of Connectivity Solution.
    Auto Detection assumes that CNVi will be enabled when available;
    Disable allows for disabling CNVi.
    CnviModeDisabled = Disabled,
    <b>CnviModeAuto = Auto Detection</b>
  **/
  UINT32 Mode                  :  1;
  UINT32 BtCore                :  1; ///< The option to turn ON or OFF the BT Core. 0: Disabled, <b>1: Enabled</b>
  /**
    The option to enable or disable BT Audio Offload.
    <b>0: Disabled</b>, 1: Enabled
    @note This feature only support with Intel(R) Wireless-AX 22560
  **/
  UINT32 BtAudioOffload        :  1;
  UINT32 RsvdBits              : 29;
  /**
    CNVi PinMux Configuration
    RESET#/CLKREQ to CRF, can have two alternative mappings, depending on board routing requirements.
  **/
  CNVI_PIN_MUX          PinMux;
} CNVI_CONFIG;

#pragma pack (pop)

#endif // _CNVI_CONFIG_H_
