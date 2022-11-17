/** @file
  CPU Security PreMemory Config Block.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CPU_CONFIG_LIB_PREMEM_CONFIG_H_
#define _CPU_CONFIG_LIB_PREMEM_CONFIG_H_

#define CPU_CONFIG_LIB_PREMEM_CONFIG_REVISION 5

extern EFI_GUID gCpuConfigLibPreMemConfigGuid;

#pragma pack (push,1)

/**
  CPU Config Library PreMemory Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Update for JTAG Power Gate comment.
  <b>Revision 3</b>:
  - Add PeciSxReset and PeciC10Reset
  <b>Revision 4</b>:
  - Add SkipMpInit
  <b>Revision 5</b>:
  - Add DpSscMarginEnable
**/
typedef struct {
  CONFIG_BLOCK_HEADER Header;            ///< Config Block Header
  UINT32 HyperThreading             : 1; ///< Enable or Disable Hyper Threading; 0: Disable; <b>1: Enable</b>.
  /**
  Sets the boot frequency starting from reset vector.
   - 0: Maximum battery performance.
   - <b>1: Maximum non-turbo performance</b>.
   - 2: Turbo performance.
  @note If Turbo is selected BIOS will start in max non-turbo mode and switch to Turbo mode.
  **/
  UINT32 BootFrequency              : 2;
  /**
    Number of processor cores to enable.
    - <b> 0: All cores</b>
    -     1: 1 core
    -     2: 2 cores
    -     3: 3 cores
  **/
  UINT32 ActiveCoreCount            : 3;
  UINT32 JtagC10PowerGateDisable    : 1; ///< False: JTAG is power gated in C10 state. True: keeps the JTAG power up during C10 and deeper power states for debug purpose. <b>0: False<\b>; 1: True.
  UINT32 BistOnReset                : 1; ///< <b>(Test)</b> Enable or Disable BIST on Reset; <b>0: Disable</b>; 1: Enable.
  /**
    Enable or Disable Virtual Machine Extensions (VMX) feature.
    -    0: Disable
    - <b>1: Enable</b>
  **/
  UINT32 VmxEnable                  : 1;
  /**
  Processor Early Power On Configuration FCLK setting.
   - <b>0: 800 MHz (ULT/ULX)</b>.
   - <b>1: 1 GHz (DT/Halo)</b>. Not supported on ULT/ULX.
   - 2: 400 MHz.
   - 3: Reserved.
  **/
  UINT32 FClkFrequency              : 2;
  /**
    Enables a mailbox command to resolve rare PECI related Sx issues.
    @note This should only be used on systems that observe PECI Sx issues.
    - <b>0: Disable</b>
    - 1: Enable
  **/
  UINT32 PeciSxReset               : 1;
  /**
    Enables the mailbox command to resolve PECI reset issues during Pkg-C10 exit.
    If Enabled, BIOS will send the CPU message to disable peci reset on C10 exit.
    The default value is <b>0: Disable</b> for CNL, and <b>1: Enable</b> for all other CPU's
    - 0: Disable
    - 1: Enable
  **/
  UINT32 PeciC10Reset               : 1;
  /**
    For Fsp only, Silicon Initialization will skip MP Initialization
    (including BSP) if enabled. For non-FSP, this should always be 0.
    - <b>0: Disable</b>
    - 1: Enable
  **/
  UINT32 SkipMpInit                 : 1;
  /**
    Enable DisplayPort SSC range reduction
    @note This should only be used on systems that exceeds allowed SSC modulation range as defined in VESA's spec.
    - <b>0: Disable</b>
    - 1: Enable
  **/
  UINT32 DpSscMarginEnable          : 1;
  UINT32 RsvdBits                   : 17;
  /**
    CpuRatio - Max non-turbo ratio (Flexible Ratio Boot) is set to CpuRatio. <b>0: Disabled</b> If disabled, doesn't override max-non turbo ratio.
  **/
  UINT8  CpuRatio;
  UINT8  Reserved[3];                    ///< Reserved for alignment
} CPU_CONFIG_LIB_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _CPU_CONFIG_LIB_PREMEM_CONFIG_H_
