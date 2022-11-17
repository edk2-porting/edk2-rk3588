/** @file
  Scs policy

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _SCS_CONFIG_H_
#define _SCS_CONFIG_H_

#define SCS_CONFIG_REVISION 1
extern EFI_GUID gScsConfigGuid;

#pragma pack (push,1)

typedef enum {
  PchScsSdDisabled = 0,
  PchScsSdcardMode = 2
} PCH_SCS_DEV_SD_MODE;

typedef enum {
  DriverStrength33Ohm = 0,
  DriverStrength40Ohm,
  DriverStrength50Ohm
} PCH_SCS_EMMC_DRIVER_STRENGTH;

///
/// The PCH_SCS_CONFIG block describes Storage and Communication Subsystem (SCS) settings for PCH.
///
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
      - if driver strength value changes (ScsEmmcHs400DriverStrength) re-tuning is required.
  **/
  UINT32    ScsEmmcHs400TuningRequired    :  1;
  UINT32    ScsEmmcHs400DllDataValid      :  1;   ///< Set if HS400 Tuning Data Valid
  UINT32    ScsEmmcHs400RxStrobeDll1      :  7;   ///< Rx Strobe Delay Control - Rx Strobe Delay DLL 1 (HS400 Mode)
  UINT32    ScsEmmcHs400TxDataDll         :  7;   ///< Tx Data Delay Control 1 - Tx Data Delay (HS400 Mode)
  UINT32    ScsEmmcHs400DriverStrength    :  3;   ///< I/O driver strength: 0 - 33 Ohm, <b>1 - 40 Ohm</b>, 2 - 50 Ohm
  UINT32    ScsSdSwitch                   :  3;   ///< Determine the operating mode of SDHC. Refer to PCH_SCS_DEV_SD_MODE for each value - 0: Disabled, <b>2: SDCard</b>.
  UINT32    RsvdBits                      :  7;
  UINT32    Rsvd0;                                ///< Reserved bytes
} PCH_SCS_CONFIG;

#pragma pack (pop)

#endif // _SCS_CONFIG_H_
