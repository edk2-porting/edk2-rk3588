## @file
#  Component description file for the CN913x CEx7 Evaluation Board
#
#  Copyright (c) 2021 Semihalf
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = Cn913xCEx7Eval
  PLATFORM_GUID                  = 4e2ffdd1-c82e-497e-936b-76217e54848a
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x0001001B
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)-$(ARCH)
  SUPPORTED_ARCHITECTURES        = AARCH64|ARM
  BUILD_TARGETS                  = DEBUG|RELEASE|NOOPT
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = Silicon/Marvell/Armada7k8k/Armada7k8k.fdf
  BOARD_DXE_FV_COMPONENTS        = Platform/SolidRun/Cn913xCEx7Eval/Cn913xCEx7Eval.fdf.inc
  CAPSULE_ENABLE                 = TRUE

  #
  # Network definition
  #
  DEFINE NETWORK_IP6_ENABLE             = FALSE
  DEFINE NETWORK_TLS_ENABLE             = FALSE
  DEFINE NETWORK_HTTP_BOOT_ENABLE       = FALSE
  DEFINE NETWORK_ISCSI_ENABLE           = FALSE

!include Silicon/Marvell/Armada7k8k/Armada7k8k.dsc.inc
!include MdePkg/MdeLibs.dsc.inc
!include Platform/SolidRun/Cn913xCEx7Eval/Cn913xCEx7.dsc.inc
!include Platform/SolidRun/Cn913xCEx7Eval/Cn9130Eval.dsc.inc
!include Platform/SolidRun/Cn913xCEx7Eval/Cn9131Eval.dsc.inc
!include Platform/SolidRun/Cn913xCEx7Eval/Cn9132Eval.dsc.inc

[Components.common]
 Silicon/Marvell/OcteonTx/DeviceTree/T91/$(PLATFORM_NAME).inf

[Components.AARCH64]
  Silicon/Marvell/OcteonTx/AcpiTables/T91/$(PLATFORM_NAME).inf

[LibraryClasses.common]
  NonDiscoverableInitLib|Platform/SolidRun/Cn913xCEx7Eval/NonDiscoverableInitLib/NonDiscoverableInitLib.inf
  ArmadaBoardDescLib|Platform/SolidRun/Cn913xCEx7Eval/BoardDescriptionLib/BoardDescriptionLib.inf

[PcdsFixedAtBuild.common]
  #Platform description
  gMarvellTokenSpaceGuid.PcdProductManufacturer|"SolidRun"
  gMarvellTokenSpaceGuid.PcdProductPlatformName|"CN913x CEx7 Evaluation Board"
  gMarvellTokenSpaceGuid.PcdProductVersion|"Rev. 1.1"
