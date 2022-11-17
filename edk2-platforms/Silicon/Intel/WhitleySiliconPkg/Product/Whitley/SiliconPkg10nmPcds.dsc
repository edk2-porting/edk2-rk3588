## @file
# Pcd definitions for 14nm & 10nm wave1 & 10nm wave2 CPU.
#
# @copyright
# Copyright 2018 - 2021 Intel Corporation. <BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
##

################################################################################
#
# PCD definitions section - list of all PCD definitions needed by this Platform.
#
################################################################################
[PcdsFixedAtBuild]

  # Indicates the max nested level
  gEfiCpRcPkgTokenSpaceGuid.PcdMaxNestedLevel|0x00000010

  ## Socket count used to indicate maximum number of CPU sockets supported by the platform.
  gEfiCpRcPkgTokenSpaceGuid.PcdMaxCpuSocketCount|$(MAX_SOCKET)
  gEfiCpRcPkgTokenSpaceGuid.PcdMaxCpuCoreCount|$(MAX_CORE)
  gEfiCpRcPkgTokenSpaceGuid.PcdMaxCpuThreadCount|$(MAX_THREAD)

  ## Indicates whether it needs to clear temp bus assignment in PCIE common init library
  gEfiCpRcPkgTokenSpaceGuid.PcdCleanTempBusAssignment|TRUE

  # Default SMBUS speed for Whitley is 700Khz - see SMB_CLOCK_FREQUENCY definition
  gEfiCpRcPkgTokenSpaceGuid.PcdMrcSmbusSpeedDefault|0x2

!if (($(CPUTARGET) == "ICX"))
  # Overrides specific to ICX-SP
  gEfiCpRcPkgTokenSpaceGuid.PcdDimmIsolationDefault        |0

  # Enable LRDIMM DB DFE for ICX + PMem
  gEfiCpRcPkgTokenSpaceGuid.PcdMrcLrdimmDbDfeDefault       |1

  # Additional Buffer Delay for Roundtrip
  gEfiCpRcPkgTokenSpaceGuid.PcdRoundTripBufferDelayDclk    |8
!endif

  # Memory health check default
  # 00=>Auto (Use defaults), 01=>Manual (Override defaults with setup option), 02=>Disable (Disable feature)
!if (($(CPUTARGET) == "ICX"))
  gEfiCpRcPkgTokenSpaceGuid.PcdMemBootHealthCheck|2
!endif

  gEfiCpRcPkgTokenSpaceGuid.PcdMrcMultiThreadedDefault|TRUE

  #
  # Override MRC default values for SKX, defaults are all set
  # for 10nm
  #

!if $(CPU_SKX_ONLY_SUPPORT) == TRUE

  gEfiCpRcPkgTokenSpaceGuid.PcdMrcBdatDefault|FALSE
  gEfiCpRcPkgTokenSpaceGuid.PcdMrcWritePreambleTclkDefault|0x0
  gEfiCpRcPkgTokenSpaceGuid.PcdMrcReadPreambleTclkDefault|0x0
  gEfiCpRcPkgTokenSpaceGuid.PcdMrcRxDfeDefault|0x0
  gEfiCpRcPkgTokenSpaceGuid.PcdMrcMcOdtDefault|0x0
  gEfiCpRcPkgTokenSpaceGuid.PcdMrcTxRfSlewRateDefault|0x0
  gEfiCpRcPkgTokenSpaceGuid.PcdMrcPmemMemHoleDefault|TRUE
  gEfiCpRcPkgTokenSpaceGuid.PcdMrcCrQosConfigDefault|0x0

  gEfiCpRcPkgTokenSpaceGuid.PcdClttTempLoSingleRefreshDefault|82
  gEfiCpRcPkgTokenSpaceGuid.PcdClttTempMidSingleRefreshDefault|82
  gEfiCpRcPkgTokenSpaceGuid.PcdClttTempHiSingleRefreshDefault|100
  gEfiCpRcPkgTokenSpaceGuid.PcdClttTempMidSingleRefreshAepDefault|83

  gEfiCpRcPkgTokenSpaceGuid.PcdClttTempLoDoubleRefreshDefault|84
  gEfiCpRcPkgTokenSpaceGuid.PcdClttTempMidDoubleRefreshDefault|93
  gEfiCpRcPkgTokenSpaceGuid.PcdClttTempHiDoubleRefreshDefault|100

!else
  gEfiCpRcPkgTokenSpaceGuid.PcdMrcMcRonDefault|0x0
  gEfiCpRcPkgTokenSpaceGuid.PcdMrcDramRonDefault|0x0
!if ($(CPUTARGET) == "ICX")
  gEfiCpRcPkgTokenSpaceGuid.PcdMrcQosLowTimerLim| 0x290
  gEfiCpRcPkgTokenSpaceGuid.PcdMrcQosMedTimerLim|0x290
  gEfiCpRcPkgTokenSpaceGuid.PcdMrcQosHighTimerLim|0x290
!endif
!endif

  gEfiCpRcPkgTokenSpaceGuid.PcdReserved12|1

  gEfiCpRcPkgTokenSpaceGuid.PcdReserved13|FALSE

#
# Enable DDR4 and DDRT turnaround timing optimization for all Whitley
# platforms
#
  gEfiCpRcPkgTokenSpaceGuid.PcdMrcTurnaroundOptimizations|TRUE
  gEfiCpRcPkgTokenSpaceGuid.PcdMrcTurnaroundOptimizationsDdrt|TRUE

  #
  # enable NVDIMM support
  #
  gEfiCpRcPkgTokenSpaceGuid.PcdNvDimmEn|TRUE