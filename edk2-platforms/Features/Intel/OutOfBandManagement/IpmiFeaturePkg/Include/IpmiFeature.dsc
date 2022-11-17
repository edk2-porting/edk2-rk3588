## @file
# This is a build description file for the Intelligent Platform Management Interface (IPMI) advanced feature.
# This file should be included into another package DSC file to build this feature.
#
# The DEC files are used by the utilities that parse DSC and
# INF files to generate AutoGen.c and AutoGen.h files
# for the build infrastructure.
#
# Copyright (c) 2019 - 2021, Intel Corporation. All rights reserved.<BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
!ifndef $(PEI_ARCH)
  !error "PEI_ARCH must be specified to build this feature!"
!endif
!ifndef $(DXE_ARCH)
  !error "DXE_ARCH must be specified to build this feature!"
!endif

################################################################################
#
# Library Class section - list of all Library Classes needed by this feature.
#
################################################################################

!include MdePkg/MdeLibs.dsc.inc

[LibraryClasses]
  #######################################
  # Edk2 Packages
  #######################################
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLibRepStr/BaseMemoryLibRepStr.inf
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  IpmiLib|MdeModulePkg/Library/BaseIpmiLibNull/BaseIpmiLibNull.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  TimerLib|MdePkg/Library/BaseTimerLibNullTemplate/BaseTimerLibNullTemplate.inf

  #####################################
  # IPMI Feature Package
  #####################################
  IpmiCommandLib|OutOfBandManagement/IpmiFeaturePkg/Library/IpmiCommandLib/IpmiCommandLib.inf
  IpmiPlatformHookLib|OutOfBandManagement/IpmiFeaturePkg/Library/IpmiPlatformHookLibNull/IpmiPlatformHookLibNull.inf

[LibraryClasses.common.PEI_CORE,LibraryClasses.common.PEIM]
  #######################################
  # Edk2 Packages
  #######################################
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  PeimEntryPoint|MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLibIdt/PeiServicesTablePointerLibIdt.inf
  ReportStatusCodeLib|MdeModulePkg/Library/PeiReportStatusCodeLib/PeiReportStatusCodeLib.inf

  #####################################
  # IPMI Feature Package
  #####################################
  IpmiBaseLib|OutOfBandManagement/IpmiFeaturePkg/Library/PeiIpmiBaseLib/PeiIpmiBaseLib.inf

[LibraryClasses.common.DXE_DRIVER,LibraryClasses.common.UEFI_DRIVER]
  #######################################
  # Edk2 Packages
  #######################################
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf

  #####################################
  # IPMI Feature Package
  #####################################
  IpmiBaseLib|OutOfBandManagement/IpmiFeaturePkg/Library/IpmiBaseLib/IpmiBaseLib.inf

################################################################################
#
# Component section - list of all components that need built for this feature.
#
# Note: The EDK II DSC file is not used to specify how compiled binary images get placed
#       into firmware volume images. This section is just a list of modules to compile from
#       source into UEFI-compliant binaries.
#       It is the FDF file that contains information on combining binary files into firmware
#       volume images, whose concept is beyond UEFI and is described in PI specification.
#       There may also be modules listed in this section that are not required in the FDF file,
#       When a module listed here is excluded from FDF file, then UEFI-compliant binary will be
#       generated for it, but the binary will not be put into any firmware volume.
#
################################################################################
#
# Feature PEI Components
#

# @todo: Change below line to [Components.$(PEI_ARCH)] after https://bugzilla.tianocore.org/show_bug.cgi?id=2308
#        is completed.
[Components.IA32]
  #####################################
  # IPMI Feature Package
  #####################################

  # Add library instances here that are not included in package components and should be tested
  # in the package build.

  OutOfBandManagement/IpmiFeaturePkg/Library/IpmiPlatformHookLibNull/IpmiPlatformHookLibNull.inf

  #
  # Add components here that should be included in the package build.
  #
  OutOfBandManagement/IpmiFeaturePkg/GenericIpmi/Pei/PeiGenericIpmi.inf
  OutOfBandManagement/IpmiFeaturePkg/Frb/FrbPei.inf
  OutOfBandManagement/IpmiFeaturePkg/IpmiInit/PeiIpmiInit.inf

#
# Feature DXE Components
#

# @todo: Change below line to [Components.$(DXE_ARCH)] after https://bugzilla.tianocore.org/show_bug.cgi?id=2308
#        is completed.
[Components.X64]
  #####################################
  # IPMI Feature Package
  #####################################

  # Add library instances here that are not included in package components and should be tested
  # in the package build.

  OutOfBandManagement/IpmiFeaturePkg/Library/IpmiPlatformHookLibNull/IpmiPlatformHookLibNull.inf

  #
  # Add components here that should be included in the package build.
  #
  OutOfBandManagement/IpmiFeaturePkg/GenericIpmi/Dxe/GenericIpmi.inf
  OutOfBandManagement/IpmiFeaturePkg/Library/SmmIpmiBaseLib/SmmIpmiBaseLib.inf
  OutOfBandManagement/IpmiFeaturePkg/BmcAcpi/BmcAcpi.inf
  OutOfBandManagement/IpmiFeaturePkg/BmcElog/BmcElog.inf
  OutOfBandManagement/IpmiFeaturePkg/Frb/FrbDxe.inf
  OutOfBandManagement/IpmiFeaturePkg/IpmiFru/IpmiFru.inf
  OutOfBandManagement/IpmiFeaturePkg/IpmiInit/DxeIpmiInit.inf
  OutOfBandManagement/IpmiFeaturePkg/OsWdt/OsWdt.inf
  OutOfBandManagement/IpmiFeaturePkg/SolStatus/SolStatus.inf

###################################################################################################
#
# BuildOptions Section - Define the module specific tool chain flags that should be used as
#                        the default flags for a module. These flags are appended to any
#                        standard flags that are defined by the build process. They can be
#                        applied for any modules or only those modules with the specific
#                        module style (EDK or EDKII) specified in [Components] section.
#
#                        For advanced features, it is recommended to enable [BuildOptions] in
#                        the applicable INF file so it does not affect the whole board package
#                        build when this DSC file is active.
#
###################################################################################################
[BuildOptions]
