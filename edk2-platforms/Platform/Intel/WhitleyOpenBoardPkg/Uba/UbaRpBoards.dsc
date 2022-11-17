## @file UbaRpBoards.dsc
# UBA DSC include file for board specific build items
#
# @copyright
# Copyright 2012 - 2021 Intel Corporation. <BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
##

[Components.IA32]
#
# Board Init Peim
#
$(RP_PKG)/Uba/BoardInit/Pei/BoardInitPei.inf {
        <LibraryClasses>
           NULL|$(RP_PKG)/Uba/UbaMain/TypeWilsonCityRP/Pei/PeiBoardInitLib.inf
           NULL|$(RP_PKG)/Uba/UbaMain/TypeCooperCityRP/Pei/PeiBoardInitLib.inf
           NULL|$(RP_PKG)/Uba/UbaMain/TypeWilsonCitySMT/Pei/PeiBoardInitLib.inf
           #
           #### NO PLATFORM SPECIFIC LIBRARY CLASSES AFTER THIS LINE!!!!
           #
           # Do not place any platform specific PeiBoardInitLib.inf entries after PeiCommonBoardInitLib.inf
           # The order of this libary class list is translated directly into the autogen.c created
           # to execute the libary constructors for all the platforms in this list.
           # PeiCommonBoardInitLib.inf depends on being the last constructor to execute and
           # assumes that a platform specific constructor has executed and installed its UBA
           # configuration information.
           #
           NULL|$(RP_PKG)/Uba/UbaMain/Common/Pei/PeiCommonBoardInitLib.inf
}

[Components.X64]
#
# Platform TypeWilsonCityRP
#
$(RP_PKG)/Uba/UbaMain/TypeWilsonCityRP/Dxe/UsbOcUpdateDxe/UsbOcUpdateDxe.inf
$(RP_PKG)/Uba/UbaMain/TypeWilsonCityRP/Dxe/IioCfgUpdateDxe/IioCfgUpdateDxe.inf
$(RP_PKG)/Uba/UbaMain/TypeWilsonCityRP/Dxe/SlotDataUpdateDxe/SlotDataUpdateDxe.inf

#
# Platform TypeCooperCityRP
#
$(RP_PKG)/Uba/UbaMain/TypeCooperCityRP/Dxe/UsbOcUpdateDxe/UsbOcUpdateDxe.inf
$(RP_PKG)/Uba/UbaMain/TypeCooperCityRP/Dxe/IioCfgUpdateDxe/IioCfgUpdateDxe.inf
$(RP_PKG)/Uba/UbaMain/TypeCooperCityRP/Dxe/SlotDataUpdateDxe/SlotDataUpdateDxe.inf

#
# Platform TypeWilsonCitySMT
#
$(RP_PKG)/Uba/UbaMain/TypeWilsonCitySMT/Dxe/UsbOcUpdateDxe/UsbOcUpdateDxe.inf
$(RP_PKG)/Uba/UbaMain/TypeWilsonCitySMT/Dxe/IioCfgUpdateDxe/IioCfgUpdateDxe.inf
$(RP_PKG)/Uba/UbaMain/TypeWilsonCitySMT/Dxe/SlotDataUpdateDxe/SlotDataUpdateDxe.inf
