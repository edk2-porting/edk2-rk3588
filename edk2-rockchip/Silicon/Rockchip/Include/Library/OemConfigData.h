/** @file
*
*  Copyright (c) 2017 - 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2017 - 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/


#ifndef _OEM_CONFIG_DATA_H_
#define _OEM_CONFIG_DATA_H_

#define PCIE_MAX_TOTAL_PORTS                 16
#define OEM_CONFIG_NAME                      L"OemConfig"
#define PLATFORM_SETUP_VARIABLE_FLAG         (EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE)

#pragma pack(1)
typedef struct {
  /*Memory Config*/
  UINT8         DdrDebugLevel;
  UINT8         DdrFreqLimit;
  UINT8         DdrRefreshSupport;
  UINT8         DdrRefreshRate;
  UINT8         RankMargin;
  UINT8         RankMarginMode;
  UINT32        rmtPatternLength;
  UINT8         perbitmargin;
  UINT8         CaMargin;
  UINT8         CaVrefMarginOption;
  UINT8         NumaEn;
  UINT8         HwMemTest;
  UINT8         DieInterleaving;
  UINT8         ChannelInterleaving;
  UINT8         RankInterleaving;
  UINT8         EccSupport;
  /*iBMC Config*/
  UINT8         BmcWdtEnable;
  UINT8         BmcWdtTimeout;
  UINT8         BmcWdtAction;
  UINT8         OSWdtEnable;
  UINT8         OSWdtTimeout;
  UINT8         OSWdtAction;
  /*PCIe Config*/
  UINT8         PcieSRIOVSupport;
  UINT8         PciePort[PCIE_MAX_TOTAL_PORTS];
  UINT8         PcieLinkSpeedPort[PCIE_MAX_TOTAL_PORTS];
  UINT8         PcieLinkDeEmphasisPort[PCIE_MAX_TOTAL_PORTS];
  UINT8         PcieLinkStatusPort[PCIE_MAX_TOTAL_PORTS];
  UINT8         PcieLinkSpeedRateStatusPort[PCIE_MAX_TOTAL_PORTS];
  UINT8         PcieLinkMaxPort[PCIE_MAX_TOTAL_PORTS];
  UINT8         PcieMaxPayloadSizePort[PCIE_MAX_TOTAL_PORTS];
  UINT8         PcieAspmPort[PCIE_MAX_TOTAL_PORTS];
  /*Misc Config*/
  UINT8         EnableSmmu;
  UINT8         EnableFdtTable;
  UINT8         EnableGOP;
  /*RAS Config*/
  UINT8         EnRasSupport;
  UINT8         EnPoison;
  UINT8         CheckAlgorithm;
  UINT8         PatrolScrub;
  UINT8         PatrolScrubDuration;
  UINT8         DemandScrubMode;
  UINT8         CorrectErrorThreshold;
  UINT8         AdvanceDeviceCorrection;
  UINT8         RankSparing;
  UINT8         FunnelPeriod;
  UINT8         DpcFeature;
  UINT8         EcrcFeature;
  UINT8         CompletionTimeout;
  UINT8         CompletionTimeoutValue;
  UINT8         HotPlug;

} OEM_CONFIG_DATA;
#pragma pack()

#endif
