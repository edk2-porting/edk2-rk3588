## @file
#  PCD configuration build description file for the Aspire VN7-572G board.
#
# Copyright (c) 2017 - 2020, Intel Corporation. All rights reserved.<BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

################################################################################
#
# Pcd Section - list of all PCD Entries used by this board.
#
################################################################################

# TODO: Harden and tune platform by PCDs
# TODO: Consider removing PCDs declared by build report to be unused (but confirm first)
# - Also, consider more "fixed" and more "dynamic"/"patchable"

[PcdsFixedAtBuild.common]
  ######################################
  # Key Boot Stage and FSP configuration
  ######################################
  #
  # Please select the Boot Stage here.
  # Stage 1 - enable debug (system deadloop after debug init)
  # Stage 2 - mem init (system deadloop after mem init)
  # Stage 3 - boot to shell only
  # Stage 4 - boot to OS
  # Stage 5 - boot to OS with security boot enabled
  # Stage 6 - boot with advanced features enabled
  #
  gMinPlatformPkgTokenSpaceGuid.PcdBootStage|6

  #
  # 0: FSP Wrapper is running in Dispatch mode.
  # 1: FSP Wrapper is running in API mode.
  #
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection|0

  #
  # FALSE: The board is not a FSP wrapper (FSP binary not used)
  # TRUE:  The board is a FSP wrapper (FSP binary is used)
  #
  gMinPlatformPkgTokenSpaceGuid.PcdFspWrapperBootMode|TRUE

  #
  # FALSE: The PEI Main included in FvPreMemory is used to dispatch all PEIMs
  #        (both inside FSP and outside FSP).
  #        Pros:
  #          * PEI Main is re-built from source and is always the latest version
  #          * Platform code can link any desired LibraryClass to PEI Main
  #            (Ex: Custom DebugLib instance, SerialPortLib, etc.)
  #        Cons:
  #          * The PEI Main being used to execute FSP PEIMs is not the PEI Main
  #            that the FSP PEIMs were tested with, adding risk of breakage.
  #          * Two copies of PEI Main will exist in the final binary,
  #            #1 in FSP-M, #2 in FvPreMemory. The copy in FSP-M is never
  #            executed, wasting space.
  #
  # <b>TRUE</b>:  The PEI Main included in FSP is used to dispatch all PEIMs
  #        (both inside FSP and outside FSP). PEI Main will not be included in
  #        FvPreMemory. This is the default and is the recommended choice.
  #
  gMinPlatformPkgTokenSpaceGuid.PcdFspDispatchModeUseFspPeiMain|TRUE

  #
  # FSP Base address PCD will be updated in FDF basing on flash map.
  #
  gIntelFsp2WrapperTokenSpaceGuid.PcdFsptBaseAddress|0
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspmBaseAddress|0

  gIntelFsp2PkgTokenSpaceGuid.PcdTemporaryRamBase|0xFEF00000
  gIntelFsp2PkgTokenSpaceGuid.PcdTemporaryRamSize|0x00040000
  gSiPkgTokenSpaceGuid.PcdTemporaryRamBase|0xFEF80000
  gSiPkgTokenSpaceGuid.PcdTemporaryRamSize|0x00040000
  gSiPkgTokenSpaceGuid.PcdTsegSize|0x0800000  # Now hooked up

!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  #
  # FSP API mode does not share stack with the boot loader,
  # so FSP needs more temporary memory for FSP heap + stack size.
  #
  gIntelFsp2PkgTokenSpaceGuid.PcdFspTemporaryRamSize|0x28000  # FIXME: Confirm matches UPD default
  #
  # FSP API mode does not need to enlarge the boot loader stack size
  # since the stacks are separate.
  #
  gSiPkgTokenSpaceGuid.PcdPeiTemporaryRamStackSize|0x20000  # Not hooked up, not used (functionally equivalent and equal to UefiCpuPkg)
!else
  #
  # In FSP Dispatch mode boot loader stack size must be large
  # enough for executing both boot loader and FSP.
  #
  gSiPkgTokenSpaceGuid.PcdPeiTemporaryRamStackSize|0x40000 # Not hooked up, not used (functionally equivalent but NOT equal to UefiCpuPkg)
!endif

!if (gMinPlatformPkgTokenSpaceGuid.PcdFspWrapperBootMode == FALSE) || (gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1)
  gSiPkgTokenSpaceGuid.PcdSiPciExpressBaseAddress|gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress
  gSiPkgTokenSpaceGuid.PcdSiPciExpressRegionLength|gMinPlatformPkgTokenSpaceGuid.PcdPciExpressRegionLength
!else
  #
  # FSP Dispatch mode requires more platform memory as boot loader and FSP sharing the same
  # platform memory.
  #
  gSiPkgTokenSpaceGuid.PcdPeiMinMemorySize|0x5500000
!endif

[PcdsFeatureFlag.common]
  ######################################
  # Edk2 Configuration
  ######################################
  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreImageLoaderSearchTeSectionFirst|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdInstallAcpiSdtProtocol|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutUgaSupport|FALSE  # Deprecated, only use GOP
  gEfiMdePkgTokenSpaceGuid.PcdUgaConsumeSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdPs2KbdExtendedVerification|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdPs2MouseExtendedVerification|FALSE  # TODO/TEST
  gEfiMdeModulePkgTokenSpaceGuid.PcdHiiOsRuntimeSupport|FALSE
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmEnableBspElection|FALSE
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmProfileEnable|FALSE

# TODO: Prune this list to relevant features only
!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 6
  # FIXME: SMM path also PatchAndLoadAcpiTable()
  gAcpiDebugFeaturePkgTokenSpaceGuid.PcdAcpiDebugFeatureEnable            |FALSE
  # PcdIpmiFeatureEnable will not be enabled (no BMC)
  # TODO: Can be build-time (user) choice
  gNetworkFeaturePkgTokenSpaceGuid.PcdNetworkFeatureEnable                |FALSE
  # TODO: Continue developing support. Broken at present.
  # - PeiSmmAccessLib in IntelSiliconPkg seems like a stub
  #   - May require a PeiSmmControlLib to SMM communicate
  gS3FeaturePkgTokenSpaceGuid.PcdS3FeatureEnable                          |FALSE
  # TODO: Definitions (now added SmbiosDxe)
  gSmbiosFeaturePkgTokenSpaceGuid.PcdSmbiosFeatureEnable                  |TRUE
  # Requires actual hook-up
  gUsb3DebugFeaturePkgTokenSpaceGuid.PcdUsb3DebugFeatureEnable            |FALSE
  # FIXME: (Similar) DXE module is duplicate?
  gUserAuthFeaturePkgTokenSpaceGuid.PcdUserAuthenticationFeatureEnable    |FALSE
  # FIXME: Must BootLogoEnableLogo() to turn platform logo into boot logo
  # - BGRT must be BMP, but this duplicates FSP logo. Can GetSectionFromAnyFv()?
  gLogoFeaturePkgTokenSpaceGuid.PcdLogoFeatureEnable                      |FALSE
  gLogoFeaturePkgTokenSpaceGuid.PcdJpgEnable                              |FALSE
!endif

  ######################################
  # Silicon Configuration
  ######################################
  # TODO: Set FSP policy by switches? Otherwise, only FSP binary builds?
  # Build switches
  gSiPkgTokenSpaceGuid.PcdOptimizeCompilerEnable|TRUE

  # CPU
  gSiPkgTokenSpaceGuid.PcdBiosGuardEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSourceDebugEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdTxtEnable|FALSE

  # SA
  gSiPkgTokenSpaceGuid.PcdIgdEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdPegEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSgEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSaDmiEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSkycamEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdGmmEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSaOcEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdVtdEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdPeiDisplayEnable|TRUE

  # ME
  gSiPkgTokenSpaceGuid.PcdAmtEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdAtaEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdPttEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdJhiEnable|TRUE

  # Others
  gSiPkgTokenSpaceGuid.PcdAcpiEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdBdatEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdBootGuardEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdCpuPowerOnConfigEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdEvLoaderEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdIntegratedTouchEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdOcWdtEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdOverclockEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdPpmEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdS3Enable|TRUE
  gSiPkgTokenSpaceGuid.PcdSerialGpioEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable|$(RELEASE_LOGGING)
  gSiPkgTokenSpaceGuid.PcdSiCsmEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdSmbiosEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSmmVariableEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSoftwareGuardEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSsaFlagEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdTraceHubEnable|TRUE

  ######################################
  # Platform Configuration
  ######################################
  gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterDebugInit|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterMemInit|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable|FALSE  # FIXME: Define by PERFORMANCE_BUILD?
  gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdSerialTerminalEnable|FALSE  # FIXME: Define in build-system?

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 1
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterDebugInit|TRUE
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 2
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterDebugInit|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterMemInit|TRUE
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 3
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterMemInit|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly|TRUE
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 4
  gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly|FALSE
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 5
  gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable|TRUE
  gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable|TRUE
!endif

# TODO: Is TESTING setting, is not test point
!if $(TARGET) == DEBUG
  gMinPlatformPkgTokenSpaceGuid.PcdSmiHandlerProfileEnable|TRUE
!else
  gMinPlatformPkgTokenSpaceGuid.PcdSmiHandlerProfileEnable|FALSE
!endif

  ######################################
  # Board Configuration
  ######################################
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdMultiBoardSupport|FALSE
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdTbtEnable|FALSE  # TODO: Enable if supporting Newgate

[PcdsFixedAtBuild.common]
  ######################################
  # Edk2 Configuration
  ######################################
!if $(TARGET) == RELEASE
!if $(RELEASE_LOGGING) == TRUE
!if $(TESTING) == TRUE
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x07
!else
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x03
!endif # $(TESTING)
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07
!else
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x0
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x3
!endif # $(RELEASE_LOGGING)
!else
  # FIXME: More than just compiler optimisation is hooked to DEBUG builds.
  #        Make asserts non-fatal for limited debugging system
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x0F
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07
!endif # $(TARGET)
!if gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable == TRUE
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0x1
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxPeiPerformanceLogEntries|140
!endif
  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress|0xE0000000

  gEfiMdeModulePkgTokenSpaceGuid.PcdAriSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdBrowserFieldTextColor|0x01
  gEfiMdeModulePkgTokenSpaceGuid.PcdBrowserSubtitleTextColor|0x0
  gEfiMdeModulePkgTokenSpaceGuid.PcdCpuStackGuard|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdFastPS2Detection|TRUE  # TODO/TEST
  gEfiMdeModulePkgTokenSpaceGuid.PcdHwErrStorageSize|0x00000800
  gEfiMdeModulePkgTokenSpaceGuid.PcdLoadModuleAtFixAddressEnable|$(TOP_MEMORY_ADDRESS)
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxHardwareErrorVariableSize|0x400
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVariableSize|0x8000
!if $(TESTING) == TRUE
  # Test with non-stop mode, so not disabling for loader.
  gEfiMdeModulePkgTokenSpaceGuid.PcdNullPointerDetectionPropertyMask|0x43
!else
  # FIXME: Can be broken for CSM. At this time, be permissive for loader.
  gEfiMdeModulePkgTokenSpaceGuid.PcdNullPointerDetectionPropertyMask|0x83
!endif
  gEfiMdeModulePkgTokenSpaceGuid.PcdReclaimVariableSpaceAtEndOfDxe|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetNxForStack|TRUE
!if gMinPlatformPkgTokenSpaceGuid.PcdSmiHandlerProfileEnable == TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSmiHandlerProfilePropertyMask|0x1
!endif
  gEfiMdeModulePkgTokenSpaceGuid.PcdSrIovSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseMemory|FALSE
!if $(TARGET) == RELEASE
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|FALSE
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialUseHardwareFlowControl|FALSE
!endif

  # UPDs are updated at runtime, don't bother measuring
  # BUGBUG: FSP-S measurement returns DEVICE_ERROR from PtpCrbTpmCommand() - Step 0.
  # - Similarly, Tcg2Dxe.c:Tpm2GetCapabilityManufactureID() - first command - fails?
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspMeasurementConfig|0x00000006

  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoBarEnableMask|0x80
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciBarRegisterOffset|0x40
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciBusNumber|0x0
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciDeviceNumber|0x1F
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciEnableRegisterOffset|0x44
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciFunctionNumber|0x2
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPortBaseAddress|0x1800
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPortBaseAddressMask|0xFFFC
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiPm1TmrOffset|0x08

  # Specifies timeout value in microseconds for the BSP to detect all APs for the first time.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuApInitTimeOutInMicroSeconds|1000
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmApSyncTimeout|10000
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmStackSize|0x20000
!if (gMinPlatformPkgTokenSpaceGuid.PcdFspWrapperBootMode == FALSE) || (gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1)
  #
  # In non-FSP build (EDK2 build) or FSP API mode below PCD are FixedAtBuild
  # (They will be DynamicEx in FSP Dispatch mode)
  #
  ## Specifies max supported number of Logical Processors.
  # @Prompt Configure max supported number of Logical Processors
  gUefiCpuPkgTokenSpaceGuid.PcdCpuMaxLogicalProcessorNumber|8

  ## Specifies the size of the microcode Region.
  # @Prompt Microcode Region size.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuMicrocodePatchRegionSize|0

  ## Specifies the AP wait loop state during POST phase.
  #  The value is defined as below.
  #  1: Place AP in the Hlt-Loop state.
  #  2: Place AP in the Mwait-Loop state.
  #  3: Place AP in the Run-Loop state.
  # @Prompt The AP wait loop state.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuApLoopMode|2
!endif

  ######################################
  # Silicon Configuration
  ######################################

  # Refer to HstiFeatureBit.h for bit definitions
  gSiPkgTokenSpaceGuid.PcdHstiIhvFeature1|0xF2  # FIXME: Boot Guard and BIOS Guard not present, measured boot enforcement checking code not present
  gSiPkgTokenSpaceGuid.PcdHstiIhvFeature2|0x07

  ######################################
  # Platform Configuration
  ######################################
  gMinPlatformPkgTokenSpaceGuid.PcdMaxCpuSocketCount|1
  gMinPlatformPkgTokenSpaceGuid.PcdMaxCpuCoreCount|4
  gMinPlatformPkgTokenSpaceGuid.PcdMaxCpuThreadCount|2
  gMinPlatformPkgTokenSpaceGuid.PcdPciExpressRegionLength|0x10000000

  #
  # The PCDs are used to control the Windows SMM Security Mitigations Table - Protection Flags
  #
  # BIT0: If set, expresses that for all synchronous SMM entries,SMM will validate that input and output buffers lie entirely within the expected fixed memory regions.
  # BIT1: If set, expresses that for all synchronous SMM entries, SMM will validate that input and output pointers embedded within the fixed communication buffer only refer to address ranges \
  #       that lie entirely within the expected fixed memory regions.
  # BIT2: Firmware setting this bit is an indication that it will not allow reconfiguration of system resources via non-architectural mechanisms.
  # BIT3-31: Reserved
  #
  gMinPlatformPkgTokenSpaceGuid.PcdWsmtProtectionFlags|0x07

  ## This PCD is to control which device is the potential trusted console input device.<BR><BR>
  # For example:<BR>
  # PS/2 keyboard: PciRoot(0x0)/Pci(0x1F,0x0)/Acpi(PNP0303,0x0)<BR>
  #   //Header                    HID                     UID<BR>
  #     {0x02, 0x01, 0x0C, 0x00,  0xd0, 0x41, 0x03, 0x0A, 0x00, 0x00, 0x00, 0x00,<BR>
  #   //Header                    Func  Dev<BR>
  #      0x01, 0x01, 0x06, 0x00,  0x00, 0x1F,<BR>
  #   //Header                    HID                     UID<BR>
  #      0x02, 0x01, 0x0C, 0x00,  0xd0, 0x41, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00,<BR>
  #   //Header<BR>
  #      0x7F, 0xFF, 0x04, 0x00}<BR>
  gMinPlatformPkgTokenSpaceGuid.PcdTrustedConsoleInputDevicePath|{0x02, 0x01, 0x0C, 0x00,  0xd0, 0x41, 0x03, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x06, 0x00,  0x00, 0x1F, 0x02, 0x01, 0x0C, 0x00,  0xd0, 0x41, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0x04, 0x00}

!if $(TARGET) == RELEASE
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiReservedMemorySize|0x800
!else
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiReservedMemorySize|0x188B  # TODO
!endif
  # TODO: Consider using reserved space instead for debug log
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiRtDataMemorySize|0x200
!if $(TARGET) == RELEASE
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiRtCodeMemorySize|0x70
!else
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiRtCodeMemorySize|0xE0
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage == 1
  gMinPlatformPkgTokenSpaceGuid.PcdTestPointIbvPlatformFeature|{0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage == 2
  gMinPlatformPkgTokenSpaceGuid.PcdTestPointIbvPlatformFeature|{0x03, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage == 3
  gMinPlatformPkgTokenSpaceGuid.PcdTestPointIbvPlatformFeature|{0x03, 0x07, 0x03, 0x05, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage == 4
  gMinPlatformPkgTokenSpaceGuid.PcdTestPointIbvPlatformFeature|{0x03, 0x07, 0x03, 0x05, 0x3F, 0x00, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage == 5
  gMinPlatformPkgTokenSpaceGuid.PcdTestPointIbvPlatformFeature|{0x03, 0x0F, 0x07, 0x1F, 0x3F, 0x0F, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 6
  gMinPlatformPkgTokenSpaceGuid.PcdTestPointIbvPlatformFeature|{0x03, 0x0F, 0x07, 0x1F, 0x3F, 0x0F, 0x0F, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
!endif

  ######################################
  # Board Configuration
  ######################################
  gBoardModulePkgTokenSpaceGuid.PcdPs2KbMsEnable|1
  gBoardModulePkgTokenSpaceGuid.PcdSuperIoPciIsaBridgeDevice|{0x00, 0x00, 0x1F, 0x00}

[PcdsFixedAtBuild.IA32]
  ######################################
  # Edk2 Configuration
  ######################################
  gIntelFsp2PkgTokenSpaceGuid.PcdGlobalDataPointerAddress|0xFED00148
  gIntelFsp2WrapperTokenSpaceGuid.PcdPeiMinMemSize|0x3800000

  ######################################
  # Platform Configuration
  ######################################
  gMinPlatformPkgTokenSpaceGuid.PcdPeiPhaseStackTop|0xA0000

[PcdsFixedAtBuild.X64]
  ######################################
  # Edk2 Configuration
  ######################################

  # Default platform supported RFC 4646 languages: (American) English
  gEfiMdePkgTokenSpaceGuid.PcdUefiVariableDefaultPlatformLangCodes|"en-US"

[PcdsPatchableInModule.common]
  ######################################
  # Edk2 Configuration
  ######################################
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000046  # 0x804800C7/0x806A15CF give useful information, but is very noisy

  ######################################
  # Silicon Configuration
  ######################################
!if $(TARGET) == DEBUG
  gSiPkgTokenSpaceGuid.PcdSerialIoUartDebugEnable|1
!endif

  ######################################
  # Platform Configuration
  ######################################
!if $(TARGET) == DEBUG
  gMinPlatformPkgTokenSpaceGuid.PcdSecSerialPortDebugEnable|1
!else
  gMinPlatformPkgTokenSpaceGuid.PcdSecSerialPortDebugEnable|0
!endif

[PcdsDynamicDefault]
  ######################################
  # Edk2 Configuration
  ######################################
  gEfiMdeModulePkgTokenSpaceGuid.PcdAtaSmartEnable|TRUE  # Why dynamic?
  gEfiMdeModulePkgTokenSpaceGuid.PcdConInConnectOnDemand|FALSE  # Why dynamic?
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|0x0
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|0x0
  gEfiMdeModulePkgTokenSpaceGuid.PcdS3BootScriptTablePrivateDataPtr|0x0

  #
  #  Set video to native resolution as Windows 8 WHCK requirement.
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|0x0
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|0x0

  gEfiSecurityPkgTokenSpaceGuid.PcdTcg2HashAlgorithmBitmap|0
  gEfiSecurityPkgTokenSpaceGuid.PcdTpm2HashMask|0x0000001F
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmInitializationPolicy|1
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmInstanceGuid|{0x5a, 0xf2, 0x6b, 0x28, 0xc3, 0xc2, 0x8c, 0x40, 0xb3, 0xb4, 0x25, 0xe6, 0x75, 0x8b, 0x73, 0x17}

  #
  # FSP Base address PCD will be updated in FDF basing on flash map.
  #
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspsBaseAddress|0
  # Platform will pre-allocate UPD buffer and pass it to FspWrapper
  # Those dummy address will be patched before FspWrapper executing
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspmUpdDataAddress|0xFFFFFFFF
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspsUpdDataAddress|0xFFFFFFFF

  ######################################
  # Board Configuration
  ######################################
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDisablePassiveTripPoints|1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdLowPowerS0Idle|1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdPciExpNative|1

  # Thunderbolt Configuration (FIXME: Remove if not supporting Newgate)
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtAcDcSwitch|0x0
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtAcpiGpeSignature|0
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtAcpiGpeSignaturePorting|0
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtAspm|0x0
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtCioPlugEventGpioPad|0x02010011
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtControllerEn|0x1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtControllerType|0x1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtForcepowerGpioPad|13
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtGpioAccessType|0x2
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtGpioLevel|0x1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtHotNotify|0x1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtHotSMI|0x1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtPcieExtraBusRsvd|56
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtPcieMemAddrRngMax|26
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtPcieMemRsvd|100
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtPciePMemAddrRngMax|28
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtPciePMemRsvd|100
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtPcieRpNumber|0x1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtSecurityMode|0x1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtSetClkReq|0x1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtWakeupSupport|0x0
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtWin10Support|0x0
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdExpander|0x0
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdPchPcieRootPortHpe|0x00000001
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdRtd3Tbt|0x1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdRtd3TbtClkReq|0x1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdRtd3TbtClkReqDelay|0x0
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdRtd3TbtOffDelay|5000

[PcdsDynamicHii.X64.DEFAULT]
  ######################################
  # Edk2 Configuration
  ######################################
  gEfiMdePkgTokenSpaceGuid.PcdHardwareErrorRecordLevel|L"HwErrRecSupport"|gEfiGlobalVariableGuid|0x0|1 # Variable: L"HwErrRecSupport"
!if gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable == TRUE
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|L"Timeout"|gEfiGlobalVariableGuid|0x0|1 # Variable: L"Timeout"
!else
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|L"Timeout"|gEfiGlobalVariableGuid|0x0|5 # Variable: L"Timeout"
!endif
!if gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable == TRUE
  gEfiSecurityPkgTokenSpaceGuid.PcdTcgPhysicalPresenceInterfaceVer|L"TCG2_VERSION"|gTcg2ConfigFormSetGuid|0x0|"1.3"|NV,BS
  gEfiSecurityPkgTokenSpaceGuid.PcdTpm2AcpiTableRev|L"TCG2_VERSION"|gTcg2ConfigFormSetGuid|0x8|3|NV,BS
!endif
