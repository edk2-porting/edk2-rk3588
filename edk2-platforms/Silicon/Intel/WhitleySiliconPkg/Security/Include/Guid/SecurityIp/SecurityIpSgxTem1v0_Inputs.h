/** @file
  Provides data structure information used by SiliconIp SGX-TEM

  @copyright
  Copyright 2020 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//
// SGX
//
UINT8  EnableSgx;                               ///< Enable SGX
UINT8  SgxFactoryReset;                         ///< Delete all registration data, if SGX enabled force IPE/FirstBinding flow
UINT64 PrmrrSize;                               ///< SGX PRMRR size
UINT64 ReservedS239;
UINT8  SgxQoS;                                  ///< SGX Quality of Service
UINT8  SgxAutoRegistrationAgent;                ///< SGX Auto Registration Agent
UINT8  SgxPackageInfoInBandAccess;              ///< SGX Expose Package Info to OS
UINT8  EpochUpdate;                             ///< SGX EPOCH Update
UINT64 SgxEpoch0;                               ///< SGX EPOCH0 value {0 - 0xFFFFFFFFFFFFFFFF}
UINT64 SgxEpoch1;                               ///< SGX EPOCH1 value {0 - 0xFFFFFFFFFFFFFFFF}
UINT8  SgxLeWr;                                 ///< Flexible Launch Enclave Policy (Wr En)
UINT64 SgxLePubKeyHash0;                        ///< Launch Enclave Hash 0
UINT64 SgxLePubKeyHash1;                        ///< Launch Enclave Hash 1
UINT64 SgxLePubKeyHash2;                        ///< Launch Enclave Hash 2
UINT64 SgxLePubKeyHash3;                        ///< Launch Enclave Hash 3

//
// DEPRECATED
//
UINT8  SgxSinitNvsData;                         ///< @deprecated SGX NVS data from Flash passed during previous boot using CPU_INFO_PROTOCOL.SGX_INFO;
                                                ///              Pass value of zero if there is not data saved or when SGX is disabled.
UINT8  SgxSinitDataFromTpm;                     ///< @deprecated SGX SVN data from TPM; 0: when SGX is disabled or TPM is not present or no data
                                                ///              is present in TPM.
UINT8  SgxDebugMode;                            ///< @deprecated

UINT8  ReservedS240;
UINT8  ReservedS241;
UINT8  ReservedS242;
UINT8  ReservedS243;
UINT8  ReservedS244;
