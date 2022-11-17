## @file EnableRichDebugMessages.dsc
# This takes care to turn on a higher level of debug messages that produces a
# balance between performance and greater levels of detail.
# This also customizes cores to use BaseDebugLibSerialPort for maximum
# messaging.
#
# @copyright
# Copyright 2020 - 2021 Intel Corporation. <BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
##

#
# Customize debug messages
#
[PcdsFixedAtBuild]
  ## This flag is used to control the built in Debug messages.
  #  BIT0  - Initialization message.<BR>
  #  BIT1  - Warning message.<BR>
  #  BIT2  - Load Event message.<BR>
  #  BIT3  - File System message.<BR>
  #  BIT4  - Allocate or Free Pool message.<BR>
  #  BIT5  - Allocate or Free Page message.<BR>
  #  BIT6  - Information message.<BR>
  #  BIT7  - Dispatcher message.<BR>
  #  BIT8  - Variable message.<BR>
  #  BIT10 - Boot Manager message.<BR>
  #  BIT12 - BlockIo Driver message.<BR>
  #  BIT14 - Network Driver message.<BR>
  #  BIT16 - UNDI Driver message
  #  BIT17 - LoadFile message.<BR>
  #  BIT19 - Event message.<BR>
  #  BIT20 - Global Coherency Database changes message.<BR>
  #  BIT21 - Memory range cachability changes message.<BR>
  #  BIT22 - Detailed debug message.<BR>
  #  BIT31 - Error message.<BR>
  gEfiMdePkgTokenSpaceGuid.PcdFixedDebugPrintErrorLevel|0x802A00C7

[PcdsPatchableInModule]
  #
  # This flag is used to control the displayed Debug messages.
  # For simplification, we like to ensure both built in and enabled are in sync
  #
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|gEfiMdePkgTokenSpaceGuid.PcdFixedDebugPrintErrorLevel

[LibraryClasses.IA32.PEI_CORE]
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf

[LibraryClasses.X64.DXE_CORE]
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
