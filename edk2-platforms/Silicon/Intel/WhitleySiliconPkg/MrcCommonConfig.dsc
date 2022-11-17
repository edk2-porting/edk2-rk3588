## @file
# Memory Reference Code configuration file.
#
# @copyright
# Copyright 2018 - 2021 Intel Corporation. <BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
##

#
# This file is for common defintions for all MRC build targets.
# This is not for platform specific defintions
#

[PcdsFixedAtBuild]

#
# MRC Debug configuration options
#

gEfiCpRcPkgTokenSpaceGuid.PcdMrcDebugTurnarounds|FALSE
gEfiCpRcPkgTokenSpaceGuid.PcdMrcDebugPerformanceStats|TRUE
gEfiCpRcPkgTokenSpaceGuid.PcdMrcDebugSenseAmp|FALSE
gEfiCpRcPkgTokenSpaceGuid.PcdMrcDisplayPerformanceValues|FALSE
gEfiCpRcPkgTokenSpaceGuid.PcdMrcDebugLateCmdClk|FALSE
gEfiCpRcPkgTokenSpaceGuid.PcdMrcDebugPowerTraining|FALSE
gEfiCpRcPkgTokenSpaceGuid.PcdMrcDebugLrdimmExtraMessages|FALSE
gEfiCpRcPkgTokenSpaceGuid.PcdMrcBeginEndDebugHooks|TRUE
gEfiCpRcPkgTokenSpaceGuid.PcdMrcDebugSwizzleInspection|FALSE

#
# Dimm support
#

gEfiCpRcPkgTokenSpaceGuid.PcdMrcQrDimmSupport|TRUE
gEfiCpRcPkgTokenSpaceGuid.PcdMrcSoDimmSupport|TRUE

#
# Miscellaneous MRC options
#

gEfiCpRcPkgTokenSpaceGuid.PcdMrcPurleyBiosHeaderOverride|TRUE
gEfiCpRcPkgTokenSpaceGuid.PcdMrcXmpSupport|TRUE

[PcdsFixedAtBuild.IA32]

gEfiCpRcPkgTokenSpaceGuid.PcdReserved7|TRUE

[BuildOptions]

#
# Size of NvramData structure, per socket
#
    DEFINE NUMBER_OF_SUB_CHANNELS = -D SUB_CH=1

    DEFINE NUMBER_OF_MAX_RANK_DIMM = -D MAX_RANK_DIMM=4
    DEFINE NUMBER_OF_MAX_RANK_CH   = -D MAX_RANK_CH=8

*_*_*_CC_FLAGS =  $(NUMBER_OF_SUB_CHANNELS) $(NUMBER_OF_MAX_RANK_DIMM) $(NUMBER_OF_MAX_RANK_CH)

#
# Always define IA32 for PEI, undefine for DXE and SMM
#

*_*_IA32_CC_FLAGS     = -D IA32
*_*_X64_CC_FLAGS     = -U IA32





