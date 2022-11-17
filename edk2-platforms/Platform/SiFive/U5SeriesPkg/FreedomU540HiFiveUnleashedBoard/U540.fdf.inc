## @file
#  Definitions of Flash definition file on SiFive Freedom U540 HiFive Unleashed RISC-V platform
#
#  Copyright (c) 2019, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##
[Defines]
DEFINE BLOCK_SIZE        = 0x1000

DEFINE FW_BASE_ADDRESS   = 0x80000000
DEFINE FW_SIZE           = 0x00800000
DEFINE FW_BLOCKS         = 0x800

#
# 0x000000-0x7DFFFF code
# 0x7E0000-0x800000 variables
#
DEFINE CODE_BASE_ADDRESS = 0x80000000
DEFINE CODE_SIZE         = 0x007E0000
DEFINE CODE_BLOCKS       = 0x7E0
DEFINE VARS_BLOCKS       = 0x20

DEFINE SECFV_OFFSET      = 0x00000000
DEFINE SECFV_SIZE        = 0x00020000
DEFINE PEIFV_OFFSET      = 0x00020000
DEFINE PEIFV_SIZE        = 0x00080000
DEFINE SCRATCH_OFFSET    = 0x000a0000
DEFINE SCRATCH_SIZE      = 0x00010000
DEFINE FVMAIN_OFFSET     = 0x00100000 # Must be power of 2 for PMP setting
DEFINE FVMAIN_SIZE       = 0x0018C000
DEFINE VARS_OFFSET       = 0x007E0000
DEFINE VARS_SIZE         = 0x00020000

SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdVariableFdBaseAddress = $(FW_BASE_ADDRESS) + $(VARS_OFFSET)
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdVariableFdSize        = $(VARS_SIZE)
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdVariableFdBlockSize   = $(BLOCK_SIZE)

SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdFwStartAddress   = $(CODE_BASE_ADDRESS)
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdFwEndAddress     = $(CODE_BASE_ADDRESS) + $(SECFV_SIZE) + $(PEIFV_SIZE) + $(SCRATCH_SIZE)
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdOpenSbiStackSize = 8192
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdScratchRamBase   = $(CODE_BASE_ADDRESS) + $(SCRATCH_OFFSET)
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdScratchRamSize   = $(SCRATCH_SIZE)
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdTemporaryRamBase = $(CODE_BASE_ADDRESS) + $(FW_SIZE)
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdTemporaryRamSize = 0x10000


SET gUefiRiscVPkgTokenSpaceGuid.PcdRiscVMachineTimerFrequencyInHerz    = 1000000
SET gSiFiveU5SeriesPlatformsPkgTokenSpaceGuid.PcdU5PlatformSystemClock = 1000000000 # 1GHz system clock
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdHartCount                   = 5          # Total cores on U540 platform
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdBootHartId                  = 1          # Boot hart ID
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdBootableHartNumber          = 4          # The bootable hart core number.
                                                                                    # Which is incorporate with OpenSBI
                                                                                    # platform hart_index2id value.
SET gSiFiveU5SeriesPlatformsPkgTokenSpaceGuid.PcdNumberofU5Cores       = 4          # Total U5 cores enabled on U540 platform
SET gSiFiveU5SeriesPlatformsPkgTokenSpaceGuid.PcdE5MCSupported         = True       # E51 MC exists.
SET gSiFiveU5SeriesPlatformsPkgTokenSpaceGuid.PcdU5UartBase            = 0x10010000 # Serial port base address
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdPeiCorePrivilegeMode        = 1          # Set PeiCore to S-Mode
