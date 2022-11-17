/** @file
  Scs policy

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SCS_CONFIG_H_
#define _SCS_CONFIG_H_

#include <ConfigBlock.h>

#define SCS_CONFIG_REVISION 2
extern EFI_GUID gScsConfigGuid;

#pragma pack (push,1)

typedef enum {
  DriverStrength33Ohm = 0,
  DriverStrength40Ohm,
  DriverStrength50Ohm
} PCH_SCS_EMMC_DRIVER_STRENGTH;

/**
  The PCH_SCS_CONFIG block describes Storage and Communication Subsystem (SCS) settings for PCH.

  <b>Revision 1</b>:
  - Initial version
  <b>Revision 2</b>:
  - Add policy SdCardPowerEnableActiveHigh
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header

  UINT32    ScsEmmcEnabled                :  2;   ///< Determine if eMMC is enabled - 0: Disabled, <b>1: Enabled</b>.
  UINT32    ScsEmmcHs400Enabled           :  1;   ///< Determine eMMC HS400 Mode if ScsEmmcEnabled - <b>0: Disabled</b>, 1: Enabled
  /**
    Determine if HS400 Training is required, set to FALSE if Hs400 Data is valid. <b>0: Disabled</b>, 1: Enabled.
    First Boot or CMOS clear, system boot with Default settings, set tuning required.
    Subsequent Boots, Get Variable 'Hs400TuningData'
      - if failed to get variable, set tuning required
      - if passed, retrieve Hs400DataValid, Hs400RxStrobe1Dll and Hs400TxDataDll from variable. Set tuning not required.
  **/
  UINT32    ScsEmmcHs400TuningRequired    :  1;
  UINT32    ScsEmmcHs400DllDataValid      :  1;   ///< Set if HS400 Tuning Data Valid
  UINT32    ScsEmmcHs400RxStrobeDll1      :  7;   ///< Rx Strobe Delay Control - Rx Strobe Delay DLL 1 (HS400 Mode)
  UINT32    ScsEmmcHs400TxDataDll         :  7;   ///< Tx Data Delay Control 1 - Tx Data Delay (HS400 Mode)
  UINT32    ScsEmmcHs400DriverStrength    :  3;   ///< I/O driver strength: 0 - 33 Ohm, <b>1 - 40 Ohm</b>, 2 - 50 Ohm
  /**
    Sd Card Controler 0: Disable; <b>1: Enable</b>.
    For Desktop sku, the SD Card Controller POR should be disabled. <b> 0:Disable </b>.
  **/
  UINT32    ScsSdcardEnabled              :  1;
  UINT32    ScsUfsEnabled                 :  1;   ///< Determine if Ufs is enabled 0: Disabled 1: Enabled
  UINT32    SdCardPowerEnableActiveHigh   :  1;   ///< Determine SD_PWREN# polarity 0: Active low, <b>1: Active high</b>
  UINT32    RsvdBits                      :  7;
  UINT32    Rsvd0;                                ///< Reserved bytes
} PCH_SCS_CONFIG;

#pragma pack (pop)

#endif // _SCS_CONFIG_H_
