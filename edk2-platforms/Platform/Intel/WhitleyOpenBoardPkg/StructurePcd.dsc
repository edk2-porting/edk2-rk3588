## @file
# @copyright
# Copyright 2019 - 2021 Intel Corporation. <BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
##

[Defines]
  VPD_TOOL_GUID                  = 8C3D856A-9BE6-468E-850A-24F7A8D38E08

[SkuIds]
  0|DEFAULT              # The entry: 0|DEFAULT is reserved and always required.

[DefaultStores]
  0|STANDARD             # UEFI Standard default  0|STANDARD is reserved.
  1|MANUFACTURING        # UEFI Manufacturing default 1|MANUFACTURING is reserved.

[PcdsDynamicExVpd.common.DEFAULT]
  gEfiMdeModulePkgTokenSpaceGuid.PcdNvStoreDefaultValueBuffer|*

[PcdsDynamicExHii.common.DEFAULT.STANDARD]
gStructPcdTokenSpaceGuid.PcdFpgaSocketConfig|L"FpgaSocketConfig"|gFpgaSocketVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdPchSetup|L"PchSetup"|gPchSetupVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdSetup|L"Setup"|gEfiSetupVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig|L"SocketCommonRcConfig"|gEfiSocketCommonRcVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdSocketIioConfig|L"SocketIioConfig"|gEfiSocketIioVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig|L"SocketMemoryConfig"|gEfiSocketMemoryVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig|L"SocketMpLinkConfig"|gEfiSocketMpLinkVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig|L"SocketPowerManagementConfig"|gEfiSocketPowermanagementVarGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig|L"SocketProcessorCoreConfig"|gEfiSocketProcessorCoreVarGuid|0x00||NV, BS, RT

!if $(TARGET) == "DEBUG"
gStructPcdTokenSpaceGuid.PcdSetup.serialDebugMsgLvl|0x4
gStructPcdTokenSpaceGuid.PcdSetup.serialDebugMsgLvlTrainResults|0x8
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.DebugPrintLevel|0xF
!else
gStructPcdTokenSpaceGuid.PcdSetup.serialDebugMsgLvl|0x0
gStructPcdTokenSpaceGuid.PcdSetup.serialDebugMsgLvlTrainResults|0x0
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.DebugPrintLevel|0x0
!endif

gStructPcdTokenSpaceGuid.PcdEmulationDfxConfig.DfxHybridSystemLevelEmulation|0x0                   # Hybrid SLE Mode
gStructPcdTokenSpaceGuid.PcdEmulationDfxConfig.DfxPmMsrTrace|0x0                                   # MSR Trace for PM
gStructPcdTokenSpaceGuid.PcdEmulationDfxConfig.DfxUbiosGeneration|0x0                              # uBIOS Generation
gStructPcdTokenSpaceGuid.PcdFpgaSocketConfig.FpgaSetupEnabled|0x3                                  # Sockets Enable Bitmap(Hex)
gStructPcdTokenSpaceGuid.PcdFpgaSocketConfig.FpgaSocketGuid[0]|0x0                                 # Socket 0 BitStream
gStructPcdTokenSpaceGuid.PcdFpgaSocketConfig.FpgaSocketGuid[1]|0x0                                 # Socket 1 BitStream
gStructPcdTokenSpaceGuid.PcdFpgaSocketConfig.FpgaThermalTH1[0]|0x5a                                # Socket 0 Threshold 1
gStructPcdTokenSpaceGuid.PcdFpgaSocketConfig.FpgaThermalTH1[1]|0x5a                                # Socket 1 Threshold 1
gStructPcdTokenSpaceGuid.PcdFpgaSocketConfig.FpgaThermalTH2[0]|0x5d                                # Socket 0 Threshold 2
gStructPcdTokenSpaceGuid.PcdFpgaSocketConfig.FpgaThermalTH2[1]|0x5d                                # Socket 1 Threshold 2
gStructPcdTokenSpaceGuid.PcdIeRcConfiguration.IeDidEnabled|0x1                                     # DRAM Init Done Enable
gStructPcdTokenSpaceGuid.PcdIeRcConfiguration.IeHeci1Enabled|0x0                                   # HECI-1 Enable
gStructPcdTokenSpaceGuid.PcdIeRcConfiguration.IeHeci2Enabled|0x0                                   # HECI-2 Enable
gStructPcdTokenSpaceGuid.PcdIeRcConfiguration.IeHeci3Enabled|0x0                                   # HECI-3 Enable
gStructPcdTokenSpaceGuid.PcdIeRcConfiguration.IeIderEnabled|0x0                                    # IDER Enable
gStructPcdTokenSpaceGuid.PcdIeRcConfiguration.IeKtEnabled|0x0                                      # KT Enable
gStructPcdTokenSpaceGuid.PcdIeRcConfiguration.SubsystemId|0x7270                                   # Subsystem ID
gStructPcdTokenSpaceGuid.PcdIeRcConfiguration.SubsystemVendorId|0x8086                             # Subsystem Vendor ID
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.AmtCiraRequest|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.AmtCiraTimeout|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.AmtbxHotKeyPressed|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.AmtbxSelectionScreen|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.Amt|0x1
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.Asf|0x1
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.BiosPause|0x1                                        # BIOS Pause Before Booting Capability State
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.BiosReflash|0x1                                      # BIOS Reflash Capability State
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.BiosSetup|0x1                                        # BIOS Boot to Setup Capability State
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.BreakRtcEnabled|0x0                                  # Break RTC Configuration
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.CLINKDisableOverride|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.CoreBiosDoneEnabled|0x1                              # Core Bios Done Message
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.CpuTypeEmulation|0xa
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.DelayedAuthenticationModeOverride|0x0                # Delayed Authentication Mode (DAM) Override
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.DelayedAuthenticationMode|0x0                        # Delayed Authentication Mode (DAM)
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.DidInitStat|0x0                                      # DRAM Initialization Status
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.DisableD0I3SettingForHeci|0x0                        # D0I3 Setting for HECI Disable
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.EndOfPostMessage|0x2                                 # END_OF_POST Message
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.HeciCommunication2|0x2                               # HECI-2 Enable
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.HeciCommunication3|0x2                               # HECI-3 Enable
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.HeciCommunication|0x2                                # HECI-1 Enable
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.FWProgress|0x1
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.Frequency[0]|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.Frequency[1]|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.HideUnConfigureMeConfirm|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.HostResetNotification|0x1                            # Host Reset Warning
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.HsioMessagingEnabled|0x0                             # Enable HSIO Messaging
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.IccClkOverride|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.IccProfile|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.KtDeviceEnable|0x2                                   # KT Enable
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.LocalFwUpdEnabled|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MdesCapability|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MeEnableHeciTraces|0x0                               # Enable HECI Dump
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MeFirmwareMode|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MeFwDowngrade|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MeGrLockEnabled|0x1                                  # Global Reset Lock
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MeGrPromotionEnabled|0x0                             # CF9 global reset promotion
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MeHeci1HideInMe|0x0                                  # HECI-1 Hide in ME
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MeHeci2HideInMe|0x0                                  # HECI-2 Hide in ME
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MeHeci3HideInMe|0x0                                  # HECI-3 Hide in ME
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MeHmrfpoEnableEnabled|0x0                            # HMRFPO_ENABLE Message
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MeHmrfpoLockEnabled|0x1                              # HMRFPO_LOCK Message
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MeIderEnabled|0x2                                    # IDEr Enable
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MeImageType|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MePttEnabled|0x0                                     # PTT Support
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MePttSupported|0x0                                   # ME PTT Supported
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MePttSuppressCommandSend|0x0                         # Suppress PTT Commands
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MeRegionSelectEnabled|0x0                            # REGION_SELECT Message
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MeStateControl|0x1
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MeTimeout|0x2                                        # ME Initialization Complete Timeout
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MeType|0xee                                          # ME Firmware Type
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MeWatchdogControlEnabled|0x1                         # WATCHDOG_CONTROL Message
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MebxDebugMsg|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MebxGraphicsMode|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MebxNonUiTextMode|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MebxUiTextMode|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.MngState|0x1
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.NmCores2DisableOverride|0x0                          # Cores Disable Override
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.NmCores2Disable|0x0                                  # Cores To Disable
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.NmHwChangeOverride|0x0                               # Hardware Change Override
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.NmHwChangeStatus|0x0                                 # Hardware Changed
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.NmPowerMsmtOverride|0x0                              # Power Measurement Override
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.NmPowerMsmtSupport|0x0                               # Power Measurement
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.NmPtuLoadOverride|0x0                                # PTU Load Override
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.NmPwrOptBootOverride|0x0                             # Boot Mode Override
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.NmPwrOptBoot|0x0                                     # Boot Mode
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.PreDidMeResetEnabled|0x0                             # Pre-DramInitDone ME Reset
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.RegLock|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.RemoteSessionActive|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.SecureBootExposureToFw|0x1                           # BIOS Secure Boot Capability Exposure to FW State
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.Spread[0]|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.Spread[1]|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.SendDidMsg|0x1                                       # DRAM Init Done Enable
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.SpsAltitude|0x8000                                   # Altitude
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.SpsMctpBusOwner|0x0                                  # MCTP Bus Owner
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.SpsPsuAddress[0]|0x58                                # PSU #1
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.SpsPsuAddress[1]|0x59                                # PSU #2
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.SpsPsuAddress[2]|0x0                                 # PSU #3
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.SpsPsuAddress[3]|0x0                                 # PSU #4
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.USBProvision|0x1
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.UnConfigureMe|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.VproAllowed|0x1
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.WatchDogOs|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.WatchDogTimerBios|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.WatchDogTimerOs|0x0
gStructPcdTokenSpaceGuid.PcdMeRcConfiguration.WatchDog|0x0
gStructPcdTokenSpaceGuid.PcdMemBootHealthConfig.CriticalRxDqsDelayLeftEdge|0x2                     # RxDqsDelay Left Edge
gStructPcdTokenSpaceGuid.PcdMemBootHealthConfig.CriticalRxDqsDelayRightEdge|0x2                    # RxDqsDelay Right Edge
gStructPcdTokenSpaceGuid.PcdMemBootHealthConfig.CriticalRxVrefLeftEdge|0x2                         # RxVref Left Edge
gStructPcdTokenSpaceGuid.PcdMemBootHealthConfig.CriticalRxVrefRightEdge|0x2                        # RxVref Right Edge
gStructPcdTokenSpaceGuid.PcdMemBootHealthConfig.CriticalTxDqDelayLeftEdge|0x2                      # TxDqDelay Left Edge
gStructPcdTokenSpaceGuid.PcdMemBootHealthConfig.CriticalTxDqDelayRightEdge|0x2                     # TxDqDelay Right Edge
gStructPcdTokenSpaceGuid.PcdMemBootHealthConfig.CriticalTxVrefLeftEdge|0x2                         # TxVref Left Edge
gStructPcdTokenSpaceGuid.PcdMemBootHealthConfig.CriticalTxVrefRightEdge|0x2                        # TxVref Right Edge
gStructPcdTokenSpaceGuid.PcdMemBootHealthConfig.MemBootHealthCheck|0x2                             # Memory Boot Health Check
gStructPcdTokenSpaceGuid.PcdMemBootHealthConfig.ResetOnCriticalError|0x1                           # Reboot On Critical Failure
gStructPcdTokenSpaceGuid.PcdMemBootHealthConfig.WarningRxDqsDelayLeftEdge|0x5                      # RxDqsDelay Left Edge
gStructPcdTokenSpaceGuid.PcdMemBootHealthConfig.WarningRxDqsDelayRightEdge|0x5                     # RxDqsDelay Right Edge
gStructPcdTokenSpaceGuid.PcdMemBootHealthConfig.WarningRxVrefLeftEdge|0x5                          # RxVref Left Edge
gStructPcdTokenSpaceGuid.PcdMemBootHealthConfig.WarningRxVrefRightEdge|0x5                         # RxVref Right Edge
gStructPcdTokenSpaceGuid.PcdMemBootHealthConfig.WarningTxDqDelayLeftEdge|0x5                       # TxDqDelay Left Edge
gStructPcdTokenSpaceGuid.PcdMemBootHealthConfig.WarningTxDqDelayRightEdge|0x5                      # TxDqDelay Right Edge
gStructPcdTokenSpaceGuid.PcdMemBootHealthConfig.WarningTxVrefLeftEdge|0x5                          # TxVref Left Edge
gStructPcdTokenSpaceGuid.PcdMemBootHealthConfig.WarningTxVrefRightEdge|0x5                         # TxVref Right Edge
gStructPcdTokenSpaceGuid.PcdPchSetup.AdrCpuThermalWdt|0x0                                          # CPU Thermal WDT ADR Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.AdrGpioSel|0x0                                                # ADR GPIO
gStructPcdTokenSpaceGuid.PcdPchSetup.AdrHostPartitionReset|0x0                                     # Host Partition Reset ADR Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.AdrMultiplierVal|0x63                                         # ADR timer multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.AdrOverClockingWdt|0x0                                        # Over-Clocking WDT ADR Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.AdrPmcParityError|0x0                                         # PMC Parity Error ADR Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.AdrSysPwrOk|0x0                                               # SYS_PWROK Failure ADR Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.AdrTimerEn|0x0                                                # Enable/Disable ADR Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.AdrTimerVal|0x4                                               # ADR timer expire time
gStructPcdTokenSpaceGuid.PcdPchSetup.AetEnableMode|0x0                                             # AET Enable Mode
gStructPcdTokenSpaceGuid.PcdPchSetup.Btcg|0x1                                                      # Trunk Clock Gating (BTCG)
gStructPcdTokenSpaceGuid.PcdPchSetup.DciEn|0x0                                                     # DCI enable (HDCIEN)
gStructPcdTokenSpaceGuid.PcdPchSetup.DeepSxMode|0x0                                                # DeepSx Power Policies
gStructPcdTokenSpaceGuid.PcdPchSetup.DfxHdaVcType|0x0                                              # Virtual Channel for HD Audio
gStructPcdTokenSpaceGuid.PcdPchSetup.DmiLinkDownHangBypass|0x0                                     # DMI Link Down Hang Bypass (DLDHB)
gStructPcdTokenSpaceGuid.PcdPchSetup.DwrEn_IEWDT|0x0                                               # IE FW Watchdog Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.DwrEn_MEWDT|0x0                                               # ME FW Watchdog Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.DwrEn_PMCGBL|0x1                                              # Host Reset Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.Dwr_BmcRootPort|0x5                                           # BMC RootPort
gStructPcdTokenSpaceGuid.PcdPchSetup.Dwr_Enable|0x0                                                # Dirty Warm Reset
gStructPcdTokenSpaceGuid.PcdPchSetup.Dwr_IeResetPrepDone|0x1                                       # IE Reset Prep Done
gStructPcdTokenSpaceGuid.PcdPchSetup.Dwr_MeResetPrepDone|0x1                                       # ME Reset Prep Done
gStructPcdTokenSpaceGuid.PcdPchSetup.Dwr_Stall|0x0                                                 # Dirty Warm Reset Stall
gStructPcdTokenSpaceGuid.PcdPchSetup.EnableClockSpreadSpec|0x1                                     # External SSC Enable - CK420
gStructPcdTokenSpaceGuid.PcdPchSetup.EnableUsb3Pin[0]|0x0                                          # USB 3.0 pin #1
gStructPcdTokenSpaceGuid.PcdPchSetup.EnableUsb3Pin[1]|0x0                                          # USB 3.0 pin #2
gStructPcdTokenSpaceGuid.PcdPchSetup.EnableUsb3Pin[2]|0x0                                          # USB 3.0 pin #3
gStructPcdTokenSpaceGuid.PcdPchSetup.EnableUsb3Pin[3]|0x0                                          # USB 3.0 pin #4
gStructPcdTokenSpaceGuid.PcdPchSetup.EnableUsb3Pin[4]|0x0                                          # USB 3.0 pin #5
gStructPcdTokenSpaceGuid.PcdPchSetup.EnableUsb3Pin[5]|0x0                                          # USB 3.0 pin #6
gStructPcdTokenSpaceGuid.PcdPchSetup.EnableUsb3Pin[6]|0x0                                          # USB 3.0 pin #7
gStructPcdTokenSpaceGuid.PcdPchSetup.EnableUsb3Pin[7]|0x0                                          # USB 3.0 pin #8
gStructPcdTokenSpaceGuid.PcdPchSetup.EnableUsb3Pin[8]|0x0                                          # USB 3.0 pin #9
gStructPcdTokenSpaceGuid.PcdPchSetup.EnableUsb3Pin[9]|0x0                                          # USB 3.0 pin #10
gStructPcdTokenSpaceGuid.PcdPchSetup.FirmwareConfiguration|0x0                                     # Firmware Configuration
gStructPcdTokenSpaceGuid.PcdPchSetup.FlashLockDown|0x0                                             # Flash Lock-Down
gStructPcdTokenSpaceGuid.PcdPchSetup.GbeRegionInvalid|0x0                                          # Gbe Region Valid
gStructPcdTokenSpaceGuid.PcdPchSetup.Gp27WakeFromDeepSx|0x0                                        # GP27 Wake From DeepSx
gStructPcdTokenSpaceGuid.PcdPchSetup.IchPort80Route|0x0                                            # Port 80h Redirection
gStructPcdTokenSpaceGuid.PcdPchSetup.PchAdrEn|0x0                                                  # Enable/Disable ADR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchAllUnLock|0x0                                              # Unlock All PCH registers
gStructPcdTokenSpaceGuid.PcdPchSetup.PchBiosLock|0x1                                               # BIOS Lock
gStructPcdTokenSpaceGuid.PcdPchSetup.PchCrossThrottling|0x1                                        # PCH Cross Throttling
gStructPcdTokenSpaceGuid.PcdPchSetup.PchDciAutoDetect|0x0                                          # DCI Auto Detect Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchEvaLockDown|0x1                                            # EVA registers LOCK
gStructPcdTokenSpaceGuid.PcdPchSetup.PchEvaMrom0HookEnable|0x1                                     # MROM 0 Hook
gStructPcdTokenSpaceGuid.PcdPchSetup.PchEvaMrom1HookEnable|0x1                                     # MROM 1 Hook
gStructPcdTokenSpaceGuid.PcdPchSetup.PchGpioLockDown|0x1                                           # GPIO Lockdown
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudio|0x1                                                # Azalia
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioCodecSelect|0x0                                     # HDA-Link Codec Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPme|0x0                                             # Azalia PME Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIoApic24119Entries|0x1                                     # IO-APIC 24-119 RTE
gStructPcdTokenSpaceGuid.PcdPchSetup.PchLanK1Off|0x0                                               # K1 off
gStructPcdTokenSpaceGuid.PcdPchSetup.PchP2sbDevReveal|0x0                                          # Reveal PCH P2SB device
gStructPcdTokenSpaceGuid.PcdPchSetup.PchP2sbUnlock|0x0                                             # Unlock PCH P2SB
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieForceLtrOverride[0]|0x0                                # Force LTR Override PCIE1
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieForceLtrOverride[1]|0x0                                # Force LTR Override PCIE2
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieForceLtrOverride[2]|0x0                                # Force LTR Override PCIE3
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieForceLtrOverride[3]|0x0                                # Force LTR Override PCIE4
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieForceLtrOverride[4]|0x0                                # Force LTR Override PCIE5
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieForceLtrOverride[5]|0x0                                # Force LTR Override PCIE6
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieForceLtrOverride[6]|0x0                                # Force LTR Override PCIE7
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieForceLtrOverride[7]|0x0                                # Force LTR Override PCIE8
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieForceLtrOverride[8]|0x0                                # Force LTR Override PCIE9
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieForceLtrOverride[9]|0x0                                # Force LTR Override PCIE10
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieForceLtrOverride[10]|0x0                               # Force LTR Override PCIE11
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieForceLtrOverride[11]|0x0                               # Force LTR Override PCIE12
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieForceLtrOverride[12]|0x0                               # Force LTR Override PCIE13
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieForceLtrOverride[13]|0x0                               # Force LTR Override PCIE14
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieForceLtrOverride[14]|0x0                               # Force LTR Override PCIE15
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieForceLtrOverride[15]|0x0                               # Force LTR Override PCIE16
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieForceLtrOverride[16]|0x0                               # Force LTR Override PCIE17
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieForceLtrOverride[17]|0x0                               # Force LTR Override PCIE18
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieForceLtrOverride[18]|0x0                               # Force LTR Override PCIE19
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieForceLtrOverride[19]|0x0                               # Force LTR Override PCIE20
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieGlobalAspm|0x2                                         # PCI-E ASPM Support (Global)
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[0]|0x1                                       # PCH PCIE1 LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[1]|0x1                                       # PCH PCIE2 LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[2]|0x1                                       # PCH PCIE3 LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[3]|0x1                                       # PCH PCIE4 LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[4]|0x1                                       # PCH PCIE5 LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[5]|0x1                                       # PCH PCIE6 LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[6]|0x1                                       # PCH PCIE7 LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[7]|0x1                                       # PCH PCIE8 LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[8]|0x1                                       # PCH PCIE9 LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[9]|0x1                                       # PCH PCIE10 LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[10]|0x1                                      # PCH PCIE11 LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[11]|0x1                                      # PCH PCIE12 LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[12]|0x1                                      # PCH PCIE13 LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[13]|0x1                                      # PCH PCIE14 LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[14]|0x1                                      # PCH PCIE15 LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[15]|0x1                                      # PCH PCIE16 LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[16]|0x1                                      # PCH PCIE17 LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[17]|0x1                                      # PCH PCIE18 LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[18]|0x1                                      # PCH PCIE19 LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[19]|0x1                                      # PCH PCIE20 LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[0]|0x2                     # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[1]|0x2                     # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[2]|0x2                     # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[3]|0x2                     # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[4]|0x2                     # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[5]|0x2                     # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[6]|0x2                     # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[7]|0x2                     # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[8]|0x2                     # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[9]|0x2                     # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[10]|0x2                    # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[11]|0x2                    # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[12]|0x2                    # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[13]|0x2                    # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[14]|0x2                    # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[15]|0x2                    # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[16]|0x2                    # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[17]|0x2                    # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[18]|0x2                    # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[19]|0x2                    # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[0]|0x2               # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[1]|0x2               # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[2]|0x2               # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[3]|0x2               # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[4]|0x2               # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[5]|0x2               # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[6]|0x2               # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[7]|0x2               # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[8]|0x2               # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[9]|0x2               # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[10]|0x2              # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[11]|0x2              # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[12]|0x2              # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[13]|0x2              # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[14]|0x2              # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[15]|0x2              # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[16]|0x2              # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[17]|0x2              # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[18]|0x2              # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[19]|0x2              # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[0]|0x3c                   # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[1]|0x3c                   # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[2]|0x3c                   # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[3]|0x3c                   # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[4]|0x3c                   # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[5]|0x3c                   # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[6]|0x3c                   # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[7]|0x3c                   # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[8]|0x3c                   # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[9]|0x3c                   # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[10]|0x3c                  # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[11]|0x3c                  # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[12]|0x3c                  # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[13]|0x3c                  # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[14]|0x3c                  # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[15]|0x3c                  # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[16]|0x3c                  # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[17]|0x3c                  # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[18]|0x3c                  # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[19]|0x3c                  # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[0]|0x2                        # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[1]|0x2                        # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[2]|0x2                        # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[3]|0x2                        # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[4]|0x2                        # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[5]|0x2                        # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[6]|0x2                        # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[7]|0x2                        # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[8]|0x2                        # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[9]|0x2                        # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[10]|0x2                       # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[11]|0x2                       # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[12]|0x2                       # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[13]|0x2                       # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[14]|0x2                       # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[15]|0x2                       # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[16]|0x2                       # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[17]|0x2                       # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[18]|0x2                       # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[19]|0x2                       # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[0]|0x2                  # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[1]|0x2                  # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[2]|0x2                  # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[3]|0x2                  # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[4]|0x2                  # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[5]|0x2                  # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[6]|0x2                  # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[7]|0x2                  # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[8]|0x2                  # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[9]|0x2                  # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[10]|0x2                 # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[11]|0x2                 # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[12]|0x2                 # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[13]|0x2                 # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[14]|0x2                 # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[15]|0x2                 # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[16]|0x2                 # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[17]|0x2                 # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[18]|0x2                 # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[19]|0x2                 # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[0]|0x3c                      # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[1]|0x3c                      # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[2]|0x3c                      # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[3]|0x3c                      # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[4]|0x3c                      # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[5]|0x3c                      # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[6]|0x3c                      # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[7]|0x3c                      # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[8]|0x3c                      # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[9]|0x3c                      # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[10]|0x3c                     # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[11]|0x3c                     # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[12]|0x3c                     # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[13]|0x3c                     # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[14]|0x3c                     # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[15]|0x3c                     # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[16]|0x3c                     # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[17]|0x3c                     # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[18]|0x3c                     # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[19]|0x3c                     # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieUX16CompletionTimeout|0x0                              # CTO for Uplink x16
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieUX16MaxPayloadSize|0x2                                 # MPL for Uplink x16
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieUX8CompletionTimeout|0x0                               # CTO for Uplink x8
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieUX8MaxPayloadSize|0x2                                  # MPL for Uplink x8
gStructPcdTokenSpaceGuid.PcdPchSetup.PchRtcLock|0x1                                                # RTC Lock
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSSataLtrConfigLock|0x1                                     # sSATA LTR Lock
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSSataLtrEnable|0x1                                         # PCH sSATA LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSSataLtrOverride|0x0                                       # sSATA LTR Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSSataSnoopLatencyOverrideMultiplier|0x2                    # sSATA Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSSataSnoopLatencyOverrideValue|0x28                        # sSATA Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSata|0x1                                                   # SATA Controller
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSataLtrConfigLock|0x1                                      # SATA LTR Lock
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSataLtrEnable|0x1                                          # PCH SATA LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSataLtrOverride|0x0                                        # SATA LTR Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSataSnoopLatencyOverrideMultiplier|0x2                     # SATA Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSataSnoopLatencyOverrideValue|0x28                         # SATA Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSirqMode|0x0                                               # Serial IRQ Mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSlpLanLowDc|0x1                                            # SLP_LAN# Low on DC Power
gStructPcdTokenSpaceGuid.PcdPchSetup.PchThermalUnlock|0x0                                          # Unlock Thermal Registers
gStructPcdTokenSpaceGuid.PcdPchSetup.PchTraceHubHide|0x1                                           # Hide Trace Hub
gStructPcdTokenSpaceGuid.PcdPchSetup.PchTraceHubMemReg0Size|0x0                                    # MemRegion 0 Buffer Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PchTraceHubMemReg1Size|0x0                                    # MemRegion 1 Buffer Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PchTraceHubMode|0x2                                           # TraceHub Enable Mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbDegradeBar|0x0                                          # Place XHCI BAR below 4GB
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[0]|0x1                                           # USB HS Physical Connector #0 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[1]|0x1                                           # USB HS Physical Connector #1 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[2]|0x1                                           # USB HS Physical Connector #2 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[3]|0x1                                           # USB HS Physical Connector #3 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[4]|0x1                                           # USB HS Physical Connector #4 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[5]|0x1                                           # USB HS Physical Connector #5 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[6]|0x1                                           # USB HS Physical Connector #6 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[7]|0x1                                           # USB HS Physical Connector #7 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[8]|0x1                                           # USB HS Physical Connector #8 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[9]|0x1                                           # USB HS Physical Connector #9 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[10]|0x1                                          # USB HS Physical Connector #10 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[11]|0x1                                          # USB HS Physical Connector #11 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[12]|0x1                                          # USB HS Physical Connector #12 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[13]|0x1                                          # USB HS Physical Connector #13 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbManualMode|0x0                                          # XHCI Manual Mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbPerPortCtl|0x0                                          # USB Per-Connector Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbSsPort[0]|0x1                                           # USB SS Physical Connector #0 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbSsPort[1]|0x1                                           # USB SS Physical Connector #1 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbSsPort[2]|0x1                                           # USB SS Physical Connector #2 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbSsPort[3]|0x1                                           # USB SS Physical Connector #3 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbSsPort[4]|0x1                                           # USB SS Physical Connector #4 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbSsPort[5]|0x1                                           # USB SS Physical Connector #5 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbSsPort[6]|0x1                                           # USB SS Physical Connector #6 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbSsPort[7]|0x1                                           # USB SS Physical Connector #7 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbSsPort[8]|0x1                                           # USB SS Physical Connector #8 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbSsPort[9]|0x1                                           # USB SS Physical Connector #9 Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchWakeOnLan|0x0                                              # Wake on LAN
gStructPcdTokenSpaceGuid.PcdPchSetup.PchsSata|0x1                                                  # sSATA Controller
gStructPcdTokenSpaceGuid.PcdPchSetup.PciDelayOptimizationEcr|0x0                                   # PCI Delay Optimization
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieComplianceTestMode|0x0                                    # Compliance Test Mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieDmiExtSync|0x0                                            # DMI Link Extended Synch Control
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieDmiStopAndScreamEnable|0x0                                # Stop and Scream
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCm[0]|0x6                                             # PCIE1  Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCm[1]|0x6                                             # PCIE2  Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCm[2]|0x6                                             # PCIE3  Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCm[3]|0x6                                             # PCIE4  Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCm[4]|0x6                                             # PCIE5  Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCm[5]|0x6                                             # PCIE6  Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCm[6]|0x6                                             # PCIE7  Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCm[7]|0x6                                             # PCIE8  Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCm[8]|0x6                                             # PCIE9  Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCm[9]|0x6                                             # PCIE10 Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCm[10]|0x6                                            # PCIE11 Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCm[11]|0x6                                            # PCIE12 Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCm[12]|0x6                                            # PCIE13 Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCm[13]|0x6                                            # PCIE14 Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCm[14]|0x6                                            # PCIE15 Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCm[15]|0x6                                            # PCIE16 Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCm[16]|0x6                                            # PCIE17 Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCm[17]|0x6                                            # PCIE18 Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCm[18]|0x6                                            # PCIE19 Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCm[19]|0x6                                            # PCIE20 Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCp[0]|0x6                                             # PCIE1  Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCp[1]|0x6                                             # PCIE2  Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCp[2]|0x6                                             # PCIE3  Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCp[3]|0x6                                             # PCIE4  Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCp[4]|0x6                                             # PCIE5  Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCp[5]|0x6                                             # PCIE6  Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCp[6]|0x6                                             # PCIE7  Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCp[7]|0x6                                             # PCIE8  Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCp[8]|0x6                                             # PCIE9  Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCp[9]|0x6                                             # PCIE10 Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCp[10]|0x6                                            # PCIE11 Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCp[11]|0x6                                            # PCIE12 Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCp[12]|0x6                                            # PCIE13 Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCp[13]|0x6                                            # PCIE14 Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCp[14]|0x6                                            # PCIE15 Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCp[15]|0x6                                            # PCIE16 Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCp[16]|0x6                                            # PCIE17 Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCp[17]|0x6                                            # PCIE18 Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCp[18]|0x6                                            # PCIE19 Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieLaneCp[19]|0x6                                            # PCIE20 Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PciePllSsc|0xfe                                               # Pcie Pll SSC
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[0]|0x1                                        # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[1]|0x1                                        # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[2]|0x1                                        # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[3]|0x1                                        # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[4]|0x1                                        # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[5]|0x1                                        # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[6]|0x1                                        # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[7]|0x1                                        # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[8]|0x1                                        # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[9]|0x1                                        # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[10]|0x1                                       # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[11]|0x1                                       # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[12]|0x1                                       # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[13]|0x1                                       # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[14]|0x1                                       # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[15]|0x1                                       # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[16]|0x1                                       # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[17]|0x1                                       # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[18]|0x1                                       # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[19]|0x1                                       # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[0]|0x1                                        # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[1]|0x1                                        # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[2]|0x1                                        # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[3]|0x1                                        # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[4]|0x1                                        # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[5]|0x1                                        # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[6]|0x1                                        # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[7]|0x1                                        # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[8]|0x1                                        # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[9]|0x1                                        # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[10]|0x1                                       # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[11]|0x1                                       # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[12]|0x1                                       # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[13]|0x1                                       # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[14]|0x1                                       # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[15]|0x1                                       # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[16]|0x1                                       # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[17]|0x1                                       # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[18]|0x1                                       # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[19]|0x1                                       # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[0]|0x0                                       # PCIE ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[1]|0x0                                       # PCIE ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[2]|0x0                                       # PCIE ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[3]|0x0                                       # PCIE ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[4]|0x0                                       # PCIE ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[5]|0x0                                       # PCIE ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[6]|0x0                                       # PCIE ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[7]|0x0                                       # PCIE ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[8]|0x0                                       # PCIE ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[9]|0x0                                       # PCIE ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[10]|0x0                                      # PCIE ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[11]|0x0                                      # PCIE ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[12]|0x0                                      # PCIE ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[13]|0x0                                      # PCIE ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[14]|0x0                                      # PCIE ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[15]|0x0                                      # PCIE ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[16]|0x0                                      # PCIE ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[17]|0x0                                      # PCIE ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[18]|0x0                                      # PCIE ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[19]|0x0                                      # PCIE ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[0]|0x0                                        # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[1]|0x0                                        # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[2]|0x0                                        # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[3]|0x0                                        # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[4]|0x0                                        # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[5]|0x0                                        # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[6]|0x0                                        # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[7]|0x0                                        # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[8]|0x0                                        # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[9]|0x0                                        # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[10]|0x0                                       # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[11]|0x0                                       # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[12]|0x0                                       # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[13]|0x0                                       # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[14]|0x0                                       # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[15]|0x0                                       # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[16]|0x0                                       # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[17]|0x0                                       # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[18]|0x0                                       # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[19]|0x0                                       # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCompletionTimeout[0]|0x0                          # Compl. Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCompletionTimeout[1]|0x0                          # Compl. Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCompletionTimeout[2]|0x0                          # Compl. Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCompletionTimeout[3]|0x0                          # Compl. Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCompletionTimeout[4]|0x0                          # Compl. Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCompletionTimeout[5]|0x0                          # Compl. Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCompletionTimeout[6]|0x0                          # Compl. Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCompletionTimeout[7]|0x0                          # Compl. Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCompletionTimeout[8]|0x0                          # Compl. Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCompletionTimeout[9]|0x0                          # Compl. Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCompletionTimeout[10]|0x0                         # Compl. Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCompletionTimeout[11]|0x0                         # Compl. Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCompletionTimeout[12]|0x0                         # Compl. Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCompletionTimeout[13]|0x0                         # Compl. Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCompletionTimeout[14]|0x0                         # Compl. Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCompletionTimeout[15]|0x0                         # Compl. Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCompletionTimeout[16]|0x0                         # Compl. Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCompletionTimeout[17]|0x0                         # Compl. Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCompletionTimeout[18]|0x0                         # Compl. Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCompletionTimeout[19]|0x0                         # Compl. Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[0]|0x1                                         # PCI Express Root Port 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[1]|0x1                                         # PCI Express Root Port 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[2]|0x1                                         # PCI Express Root Port 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[3]|0x1                                         # PCI Express Root Port 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[4]|0x1                                         # PCI Express Root Port 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[5]|0x1                                         # PCI Express Root Port 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[6]|0x1                                         # PCI Express Root Port 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[7]|0x1                                         # PCI Express Root Port 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[8]|0x1                                         # PCI Express Root Port 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[9]|0x1                                         # PCI Express Root Port 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[10]|0x1                                        # PCI Express Root Port 11
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[11]|0x1                                        # PCI Express Root Port 12
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[12]|0x1                                        # PCI Express Root Port 13
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[13]|0x1                                        # PCI Express Root Port 14
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[14]|0x1                                        # PCI Express Root Port 15
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[15]|0x1                                        # PCI Express Root Port 16
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[16]|0x1                                        # PCI Express Root Port 17
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[17]|0x1                                        # PCI Express Root Port 18
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[18]|0x1                                        # PCI Express Root Port 19
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[19]|0x1                                        # PCI Express Root Port 20
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEqPh3Method[0]|0x1                                # Gen3 Eq Phase3 Method
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEqPh3Method[1]|0x1                                # Gen3 Eq Phase3 Method
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEqPh3Method[2]|0x1                                # Gen3 Eq Phase3 Method
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEqPh3Method[3]|0x1                                # Gen3 Eq Phase3 Method
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEqPh3Method[4]|0x1                                # Gen3 Eq Phase3 Method
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEqPh3Method[5]|0x1                                # Gen3 Eq Phase3 Method
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEqPh3Method[6]|0x1                                # Gen3 Eq Phase3 Method
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEqPh3Method[7]|0x1                                # Gen3 Eq Phase3 Method
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEqPh3Method[8]|0x1                                # Gen3 Eq Phase3 Method
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEqPh3Method[9]|0x1                                # Gen3 Eq Phase3 Method
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEqPh3Method[10]|0x1                               # Gen3 Eq Phase3 Method
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEqPh3Method[11]|0x1                               # Gen3 Eq Phase3 Method
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEqPh3Method[12]|0x1                               # Gen3 Eq Phase3 Method
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEqPh3Method[13]|0x1                               # Gen3 Eq Phase3 Method
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEqPh3Method[14]|0x1                               # Gen3 Eq Phase3 Method
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEqPh3Method[15]|0x1                               # Gen3 Eq Phase3 Method
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEqPh3Method[16]|0x1                               # Gen3 Eq Phase3 Method
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEqPh3Method[17]|0x1                               # Gen3 Eq Phase3 Method
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEqPh3Method[18]|0x1                               # Gen3 Eq Phase3 Method
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEqPh3Method[19]|0x1                               # Gen3 Eq Phase3 Method
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[0]|0x0                                        # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[1]|0x0                                        # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[2]|0x0                                        # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[3]|0x0                                        # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[4]|0x0                                        # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[5]|0x0                                        # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[6]|0x0                                        # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[7]|0x0                                        # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[8]|0x0                                        # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[9]|0x0                                        # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[10]|0x0                                       # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[11]|0x0                                       # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[12]|0x0                                       # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[13]|0x0                                       # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[14]|0x0                                       # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[15]|0x0                                       # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[16]|0x0                                       # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[17]|0x0                                       # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[18]|0x0                                       # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[19]|0x0                                       # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFunctionSwapping|0x1                              # PCIe Root Port Function Swapping
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[0]|0x0                                        # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[1]|0x0                                        # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[2]|0x0                                        # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[3]|0x0                                        # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[4]|0x0                                        # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[5]|0x0                                        # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[6]|0x0                                        # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[7]|0x0                                        # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[8]|0x0                                        # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[9]|0x0                                        # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[10]|0x0                                       # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[11]|0x0                                       # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[12]|0x0                                       # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[13]|0x0                                       # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[14]|0x0                                       # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[15]|0x0                                       # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[16]|0x0                                       # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[17]|0x0                                       # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[18]|0x0                                       # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[19]|0x0                                       # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[0]|0x3                                # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[1]|0x3                                # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[2]|0x3                                # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[3]|0x3                                # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[4]|0x3                                # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[5]|0x3                                # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[6]|0x3                                # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[7]|0x3                                # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[8]|0x3                                # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[9]|0x3                                # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[10]|0x3                               # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[11]|0x3                               # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[12]|0x3                               # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[13]|0x3                               # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[14]|0x3                               # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[15]|0x3                               # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[16]|0x3                               # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[17]|0x3                               # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[18]|0x3                               # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[19]|0x3                               # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortMaxPayLoadSize[0]|0x1                             # Max Payload Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortMaxPayLoadSize[1]|0x1                             # Max Payload Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortMaxPayLoadSize[2]|0x1                             # Max Payload Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortMaxPayLoadSize[3]|0x1                             # Max Payload Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortMaxPayLoadSize[4]|0x1                             # Max Payload Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortMaxPayLoadSize[5]|0x1                             # Max Payload Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortMaxPayLoadSize[6]|0x1                             # Max Payload Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortMaxPayLoadSize[7]|0x1                             # Max Payload Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortMaxPayLoadSize[8]|0x1                             # Max Payload Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortMaxPayLoadSize[9]|0x1                             # Max Payload Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortMaxPayLoadSize[10]|0x1                            # Max Payload Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortMaxPayLoadSize[11]|0x1                            # Max Payload Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortMaxPayLoadSize[12]|0x1                            # Max Payload Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortMaxPayLoadSize[13]|0x1                            # Max Payload Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortMaxPayLoadSize[14]|0x1                            # Max Payload Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortMaxPayLoadSize[15]|0x1                            # Max Payload Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortMaxPayLoadSize[16]|0x1                            # Max Payload Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortMaxPayLoadSize[17]|0x1                            # Max Payload Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortMaxPayLoadSize[18]|0x1                            # Max Payload Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortMaxPayLoadSize[19]|0x1                            # Max Payload Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortMaxReadRequestSize|0x5                            # Max Read Request Size
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[0]|0x0                                        # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[1]|0x0                                        # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[2]|0x0                                        # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[3]|0x0                                        # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[4]|0x0                                        # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[5]|0x0                                        # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[6]|0x0                                        # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[7]|0x0                                        # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[8]|0x0                                        # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[9]|0x0                                        # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[10]|0x0                                       # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[11]|0x0                                       # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[12]|0x0                                       # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[13]|0x0                                       # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[14]|0x0                                       # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[15]|0x0                                       # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[16]|0x0                                       # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[17]|0x0                                       # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[18]|0x0                                       # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[19]|0x0                                       # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[0]|0x0                                       # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[1]|0x0                                       # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[2]|0x0                                       # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[3]|0x0                                       # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[4]|0x0                                       # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[5]|0x0                                       # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[6]|0x0                                       # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[7]|0x0                                       # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[8]|0x0                                       # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[9]|0x0                                       # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[10]|0x0                                      # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[11]|0x0                                      # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[12]|0x0                                      # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[13]|0x0                                      # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[14]|0x0                                      # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[15]|0x0                                      # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[16]|0x0                                      # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[17]|0x0                                      # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[18]|0x0                                      # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[19]|0x0                                      # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSBDE|0x0                                          # Subtractive Decode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[0]|0x0                                        # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[1]|0x0                                        # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[2]|0x0                                        # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[3]|0x0                                        # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[4]|0x0                                        # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[5]|0x0                                        # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[6]|0x0                                        # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[7]|0x0                                        # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[8]|0x0                                        # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[9]|0x0                                        # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[10]|0x0                                       # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[11]|0x0                                       # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[12]|0x0                                       # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[13]|0x0                                       # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[14]|0x0                                       # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[15]|0x0                                       # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[16]|0x0                                       # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[17]|0x0                                       # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[18]|0x0                                       # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[19]|0x0                                       # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[0]|0x0                                        # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[1]|0x0                                        # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[2]|0x0                                        # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[3]|0x0                                        # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[4]|0x0                                        # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[5]|0x0                                        # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[6]|0x0                                        # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[7]|0x0                                        # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[8]|0x0                                        # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[9]|0x0                                        # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[10]|0x0                                       # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[11]|0x0                                       # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[12]|0x0                                       # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[13]|0x0                                       # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[14]|0x0                                       # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[15]|0x0                                       # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[16]|0x0                                       # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[17]|0x0                                       # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[18]|0x0                                       # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[19]|0x0                                       # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[0]|0x0                                        # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[1]|0x0                                        # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[2]|0x0                                        # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[3]|0x0                                        # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[4]|0x0                                        # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[5]|0x0                                        # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[6]|0x0                                        # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[7]|0x0                                        # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[8]|0x0                                        # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[9]|0x0                                        # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[10]|0x0                                       # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[11]|0x0                                       # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[12]|0x0                                       # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[13]|0x0                                       # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[14]|0x0                                       # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[15]|0x0                                       # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[16]|0x0                                       # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[17]|0x0                                       # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[18]|0x0                                       # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[19]|0x0                                       # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[0]|0x0                                      # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[1]|0x0                                      # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[2]|0x0                                      # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[3]|0x0                                      # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[4]|0x0                                      # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[5]|0x0                                      # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[6]|0x0                                      # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[7]|0x0                                      # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[8]|0x0                                      # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[9]|0x0                                      # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[10]|0x0                                     # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[11]|0x0                                     # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[12]|0x0                                     # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[13]|0x0                                     # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[14]|0x0                                     # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[15]|0x0                                     # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[16]|0x0                                     # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[17]|0x0                                     # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[18]|0x0                                     # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[19]|0x0                                     # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[0]|0x0                                        # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[1]|0x0                                        # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[2]|0x0                                        # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[3]|0x0                                        # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[4]|0x0                                        # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[5]|0x0                                        # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[6]|0x0                                        # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[7]|0x0                                        # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[8]|0x0                                        # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[9]|0x0                                        # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[10]|0x0                                       # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[11]|0x0                                       # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[12]|0x0                                       # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[13]|0x0                                       # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[14]|0x0                                       # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[15]|0x0                                       # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[16]|0x0                                       # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[17]|0x0                                       # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[18]|0x0                                       # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[19]|0x0                                       # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieSBDEPort|0x0                                              # Subtractive Decode Port#
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieSwEqCoeffCm[0]|0x6                                        # Coeff0 Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieSwEqCoeffCm[1]|0x4                                        # Coeff1 Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieSwEqCoeffCm[2]|0x8                                        # Coeff2 Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieSwEqCoeffCm[3]|0x2                                        # Coeff3 Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieSwEqCoeffCm[4]|0xa                                        # Coeff4 Cm
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieSwEqCoeffCp[0]|0x2                                        # Coeff0 Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieSwEqCoeffCp[1]|0x2                                        # Coeff1 Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieSwEqCoeffCp[2]|0x2                                        # Coeff2 Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieSwEqCoeffCp[3]|0x2                                        # Coeff3 Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieSwEqCoeffCp[4]|0x2                                        # Coeff4 Cp
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieSwEqOverride|0x0                                          # Override SW/HW EQ settings
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieTopology[0]|0x0                                           # PCIE Lane Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieTopology[1]|0x0                                           # PCIE Lane Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieTopology[2]|0x0                                           # PCIE Lane Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieTopology[3]|0x0                                           # PCIE Lane Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieTopology[4]|0x0                                           # PCIE Lane Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieTopology[5]|0x0                                           # PCIE Lane Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieTopology[6]|0x0                                           # PCIE Lane Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieTopology[7]|0x0                                           # PCIE Lane Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieTopology[8]|0x0                                           # PCIE Lane Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieTopology[9]|0x0                                           # PCIE Lane Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieTopology[10]|0x0                                          # PCIE Lane Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieTopology[11]|0x0                                          # PCIE Lane Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieTopology[12]|0x0                                          # PCIE Lane Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieTopology[13]|0x0                                          # PCIE Lane Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieTopology[14]|0x0                                          # PCIE Lane Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieTopology[15]|0x0                                          # PCIE Lane Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieTopology[16]|0x0                                          # PCIE Lane Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieTopology[17]|0x0                                          # PCIE Lane Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieTopology[18]|0x0                                          # PCIE Lane Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieTopology[19]|0x0                                          # PCIE Lane Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.PmcReadDisable|0x1                                            # PMC Read Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.PxDevSlp[2]|0x0                                               # SATA Port 2 DevSlp
gStructPcdTokenSpaceGuid.PcdPchSetup.RstPcieStorageRemap[0]|0x0                                    # NVRAM CYCLE ROUTER 0 ENABLE
gStructPcdTokenSpaceGuid.PcdPchSetup.RstPcieStorageRemap[1]|0x0                                    # NVRAM CYCLE ROUTER 1 ENABLE
gStructPcdTokenSpaceGuid.PcdPchSetup.RstPcieStorageRemap[2]|0x0                                    # NVRAM CYCLE ROUTER 2 ENABLE
gStructPcdTokenSpaceGuid.PcdPchSetup.RstPcieStorageRemapPort[0]|0x0                                # NVRAM CR0 PCIE Root Port Number
gStructPcdTokenSpaceGuid.PcdPchSetup.RstPcieStorageRemapPort[1]|0x0                                # NVRAM CR1 PCIE Root Port Number
gStructPcdTokenSpaceGuid.PcdPchSetup.RstPcieStorageRemapPort[2]|0x0                                # NVRAM CR2 PCIE Root Port Number
gStructPcdTokenSpaceGuid.PcdPchSetup.SataAlternateId|0x0                                           # Alternate Device ID on RAID
gStructPcdTokenSpaceGuid.PcdPchSetup.SataExternal[0]|0x0                                           # Configure as eSATA
gStructPcdTokenSpaceGuid.PcdPchSetup.SataExternal[1]|0x0                                           # Configure as eSATA
gStructPcdTokenSpaceGuid.PcdPchSetup.SataExternal[2]|0x0                                           # Configure as eSATA
gStructPcdTokenSpaceGuid.PcdPchSetup.SataExternal[3]|0x0                                           # Configure as eSATA
gStructPcdTokenSpaceGuid.PcdPchSetup.SataExternal[4]|0x0                                           # Configure as eSATA
gStructPcdTokenSpaceGuid.PcdPchSetup.SataExternal[5]|0x0                                           # Configure as eSATA
gStructPcdTokenSpaceGuid.PcdPchSetup.SataExternal[6]|0x0                                           # Configure as eSATA
gStructPcdTokenSpaceGuid.PcdPchSetup.SataExternal[7]|0x0                                           # Configure as eSATA
gStructPcdTokenSpaceGuid.PcdPchSetup.SataHddlk|0x1                                                 # SATA HDD Unlock
gStructPcdTokenSpaceGuid.PcdPchSetup.SataHotPlug[0]|0x0                                            # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.SataHotPlug[1]|0x0                                            # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.SataHotPlug[2]|0x0                                            # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.SataHotPlug[3]|0x0                                            # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.SataHotPlug[4]|0x0                                            # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.SataHotPlug[5]|0x0                                            # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.SataHotPlug[6]|0x0                                            # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.SataHotPlug[7]|0x0                                            # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.SataInterfaceMode|0x0                                         # Configure SATA as
gStructPcdTokenSpaceGuid.PcdPchSetup.SataLedl|0x1                                                  # SATA Led locate
gStructPcdTokenSpaceGuid.PcdPchSetup.SataMechanicalSw[0]|0x0                                       # Mechanical Presence Switch
gStructPcdTokenSpaceGuid.PcdPchSetup.SataMechanicalSw[1]|0x0                                       # Mechanical Presence Switch
gStructPcdTokenSpaceGuid.PcdPchSetup.SataMechanicalSw[2]|0x0                                       # Mechanical Presence Switch
gStructPcdTokenSpaceGuid.PcdPchSetup.SataMechanicalSw[3]|0x0                                       # Mechanical Presence Switch
gStructPcdTokenSpaceGuid.PcdPchSetup.SataMechanicalSw[4]|0x0                                       # Mechanical Presence Switch
gStructPcdTokenSpaceGuid.PcdPchSetup.SataMechanicalSw[5]|0x0                                       # Mechanical Presence Switch
gStructPcdTokenSpaceGuid.PcdPchSetup.SataMechanicalSw[6]|0x0                                       # Mechanical Presence Switch
gStructPcdTokenSpaceGuid.PcdPchSetup.SataMechanicalSw[7]|0x0                                       # Mechanical Presence Switch
gStructPcdTokenSpaceGuid.PcdPchSetup.SataPort[0]|0x1                                               # Port 0
gStructPcdTokenSpaceGuid.PcdPchSetup.SataPort[1]|0x1                                               # Port 1
gStructPcdTokenSpaceGuid.PcdPchSetup.SataPort[2]|0x1                                               # Port 2
gStructPcdTokenSpaceGuid.PcdPchSetup.SataPort[3]|0x1                                               # Port 3
gStructPcdTokenSpaceGuid.PcdPchSetup.SataPort[4]|0x1                                               # Port 4
gStructPcdTokenSpaceGuid.PcdPchSetup.SataPort[5]|0x1                                               # Port 5
gStructPcdTokenSpaceGuid.PcdPchSetup.SataPort[6]|0x1                                               # Port 6
gStructPcdTokenSpaceGuid.PcdPchSetup.SataPort[7]|0x1                                               # Port 7
gStructPcdTokenSpaceGuid.PcdPchSetup.SataRaidIooe|0x1                                              # IRRT Only on ESATA
gStructPcdTokenSpaceGuid.PcdPchSetup.SataRaidIrrt|0x1                                              # Intel Rapid Recovery Technology
gStructPcdTokenSpaceGuid.PcdPchSetup.SataRaidLoadEfiDriver[0]|0x0                                  # Load EFI Driver for RAID
gStructPcdTokenSpaceGuid.PcdPchSetup.SataRaidLoadEfiDriver[1]|0x0                                  # Load EFI Driver for RAID
gStructPcdTokenSpaceGuid.PcdPchSetup.SataRaidOromDelay|0x0                                         # RAID OROM prompt delay
gStructPcdTokenSpaceGuid.PcdPchSetup.SataRaidOub|0x1                                               # RAID Option ROM  UI banner
gStructPcdTokenSpaceGuid.PcdPchSetup.SataRaidR0|0x1                                                # RAID 0
gStructPcdTokenSpaceGuid.PcdPchSetup.SataRaidR1|0x1                                                # RAID 1
gStructPcdTokenSpaceGuid.PcdPchSetup.SataRaidR10|0x1                                               # RAID 10
gStructPcdTokenSpaceGuid.PcdPchSetup.SataRaidR5|0x1                                                # RAID 5
gStructPcdTokenSpaceGuid.PcdPchSetup.SataRaidSrt|0x1                                               # Smart Response Technology
gStructPcdTokenSpaceGuid.PcdPchSetup.SataSalp|0x1                                                  # Support Aggressive Link Power Management
gStructPcdTokenSpaceGuid.PcdPchSetup.SataSpinUp[0]|0x0                                             # Spin Up Device
gStructPcdTokenSpaceGuid.PcdPchSetup.SataSpinUp[1]|0x0                                             # Spin Up Device
gStructPcdTokenSpaceGuid.PcdPchSetup.SataSpinUp[2]|0x0                                             # Spin Up Device
gStructPcdTokenSpaceGuid.PcdPchSetup.SataSpinUp[3]|0x0                                             # Spin Up Device
gStructPcdTokenSpaceGuid.PcdPchSetup.SataSpinUp[4]|0x0                                             # Spin Up Device
gStructPcdTokenSpaceGuid.PcdPchSetup.SataSpinUp[5]|0x0                                             # Spin Up Device
gStructPcdTokenSpaceGuid.PcdPchSetup.SataSpinUp[6]|0x0                                             # Spin Up Device
gStructPcdTokenSpaceGuid.PcdPchSetup.SataSpinUp[7]|0x0                                             # Spin Up Device
gStructPcdTokenSpaceGuid.PcdPchSetup.SataTestMode|0x0                                              # SATA test mode
gStructPcdTokenSpaceGuid.PcdPchSetup.SataTopology[0]|0x0                                           # SATA Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.SataTopology[1]|0x0                                           # SATA Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.SataTopology[2]|0x0                                           # SATA Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.SataTopology[3]|0x0                                           # SATA Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.SataTopology[4]|0x0                                           # SATA Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.SataTopology[5]|0x0                                           # SATA Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.SataTopology[6]|0x0                                           # SATA Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.SataTopology[7]|0x0                                           # SATA Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.SataType[0]|0x0                                               # SATA Device Type
gStructPcdTokenSpaceGuid.PcdPchSetup.SataType[1]|0x0                                               # SATA Device Type
gStructPcdTokenSpaceGuid.PcdPchSetup.SataType[2]|0x0                                               # SATA Device Type
gStructPcdTokenSpaceGuid.PcdPchSetup.SataType[3]|0x0                                               # SATA Device Type
gStructPcdTokenSpaceGuid.PcdPchSetup.SataType[4]|0x0                                               # SATA Device Type
gStructPcdTokenSpaceGuid.PcdPchSetup.SataType[5]|0x0                                               # SATA Device Type
gStructPcdTokenSpaceGuid.PcdPchSetup.SataType[6]|0x0                                               # SATA Device Type
gStructPcdTokenSpaceGuid.PcdPchSetup.SataType[7]|0x0                                               # SATA Device Type
gStructPcdTokenSpaceGuid.PcdPchSetup.ShutdownPolicySelect|0x1                                      # Shutdown Policy
gStructPcdTokenSpaceGuid.PcdPchSetup.StateAfterG3|0x0                                              # PCH state after G3
gStructPcdTokenSpaceGuid.PcdPchSetup.TestDmiAspmCtrl|0x1                                           # PCH DMI ASPM Testing
gStructPcdTokenSpaceGuid.PcdPchSetup.TestMctpBroadcastCycle|0x0                                    # MCTP Broadcast Cycle
gStructPcdTokenSpaceGuid.PcdPchSetup.TestSmbusSpdWriteDisable|0x1                                  # SPD Write Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.ThermalDeviceEnable|0x3                                       # PCH Thermal Device
gStructPcdTokenSpaceGuid.PcdPchSetup.Usb3PinsTermination|0x1                                       # Enable USB 3.0 pins
gStructPcdTokenSpaceGuid.PcdPchSetup.UsbPrecondition|0x0                                           # USB Precondition
gStructPcdTokenSpaceGuid.PcdPchSetup.XTpmLen|0x1                                                   # Expanded SPI TPM Transaction Length Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.XhciDisMSICapability|0x0                                      # USB XHCI MSI Disable WA
gStructPcdTokenSpaceGuid.PcdPchSetup.XhciIdleL1|0x1                                                # XHCI Idle L1
gStructPcdTokenSpaceGuid.PcdPchSetup.XhciOcMapEnabled|0x1                                          # XHCI Over Current Pins
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataAlternateId|0x0                                          # Alternate Device ID on RAID
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataExternal[0]|0x0                                          # Configure as eSATA
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataExternal[1]|0x0                                          # Configure as eSATA
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataExternal[2]|0x0                                          # Configure as eSATA
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataExternal[3]|0x0                                          # Configure as eSATA
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataExternal[4]|0x0                                          # Configure as eSATA
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataExternal[5]|0x0                                          # Configure as eSATA
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataHddlk|0x1                                                # SATA HDD Unlock
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataHotPlug[0]|0x0                                           # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataHotPlug[1]|0x0                                           # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataHotPlug[2]|0x0                                           # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataHotPlug[3]|0x0                                           # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataHotPlug[4]|0x0                                           # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataHotPlug[5]|0x0                                           # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataInterfaceMode|0x0                                        # Configure sSATA as
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataLedl|0x1                                                 # SATA Led locate
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataPort[0]|0x1                                              # Port 0
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataPort[1]|0x1                                              # Port 1
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataPort[2]|0x1                                              # Port 2
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataPort[3]|0x1                                              # Port 3
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataPort[4]|0x1                                              # Port 4
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataPort[5]|0x1                                              # Port 5
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataRaidIooe|0x1                                             # IRRT Only on ESATA
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataRaidIrrt|0x1                                             # Intel Rapid Recovery Technology
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataRaidOromDelay|0x0                                        # RAID OROM prompt delay
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataRaidOub|0x1                                              # RAID Option ROM  UI banner
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataRaidR0|0x1                                               # RAID 0
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataRaidR1|0x1                                               # RAID 1
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataRaidR10|0x1                                              # RAID 10
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataRaidR5|0x1                                               # RAID 5
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataRaidSrt|0x1                                              # Smart Response Technology
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataSalp|0x1                                                 # Support Aggressive Link Power Management
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataSpinUp[0]|0x0                                            # Spin Up Device
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataSpinUp[1]|0x0                                            # Spin Up Device
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataSpinUp[2]|0x0                                            # Spin Up Device
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataSpinUp[3]|0x0                                            # Spin Up Device
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataSpinUp[4]|0x0                                            # Spin Up Device
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataSpinUp[5]|0x0                                            # Spin Up Device
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataTestMode|0x0                                             # SATA test mode
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataTopology[0]|0x0                                          # SATA Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataTopology[1]|0x0                                          # SATA Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataTopology[2]|0x0                                          # SATA Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataTopology[3]|0x0                                          # SATA Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataTopology[4]|0x0                                          # SATA Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataTopology[5]|0x0                                          # SATA Topology
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataType[0]|0x0                                              # sSATA Device Type
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataType[1]|0x0                                              # sSATA Device Type
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataType[2]|0x0                                              # sSATA Device Type
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataType[3]|0x0                                              # sSATA Device Type
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataType[4]|0x0                                              # sSATA Device Type
gStructPcdTokenSpaceGuid.PcdPchSetup.sSataType[5]|0x0                                              # sSATA Device Type
gStructPcdTokenSpaceGuid.PcdSetup.AEPErrorInjEn|0x0                                                # PMem Error Injection
gStructPcdTokenSpaceGuid.PcdSetup.ARIEnable|0x1                                                    # ARI Support
gStructPcdTokenSpaceGuid.PcdSetup.ARIForward|0x0                                                   # ARI Forward
gStructPcdTokenSpaceGuid.PcdSetup.AntiFlashWearoutSupported|0x0                                    # Anti Flash Wearout Supported
gStructPcdTokenSpaceGuid.PcdSetup.ApplicationProfile|0xff                                          # Application Profile Configuration
gStructPcdTokenSpaceGuid.PcdSetup.BaudRate|0x5
gStructPcdTokenSpaceGuid.PcdSetup.BiosGuardEnabled|0x0                                             # BIOS Guard
gStructPcdTokenSpaceGuid.PcdSetup.BiosGuardPlatformSupported|0x0                                   # Bios Guard Supported
gStructPcdTokenSpaceGuid.PcdSetup.BootAllOptions|0x0                                               # Boot All Options
gStructPcdTokenSpaceGuid.PcdSetup.BootNetwork|0x0                                                  # Boot to Network
gStructPcdTokenSpaceGuid.PcdSetup.Ce2LmLoggingEn|0x1                                               # 2LM Correctable Error Logging in m2mem
gStructPcdTokenSpaceGuid.PcdSetup.ClearViralStatus|0x0                                             # Clear Viral Status
gStructPcdTokenSpaceGuid.PcdSetup.CloakDevHideRegistersOs|0x0                                      # Cloak Devhide registers from being accessible from OS
gStructPcdTokenSpaceGuid.PcdSetup.CloakingEn|0x0                                                   # System Cloaking
gStructPcdTokenSpaceGuid.PcdSetup.ClrExtraTurboVoltage|0x0                                         # CLR Extra Turbo Voltage
gStructPcdTokenSpaceGuid.PcdSetup.ClrMaxOcRatio|0x0                                                # CLR Max OC Ratio
gStructPcdTokenSpaceGuid.PcdSetup.ClrVoltageMode|0x0                                               # CLR Voltage Mode
gStructPcdTokenSpaceGuid.PcdSetup.ClrVoltageOffset|0x0                                             # CLR Voltage Offset
gStructPcdTokenSpaceGuid.PcdSetup.ClrVoltageOffsetPrefix|0x0                                       # Offset Prefix
gStructPcdTokenSpaceGuid.PcdSetup.ClrVoltageOverride|0x0                                           # CLR Voltage Override
gStructPcdTokenSpaceGuid.PcdSetup.ConsoleRedirection|0x1                                           # Console Redirection
gStructPcdTokenSpaceGuid.PcdSetup.CoreExtraTurboVoltage|0x0                                        # Core Extra Turbo Voltage
gStructPcdTokenSpaceGuid.PcdSetup.CoreMaxOcRatio|0x0                                               # Core Max OC Ratio
gStructPcdTokenSpaceGuid.PcdSetup.CoreVoltageMode|0x0                                              # Core Voltage Mode
gStructPcdTokenSpaceGuid.PcdSetup.CoreVoltageOffset|0x0                                            # Core Voltage Offset
gStructPcdTokenSpaceGuid.PcdSetup.CoreVoltageOffsetPrefix|0x0                                      # Offset Prefix
gStructPcdTokenSpaceGuid.PcdSetup.CoreVoltageOverride|0x0                                          # Core Voltage Override
gStructPcdTokenSpaceGuid.PcdSetup.CorrMemErrEn|0x1                                                 # Memory Corrected Error
gStructPcdTokenSpaceGuid.PcdSetup.CpuVccInVoltage|0x167                                            # CPU VCCin Voltage Level
gStructPcdTokenSpaceGuid.PcdSetup.CsmiDynamicDisable|0x1                                           # CSMI Dynamic Disable
gStructPcdTokenSpaceGuid.PcdSetup.DataBits|0x8                                                     # Data Bits
gStructPcdTokenSpaceGuid.PcdSetup.DcpmmUncPoison|0x1                                               # PMem UNC Poison
gStructPcdTokenSpaceGuid.PcdSetup.DdrtInternalAlertEn|0x1                                          # PMem Host Alert Policy
gStructPcdTokenSpaceGuid.PcdSetup.ReservedS2|0x2
gStructPcdTokenSpaceGuid.PcdSetup.ReservedS1|0x0
gStructPcdTokenSpaceGuid.PcdSetup.ReservedS3|0x2
gStructPcdTokenSpaceGuid.PcdSetup.DisableMAerrorLoggingDueToLER|0x1                                # LER MA Error Logging
gStructPcdTokenSpaceGuid.PcdSetup.EdpcEn|0x0                                                       # IIO eDPC Support
gStructPcdTokenSpaceGuid.PcdSetup.EdpcErrCorMsg|0x1                                                # IIO eDPC ERR_COR Message
gStructPcdTokenSpaceGuid.PcdSetup.EdpcInterrupt|0x1                                                # IIO eDPC Interrupt
gStructPcdTokenSpaceGuid.PcdSetup.EfiNetworkSupport|0x0                                            # EFI Network
gStructPcdTokenSpaceGuid.PcdSetup.ElogCorrErrEn|0x1                                                # Corrected Error eLog
gStructPcdTokenSpaceGuid.PcdSetup.ElogMemErrEn|0x1                                                 # Memory Error eLog
gStructPcdTokenSpaceGuid.PcdSetup.ElogProcErrEn|0x1                                                # Processor Error eLog
gStructPcdTokenSpaceGuid.PcdSetup.EmcaCsmiEn|0x2                                                   # EMCA CMCI-SMI Morphing
gStructPcdTokenSpaceGuid.PcdSetup.EmcaCsmiThreshold|0x0                                            # EMCA CMCI-SMI Threshold
gStructPcdTokenSpaceGuid.PcdSetup.EmcaEn|0x1                                                       # EMCA Logging Support
gStructPcdTokenSpaceGuid.PcdSetup.EmcaIgnOptin|0x0                                                 # Ignore OS EMCA Opt-in
gStructPcdTokenSpaceGuid.PcdSetup.EmcaMsmiEn|0x2                                                   # EMCA MCE-SMI Enable
gStructPcdTokenSpaceGuid.PcdSetup.EnableAntiFlashWearout|0x0                                       # Flash Wear Out Protection
gStructPcdTokenSpaceGuid.PcdSetup.ExpectedBer|0x7ffffffff                                          # Expected BER
gStructPcdTokenSpaceGuid.PcdSetup.FanPwmOffset|0x0                                                 # Fan PWM Offset
gStructPcdTokenSpaceGuid.PcdSetup.FatalErrSpinLoopEn|0x0                                           # FatalErrDebugHalt
gStructPcdTokenSpaceGuid.PcdSetup.FilterPll|0x0                                                    # Filter Pll
gStructPcdTokenSpaceGuid.PcdSetup.FivrEfficiencyEnable|0x1                                         # FIVR Efficiency Management
gStructPcdTokenSpaceGuid.PcdSetup.FivrFaultsEnable|0x1                                             # FIVR Faults
gStructPcdTokenSpaceGuid.PcdSetup.FlowControl|0x0                                                  # Flow Control
gStructPcdTokenSpaceGuid.PcdSetup.FnvErrorEn|0x1                                                   # PMem CTLR Errors
gStructPcdTokenSpaceGuid.PcdSetup.FnvErrorHighPrioritySignal|0x1                                   # PMem CTLR High Priority Error Signaling
gStructPcdTokenSpaceGuid.PcdSetup.FnvErrorLowPrioritySignal|0x1                                    # PMem CTLR Low Priority Error Signaling
gStructPcdTokenSpaceGuid.PcdSetup.ForceSetup|0x0                                                   # Force Setup
gStructPcdTokenSpaceGuid.PcdSetup.FpkPortConfig[0]|0x1                                             # FPK Port 1
gStructPcdTokenSpaceGuid.PcdSetup.FpkPortConfig[1]|0x1                                             # FPK Port 2
gStructPcdTokenSpaceGuid.PcdSetup.FpkPortConfig[2]|0x1                                             # FPK Port 3
gStructPcdTokenSpaceGuid.PcdSetup.FpkPortConfig[3]|0x1                                             # FPK Port 4
gStructPcdTokenSpaceGuid.PcdSetup.GbeEnabled|0x0                                                   # PCH Internal LAN
gStructPcdTokenSpaceGuid.PcdSetup.GbePciePortNum|0x0                                               # PCIE Port assigned to LAN
gStructPcdTokenSpaceGuid.PcdSetup.Gen12ErrorThreshold|0x0                                          # Error Threshold (Gen1/2)
gStructPcdTokenSpaceGuid.PcdSetup.Gen12TimeWindow|0xffff                                           # Time Window (Gen1/2)
gStructPcdTokenSpaceGuid.PcdSetup.Gen2LinkDegradation|0x1                                          # Gen2 Link Degradation
gStructPcdTokenSpaceGuid.PcdSetup.Gen34ErrorThreshold|0x10                                         # Error Threshold (Gen3/4)
gStructPcdTokenSpaceGuid.PcdSetup.Gen34ReEqualization|0x1                                          # Gen3/4 Re-Equalization
gStructPcdTokenSpaceGuid.PcdSetup.Gen34TimeWindow|0x2                                              # Time Window (Gen3/4)
gStructPcdTokenSpaceGuid.PcdSetup.Gen3LinkDegradation|0x1                                          # Gen3 Link Degradation
gStructPcdTokenSpaceGuid.PcdSetup.Gen4LinkDegradation|0x1                                          # Gen4 Link Degradation
gStructPcdTokenSpaceGuid.PcdSetup.HideWriteDataParityLogs|0x1                                      # Hide Data Parity Error Logs
gStructPcdTokenSpaceGuid.PcdSetup.IioDmaErrorEn|0x1                                                # IIO Dma Error
gStructPcdTokenSpaceGuid.PcdSetup.IioDmiErrorEn|0x1                                                # IIO Dmi Error
gStructPcdTokenSpaceGuid.PcdSetup.IioErrRegistersClearEn|0x1                                       # IIO Error Registers Clear
gStructPcdTokenSpaceGuid.PcdSetup.IioErrorEn|0x1                                                   # IIO/PCH Global Error Support
gStructPcdTokenSpaceGuid.PcdSetup.IioErrorPin0En|0x0                                               # IIO Error Pin0 Enable
gStructPcdTokenSpaceGuid.PcdSetup.IioErrorPin1En|0x0                                               # IIO Error Pin1 Enable
gStructPcdTokenSpaceGuid.PcdSetup.IioErrorPin2En|0x0                                               # IIO Error Pin2 Enable
gStructPcdTokenSpaceGuid.PcdSetup.IioIrpErrorEn|0x1                                                # IIO Coherent Interface Error
gStructPcdTokenSpaceGuid.PcdSetup.IioMiscErrorEn|0x1                                               # IIO Misc. Error
gStructPcdTokenSpaceGuid.PcdSetup.IioOOBMode|0x1                                                   # IIO OOB Mode
gStructPcdTokenSpaceGuid.PcdSetup.IioPcieAddCorrErrorEn|0x1                                        # IIO PCIE Additional Corrected Error
gStructPcdTokenSpaceGuid.PcdSetup.IioPcieAddRcvComWithUr|0x0                                       # IIO PCIE Additional Received Completion With UR
gStructPcdTokenSpaceGuid.PcdSetup.IioPcieAddUnCorrEn|0x1                                           # IIO PCIE Additional Uncorrected Error
gStructPcdTokenSpaceGuid.PcdSetup.IioPcieAerSpecCompEn|0x0                                         # IIO PCIE AER Spec Compliant
gStructPcdTokenSpaceGuid.PcdSetup.IioSev1Pcc|0x0                                                   # Clear PCC for IIO Non-Fatal Error
gStructPcdTokenSpaceGuid.PcdSetup.IioVtdErrorEn|0x1                                                # IIO Vtd Error
gStructPcdTokenSpaceGuid.PcdSetup.IoMcaEn|0x1                                                      # IIO MCA Support
gStructPcdTokenSpaceGuid.PcdSetup.IoaVoltageOffset|0x0                                             # IOA Voltage Offset
gStructPcdTokenSpaceGuid.PcdSetup.IoaVoltageOffsetPrefix|0x0                                       # Offset Prefix
gStructPcdTokenSpaceGuid.PcdSetup.IodVoltageOffset|0x0                                             # IOD Voltage Offset
gStructPcdTokenSpaceGuid.PcdSetup.IodVoltageOffsetPrefix|0x0                                       # Offset Prefix
gStructPcdTokenSpaceGuid.PcdSetup.ItcOtcCaMaEnable|0x0                                             # ITC/OTC CA/MA Errors
gStructPcdTokenSpaceGuid.PcdSetup.KTIFailoverSmiEn|0x0
gStructPcdTokenSpaceGuid.PcdSetup.KcsAccessPolicy|0x3                                              # KCS Access Control Policy
gStructPcdTokenSpaceGuid.PcdSetup.KtiFirstCeLatchEn|0x0                                            # Latch First Corrected Error in KTI
gStructPcdTokenSpaceGuid.PcdSetup.LegacyOsRedirection|0x1                                          # Legacy OS Redirection
gStructPcdTokenSpaceGuid.PcdSetup.LegacyPxeRom|0x0                                                 # Legacy Option ROMs support
gStructPcdTokenSpaceGuid.PcdSetup.LerEn|0x0                                                        # IIO LER Support
gStructPcdTokenSpaceGuid.PcdSetup.LmceEn|0x1                                                       # LMCE Support
gStructPcdTokenSpaceGuid.PcdSetup.LomDisableByGpio|0x1                                             # LOM
gStructPcdTokenSpaceGuid.PcdSetup.MRIOVEnable|0x0                                                  # MR-IOV Support
gStructPcdTokenSpaceGuid.PcdSetup.McBankWarmBootClearError|0x1                                     # Mca Bank Warm Boot Clear Errors
gStructPcdTokenSpaceGuid.PcdSetup.McaBankErrInjEn|0x0                                              # Mca Bank Error Injection Support
gStructPcdTokenSpaceGuid.PcdSetup.McaSpinLoop|0x0                                                  # MCA Spin Loop
gStructPcdTokenSpaceGuid.PcdSetup.MeSegErrorInjEn|0x0                                              # ME Seg Error Injection Support
gStructPcdTokenSpaceGuid.PcdSetup.MemErrEn|0x1                                                     # Memory Error
gStructPcdTokenSpaceGuid.PcdSetup.NgnAddressRangeScrub|0x0                                         # Set PMem Address Range Scrub
gStructPcdTokenSpaceGuid.PcdSetup.NgnHostAlertDpa|0x0                                              # Set PMem Host Alert Policy for DPA Error
gStructPcdTokenSpaceGuid.PcdSetup.NgnHostAlertPatrolScrubUNC|0x1                                   # Set PMem Host Alert Policy for Patrol Scrub
gStructPcdTokenSpaceGuid.PcdSetup.Numlock|0x0                                                      # NumLock
gStructPcdTokenSpaceGuid.PcdSetup.OsNativeAerSupport|0x0                                           # Os Native AER Support
gStructPcdTokenSpaceGuid.PcdSetup.OverclockingSupport|0x0                                          # OverClocking Feature
gStructPcdTokenSpaceGuid.PcdSetup.Parity|0x1                                                       # Parity
gStructPcdTokenSpaceGuid.PcdSetup.PatrolScrubErrorReporting|0x1                                    # Patrol Scrub Error Reporting
gStructPcdTokenSpaceGuid.PcdSetup.PchStepping|0x0                                                  # PCH Stepping
gStructPcdTokenSpaceGuid.PcdSetup.PcieAerAdNfatErrEn|0x1                                           # PCIE AER Advisory Nonfatal Error
gStructPcdTokenSpaceGuid.PcdSetup.PcieAerCorrErrEn|0x1                                             # PCIE AER Corrected Errors
gStructPcdTokenSpaceGuid.PcdSetup.PcieAerEcrcEn|0x1
gStructPcdTokenSpaceGuid.PcdSetup.PcieAerFatErrEn|0x1                                              # PCIE AER Fatal Error
gStructPcdTokenSpaceGuid.PcdSetup.PcieAerNfatErrEn|0x1                                             # PCIE AER NonFatal Error
gStructPcdTokenSpaceGuid.PcdSetup.PcieAerSurpriseLinkDownEn|0x0                                    # PCIE Surprise Link Down Error
gStructPcdTokenSpaceGuid.PcdSetup.PcieAerUreEn|0x0                                                 # PCIE Unsupported Request Error
gStructPcdTokenSpaceGuid.PcdSetup.PcieClockGatingDisabled|0x0                                      # PCIE Clock Gating
gStructPcdTokenSpaceGuid.PcdSetup.PcieCorErrCntr|0x0                                               # PCIE Corrected Error Threshold Counter
gStructPcdTokenSpaceGuid.PcdSetup.PcieCorErrLimit|0x50                                             # PCIe Corrected Error Limit
gStructPcdTokenSpaceGuid.PcdSetup.PcieCorErrLimitEn|0x0                                            # PCIe Corrected Error Limit Check
gStructPcdTokenSpaceGuid.PcdSetup.PcieCorErrThres|0x1                                              # PCIE Corrected Error Threshold
gStructPcdTokenSpaceGuid.PcdSetup.PcieCorrErrEn|0x1                                                # Corrected Error
gStructPcdTokenSpaceGuid.PcdSetup.PcieDmiAspm|0x0                                                  # PCH DMI ASPM
gStructPcdTokenSpaceGuid.PcdSetup.PcieErrEn|0x1                                                    # PCIE Error
gStructPcdTokenSpaceGuid.PcdSetup.PcieErrInjActionTable|0x0                                        # Whea PCIe Error Injection Action Table
gStructPcdTokenSpaceGuid.PcdSetup.PcieFatalErrEn|0x1                                               # Fatal Error Enable
gStructPcdTokenSpaceGuid.PcdSetup.PcieUncorrErrEn|0x1                                              # Uncorrected Error
gStructPcdTokenSpaceGuid.PcdSetup.PfrBmcPfmActiveMajorVersion|0x0                                  # BMC PFM Active Major Version
gStructPcdTokenSpaceGuid.PcdSetup.PfrBmcPfmActiveMinorVersion|0x0                                  # BMC PFM Active Minor Version
gStructPcdTokenSpaceGuid.PcdSetup.PfrBmcPfmRecoveryMajorVersion|0x0                                # BMC PFM Recovery Major Version
gStructPcdTokenSpaceGuid.PcdSetup.PfrBmcPfmRecoveryMinorVersion|0x0                                # BMC PFM Recovery Minor Version
gStructPcdTokenSpaceGuid.PcdSetup.PfrBmcPfrActiveSvn|0x0                                           # BMC PFR Active SVN
gStructPcdTokenSpaceGuid.PcdSetup.PfrBmcPfrRecoverySvn|0x0                                         # BMC PFR Recovery SVN
gStructPcdTokenSpaceGuid.PcdSetup.PfrCpldRotReleaseVersion|0x0                                     # CPLD RoT Release Version
gStructPcdTokenSpaceGuid.PcdSetup.PfrCpldRotSvn|0x0                                                # CPLD RoT SVN
gStructPcdTokenSpaceGuid.PcdSetup.PfrLock|0x0                                                      # PFR Lock
gStructPcdTokenSpaceGuid.PcdSetup.PfrLockStatus|0x0                                                # PFR Status: Locked
gStructPcdTokenSpaceGuid.PcdSetup.PfrPchPfmActiveMajorVersion|0x0                                  # PCH PFM Active Major Version
gStructPcdTokenSpaceGuid.PcdSetup.PfrPchPfmActiveMinorVersion|0x0                                  # PCH PFM Active Minor Version
gStructPcdTokenSpaceGuid.PcdSetup.PfrPchPfmRecoveryMajorVersion|0x0                                # PCH PFM Recovery Major Version
gStructPcdTokenSpaceGuid.PcdSetup.PfrPchPfmRecoveryMinorVersion|0x0                                # PCH PFM Recovery Minor Version
gStructPcdTokenSpaceGuid.PcdSetup.PfrPchPfrActiveSvn|0x0                                           # PCH PFR Active SVN
gStructPcdTokenSpaceGuid.PcdSetup.PfrPchPfrRecoverySvn|0x0                                         # PCH PFR Recovery SVN
gStructPcdTokenSpaceGuid.PcdSetup.PfrPitL1|0x0                                                     # PFR PIT Level-1 Protection
gStructPcdTokenSpaceGuid.PcdSetup.PfrPitL1Status|0x0                                               # PFR Status: PIT Level-1 Protection Enabled
gStructPcdTokenSpaceGuid.PcdSetup.PfrPitL2|0x0                                                     # PFR PIT Level-2 Protection
gStructPcdTokenSpaceGuid.PcdSetup.PfrPitL2Status|0x0                                               # PFR Status: PIT Level-2 Protection Enabled
gStructPcdTokenSpaceGuid.PcdSetup.PfrProvision|0x0                                                 # PFR Provision
gStructPcdTokenSpaceGuid.PcdSetup.PfrProvisionStatus|0x0                                           # PFR Status: Provisioned
gStructPcdTokenSpaceGuid.PcdSetup.PfrSupported|0x0                                                 # PFR Supported
gStructPcdTokenSpaceGuid.PcdSetup.PfrUnProvision|0x0                                               # PFR UnProvision
gStructPcdTokenSpaceGuid.PcdSetup.PlatformOCSupport|0x0                                            # Platform Over Clocking Support
gStructPcdTokenSpaceGuid.PcdSetup.PmsbRouterParityErrEn|0x1                                        # PMSB Router Parity Error
gStructPcdTokenSpaceGuid.PcdSetup.PoisonEn|0x1                                                     # System Memory Poison
gStructPcdTokenSpaceGuid.PcdSetup.PropagatePerr|0x1                                                # Assert NMI on PERR
gStructPcdTokenSpaceGuid.PcdSetup.PropagateSerr|0x1                                                # Assert NMI on SERR
gStructPcdTokenSpaceGuid.PcdSetup.Ps2PortSwap|0x0                                                  # PS2 Port Swap
gStructPcdTokenSpaceGuid.PcdSetup.PsfUrEnable|0x1                                                  # PSF UR Error
gStructPcdTokenSpaceGuid.PcdSetup.PublishSetupPgPtr|0x0                                            # Publish Setup page Pointer
gStructPcdTokenSpaceGuid.PcdSetup.RTCWakeupTimeHour|0x0                                            # Wake up hour
gStructPcdTokenSpaceGuid.PcdSetup.RTCWakeupTimeMinute|0x0                                          # Wake up minute
gStructPcdTokenSpaceGuid.PcdSetup.RTCWakeupTimeSecond|0x0                                          # Wake up second
gStructPcdTokenSpaceGuid.PcdSetup.RasLogLevel|0x1                                                  # RAS Log Level
gStructPcdTokenSpaceGuid.PcdSetup.ReportAlertSPA|0x1                                               # Enable Reporting SPA to OS
gStructPcdTokenSpaceGuid.PcdSetup.ReserveMem|0x0                                                   # Reserve Memory Range
gStructPcdTokenSpaceGuid.PcdSetup.ReserveStartAddr|0x100000                                        # Start Address
gStructPcdTokenSpaceGuid.PcdSetup.ResetOnMemMapChange|0x0                                          # Reset Platform on Memory Map Change
gStructPcdTokenSpaceGuid.PcdSetup.RsaSupport|0x0                                                   # RSA Support
gStructPcdTokenSpaceGuid.PcdSetup.SRIOVEnable|0x1                                                  # SR-IOV Support
gStructPcdTokenSpaceGuid.PcdSetup.SerialBaudRate|0x1c200                                           # BIOS Serial Port Baud Rate
gStructPcdTokenSpaceGuid.PcdSetup.SetShellFirst|0x0                                                # Boot Shell First
gStructPcdTokenSpaceGuid.PcdSetup.SgxErrorInjEn|0x0                                                # SGX Memory Error Injection Support
gStructPcdTokenSpaceGuid.PcdSetup.ShellEntryTime|0x5                                               # System Shell Timeout
gStructPcdTokenSpaceGuid.PcdSetup.ShutdownSuppression|0x0                                          # Shutdown Suppression
gStructPcdTokenSpaceGuid.PcdSetup.SkipXmlComprs|0x0                                                # Skip XML Compression
gStructPcdTokenSpaceGuid.PcdSetup.SpareIntSelect|0x1                                               # Spare Interrupt
gStructPcdTokenSpaceGuid.PcdSetup.StopBits|0x1                                                     # Stop Bits
gStructPcdTokenSpaceGuid.PcdSetup.StorageOpROMSuppression|0x0                                      # Storage OPROM Suppression
gStructPcdTokenSpaceGuid.PcdSetup.SvidEnable|0x1                                                   # SVID Support
gStructPcdTokenSpaceGuid.PcdSetup.SvidVoltageOverride|0x0                                          # SVID Voltage Override
gStructPcdTokenSpaceGuid.PcdSetup.SystemErrorEn|0x1                                                # System Errors
gStructPcdTokenSpaceGuid.PcdSetup.SystemPageSize|0x0                                               # SR-IOV SystemPageSize
gStructPcdTokenSpaceGuid.PcdSetup.TagecMem|0x0                                                     # Reserve TAGEC Memory
gStructPcdTokenSpaceGuid.PcdSetup.TerminalResolution|0x0                                           # Terminal Resolution
gStructPcdTokenSpaceGuid.PcdSetup.TerminalType|0x2                                                 # Terminal Type
gStructPcdTokenSpaceGuid.PcdSetup.UboxErrorMask|0x0                                                # Ubox Error Mask
gStructPcdTokenSpaceGuid.PcdSetup.UboxToPcuMcaEn|0x1                                               # UboxToPcuMca Enabling
gStructPcdTokenSpaceGuid.PcdSetup.UncoreVoltageOffset|0x0                                          # Uncore Voltage Offset
gStructPcdTokenSpaceGuid.PcdSetup.UncoreVoltageOffsetPrefix|0x0                                    # Offset Prefix
gStructPcdTokenSpaceGuid.PcdSetup.UsbEmul6064|0x0                                                  # Port 60/64 Emulation
gStructPcdTokenSpaceGuid.PcdSetup.UsbLegacySupport|0x0                                             # Legacy USB Support
gStructPcdTokenSpaceGuid.PcdSetup.UsbMassResetDelay|0x1                                            # Device Reset timeout
gStructPcdTokenSpaceGuid.PcdSetup.UsbNonBoot|0x0                                                   # Make USB Devices Non-Bootable
gStructPcdTokenSpaceGuid.PcdSetup.Use1GPageTable|0x1                                               # Max Page Table Size Select
gStructPcdTokenSpaceGuid.PcdSetup.ValidationBreakpointType|0x0                                     # Breakpoint Type
gStructPcdTokenSpaceGuid.PcdSetup.VccIoVoltage|0x0                                                 # VccIo Voltage Control
gStructPcdTokenSpaceGuid.PcdSetup.VideoSelect|0x0                                                  # Active Video
gStructPcdTokenSpaceGuid.PcdSetup.ViralEn|0x0                                                      # Viral Status
gStructPcdTokenSpaceGuid.PcdSetup.WakeOnLanS5|0x0                                                  # Wake On Lan from S5
gStructPcdTokenSpaceGuid.PcdSetup.WakeOnLanSupport|0x0                                             # Wake On Lan Support
gStructPcdTokenSpaceGuid.PcdSetup.WakeOnRTCS4S5|0x0                                                # RTC Wake system from S4/S5
gStructPcdTokenSpaceGuid.PcdSetup.WheaErrInjEn|0x1                                                 # WHEA Error Injection 5.0 Extension
gStructPcdTokenSpaceGuid.PcdSetup.WheaErrorInjSupportEn|0x0                                        # WHEA Error Injection Support
gStructPcdTokenSpaceGuid.PcdSetup.WheaLogMemoryEn|0x1                                              # Whea Log Memory Error
gStructPcdTokenSpaceGuid.PcdSetup.WheaLogPciEn|0x1                                                 # Whea Log PCI Error
gStructPcdTokenSpaceGuid.PcdSetup.WheaLogProcEn|0x1                                                # Whea Log Processor Error
gStructPcdTokenSpaceGuid.PcdSetup.WheaPcieErrInjEn|0x0                                             # Whea PCIE Error Injection Support
gStructPcdTokenSpaceGuid.PcdSetup.WheaSupportEn|0x1                                                # WHEA Support
gStructPcdTokenSpaceGuid.PcdSetup.XhciWakeOnUsbEnabled|0x1                                         # XHCI Wake On Usb Enable
gStructPcdTokenSpaceGuid.PcdSetup.XmlCliSupport|0x1                                                # Xml Cli Support
gStructPcdTokenSpaceGuid.PcdSetup.bsdBreakpoint|0x0                                                # BsdBreakPoint
gStructPcdTokenSpaceGuid.PcdSetup.irpp0_crreccCs0|0x1                                              # IIO IRP0 wrcache correcccs0 error
gStructPcdTokenSpaceGuid.PcdSetup.irpp0_crreccCs1|0x1                                              # IIO IRP0 wrcache correcccs1 error
gStructPcdTokenSpaceGuid.PcdSetup.irpp0_csraccunaligned|0x1                                        # IIO IRP0 csr acc 32b unaligned
gStructPcdTokenSpaceGuid.PcdSetup.irpp0_parityError|0x1                                            # IIO IRP0 protocol parity error
gStructPcdTokenSpaceGuid.PcdSetup.irpp0_qtOverflow|0x1                                             # IIO IRP0 protocol qt overflow underflow error
gStructPcdTokenSpaceGuid.PcdSetup.irpp0_rcvdpoison|0x1                                             # IIO IRP0 protocol rcvd poison error
gStructPcdTokenSpaceGuid.PcdSetup.irpp0_unceccCs0|0x1                                              # IIO IRP0 wrcache uncecccs0 error
gStructPcdTokenSpaceGuid.PcdSetup.irpp0_unceccCs1|0x1                                              # IIO IRP0 wrcache uncecccs1 error
gStructPcdTokenSpaceGuid.PcdSetup.irpp0_unexprsp|0x1                                               # IIO IRP0 protocol rcvd unexprsp
gStructPcdTokenSpaceGuid.PcdSetup.serialDebugTrace|0x0                                             # Trace Messages
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS8|0x0
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[0]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[1]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[2]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[3]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[4]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[5]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[6]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[7]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[8]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[9]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[10]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[11]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[12]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[13]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[14]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[15]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[16]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[17]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[18]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[19]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[20]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[21]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[22]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[23]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[24]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[25]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[26]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[27]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[28]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[29]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[30]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS7[31]|0xff
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.ReservedS6|0xf
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.IsocEn|0x2                                        # Isoc Mode
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.MirrorMode|0x0                                    # Mirror Mode
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.MmcfgBase|0x6                                     # MMCFG Base
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.MmcfgSize|0x6                                     # MMCFG Size
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.MmiohBase|0x2                                     # MMIO High Base
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.MmiohSize|0x3                                     # MMIO High Granularity Size
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.NumaEn|0x1                                        # Numa
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.SystemRasType|0x0                                 # System RAS Type
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.UmaBasedClustering|0x2                            # UMA-Based Clustering
gStructPcdTokenSpaceGuid.PcdSocketCommonRcConfig.VirtualNumaEnable|0x0                             # Virtual Numa
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[1]|0x0                                          # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[2]|0x0                                          # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[3]|0x0                                          # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[4]|0x0                                          # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[5]|0x0                                          # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[6]|0x0                                          # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[7]|0x0                                          # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[8]|0x0                                          # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[9]|0x0                                          # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[10]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[11]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[12]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[13]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[14]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[15]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[16]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[17]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[18]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[19]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[20]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[21]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[22]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[23]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[24]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[25]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[26]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[27]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[28]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[29]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[30]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[31]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[32]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[33]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[34]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[35]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[36]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[37]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[38]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[39]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[40]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[41]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[42]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[43]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[44]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[45]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[46]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[47]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[48]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[49]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[50]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[51]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[52]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[53]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[54]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[55]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[56]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[57]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[58]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[59]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[60]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[61]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[62]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[63]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[64]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[65]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[66]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[67]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[68]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[69]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[70]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[71]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[72]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[73]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[74]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[75]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[76]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[77]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[78]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[79]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[80]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[81]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[82]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIHP[83]|0x0                                         # HotPlug ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[1]|0x0                                          # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[2]|0x0                                          # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[3]|0x0                                          # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[4]|0x0                                          # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[5]|0x0                                          # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[6]|0x0                                          # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[7]|0x0                                          # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[8]|0x0                                          # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[9]|0x0                                          # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[10]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[11]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[12]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[13]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[14]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[15]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[16]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[17]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[18]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[19]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[20]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[21]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[22]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[23]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[24]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[25]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[26]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[27]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[28]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[29]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[30]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[31]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[32]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[33]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[34]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[35]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[36]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[37]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[38]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[39]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[40]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[41]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[42]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[43]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[44]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[45]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[46]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[47]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[48]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[49]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[50]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[51]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[52]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[53]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[54]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[55]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[56]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[57]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[58]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[59]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[60]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[61]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[62]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[63]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[64]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[65]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[66]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[67]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[68]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[69]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[70]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[71]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[72]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[73]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[74]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[75]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[76]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[77]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[78]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[79]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[80]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[81]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[82]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPM[83]|0x0                                         # PM ACPI Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[0]|0x0                                        # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[1]|0x0                                        # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[2]|0x0                                        # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[3]|0x0                                        # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[4]|0x0                                        # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[5]|0x0                                        # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[6]|0x0                                        # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[7]|0x0                                        # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[8]|0x0                                        # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[9]|0x0                                        # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[10]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[11]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[12]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[13]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[14]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[15]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[16]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[17]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[18]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[19]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[20]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[21]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[22]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[23]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[24]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[25]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[26]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[27]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[28]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[29]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[30]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[31]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[32]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[33]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[34]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[35]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[36]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[37]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[38]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[39]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[40]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[41]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[42]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[43]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[44]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[45]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[46]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[47]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[48]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[49]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[50]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[51]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[52]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[53]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[54]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[55]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[56]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[57]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[58]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[59]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[60]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[61]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[62]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[63]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[64]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[65]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[66]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[67]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[68]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[69]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[70]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[71]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[72]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[73]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[74]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[75]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[76]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[77]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[78]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[79]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[80]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[81]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[82]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ACPIPMEn[83]|0x0                                       # ACPI PME Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ATS|0x1                                                # ATS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[0]|0x0                                   # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[1]|0x0                                   # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[2]|0x0                                   # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[3]|0x0                                   # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[4]|0x0                                   # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[5]|0x0                                   # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[6]|0x0                                   # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[7]|0x0                                   # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[8]|0x0                                   # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[9]|0x0                                   # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[10]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[11]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[12]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[13]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[14]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[15]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[16]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[17]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[18]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[19]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[20]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[21]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[22]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[23]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[24]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[25]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[26]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[27]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[28]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[29]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[30]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[31]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[32]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[33]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[34]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[35]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[36]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[37]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[38]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[39]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[40]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[41]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[42]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[43]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[44]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[45]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[46]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[47]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[48]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[49]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[50]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[51]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[52]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[53]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[54]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[55]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[56]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[57]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[58]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[59]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[60]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[61]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[62]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[63]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[64]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[65]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[66]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[67]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[68]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[69]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[70]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[71]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[72]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[73]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[74]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[75]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[76]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[77]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[78]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[79]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[80]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[81]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[82]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.AltAttenTable[83]|0x0                                  # Alt ATTEN Table
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[0]|0x1                                        # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[1]|0x1                                        # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[2]|0x1                                        # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[3]|0x1                                        # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[4]|0x1                                        # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[5]|0x1                                        # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[6]|0x1                                        # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[7]|0x1                                        # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[8]|0x1                                        # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[9]|0x1                                        # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[10]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[11]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[12]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[13]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[14]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[15]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[16]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[17]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[18]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[19]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[20]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[21]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[22]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[23]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[24]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[25]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[26]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[27]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[28]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[29]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[30]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3DmaEn[31]|0x1                                       # DMA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[0]|0x0                                    # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[1]|0x0                                    # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[2]|0x0                                    # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[3]|0x0                                    # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[4]|0x0                                    # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[5]|0x0                                    # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[6]|0x0                                    # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[7]|0x0                                    # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[8]|0x0                                    # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[9]|0x0                                    # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[10]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[11]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[12]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[13]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[14]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[15]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[16]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[17]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[18]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[19]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[20]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[21]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[22]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[23]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[24]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[25]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[26]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[27]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[28]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[29]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[30]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Cb3NoSnoopEn[31]|0x0                                   # No Snoop
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.CbDmaMultiCastEnable|0x1                               # CbDma MultiCast Enable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.CbRelaxedOrdering|0x0                                  # Relaxed Ordering
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.CoherencySupport|0x1                                   # Coherency Support (Non-Isoch)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.CoherentReadFull|0x0                                   # PCIE Coherent Read Full
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.CoherentReadPart|0x0                                   # PCIE Coherent Read Partial
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.CompletionTimeoutGlobal|0x1                            # PCI-E Completion Timeout
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.CompletionTimeoutGlobalValue|0x9                       # PCI-E Completion Timeout
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.CompletionTimeoutValue[0]|0x9                          # PCI-E Completion Timeout
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.CompletionTimeoutValue[1]|0x9                          # PCI-E Completion Timeout
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.CompletionTimeoutValue[2]|0x9                          # PCI-E Completion Timeout
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.CompletionTimeoutValue[3]|0x9                          # PCI-E Completion Timeout
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[0]|0x0                                  # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[1]|0x0                                  # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[2]|0x0                                  # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[3]|0x0                                  # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[4]|0x0                                  # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[5]|0x0                                  # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[6]|0x0                                  # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[7]|0x0                                  # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[8]|0x0                                  # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[9]|0x0                                  # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[10]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[11]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[12]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[13]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[14]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[15]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[16]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[17]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[18]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[19]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[20]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[21]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[22]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[23]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[24]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[25]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[26]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[27]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[28]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[29]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[30]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[31]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[32]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[33]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[34]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[35]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[36]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[37]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[38]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[39]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[40]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[41]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[42]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[43]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[44]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[45]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[46]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[47]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[48]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[49]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[50]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[51]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[52]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[53]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[54]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[55]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[56]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[57]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[58]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[59]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[60]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[61]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[62]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[63]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[64]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[65]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[66]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[67]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[68]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[69]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[70]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[71]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[72]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[73]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[74]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[75]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[76]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[77]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[78]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[79]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[80]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[81]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[82]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ComplianceMode[83]|0x0                                 # Compliance Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ConfigIOU0[0]|0xff                                     # IOU0 (IIO PCIe Port 1)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ConfigIOU0[1]|0xff                                     # IOU0 (IIO PCIe Port 1)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ConfigIOU0[2]|0xff                                     # IOU0 (IIO PCIe Port 1)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ConfigIOU0[3]|0xff                                     # IOU0 (IIO PCIe Port 1)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ConfigIOU1[0]|0xff                                     # IOU1 (IIO PCIe Port 2)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ConfigIOU1[1]|0xff                                     # IOU1 (IIO PCIe Port 2)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ConfigIOU1[2]|0xff                                     # IOU1 (IIO PCIe Port 2)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ConfigIOU1[3]|0xff                                     # IOU1 (IIO PCIe Port 2)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ConfigIOU2[0]|0xff                                     # IOU2 (IIO PCIe Port 3)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ConfigIOU2[1]|0xff                                     # IOU2 (IIO PCIe Port 3)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ConfigIOU2[2]|0xff                                     # IOU2 (IIO PCIe Port 3)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ConfigIOU2[3]|0xff                                     # IOU2 (IIO PCIe Port 3)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ConfigIOU3[0]|0xff                                     # IOU3 (IIO PCIe Port 4)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ConfigIOU3[1]|0xff                                     # IOU3 (IIO PCIe Port 4)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ConfigIOU3[2]|0xff                                     # IOU3 (IIO PCIe Port 4)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ConfigIOU3[3]|0xff                                     # IOU3 (IIO PCIe Port 4)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ConfigIOU4[0]|0xff                                     # IOU4 (IIO PCIe Port 5)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ConfigIOU4[1]|0xff                                     # IOU4 (IIO PCIe Port 5)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ConfigIOU4[2]|0xff                                     # IOU4 (IIO PCIe Port 5)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ConfigIOU4[3]|0xff                                     # IOU4 (IIO PCIe Port 5)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ControlIommu|0x0                                       # Pre-boot DMA Protection
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS9|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[0]|0x0                                      # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[1]|0x1                                      # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[2]|0x1                                      # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[3]|0x1                                      # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[4]|0x1                                      # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[5]|0x1                                      # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[6]|0x1                                      # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[7]|0x1                                      # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[8]|0x1                                      # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[9]|0x1                                      # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[10]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[11]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[12]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[13]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[14]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[15]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[16]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[17]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[18]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[19]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[20]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[21]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[22]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[23]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[24]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[25]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[26]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[27]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[28]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[29]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[30]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[31]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[32]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[33]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[34]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[35]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[36]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[37]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[38]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[39]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[40]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[41]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[42]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[43]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[44]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[45]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[46]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[47]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[48]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[49]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[50]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[51]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[52]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[53]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[54]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[55]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[56]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[57]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[58]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[59]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[60]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[61]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[62]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[63]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[64]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[65]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[66]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[67]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[68]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[69]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[70]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[71]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[72]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[73]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[74]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[75]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[76]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[77]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[78]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[79]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[80]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[81]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[82]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DeEmphasis[83]|0x1                                     # PCI-E Port DeEmphasis
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DelayBeforePCIeLinkTraining|0x0                        # Delay before link training
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DevPresIoApicIio[0]|0x0                                # IIO IOAPIC Stack 0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS19|0x2
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS10|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[0]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[1]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[2]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[3]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[4]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[5]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[6]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[7]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[8]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[9]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[10]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[11]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[12]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[13]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[14]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[15]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[16]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[17]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[18]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[19]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[20]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[21]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[22]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[23]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[24]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[25]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[26]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[27]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[28]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[29]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[30]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[31]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[32]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[33]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[34]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[35]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[36]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[37]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[38]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[39]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[40]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[41]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[42]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[43]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[44]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[45]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[46]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[47]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[48]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[49]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[50]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[51]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[52]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[53]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[54]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[55]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[56]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[57]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[58]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[59]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[60]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[61]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[62]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[63]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[64]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[65]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[66]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[67]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[68]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[69]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[70]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[71]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[72]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[73]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[74]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[75]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[76]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[77]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[78]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[79]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[80]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[81]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[82]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS39[83]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[0]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[1]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[2]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[3]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[4]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[5]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[6]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[7]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[8]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[9]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[10]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[11]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[12]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[13]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[14]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[15]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[16]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[17]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[18]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[19]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[20]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[21]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[22]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[23]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[24]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[25]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[26]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[27]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[28]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[29]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[30]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[31]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[32]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[33]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[34]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[35]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[36]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[37]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[38]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[39]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[40]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[41]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[42]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[43]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[44]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[45]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[46]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[47]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[48]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[49]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[50]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[51]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[52]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[53]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[54]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[55]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[56]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[57]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[58]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[59]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[60]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[61]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[62]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[63]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[64]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[65]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[66]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[67]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[68]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[69]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[70]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[71]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[72]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[73]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[74]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[75]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[76]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[77]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[78]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[79]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[80]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[81]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[82]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS42[83]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[0]|0x29
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[1]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[2]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[3]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[4]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[5]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[6]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[7]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[8]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[9]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[10]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[11]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[12]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[13]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[14]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[15]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[16]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[17]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[18]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[19]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[20]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[21]|0x29
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[22]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[23]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[24]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[25]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[26]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[27]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[28]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[29]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[30]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[31]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[32]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[33]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[34]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[35]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[36]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[37]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[38]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[39]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[40]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[41]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[42]|0x29
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[43]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[44]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[45]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[46]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[47]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[48]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[49]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[50]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[51]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[52]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[53]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[54]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[55]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[56]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[57]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[58]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[59]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[60]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[61]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[62]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[63]|0x29
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[64]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[65]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[66]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[67]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[68]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[69]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[70]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[71]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[72]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[73]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[74]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[75]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[76]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[77]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[78]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[79]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[80]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[81]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[82]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS26[83]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[0]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[1]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[2]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[3]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[4]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[5]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[6]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[7]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[8]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[9]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[10]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[11]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[12]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[13]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[14]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[15]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[16]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[17]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[18]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[19]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[20]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[21]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[22]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[23]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[24]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[25]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[26]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[27]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[28]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[29]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[30]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[31]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[32]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[33]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[34]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[35]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[36]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[37]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[38]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[39]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[40]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[41]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[42]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[43]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[44]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[45]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[46]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[47]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[48]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[49]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[50]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[51]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[52]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[53]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[54]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[55]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[56]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[57]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[58]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[59]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[60]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[61]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[62]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[63]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[64]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[65]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[66]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[67]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[68]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[69]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[70]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[71]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[72]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[73]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[74]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[75]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[76]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[77]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[78]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[79]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[80]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[81]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[82]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS27[83]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[0]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[1]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[2]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[3]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[4]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[5]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[6]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[7]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[8]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[9]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[10]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[11]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[12]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[13]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[14]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[15]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[16]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[17]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[18]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[19]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[20]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[21]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[22]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[23]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[24]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[25]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[26]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[27]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[28]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[29]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[30]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[31]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[32]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[33]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[34]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[35]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[36]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[37]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[38]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[39]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[40]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[41]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[42]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[43]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[44]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[45]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[46]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[47]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[48]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[49]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[50]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[51]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[52]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[53]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[54]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[55]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[56]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[57]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[58]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[59]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[60]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[61]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[62]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[63]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[64]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[65]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[66]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[67]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[68]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[69]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[70]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[71]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[72]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[73]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[74]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[75]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[76]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[77]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[78]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[79]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[80]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[81]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[82]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS25[83]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS29[0]|0x29
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS29[21]|0x29
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS29[42]|0x29
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS29[63]|0x29
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[0]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[1]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[2]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[3]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[4]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[5]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[6]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[7]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[8]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[9]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[10]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[11]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[12]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[13]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[14]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[15]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[16]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[17]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[18]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[19]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[20]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[21]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[22]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[23]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[24]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[25]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[26]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[27]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[28]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[29]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[30]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[31]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[32]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[33]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[34]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[35]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[36]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[37]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[38]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[39]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[40]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[41]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[42]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[43]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[44]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[45]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[46]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[47]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[48]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[49]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[50]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[51]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[52]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[53]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[54]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[55]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[56]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[57]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[58]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[59]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[60]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[61]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[62]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[63]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[64]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[65]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[66]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[67]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[68]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[69]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[70]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[71]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[72]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[73]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[74]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[75]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[76]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[77]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[78]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[79]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[80]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[81]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[82]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS30[83]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[0]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[1]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[2]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[3]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[4]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[5]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[6]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[7]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[8]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[9]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[10]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[11]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[12]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[13]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[14]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[15]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[16]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[17]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[18]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[19]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[20]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[21]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[22]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[23]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[24]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[25]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[26]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[27]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[28]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[29]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[30]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[31]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[32]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[33]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[34]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[35]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[36]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[37]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[38]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[39]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[40]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[41]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[42]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[43]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[44]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[45]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[46]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[47]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[48]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[49]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[50]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[51]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[52]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[53]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[54]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[55]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[56]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[57]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[58]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[59]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[60]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[61]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[62]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[63]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[64]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[65]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[66]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[67]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[68]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[69]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[70]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[71]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[72]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[73]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[74]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[75]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[76]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[77]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[78]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[79]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[80]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[81]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[82]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS28[83]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[0]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[1]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[2]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[3]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[4]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[5]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[6]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[7]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[8]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[9]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[10]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[11]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[12]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[13]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[14]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[15]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[16]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[17]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[18]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[19]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[20]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[21]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[22]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[23]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[24]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[25]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[26]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[27]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[28]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[29]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[30]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[31]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[32]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[33]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[34]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[35]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[36]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[37]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[38]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[39]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[40]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[41]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[42]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[43]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[44]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[45]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[46]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[47]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[48]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[49]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[50]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[51]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[52]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[53]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[54]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[55]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[56]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[57]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[58]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[59]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[60]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[61]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[62]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[63]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[64]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[65]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[66]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[67]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[68]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[69]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[70]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[71]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[72]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[73]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[74]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[75]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[76]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[77]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[78]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[79]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[80]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[81]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[82]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS23[83]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[0]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[1]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[2]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[3]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[4]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[5]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[6]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[7]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[8]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[9]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[10]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[11]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[12]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[13]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[14]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[15]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[16]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[17]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[18]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[19]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[20]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[21]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[22]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[23]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[24]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[25]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[26]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[27]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[28]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[29]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[30]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[31]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[32]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[33]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[34]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[35]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[36]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[37]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[38]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[39]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[40]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[41]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[42]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[43]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[44]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[45]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[46]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[47]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[48]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[49]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[50]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[51]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[52]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[53]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[54]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[55]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[56]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[57]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[58]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[59]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[60]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[61]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[62]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[63]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[64]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[65]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[66]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[67]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[68]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[69]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[70]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[71]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[72]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[73]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[74]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[75]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[76]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[77]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[78]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[79]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[80]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[81]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[82]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS24[83]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[1]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[2]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[3]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[4]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[5]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[6]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[7]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[8]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[9]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[10]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[11]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[12]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[13]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[14]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[15]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[16]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[17]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[18]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[19]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[20]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[22]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[23]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[24]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[25]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[26]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[27]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[28]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[29]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[30]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[31]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[32]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[33]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[34]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[35]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[36]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[37]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[38]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[39]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[40]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[41]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[43]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[44]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[45]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[46]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[47]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[48]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[49]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[50]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[51]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[52]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[53]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[54]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[55]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[56]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[57]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[58]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[59]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[60]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[61]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[62]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[64]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[65]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[66]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[67]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[68]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[69]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[70]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[71]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[72]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[73]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[74]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[75]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[76]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[77]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[78]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[79]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[80]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[81]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[82]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS34[83]|0x18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[1]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[2]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[3]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[4]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[5]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[6]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[7]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[8]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[9]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[10]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[11]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[12]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[13]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[14]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[15]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[16]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[17]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[18]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[19]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[20]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[22]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[23]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[24]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[25]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[26]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[27]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[28]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[29]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[30]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[31]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[32]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[33]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[34]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[35]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[36]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[37]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[38]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[39]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[40]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[41]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[43]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[44]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[45]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[46]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[47]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[48]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[49]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[50]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[51]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[52]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[53]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[54]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[55]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[56]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[57]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[58]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[59]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[60]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[61]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[62]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[64]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[65]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[66]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[67]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[68]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[69]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[70]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[71]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[72]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[73]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[74]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[75]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[76]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[77]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[78]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[79]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[80]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[81]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[82]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS35[83]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[1]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[2]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[3]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[4]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[5]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[6]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[7]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[8]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[9]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[10]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[11]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[12]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[13]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[14]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[15]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[16]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[17]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[18]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[19]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[20]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[22]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[23]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[24]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[25]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[26]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[27]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[28]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[29]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[30]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[31]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[32]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[33]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[34]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[35]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[36]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[37]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[38]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[39]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[40]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[41]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[43]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[44]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[45]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[46]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[47]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[48]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[49]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[50]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[51]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[52]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[53]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[54]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[55]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[56]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[57]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[58]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[59]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[60]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[61]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[62]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[64]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[65]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[66]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[67]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[68]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[69]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[70]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[71]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[72]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[73]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[74]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[75]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[76]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[77]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[78]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[79]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[80]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[81]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[82]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS33[83]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[1]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[2]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[3]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[4]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[5]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[6]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[7]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[8]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[9]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[10]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[11]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[12]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[13]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[14]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[15]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[16]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[17]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[18]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[19]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[20]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[22]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[23]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[24]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[25]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[26]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[27]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[28]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[29]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[30]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[31]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[32]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[33]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[34]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[35]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[36]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[37]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[38]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[39]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[40]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[41]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[43]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[44]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[45]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[46]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[47]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[48]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[49]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[50]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[51]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[52]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[53]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[54]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[55]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[56]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[57]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[58]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[59]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[60]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[61]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[62]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[64]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[65]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[66]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[67]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[68]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[69]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[70]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[71]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[72]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[73]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[74]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[75]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[76]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[77]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[78]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[79]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[80]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[81]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[82]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS38[83]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[1]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[2]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[3]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[4]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[5]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[6]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[7]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[8]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[9]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[10]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[11]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[12]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[13]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[14]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[15]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[16]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[17]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[18]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[19]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[20]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[22]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[23]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[24]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[25]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[26]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[27]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[28]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[29]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[30]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[31]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[32]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[33]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[34]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[35]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[36]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[37]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[38]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[39]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[40]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[41]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[43]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[44]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[45]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[46]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[47]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[48]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[49]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[50]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[51]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[52]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[53]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[54]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[55]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[56]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[57]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[58]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[59]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[60]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[61]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[62]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[64]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[65]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[66]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[67]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[68]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[69]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[70]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[71]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[72]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[73]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[74]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[75]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[76]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[77]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[78]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[79]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[80]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[81]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[82]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS36[83]|0xb
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[1]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[2]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[3]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[4]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[5]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[6]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[7]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[8]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[9]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[10]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[11]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[12]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[13]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[14]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[15]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[16]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[17]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[18]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[19]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[20]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[22]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[23]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[24]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[25]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[26]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[27]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[28]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[29]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[30]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[31]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[32]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[33]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[34]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[35]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[36]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[37]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[38]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[39]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[40]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[41]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[43]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[44]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[45]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[46]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[47]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[48]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[49]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[50]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[51]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[52]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[53]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[54]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[55]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[56]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[57]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[58]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[59]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[60]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[61]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[62]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[64]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[65]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[66]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[67]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[68]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[69]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[70]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[71]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[72]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[73]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[74]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[75]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[76]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[77]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[78]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[79]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[80]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[81]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[82]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS31[83]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[1]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[2]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[3]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[4]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[5]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[6]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[7]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[8]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[9]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[10]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[11]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[12]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[13]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[14]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[15]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[16]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[17]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[18]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[19]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[20]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[22]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[23]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[24]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[25]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[26]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[27]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[28]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[29]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[30]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[31]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[32]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[33]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[34]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[35]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[36]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[37]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[38]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[39]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[40]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[41]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[43]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[44]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[45]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[46]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[47]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[48]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[49]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[50]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[51]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[52]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[53]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[54]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[55]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[56]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[57]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[58]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[59]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[60]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[61]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[62]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[64]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[65]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[66]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[67]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[68]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[69]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[70]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[71]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[72]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[73]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[74]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[75]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[76]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[77]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[78]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[79]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[80]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[81]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[82]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS32[83]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS17|0x2
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS16|0x2
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS15|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[0]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[1]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[2]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[3]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[4]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[5]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[6]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[7]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[8]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[9]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[10]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[11]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[12]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[13]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[14]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[15]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[16]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[17]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[18]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[19]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[20]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[21]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[22]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[23]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[24]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[25]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[26]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[27]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[28]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[29]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[30]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[31]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[32]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[33]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[34]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[35]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[36]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[37]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[38]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[39]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[40]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[41]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[42]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[43]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[44]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[45]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[46]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[47]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[48]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[49]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[50]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[51]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[52]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[53]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[54]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[55]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[56]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[57]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[58]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[59]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[60]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[61]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[62]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[63]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[64]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[65]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[66]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[67]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[68]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[69]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[70]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[71]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[72]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[73]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[74]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[75]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[76]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[77]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[78]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[79]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[80]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[81]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[82]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS44[83]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS11|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS14|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS13|0x1
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS12|0x99
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS18|0x4
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[0]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[1]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[2]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[3]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[4]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[5]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[6]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[7]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[8]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[9]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[10]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[11]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[12]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[13]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[14]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[15]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[16]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[17]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[18]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[19]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[20]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[21]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[22]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[23]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[24]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[25]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[26]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[27]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[28]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[29]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[30]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[31]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[32]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[33]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[34]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[35]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[36]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[37]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[38]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[39]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[40]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[41]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[42]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[43]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[44]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[45]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[46]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[47]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[48]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[49]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[50]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[51]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[52]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[53]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[54]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[55]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[56]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[57]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[58]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[59]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[60]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[61]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[62]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[63]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[64]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[65]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[66]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[67]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[68]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[69]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[70]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[71]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[72]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[73]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[74]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[75]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[76]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[77]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[78]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[79]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[80]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[81]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[82]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS40[83]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[0]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[1]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[2]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[3]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[4]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[5]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[6]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[7]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[8]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[9]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[10]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[11]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[12]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[13]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[14]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[15]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[16]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[17]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[18]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[19]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[20]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[21]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[22]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[23]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[24]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[25]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[26]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[27]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[28]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[29]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[30]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[31]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[32]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[33]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[34]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[35]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[36]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[37]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[38]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[39]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[40]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[41]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[42]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[43]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[44]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[45]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[46]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[47]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[48]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[49]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[50]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[51]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[52]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[53]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[54]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[55]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[56]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[57]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[58]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[59]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[60]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[61]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[62]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[63]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[64]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[65]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[66]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[67]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[68]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[69]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[70]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[71]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[72]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[73]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[74]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[75]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[76]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[77]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[78]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[79]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[80]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[81]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[82]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[83]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[84]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[85]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[86]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[87]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[88]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[89]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[90]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[91]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[92]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[93]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[94]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[95]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[96]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[97]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[98]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[99]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[100]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[101]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[102]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[103]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[104]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[105]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[106]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[107]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[108]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[109]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[110]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[111]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[112]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[113]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[114]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[115]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[116]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[117]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[118]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[119]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[120]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[121]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[122]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[123]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[124]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[125]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[126]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[127]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[128]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[129]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[130]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[131]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[132]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[133]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[134]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[135]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[136]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[137]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[138]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[139]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[140]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[141]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[142]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[143]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[144]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[145]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[146]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[147]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[148]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[149]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[150]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[151]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[152]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[153]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[154]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[155]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[156]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[157]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[158]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[159]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[160]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[161]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[162]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[163]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[164]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[165]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[166]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[167]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[168]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[169]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[170]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[171]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[172]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[173]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[174]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[175]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[176]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[177]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[178]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[179]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[180]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[181]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[182]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[183]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[184]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[185]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[186]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[187]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[188]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[189]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[190]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[191]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[192]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[193]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[194]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[195]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[196]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[197]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[198]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[199]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[200]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[201]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[202]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[203]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[204]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[205]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[206]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[207]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[208]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[209]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[210]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[211]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[212]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[213]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[214]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[215]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[216]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[217]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[218]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[219]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[220]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[221]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[222]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[223]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[224]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[225]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[226]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[227]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[228]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[229]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[230]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[231]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[232]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[233]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[234]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[235]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[236]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[237]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[238]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[239]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[240]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[241]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[242]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[243]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[244]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[245]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[246]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[247]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[248]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[249]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[250]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[251]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[252]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[253]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[254]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS21[255]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[0]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[1]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[2]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[3]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[4]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[5]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[6]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[7]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[8]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[9]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[10]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[11]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[12]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[13]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[14]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[15]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[16]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[17]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[18]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[19]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[20]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[21]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[22]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[23]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[24]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[25]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[26]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[27]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[28]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[29]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[30]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[31]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[32]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[33]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[34]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[35]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[36]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[37]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[38]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[39]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[40]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[41]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[42]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[43]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[44]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[45]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[46]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[47]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[48]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[49]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[50]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[51]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[52]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[53]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[54]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[55]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[56]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[57]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[58]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[59]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[60]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[61]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[62]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[63]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[64]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[65]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[66]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[67]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[68]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[69]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[70]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[71]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[72]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[73]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[74]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[75]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[76]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[77]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[78]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[79]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[80]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[81]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[82]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS41[83]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[0]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[1]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[2]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[3]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[4]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[5]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[6]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[7]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[8]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[9]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[10]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[11]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[12]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[13]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[14]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[15]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[16]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[17]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[18]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[19]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[20]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[21]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[22]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[23]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[24]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[25]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[26]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[27]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[28]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[29]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[30]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[31]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[32]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[33]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[34]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[35]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[36]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[37]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[38]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[39]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[40]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[41]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[42]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[43]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[44]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[45]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[46]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[47]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[48]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[49]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[50]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[51]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[52]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[53]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[54]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[55]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[56]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[57]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[58]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[59]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[60]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[61]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[62]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[63]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[64]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[65]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[66]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[67]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[68]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[69]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[70]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[71]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[72]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[73]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[74]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[75]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[76]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[77]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[78]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[79]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[80]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[81]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[82]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS43[83]|0xff
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[0]|0x0                                     # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[1]|0x0                                     # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[2]|0x0                                     # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[3]|0x0                                     # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[4]|0x0                                     # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[5]|0x0                                     # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[6]|0x0                                     # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[7]|0x0                                     # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[8]|0x0                                     # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[9]|0x0                                     # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[10]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[11]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[12]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[13]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[14]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[15]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[16]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[17]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[18]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[19]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[20]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[21]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[22]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[23]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[24]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[25]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[26]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[27]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[28]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[29]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[30]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[31]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[32]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[33]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[34]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[35]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[36]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[37]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[38]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[39]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[40]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[41]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[42]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[43]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[44]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[45]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[46]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[47]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[48]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[49]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[50]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[51]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[52]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[53]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[54]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[55]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[56]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[57]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[58]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[59]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[60]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[61]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[62]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[63]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[64]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[65]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[66]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[67]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[68]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[69]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[70]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[71]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[72]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[73]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[74]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[75]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[76]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[77]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[78]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[79]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[80]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[81]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[82]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisPMETOAck[83]|0x0                                    # PME to ACK
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DisableTPH|0x0                                         # Disable TPH
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DmaCtrlOptIn|0x0                                       # DMA Control Opt-In Flag
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DmiAllocatingFlow|0x1                                  # DMI Allocating Write Flows
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.DualCvIoFlow|0x1                                       # Dual CV IO Flow
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EN1K|0x0                                               # EN1K
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[0]|0x1                                             # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[1]|0x0                                             # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[2]|0x0                                             # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[3]|0x0                                             # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[4]|0x0                                             # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[5]|0x0                                             # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[6]|0x0                                             # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[7]|0x0                                             # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[8]|0x0                                             # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[9]|0x0                                             # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[10]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[11]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[12]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[13]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[14]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[15]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[16]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[17]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[18]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[19]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[20]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[21]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[22]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[23]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[24]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[25]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[26]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[27]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[28]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[29]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[30]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[31]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[32]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[33]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[34]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[35]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[36]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[37]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[38]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[39]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[40]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[41]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[42]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[43]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[44]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[45]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[46]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[47]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[48]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[49]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[50]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[51]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[52]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[53]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[54]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[55]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[56]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[57]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[58]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[59]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[60]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[61]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[62]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[63]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[64]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[65]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[66]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[67]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[68]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[69]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[70]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[71]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[72]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[73]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[74]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[75]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[76]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[77]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[78]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[79]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[80]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[81]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[82]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EOI[83]|0x0                                            # EOI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[0]|0x0                                       # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[1]|0x0                                       # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[2]|0x0                                       # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[3]|0x0                                       # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[4]|0x0                                       # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[5]|0x0                                       # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[6]|0x0                                       # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[7]|0x0                                       # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[8]|0x0                                       # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[9]|0x0                                       # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[10]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[11]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[12]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[13]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[14]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[15]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[16]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[17]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[18]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[19]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[20]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[21]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[22]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[23]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[24]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[25]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[26]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[27]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[28]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[29]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[30]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[31]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[32]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[33]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[34]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[35]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[36]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[37]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[38]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[39]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[40]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[41]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[42]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[43]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[44]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[45]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[46]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[47]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[48]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[49]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[50]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[51]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[52]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[53]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[54]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[55]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[56]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[57]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[58]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[59]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[60]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[61]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[62]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[63]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[64]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[65]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[66]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[67]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[68]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[69]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[70]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[71]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[72]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[73]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[74]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[75]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[76]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[77]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[78]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[79]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[80]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[81]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[82]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcChkEn[83]|0x0                                      # ECRC Check
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[0]|0x0                                       # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[1]|0x0                                       # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[2]|0x0                                       # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[3]|0x0                                       # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[4]|0x0                                       # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[5]|0x0                                       # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[6]|0x0                                       # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[7]|0x0                                       # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[8]|0x0                                       # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[9]|0x0                                       # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[10]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[11]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[12]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[13]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[14]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[15]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[16]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[17]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[18]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[19]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[20]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[21]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[22]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[23]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[24]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[25]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[26]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[27]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[28]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[29]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[30]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[31]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[32]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[33]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[34]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[35]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[36]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[37]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[38]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[39]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[40]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[41]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[42]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[43]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[44]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[45]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[46]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[47]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[48]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[49]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[50]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[51]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[52]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[53]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[54]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[55]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[56]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[57]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[58]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[59]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[60]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[61]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[62]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[63]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[64]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[65]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[66]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[67]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[68]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[69]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[70]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[71]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[72]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[73]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[74]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[75]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[76]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[77]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[78]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[79]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[80]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[81]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[82]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.EcrcGenEn[83]|0x0                                      # ECRC Generation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[0]|0x0                                    # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[1]|0x0                                    # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[2]|0x0                                    # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[3]|0x0                                    # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[4]|0x0                                    # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[5]|0x0                                    # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[6]|0x0                                    # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[7]|0x0                                    # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[8]|0x0                                    # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[9]|0x0                                    # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[10]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[11]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[12]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[13]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[14]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[15]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[16]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[17]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[18]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[19]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[20]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[21]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[22]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[23]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[24]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[25]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[26]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[27]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[28]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[29]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[30]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[31]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[32]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[33]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[34]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[35]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[36]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[37]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[38]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[39]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[40]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[41]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[42]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[43]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[44]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[45]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[46]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[47]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[48]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[49]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[50]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[51]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[52]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[53]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[54]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[55]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[56]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[57]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[58]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[59]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[60]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[61]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[62]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[63]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[64]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[65]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[66]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[67]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[68]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[69]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[70]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[71]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[72]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[73]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[74]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[75]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[76]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[77]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[78]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[79]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[80]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[81]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[82]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ExtendedSync[83]|0x0                                   # PCI-E Extended Sync
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.GlobalPme2AckTOCtrl|0x0                                # PME2ACK Timeout
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.HaltOnDmiDegraded|0x0                                  # Skip Halt On DMI Degradation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[0]|0x6                                            # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[1]|0x6                                            # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[2]|0x6                                            # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[3]|0x6                                            # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[4]|0x6                                            # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[5]|0x6                                            # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[6]|0x6                                            # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[7]|0x6                                            # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[8]|0x6                                            # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[9]|0x6                                            # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[10]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[11]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[12]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[13]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[14]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[15]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[16]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[17]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[18]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[19]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[20]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[21]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[22]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[23]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[24]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[25]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[26]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[27]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[28]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[29]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[30]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[31]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[32]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[33]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[34]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[35]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[36]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[37]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[38]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[39]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[40]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[41]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[42]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[43]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[44]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[45]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[46]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[47]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[48]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[49]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[50]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[51]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[52]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[53]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[54]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[55]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[56]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[57]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[58]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[59]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[60]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[61]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[62]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[63]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[64]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[65]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[66]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[67]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[68]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[69]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[70]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[71]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[72]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[73]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[74]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[75]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[76]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[77]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[78]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[79]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[80]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[81]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[82]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.IODC[83]|0x6                                           # IODC Configuration
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[0]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[1]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[2]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[3]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[4]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[5]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[6]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[7]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[8]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[9]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[10]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[11]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[12]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[13]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[14]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[15]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[16]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[17]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[18]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[19]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[20]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[21]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[22]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[23]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[24]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[25]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[26]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[27]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[28]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[29]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[30]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[31]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[32]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[33]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[34]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[35]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[36]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[37]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[38]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[39]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[40]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[41]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[42]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[43]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[44]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[45]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[46]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[47]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[48]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[49]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[50]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[51]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[52]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[53]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[54]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[55]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[56]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[57]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[58]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[59]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[60]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[61]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[62]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[63]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[64]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[65]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[66]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[67]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[68]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[69]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[70]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[71]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[72]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[73]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[74]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[75]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[76]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[77]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[78]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[79]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[80]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[81]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[82]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS45[83]|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.InterruptRemap|0x2                                     # Interrupt Remapping
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[0]|0x0                                        # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[1]|0x0                                        # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[2]|0x0                                        # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[3]|0x0                                        # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[4]|0x0                                        # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[5]|0x0                                        # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[6]|0x0                                        # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[7]|0x0                                        # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[8]|0x0                                        # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[9]|0x0                                        # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[10]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[11]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[12]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[13]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[14]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[15]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[16]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[17]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[18]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[19]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[20]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[21]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[22]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[23]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[24]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[25]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[26]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[27]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[28]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[29]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[30]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[31]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[32]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[33]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[34]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[35]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[36]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[37]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[38]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[39]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[40]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[41]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[42]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[43]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[44]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[45]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[46]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[47]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[48]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[49]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[50]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[51]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[52]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[53]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[54]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[55]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[56]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[57]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[58]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[59]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[60]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[61]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[62]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[63]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[64]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[65]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[66]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[67]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[68]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[69]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[70]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[71]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[72]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[73]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[74]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[75]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[76]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[77]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[78]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[79]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[80]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[81]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[82]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSICOREN[83]|0x0                                       # Corr Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[0]|0x0                                        # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[1]|0x0                                        # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[2]|0x0                                        # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[3]|0x0                                        # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[4]|0x0                                        # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[5]|0x0                                        # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[6]|0x0                                        # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[7]|0x0                                        # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[8]|0x0                                        # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[9]|0x0                                        # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[10]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[11]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[12]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[13]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[14]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[15]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[16]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[17]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[18]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[19]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[20]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[21]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[22]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[23]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[24]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[25]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[26]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[27]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[28]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[29]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[30]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[31]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[32]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[33]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[34]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[35]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[36]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[37]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[38]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[39]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[40]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[41]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[42]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[43]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[44]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[45]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[46]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[47]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[48]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[49]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[50]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[51]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[52]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[53]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[54]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[55]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[56]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[57]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[58]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[59]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[60]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[61]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[62]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[63]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[64]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[65]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[66]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[67]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[68]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[69]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[70]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[71]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[72]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[73]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[74]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[75]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[76]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[77]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[78]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[79]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[80]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[81]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[82]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSIFATEN[83]|0x0                                       # Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[0]|0x0                                       # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[1]|0x0                                       # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[2]|0x0                                       # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[3]|0x0                                       # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[4]|0x0                                       # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[5]|0x0                                       # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[6]|0x0                                       # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[7]|0x0                                       # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[8]|0x0                                       # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[9]|0x0                                       # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[10]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[11]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[12]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[13]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[14]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[15]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[16]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[17]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[18]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[19]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[20]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[21]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[22]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[23]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[24]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[25]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[26]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[27]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[28]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[29]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[30]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[31]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[32]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[33]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[34]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[35]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[36]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[37]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[38]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[39]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[40]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[41]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[42]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[43]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[44]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[45]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[46]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[47]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[48]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[49]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[50]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[51]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[52]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[53]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[54]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[55]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[56]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[57]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[58]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[59]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[60]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[61]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[62]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[63]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[64]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[65]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[66]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[67]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[68]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[69]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[70]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[71]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[72]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[73]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[74]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[75]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[76]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[77]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[78]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[79]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[80]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[81]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[82]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MSINFATEN[83]|0x0                                      # Non-Fatal Err Over
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MaxReadCompCombSize|0x0                                # Max Read Comp Comb Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.McastBaseAddrRegion|0x0                                # MC BaseAddress Range
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.McastIndexPosition|0xc                                 # MC Index Position
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.McastNumGroup|0x8                                      # MC Num Group
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[0]|0x1                                          # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[1]|0x1                                          # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[2]|0x1                                          # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[3]|0x1                                          # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[4]|0x1                                          # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[5]|0x1                                          # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[6]|0x1                                          # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[7]|0x1                                          # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[8]|0x1                                          # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[9]|0x1                                          # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[10]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[11]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[12]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[13]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[14]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[15]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[16]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[17]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[18]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[19]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[20]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[21]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[22]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[23]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[24]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[25]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[26]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[27]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[28]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[29]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[30]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[31]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[32]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[33]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[34]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[35]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[36]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[37]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[38]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[39]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[40]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[41]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[42]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[43]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[44]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[45]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[46]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[47]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[48]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[49]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[50]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[51]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[52]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[53]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[54]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[55]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[56]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[57]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[58]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[59]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[60]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[61]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[62]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[63]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[64]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[65]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[66]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[67]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[68]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[69]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[70]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[71]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[72]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[73]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[74]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[75]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[76]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[77]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[78]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[79]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[80]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[81]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[82]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MctpEn[83]|0x1                                         # MCTP
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[0]|0x0                                           # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[1]|0x0                                           # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[2]|0x0                                           # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[3]|0x0                                           # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[4]|0x0                                           # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[5]|0x0                                           # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[6]|0x0                                           # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[7]|0x0                                           # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[8]|0x0                                           # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[9]|0x0                                           # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[10]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[11]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[12]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[13]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[14]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[15]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[16]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[17]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[18]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[19]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[20]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[21]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[22]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[23]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[24]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[25]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[26]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[27]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[28]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[29]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[30]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[31]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[32]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[33]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[34]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[35]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[36]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[37]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[38]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[39]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[40]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[41]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[42]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[43]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[44]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[45]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[46]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[47]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[48]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[49]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[50]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[51]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[52]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[53]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[54]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[55]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[56]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[57]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[58]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[59]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[60]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[61]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[62]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[63]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[64]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[65]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[66]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[67]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[68]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[69]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[70]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[71]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[72]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[73]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[74]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[75]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[76]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[77]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[78]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[79]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[80]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[81]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[82]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MsiEn[83]|0x0                                          # MSI
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.MultiCastEnable|0x0                                    # MultiCast Enable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NoSnoopRdCfg|0x0                                       # NoSnoop Read Config
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NoSnoopWrCfg|0x1                                       # NoSnoop Write Config
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NorthTraceHubMemReg0Size[0]|0x0                        # North TH Mem Buffer Size 0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NorthTraceHubMemReg0Size[1]|0x0                        # North TH Mem Buffer Size 0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NorthTraceHubMemReg0Size[2]|0x0                        # North TH Mem Buffer Size 0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NorthTraceHubMemReg0Size[3]|0x0                        # North TH Mem Buffer Size 0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NorthTraceHubMemReg1Size[0]|0x0                        # North TH Mem Buffer Size 1
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NorthTraceHubMemReg1Size[1]|0x0                        # North TH Mem Buffer Size 1
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NorthTraceHubMemReg1Size[2]|0x0                        # North TH Mem Buffer Size 1
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NorthTraceHubMemReg1Size[3]|0x0                        # North TH Mem Buffer Size 1
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NorthTraceHubMode[0]|0x0                               # North Trace Hub Enable Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NorthTraceHubMode[1]|0x0                               # North Trace Hub Enable Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NorthTraceHubMode[2]|0x0                               # North Trace Hub Enable Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NorthTraceHubMode[3]|0x0                               # North Trace Hub Enable Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ1[0]|0x16                             # Embar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ1[1]|0x16                             # Embar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ1[2]|0x16                             # Embar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ1[3]|0x16                             # Embar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ1[4]|0x16                             # Embar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ1[5]|0x16                             # Embar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ1[6]|0x16                             # Embar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ1[7]|0x16                             # Embar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ1[8]|0x16                             # Embar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ1[9]|0x16                             # Embar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ1[10]|0x16                            # Embar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ1[11]|0x16                            # Embar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ1[12]|0x16                            # Embar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ1[13]|0x16                            # Embar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ1[14]|0x16                            # Embar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ1[15]|0x16                            # Embar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ1[16]|0x16                            # Embar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ1[17]|0x16                            # Embar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ1[18]|0x16                            # Embar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ1[19]|0x16                            # Embar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2[0]|0x16                             # Embar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2[1]|0x16                             # Embar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2[2]|0x16                             # Embar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2[3]|0x16                             # Embar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2[4]|0x16                             # Embar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2[5]|0x16                             # Embar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2[6]|0x16                             # Embar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2[7]|0x16                             # Embar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2[8]|0x16                             # Embar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2[9]|0x16                             # Embar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2[10]|0x16                            # Embar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2[11]|0x16                            # Embar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2[12]|0x16                            # Embar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2[13]|0x16                            # Embar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2[14]|0x16                            # Embar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2[15]|0x16                            # Embar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2[16]|0x16                            # Embar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2[17]|0x16                            # Embar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2[18]|0x16                            # Embar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2[19]|0x16                            # Embar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_0[0]|0xc                            # Embar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_0[1]|0xc                            # Embar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_0[2]|0xc                            # Embar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_0[3]|0xc                            # Embar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_0[4]|0xc                            # Embar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_0[5]|0xc                            # Embar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_0[6]|0xc                            # Embar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_0[7]|0xc                            # Embar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_0[8]|0xc                            # Embar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_0[9]|0xc                            # Embar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_0[10]|0xc                           # Embar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_0[11]|0xc                           # Embar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_0[12]|0xc                           # Embar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_0[13]|0xc                           # Embar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_0[14]|0xc                           # Embar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_0[15]|0xc                           # Embar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_0[16]|0xc                           # Embar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_0[17]|0xc                           # Embar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_0[18]|0xc                           # Embar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_0[19]|0xc                           # Embar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_1[0]|0xc                            # Embar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_1[1]|0xc                            # Embar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_1[2]|0xc                            # Embar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_1[3]|0xc                            # Embar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_1[4]|0xc                            # Embar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_1[5]|0xc                            # Embar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_1[6]|0xc                            # Embar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_1[7]|0xc                            # Embar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_1[8]|0xc                            # Embar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_1[9]|0xc                            # Embar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_1[10]|0xc                           # Embar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_1[11]|0xc                           # Embar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_1[12]|0xc                           # Embar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_1[13]|0xc                           # Embar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_1[14]|0xc                           # Embar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_1[15]|0xc                           # Embar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_1[16]|0xc                           # Embar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_1[17]|0xc                           # Embar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_1[18]|0xc                           # Embar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeEmBarSZ2_1[19]|0xc                           # Embar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar1[0]|0x16                               # Imbar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar1[1]|0x16                               # Imbar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar1[2]|0x16                               # Imbar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar1[3]|0x16                               # Imbar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar1[4]|0x16                               # Imbar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar1[5]|0x16                               # Imbar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar1[6]|0x16                               # Imbar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar1[7]|0x16                               # Imbar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar1[8]|0x16                               # Imbar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar1[9]|0x16                               # Imbar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar1[10]|0x16                              # Imbar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar1[11]|0x16                              # Imbar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar1[12]|0x16                              # Imbar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar1[13]|0x16                              # Imbar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar1[14]|0x16                              # Imbar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar1[15]|0x16                              # Imbar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar1[16]|0x16                              # Imbar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar1[17]|0x16                              # Imbar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar1[18]|0x16                              # Imbar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar1[19]|0x16                              # Imbar1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2[0]|0x16                               # Imbar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2[1]|0x16                               # Imbar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2[2]|0x16                               # Imbar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2[3]|0x16                               # Imbar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2[4]|0x16                               # Imbar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2[5]|0x16                               # Imbar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2[6]|0x16                               # Imbar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2[7]|0x16                               # Imbar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2[8]|0x16                               # Imbar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2[9]|0x16                               # Imbar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2[10]|0x16                              # Imbar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2[11]|0x16                              # Imbar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2[12]|0x16                              # Imbar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2[13]|0x16                              # Imbar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2[14]|0x16                              # Imbar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2[15]|0x16                              # Imbar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2[16]|0x16                              # Imbar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2[17]|0x16                              # Imbar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2[18]|0x16                              # Imbar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2[19]|0x16                              # Imbar2 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_0[0]|0xc                              # Imbar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_0[1]|0xc                              # Imbar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_0[2]|0xc                              # Imbar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_0[3]|0xc                              # Imbar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_0[4]|0xc                              # Imbar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_0[5]|0xc                              # Imbar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_0[6]|0xc                              # Imbar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_0[7]|0xc                              # Imbar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_0[8]|0xc                              # Imbar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_0[9]|0xc                              # Imbar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_0[10]|0xc                             # Imbar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_0[11]|0xc                             # Imbar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_0[12]|0xc                             # Imbar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_0[13]|0xc                             # Imbar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_0[14]|0xc                             # Imbar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_0[15]|0xc                             # Imbar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_0[16]|0xc                             # Imbar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_0[17]|0xc                             # Imbar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_0[18]|0xc                             # Imbar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_0[19]|0xc                             # Imbar2_0 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_1[0]|0xc                              # Imbar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_1[1]|0xc                              # Imbar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_1[2]|0xc                              # Imbar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_1[3]|0xc                              # Imbar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_1[4]|0xc                              # Imbar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_1[5]|0xc                              # Imbar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_1[6]|0xc                              # Imbar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_1[7]|0xc                              # Imbar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_1[8]|0xc                              # Imbar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_1[9]|0xc                              # Imbar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_1[10]|0xc                             # Imbar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_1[11]|0xc                             # Imbar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_1[12]|0xc                             # Imbar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_1[13]|0xc                             # Imbar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_1[14]|0xc                             # Imbar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_1[15]|0xc                             # Imbar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_1[16]|0xc                             # Imbar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_1[17]|0xc                             # Imbar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_1[18]|0xc                             # Imbar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeImBar2_1[19]|0xc                             # Imbar2_1 Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeOverride[0]|0x0                              # Enable NTB BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeOverride[1]|0x0                              # Enable NTB BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeOverride[2]|0x0                              # Enable NTB BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeOverride[3]|0x0                              # Enable NTB BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeOverride[4]|0x0                              # Enable NTB BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeOverride[5]|0x0                              # Enable NTB BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeOverride[6]|0x0                              # Enable NTB BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeOverride[7]|0x0                              # Enable NTB BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeOverride[8]|0x0                              # Enable NTB BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeOverride[9]|0x0                              # Enable NTB BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeOverride[10]|0x0                             # Enable NTB BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeOverride[11]|0x0                             # Enable NTB BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeOverride[12]|0x0                             # Enable NTB BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeOverride[13]|0x0                             # Enable NTB BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeOverride[14]|0x0                             # Enable NTB BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeOverride[15]|0x0                             # Enable NTB BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeOverride[16]|0x0                             # Enable NTB BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeOverride[17]|0x0                             # Enable NTB BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeOverride[18]|0x0                             # Enable NTB BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbBarSizeOverride[19]|0x0                             # Enable NTB BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbLinkBiosTrainEn|0x2                                 # NTB Link Train by BIOS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbPpd[0]|0x0                                          # Non-Transparent Bridge PCIe Port Definition
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbPpd[1]|0x0                                          # Non-Transparent Bridge PCIe Port Definition
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbPpd[2]|0x0                                          # Non-Transparent Bridge PCIe Port Definition
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbPpd[3]|0x0                                          # Non-Transparent Bridge PCIe Port Definition
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbPpd[4]|0x0                                          # Non-Transparent Bridge PCIe Port Definition
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbPpd[5]|0x0                                          # Non-Transparent Bridge PCIe Port Definition
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbPpd[6]|0x0                                          # Non-Transparent Bridge PCIe Port Definition
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbPpd[7]|0x0                                          # Non-Transparent Bridge PCIe Port Definition
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbPpd[8]|0x0                                          # Non-Transparent Bridge PCIe Port Definition
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbPpd[9]|0x0                                          # Non-Transparent Bridge PCIe Port Definition
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbPpd[10]|0x0                                         # Non-Transparent Bridge PCIe Port Definition
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbPpd[11]|0x0                                         # Non-Transparent Bridge PCIe Port Definition
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbPpd[12]|0x0                                         # Non-Transparent Bridge PCIe Port Definition
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbPpd[13]|0x0                                         # Non-Transparent Bridge PCIe Port Definition
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbPpd[14]|0x0                                         # Non-Transparent Bridge PCIe Port Definition
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbPpd[15]|0x0                                         # Non-Transparent Bridge PCIe Port Definition
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbPpd[16]|0x0                                         # Non-Transparent Bridge PCIe Port Definition
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbPpd[17]|0x0                                         # Non-Transparent Bridge PCIe Port Definition
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbPpd[18]|0x0                                         # Non-Transparent Bridge PCIe Port Definition
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbPpd[19]|0x0                                         # Non-Transparent Bridge PCIe Port Definition
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbSplitBar[0]|0x0                                     # Enable SPLIT BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbSplitBar[1]|0x0                                     # Enable SPLIT BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbSplitBar[2]|0x0                                     # Enable SPLIT BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbSplitBar[3]|0x0                                     # Enable SPLIT BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbSplitBar[4]|0x0                                     # Enable SPLIT BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbSplitBar[5]|0x0                                     # Enable SPLIT BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbSplitBar[6]|0x0                                     # Enable SPLIT BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbSplitBar[7]|0x0                                     # Enable SPLIT BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbSplitBar[8]|0x0                                     # Enable SPLIT BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbSplitBar[9]|0x0                                     # Enable SPLIT BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbSplitBar[10]|0x0                                    # Enable SPLIT BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbSplitBar[11]|0x0                                    # Enable SPLIT BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbSplitBar[12]|0x0                                    # Enable SPLIT BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbSplitBar[13]|0x0                                    # Enable SPLIT BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbSplitBar[14]|0x0                                    # Enable SPLIT BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbSplitBar[15]|0x0                                    # Enable SPLIT BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbSplitBar[16]|0x0                                    # Enable SPLIT BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbSplitBar[17]|0x0                                    # Enable SPLIT BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbSplitBar[18]|0x0                                    # Enable SPLIT BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbSplitBar[19]|0x0                                    # Enable SPLIT BARs
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbXlinkCtlOverride[0]|0x3                             # Crosslink control Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbXlinkCtlOverride[1]|0x3                             # Crosslink control Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbXlinkCtlOverride[2]|0x3                             # Crosslink control Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbXlinkCtlOverride[3]|0x3                             # Crosslink control Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbXlinkCtlOverride[4]|0x3                             # Crosslink control Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbXlinkCtlOverride[5]|0x3                             # Crosslink control Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbXlinkCtlOverride[6]|0x3                             # Crosslink control Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbXlinkCtlOverride[7]|0x3                             # Crosslink control Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbXlinkCtlOverride[8]|0x3                             # Crosslink control Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbXlinkCtlOverride[9]|0x3                             # Crosslink control Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbXlinkCtlOverride[10]|0x3                            # Crosslink control Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbXlinkCtlOverride[11]|0x3                            # Crosslink control Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbXlinkCtlOverride[12]|0x3                            # Crosslink control Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbXlinkCtlOverride[13]|0x3                            # Crosslink control Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbXlinkCtlOverride[14]|0x3                            # Crosslink control Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbXlinkCtlOverride[15]|0x3                            # Crosslink control Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbXlinkCtlOverride[16]|0x3                            # Crosslink control Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbXlinkCtlOverride[17]|0x3                            # Crosslink control Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbXlinkCtlOverride[18]|0x3                            # Crosslink control Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.NtbXlinkCtlOverride[19]|0x3                            # Crosslink control Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[0]|0x0                                        # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[1]|0x0                                        # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[2]|0x0                                        # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[3]|0x0                                        # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[4]|0x0                                        # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[5]|0x0                                        # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[6]|0x0                                        # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[7]|0x0                                        # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[8]|0x0                                        # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[9]|0x0                                        # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[10]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[11]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[12]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[13]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[14]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[15]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[16]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[17]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[18]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[19]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[20]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[21]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[22]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[23]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[24]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[25]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[26]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[27]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[28]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[29]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[30]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[31]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[32]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[33]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[34]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[35]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[36]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[37]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[38]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[39]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[40]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[41]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[42]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[43]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[44]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[45]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[46]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[47]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[48]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[49]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[50]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[51]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[52]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[53]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[54]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[55]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[56]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[57]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[58]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[59]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[60]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[61]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[62]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[63]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[64]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[65]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[66]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[67]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[68]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[69]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[70]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[71]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[72]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[73]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[74]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[75]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[76]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[77]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[78]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[79]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[80]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[81]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[82]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.P2PRdDis[83]|0x0                                       # P2P Memory Read
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PCIe_AtomicOpReq|0x2                                   # PCIe Atomic Op Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PCIe_LTR|0x2                                           # PCIe LTR Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PCUF6Hide|0x0                                          # Hide PCU Func 6
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[1]|0x0                                        # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[2]|0x0                                        # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[3]|0x0                                        # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[4]|0x0                                        # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[5]|0x0                                        # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[6]|0x0                                        # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[7]|0x0                                        # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[8]|0x0                                        # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[9]|0x0                                        # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[10]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[11]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[12]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[13]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[14]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[15]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[16]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[17]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[18]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[19]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[20]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[21]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[22]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[23]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[24]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[25]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[26]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[27]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[28]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[29]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[30]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[31]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[32]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[33]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[34]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[35]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[36]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[37]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[38]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[39]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[40]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[41]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[42]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[43]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[44]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[45]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[46]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[47]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[48]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[49]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[50]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[51]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[52]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[53]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[54]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[55]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[56]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[57]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[58]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[59]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[60]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[61]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[62]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[63]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[64]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[65]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[66]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[67]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[68]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[69]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[70]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[71]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[72]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[73]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[74]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[75]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[76]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[77]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[78]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[79]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[80]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[81]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[82]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PEXPHIDE[83]|0x0                                       # Hide Port?
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Pci64BitResourceAllocation|0x1                         # PCI 64-Bit Resource Allocation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Pcie10bitTag|0x2                                       # PCIe 10-bit Tag Enable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICEnabled[0]|0x0                                  # Intel� AIC Retimer/AIC SSD HW at Stack1
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICEnabled[1]|0x0                                  # Intel� AIC Retimer/AIC SSD HW at Stack2
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICEnabled[2]|0x0                                  # Intel� AIC Retimer/AIC SSD HW at Stack3
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICEnabled[3]|0x0                                  # Intel� AIC Retimer/AIC SSD HW at Stack4
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICEnabled[4]|0x0                                  # Intel� AIC Retimer/AIC SSD HW at Stack5
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICEnabled[5]|0x0                                  # Intel� AIC Retimer/AIC SSD HW at Stack1
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICEnabled[6]|0x0                                  # Intel� AIC Retimer/AIC SSD HW at Stack2
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICEnabled[7]|0x0                                  # Intel� AIC Retimer/AIC SSD HW at Stack3
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICEnabled[8]|0x0                                  # Intel� AIC Retimer/AIC SSD HW at Stack4
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICEnabled[9]|0x0                                  # Intel� AIC Retimer/AIC SSD HW at Stack5
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICEnabled[10]|0x0                                 # Intel� AIC Retimer/AIC SSD HW at Stack1
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICEnabled[11]|0x0                                 # Intel� AIC Retimer/AIC SSD HW at Stack2
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICEnabled[12]|0x0                                 # Intel� AIC Retimer/AIC SSD HW at Stack3
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICEnabled[13]|0x0                                 # Intel� AIC Retimer/AIC SSD HW at Stack4
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICEnabled[14]|0x0                                 # Intel� AIC Retimer/AIC SSD HW at Stack5
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICEnabled[15]|0x0                                 # Intel� AIC Retimer/AIC SSD HW at Stack1
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICEnabled[16]|0x0                                 # Intel� AIC Retimer/AIC SSD HW at Stack2
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICEnabled[17]|0x0                                 # Intel� AIC Retimer/AIC SSD HW at Stack3
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICEnabled[18]|0x0                                 # Intel� AIC Retimer/AIC SSD HW at Stack4
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICEnabled[19]|0x0                                 # Intel� AIC Retimer/AIC SSD HW at Stack5
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICHotPlugEnable[0]|0x0                            # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICHotPlugEnable[1]|0x0                            # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICHotPlugEnable[2]|0x0                            # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICHotPlugEnable[3]|0x0                            # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICHotPlugEnable[4]|0x0                            # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICHotPlugEnable[5]|0x0                            # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICHotPlugEnable[6]|0x0                            # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICHotPlugEnable[7]|0x0                            # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICHotPlugEnable[8]|0x0                            # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICHotPlugEnable[9]|0x0                            # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICHotPlugEnable[10]|0x0                           # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICHotPlugEnable[11]|0x0                           # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICHotPlugEnable[12]|0x0                           # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICHotPlugEnable[13]|0x0                           # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICHotPlugEnable[14]|0x0                           # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICHotPlugEnable[15]|0x0                           # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICHotPlugEnable[16]|0x0                           # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICHotPlugEnable[17]|0x0                           # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICHotPlugEnable[18]|0x0                           # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICHotPlugEnable[19]|0x0                           # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[0]|0x0                               # Port 1A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[1]|0x0                               # Port 1B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[2]|0x0                               # Port 1C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[3]|0x0                               # Port 1D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[4]|0x0                               # Port 2A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[5]|0x0                               # Port 2B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[6]|0x0                               # Port 2C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[7]|0x0                               # Port 2D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[8]|0x0                               # Port 3A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[9]|0x0                               # Port 3B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[10]|0x0                              # Port 3C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[11]|0x0                              # Port 3D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[12]|0x0                              # Port 4A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[13]|0x0                              # Port 4B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[14]|0x0                              # Port 4C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[15]|0x0                              # Port 4D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[16]|0x0                              # Port 5A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[17]|0x0                              # Port 5B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[18]|0x0                              # Port 5C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[19]|0x0                              # Port 5D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[20]|0x0                              # Port 1A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[21]|0x0                              # Port 1B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[22]|0x0                              # Port 1C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[23]|0x0                              # Port 1D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[24]|0x0                              # Port 2A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[25]|0x0                              # Port 2B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[26]|0x0                              # Port 2C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[27]|0x0                              # Port 2D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[28]|0x0                              # Port 3A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[29]|0x0                              # Port 3B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[30]|0x0                              # Port 3C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[31]|0x0                              # Port 3D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[32]|0x0                              # Port 4A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[33]|0x0                              # Port 4B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[34]|0x0                              # Port 4C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[35]|0x0                              # Port 4D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[36]|0x0                              # Port 5A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[37]|0x0                              # Port 5B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[38]|0x0                              # Port 5C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[39]|0x0                              # Port 5D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[40]|0x0                              # Port 1A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[41]|0x0                              # Port 1B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[42]|0x0                              # Port 1C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[43]|0x0                              # Port 1D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[44]|0x0                              # Port 2A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[45]|0x0                              # Port 2B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[46]|0x0                              # Port 2C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[47]|0x0                              # Port 2D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[48]|0x0                              # Port 3A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[49]|0x0                              # Port 3B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[50]|0x0                              # Port 3C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[51]|0x0                              # Port 3D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[52]|0x0                              # Port 4A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[53]|0x0                              # Port 4B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[54]|0x0                              # Port 4C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[55]|0x0                              # Port 4D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[56]|0x0                              # Port 5A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[57]|0x0                              # Port 5B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[58]|0x0                              # Port 5C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[59]|0x0                              # Port 5D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[60]|0x0                              # Port 1A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[61]|0x0                              # Port 1B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[62]|0x0                              # Port 1C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[63]|0x0                              # Port 1D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[64]|0x0                              # Port 2A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[65]|0x0                              # Port 2B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[66]|0x0                              # Port 2C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[67]|0x0                              # Port 2D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[68]|0x0                              # Port 3A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[69]|0x0                              # Port 3B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[70]|0x0                              # Port 3C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[71]|0x0                              # Port 3D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[72]|0x0                              # Port 4A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[73]|0x0                              # Port 4B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[74]|0x0                              # Port 4C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[75]|0x0                              # Port 4D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[76]|0x0                              # Port 5A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[77]|0x0                              # Port 5B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[78]|0x0                              # Port 5C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAICPortEnable[79]|0x0                              # Port 5D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAcpiHotPlugEnable|0x0                              # PCIe ACPI Hot Plug
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAllocatingFlow|0x1                                 # PCIe Allocating Write Flows
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[0]|0x4                                        # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[1]|0x4                                        # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[2]|0x4                                        # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[3]|0x4                                        # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[4]|0x4                                        # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[5]|0x4                                        # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[6]|0x4                                        # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[7]|0x4                                        # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[8]|0x4                                        # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[9]|0x4                                        # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[10]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[11]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[12]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[13]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[14]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[15]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[16]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[17]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[18]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[19]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[20]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[21]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[22]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[23]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[24]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[25]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[26]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[27]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[28]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[29]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[30]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[31]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[32]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[33]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[34]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[35]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[36]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[37]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[38]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[39]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[40]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[41]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[42]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[43]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[44]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[45]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[46]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[47]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[48]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[49]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[50]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[51]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[52]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[53]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[54]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[55]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[56]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[57]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[58]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[59]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[60]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[61]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[62]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[63]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[64]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[65]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[66]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[67]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[68]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[69]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[70]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[71]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[72]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[73]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[74]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[75]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[76]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[77]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[78]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[79]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[80]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[81]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[82]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieAspm[83]|0x4                                       # PCI-E ASPM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieBiosTrainEnable|0x1                                # PCIe Train by BIOS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[0]|0x1                                 # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[1]|0x1                                 # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[2]|0x1                                 # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[3]|0x1                                 # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[4]|0x1                                 # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[5]|0x1                                 # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[6]|0x1                                 # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[7]|0x1                                 # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[8]|0x1                                 # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[9]|0x1                                 # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[10]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[11]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[12]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[13]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[14]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[15]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[16]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[17]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[18]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[19]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[20]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[21]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[22]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[23]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[24]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[25]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[26]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[27]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[28]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[29]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[30]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[31]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[32]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[33]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[34]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[35]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[36]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[37]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[38]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[39]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[40]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[41]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[42]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[43]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[44]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[45]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[46]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[47]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[48]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[49]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[50]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[51]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[52]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[53]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[54]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[55]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[56]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[57]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[58]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[59]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[60]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[61]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[62]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[63]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[64]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[65]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[66]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[67]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[68]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[69]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[70]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[71]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[72]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[73]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[74]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[75]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[76]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[77]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[78]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[79]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[80]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[81]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[82]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieCommonClock[83]|0x1                                # PCI-E Port Clocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[0]|0x0                                      # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[1]|0x0                                      # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[2]|0x0                                      # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[3]|0x0                                      # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[4]|0x0                                      # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[5]|0x0                                      # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[6]|0x0                                      # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[7]|0x0                                      # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[8]|0x0                                      # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[9]|0x0                                      # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[10]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[11]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[12]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[13]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[14]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[15]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[16]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[17]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[18]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[19]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[20]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[21]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[22]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[23]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[24]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[25]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[26]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[27]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[28]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[29]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[30]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[31]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[32]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[33]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[34]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[35]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[36]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[37]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[38]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[39]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[40]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[41]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[42]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[43]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[44]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[45]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[46]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[47]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[48]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[49]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[50]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[51]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[52]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[53]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[54]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[55]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[56]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[57]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[58]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[59]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[60]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[61]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[62]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[63]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[64]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[65]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[66]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[67]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[68]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[69]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[70]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[71]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[72]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[73]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[74]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[75]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[76]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[77]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[78]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[79]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[80]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[81]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[82]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDState[83]|0x0                                     # PCI-E Port D-state
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[0]|0x1               # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[1]|0x1               # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[2]|0x1               # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[3]|0x1               # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[4]|0x1               # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[5]|0x1               # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[6]|0x1               # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[7]|0x1               # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[8]|0x1               # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[9]|0x1               # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[10]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[11]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[12]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[13]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[14]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[15]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[16]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[17]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[18]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[19]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[20]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[21]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[22]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[23]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[24]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[25]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[26]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[27]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[28]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[29]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[30]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[31]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[32]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[33]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[34]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[35]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[36]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[37]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[38]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[39]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[40]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[41]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[42]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[43]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[44]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[45]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[46]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[47]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[48]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[49]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[50]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[51]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[52]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[53]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[54]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[55]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[56]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[57]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[58]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[59]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[60]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[61]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[62]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[63]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[64]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[65]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[66]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[67]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[68]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[69]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[70]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[71]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[72]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[73]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[74]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[75]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[76]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[77]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[78]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[79]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[80]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[81]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[82]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieDataLinkFeatureExchangeEnable[83]|0x1              # Data Link Feature Exchange
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieExtendedTagField|0x2                               # PCIe Extended Tag Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieGlobalAspm|0x1                                     # PCI-E ASPM Support (Global)
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugEnable|0x0                                  # PCIe Hot Plug
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[1]|0x2                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[2]|0x2                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[3]|0x2                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[4]|0x2                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[5]|0x2                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[6]|0x2                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[7]|0x2                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[8]|0x2                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[9]|0x2                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[10]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[11]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[12]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[13]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[14]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[15]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[16]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[17]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[18]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[19]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[20]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[21]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[22]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[23]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[24]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[25]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[26]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[27]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[28]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[29]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[30]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[31]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[32]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[33]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[34]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[35]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[36]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[37]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[38]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[39]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[40]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[41]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[42]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[43]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[44]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[45]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[46]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[47]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[48]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[49]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[50]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[51]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[52]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[53]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[54]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[55]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[56]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[57]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[58]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[59]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[60]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[61]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[62]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[63]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[64]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[65]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[66]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[67]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[68]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[69]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[70]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[71]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[72]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[73]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[74]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[75]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[76]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[77]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[78]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[79]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[80]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[81]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[82]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieHotPlugOnPort[83]|0x2                              # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[0]|0x4                                   # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[1]|0x4                                   # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[2]|0x4                                   # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[3]|0x4                                   # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[4]|0x4                                   # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[5]|0x4                                   # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[6]|0x4                                   # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[7]|0x4                                   # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[8]|0x4                                   # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[9]|0x4                                   # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[10]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[11]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[12]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[13]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[14]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[15]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[16]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[17]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[18]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[19]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[20]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[21]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[22]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[23]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[24]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[25]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[26]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[27]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[28]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[29]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[30]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[31]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[32]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[33]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[34]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[35]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[36]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[37]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[38]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[39]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[40]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[41]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[42]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[43]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[44]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[45]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[46]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[47]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[48]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[49]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[50]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[51]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[52]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[53]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[54]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[55]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[56]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[57]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[58]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[59]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[60]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[61]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[62]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[63]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[64]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[65]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[66]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[67]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[68]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[69]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[70]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[71]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[72]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[73]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[74]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[75]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[76]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[77]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[78]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[79]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[80]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[81]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[82]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieL1Latency[83]|0x4                                  # PCI-E Port L1 Exit Latency
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[1]|0x0                                     # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[2]|0x0                                     # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[3]|0x0                                     # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[4]|0x0                                     # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[5]|0x0                                     # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[6]|0x0                                     # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[7]|0x0                                     # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[8]|0x0                                     # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[9]|0x0                                     # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[10]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[11]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[12]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[13]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[14]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[15]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[16]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[17]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[18]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[19]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[20]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[21]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[22]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[23]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[24]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[25]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[26]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[27]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[28]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[29]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[30]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[31]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[32]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[33]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[34]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[35]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[36]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[37]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[38]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[39]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[40]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[41]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[42]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[43]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[44]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[45]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[46]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[47]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[48]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[49]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[50]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[51]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[52]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[53]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[54]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[55]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[56]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[57]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[58]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[59]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[60]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[61]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[62]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[63]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[64]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[65]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[66]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[67]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[68]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[69]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[70]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[71]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[72]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[73]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[74]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[75]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[76]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[77]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[78]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[79]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[80]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[81]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[82]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLinkDis[83]|0x0                                    # PCI-E Port Link Disable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieLowLatencyRetimersEnabled|0x0                      # PCIe Low Latency Retimers
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[0]|0x7                                  # DMI Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[1]|0x7                                  # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[2]|0x7                                  # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[3]|0x7                                  # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[4]|0x7                                  # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[5]|0x7                                  # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[6]|0x7                                  # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[7]|0x7                                  # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[8]|0x7                                  # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[9]|0x7                                  # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[10]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[11]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[12]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[13]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[14]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[15]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[16]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[17]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[18]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[19]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[20]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[21]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[22]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[23]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[24]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[25]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[26]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[27]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[28]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[29]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[30]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[31]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[32]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[33]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[34]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[35]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[36]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[37]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[38]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[39]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[40]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[41]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[42]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[43]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[44]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[45]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[46]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[47]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[48]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[49]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[50]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[51]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[52]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[53]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[54]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[55]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[56]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[57]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[58]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[59]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[60]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[61]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[62]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[63]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[64]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[65]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[66]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[67]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[68]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[69]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[70]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[71]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[72]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[73]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[74]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[75]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[76]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[77]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[78]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[79]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[80]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[81]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[82]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxPayload[83]|0x7                                 # PCI-E Port MPSS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieMaxReadRequestSize|0x5                             # PCIe Max Read Request Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePhyTestMode|0x0                                    # PCIe PHY test mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[0]|0x1                           # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[1]|0x1                           # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[2]|0x1                           # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[3]|0x1                           # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[4]|0x1                           # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[5]|0x1                           # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[6]|0x1                           # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[7]|0x1                           # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[8]|0x1                           # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[9]|0x1                           # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[10]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[11]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[12]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[13]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[14]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[15]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[16]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[17]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[18]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[19]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[20]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[21]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[22]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[23]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[24]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[25]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[26]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[27]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[28]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[29]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[30]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[31]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[32]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[33]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[34]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[35]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[36]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[37]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[38]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[39]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[40]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[41]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[42]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[43]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[44]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[45]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[46]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[47]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[48]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[49]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[50]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[51]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[52]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[53]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[54]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[55]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[56]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[57]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[58]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[59]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[60]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[61]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[62]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[63]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[64]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[65]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[66]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[67]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[68]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[69]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[70]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[71]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[72]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[73]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[74]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[75]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[76]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[77]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[78]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[79]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[80]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[81]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[82]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortClkGateEnable[83]|0x1                          # PCI-E Port Clock Gating
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[1]|0x2                                  # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[2]|0x2                                  # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[3]|0x2                                  # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[4]|0x2                                  # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[5]|0x2                                  # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[6]|0x2                                  # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[7]|0x2                                  # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[8]|0x2                                  # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[9]|0x2                                  # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[10]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[11]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[12]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[13]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[14]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[15]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[16]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[17]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[18]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[19]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[20]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[21]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[22]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[23]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[24]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[25]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[26]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[27]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[28]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[29]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[30]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[31]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[32]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[33]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[34]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[35]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[36]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[37]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[38]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[39]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[40]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[41]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[42]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[43]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[44]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[45]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[46]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[47]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[48]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[49]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[50]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[51]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[52]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[53]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[54]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[55]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[56]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[57]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[58]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[59]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[60]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[61]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[62]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[63]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[64]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[65]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[66]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[67]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[68]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[69]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[70]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[71]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[72]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[73]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[74]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[75]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[76]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[77]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[78]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[79]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[80]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[81]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[82]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortEnable[83]|0x2                                 # PCI-E Port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[0]|0x0                            # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[1]|0x0                            # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[2]|0x0                            # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[3]|0x0                            # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[4]|0x0                            # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[5]|0x0                            # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[6]|0x0                            # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[7]|0x0                            # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[8]|0x0                            # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[9]|0x0                            # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[10]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[11]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[12]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[13]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[14]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[15]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[16]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[17]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[18]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[19]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[20]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[21]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[22]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[23]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[24]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[25]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[26]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[27]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[28]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[29]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[30]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[31]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[32]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[33]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[34]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[35]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[36]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[37]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[38]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[39]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[40]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[41]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[42]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[43]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[44]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[45]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[46]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[47]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[48]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[49]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[50]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[51]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[52]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[53]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[54]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[55]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[56]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[57]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[58]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[59]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[60]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[61]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[62]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[63]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[64]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[65]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[66]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[67]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[68]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[69]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[70]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[71]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[72]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[73]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[74]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[75]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[76]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[77]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[78]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[79]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[80]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[81]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[82]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkMaxWidth[83]|0x0                           # Override Max Link Width
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[0]|0x0                               # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[1]|0x0                               # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[2]|0x0                               # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[3]|0x0                               # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[4]|0x0                               # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[5]|0x0                               # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[6]|0x0                               # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[7]|0x0                               # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[8]|0x0                               # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[9]|0x0                               # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[10]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[11]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[12]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[13]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[14]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[15]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[16]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[17]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[18]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[19]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[20]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[21]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[22]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[23]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[24]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[25]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[26]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[27]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[28]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[29]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[30]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[31]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[32]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[33]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[34]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[35]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[36]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[37]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[38]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[39]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[40]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[41]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[42]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[43]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[44]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[45]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[46]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[47]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[48]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[49]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[50]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[51]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[52]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[53]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[54]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[55]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[56]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[57]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[58]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[59]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[60]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[61]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[62]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[63]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[64]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[65]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[66]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[67]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[68]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[69]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[70]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[71]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[72]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[73]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[74]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[75]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[76]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[77]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[78]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[79]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[80]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[81]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[82]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePortLinkSpeed[83]|0x0                              # Link Speed
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PciePtm|0x2                                            # PCIe PTM Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieRelaxedOrdering|0x1                                # Pcie Relaxed Ordering
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieSlotItemCtrl|0x0                                   # PCIe Slot Item Control
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieSlotOprom1|0x1                                     # PCIe Slot 1 OpROM
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieSlotOprom2|0x1                                     # PCIe Slot 2 OpROM
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieSlotOprom3|0x1                                     # PCIe Slot 3 OpROM
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieSlotOprom4|0x1                                     # PCIe Slot 4 OpROM
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieSlotOprom5|0x1                                     # PCIe Slot 5 OpROM
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieSlotOprom6|0x1                                     # PCIe Slot 6 OpROM
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieSlotOprom7|0x1                                     # PCIe Slot 7 OpROM
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieSlotOprom8|0x1                                     # PCIe Slot 8 OpROM
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[1]|0x2                                 # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[2]|0x2                                 # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[3]|0x2                                 # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[4]|0x2                                 # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[5]|0x2                                 # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[6]|0x2                                 # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[7]|0x2                                 # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[8]|0x2                                 # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[9]|0x2                                 # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[10]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[11]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[12]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[13]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[14]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[15]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[16]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[17]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[18]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[19]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[20]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[21]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[22]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[23]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[24]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[25]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[26]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[27]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[28]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[29]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[30]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[31]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[32]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[33]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[34]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[35]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[36]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[37]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[38]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[39]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[40]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[41]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[42]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[43]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[44]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[45]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[46]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[47]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[48]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[49]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[50]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[51]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[52]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[53]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[54]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[55]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[56]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[57]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[58]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[59]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[60]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[61]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[62]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[63]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[64]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[65]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[66]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[67]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[68]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[69]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[70]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[71]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[72]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[73]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[74]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[75]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[76]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[77]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[78]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[79]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[80]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[81]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[82]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieTxRxDetPoll[83]|0x2                                # PCI-E Detect Wait Time
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[0]|0x0                         # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[1]|0x0                         # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[2]|0x0                         # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[3]|0x0                         # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[4]|0x0                         # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[5]|0x0                         # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[6]|0x0                         # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[7]|0x0                         # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[8]|0x0                         # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[9]|0x0                         # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[10]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[11]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[12]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[13]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[14]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[15]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[16]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[17]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[18]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[19]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[20]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[21]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[22]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[23]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[24]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[25]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[26]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[27]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[28]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[29]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[30]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[31]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[32]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[33]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[34]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[35]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[36]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[37]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[38]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[39]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[40]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[41]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[42]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[43]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[44]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[45]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[46]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[47]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[48]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[49]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[50]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[51]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[52]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[53]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[54]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[55]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[56]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[57]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[58]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[59]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[60]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[61]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[62]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[63]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[64]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[65]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[66]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[67]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[68]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[69]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[70]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[71]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[72]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[73]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[74]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[75]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[76]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[77]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[78]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[79]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[80]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[81]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[82]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PcieUnsupportedRequests[83]|0x0                        # Unsupported Request
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PerformanceTuningMode|0x1                              # Performance Tuning Mode
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[0]|0x0                                # Enable MMIO read cmpl poison for STACK_0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[1]|0x0                                # Enable MMIO read cmpl poison for STACK_1
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[2]|0x0                                # Enable MMIO read cmpl poison for STACK_2
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[3]|0x0                                # Enable MMIO read cmpl poison for STACK_3
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[4]|0x0                                # Enable MMIO read cmpl poison for STACK_4
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[5]|0x0                                # Enable MMIO read cmpl poison for STACK_5
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[6]|0x0                                # Enable MMIO read cmpl poison for STACK_0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[7]|0x0                                # Enable MMIO read cmpl poison for STACK_1
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[8]|0x0                                # Enable MMIO read cmpl poison for STACK_2
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[9]|0x0                                # Enable MMIO read cmpl poison for STACK_3
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[10]|0x0                               # Enable MMIO read cmpl poison for STACK_4
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[11]|0x0                               # Enable MMIO read cmpl poison for STACK_5
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[12]|0x0                               # Enable MMIO read cmpl poison for STACK_0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[13]|0x0                               # Enable MMIO read cmpl poison for STACK_1
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[14]|0x0                               # Enable MMIO read cmpl poison for STACK_2
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[15]|0x0                               # Enable MMIO read cmpl poison for STACK_3
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[16]|0x0                               # Enable MMIO read cmpl poison for STACK_4
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[17]|0x0                               # Enable MMIO read cmpl poison for STACK_5
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[18]|0x0                               # Enable MMIO read cmpl poison for STACK_0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[19]|0x0                               # Enable MMIO read cmpl poison for STACK_1
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[20]|0x0                               # Enable MMIO read cmpl poison for STACK_2
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[21]|0x0                               # Enable MMIO read cmpl poison for STACK_3
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[22]|0x0                               # Enable MMIO read cmpl poison for STACK_4
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PoisonMmioReadEn[23]|0x0                               # Enable MMIO read cmpl poison for STACK_5
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PostedInterrupt|0x1                                    # Posted Interrupt
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.PrioritizeTPH|0x0                                      # Prioritize TPH
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ProblematicPort|0x0                                    # Problematic port
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerGlParmReg0Override[2]|0x0                       # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerGlParmReg0Override[3]|0x0                       # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerGlParmReg0Override[4]|0x0                       # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerGlParmReg0Override[5]|0x0                       # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerGlParmReg0Override[6]|0x0                       # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerGlParmReg0Override[7]|0x0                       # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerGlParmReg0Override[8]|0x0                       # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerGlParmReg0Override[9]|0x0                       # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerGlParmReg0Override[10]|0x0                      # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerGlParmReg0Override[11]|0x0                      # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerGlParmReg0Override[14]|0x0                      # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerGlParmReg0Override[15]|0x0                      # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerGlParmReg0Override[16]|0x0                      # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerGlParmReg0Override[17]|0x0                      # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerGlParmReg0Override[18]|0x0                      # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerGlParmReg0Override[19]|0x0                      # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerGlParmReg0Override[20]|0x0                      # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerGlParmReg0Override[21]|0x0                      # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerGlParmReg0Override[22]|0x0                      # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerGlParmReg0Override[23]|0x0                      # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort0Reg2Override[2]|0x0                  # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort0Reg2Override[3]|0x0                  # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort0Reg2Override[4]|0x0                  # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort0Reg2Override[5]|0x0                  # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort0Reg2Override[6]|0x0                  # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort0Reg2Override[7]|0x0                  # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort0Reg2Override[8]|0x0                  # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort0Reg2Override[9]|0x0                  # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort0Reg2Override[10]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort0Reg2Override[11]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort0Reg2Override[14]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort0Reg2Override[15]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort0Reg2Override[16]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort0Reg2Override[17]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort0Reg2Override[18]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort0Reg2Override[19]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort0Reg2Override[20]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort0Reg2Override[21]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort0Reg2Override[22]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort0Reg2Override[23]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort1Reg2Override[2]|0x0                  # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort1Reg2Override[3]|0x0                  # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort1Reg2Override[4]|0x0                  # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort1Reg2Override[5]|0x0                  # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort1Reg2Override[6]|0x0                  # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort1Reg2Override[7]|0x0                  # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort1Reg2Override[8]|0x0                  # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort1Reg2Override[9]|0x0                  # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort1Reg2Override[10]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort1Reg2Override[11]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort1Reg2Override[14]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort1Reg2Override[15]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort1Reg2Override[16]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort1Reg2Override[17]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort1Reg2Override[18]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort1Reg2Override[19]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort1Reg2Override[20]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort1Reg2Override[21]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort1Reg2Override[22]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RetimerPseudoPort1Reg2Override[23]|0x0                 # - Override
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RpCorrectableErrorEsc[0]|0x0                           # Sck0 RP Correctable Err
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RpCorrectableErrorEsc[1]|0x0                           # Sck1 RP Correctable Err
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RpCorrectableErrorEsc[2]|0x0                           # Sck2 RP Correctable Err
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RpCorrectableErrorEsc[3]|0x0                           # Sck3 RP Correctable Err
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RpUncorrectableFatalErrorEsc[0]|0x0                    # Sck0 RP Fatal Uncorrectable Err
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RpUncorrectableFatalErrorEsc[1]|0x0                    # Sck1 RP Fatal Uncorrectable Err
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RpUncorrectableFatalErrorEsc[2]|0x0                    # Sck2 RP Fatal Uncorrectable Err
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RpUncorrectableFatalErrorEsc[3]|0x0                    # Sck3 RP Fatal Uncorrectable Err
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RpUncorrectableNonFatalErrorEsc[0]|0x0                 # Sck0 RP NonFatal Uncorrectable Err
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RpUncorrectableNonFatalErrorEsc[1]|0x0                 # Sck1 RP NonFatal Uncorrectable Err
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RpUncorrectableNonFatalErrorEsc[2]|0x0                 # Sck2 RP NonFatal Uncorrectable Err
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RpUncorrectableNonFatalErrorEsc[3]|0x0                 # Sck3 RP NonFatal Uncorrectable Err
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.RxClockWA|0x0                                          # Rx Clock WA
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[0]|0x0                                           # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[1]|0x0                                           # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[2]|0x0                                           # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[3]|0x0                                           # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[4]|0x0                                           # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[5]|0x0                                           # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[6]|0x0                                           # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[7]|0x0                                           # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[8]|0x0                                           # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[9]|0x0                                           # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[10]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[11]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[12]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[13]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[14]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[15]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[16]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[17]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[18]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[19]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[20]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[21]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[22]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[23]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[24]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[25]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[26]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[27]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[28]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[29]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[30]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[31]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[32]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[33]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[34]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[35]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[36]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[37]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[38]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[39]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[40]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[41]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[42]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[43]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[44]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[45]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[46]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[47]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[48]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[49]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[50]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[51]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[52]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[53]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[54]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[55]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[56]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[57]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[58]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[59]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[60]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[61]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[62]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[63]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[64]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[65]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[66]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[67]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[68]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[69]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[70]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[71]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[72]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[73]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[74]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[75]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[76]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[77]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[78]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[79]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[80]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[81]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[82]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SERRE[83]|0x0                                          # SERRE
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[1]|0x0                                       # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[2]|0x0                                       # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[3]|0x0                                       # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[4]|0x0                                       # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[5]|0x0                                       # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[6]|0x0                                       # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[7]|0x0                                       # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[8]|0x0                                       # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[9]|0x0                                       # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[10]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[11]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[12]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[13]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[14]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[15]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[16]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[17]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[18]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[19]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[20]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[21]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[22]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[23]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[24]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[25]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[26]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[27]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[28]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[29]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[30]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[31]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[32]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[33]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[34]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[35]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[36]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[37]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[38]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[39]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[40]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[41]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[42]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[43]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[44]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[45]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[46]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[47]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[48]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[49]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[50]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[51]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[52]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[53]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[54]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[55]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[56]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[57]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[58]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[59]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[60]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[61]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[62]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[63]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[64]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[65]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[66]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[67]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[68]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[69]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[70]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[71]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[72]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[73]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[74]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[75]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[76]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[77]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[78]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[79]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[80]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[81]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[82]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SLOTHPSUP[83]|0x0                                      # Surprise Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[0]|0x0                                            # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[1]|0x0                                            # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[2]|0x0                                            # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[3]|0x0                                            # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[4]|0x0                                            # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[5]|0x0                                            # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[6]|0x0                                            # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[7]|0x0                                            # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[8]|0x0                                            # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[9]|0x0                                            # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[10]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[11]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[12]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[13]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[14]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[15]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[16]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[17]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[18]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[19]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[20]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[21]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[22]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[23]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[24]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[25]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[26]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[27]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[28]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[29]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[30]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[31]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[32]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[33]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[34]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[35]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[36]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[37]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[38]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[39]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[40]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[41]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[42]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[43]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[44]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[45]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[46]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[47]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[48]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[49]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[50]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[51]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[52]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[53]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[54]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[55]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[56]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[57]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[58]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[59]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[60]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[61]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[62]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[63]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[64]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[65]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[66]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[67]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[68]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[69]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[70]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[71]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[72]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[73]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[74]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[75]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[76]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[77]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[78]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[79]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[80]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[81]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[82]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SRIS[83]|0x0                                           # SRIS
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SierraPeakMemBufferSize[0]|0x0                         # Sierra Peak Memory Region Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SierraPeakMemBufferSize[1]|0x0                         # Sierra Peak Memory Region Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SierraPeakMemBufferSize[2]|0x0                         # Sierra Peak Memory Region Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SierraPeakMemBufferSize[3]|0x0                         # Sierra Peak Memory Region Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.ReservedS46|0x0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SkipRetimersDetection|0x0                              # Skip PCIe retimers detection
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.SnoopResponseHoldOff|0x9                               # Snoop Response Hold Off
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.StopAndScream|0x0                                      # PCIE Stop & Scream Support
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[0]|0x0                                            # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[1]|0x0                                            # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[2]|0x0                                            # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[3]|0x0                                            # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[4]|0x0                                            # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[5]|0x0                                            # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[6]|0x0                                            # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[7]|0x0                                            # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[8]|0x0                                            # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[9]|0x0                                            # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[10]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[11]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[12]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[13]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[14]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[15]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[16]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[17]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[18]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[19]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[20]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[21]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[22]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[23]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[24]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[25]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[26]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[27]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[28]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[29]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[30]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[31]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[32]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[33]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[34]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[35]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[36]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[37]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[38]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[39]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[40]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[41]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[42]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[43]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[44]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[45]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[46]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[47]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[48]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[49]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[50]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[51]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[52]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[53]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[54]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[55]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[56]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[57]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[58]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[59]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[60]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[61]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[62]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[63]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[64]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[65]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[66]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[67]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[68]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[69]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[70]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[71]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[72]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[73]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[74]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[75]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[76]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[77]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[78]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[79]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[80]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[81]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[82]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.TXEQ[83]|0x0                                           # Alternate TxEq
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[0]|0x2                                   # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[1]|0x2                                   # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[2]|0x2                                   # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[3]|0x2                                   # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[4]|0x2                                   # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[5]|0x2                                   # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[6]|0x2                                   # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[7]|0x2                                   # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[8]|0x2                                   # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[9]|0x2                                   # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[10]|0x2                                  # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[11]|0x2                                  # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[12]|0x2                                  # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[13]|0x2                                  # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[14]|0x2                                  # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[15]|0x2                                  # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[16]|0x2                                  # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[17]|0x2                                  # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[18]|0x2                                  # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[19]|0x2                                  # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[20]|0x2                                  # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[21]|0x2                                  # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[22]|0x2                                  # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarAttr[23]|0x2                                  # CfgBar attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[0]|0x19                                    # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[1]|0x19                                    # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[2]|0x19                                    # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[3]|0x19                                    # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[4]|0x19                                    # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[5]|0x19                                    # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[6]|0x19                                    # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[7]|0x19                                    # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[8]|0x19                                    # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[9]|0x19                                    # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[10]|0x19                                   # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[11]|0x19                                   # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[12]|0x19                                   # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[13]|0x19                                   # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[14]|0x19                                   # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[15]|0x19                                   # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[16]|0x19                                   # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[17]|0x19                                   # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[18]|0x19                                   # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[19]|0x19                                   # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[20]|0x19                                   # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[21]|0x19                                   # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[22]|0x19                                   # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDCfgBarSz[23]|0x19                                   # CfgBar size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[0]|0x0                                 # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[1]|0x0                                 # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[2]|0x0                                 # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[3]|0x0                                 # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[4]|0x0                                 # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[5]|0x0                                 # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[6]|0x0                                 # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[7]|0x0                                 # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[8]|0x0                                 # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[9]|0x0                                 # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[10]|0x0                                # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[11]|0x0                                # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[12]|0x0                                # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[13]|0x0                                # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[14]|0x0                                # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[15]|0x0                                # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[16]|0x0                                # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[17]|0x0                                # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[18]|0x0                                # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[19]|0x0                                # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[20]|0x0                                # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[21]|0x0                                # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[22]|0x0                                # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDDirectAssign[23]|0x0                                # VMD for Direct Assign
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[0]|0x0                                      # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[1]|0x0                                      # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[2]|0x0                                      # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[3]|0x0                                      # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[4]|0x0                                      # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[5]|0x0                                      # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[6]|0x0                                      # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[7]|0x0                                      # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[8]|0x0                                      # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[9]|0x0                                      # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[10]|0x0                                     # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[11]|0x0                                     # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[12]|0x0                                     # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[13]|0x0                                     # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[14]|0x0                                     # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[15]|0x0                                     # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[16]|0x0                                     # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[17]|0x0                                     # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[18]|0x0                                     # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[19]|0x0                                     # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[20]|0x0                                     # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[21]|0x0                                     # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[22]|0x0                                     # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDEnabled[23]|0x0                                     # Enable/Disable VMD
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[0]|0x0                                # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[1]|0x0                                # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[2]|0x0                                # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[3]|0x0                                # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[4]|0x0                                # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[5]|0x0                                # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[6]|0x0                                # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[7]|0x0                                # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[8]|0x0                                # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[9]|0x0                                # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[10]|0x0                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[11]|0x0                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[12]|0x0                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[13]|0x0                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[14]|0x0                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[15]|0x0                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[16]|0x0                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[17]|0x0                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[18]|0x0                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[19]|0x0                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[20]|0x0                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[21]|0x0                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[22]|0x0                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDHotPlugEnable[23]|0x0                               # Hot Plug Capable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[0]|0x0                                  # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[1]|0x0                                  # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[2]|0x0                                  # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[3]|0x0                                  # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[4]|0x0                                  # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[5]|0x0                                  # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[6]|0x0                                  # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[7]|0x0                                  # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[8]|0x0                                  # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[9]|0x0                                  # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[10]|0x0                                 # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[11]|0x0                                 # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[12]|0x0                                 # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[13]|0x0                                 # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[14]|0x0                                 # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[15]|0x0                                 # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[16]|0x0                                 # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[17]|0x0                                 # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[18]|0x0                                 # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[19]|0x0                                 # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[20]|0x0                                 # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[21]|0x0                                 # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[22]|0x0                                 # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar1Attr[23]|0x0                                 # MemBar1 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[0]|0x1                                  # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[1]|0x1                                  # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[2]|0x1                                  # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[3]|0x1                                  # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[4]|0x1                                  # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[5]|0x1                                  # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[6]|0x1                                  # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[7]|0x1                                  # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[8]|0x1                                  # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[9]|0x1                                  # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[10]|0x1                                 # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[11]|0x1                                 # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[12]|0x1                                 # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[13]|0x1                                 # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[14]|0x1                                 # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[15]|0x1                                 # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[16]|0x1                                 # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[17]|0x1                                 # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[18]|0x1                                 # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[19]|0x1                                 # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[20]|0x1                                 # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[21]|0x1                                 # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[22]|0x1                                 # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBar2Attr[23]|0x1                                 # MemBar2 attribute
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[0]|0x19                                   # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[1]|0x19                                   # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[2]|0x19                                   # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[3]|0x19                                   # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[4]|0x19                                   # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[5]|0x19                                   # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[6]|0x19                                   # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[7]|0x19                                   # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[8]|0x19                                   # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[9]|0x19                                   # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[10]|0x19                                  # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[11]|0x19                                  # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[12]|0x19                                  # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[13]|0x19                                  # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[14]|0x19                                  # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[15]|0x19                                  # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[16]|0x19                                  # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[17]|0x19                                  # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[18]|0x19                                  # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[19]|0x19                                  # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[20]|0x19                                  # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[21]|0x19                                  # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[22]|0x19                                  # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz1[23]|0x19                                  # MemBar1 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[0]|0x14                                   # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[1]|0x14                                   # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[2]|0x14                                   # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[3]|0x14                                   # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[4]|0x14                                   # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[5]|0x14                                   # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[6]|0x14                                   # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[7]|0x14                                   # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[8]|0x14                                   # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[9]|0x14                                   # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[10]|0x14                                  # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[11]|0x14                                  # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[12]|0x14                                  # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[13]|0x14                                  # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[14]|0x14                                  # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[15]|0x14                                  # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[16]|0x14                                  # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[17]|0x14                                  # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[18]|0x14                                  # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[19]|0x14                                  # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[20]|0x14                                  # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[21]|0x14                                  # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[22]|0x14                                  # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDMemBarSz2[23]|0x14                                  # MemBar2 size
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[0]|0x0                                # PCH Root Port 0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[1]|0x0                                # PCH Root Port 1
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[2]|0x0                                # PCH Root Port 2
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[3]|0x0                                # PCH Root Port 3
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[4]|0x0                                # PCH Root Port 4
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[5]|0x0                                # PCH Root Port 5
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[6]|0x0                                # PCH Root Port 6
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[7]|0x0                                # PCH Root Port 7
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[8]|0x0                                # PCH Root Port 8
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[9]|0x0                                # PCH Root Port 9
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[10]|0x0                               # PCH Root Port 10
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[11]|0x0                               # PCH Root Port 11
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[12]|0x0                               # PCH Root Port 12
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[13]|0x0                               # PCH Root Port 13
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[14]|0x0                               # PCH Root Port 14
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[15]|0x0                               # PCH Root Port 15
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[16]|0x0                               # PCH Root Port 16
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[17]|0x0                               # PCH Root Port 17
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[18]|0x0                               # PCH Root Port 18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[19]|0x0                               # PCH Root Port 19
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[20]|0x0                               # PCH Root Port 0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[21]|0x0                               # PCH Root Port 1
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[22]|0x0                               # PCH Root Port 2
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[23]|0x0                               # PCH Root Port 3
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[24]|0x0                               # PCH Root Port 4
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[25]|0x0                               # PCH Root Port 5
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[26]|0x0                               # PCH Root Port 6
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[27]|0x0                               # PCH Root Port 7
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[28]|0x0                               # PCH Root Port 8
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[29]|0x0                               # PCH Root Port 9
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[30]|0x0                               # PCH Root Port 10
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[31]|0x0                               # PCH Root Port 11
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[32]|0x0                               # PCH Root Port 12
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[33]|0x0                               # PCH Root Port 13
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[34]|0x0                               # PCH Root Port 14
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[35]|0x0                               # PCH Root Port 15
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[36]|0x0                               # PCH Root Port 16
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[37]|0x0                               # PCH Root Port 17
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[38]|0x0                               # PCH Root Port 18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[39]|0x0                               # PCH Root Port 19
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[40]|0x0                               # PCH Root Port 0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[41]|0x0                               # PCH Root Port 1
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[42]|0x0                               # PCH Root Port 2
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[43]|0x0                               # PCH Root Port 3
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[44]|0x0                               # PCH Root Port 4
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[45]|0x0                               # PCH Root Port 5
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[46]|0x0                               # PCH Root Port 6
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[47]|0x0                               # PCH Root Port 7
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[48]|0x0                               # PCH Root Port 8
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[49]|0x0                               # PCH Root Port 9
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[50]|0x0                               # PCH Root Port 10
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[51]|0x0                               # PCH Root Port 11
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[52]|0x0                               # PCH Root Port 12
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[53]|0x0                               # PCH Root Port 13
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[54]|0x0                               # PCH Root Port 14
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[55]|0x0                               # PCH Root Port 15
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[56]|0x0                               # PCH Root Port 16
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[57]|0x0                               # PCH Root Port 17
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[58]|0x0                               # PCH Root Port 18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[59]|0x0                               # PCH Root Port 19
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[60]|0x0                               # PCH Root Port 0
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[61]|0x0                               # PCH Root Port 1
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[62]|0x0                               # PCH Root Port 2
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[63]|0x0                               # PCH Root Port 3
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[64]|0x0                               # PCH Root Port 4
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[65]|0x0                               # PCH Root Port 5
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[66]|0x0                               # PCH Root Port 6
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[67]|0x0                               # PCH Root Port 7
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[68]|0x0                               # PCH Root Port 8
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[69]|0x0                               # PCH Root Port 9
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[70]|0x0                               # PCH Root Port 10
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[71]|0x0                               # PCH Root Port 11
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[72]|0x0                               # PCH Root Port 12
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[73]|0x0                               # PCH Root Port 13
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[74]|0x0                               # PCH Root Port 14
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[75]|0x0                               # PCH Root Port 15
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[76]|0x0                               # PCH Root Port 16
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[77]|0x0                               # PCH Root Port 17
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[78]|0x0                               # PCH Root Port 18
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPchPortEnable[79]|0x0                               # PCH Root Port 19
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[4]|0x0                                   # VMD port A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[5]|0x0                                   # VMD port B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[6]|0x0                                   # VMD port C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[7]|0x0                                   # VMD port D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[8]|0x0                                   # VMD port A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[9]|0x0                                   # VMD port B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[10]|0x0                                  # VMD port C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[11]|0x0                                  # VMD port D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[12]|0x0                                  # VMD port A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[13]|0x0                                  # VMD port B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[14]|0x0                                  # VMD port C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[15]|0x0                                  # VMD port D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[16]|0x0                                  # VMD port A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[17]|0x0                                  # VMD port B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[18]|0x0                                  # VMD port C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[19]|0x0                                  # VMD port D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[20]|0x0                                  # VMD port A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[21]|0x0                                  # VMD port B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[22]|0x0                                  # VMD port C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[23]|0x0                                  # VMD port D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[28]|0x0                                  # VMD port A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[29]|0x0                                  # VMD port B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[30]|0x0                                  # VMD port C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[31]|0x0                                  # VMD port D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[32]|0x0                                  # VMD port A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[33]|0x0                                  # VMD port B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[34]|0x0                                  # VMD port C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[35]|0x0                                  # VMD port D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[36]|0x0                                  # VMD port A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[37]|0x0                                  # VMD port B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[38]|0x0                                  # VMD port C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[39]|0x0                                  # VMD port D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[40]|0x0                                  # VMD port A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[41]|0x0                                  # VMD port B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[42]|0x0                                  # VMD port C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[43]|0x0                                  # VMD port D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[44]|0x0                                  # VMD port A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[45]|0x0                                  # VMD port B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[46]|0x0                                  # VMD port C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[47]|0x0                                  # VMD port D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[52]|0x0                                  # VMD port A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[53]|0x0                                  # VMD port B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[54]|0x0                                  # VMD port C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[55]|0x0                                  # VMD port D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[56]|0x0                                  # VMD port A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[57]|0x0                                  # VMD port B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[58]|0x0                                  # VMD port C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[59]|0x0                                  # VMD port D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[60]|0x0                                  # VMD port A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[61]|0x0                                  # VMD port B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[62]|0x0                                  # VMD port C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[63]|0x0                                  # VMD port D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[64]|0x0                                  # VMD port A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[65]|0x0                                  # VMD port B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[66]|0x0                                  # VMD port C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[67]|0x0                                  # VMD port D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[68]|0x0                                  # VMD port A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[69]|0x0                                  # VMD port B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[70]|0x0                                  # VMD port C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[71]|0x0                                  # VMD port D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[76]|0x0                                  # VMD port A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[77]|0x0                                  # VMD port B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[78]|0x0                                  # VMD port C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[79]|0x0                                  # VMD port D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[80]|0x0                                  # VMD port A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[81]|0x0                                  # VMD port B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[82]|0x0                                  # VMD port C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[83]|0x0                                  # VMD port D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[84]|0x0                                  # VMD port A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[85]|0x0                                  # VMD port B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[86]|0x0                                  # VMD port C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[87]|0x0                                  # VMD port D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[88]|0x0                                  # VMD port A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[89]|0x0                                  # VMD port B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[90]|0x0                                  # VMD port C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[91]|0x0                                  # VMD port D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[92]|0x0                                  # VMD port A
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[93]|0x0                                  # VMD port B
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[94]|0x0                                  # VMD port C
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VMDPortEnable[95]|0x0                                  # VMD port D
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VTdSupport|0x1                                         # Intel� VT for Directed I/O
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VtdDisabledBitmask[0]|0x0                              # Intel� VT-d Disable Mask
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VtdDisabledBitmask[1]|0x0                              # Intel� VT-d Disable Mask
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VtdDisabledBitmask[2]|0x0                              # Intel� VT-d Disable Mask
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VtdDisabledBitmask[3]|0x0                              # Intel� VT-d Disable Mask
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VtdPciAcsCtlBit0|0x0                                   # Source Validation
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VtdPciAcsCtlBit1|0x0                                   # Translation Blocking
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VtdPciAcsCtlBit2|0x1                                   # P2P Request Redirect
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VtdPciAcsCtlBit3|0x1                                   # P2P Completion Redirect
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VtdPciAcsCtlBit4|0x1                                   # Upstream Forwarding Enable
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.VtdPciAcsCtlWaEn|0x0                                   # PCIe ACSCTL
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.X2ApicOptOut|0x0                                       # X2APIC Opt Out
gStructPcdTokenSpaceGuid.PcdSocketIioConfig.Xppdef|0x0                                             # Retimer workaround
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ADDDCEn|0x0                                         # ADDDC Sparing
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ADRDataSaveMode|0x2                                 # ADR Data Save Mode
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ADREn|0x1                                           # Enable ADR
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AdddcErrInjEn|0x1                                   # Enable ADDDC Error Injection
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AdvMemTestCondPause|0x186a0                         # Adv MemTest Pause
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AdvMemTestCondTrefi|0x3cf0                          # Adv MemTest tREFI
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AdvMemTestCondTwr|0xa                               # Adv MemTest tWR
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AdvMemTestCondVdd|0x4c4                             # Adv MemTest VDD Level
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AdvMemTestCondition|0x1                             # Adv MemTest Conditions
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AdvMemTestOptions|0x0                               # Adv MemTest Options
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AdvMemTestPpr|0x1                                   # Adv MemTest PPR
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AdvMemTestRankList[0]|0xffffffff                    # Rank location entry 0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AdvMemTestRankList[1]|0xffffffff                    # Rank location entry 1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AdvMemTestRankList[2]|0xffffffff                    # Rank location entry 2
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AdvMemTestRankList[3]|0xffffffff                    # Rank location entry 3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AdvMemTestRankList[4]|0xffffffff                    # Rank location entry 4
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AdvMemTestRankList[5]|0xffffffff                    # Rank location entry 5
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AdvMemTestRankList[6]|0xffffffff                    # Rank location entry 6
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AdvMemTestRankList[7]|0xffffffff                    # Rank location entry 7
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AdvMemTestRankListNumEntries|0x0                    # Number of Ranks to Test
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AdvMemTestResetList|0x0                             # Adv MemTest Reset Failure Tracking List
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AdvMemTestRetryAfterRepair|0x1                      # Adv MemTest Retry After Repair
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AepNotSupportedException|0x0                        # 100 series PMem Not Supported Exception
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ApdEn|0x0                                           # APD
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AppDirectMemoryHole|0x0                             # App Direct Memory Hole
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AttemptFastBoot|0x1                                 # Attempt Fast Boot
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.AttemptFastBootCold|0x1                             # Attempt Fast Cold Boot
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS92|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS99|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS100|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS101|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS104|0x2
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS89|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS93|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS102|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS103|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS91|0x10
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS88|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS90|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS95|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS96|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS94|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS97|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS98|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.Blockgnt2cmd1cyc|0x1                                # BLOCK GNT2CMD1CYC
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.BwLimitTfOvrd|0x0                                   # Override BW_LIMIT_TF
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.CacheMemType|0x0                                    # 2LM Memory Type
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.CkMode|0x2                                          # CK in SR
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.CkeIdleTimer|0x14                                   # CKE Idle Timer
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.CkeProgramming|0x0                                  # CKE Throttling
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.CmdNormalization|0x1                                # Command Normalization
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.CmdTxEqCalibration|0x0                              # CMD Tx Eq Training
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.CmdVrefEnable|0x1                                   # Command Vref Training
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.CmiInitOption|0x0                                   # CMI Init Option
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.CmsEnableDramPm|0x1                                 # CMS ENABLE DRAM PM
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ColumnCorrectionDisable|0x0                         # Column Correction Disable
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.CustomRefreshRate|0x14                              # Custom Refresh Rate
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.CustomRefreshRateEn|0x0                             # Custom Refresh Enable
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.DataDllOff|0x1                                      # Data DLL Off EN
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.DcpmmAveragePowerLimit|0x3a98                       # 200 Series PMem Average Power Limit (in mW)
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.DcpmmAveragePowerTimeConstant|0x6                   # 200 Series PMem Turbo Average Power Time Constant (in mSec)
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.DcpmmEccModeSwitch|0x1                              # ECC Mode Switch
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.DcpmmMbbAveragePowerTimeConstant|0x3a98             # 200 Series PMem Memory Bandwidth Boost(MBB) Average Power Time Constant (in mSec)
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.DcpmmMbbFeature|0x1                                 # 200 Series PMem Turbo/Memory Bandwidth Boost(MBB) Feature
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.DcpmmMbbMaxPowerLimit|0x4650                        # 200 Series PMem Turbo/Memory Bandwidth Boost Feature(MBB) Power Limit (in mW)
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.DdjcTraining|0x1                                    # DDJC Training
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.DdrCacheSize|0xff                                   # DDR Cache Size
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.DdrFreqLimit|0x0                                    # Memory Frequency
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.DdrMemoryType|0x2                                   # Memory Type
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.DdrtCkeEn|0x1                                       # PMem CKE
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.DdrtSrefEn|0x0                                      # PMem SELF REFRESH
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.DfeGainBias|0x0                                     # DfeGainBias
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS48|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS127|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS122|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS121|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS135|0x2
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS129|0x2
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS120|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS136|0x2
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS139|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS132|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS63|0x7
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS62|0xf
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS67|0x2
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS65|0x7ff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS66|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS73|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS109|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS78|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS124|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS112|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS110|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS111|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS140|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS141|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS142|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS146|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS147|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS148|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS131|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS57|0x7ff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS56|0x7ff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS61|0x1ff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS51|0x1f
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS52|0x7f
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS50|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS60|0x3f
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS58|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS59|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS55|0x7ff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS53|0x7ff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS54|0x7ff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS70|0x2
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS79|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS117|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS119|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS118|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS137|0x2
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS128|0x2
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS123|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS80|0x8
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS149|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS125|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS64|0x7ff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS113|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS126|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS134|0x2
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS130|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS145|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS49|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS69|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS133|0x2
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.DieSparing|0x1                                      # Set PMem Die Sparing
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.DisableDirForAppDirect|0x0                          # Snoopy mode for AD
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.Disddrtopprd|0x1                                    # Disable DDRT DIMM OPPRD
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.DramRaplEnable|0x1                                  # DRAM RAPL
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.DramRaplExtendedRange|0x1                           # DRAM RAPL Extended Range
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.DramRaplPwrLimitLockCsr|0x1                         # DRAM RAPL Power Limit Lock CSR
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.DramRonEn|0x0                                       # DRAM RON Training
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.DutyCycleTraining|0x1                               # Duty Cycle Training
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.EadrCacheFlushMode|0x1                              # CPU Cache Flush Mode
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.EadrSupport|0x0                                     # eADR Support
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.EccEnable|0x1                                       # ECC Enable
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.EccSupport|0x1                                      # ECC Check
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.EliminateDirectoryInFarMemory|0x1                   # Snoopy mode for 2LM
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS84|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS85|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS87|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.EnableNgnBcomMargining|0x0                          # DDRT DIMM BCOM Margining Support
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.EnableTapSweep|0x0                                  # Enable Tap Sweep
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.EnableTwoWayNmCache|0x0                             # Enable biased 2-way near memory cache
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.EnforcePOR|0x0                                      # Enforce POR
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.EnforcePopulationPor|0x1                            # Enforce Population POR
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.EsddcEn|0x0                                         # Enhanced SDDC
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ExtendedType17|0x1                                  # Extended Type 17 Structure
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.FactoryResetClear|0x0                               # PMem Factory Reset/Clear
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.FadrSupport|0x0                                     # fADR Support
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.FastGoConfig|0x6                                    # PMem FastGo Configuration
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.FastZeroMemSupport|0x0                              # Boot-time Fast Zero Memory
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.FourxRefreshValue|0x5f                              # Set FOURx Temperature Refresh
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.HalfxRefreshValue|0x19                              # Set Halfx Temperature Refresh
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.HwMemTest|0x1                                       # MemTest
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.LatchSystemShutdownState|0x1                        # PMem Latch System Shutdown State
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.LegacyADRModeEn|0x0                                 # Legacy ADR Mode
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.LegacyRmt|0x0                                       # Rank Margin Tool
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.LockNgnCsr|0x0                                      # Lock PMem CSRs
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.LrDimmBacksideVrefEn|0x1                            # LRDIMM Backside Vref
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.LrDimmRdVrefEn|0x1                                  # LRDIMM Read Vref
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.LrDimmRxDqCentering|0x1                             # LRDIMM RX DQ Centering
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.LrDimmTxDqCentering|0x1                             # LRDIMM TX DQ Centering
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.LrDimmWrVrefEn|0x1                                  # LRDIMM Write Vref
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.LrdimmDbDfeTraining|0x1                             # LRDIMM DB DFE Training
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.LsxImplementation|0x1                               # LSx implementation
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.McRonEn|0x0                                         # MC RON Training
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.MdllOffEn|0x1                                       # MDLL OFF
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.MemCeFloodPolicy|0x2                                # Memory Correctable Error Flood Policy
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.MemHotIn|0x1                                        # MemHot Input Pin
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.MemHotOuputAssertThreshold|0x1                      # MEMHOT Output Throttling Mode Options
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.MemHotOut|0x1                                       # MemHot Output Pin
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.MemTestLoops|0x1                                    # MemTest Loops
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.MemTestOnColdFastBoot|0x0                           # MemTest On Cold Fast Boot
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.MemTripReporting|0x0                                # MEMTRIP REPORTING
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.MinNormalMemSize|0x2                                # Minimum System Memory Size
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.MultiThreaded|0x1                                   # Multi-Threaded MRC
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.NfitPublishMailboxStructs|0x0                       # NVDIMM Mailbox in NFIT
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.NgnArsPublish|0x1                                   # Publish ARS capability
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.NgnCmdTime|0x2                                      # PMem CMD Time
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.NgnDebugLock|0x0                                    # Debug lock for PMem
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.NgnEccCorr|0x2                                      # PMem ECC Correctable error
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.NgnEccExitCorr|0x2                                  # PMem ECC Write Retry Flow Exit
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.NgnEccRdChk|0x1                                     # PMem ECC Read Check
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.NgnEccWrChk|0x2                                     # PMem ECC Write Check
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.NonPreferredWayMask|0x1                             # Non-preferred way mask
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.NsddcEn|0x1                                         # New SDDC Mode
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.NvDimmEnergyPolicy|0x0                              # NVDIMM Energy Policy
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.NvdimmSmbusMaxAccessTime|0x15e                      # SMBus Max Access Time
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.NvdimmSmbusReleaseDelay|0x96                        # SMBus Release Delay
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.NvmMediaStatusException|0x1                         # PMem MediaStatus Exception
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.NvmQos|0x0                                          # PMem QoS
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.NvmdimmPerfConfig|0x0                               # PMem Performance Setting
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.NvmdimmPowerCyclePolicy|0x1                         # Power Cycle on PMem Surprise Clock Stop
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.OffPkgMemToMemTrip|0x0                              # OFF PKG MEM TO MEMTRIP
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.OffPkgMemToThermTrip|0x0                            # OFF PKG MEM TO THERMTRIP
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.OffsetCmdAll|0x64                                   # Offset CmdAll
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.OffsetCmdVref|0x64                                  # Offset CmdVref
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.OffsetCtlAll|0x64                                   # Offset CtlAll
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.OffsetRecEn|0x64                                    # Offset RecEnDelay
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.OffsetRxDq|0x64                                     # Offset RxDq
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.OffsetRxVref|0x64                                   # Offset RxVref
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.OffsetTxDq|0x64                                     # Offset TxDq
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.OffsetTxVref|0x64                                   # Offset TxVref
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.OppSrefEn|0x0                                       # Opportunistic SR
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.PagePolicy|0x2                                      # Page Policy
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.PanicWm|0x0                                         # Refresh Watermarks
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.PartialMirrorUefi|0x0                               # UEFI ARM Mirror
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.PartialMirrorUefiPercent|0x0                        # ARM Mirror percentage
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.PatrolScrub|0x2                                     # Patrol Scrub
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.PatrolScrubAddrMode|0x1                             # Patrol Scrub Address Mode
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.PatrolScrubDuration|0x18                            # Patrol Scrub Interval
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.PclsEn|0x1                                          # Partial Cache Line Sparing PCLS
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.PeriodicRcomp|0x2                                   # Periodic Rcomp
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.PeriodicRcompInterval|0xf                           # Periodic Rcomp Interval
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.PkgcSrefEn|0x1                                      # PKGC SREF EN
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.PlusOneEn|0x0                                       # Plus One | SDDC
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.PmemCaching|0x0                                     # AppDirect cache
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.PpdEn|0x1                                           # PPD
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.PreferredReadFirst|0x1                              # Preferred read first
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.PxcTraining|0x1                                     # PXC Training
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.RankSparing|0x0                                     # Memory Rank Sparing
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReadPreamble|0xff                                   # Read Preamble TCLK
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.RecEnDelayAverage|0x2                               # Rank Switch Configuration
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.RmtMinimumMarginCheck|0x1                           # RMT Minimum Margin Check
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS86|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.RmtOnColdFastBoot|0x0                               # RMT On Cold Fast Boot
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.RoundTripLatency|0x1                                # RoundTrip Latency Training
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.RxCtleTrnEn|0x1                                     # Rx CTLE Training
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.RxDfe|0x0                                           # Rx Dfe Training
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.RxOdtEn|0x1                                         # Rx ODT Training
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.SaiPolicyGroupWaBiosW|0x0                           # Enable Pcode WA for SAI PG
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ScrambleEn|0x1                                      # Data Scrambling for DDR4/5
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ScrambleEnDDRT|0x1                                  # Data Scrambling for PMem
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ScrambleSeedHigh|0xd395                             # Scrambling Seed High
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ScrambleSeedLow|0xa02b                              # Scrambling Seed Low
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.SetMemTested|0x1                                    # Allow Untested Memory for DXE Drivers
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ShortStroke2GB|0x0                                  # 2GB Short Stroke Configuration
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.SmartTestKey|0x0                                    # SmartTestKey
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.SmbSpdAccess|0x0                                    # SPD-SMBUS Access
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.SpareSwErrTh|0x4                                    # SW Per Bank Threshold
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.SpdPrintEn|0x0                                      # SPD Print
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.SpdPrintLength|0x0                                  # SPD Print Length
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.Srat|0x1                                            # Publish SRAT
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.SratCpuHotPlug|0x0                                  # SRAT CPU Hot Plug
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.SratMemoryHotPlug|0x0                               # SRAT Memory Hot Plug
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.SrefProgramming|0x0                                 # SREF Feature
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.Tap1End|0xa                                         # Tap1End
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.Tap1Size|0x1                                        # Tap1Step
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.Tap1Start|0x8c                                      # Tap1Start
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.Tap2End|0xf                                         # Tap2End
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.Tap2Size|0x1                                        # Tap2Step
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.Tap2Start|0x73                                      # Tap2Start
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.Tap3End|0xc                                         # Tap3End
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.Tap3Size|0x1                                        # Tap3Step
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.Tap3Start|0x70                                      # Tap3Start
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.Tap4End|0x9                                         # Tap4End
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.Tap4Size|0x1                                        # Tap4Step
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.Tap4Start|0x6d                                      # Tap4Start
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.TcoCompTraining|0x0                                 # Tco Comp Training
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.TempRefreshOption|0x0                               # Select Temperature Refresh Value
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.TrainingCompOptions|0x0                             # Training Compensation Options Values
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.TrainingResultOffsetFunctionEnable|0x0              # Training Result Offset
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.TrefiDelay|0x0                                      # Additional TREFI Delay
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.TrefiNumofRank|0x1                                  # The Number of Ranks for Stagger TREFI
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.TrefiPerChannel|0x0                                 # Stagger TREFI Per Available Channel
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.TriggerSWErrThEn|0x0                                # Trigger SW Error Threshold
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.TwoxRefreshValue|0x53                               # Set TWOx Temperature Refresh
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.TxRiseFallSlewRate|0x2                              # TX Rise Fall Slew Rate Training
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.Vdd|0x4b0                                           # Memory Voltage
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.WrCRC|0x0                                           # WR CRC feature Control
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.WritePreamble|0xff                                  # Write Preamble TCLK
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.XMPMode|0x0                                         # XMP Profile
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.XMPProfilesSup|0x0                                  # XMP Profiles Supported
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.allowCorrectableError|0x0                           # Allow Memory Training Correctable Error
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.allowCorrectableMemTestError|0x1                    # Allow Memory Test Correctable Error
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.bdatEn|0x1                                          # BDAT
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.check_platform_detect|0x0                           # Check PlatformDetectADR
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.check_pm_sts|0x0                                    # Check PCH_PM_STS
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.cmdSetupPercentOffset|0x32                          # Cmd Setup % Offset
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.commandTiming|0x0                                   # Command Timing
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS83|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS77|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS72|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS68|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS106|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS108|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS105|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS71|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS74[0]|0x3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS74[1]|0x3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS74[2]|0x3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS74[3]|0x3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS74[4]|0x3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS74[5]|0x3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS74[6]|0x3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS74[7]|0x3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS74[8]|0x3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS74[9]|0x3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS74[10]|0x3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS74[11]|0x3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS74[12]|0x3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS74[13]|0x3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS74[14]|0x3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS74[15]|0x3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS75|0x1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[0]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[1]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[3]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[4]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[6]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[7]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[9]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[10]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[12]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[13]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[15]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[16]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[18]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[19]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[21]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[22]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[24]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[25]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[27]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[28]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[30]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[31]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[33]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[34]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[36]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[37]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[39]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[40]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[42]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[43]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[45]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS81[46]|0xff
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS82|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS107|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.dimmIsolation|0x0                                   # DIMM Isolation Enable
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.dllResetTestLoops|0x0                               # DLL Reset Test
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.eraseArmNVDIMMS|0x1                                 # Erase-Arm NVDIMMs
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.eyeDiagram|0x0                                      # Eye Diagrams
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.haltOnMemErr|0x1                                    # Halt on mem Training Error
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.iModeTraining|0x1                                   # iMode Training
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.imcBclk|0x0                                         # IMC BCLK
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.interNVDIMMS|0x1                                    # Interleave NVDIMMs
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.leakyBktHi|0x29                                     # Leaky bucket high bit
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.leakyBktHour|0x18                                   # Leaky bucket time window based interface Hour
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.leakyBktLo|0x28                                     # Leaky bucket low bit
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.leakyBktMinute|0x0                                  # Leaky bucket time window based interface Minute
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.leakyBktTimeWindow|0x0                              # Leaky bucket time window based interface
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.logParsing|0x0                                      # Enhanced Log Parsing
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.lrdimmModuleDelay|0x0                               # LRDIMM Module Delay
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.mdllSden|0x2                                        # MDLL Off
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.memFlows|0xffffffff                                 # Mem Flows
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.memFlowsExt|0xffffffff                              # Mem FlowsExt
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.memFlowsExt2|0xffffffff                             # Mem FlowsExt2
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.memFlowsExt3|0xffffffff                             # Mem FlowsExt3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.memInterleaveGran1LM|0x3                            # 1LM Memory Interleave Granularity
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.memhotSupport|0x1                                   # MEMHOT Throttling Mode
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.mrcRepeatTest|0x0                                   # DDR Cycling
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.multiSparingRanks|0x2                               # Multi Rank Sparing
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.normOppInterval|0x400                               # Normal Operation Duration
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.oneRankTimingMode|0x1                               # One Rank Timing Mode
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.oppReadInWmm|0x1                                    # Opp read during WMM
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.partialmirrorsad0|0x0                               # Mirror TAD0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.partialmirrorsize[0]|0x0                            # Partial Mirror 1 Size (GB)
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.partialmirrorsize[1]|0x0                            # Partial Mirror 2 Size (GB)
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.partialmirrorsize[2]|0x0                            # Partial Mirror 3 Size (GB)
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.partialmirrorsize[3]|0x0                            # Partial Mirror 4 Size (GB)
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.pda|0x1                                             # PDA
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.pprErrInjTest|0x0                                   # PPR Error Injection test
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.pprType|0x2                                         # PPR Type
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.promoteMrcWarnings|0x0                              # MRC Promote Warnings
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.promoteWarnings|0x1                                 # Promote Warnings
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.readVrefCenter|0x1                                  # Read Vref Centering
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.refreshMode|0x2                                     # 2x Refresh Enable
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.restoreNVDIMMS|0x1                                  # Restore NVDIMMs
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseAllDIMMs|0x0                          # Erase All DIMMs
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[0]|0x0                          # S0 CH0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[1]|0x0                          # S0 CH1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[2]|0x0                          # S0 CH2
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[3]|0x0                          # S0 CH3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[4]|0x0                          # S0 CH4
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[5]|0x0                          # S0 CH5
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[6]|0x0                          # S0 CH6
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[7]|0x0                          # S0 CH7
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[8]|0x0                          # S1 CH0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[9]|0x0                          # S1 CH1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[10]|0x0                         # S1 CH2
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[11]|0x0                         # S1 CH3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[12]|0x0                         # S1 CH4
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[13]|0x0                         # S1 CH5
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[14]|0x0                         # S1 CH6
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[15]|0x0                         # S1 CH7
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[16]|0x0                         # S2 CH0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[17]|0x0                         # S2 CH1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[18]|0x0                         # S2 CH2
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[19]|0x0                         # S2 CH3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[20]|0x0                         # S2 CH4
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[21]|0x0                         # S2 CH5
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[22]|0x0                         # S2 CH6
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[23]|0x0                         # S2 CH7
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[24]|0x0                         # S3 CH0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[25]|0x0                         # S3 CH1
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[26]|0x0                         # S3 CH2
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[27]|0x0                         # S3 CH3
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[28]|0x0                         # S3 CH4
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[29]|0x0                         # S3 CH5
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[30]|0x0                         # S3 CH6
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.setSecureEraseSktCh[31]|0x0                         # S3 CH7
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.smbSpeed|0x2                                        # SMB Clock Frequency
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.spareErrTh|0x7fff                                   # Correctable Error Threshold
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.spdCrcCheck|0x2                                     # SPD CRC Check
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.tCAS|0x0                                            # CAS Latency
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.tCWL|0x0                                            # tCWL
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.tFAW|0x0                                            # tFAW
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.tRAS|0x0                                            # tRAS
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.tRC|0x0                                             # tRC
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.tRCD|0x0                                            # tRCD
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.tREFI|0x0                                           # Refresh Rate
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.tRFC|0x0                                            # tRFC
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.tRP|0x0                                             # tRP
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.tRRD|0x0                                            # tRRD
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.tRRD_L|0x0                                          # tRRD_L
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.tRTP|0x0                                            # tRTP
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.tWR|0x0                                             # tWR
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.tWTR|0x0                                            # tWTR
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.thermalthrottlingsupport|0x2                        # Throttling Mode
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.timeWindow|0x18                                     # SW Correctable Error Time Window
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.turnaroundOpt|0x1                                   # Turnaround Time Optimization
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.turnaroundOptDdrt|0x1                               # Turnaround Time Optimization PMem
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.txEqCalibration|0x1                                 # Tx Eq Training
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.volMemMode|0x0                                      # Volatile Memory Mode
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.wrVrefCenter|0x1                                    # Write Vref Centering
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.BusRatio[0]|0x1                                     # Bus Resources Allocation Ratio
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.BusRatio[1]|0x1                                     # Bus Resources Allocation Ratio
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.BusRatio[2]|0x1                                     # Bus Resources Allocation Ratio
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.BusRatio[3]|0x1                                     # Bus Resources Allocation Ratio
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu0P0ReservedS189|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu0P0ReservedS246|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu0P0ReservedS190|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu0P0ReservedS191|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu0P0KtiLinkSpeed|0x3                              # Current UPI Link Speed
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu0P0KtiLinkVnaOverride|0x7f                       # UPI VNA Credit Override
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu0P0KtiPortDisable|0x0                            # Link Disable
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu0P1ReservedS189|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu0P1ReservedS246|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu0P1ReservedS190|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu0P1ReservedS191|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu0P1KtiLinkSpeed|0x3                              # Current UPI Link Speed
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu0P1KtiLinkVnaOverride|0x7f                       # UPI VNA Credit Override
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu0P1KtiPortDisable|0x0                            # Link Disable
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu0P2ReservedS189|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu0P2ReservedS246|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu0P2ReservedS190|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu0P2ReservedS191|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu0P2KtiLinkSpeed|0x3                              # Current UPI Link Speed
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu0P2KtiLinkVnaOverride|0x7f                       # UPI VNA Credit Override
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu0P2KtiPortDisable|0x0                            # Link Disable
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu1P0ReservedS189|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu1P0ReservedS246|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu1P0ReservedS190|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu1P0ReservedS191|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu1P0KtiLinkSpeed|0x3                              # Current UPI Link Speed
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu1P0KtiLinkVnaOverride|0x7f                       # UPI VNA Credit Override
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu1P0KtiPortDisable|0x0                            # Link Disable
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu1P1ReservedS189|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu1P1ReservedS246|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu1P1ReservedS190|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu1P1ReservedS191|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu1P1KtiLinkSpeed|0x3                              # Current UPI Link Speed
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu1P1KtiLinkVnaOverride|0x7f                       # UPI VNA Credit Override
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu1P1KtiPortDisable|0x0                            # Link Disable
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu1P2ReservedS189|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu1P2ReservedS246|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu1P2ReservedS190|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu1P2ReservedS191|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu1P2KtiLinkSpeed|0x3                              # Current UPI Link Speed
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu1P2KtiLinkVnaOverride|0x7f                       # UPI VNA Credit Override
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu1P2KtiPortDisable|0x0                            # Link Disable
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu2P0ReservedS189|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu2P0ReservedS246|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu2P0ReservedS190|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu2P0ReservedS191|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu2P0KtiLinkSpeed|0x3                              # Current UPI Link Speed
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu2P0KtiLinkVnaOverride|0x7f                       # UPI VNA Credit Override
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu2P0KtiPortDisable|0x0                            # Link Disable
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu2P1ReservedS189|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu2P1ReservedS246|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu2P1ReservedS190|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu2P1ReservedS191|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu2P1KtiLinkSpeed|0x3                              # Current UPI Link Speed
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu2P1KtiLinkVnaOverride|0x7f                       # UPI VNA Credit Override
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu2P1KtiPortDisable|0x0                            # Link Disable
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu2P2ReservedS189|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu2P2ReservedS246|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu2P2ReservedS190|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu2P2ReservedS191|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu2P2KtiLinkSpeed|0x3                              # Current UPI Link Speed
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu2P2KtiLinkVnaOverride|0x7f                       # UPI VNA Credit Override
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu2P2KtiPortDisable|0x0                            # Link Disable
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu3P0ReservedS189|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu3P0ReservedS246|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu3P0ReservedS190|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu3P0ReservedS191|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu3P0KtiLinkSpeed|0x3                              # Current UPI Link Speed
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu3P0KtiLinkVnaOverride|0x7f                       # UPI VNA Credit Override
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu3P0KtiPortDisable|0x0                            # Link Disable
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu3P1ReservedS189|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu3P1ReservedS246|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu3P1ReservedS190|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu3P1ReservedS191|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu3P1KtiLinkSpeed|0x3                              # Current UPI Link Speed
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu3P1KtiLinkVnaOverride|0x7f                       # UPI VNA Credit Override
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu3P1KtiPortDisable|0x0                            # Link Disable
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu3P2ReservedS189|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu3P2ReservedS246|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu3P2ReservedS190|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu3P2ReservedS191|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu3P2KtiLinkSpeed|0x3                              # Current UPI Link Speed
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu3P2KtiLinkVnaOverride|0x7f                       # UPI VNA Credit Override
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Cpu3P2KtiPortDisable|0x0                            # Link Disable
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.D2KCreditConfig|0x2                                 # D2K Credit configuration
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.Degrade4SPreference|0x0                             # Degraded 4S Topology Preference
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.DegradePrecedence|0x0                               # Degrade Precedence
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS154|0x5
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS159|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS176|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS181|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS182|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS180|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS178|0x3
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS179|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS183|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS167|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS168|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS158|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS164|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS157|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS163|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS151|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS156|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS162|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS187|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS186|0xffffffff
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS152|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS153|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS169|0x4
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS185|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS173|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS177|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS160|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS161|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS170|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS171|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS172|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS184|0x3
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS174|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS155|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS165|0x1
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS166|0x2
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.ReservedS175|0x5
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.DirectoryModeEn|0x2                                 # Directory Mode Enable
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.IoDcMode|0x1                                        # IO Directory Cache (IODC)
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.IrqThreshold|0x1                                    # Local/Remote Threshold
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.KtiAdaptationEn|0x2                                 # UPI Adaptation Enable
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.KtiAdaptationSpeed|0x3                              # UPI Adaptation Speed
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.KtiCpuSktHotPlugTopology|0x0                        # CPU Hot Plug Topology
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.KtiCrcMode|0x2                                      # CRC Mode
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.KtiFailoverEn|0x2                                   # UPI Dynamic Link Width Reduction Support
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.KtiLbEn|0x0                                         # UPI Load Board for Failed Links
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.KtiLinkL0pEn|0x2                                    # Link L0p Enable
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.KtiLinkL1En|0x2                                     # Link L1 Enable
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.KtiLinkVnaOverride|0x7f                             # UPI VNA Credit Override
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.KtiPrefetchEn|0x2                                   # KTI Prefetch
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.KtiSkuMismatchCheck|0x1                             # CPU SKU Type Mismatch check
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.LLCDeadLineAlloc|0x1                                # LLC dead line alloc
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.LegacyVgaSoc|0x0                                    # Legacy VGA Socket
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.LegacyVgaStack|0x0                                  # Legacy VGA Stack
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.MbeBwCal|0x3                                        # MBA BW Calibration
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.P2pRelaxedOrdering|0x0                              # PCIe Remote P2P Relaxed Ordering
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.QpiCpuSktHotPlugEn|0x0                              # CPU Hot Plug
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.QpiLinkSpeed|0x3                                    # Link Frequency Select
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.QpiLinkSpeedMode|0x1                                # Link Speed Mode
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.RdCurForXptPrefetchEn|0x2                           # RdCur for XPT Prefetch
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.SncEn|0x0                                           # SNC (Sub NUMA)
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.SnoopAllCores|0x2                                   # Snoop All Cores Configuration
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.SnoopThrottleConfig|0x4                             # Snoop Throttle Configuration
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.SplitLock|0x0                                       # SplitLock
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.StaleAtoSOptEn|0x2                                  # Stale AtoS
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.TorThresLoctoremEmpty|0x1                           # Loctorem Thresholds Empty
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.TorThresLoctoremNorm|0x1                            # Loctorem Thresholds Normal
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.TscSyncEn|0x2                                       # TSC Sync support
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.XptPrefetchEn|0x2                                   # XPT Prefetch
gStructPcdTokenSpaceGuid.PcdSocketMpLinkConfig.XptRemotePrefetchEn|0x2                             # XPT Remote Prefetch
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.AcpiS3Enable|0x0                           # ACPI S3
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.AcpiS4Enable|0x0                           # ACPI S4
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.AllowLpStateMdfs[0]|0x0                    # Allow LP state
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.AllowLpStateMdfs[1]|0x0                    # Allow LP state
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.AllowLpStateMdfs[2]|0x0                    # Allow LP state
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.AllowLpStateMdfs[3]|0x0                    # Allow LP state
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.AltEngPerfBIAS|0x7                         # ENERGY_PERF_BIAS_CFG mode
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.AvxIccpLevel|0x1                           # AVX ICCP pre-grant level
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.AvxLicensePreGrant|0x0                     # AVX Licence Pre-Grant Override
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.AvxSupport|0x1                             # AVX Support
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.BootPState|0x0                             # Boot performance mode
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.C1AutoDemotion|0x1                         # CPU C1 auto demotion
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.C1AutoUnDemotion|0x1                       # CPU C1 auto undemotion
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.C2C3TT|0x0                                 # C2C3TT
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.C6Enable|0xff                              # CPU C6 report
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.CStateLatencyCtrlMultiplier[0]|0x0         # MULTIPLIER:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.CStateLatencyCtrlMultiplier[1]|0x0         # MULTIPLIER:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.CStateLatencyCtrlMultiplier[2]|0x0         # MULTIPLIER:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.CStateLatencyCtrlValid[0]|0x0              # VALID:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.CStateLatencyCtrlValid[1]|0x0              # VALID:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.CStateLatencyCtrlValid[2]|0x0              # VALID:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.CStateLatencyCtrlValue[0]|0x0              # VALUE:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.CStateLatencyCtrlValue[1]|0x0              # VALUE:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.CStateLatencyCtrlValue[2]|0x0              # VALUE:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.ConfigTdpLevel|0x0                         # AVX P1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.ConfigTdpLock|0x1                          # Config TDP Lock
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.CurrentConfig|0x0                          # Current Limit Override
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.CurrentLimit|0x438                         # Current Limitation
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.DynamicIss|0x0                             # Dynamic SST-PP
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.DynamicL1|0x0                              # Dynamic L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EETurboDisable|0x0                         # Energy Efficient Turbo
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EetOverrideEn|0x0                          # EET Mode
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnableLinkInL1Iio[0]|0x0                   # Link in L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnableLinkInL1Iio[1]|0x0                   # Link in L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnableLinkInL1Iio[2]|0x0                   # Link in L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnableLinkInL1Iio[3]|0x0                   # Link in L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnableLinkInL1Kti[0]|0x0                   # Link in L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnableLinkInL1Kti[1]|0x0                   # Link in L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnableLinkInL1Kti[2]|0x0                   # Link in L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnableLinkInL1Kti[3]|0x0                   # Link in L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnableLinkInL1Rlink[0]|0x0                 # Link in L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnableLinkInL1Rlink[1]|0x0                 # Link in L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnableLinkInL1Rlink[2]|0x0                 # Link in L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnableLinkInL1Rlink[3]|0x0                 # Link in L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnableLowerLatencyMode|0x0                 # Register Access Low Latency Mode
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaDino[0]|0x0              # PKGC_CRITERIA DINO
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaDino[1]|0x0              # PKGC_CRITERIA DINO
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaDino[2]|0x0              # PKGC_CRITERIA DINO
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaDino[3]|0x0              # PKGC_CRITERIA DINO
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaFxr[0]|0x0               # PKGC_CRITERIA FXR
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaFxr[1]|0x0               # PKGC_CRITERIA FXR
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaFxr[2]|0x0               # PKGC_CRITERIA FXR
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaFxr[3]|0x0               # PKGC_CRITERIA FXR
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaHbm[0]|0x1               # PKGC_CRITERIA HBMx
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaHbm[1]|0x1               # PKGC_CRITERIA HBMx
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaHbm[2]|0x1               # PKGC_CRITERIA HBMx
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaHbm[3]|0x1               # PKGC_CRITERIA HBMx
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaHcx[0]|0x0               # PKGC_CRITERIA HCX
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaHcx[1]|0x0               # PKGC_CRITERIA HCX
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaHcx[2]|0x0               # PKGC_CRITERIA HCX
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaHcx[3]|0x0               # PKGC_CRITERIA HCX
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaHqm[0]|0x0               # PKGC_CRITERIA HQM
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaHqm[1]|0x0               # PKGC_CRITERIA HQM
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaHqm[2]|0x0               # PKGC_CRITERIA HQM
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaHqm[3]|0x0               # PKGC_CRITERIA HQM
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaIio[0]|0x0               # PKGC_CRITERIA IIOx
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaIio[1]|0x0               # PKGC_CRITERIA IIOx
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaIio[2]|0x0               # PKGC_CRITERIA IIOx
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaIio[3]|0x0               # PKGC_CRITERIA IIOx
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaKti[0]|0x0               # PKGC_CRITERIA KTI
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaKti[1]|0x0               # PKGC_CRITERIA KTI
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaKti[2]|0x0               # PKGC_CRITERIA KTI
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaKti[3]|0x0               # PKGC_CRITERIA KTI
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaMcddr[0]|0xff            # PKGC_CRITERIA MCDDRx
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaMcddr[1]|0xff            # PKGC_CRITERIA MCDDRx
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaMcddr[2]|0xff            # PKGC_CRITERIA MCDDRx
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaMcddr[3]|0xff            # PKGC_CRITERIA MCDDRx
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaMdfs[0]|0x0              # PKGC_CRITERIA MDFS
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaMdfs[1]|0x0              # PKGC_CRITERIA MDFS
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaMdfs[2]|0x0              # PKGC_CRITERIA MDFS
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaMdfs[3]|0x0              # PKGC_CRITERIA MDFS
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaNac[0]|0x0               # PKGC_CRITERIA NAC
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaNac[1]|0x0               # PKGC_CRITERIA NAC
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaNac[2]|0x0               # PKGC_CRITERIA NAC
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaNac[3]|0x0               # PKGC_CRITERIA NAC
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaRlink[0]|0x0             # PKGC_CRITERIA Rlink
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaRlink[1]|0x0             # PKGC_CRITERIA Rlink
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaRlink[2]|0x0             # PKGC_CRITERIA Rlink
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaRlink[3]|0x0             # PKGC_CRITERIA Rlink
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaTip[0]|0x0               # PKGC_CRITERIA TIP
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaTip[1]|0x0               # PKGC_CRITERIA TIP
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaTip[2]|0x0               # PKGC_CRITERIA TIP
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgCCriteriaTip[3]|0x0               # PKGC_CRITERIA TIP
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnablePkgcCriteria|0xff                    # Enable PKGC_SA_PS_CRITERIA
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnableProcHot|0x3                          # PROCHOT Modes
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnableThermalMonitor|0x1                   # Thermal Monitor
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnableXe|0x1                               # Extreme Edition
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EngAvgTimeWdw1|0x1a                        # Averaging Time Window
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.EnhancedPmaxDetector|0x1                   # PMAX Detector Enhancement
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.FastRaplDutyCycle|0x40                     # FAST_RAPL_NSTRIKE_PL2_DUTY_CYCLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.GpssTimer|0x32                             # GPSS timer
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Iio0PkgcClkGateDis[0]|0x0                  # IIO0_PKGC_CLK_GATE_DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Iio0PkgcClkGateDis[1]|0x0                  # IIO0_PKGC_CLK_GATE_DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Iio0PkgcClkGateDis[2]|0x0                  # IIO0_PKGC_CLK_GATE_DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Iio0PkgcClkGateDis[3]|0x0                  # IIO0_PKGC_CLK_GATE_DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Iio1PkgcClkGateDis[0]|0x0                  # IIO1_PKGC_CLK_GATE_DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Iio1PkgcClkGateDis[1]|0x0                  # IIO1_PKGC_CLK_GATE_DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Iio1PkgcClkGateDis[2]|0x0                  # IIO1_PKGC_CLK_GATE_DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Iio1PkgcClkGateDis[3]|0x0                  # IIO1_PKGC_CLK_GATE_DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Iio2PkgcClkGateDis[0]|0x0                  # IIO2_PKGC_CLK_GATE_DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Iio2PkgcClkGateDis[1]|0x0                  # IIO2_PKGC_CLK_GATE_DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Iio2PkgcClkGateDis[2]|0x0                  # IIO2_PKGC_CLK_GATE_DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Iio2PkgcClkGateDis[3]|0x0                  # IIO2_PKGC_CLK_GATE_DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.InputUncoreFreq|0x7f                       # Uncore Freq:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.IoBwPlmtOvrdEn|0x0                         # IO_BW_PLIMIT_OVRD_EN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.IomApmOvrdEn|0x0                           # IOM_APM_OVERRIDE_ENABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.IssTdpLevel|0x0                            # Intel SST-PP
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti01PkgcClkGateDis[0]|0x0                 # UPI01_PKGC_CLK_GATE_DISABL
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti01PkgcClkGateDis[1]|0x0                 # UPI01_PKGC_CLK_GATE_DISABL
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti01PkgcClkGateDis[2]|0x0                 # UPI01_PKGC_CLK_GATE_DISABL
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti01PkgcClkGateDis[3]|0x0                 # UPI01_PKGC_CLK_GATE_DISABL
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti01pllOffEna[0]|0x1                      # UPI01 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti01pllOffEna[1]|0x1                      # UPI01 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti01pllOffEna[2]|0x1                      # UPI01 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti01pllOffEna[3]|0x1                      # UPI01 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti0In[0]|0x1                              # UPI_0_IN_L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti0In[1]|0x1                              # UPI_0_IN_L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti0In[2]|0x1                              # UPI_0_IN_L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti0In[3]|0x1                              # UPI_0_IN_L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti1In[0]|0x1                              # UPI_1_IN_L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti1In[1]|0x1                              # UPI_1_IN_L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti1In[2]|0x1                              # UPI_1_IN_L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti1In[3]|0x1                              # UPI_1_IN_L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti23PkgcClkGateDis[0]|0x0                 # UPI23_PKGC_CLK_GATE_DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti23PkgcClkGateDis[1]|0x0                 # UPI23_PKGC_CLK_GATE_DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti23PkgcClkGateDis[2]|0x0                 # UPI23_PKGC_CLK_GATE_DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti23PkgcClkGateDis[3]|0x0                 # UPI23_PKGC_CLK_GATE_DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti23pllOffEna[0]|0x1                      # UPI23 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti23pllOffEna[1]|0x1                      # UPI23 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti23pllOffEna[2]|0x1                      # UPI23 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti23pllOffEna[3]|0x1                      # UPI23 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti2In[0]|0x1                              # UPI_2_IN_L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti2In[1]|0x1                              # UPI_2_IN_L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti2In[2]|0x1                              # UPI_2_IN_L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti2In[3]|0x1                              # UPI_2_IN_L1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti45PkgcClkGateDis[0]|0x0                 # UPI45_PKGC_CLK_GATE_DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti45PkgcClkGateDis[1]|0x0                 # UPI45_PKGC_CLK_GATE_DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti45PkgcClkGateDis[2]|0x0                 # UPI45_PKGC_CLK_GATE_DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti45PkgcClkGateDis[3]|0x0                 # UPI45_PKGC_CLK_GATE_DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti45pllOffEna[0]|0x1                      # UPI45 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti45pllOffEna[1]|0x1                      # UPI45 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti45pllOffEna[2]|0x1                      # UPI45 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Kti45pllOffEna[3]|0x1                      # UPI45 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.KtiApmOvrdEn|0x0                           # UPI_APM_OVERRIDE_ENABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.LTRSwInput|0x1                             # LTR IIO Input
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Mc0PkgcClkGateDis[0]|0x0                   # MC0 PKGC CLK GATE DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Mc0PkgcClkGateDis[1]|0x0                   # MC0 PKGC CLK GATE DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Mc0PkgcClkGateDis[2]|0x0                   # MC0 PKGC CLK GATE DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Mc0PkgcClkGateDis[3]|0x0                   # MC0 PKGC CLK GATE DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Mc0pllOffEna[0]|0x1                        # MC0 PLLs Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Mc0pllOffEna[1]|0x1                        # MC0 PLLs Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Mc0pllOffEna[2]|0x1                        # MC0 PLLs Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Mc0pllOffEna[3]|0x1                        # MC0 PLLs Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Mc1PkgcClkGateDis[0]|0x0                   # MC1 PKGC CLK GATE DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Mc1PkgcClkGateDis[1]|0x0                   # MC1 PKGC CLK GATE DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Mc1PkgcClkGateDis[2]|0x0                   # MC1 PKGC CLK GATE DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Mc1PkgcClkGateDis[3]|0x0                   # MC1 PKGC CLK GATE DISABLE
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Mc1pllOffEna[0]|0x1                        # MC1 PLLs Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Mc1pllOffEna[1]|0x1                        # MC1 PLLs Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Mc1pllOffEna[2]|0x1                        # MC1 PLLs Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.Mc1pllOffEna[3]|0x1                        # MC1 PLLs Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.MonitorMWait|0x1                           # Enable Monitor MWAIT
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.NativeAspmEnable|0x2                       # Native ASPM
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.NonSnpLatMult|0x0                          # Non-Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.NonSnpLatOvrd|0x0                          # Non-Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.NonSnpLatVal|0x0                           # Non-Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.NonSnpLatVld|0x0                           # Non-Snoop Latency Override Valid
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.OSCx|0x0                                   # OS ACPI Cx
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.OnDieThermalThrottling|0x0                 # T-State Throttle Level
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.OverclockingLock|0x1                       # Overclocking Lock
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.P0TtlTimeHigh1|0x3f                        # P0 TotalTimeThreshold High
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.P0TtlTimeLow1|0x28                         # P0 TotalTimeThreshold Low
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.P0pllOffEna[0]|0x1                         # PCIe IIO0 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.P0pllOffEna[1]|0x1                         # PCIe IIO0 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.P0pllOffEna[2]|0x1                         # PCIe IIO0 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.P0pllOffEna[3]|0x1                         # PCIe IIO0 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.P1pllOffEna[0]|0x1                         # PCIe IIO1 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.P1pllOffEna[1]|0x1                         # PCIe IIO1 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.P1pllOffEna[2]|0x1                         # PCIe IIO1 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.P1pllOffEna[3]|0x1                         # PCIe IIO1 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.P2pllOffEna[0]|0x1                         # PCIe IIO2 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.P2pllOffEna[1]|0x1                         # PCIe IIO2 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.P2pllOffEna[2]|0x1                         # PCIe IIO2 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.P2pllOffEna[3]|0x1                         # PCIe IIO2 PLL Shutdown En
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PackageCState|0xff                         # Package C State
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio0In[0]|0x1                          # PCIE Port0 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio0In[1]|0x1                          # PCIE Port0 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio0In[2]|0x1                          # PCIE Port0 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio0In[3]|0x1                          # PCIE Port0 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio1In[0]|0x1                          # PCIE Port1 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio1In[1]|0x1                          # PCIE Port1 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio1In[2]|0x1                          # PCIE Port1 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio1In[3]|0x1                          # PCIE Port1 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio2In[0]|0x1                          # PCIE Port2 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio2In[1]|0x1                          # PCIE Port2 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio2In[2]|0x1                          # PCIE Port2 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio2In[3]|0x1                          # PCIE Port2 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio3In[0]|0x1                          # PCIE Port3 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio3In[1]|0x1                          # PCIE Port3 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio3In[2]|0x1                          # PCIE Port3 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio3In[3]|0x1                          # PCIE Port3 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio4In[0]|0x1                          # PCIE Port4 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio4In[1]|0x1                          # PCIE Port4 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio4In[2]|0x1                          # PCIE Port4 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio4In[3]|0x1                          # PCIE Port4 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio5In[0]|0x1                          # PCIE Port5 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio5In[1]|0x1                          # PCIE Port5 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio5In[2]|0x1                          # PCIE Port5 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcieIio5In[3]|0x1                          # PCIE Port5 PCIE_IN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PcodeWdogTimerEn|0x2                       # Pcode Dispatcher Watchdog Timer
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PerfPLimitClipC|0x1f                       # Perf P-Limit Clip
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PerfPLimitEn|0x1                           # Perf P Limit
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PerfPLmtThshld|0xf                         # Perf P-Limit Threshold
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PerfPlimitDifferential|0x1                 # Perf P-Limit Differential
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskDino[0]|0x0       # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskDino[1]|0x0       # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskDino[2]|0x0       # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskDino[3]|0x0       # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskFxr[0]|0x0        # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskFxr[1]|0x0        # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskFxr[2]|0x0        # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskFxr[3]|0x0        # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskHcx[0]|0x0        # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskHcx[1]|0x0        # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskHcx[2]|0x0        # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskHcx[3]|0x0        # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskHqm[0]|0x0        # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskHqm[1]|0x0        # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskHqm[2]|0x0        # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskHqm[3]|0x0        # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskNac[0]|0x0        # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskNac[1]|0x0        # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskNac[2]|0x0        # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskNac[3]|0x0        # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskTip[0]|0x0        # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskTip[1]|0x0        # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskTip[2]|0x0        # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaAllowedPsMaskTip[3]|0x0        # PKGC_ALLOWED_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoDino[0]|0x0          # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoDino[1]|0x0          # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoDino[2]|0x0          # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoDino[3]|0x0          # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoFxr[0]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoFxr[1]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoFxr[2]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoFxr[3]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoHbm[0]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoHbm[1]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoHbm[2]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoHbm[3]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoHcx[0]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoHcx[1]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoHcx[2]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoHcx[3]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoHqm[0]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoHqm[1]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoHqm[2]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoHqm[3]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoIio[0]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoIio[1]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoIio[2]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoIio[3]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoKti[0]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoKti[1]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoKti[2]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoKti[3]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoMcddr[0]|0x0         # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoMcddr[1]|0x0         # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoMcddr[2]|0x0         # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoMcddr[3]|0x0         # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoMdfs[0]|0x0          # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoMdfs[1]|0x0          # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoMdfs[2]|0x0          # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoMdfs[3]|0x0          # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoNac[0]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoNac[1]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoNac[2]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoNac[3]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoRlink[0]|0x0         # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoRlink[1]|0x0         # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoRlink[2]|0x0         # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoRlink[3]|0x0         # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoTip[0]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoTip[1]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoTip[2]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaInstanceNoTip[3]|0x0           # Instance_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaLogicalIpType[0]|0x0           # CPU0 Logical_ip_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaLogicalIpType[1]|0x0           # CPU1 Logical_ip_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaLogicalIpType[2]|0x0           # CPU2 Logical_ip_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaLogicalIpType[3]|0x0           # CPU3 Logical_ip_type
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaLogicalIpTypeHbm[0]|0x18       # HBM:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaLogicalIpTypeHbm[1]|0x18       # HBM:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaLogicalIpTypeHbm[2]|0x18       # HBM:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaLogicalIpTypeHbm[3]|0x18       # HBM:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaLogicalIpTypeIio[0]|0x20       # IIO:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaLogicalIpTypeIio[1]|0x20       # IIO:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaLogicalIpTypeIio[2]|0x20       # IIO:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaLogicalIpTypeIio[3]|0x20       # IIO:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaLogicalIpTypeMcddr[0]|0x10     # MCDDR:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaLogicalIpTypeMcddr[1]|0x10     # MCDDR:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaLogicalIpTypeMcddr[2]|0x10     # MCDDR:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCCriteriaLogicalIpTypeMcddr[3]|0x10     # MCDDR:
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCLatNeg|0x1                             # PKG C-state Lat. Neg.
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgCstEntryValCtl|0x0                      # PKGC_ENTRY_CRITERIA OVRD
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskDino[0]|0x0               # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskDino[1]|0x0               # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskDino[2]|0x0               # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskDino[3]|0x0               # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskFxr[0]|0x0                # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskFxr[1]|0x0                # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskFxr[2]|0x0                # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskFxr[3]|0x0                # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskHcx[0]|0x0                # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskHcx[1]|0x0                # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskHcx[2]|0x0                # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskHcx[3]|0x0                # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskHqm[0]|0x0                # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskHqm[1]|0x0                # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskHqm[2]|0x0                # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskHqm[3]|0x0                # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskNac[0]|0x0                # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskNac[1]|0x0                # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskNac[2]|0x0                # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskNac[3]|0x0                # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskTip[0]|0x0                # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskTip[1]|0x0                # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskTip[2]|0x0                # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteraPsMaskTip[3]|0x0                # PKGC_CRITERIA_PS_MASK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteriaPsOptionHbm[0]|0x0             # HBM Option
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteriaPsOptionHbm[1]|0x0             # HBM Option
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteriaPsOptionHbm[2]|0x0             # HBM Option
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteriaPsOptionHbm[3]|0x0             # HBM Option
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteriaPsOptionMcddr[0]|0x0           # MCDDR Option
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteriaPsOptionMcddr[1]|0x0           # MCDDR Option
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteriaPsOptionMcddr[2]|0x0           # MCDDR Option
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PkgcCriteriaPsOptionMcddr[3]|0x0           # MCDDR Option
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PmaxAutoAdjustment|0x0                     # BIOS Auto Adjustment
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PmaxDetector|0x1                           # Detector
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PmaxLoadLine|0x0                           # DC LL Select
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PmaxOffset|0x0                             # PMAX Config Positive Offset
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PmaxOffsetNegative|0x0                     # PMAX Config Negative Offset
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PmaxSign|0x0                               # PMAX Config Sign
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PmaxTriggerSetup|0x0                       # Trigger Setup
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PowerLimit1En|0x1                          # PL1 Limit
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PowerLimit1Power|0x0                       # PL1 Power Limit
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PowerLimit1Time|0x1                        # PL1 Time Window
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PowerLimit2En|0x1                          # PL2 Limit
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PowerLimit2Power|0x0                       # PL2 Power Limit
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PowerLimit2Time|0x1                        # PL2 Time Window
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PpcccLock|0x1                              # Lock Indication
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.ProcessorAPSrocketing|0x0                  # APS rocketing
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.ProcessorActivePbf|0x0                     # Activate SST-BF
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.ProcessorC1eEnable|0x1                     # Enhanced Halt State (C1E)
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.ProcessorConfigurePbf|0x1                  # Configure SST-BF
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.ProcessorEPPEnable|0x1                     # EPP Enable
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.ProcessorEistEnable|0x1                    # SpeedStep (Pstates)
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.ProcessorEistPsdFunc|0x0                   # EIST PSD Function
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.ProcessorEppProfile|0x80                   # EPP profile
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.ProcessorHWPMEnable|0x1                    # Hardware P-States
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.ProcessorHWPMInterrupt|0x0                 # HardwarePM Interrupt
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.ProcessorOutofBandAlternateEPB|0x0         # PECI PCS EPB
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.ProcessorRaplPrioritization|0x0            # RAPL Prioritization
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.ProcessorScalability|0x0                   # Scalability
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.ProchotLock|0x1                            # PROCHOT LOCK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.ProchotResponseRatio|0x0                   # PROCHOT RATIO
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PwrPerfSwitch|0x1                          # Dynamic Loadline Switch
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.PwrPerfTuning|0x0                          # Power Performance Tuning
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.RunCpuPpmInPei|0x0                         # Run CPU PPM code in PEI
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.SAPMControl|0x0                            # SAPM Control
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.SapmCtlLock[0]|0x1                         # SAPMCTL_CFG LOCK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.SapmCtlLock[1]|0x1                         # SAPMCTL_CFG LOCK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.SapmCtlLock[2]|0x1                         # SAPMCTL_CFG LOCK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.SapmCtlLock[3]|0x1                         # SAPMCTL_CFG LOCK
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.SetvidDecayDisable[0]|0x0                  # SetVID Decay Disable
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.SetvidDecayDisable[1]|0x0                  # SetVID Decay Disable
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.SetvidDecayDisable[2]|0x0                  # SetVID Decay Disable
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.SetvidDecayDisable[3]|0x0                  # SetVID Decay Disable
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.SnpLatMult|0x0                             # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.SnpLatOvrd|0x0                             # Force Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.SnpLatVal|0x0                              # Force Non-Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.SnpLatVld|0x0                              # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.SwLtrOvrdCtl|0x0                           # PCIe LTR Override Control
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.TCCActivationOffset|0x0                    # TCC Activation Offset
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.TStateEnable|0x0                           # Software Controlled T-States
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.ThermalMonitorStatusFilter|0x0             # Therm-Monitor-Status Filter
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.ThermalMonitorStatusFilterTimeWindow|0x9   # Therm-Monitor-Status Filter Time Window
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.TurboMode|0x1                              # Turbo Mode
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.TurboPowerLimitCsrLock|0x1                 # Package RAPL Limit CSR Lock
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.TurboPowerLimitLock|0x0                    # Package RAPL Limit MSR Lock
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.TurboRatioLimitCores[0]|0xff               # TurboRatioCores0
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.TurboRatioLimitCores[1]|0xff               # TurboRatioCores1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.TurboRatioLimitCores[2]|0xff               # TurboRatioCores2
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.TurboRatioLimitCores[3]|0xff               # TurboRatioCores3
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.TurboRatioLimitCores[4]|0xff               # TurboRatioCores4
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.TurboRatioLimitCores[5]|0xff               # TurboRatioCores5
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.TurboRatioLimitCores[6]|0xff               # TurboRatioCores6
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.TurboRatioLimitCores[7]|0xff               # TurboRatioCores7
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.TurboRatioLimitRatio[0]|0x0                # TurboRatioLimit0
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.TurboRatioLimitRatio[1]|0x0                # TurboRatioLimit1
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.TurboRatioLimitRatio[2]|0x0                # TurboRatioLimit2
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.TurboRatioLimitRatio[3]|0x0                # TurboRatioLimit3
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.TurboRatioLimitRatio[4]|0x0                # TurboRatioLimit4
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.TurboRatioLimitRatio[5]|0x0                # TurboRatioLimit5
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.TurboRatioLimitRatio[6]|0x0                # TurboRatioLimit6
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.TurboRatioLimitRatio[7]|0x0                # TurboRatioLimit7
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.UncoreFreqRaplLimit|0x1                    # Uncore Freq RAPL
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.UncoreFreqScaling|0x1                      # Uncore Freq Scaling
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.UncrPerfPlmtOvrdEn|0x1                     # UNCORE_PERF_PLIMIT_OVRD_EN
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.VccSAandVccIOdisable|0x0                   # VccSA/VccIO Disable
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.WFRWAEnable|0x0
gStructPcdTokenSpaceGuid.PcdSocketPowerManagementConfig.WorkLdConfig|0x0                           # Workload Configuration
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.AcExceptionOnSplitLockEnable|0x0             # #AC Exception On Split Lock
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.AcmType|0x0                                  # ACM Type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.AesEnable|0x1                                # AES-NI
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.AllowMixedPowerOnCpuRatio|0x0                # Skip Flex Ratio Override
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.AmpPrefetchEnable|0x0                        # AMP Prefetch
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.BiosAcmErrorReset|0x0                        # BIOS ACM Error Reset
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.BspSelection|0xff                            # Bsp Selection
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.CFRPucodeEnable|0x0                          # Processor Pcode/Ucode CFR
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.CFRPucodeManualCommit|0x0                    # Manual Commit Pcode/Ucode CFR
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.CFRS3mEnable|0x0                             # Processor S3M CFR
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.CFRS3mManualCommit|0x0                       # Manual Commit S3M FW CFR
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.CheckCpuBist|0x1                             # Check CPU BIST Result
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.CoreDisableMask[0]|0x0                       # Disable Bitmap
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.CoreDisableMask[1]|0x0                       # Disable Bitmap
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.CoreDisableMask[2]|0x0                       # Disable Bitmap
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.CoreDisableMask[3]|0x0                       # Disable Bitmap
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.CoreFailover|0x1                             # Core Failover
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.CpuCrashLogGprs|0x0                          # Cpu CrashLog Gprs
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.CpuDbpEnable|0x0                             # DBP-F
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.CpuL1NextPagePrefetcherDisable|0x0           # L1 Next Page Prefetcher
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.CpuMtoIWa|0x1                                # MtoI Workaround
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.CpuPaLimit|0x1                               # Limit CPU PA to 46 bits
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.CpuidMaxValue|0x0                            # Max CPUID Value Limit
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.CrDimmsPresent|0x0                           # SW Guard Extensions (SGX)
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.DCUIPPrefetcherEnable|0x1                    # DCU IP Prefetcher
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.DCUModeSelection|0x0                         # DCU Mode
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.DCUStreamerPrefetcherEnable|0x1              # DCU Streamer Prefetcher
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.DebugInterface|0x0                           # DEBUG INTERFACE
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ReservedS245|0x2
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ReservedS192|0x0
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ReservedS234|0x0
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ReservedS241|0x2
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ReservedS238|0x2
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ReservedS242|0x2
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ReservedS244|0x2
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ReservedS240|0x2
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ReservedS243|0x2
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ReservedS236|0x0
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ReservedS237|0x0
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ReservedS235|0x0                         # TME key restore
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.EnableMktme|0x0                              # Total Memory Encryption Multi-Tenant(TME-MT)
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.EnableSaf|0x0                                # Enable FuSa (SAF) - variable to handle dynamic BIOS menu
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.EnableSgx|0x0                                # SW Guard Extensions (SGX)
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.EnableTme|0x0                                # Total Memory Encryption (TME)
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.EpochUpdate|0x2                              # Select Owner EPOCH input type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.FastStringEnable|0x1                         # Fast String
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ForcePhysicalModeEnable|0x0                  # APIC Physical Mode
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.GlobalPsmiEnable|0x1                         # Global PSMI Enable
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.IedTraceSize|0x0                             # IED Trace memory
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.IioLlcWaysMask|0x0                           # IIO LLC Ways [19:0](Hex)
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.IotEn[0]|0x0                                 # IOT Cfg
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.IotEn[1]|0x0                                 # IOT Cfg
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.IotEn[2]|0x0                                 # IOT Cfg
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.IotEn[3]|0x0                                 # IOT Cfg
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.L2RfoPrefetchDisable|0x0                     # L2 RFO Prefetch Disable
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.LlcPrefetchEnable|0x1                        # LLC Prefetch
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.LockChipset|0x1                              # Lock Chipset
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.MachineCheckEnable|0x1                       # Machine Check
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.MlcSpatialPrefetcherEnable|0x1               # Adjacent Cache Prefetch
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.MlcStreamerPrefetcherEnable|0x1              # Hardware Prefetcher
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.OclaMaxTorEntry[0]|0x1                       # OCLA Tor IDs
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.OclaMaxTorEntry[1]|0x1                       # OCLA Tor IDs
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.OclaMaxTorEntry[2]|0x1                       # OCLA Tor IDs
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.OclaMaxTorEntry[3]|0x1                       # OCLA Tor IDs
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.OclaMinWay[0]|0x1                            # Num of OCLA ways
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.OclaMinWay[1]|0x1                            # Num of OCLA Ways
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.OclaMinWay[2]|0x1                            # Num of OCLA Ways
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.OclaMinWay[3]|0x1                            # Num of OCLA Ways
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PCIeDownStreamPECIWrite|0x0                  # Down Stream PECI
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PeciAgtEspiTrustBit|0x0                      # eSPI Agent
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PeciAgtGenericTrustBit|0x0                   # Generic Agent
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PeciAgtIeTrustBit|0x1                        # IE Agent
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PeciAgtLegacyTrustBit|0x1                    # Legacy Agent
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PeciAgtSmbusTrustBit|0x0                     # SMBus Agent
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PeciInTrustControlBit|0x1                    # PECI
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PpinControl|0x1                              # PPIN Control
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PrmrrSize|0x80000000                         # PRMRR Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ProcessorFlexibleRatio|0x17                  # CPU Core Flex Ratio
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ProcessorFlexibleRatioOverrideEnable|0x0     # CPU Flex Ratio Override
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ProcessorHyperThreadingDisable|0x0           # Hyper-Threading [ALL]
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ProcessorLtsxEnable|0x0                      # Enable Intel(R) TXT
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ProcessorMsrLockControl|0x1                  # MSR Lock Control
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ProcessorMsrPkgCstConfigControlLock|0x1      # PKG CST CONFIG CONTROL MSR Lock
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ProcessorSmxEnable|0x0                       # Enable SMX
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ProcessorVmxEnable|0x1                       # VMX
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ProcessorX2apic|0x1                          # Extended APIC
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiHandlerSize[0]|0x1                       # PSMI Handler Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiHandlerSize[1]|0x1                       # PSMI Handler Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiHandlerSize[2]|0x1                       # PSMI Handler Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiHandlerSize[3]|0x1                       # PSMI Handler Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTrace[0]|0x0                             # PSMI Enable
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTrace[1]|0x0                             # PSMI Enable
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTrace[2]|0x0                             # PSMI Enable
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTrace[3]|0x0                             # PSMI Enable
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceBufferSizeRegion0[0]|0x1            # Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceBufferSizeRegion0[1]|0x1            # Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceBufferSizeRegion0[2]|0x1            # Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceBufferSizeRegion0[3]|0x1            # Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceBufferSizeRegion1[0]|0x1            # Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceBufferSizeRegion1[1]|0x1            # Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceBufferSizeRegion1[2]|0x1            # Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceBufferSizeRegion1[3]|0x1            # Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceBufferSizeRegion2[0]|0x1            # Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceBufferSizeRegion2[1]|0x1            # Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceBufferSizeRegion2[2]|0x1            # Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceBufferSizeRegion2[3]|0x1            # Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceBufferSizeRegion3[0]|0x1            # Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceBufferSizeRegion3[1]|0x1            # Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceBufferSizeRegion3[2]|0x1            # Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceBufferSizeRegion3[3]|0x1            # Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceBufferSizeRegion4[0]|0x1            # Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceBufferSizeRegion4[1]|0x1            # Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceBufferSizeRegion4[2]|0x1            # Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceBufferSizeRegion4[3]|0x1            # Buffer Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceMemTypeRegion0[0]|0x0               # Cache Type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceMemTypeRegion0[1]|0x0               # Cache Type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceMemTypeRegion0[2]|0x0               # Cache Type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceMemTypeRegion0[3]|0x0               # Cache Type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceMemTypeRegion1[0]|0x0               # Cache Type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceMemTypeRegion1[1]|0x0               # Cache Type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceMemTypeRegion1[2]|0x0               # Cache Type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceMemTypeRegion1[3]|0x0               # Cache Type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceMemTypeRegion2[0]|0x0               # Cache Type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceMemTypeRegion2[1]|0x0               # Cache Type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceMemTypeRegion2[2]|0x0               # Cache Type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceMemTypeRegion2[3]|0x0               # Cache Type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceMemTypeRegion3[0]|0x0               # Cache Type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceMemTypeRegion3[1]|0x0               # Cache Type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceMemTypeRegion3[2]|0x0               # Cache Type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceMemTypeRegion3[3]|0x0               # Cache Type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceMemTypeRegion4[0]|0x0               # Cache Type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceMemTypeRegion4[1]|0x0               # Cache Type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceMemTypeRegion4[2]|0x0               # Cache Type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceMemTypeRegion4[3]|0x0               # Cache Type
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceRegion0[0]|0x0                      # PSMI Trace Region 0
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceRegion0[1]|0x0                      # PSMI Trace Region 0
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceRegion0[2]|0x0                      # PSMI Trace Region 0
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceRegion0[3]|0x0                      # PSMI Trace Region 0
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceRegion1[0]|0x0                      # PSMI Trace Region 1
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceRegion1[1]|0x0                      # PSMI Trace Region 1
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceRegion1[2]|0x0                      # PSMI Trace Region 1
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceRegion1[3]|0x0                      # PSMI Trace Region 1
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceRegion2[0]|0x0                      # PSMI Trace Region 2
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceRegion2[1]|0x0                      # PSMI Trace Region 2
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceRegion2[2]|0x0                      # PSMI Trace Region 2
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceRegion2[3]|0x0                      # PSMI Trace Region 2
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceRegion3[0]|0x0                      # PSMI Trace Region 3
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceRegion3[1]|0x0                      # PSMI Trace Region 3
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceRegion3[2]|0x0                      # PSMI Trace Region 3
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceRegion3[3]|0x0                      # PSMI Trace Region 3
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceRegion4[0]|0x0                      # PSMI Trace Region 4
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceRegion4[1]|0x0                      # PSMI Trace Region 4
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceRegion4[2]|0x0                      # PSMI Trace Region 4
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PsmiTraceRegion4[3]|0x0                      # PSMI Trace Region 4
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.RdtCatOpportunisticTuning|0x0                # RDT CAT Opportunistic Tuning
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.RemoteWaysMask|0x0                           # Remote Ways [22:12](Hex)
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.SgxAutoRegistrationAgent|0x0                 # Enable/Disable SGX Auto MP Registration Agent
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.SgxDebugMode|0x0                             # Enable/Disable SGX Debug Mode
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.SgxEpoch0|0x0                                # Software Guard Extensions Epoch 0
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.SgxEpoch1|0x0                                # Software Guard Extensions Epoch 1
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.SgxFactoryReset|0x0                          # SGX Factory Reset
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.SgxLePubKeyHash0|0x0                         # SGXLEPUBKEYHASH0
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.SgxLePubKeyHash1|0x0                         # SGXLEPUBKEYHASH1
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.SgxLePubKeyHash2|0x0                         # SGXLEPUBKEYHASH2
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.SgxLePubKeyHash3|0x0                         # SGXLEPUBKEYHASH3
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.SgxLeWr|0x1                                  # SGXLEPUBKEYHASHx Write Enable
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.SgxPackageInfoInBandAccess|0x0               # SGX Package Info In-Band Access
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.SgxQoS|0x1                                   # SGX QoS
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.SmbusErrorRecovery|0x1                       # Smbus Error Recovery
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.SmmBlockedDelayed|0x0                        # SMM Blocked and Delayed
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ThreeStrikeTimer|0x1                         # 3StrikeTimer
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.TscResetEnable|0x0                           # TSC Reset
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.TsegSize|0x5                                 # TSEG Smram Size
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.eSmmSaveState|0x0                            # eSMM Save State
gStructPcdTokenSpaceGuid.PcdSvConfiguration.BaseAboveFourGiga|0x100000000                          # Mega-Block base above 4GB
gStructPcdTokenSpaceGuid.PcdSvConfiguration.BaseAboveOneMega|0x100000                              # Mega-Block base above 1MB
gStructPcdTokenSpaceGuid.PcdSvConfiguration.BaseBelowOneMega|0x50000                               # Mega-Block base below 1MB
gStructPcdTokenSpaceGuid.PcdSvConfiguration.CoreMegaBlock|0x0                                      # Core Mega Block Support
gStructPcdTokenSpaceGuid.PcdSvConfiguration.PolicyUnUsedAboveFourGiga|0x0                          # Mega-Block non-used policy above 4GB
gStructPcdTokenSpaceGuid.PcdSvConfiguration.PolicyUnUsedAboveOneMega|0x0                           # Mega-Block non-used policy above 1MB
gStructPcdTokenSpaceGuid.PcdSvConfiguration.PolicyUnUsedBelowOneMega|0x0                           # Mega-Block non-used policy below 1MB
gStructPcdTokenSpaceGuid.PcdSvConfiguration.PolicyUsedAboveFourGiga|0x4                            # Mega-Block policy above 4GB
gStructPcdTokenSpaceGuid.PcdSvConfiguration.PolicyUsedAboveOneMega|0x4                             # Mega-Block policy above 1MB
gStructPcdTokenSpaceGuid.PcdSvConfiguration.PolicyUsedBelowOneMega|0x4                             # Mega-Block policy below 1MB
gStructPcdTokenSpaceGuid.PcdSvConfiguration.SizeAboveFourGiga|0x0                                  # Mega-Block size above 4GB
gStructPcdTokenSpaceGuid.PcdSvConfiguration.SizeAboveOneMega|0x0                                   # Mega-Block size above 1MB
gStructPcdTokenSpaceGuid.PcdSvConfiguration.SizeBelowOneMega|0x0                                   # Mega-Block size below 1MB
gStructPcdTokenSpaceGuid.PcdTCG2_CONFIGURATION.TpmDevice|0x1                                       # Attempt TPM Device
gStructPcdTokenSpaceGuid.PcdTCG2_VERSION.PpiVersion|0x332e31                                       # Attempt PPI Version
gStructPcdTokenSpaceGuid.PcdTCG2_VERSION.Tpm2AcpiTableRev|0x4                                      # Attempt Rev of TPM2 ACPI Table

[PcdsDynamicHii.common.DEFAULT.MANUFACTURING]
gStructPcdTokenSpaceGuid.PcdPchSetup.Dwr_IeResetPrepDone|0x0                                       # IE Reset Prep Done
gStructPcdTokenSpaceGuid.PcdPchSetup.Dwr_MeResetPrepDone|0x0                                       # ME Reset Prep Done
gStructPcdTokenSpaceGuid.PcdSetup.StorageOpROMSuppression|0x1                                      # Storage OPROM Suppression
gStructPcdTokenSpaceGuid.PcdSetup.TagecMem|0x1                                                     # Reserve TAGEC Memory
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.ReservedS117|0x0
gStructPcdTokenSpaceGuid.PcdSocketMemoryConfig.MemHotOuputAssertThreshold|0x0                      # MEMHOT Output Throttling Mode Options
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.PCIeDownStreamPECIWrite|0x1                  # Down Stream PECI
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ProcessorMsrLockControl|0x0                  # MSR Lock Control
gStructPcdTokenSpaceGuid.PcdSocketProcessorCoreConfig.ProcessorVmxEnable|0x0                       # VMX

