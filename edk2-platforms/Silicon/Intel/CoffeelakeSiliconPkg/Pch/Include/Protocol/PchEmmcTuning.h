/** @file
  PCH eMMC HS400 Tuning Protocol

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_EMMC_TUNING_PROTOCOL_H_
#define _PCH_EMMC_TUNING_PROTOCOL_H_

#define PCH_EMMC_TUNING_PROTOCOL_REVISION 1
//
// Extern the GUID for protocol users.
//
extern EFI_GUID  gPchEmmcTuningProtocolGuid;

//
// Forward declaration for PCH_EMMC_TUNING_PROTOCOL
//
typedef struct _PCH_EMMC_TUNING_PROTOCOL PCH_EMMC_TUNING_PROTOCOL;

/**
  This structure decribes the required Emmc info for HS400 tuning
**/
typedef struct {
  EFI_HANDLE                PartitionHandle;    ///< eMMC partition handle for block read/write
  EFI_LBA                   Lba;                ///< Logical Block Address for HS400 Tuning block read/write
  UINT32                    RelativeDevAddress; ///< Device system address, dynamically assigned by the host during initialization.
  UINT8                     HS200BusWidth;      ///< The value to be programmed for BUS_WIDTH[183] byte
} EMMC_INFO;

///
/// This structure describes the return value after HS400 tuning
///
typedef struct {
  UINT8       Hs400DataValid;     ///< Set if Hs400 Tuning Data is valid after tuning
  UINT8       Hs400RxStrobe1Dll;  ///< Rx Strobe Delay Control - Rx Strobe Delay DLL 1 (HS400 Mode)
  UINT8       Hs400TxDataDll;     ///< Tx Data Delay Control 1 - Tx Data Delay (HS400 Mode)
} EMMC_TUNING_DATA;

///
/// EMMC HS400 TUNING INTERFACE
///
typedef EFI_STATUS (EFIAPI *EMMC_TUNE) (
  IN   PCH_EMMC_TUNING_PROTOCOL         *This,              ///< This pointer to PCH_EMMC_TUNING_PROTOCOL
  /**
    Revision parameter is used to verify the layout of EMMC_INFO and TUNINGDATA.
    If the revision is not matched, means the revision of EMMC_INFO and TUNINGDATA is not matched.
    And function will return immediately.
  **/
  IN   UINT8                            Revision,
  IN   EMMC_INFO                        *EmmcInfo,          ///< Pointer to EMMC_INFO
  OUT  EMMC_TUNING_DATA                 *EmmcTuningData     ///< Pointer to EMMC_TUNING_DATA
);

/**
  PCH EMMC TUNING PROTOCOL INTERFACE
  Platform code uses this protocol to configure Emmc Hs400 mode, by passing the EMMC_INFO information.
  PCH will setting EMMC controller based on EMMC_INFO and return EMMC_TUNING_DATA to platform code.
  Platform should keep values of EMMC_TUNING_DATA and uses to configure EMMC through policies, to
  prevent from doing EMMC tuning every boot.
**/
struct _PCH_EMMC_TUNING_PROTOCOL {
  EMMC_TUNE  EmmcTune;  ///< Emmc Hs400 Tuning Interface
};

#endif
