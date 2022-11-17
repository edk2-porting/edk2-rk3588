## @file
#  Component description file for the CN9130 Development Board (variant A)
#
#  Copyright (c) 2019 Marvell International Ltd.<BR>
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
!if $(CN9130)
  PLATFORM_NAME                  = Cn9130DbA
!elseif $(CN9131)
  PLATFORM_NAME                  = Cn9131DbA
!elseif $(CN9132)
  PLATFORM_NAME                  = Cn9132DbA
!endif
  PLATFORM_GUID                  = 087305a1-8ddd-4027-89ca-68a3ef78fcc7
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x0001001B
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)-$(ARCH)
  SUPPORTED_ARCHITECTURES        = AARCH64|ARM
  BUILD_TARGETS                  = DEBUG|RELEASE|NOOPT
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = Silicon/Marvell/Armada7k8k/Armada7k8k.fdf
  BOARD_DXE_FV_COMPONENTS        = Platform/Marvell/Cn913xDb/Cn913xDbA.fdf.inc

  #
  # Network definition
  #
  DEFINE NETWORK_IP6_ENABLE             = FALSE
  DEFINE NETWORK_TLS_ENABLE             = FALSE
  DEFINE NETWORK_HTTP_BOOT_ENABLE       = FALSE
  DEFINE NETWORK_ISCSI_ENABLE           = FALSE

!include Silicon/Marvell/Armada7k8k/Armada7k8k.dsc.inc
!include MdePkg/MdeLibs.dsc.inc
!include Platform/Marvell/Cn913xDb/Cn9130DbA.dsc.inc
!if $(CN9131) || $(CN9132)
!include Platform/Marvell/Cn913xDb/Cn9131DbA.dsc.inc
!endif
!if $(CN9132)
!include Platform/Marvell/Cn913xDb/Cn9132DbA.dsc.inc
!endif

[Components.common]
  Silicon/Marvell/OcteonTx/DeviceTree/T91/$(PLATFORM_NAME).inf

!ifndef $(CN9132)
[Components.AARCH64]
  Silicon/Marvell/OcteonTx/AcpiTables/T91/$(PLATFORM_NAME).inf
!else
  Silicon/Marvell/OcteonTx/AcpiTables/T91/Cn9131DbA.inf
!endif

[LibraryClasses.common]
!if $(CN9132)
  ArmadaBoardDescLib|Platform/Marvell/Cn913xDb/BoardDescriptionLib/Cn9132DbABoardDescLib.inf
!else
  ArmadaBoardDescLib|Platform/Marvell/Cn913xDb/BoardDescriptionLib/Cn9130DbABoardDescLib.inf
!endif
  NonDiscoverableInitLib|Platform/Marvell/Cn913xDb/NonDiscoverableInitLib/NonDiscoverableInitLib.inf

[PcdsFixedAtBuild.common]
  #Platform description
  !if $(CN9130)
  gMarvellTokenSpaceGuid.PcdProductPlatformName|"CN9130 DB-A"
  !elseif $(CN9131)
  gMarvellTokenSpaceGuid.PcdProductPlatformName|"CN9131 DB-A"
  !elseif $(CN9132)
  gMarvellTokenSpaceGuid.PcdProductPlatformName|"CN9132 DB-A"
  !endif
  gMarvellTokenSpaceGuid.PcdProductVersion|"Rev. 1.1"
