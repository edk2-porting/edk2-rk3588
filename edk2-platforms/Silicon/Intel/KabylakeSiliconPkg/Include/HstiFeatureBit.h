/** @file
  This file contains various definitions for IHV HSTI implementation
  including error string definitions

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __HSTI_FEATURE_BIT_H__
#define __HSTI_FEATURE_BIT_H__

#define HSTI_SECURITY_FEATURE_SIZE        3

#define HSTI_ERROR                                                                L"Error "
#define HSTI_PLATFORM_SECURITY_SPECIFICATION                                      L" Platform Security Specification"
#define HSTI_BOOTGUARD_CONFIGURATION                                              L" - Boot Guard Configuration - "
#define HSTI_SPI_FLASH_CONFIGURATION                                              L" - SPI Flash Configuration - "
#define HSTI_PCH_SECURITY_CONFIGURATION                                           L" - PCH Security Configuration - "
#define HSTI_BIOS_GUARD_SECURITY_CONFIGURATION                                    L" - BIOS Guard Security Configuration - "
#define HSTI_MEASURED_BOOT_CONFIGURATION                                          L" - Measured Boot Configuration - "
#define HSTI_INTEGRATED_DEVICE_DMA_PROTECTION                                     L" - Integrated Device DMA Protection - "
#define HSTI_CPU_SECURITY_CONFIGURATION                                           L" - CPU Security Configuration - "
#define HSTI_SYSTEM_AGENT_SECURITY_CONFIGURATION                                  L" - Secure System Agent Security Configuration - "
#define HSTI_MEMORY_MAP_SECURITY_CONFIGURATION                                    L" - Memory Map Security Configuration - "
#define HSTI_PROCESSOR_GRAPHICS_SECURITY_CONFIGURATION                            L" - Processor Graphics Security Configuration - "
#define HSTI_PROCESSOR_SPD_SECURITY_CONFIGURATION                                 L" - SPD Security Configuration - "


#define HSTI_BYTE0_HARDWARE_ROOTED_BOOT_INTEGRITY                                 BIT0
#define      HSTI_BYTE0_HARDWARE_ROOTED_BOOT_INTEGRITY_ERROR_CODE_1               L"0x00000001"
#define      HSTI_BYTE0_HARDWARE_ROOTED_BOOT_INTEGRITY_ERROR_STRING_1             L"Boot Guard configured without Verified Boot\r\n"
#define      HSTI_BYTE0_HARDWARE_ROOTED_BOOT_INTEGRITY_ERROR_CODE_2               L"0x00000002"
#define      HSTI_BYTE0_HARDWARE_ROOTED_BOOT_INTEGRITY_ERROR_STRING_2             L"Boot Guard disabled\r\n"

#define HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION                                 BIT1
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_CODE_1               L"0x00010001"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_STRING_1             L"SPI Flash not write protected\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_CODE_2               L"0x00010002"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_STRING_2             L"SPI Flash descriptor overridden\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_CODE_3               L"0x00010003"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_STRING_3             L"SPI Controller configuration unlocked\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_CODE_4               L"0x00010004"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_STRING_4             L"SPI Controller BIOS Interface unlocked\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_CODE_5               L"0x00010005"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_STRING_5             L"Top Swap enabled\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_CODE_6               L"0x00010006"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_STRING_6             L"SPI Vendor Specific Component Capabilities unlocked\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_CODE_7               L"0x00010007"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_STRING_7             L"SPI Controller GbE Interface unlocked\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_CODE_8               L"0x00010008"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_STRING_8             L"ME FW not in Normal Working State\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_CODE_9               L"0x00010009"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_STRING_9             L"Flash Descriptor Invalid\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_CODE_A               L"0x0001000A"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_STRING_A             L"SPI Region Access Rights Invalid\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_CODE_B               L"0x0001000B"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_STRING_B             L"SPI SW Sequencing OPCODEs insufficiently restrained\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_CODE_C               L"0x0001000C"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_STRING_C             L"Allowed SPI SW Sequencing OPCODE configuration incorrect\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_CODE_D               L"0x0001000D"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_STRING_D             L"Global SMI not enabled and locked\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_CODE_E               L"0x0001000E"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_STRING_E             L"TCO SMI not enabled and locked\r\n"

#define HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE                                         BIT2
#define      HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE_ERROR_CODE_1                       L"0x00020001"
#define      HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE_ERROR_STRING_1                     L"BIOS Guard unsupported\r\n"
#define      HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE_ERROR_CODE_2                       L"0x00020002"
#define      HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE_ERROR_STRING_2                     L"BIOS Guard configuration unlocked\r\n"
#define      HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE_ERROR_CODE_3                       L"0x00020001"
#define      HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE_ERROR_STRING_3                     L"BIOS Guard disabled\r\n"

#define HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT                                      BIT3
#define      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_ERROR_CODE_1                    L"0x00030001"
#define      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_ERROR_STRING_1                  L"Chipset supports FW TPM, however no TPM enabled\r\n"

#define      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_ERROR_CODE_2                    L"0x00030002"
#define      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_ERROR_STRING_2                  L"PCR[7] is not initialized\r\n"

#define      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_ERROR_CODE_3                    L"0x00030003"
#define      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_ERROR_STRING_3                  L"Event Log is not published or invalid\r\n"

#define      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_ERROR_CODE_4                    L"0x00030004"
#define      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_ERROR_STRING_4                  L"Platform Auth accessible via 0x00000000\r\n"


#define HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION                               BIT4
#define      HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_ERROR_CODE_1             L"0x00040001"
#define      HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_ERROR_STRING_1           L"Bus Mastering Enabled for non-boot, integrated device\r\n"

#define HSTI_BYTE0_DEBUG_MODE_DISABLED_VERIFICATION                               BIT5
#define      HSTI_BYTE0_DEBUG_MODE_DISABLED_VERIFICATION_ERROR_CODE_1             L"0x00050001"
#define      HSTI_BYTE0_DEBUG_MODE_DISABLED_VERIFICATION_ERROR_STRING_1           L"Debug MSR enabled\r\n"

#define      HSTI_BYTE0_DEBUG_MODE_DISABLED_VERIFICATION_ERROR_CODE_2             L"0x00050002"
#define      HSTI_BYTE0_DEBUG_MODE_DISABLED_VERIFICATION_ERROR_STRING_2           L"Debug interface unlocked\r\n"

#define HSTI_BYTE0_SECURE_CPU_CONFIGURATION                                       BIT6
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_1                     L"0x00060001"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_1                   L" Minimum uCode patch revision not met\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_2                     L"0x00060002"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_2                   L" Pre-production silicon in use\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_3                     L"0x00060003"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_3                   L"VMX & Senter feature configuration unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_4                     L"0x00060004"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_4                   L" SMM Code Fetch feature configuration unlocked\r\n"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_4A                  L" SMM Code Fetch outside SMRAM detection feature is disabled\r\n"


#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_5                     L"0x00060005"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_5                   L" AES-NI Feature configuration unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_6                     L"0x00060006"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_6                   L" FIT table not present\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_7                     L"0x00060007"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_7                   L" MicrocodeRangeRegisters improperly configured or unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_8                     L"0x00060008"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_8                   L" SMM COnfiguration Unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_9                     L"0x00060009"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_9                   L" TSEG not naturally aligned\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_A                     L"0x0006000A"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_A                   L" Improper SMRR configuration\r\n"

#define HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION                              BIT7
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_ERROR_CODE_1            L"0x00070001"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_ERROR_STRING_1          L"Intel TXT configuration unlocked\r\n"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_ERROR_CODE_2            L"0x00070002"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_ERROR_STRING_2          L"Memory Controller configuration unlocked\r\n"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_ERROR_CODE_3            L"0x00070003"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_ERROR_STRING_3          L"System Agent configuration unlocked\r\n"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_ERROR_CODE_4            L"0x00070004"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_ERROR_STRING_4          L"Graphics memory configuration unlocked\r\n"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_ERROR_CODE_5            L"0x00070005"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_ERROR_STRING_5          L"VT-d configuration unlocked\r\n"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_ERROR_CODE_6            L"0x00070006"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_ERROR_STRING_6          L"Power Plane 0 Current Config unlocked\r\n"

#define HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION                                BIT0
#define      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_ERROR_CODE_1              L"0x00080001"
#define      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_ERROR_STRING_1            L" Memory BAR configuration unlocked\r\n"

#define      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_ERROR_CODE_2              L"0x00080002"
#define      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_ERROR_STRING_2            L" Fixed MMIO regions overlap\r\n"

#define      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_ERROR_CODE_3              L"0x00080003"
#define      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_ERROR_STRING_3            L" Non lockable MMIO ranges overlap other critical regions\r\n"

#define      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_ERROR_CODE_4              L"0x00080004"
#define      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_ERROR_STRING_4            L" SPD not write protected\r\n"

#define HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION                       BIT1
#define      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_CODE_1     L"0x00090001"
#define      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_STRING_1   L" Graphics configuration unlocked\r\n"

#define      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_CODE_2     L"0x00090002"
#define      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_STRING_2   L"Invalid Graphics Memory Alignment\r\n"

#define      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_CODE_3     L"0x00090003"
#define      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_STRING_3   L" Reserved Check failed\r\n"

#define HSTI_BYTE1_SECURE_PCH_CONFIGURATION                                       BIT2
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_ERROR_CODE_1                     L"0x000A0001"
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_ERROR_STRING_1                   L" Thermal Configuration unlocked\r\n"
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_ERROR_CODE_2                     L"0x000A0002"
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_ERROR_STRING_2                   L"BAR's unlocked\r\n"
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_ERROR_CODE_3                     L"0x000A0003"
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_ERROR_STRING_3                   L" Reserved Check failure\r\n"

#endif
