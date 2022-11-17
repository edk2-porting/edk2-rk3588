/** @file

  @copyright
  Copyright 2020 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PLATFORM_INFO_TYPES_H_
#define _PLATFORM_INFO_TYPES_H_

//
// DIMM Connector type
//
typedef enum {
  DimmConnectorPth = 0x00, // Through hole connector
  DimmConnectorSmt,        // Surface mount connector
  DimmConnectorMemoryDown, // Platform soldered DRAMs
  DimmConnectorIgnore,     // Ignore connector type
  DimmConnectorMax
} EFI_MEMORY_DIMM_CONNECTOR_TYPE;

//
// Platform types - used with EFI_PLATFORM_INFO BoardId
//
typedef enum {
  StartOfEfiPlatformTypeEnum = 0x00,
  //For PPO
  TypeNeonCityEPRP,
  TypeWolfPass,
  TypeTennesseePass,
  TypeHedtCRB,
  TypeLightningRidgeEXRP,
  TypeLightningRidgeEX8S1N,
  TypeBarkPeak,
  TypeYubaCityRP,
  TypeRidgeport,
  //End PPO
  TypeWilsonCityRP,
  TypeWilsonCityModular,
  TypeCoyotePass,
  TypeIdaville,
  TypeMoroCityRP,
  TypeBrightonCityRp,
  TypeJacobsville,
  TypeSnrSvp,
  TypeSnrSvpSodimm,
  TypeJacobsvilleMDV,
  TypeFrostCreekRP,
  TypeVictoriaCanyonRP,
  TypeArcherCityRP,
  TypeNeonCityEPECB,
  TypeIsoscelesPeak,
  TypeWilsonPointRP,
  TypeWilsonPointModular,
  TypeBretonSound,
  TypeWilsonCityPPV,
  TypeCooperCityRP,
  TypeWilsonCitySMT,
  TypeSnrSvpSodimmB,
  TypeArcherCityModular,
  TypeArcherCityEVB,
  TypeArcherCityXPV,
  TypeBigPineKey,
  TypeExperWorkStationRP,
  EndOfEfiPlatformTypeEnum
} EFI_PLATFORM_TYPE;

#define TypePlatformUnknown       0xFF
#define TypePlatformMin           StartOfEfiPlatformTypeEnum + 1
#define TypePlatformMax           EndOfEfiPlatformTypeEnum - 1
#define TypePlatformDefault       TypeWilsonPointRP

//
// CPU type: Standard (no MCP), -F, etc
//
typedef enum {
  CPU_TYPE_STD,
  CPU_TYPE_F,
  CPU_TYPE_P,
  CPU_TYPE_MAX
} CPU_TYPE;

#define CPU_TYPE_STD_MASK (1 << CPU_TYPE_STD)
#define CPU_TYPE_F_MASK   (1 << CPU_TYPE_F)
#define CPU_TYPE_P_MASK   (1 << CPU_TYPE_P)

typedef enum {
  DaisyChainTopology = 0x00,
  InvSlotsDaisyChainTopology,
  TTopology
} EFI_MEMORY_TOPOLOGY_TYPE;

//
// Values for SocketConfig
//

#define SOCKET_UNDEFINED  0
#define SOCKET_4S         1
#define SOCKET_HEDT       2
#define SOCKET_1S         3
#define SOCKET_1SWS       4
#define SOCKET_8S         5
#define SOCKET_2S         6

#endif // #ifndef _PLATFORM_INFO_TYPES_H_
