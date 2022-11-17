;
; This file contains an 'Intel Peripheral Driver' and is
; licensed for Intel CPUs and chipsets under the terms of your
; license agreement with Intel or your vendor.  This file may
; be modified by the user, subject to additional terms of the
; license agreement
;; @file
; This is the code that goes from real-mode to protected mode.
; It consumes the reset vector, calls TempRamInit API from FSP binary.
;
; Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
; SPDX-License-Identifier: BSD-2-Clause-Patent
;
;;

SECTION .text

global  ASM_PFX(BoardBeforeTempRamInit)
ASM_PFX(BoardBeforeTempRamInit):
        ;
        ; This hook is called before FSP TempRamInit API call
        ; ESI, EDI need to be preserved
        ; ESP contains return address
        ;
        jmp     esp        

