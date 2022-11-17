## @file
# platform configuration file for DEBUG build.
#
# @copyright
# Copyright 2011 - 2021 Intel Corporation. <BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
##

#
# TRUE is ENABLE. FALSE is DISABLE.
#

DEFINE CRB_FLAG_ENABLE                       = TRUE
DEFINE DEBUG_FLAGS_ENABLE                    = FALSE

DEFINE PERFORMANCE_ENABLE                    = TRUE

DEFINE SERVER_BIOS_ENABLE                    = TRUE
DEFINE PCH_SERVER_BIOS_ENABLE                = TRUE

!if $(CPUTARGET) == "CPX"
  DEFINE CPU_SKX_ONLY_SUPPORT                = TRUE
!else
  DEFINE CPU_SKX_ONLY_SUPPORT                 = FALSE
!endif

!if $(CPUTARGET) == "CPX"
  DEFINE CPU_CPX_SUPPORT                     = TRUE
!else
  DEFINE CPU_CPX_SUPPORT                     = FALSE
!endif

DEFINE RAS_CPU_ONLINE_OFFLINE_ENABLE         = FALSE

DEFINE SPARING_SCRATCHPAD_ENABLE             = TRUE
DEFINE SCRATCHPAD_DEBUG                      = TRUE

DEFINE TPM2_ENABLE                         = FALSE

DEFINE ME_PATH_CONFIG                        = Me/MeSps.4

DEFINE SECURE_BOOT_ENABLE                    = FALSE

DEFINE PLATFORMX64_ENABLE = TRUE
