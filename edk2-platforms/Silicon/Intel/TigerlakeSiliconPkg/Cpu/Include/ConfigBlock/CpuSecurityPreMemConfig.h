/** @file
  CPU Security PreMemory Config Block.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _CPU_SECURITY_PREMEM_CONFIG_H_
#define _CPU_SECURITY_PREMEM_CONFIG_H_

#define CPU_SECURITY_PREMEM_CONFIG_REVISION 1

extern EFI_GUID gCpuSecurityPreMemConfigGuid;

#pragma pack (push,1)

/**
  CPU Security PreMemory Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER Header;                      ///< Config Block Header
  UINT32              PrmrrSize;                   ///< PRMRR Size.<b>Software Control: 0x0</b>  32MB: 0x2000000, 64MB: 0x4000000, 128 MB: 0x8000000, 256 MB: 0x10000000, 512 MB: 0x20000000
  UINT16              BiosSize;                    ///< Flash information for BIOS Guard: BIOS Size in KB.
  UINT8               Reserved[2];                 ///< Reserved for future use
/**
   Enable or Disable BIOS Guard; 0: Disable; <b>1: Enable</b>.
    - This is an optional feature and can be opted out.
    - If this policy is set to Disabled, the policies in the BIOS_GUARD_CONFIG will be ignored.
    - If PeiBiosGuardLibNull is used, this policy will have no effect.
**/
  UINT32              BiosGuard               :  1;
  UINT32              BiosGuardToolsInterface :  1; ///< BIOS Guard Tools Interface; <b>0: Disable</b>, 1:Enable
/**
   Enable or Disable Software Guard Extensions; <b>0: Disable</b>; 1: Enable.
    - This is an optional feature and can be opted out.
    - If this policy is set to Disabled, the policies in the CPU_SGX_CONFIG will be ignored.
    - If BaseSoftwareGuardLibNull is used, this policy will have no effect.
**/
  UINT32              EnableSgx               :  1;
/**
   Enable or Disable Trusted Execution Technology; <b>0: Disable</b>; 1: Enable.
    - This is an optional feature and can be opted out.
    - If this policy is set to Disabled, the policies in the CPU_TXT_PREMEM_CONFIG will be ignored.
    - If PeiTxtLibNull is used, this policy will have no effect.
**/
  UINT32              Txt                     :  1;
  UINT32              SkipStopPbet            :  1; ///< <b>(Test)</b> Skip Stop PBET Timer; <b>0: Disable</b>; 1: Enable.
  ///
  /// <b>(Test)</b> This policy indicates whether or not BIOS should allocate PRMRR memory for C6DRAM power gating feature.
  ///  - 0: Don't allocate any PRMRR memory for C6DRAM power gating feature.
  ///  - <b>1: Allocate PRMRR memory for C6DRAM power gating feature</b>.
  ///
  UINT32              EnableC6Dram            :  1;
  UINT32              ResetAux                :  1; ///< <b>(Test)</b> Reset Auxiliary content, <b>0: Disabled</b>, 1: Enabled
  UINT32              TxtAcheckRequest        :  1; ///< <b>(Test)</b> AcheckRequest <b>0: Disabled</b>, 1: Enabled. When Enabled, it will call Acheck regardless of crashcode value
  UINT32              RsvdBits                : 24; ///< Reserved for future use
} CPU_SECURITY_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _CPU_SECURITY_CONFIG_H_
