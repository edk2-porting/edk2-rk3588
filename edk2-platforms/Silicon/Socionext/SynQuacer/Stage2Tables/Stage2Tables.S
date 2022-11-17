/** @file
  Copyright (c) 2018, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

/*
 * This file contains the assembler code to instantiate a set of stage 2
 * translation tables that make the ECAM space of the Synopsys DesignWare
 * PCIe root complexes appear sane to the OS.
 * - ECAM 'shadows' caused by non TLP filtering root ports are eliminated
 * - MMIO region are mapped with device attributes that supersede write combine
 *   attributes that the OS may attempt to use, and which is not supported by
 *   the SoC.
 */

#define TT_S2_CONT_SHIFT            52
#define TT_S2_AF                    (0x1 << 10)
#define TT_S2_SH_NON_SHAREABLE      (0x0 << 8)
#define TT_S2_AP_RO                 (0x1 << 6)
#define TT_S2_AP_RW                 (0x3 << 6)
#define TT_S2_MEMATTR_DEVICE_nGRE   (0x2 << 2)
#define TT_S2_MEMATTR_DEVICE_nGnRE  (0x1 << 2)
#define TT_S2_MEMATTR_MEMORY_WB     (0xf << 2)
#define TT_S2_TABLE                 (0x3 << 0)
#define TT_S2_L3_PAGE               (0x1 << 1)
#define TT_S2_VALID                 (0x1 << 0)

#ifdef __aarch64__
#define QWORD(x)    .quad (x)
#else
#define QWORD(x)    .long (x), 0
#endif

  .altmacro
  .macro    for, start, count, do, arg2, arg3, arg4
  .if       \count == 1
  \do       \start, \arg2, \arg3, \arg4
  .elseif   \count > 1
  for       \start, %(\count / 2), \do, \arg2, \arg3, \arg4
  for       %(\start + \count / 2), %((\count + 1) / 2), \do, \arg2, \arg3, \arg4
  .endif
  .endm

  .macro    s2_dev_entry, base, shift=30, offset=0, cont=0
  .quad     ((\base << \shift) + \offset) | TT_S2_AF | TT_S2_AP_RW | \
            TT_S2_SH_NON_SHAREABLE | TT_S2_MEMATTR_DEVICE_nGRE | \
            TT_S2_VALID | (\cont << TT_S2_CONT_SHIFT)
  .endm

  .macro    s2_mem_entry, base, shift=30, offset=0, cont=0
  .quad     ((\base << \shift) + \offset) | TT_S2_AF | TT_S2_AP_RW | \
            TT_S2_SH_NON_SHAREABLE | TT_S2_MEMATTR_MEMORY_WB | \
            TT_S2_VALID | (\cont << TT_S2_CONT_SHIFT)
  .endm

  .macro    s2_l3_entry, base, offset=0, cont=0
  .quad     ((\base << 12) + \offset) | TT_S2_AF | TT_S2_AP_RW | \
            TT_S2_SH_NON_SHAREABLE | TT_S2_MEMATTR_MEMORY_WB | \
            TT_S2_L3_PAGE | TT_S2_VALID | (\cont << TT_S2_CONT_SHIFT)
  .endm

  .macro    smmu_l3_entry, base, offset=0, ignore=0
  .quad     ((\base << 12) + \offset) | TT_S2_AF | TT_S2_AP_RO | \
            TT_S2_SH_NON_SHAREABLE | TT_S2_MEMATTR_DEVICE_nGnRE | \
            TT_S2_L3_PAGE | TT_S2_VALID
  .endm

  .section  ".rodata", "a", %progbits
  /* level 1 */
  s2_mem_entry  0      /* 0x0000_0000 - 0x3fff_ffff */
  QWORD   (1f + TT_S2_TABLE) /* 0x4000_0000 - 0x7fff_ffff */
  for       2, 246, s2_mem_entry  /* 0x8000_0000 - 0x3d_ffff_ffff */
  for     248,   8, s2_dev_entry  /* PCIe MMIO64 */
  for     256, 768, s2_mem_entry  /* 0x40_0000_0000 - 0xff_ffff_ffff */

  /* level 2 */
1:for     0, 256, s2_mem_entry, 21, 0x40000000, 1

  QWORD   (2f + TT_S2_TABLE) /* 0x6000_0000 -> RC #0 bus 0 */
  for     1, 15, s2_mem_entry, 21, 0x60000000
  for     0, 48, s2_mem_entry, 21, 0x62000000, 1
  for     0, 64, s2_dev_entry, 21, 0x68000000, 1 /* PCIe MMIO32 */

  QWORD   (3f + TT_S2_TABLE) /* 0x7000_0000 -> RC #1 bus 0 */
  for     1, 15, s2_mem_entry, 21, 0x70000000
  for     0, 48, s2_mem_entry, 21, 0x72000000, 1
  for     0, 64, s2_dev_entry, 21, 0x78000000, 1 /* PCIe MMIO32 */

  /* level 3 */
2:for     0,   8, s2_l3_entry, 0x60000000
  for     0,   8, s2_l3_entry, 0x60010000  /* hide device #1 */
  for     0, 496, s2_l3_entry, 0x60010000, 1
3:for     0,   8, s2_l3_entry, 0x70000000
  for     0,   8, s2_l3_entry, 0x70010000  /* hide device #1 */
  for     0, 496, s2_l3_entry, 0x70010000, 1

  /* level 3 for north SMMU */
  .org    0x6000
  for     0,   8, smmu_l3_entry, 0xc00060000000
  for     0,   8, smmu_l3_entry, 0xc00060010000  /* hide device #1 */
  for     0, 496, smmu_l3_entry, 0xc00060010000
  for     0,   8, smmu_l3_entry, 0x800070000000
  for     0,   8, smmu_l3_entry, 0x800070010000  /* hide device #1 */
  for     0, 496, smmu_l3_entry, 0x800070010000
