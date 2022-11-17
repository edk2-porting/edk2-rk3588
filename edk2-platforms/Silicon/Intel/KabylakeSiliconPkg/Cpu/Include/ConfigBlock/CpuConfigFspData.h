/** @file
  FSP CPU Data Config Block.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _CPU_CONFIG_FSP_DATA_H_
#define _CPU_CONFIG_FSP_DATA_H_

#pragma pack (push,1)
typedef union {
  struct {
    /**
      Enable or Disable Advanced Encryption Standard (AES) feature.
      For some countries, this should be disabled for legal reasons.
      -    0: Disable
      - <b>1: Enable</b>
    **/
    UINT32 AesEnable           : 1;
    /**
    Processor Early Power On Configuration FCLK setting.
     - <b>0: 800 MHz (ULT/ULX)</b>.
     - <b>1: 1 GHz (DT/Halo)</b>. Not supported on ULT/ULX.
     - 2: 400 MHz.
     - 3: Reserved.
    **/
    UINT32 FClkFrequency       : 2;
    UINT32 EnableRsr           : 1;                 ///< Enable or Disable RSR feature; 0: Disable; <b>1: Enable </b>
    /**
    Policies to obtain CPU temperature.
     - <b>0: ACPI thermal management uses EC reported temperature values</b>.
     - 1: ACPI thermal management uses DTS SMM mechanism to obtain CPU temperature values.
     - 2: ACPI Thermal Management uses EC reported temperature values and DTS SMM is used to handle Out of Spec condition.
    **/
    UINT32 EnableDts           : 2;
    UINT32 SmmbaseSwSmiNumber  : 8;                 ///< Software SMI number for handler to save CPU information in SMRAM.
    /**
      Enable or Disable Virtual Machine Extensions (VMX) feature.
      -    0: Disable
      - <b>1: Enable</b>
    **/
    UINT32 VmxEnable           : 1;
    /**
      Enable or Disable Trusted Execution Technology (TXT) feature.
      -    0: Disable
      - <b>1: Enable</b>
    **/
    UINT32 TxtEnable           : 1;
    UINT32 SkipMpInit          : 1;                 ///< For Fsp only, Silicon Initialization will skip MP Initialization (including BSP) if enabled. For non-FSP, this should always be 0.
    UINT32 RsvdBits            : 15;                ///< Reserved for future use
    UINT32 Reserved;
    } Bits;
    UINT32 Uint32[2];
  } CPU_CONFIG_FSP_DATA;

#pragma pack (pop)

#endif // _CPU_CONFIG_FSP_DATA_H_
