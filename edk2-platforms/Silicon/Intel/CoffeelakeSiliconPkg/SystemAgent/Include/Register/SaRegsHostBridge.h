/** @file
  Register names for Host Bridge block
  <b>Conventions</b>:
  - Prefixes:
    - Definitions beginning with "R_" are registers
    - Definitions beginning with "B_" are bits within registers
    - Definitions beginning with "V_" are meaningful values of bits within the registers
    - Definitions beginning with "S_" are register sizes
    - Definitions beginning with "N_" are the bit position
  - In general, SA registers are denoted by "_SA_" in register names
  - Registers / bits that are different between SA generations are denoted by
    "_SA_[generation_name]_" in register/bit names. e.g., "_SA_HSW_"
  - Registers / bits that are different between SKUs are denoted by "_[SKU_name]"
    at the end of the register/bit names
  - Registers / bits of new devices introduced in a SA generation will be just named
    as "_SA_" without [generation_name] inserted.

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SA_REGS_HOST_BRIDGE_H_
#define _SA_REGS_HOST_BRIDGE_H_

//
// DEVICE 0 (Memory Controller Hub)
//
#define SA_MC_BUS          0x00
#define SA_MC_DEV          0x00
#define SA_MC_FUN          0x00
#define V_SA_MC_VID        0x8086
#define R_SA_MC_DEVICE_ID  0x02
#define R_SA_MC_CAPID0_B   0xE8

///
/// Maximum number of SDRAM channels supported by the memory controller
///
#define SA_MC_MAX_CHANNELS 2

///
/// Maximum number of DIMM sockets supported by each channel
///
#define SA_MC_MAX_SLOTS 2

///
/// Maximum number of sides supported per DIMM
///
#define SA_MC_MAX_SIDES 2

///
/// Maximum number of DIMM sockets supported by the memory controller
///
#define SA_MC_MAX_SOCKETS (SA_MC_MAX_CHANNELS * SA_MC_MAX_SLOTS)

///
/// Maximum number of rows supported by the memory controller
///
#define SA_MC_MAX_RANKS (SA_MC_MAX_SOCKETS * SA_MC_MAX_SIDES)

///
/// Maximum number of rows supported by the memory controller
///
#define SA_MC_MAX_ROWS (SA_MC_MAX_SIDES * SA_MC_MAX_SOCKETS)

///
/// Maximum memory supported by the memory controller
/// 4 GB in terms of KB
///
#define SA_MC_MAX_MEM_CAPACITY (4 * 1024 * 1024)

///
/// Define the maximum number of data bytes on a system with no ECC memory support.
///
#define SA_MC_MAX_BYTES_NO_ECC (8)

///
/// Define the maximum number of SPD data bytes on a DIMM.
///
#define SA_MC_MAX_SPD_SIZE (512)
//
// Maximum DMI lanes and bundles supported (x8 and 4 lanes)
//
#define SA_DMI_MAX_LANE                      0x04
#define SA_DMI_MAX_BUNDLE                    0x02

#define SA_DMI_CFL_MAX_LANE                  0x04
#define SA_DMI_CFL_MAX_BUNDLE                0x02
//
// KabyLake CPU Mobile SA Device IDs B0:D0:F0
//
#define V_SA_DEVICE_ID_KBL_MB_ULT_1 0x5904   ///< Kabylake Ult (OPI) (2+1F/1.5F/2F/3/3E) Mobile SA DID
//
// KabyLake CPU Halo SA Device IDs B0:D0:F0
//
#define V_SA_DEVICE_ID_KBL_HALO_2   0x5910   ///< Kabylake Halo (4+2/4E/3FE) SA DID
//
// KabyLake CPU Desktop SA Device IDs B0:D0:F0
//
#define V_SA_DEVICE_ID_KBL_DT_2     0x591F   ///< Kabylake Desktop (4+1.5F/2/4) SA DID
//
// KabyLake CPU Server SA Device IDs B0:D0:F0
//
#define V_SA_DEVICE_ID_KBL_SVR_2    0x5918   ///< Kabylake Server (4+1/2/4E) SA DID

//
// CoffeeLake CPU Mobile SA Device IDs B0:D0:F0
//
#define V_SA_DEVICE_ID_CFL_ULT_1        0x3ED0   ///< CoffeeLake Mobile (CFL-U 4+3e) SA DID
#define V_SA_DEVICE_ID_CFL_ULT_2        0x3ECC   ///< CoffeeLake Mobile (CFL-U 2+3e) SA DID
#define V_SA_DEVICE_ID_CFL_ULT_3        0x3E34   ///< CoffeeLake Mobile (CFL-U 4+(1 or 2)) SA DID
#define V_SA_DEVICE_ID_CFL_ULT_4        0x3E35   ///< CoffeeLake Mobile (CFL-U 2+(1 or 2)) SA DID
#define V_SA_DEVICE_ID_CFL_ULT_6        0x3ECC   ///< CoffeeLake Mobile (CFL-U 2+3e) SA DID

#define V_SA_DEVICE_ID_CML_ULT_1        0x9B51  ///< CometLake (CML-U 6+2) SA DID
#define V_SA_DEVICE_ID_CML_ULT_2        0x9B61  ///< CometLake (CML-U 4+2) SA DID
#define V_SA_DEVICE_ID_CML_ULT_3        0x9B71  ///< CometLake (CML-U 2+2) SA DID
//
// CoffeeLake CPU Desktop SA Device IDs B0:D0:F0
//
#define V_SA_DEVICE_ID_CFL_DT_1         0x3EC2   ///< CoffeeLake Desktop (6+2) SA DID
#define V_SA_DEVICE_ID_CFL_DT_2         0x3E1F   ///< CoffeeLake Desktop (4+2) SA DID
#define V_SA_DEVICE_ID_CFL_DT_3         0x3E0F   ///< CoffeeLake Desktop (2+2) SA DID
#define V_SA_DEVICE_ID_CFL_DT_4         0x3E30   ///< CoffeeLake Desktop (8+2) SA DID

//
// CoffeeLake CPU Halo SA Device IDs B0:D0:F0
//
#define V_SA_DEVICE_ID_CFL_HALO_1       0x3EC4   ///< CoffeeLake Halo (6+2) SA DID
#define V_SA_DEVICE_ID_CFL_HALO_2       0x3E10   ///< CoffeeLake Halo (4+2) SA DID
#define V_SA_DEVICE_ID_CFL_HALO_3       0x3E20   ///< CoffeeLake Halo (8+2) SA DID

//
// CoffeeLake CPU WS SA Device IDs B0:D0:F0
//
#define V_SA_DEVICE_ID_CFL_WS_1         0x3EC6   ///< CoffeeLake WorkStation (6+2) SA DID
#define V_SA_DEVICE_ID_CFL_WS_2         0x3E18   ///< CoffeeLake WrokStation (4+2) SA DID
#define V_SA_DEVICE_ID_CFL_WS_3         0x3E31   ///< CoffeeLake WrokStation (8+2) SA DID

//
// CPU Server SA Device IDs B0:D0:F0
//
#define V_SA_DEVICE_ID_CFL_SVR_1        0x3ECA   ///< CoffeeLake Server (6+0) SA DID
#define V_SA_DEVICE_ID_CFL_SVR_2        0x3E32   ///< CoffeeLake Server (8+0) SA DID
#define V_SA_DEVICE_ID_CFL_SVR_3        0x3E33   ///< CoffeeLake Server (4+0) SA DID
/**
 <b>Description</b>:
 - This is the base address for the Host Memory Mapped Configuration space.  There is no physical memory within this 32KB window that can be addressed.  The 32KB reserved by this register does not alias to any PCI 2.3 compliant memory mapped space.  On reset, the Host MMIO Memory Mapped Configuation space is disabled and must be enabled by writing a 1 to MCHBAREN [Dev 0, offset48h, bit 0].
 - All the bits in this register are locked in LT mode.
 - The register space contains memory control, initialization, timing, and buffer strength registers; clocking registers; and power and thermal management registers.
**/
#define R_SA_MCHBAR  (0x48)
/**
 <b>Description</b>:
 - All the bits in this register are LT lockable.
**/
#define R_SA_GGC (0x50)
#define N_SA_GGC_GMS_OFFSET  (0x8)
#define B_SA_GGC_GMS_MASK    (0xff00)
#define N_SA_GGC_GGMS_OFFSET  (0x6)
#define B_SA_GGC_GGMS_MASK    (0xc0)
#define V_SA_GGC_GGMS_8MB     3
/**
 Description:
 - Allows for enabling/disabling of PCI devices and functions that are within the CPU package. The table below the bit definitions describes the behavior of all combinations of transactions to devices controlled by this register.
  All the bits in this register are LT Lockable.
**/
#define R_SA_DEVEN (0x54)
#define B_SA_DEVEN_D2EN_MASK     (0x10)
/**
 Description:
  This is the base address for the PCI Express configuration space.  This window of addresses contains the 4KB of configuration space for each PCI Express device that can potentially be part of the PCI Express Hierarchy associated with the Uncore.  There is no actual physical memory within this window of up to 256MB that can be addressed.  The actual size of this range is determined by a field in this register.
  Each PCI Express Hierarchy requires a PCI Express BASE register.  The Uncore supports one PCI Express Hierarchy.  The region reserved by this register does not alias to any PCI2.3 compliant memory mapped space.  For example, the range reserved for MCHBAR is outside of PCIEXBAR space.
  On reset, this register is disabled and must be enabled by writing a 1 to the enable field in this register.  This base address shall be assigned on a boundary consistent with the number of buses (defined by the length field in this register), above TOLUD and still within 39-bit addressable memory space.
  The PCI Express Base Address cannot be less than the maximum address written to the Top of physical memory register (TOLUD).  Software must guarantee that these ranges do not overlap with known ranges located above TOLUD.
  Software must ensure that the sum of the length of the enhanced configuration region + TOLUD + any other known ranges reserved above TOLUD is not greater than the 39-bit addessable limit of 512GB.  In general, system implementation and the number of PCI/PCI Express/PCI-X buses supported in the hierarchy will dictate the length of the region.
  All the bits in this register are locked in LT mode.
**/
#define R_SA_PCIEXBAR  (0x60)

/**
 Description:
 - This register controls the read, write and shadowing attributes of the BIOS range from F_0000h to F_FFFFh.  The Uncore allows programmable memory attributes on 13 legacy memory segments of various sizes in the 768KB to 1MB address range.  Seven Programmable Attribute Map (PAM) registers are used to support these features.  Cacheability of these areas is controlled via the MTRR register in the core.
 - Two bits are used to specify memory attributes for each memory segment.  These bits apply to host accesses to the PAM areas.  These attributes are:
 - RE - Read Enable.  When RE=1, the host read accesses to the corresponding memory segment are claimed by the Uncore and directed to main memory.  Conversely, when RE=0, the host read accesses are directed to DMI.
 - WE - Write Enable.  When WE=1, the host write accesses to the corresponding memory segment are claimed by the Uncore and directed to main memory.  Conversely, when WE=0, the host read accesses are directed to DMI.
 - The RE and WE attributes permit a memory segment to be Read Only, Write Only, Read/Write or Disabled.  For example, if a memory segment has RE=1 and WE=0, the segment is Read Only.
**/
#define R_SA_PAM0  (0x80)

///
/// Description:
///  The SMRAMC register controls how accesses to Compatible SMRAM spaces are treated.  The Open, Close and Lock bits function only when G_SMRAME bit is set to 1.  Also, the Open bit must be reset before the Lock bit is set.
///
#define R_SA_SMRAMC  (0x88)
#define B_SA_SMRAMC_D_LCK_MASK     (0x10)
#define B_SA_SMRAMC_D_CLS_MASK     (0x20)
#define B_SA_SMRAMC_D_OPEN_MASK    (0x40)
///
/// Description:
///  This register contains the Top of low memory address.
///
#define R_SA_TOLUD (0xbc)
#define R_SA_MC_CAPID0_A_OFFSET    0xE4
//
// MCHBAR IO Register Offset Equates
//
#define R_SA_MCHBAR_BIOS_RESET_CPL_OFFSET          0x5DA8

#define V_SA_LTR_MAX_SNOOP_LATENCY_VALUE           0x0846  ///< Intel recommended maximum value for Snoop Latency (70us)
#define V_SA_LTR_MAX_NON_SNOOP_LATENCY_VALUE       0x0846  ///< Intel recommended maximum value for Non-Snoop Latency (70us)
///
/// Vt-d Engine base address.
///
#define R_SA_MCHBAR_VTD1_OFFSET                 0x5400  ///< HW UNIT1 for IGD
#define R_SA_MCHBAR_VTD3_OFFSET      0x5410  ///< HW UNIT3 for all other - PEG, USB, SATA etc
#endif
