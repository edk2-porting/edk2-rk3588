/** @file
  Register names for System Agent (SA) registers
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

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _SA_REGS_H_
#define _SA_REGS_H_

//
// DEVICE 0 (Memory Controller Hub)
//
#define SA_MC_BUS          0x00
#define SA_MC_DEV          0x00
#define SA_MC_FUN          0x00
#define V_SA_MC_VID        0x8086
#define R_SA_MC_DEVICE_ID  0x02
#define R_SA_MC_CAPID0_B   0xE8

//
// Macros that judge which type a device ID belongs to
//

//
// CPU Mobile SA Device IDs B0:D0:F0
//
#define V_SA_DEVICE_ID_SKL_MB_ULT_1 0x1904   ///< Skylake Ult (OPI) (2+1F/1.5F/2F/3/3E) Mobile SA DID
#define V_SA_DEVICE_ID_SKL_MB_ULX_2 0x190C   ///< Skylake Ulx (OPI) (2+1F/1.5F/2) SA DID
#define V_SA_DEVICE_ID_SKL_MB_ULX_3 0x1924   ///< Skylake Ulx (OPI)
//
// CPU Halo SA Device IDs B0:D0:F0
//
#define V_SA_DEVICE_ID_SKL_HALO_1   0x1900   ///< Skylake Halo (2+2/1) SA DID
#define V_SA_DEVICE_ID_SKL_HALO_2   0x1910   ///< Skylake Halo (4+2/4E/3FE) SA DID
#define V_SA_DEVICE_ID_SKL_HALO_3   0x1918   ///< Skylake Halo (4+3E/4E) Embedded SA DID
//
// CPU Desktop SA Device IDs B0:D0:F0
//
#define V_SA_DEVICE_ID_SKL_DT_1     0x190F   ///< Skylake Desktop (2+1F/1.5F/2) SA DID
#define V_SA_DEVICE_ID_SKL_DT_2     0x191F   ///< Skylake Desktop (4+2/4) SA DID
//
// CPU Server SA Device IDs B0:D0:F0
//
#define V_SA_DEVICE_ID_SKL_SVR_1    0x1908   ///< Skylake Server (2+2/3E) SA DID
#define V_SA_DEVICE_ID_SKL_SVR_2    0x1918   ///< Skylake Server (4+1/2/4E) SA DID

//
// KabyLake CPU Mobile SA Device IDs B0:D0:F0
//
#define V_SA_DEVICE_ID_KBL_MB_ULT_1  0x5904   ///< Kabylake Ult (OPI) (2+1F/1.5F/2F/3/3E) Mobile SA DID
#define V_SA_DEVICE_ID_KBLR_MB_ULT_1 0x5914   ///< Kabylake-R Ult Mobile SA DID
#define V_SA_DEVICE_ID_KBL_MB_ULX_1  0x590C   ///< Kabylake Ulx (OPI) (2+1F/1.5F/2) SA DID
//
// KabyLake CPU Halo SA Device IDs B0:D0:F0
//
#define V_SA_DEVICE_ID_KBL_HALO_1   0x5900   ///< Kabylake Halo (2+2/1) SA DID
#define V_SA_DEVICE_ID_KBL_HALO_2   0x5910   ///< Kabylake Halo (4+2/4E/3FE) SA DID
//
// KabyLake CPU Desktop SA Device IDs B0:D0:F0
//
#define V_SA_DEVICE_ID_KBL_DT_1     0x590F   ///< Kabylake Desktop (2+1F/1.5F/2) SA DID
#define V_SA_DEVICE_ID_KBL_DT_2     0x591F   ///< Kabylake Desktop (4+1.5F/2/4) SA DID
//
// KabyLake CPU Server SA Device IDs B0:D0:F0
//
#define V_SA_DEVICE_ID_KBL_SVR_1    0x5908   ///< Kabylake Server (2+2/3E) SA DID
#define V_SA_DEVICE_ID_KBL_SVR_2    0x5918   ///< Kabylake Server (4+1/2/4E) SA DID
//
// CoffeeLake CPU Desktop SA Device IDs B0:D0:F0
//
#define V_SA_DEVICE_ID_CFL_DT       0x3EC2   ///< CoffeeLake Desktop (6+2) SA DID

///
/// Device IDs that are Mobile specific B0:D0:F0
///
#define IS_SA_DEVICE_ID_MOBILE(DeviceId) \
    ( \
      (DeviceId == V_SA_DEVICE_ID_SKL_MB_ULT_1) || \
      (DeviceId == V_SA_DEVICE_ID_SKL_MB_ULX_2) || \
      (DeviceId == V_SA_DEVICE_ID_SKL_MB_ULX_3) || \
      (DeviceId == V_SA_DEVICE_ID_KBL_MB_ULT_1) || \
      (DeviceId == V_SA_DEVICE_ID_KBLR_MB_ULT_1) || \
      (DeviceId == V_SA_DEVICE_ID_KBL_MB_ULX_1) \
    )

///
/// Device IDs that are Desktop specific B0:D0:F0
///
#define IS_SA_DEVICE_ID_DESKTOP(DeviceId) \
    ( \
      (DeviceId == V_SA_DEVICE_ID_SKL_DT_1) || \
      (DeviceId == V_SA_DEVICE_ID_SKL_DT_2) || \
      (DeviceId == V_SA_DEVICE_ID_KBL_DT_1) || \
      (DeviceId == V_SA_DEVICE_ID_KBL_DT_2) || \
      (DeviceId == V_SA_DEVICE_ID_CFL_DT) \
    )

///
/// Device IDS that are Server specific B0:D0:F0
///
#define IS_SA_DEVICE_ID_SERVER(DeviceId) \
    ( \
      (DeviceId == V_SA_DEVICE_ID_SKL_SVR_1) || \
      (DeviceId == V_SA_DEVICE_ID_SKL_SVR_2) || \
      (DeviceId == V_SA_DEVICE_ID_KBL_SVR_1) || \
      (DeviceId == V_SA_DEVICE_ID_KBL_SVR_2) \
    )

///
/// Device IDs that are Halo specific B0:D0:F0
///
#define IS_SA_DEVICE_ID_HALO(DeviceId) \
    ( \
      (DeviceId == V_SA_DEVICE_ID_SKL_HALO_1) || \
      (DeviceId == V_SA_DEVICE_ID_SKL_HALO_2) || \
      (DeviceId == V_SA_DEVICE_ID_SKL_HALO_3) || \
      (DeviceId == V_SA_DEVICE_ID_KBL_HALO_1) || \
      (DeviceId == V_SA_DEVICE_ID_KBL_HALO_2) \
    )

/**
 <b>Description</b>:
  This is the base address for the PCI Express Egress Port MMIO Configuration space.  There is no physical memory within this 4KB window that can be addressed.  The 4KB reserved by this register does not alias to any PCI 2.3 compliant memory mapped space.  On reset, the EGRESS port MMIO configuration space is disabled and must be enabled by writing a 1 to PXPEPBAREN [Dev 0, offset 40h, bit 0].
  All the bits in this register are locked in LT mode.
**/
#define R_SA_PXPEPBAR  (0x40)
//
// Description of PXPEPBAREN (0:0)
// - 0:  PXPEPBAR is disabled and does not claim any memory
// - 1:  PXPEPBAR memory mapped accesses are claimed and decoded appropriately
// - This register is locked by LT.
//
#define N_SA_PXPEPBAR_PXPEPBAREN_OFFSET  (0x0)
#define S_SA_PXPEPBAR_PXPEPBAREN_WIDTH   (0x1)
#define B_SA_PXPEPBAR_PXPEPBAREN_MASK    (0x1)
#define V_SA_PXPEPBAR_PXPEPBAREN_DEFAULT (0x0)
//
// Description of PXPEPBAR (12:38)
// - This field corresponds to bits 38 to 12 of the base address PCI Express Egress Port MMIO configuration space.  BIOS will program this register resulting in a base address for a 4KB block of contiguous memory address space.  This register ensures that a naturally aligned 4KB space is allocated within the first 512GB of addressable memory space.  System Software uses this base address to program the PCI Express Egress Port MMIO register set.  All the bits in this register are locked in LT mode.
//
#define N_SA_PXPEPBAR_PXPEPBAR_OFFSET  (0xc)
#define S_SA_PXPEPBAR_PXPEPBAR_WIDTH   (0x1b)
#define B_SA_PXPEPBAR_PXPEPBAR_MASK    (0x7ffffff000)
#define V_SA_PXPEPBAR_PXPEPBAR_DEFAULT (0x0)

/**
 <b>Description</b>:
 - This is the base address for the Host Memory Mapped Configuration space.  There is no physical memory within this 32KB window that can be addressed.  The 32KB reserved by this register does not alias to any PCI 2.3 compliant memory mapped space.  On reset, the Host MMIO Memory Mapped Configuation space is disabled and must be enabled by writing a 1 to MCHBAREN [Dev 0, offset48h, bit 0].
 - All the bits in this register are locked in LT mode.
 - The register space contains memory control, initialization, timing, and buffer strength registers; clocking registers; and power and thermal management registers.
**/
#define R_SA_MCHBAR  (0x48)
/**
 Description of MCHBAREN (0:0)
 - 0: MCHBAR is disabled and does not claim any memory
 - 1: MCHBAR memory mapped accesses are claimed and decoded appropriately
 - This register is locked by LT.
**/
#define N_SA_MCHBAR_MCHBAREN_OFFSET  (0x0)
#define S_SA_MCHBAR_MCHBAREN_WIDTH   (0x1)
#define B_SA_MCHBAR_MCHBAREN_MASK    (0x1)
#define V_SA_MCHBAR_MCHBAREN_DEFAULT (0x0)
/**
 Description of MCHBAR (15:38)
 - This field corresponds to bits 38 to 15 of the base address Host Memory Mapped configuration space.  BIOS will program this register resulting in a base address for a 32KB block of contiguous memory address space.  This register ensures that a naturally aligned 32KB space is allocated within the first 512GB of addressable memory space. System Software uses this base address to program the Host Memory Mapped register set. All the bits in this register are locked in LT mode.
**/
#define N_SA_MCHBAR_MCHBAR_OFFSET  (0xf)
#define S_SA_MCHBAR_MCHBAR_WIDTH   (0x18)
#define B_SA_MCHBAR_MCHBAR_MASK    (0x7fffff8000ULL)
#define V_SA_MCHBAR_MCHBAR_DEFAULT (0x0)

/**
 <b>Description</b>:
 - All the bits in this register are LT lockable.
**/
#define R_SA_GGC (0x50)
/**
 Description of GGCLCK (0:0)
 - When set to 1b, this bit will lock all bits in this register.
**/
#define N_SA_GGC_GGCLCK_OFFSET   (0x0)
#define S_SA_GGC_GGCLCK_WIDTH    (0x1)
#define B_SA_GGC_GGCLCK_MASK     (0x1)
#define V_SA_GGC_GGCLCK_DEFAULT  (0x0)
/**
 Description of IVD (1:1)
 - 0: Enable.  Device 2 (IGD) claims VGA memory and IO cycles, the Sub-Class Code within Device 2 Class Code register is 00.
 - 1: Disable.  Device 2 (IGD) does not claim VGA cycles (Mem and IO), and the Sub- Class Code field within Device 2 function 0 Class Code register is 80.
 - BIOS Requirement:  BIOS must not set this bit to 0 if the GMS field (bits 7:3 of this register) pre-allocates no memory.
 - This bit MUST be set to 1 if Device 2 is disabled either via a fuse or fuse override (CAPID0[46] = 1) or via a register (DEVEN[3] = 0).
 - This register is locked by LT lock.
**/
#define N_SA_GGC_IVD_OFFSET  (0x1)
#define S_SA_GGC_IVD_WIDTH   (0x1)
#define B_SA_GGC_IVD_MASK    (0x2)
#define V_SA_GGC_IVD_DEFAULT (0x0)
/**
 For SKL
 Description of GMS (8:15)
 - This field is used to select the amount of Main Memory that is pre-allocated to support the Internal Graphics device in VGA (non-linear) and Native (linear) modes.  The BIOS ensures that memory is pre-allocated only when Internal graphics is enabled.
 - This register is also LT lockable.
 - Valid options are 0 (0x0) to 2048MB (0x40) in multiples of 32 MB
 - Default is 64MB
 - All other values are reserved
 - Hardware does not clear or set any of these bits automatically based on IGD being disabled/enabled.
 - BIOS Requirement: BIOS must not set this field to 0h if IVD (bit 1 of this register) is 0.
**/
#define N_SKL_SA_GGC_GMS_OFFSET  (0x8)
#define S_SKL_SA_GGC_GMS_WIDTH   (0x8)
#define B_SKL_SA_GGC_GMS_MASK    (0xff00)
#define V_SKL_SA_GGC_GMS_DEFAULT (0x01)

/**
 For SKL
 Description of GGMS (6:7)
 - This field is used to select the amount of Main Memory that is pre-allocated to support the Internal Graphics Translation Table.  The BIOS ensures that memory is pre-allocated only when Internal graphics is enabled.
 - GSM is assumed to be a contiguous physical DRAM space with DSM, and BIOS needs to allocate a contiguous memory chunk.  Hardware will derive the base of GSM from DSM only using the GSM size programmed in the register.
 - Valid options:
 - 0h: 0 MB of memory pre-allocated for GTT.
 - 1h: 2 MB of memory pre-allocated for GTT.
 - 2h: 4 MB of memory pre-allocated for GTT. (default)
 - 3h: 8 MB of memory pre-allocated for GTT.
 - Others: Reserved
 - Hardware functionality in case of programming this value to Reserved is not guaranteed.
**/
#define N_SKL_SA_GGC_GGMS_OFFSET  (0x6)
#define S_SKL_SA_GGC_GGMS_WIDTH   (0x2)
#define B_SKL_SA_GGC_GGMS_MASK    (0xc0)
#define V_SKL_SA_GGC_GGMS_DEFAULT (2)
#define V_SKL_SA_GGC_GGMS_DIS     0
#define V_SKL_SA_GGC_GGMS_2MB     1
#define V_SKL_SA_GGC_GGMS_4MB     2
#define V_SKL_SA_GGC_GGMS_8MB     3

/**
 Description:
 - Allows for enabling/disabling of PCI devices and functions that are within the CPU package. The table below the bit definitions describes the behavior of all combinations of transactions to devices controlled by this register.
  All the bits in this register are LT Lockable.
**/
#define R_SA_DEVEN (0x54)
#define B_SA_DEVEN_D2EN_MASK     (0x10)

/**
 Description
 - Protected Audio Video Path Control
 - All the bits in this register are locked by LT.  When locked the R/W bits are RO.
**/
#define R_SA_PAVPC (0x58)
/**
 Description of PCME (0:0)
 - This field enables Protected Content Memory within Graphics Stolen Memory.
 - This register is locked (becomes read-only) when PAVPLCK = 1b.
 - This register is read-only (stays at 0b) when PAVP fuse is set to "disabled"
 - 0: Protected Content Memory is disabled
 - 1: Protected Content Memory is enabled
**/
#define N_SA_PAVPC_PCME_OFFSET   (0x0)
#define S_SA_PAVPC_PCME_WIDTH    (0x1)
#define B_SA_PAVPC_PCME_MASK     (0x1)
#define V_SA_PAVPC_PCME_MASK     (0x0)
/**
 Description of PAVPE (1:1)
 - 0: PAVP path is disabled
 - 1: PAVP path is enabled
 - This register is locked (becomes read-only) when PAVPLCK = 1b
 - This register is read-only (stays at 0b) when PAVP capability is set to "disabled" as defined by CAPID0_B[PAVPE].
**/
#define N_SA_PAVPC_PAVPE_OFFSET  (0x1)
#define S_SA_PAVPC_PAVPE_WIDTH   (0x1)
#define B_SA_PAVPC_PAVPE_MASK    (0x2)
#define V_SA_PAVPC_PAVPE_DEFAULT (0x0)
/**
 Description of PAVPLCK (2:2)
 - This bit will lock all writeable contents in this register when set (including itself).
 - This bit will be locked if PAVP is fused off.
**/
#define N_SA_PAVPC_PAVPLCK_OFFSET  (0x2)
#define S_SA_PAVPC_PAVPLCK_WIDTH   (0x1)
#define B_SA_PAVPC_PAVPLCK_MASK    (0x4)
#define V_SA_PAVPC_PAVPLCK_DEFAULT (0x0)
/**
 Description of PCMBASE (20:31)
 - This field is used to set the base of Protected Content Memory.
 - This corresponds to bits 31:20 of the system memory address range, giving a 1MB granularity. This value MUST be at least 1MB above the base and below the top of stolen memory.
 - This register is locked (becomes read-only) when PAVPE = 1b.
**/
#define N_SA_PAVPC_PCMBASE_OFFSET  (0x14)
#define S_SA_PAVPC_PCMBASE_WIDTH   (0xc)
#define B_SA_PAVPC_PCMBASE_MASK    (0xfff00000)
#define V_SA_PAVPC_PCMBASE_DEFAULT (0x0)

#define R_SA_DPR (0x5c) ///< DMA protected range register
/**
 Description of LOCK (0:0)
 - This bit will lock all writeable settings in this register, including itself.
**/
#define N_SA_DPR_LOCK_OFFSET   (0x0)
#define S_SA_DPR_LOCK_WIDTH    (0x1)
#define B_SA_DPR_LOCK_MASK     (0x1)
#define V_SA_DPR_LOCK_DEFAULT  (0x0)
/**
 Description of PRS (1:1)
 - This field indicates the status of DPR.
 - 0: DPR protection disabled
 - 1: DPR protection enabled
**/
#define N_SA_DPR_PRS_OFFSET   (0x1)
#define S_SA_DPR_PRS_WIDTH    (0x1)
#define B_SA_DPR_PRS_MASK     (0x2)
#define V_SA_DPR_PRS_DEFAULT  (0x0)
/**
 Description of EPM (2:2)
 - This field controls DMA accesses to the DMA Protected Range (DPR) region.
 - 0: DPR is disabled
 - 1: DPR is enabled.  All DMA requests accessing DPR region are blocked.
 - HW reports the status of DPR enable/disable through the PRS field in this register.
**/
#define N_SA_DPR_EPM_OFFSET  (0x2)
#define S_SA_DPR_EPM_WIDTH   (0x1)
#define B_SA_DPR_EPM_MASK    (0x4)
#define V_SA_DPR_EPM_DEFAULT (0x0)
/**
 Description of DPRSIZE (11:4)
 - This field is used to specify the size of memory protected from DMA access in MB
 - The maximum amount of memory that will be protected is 255MB
 - The Top of protected range is the base of TSEG-1
**/
#define N_DPR_DPRSIZE_OFFSET  (0x4)
#define V_DPR_DPRSIZE_WIDTH   (0x8)
#define V_DPR_DPRSIZE_MASK    (0xFF0)
#define V_DPR_DPRSIZE_DEFAULT (0x0)
/**
 Description of TOPOFDPR (31:20)
 - This is the Top address 1 of DPR - Base of TSEG
**/
#define N_SA_DPR_TOPOFDPR_OFFSET  (20)
#define S_SA_DPR_TOPOFDPR_WIDTH   (0xC)
#define B_SA_DPR_TOPOFDPR_MASK    (0xFFF00000)
#define V_SA_DPR_TOPOFDPR_DEFAULT (0x0)

/**
 This is the base address for the Root Complex configuration space. This window of addresses contains the Root Complex Register set for the PCI Express Hierarchy associated with the Host Bridge. There is no physical memory within this 4KB window that can be addressed. The 4KB reserved by this register does not alias to any PCI 2.3 compliant memory mapped space. On reset, the Root Complex configuration space is disabled and must be enabled by writing a 1 to DMIBAREN [Dev 0, offset 68h, bit 0] All the bits in this register are locked in LT mode.
**/
#define R_SA_DMIBAR  (0x68)
/**
 Description of DMIBAREN (0:0)
 - 0: DMIBAR is disabled and does not claim any memory
 - 1: DMIBAR memory mapped accesses are claimed and decoded appropriately
 - This register is locked by LT.
**/
#define N_SA_DMIBAR_DMIBAREN_OFFSET  (0x0)
#define S_SA_DMIBAR_DMIBAREN_WIDTH   (0x1)
#define B_SA_DMIBAR_DMIBAREN_MASK    (0x1)
#define V_SA_DMIBAR_DMIBAREN_DEFAULT (0x0)
/**
 Description of DMIBAR (12:38)
 - This field corresponds to bits 38 to 12 of the base address DMI configuration space. BIOS will program this register resulting in a base address for a 4KB block of contiguous memory address space. This register ensures that a naturally aligned 4KB space is allocated within the first 512GB of addressable memory space. System Software uses this base address to program the DMI register set. All the Bits in this register are locked in LT mode.
**/
#define N_SA_DMIBAR_DMIBAR_OFFSET  (0xc)
#define S_SA_DMIBAR_DMIBAR_WIDTH   (0x1b)
#define B_SA_DMIBAR_DMIBAR_MASK    (0x7ffffff000)
#define V_SA_DMIBAR_DMIBAR_DEFAULT (0x0)

/**
 Description:
 - This register determines the Mask Address register of the memory range that is pre-allocated to the Manageability Engine.  Together with the MESEG_BASE register it controls the amount of memory allocated to the ME.
 - This register is locked by LT.
**/
#define R_SA_MESEG_MASK  (0x78)
/**
 Description of MELCK (10:10)
 - This field indicates whether all bits in the MESEG_BASE and MESEG_MASK registers are locked.  When locked, updates to any field for these registers must be dropped.
**/
#define N_SA_MESEG_MASK_MELCK_OFFSET   (0xa)
#define S_SA_MESEG_MASK_MELCK_WIDTH    (0x1)
#define B_SA_MESEG_MASK_MELCK_MASK     (0x400)
#define V_SA_MESEG_MASK_MELCK_DEFAULT  (0x0)
/**
 Description of ME_STLEN_EN (11:11)
 - Indicates whether the ME stolen  Memory range is enabled or not.
**/
#define N_SA_MESEG_MASK_ME_STLEN_EN_OFFSET   (0xb)
#define S_SA_MESEG_MASK_ME_STLEN_EN_WIDTH    (0x1)
#define V_SA_MESEG_MASK_ME_STLEN_EN_DEFAULT  (0x0)
#define B_SA_MESEG_MASK_ME_STLEN_EN_MASK     (0x800)
/**
 Description of MEMASK (20:38)
 - This field indicates the bits that must match MEBASE in order to qualify as an ME Memory Range access.
 - For example, if the field is set to 7FFFFh, then ME Memory is 1MB in size.
 - Another example is that if the field is set to 7FFFEh, then ME Memory is 2MB in size.
 - In other words, the size of ME Memory Range is limited to power of 2 times 1MB.
**/
#define N_SA_MESEG_MASK_MEMASK_OFFSET  (0x14)
#define S_SA_MESEG_MASK_MEMASK_WIDTH   (0x13)
#define V_SA_MESEG_MASK_MEMASK_DEFAULT (0x0)
#define B_SA_MESEG_MASK_MEMASK_MASK    (0x7ffff00000)
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
///  This register controls the read, write and shadowing attributes of the BIOS range from E_0000h to E_7FFFh.  The Uncore allows programmable memory attributes on 13 legacy memory segments of various sizes in the 768KB to 1MB address range.  Seven Programmable Attribute Map (PAM) registers are used to support these features.  Cacheability of these areas is controlled via the MTRR register in the core.
///  Two bits are used to specify memory attributes for each memory segment.  These bits apply to host accesses to the PAM areas.  These attributes are:
///  RE - Read Enable.  When RE=1, the host read accesses to the corresponding memory segment are claimed by the Uncore and directed to main memory.  Conversely, when RE=0, the host read accesses are directed to DMI.
///  WE - Write Enable.  When WE=1, the host write accesses to the corresponding memory segment are claimed by the Uncore and directed to main memory.  Conversely, when WE=0, the host read accesses are directed to DMI.
///  The RE and WE attributes permit a memory segment to be Read Only, Write Only, Read/Write or Disabled.  For example, if a memory segment has RE=1 and WE=0, the segment is Read Only.
///
#define R_SA_PAM5  (0x85)
///
/// Description:
///  This register controls the read, write and shadowing attributes of the BIOS range from E_8000h to E_FFFFh.  The Uncore allows programmable memory attributes on 13 legacy memory segments of various sizes in the 768KB to 1MB address range.  Seven Programmable Attribute Map (PAM) registers are used to support these features.  Cacheability of these areas is controlled via the MTRR register in the core.
///  Two bits are used to specify memory attributes for each memory segment.  These bits apply to host accesses to the PAM areas.  These attributes are:
///  RE - Read Enable.  When RE=1, the host read accesses to the corresponding memory segment are claimed by the Uncore and directed to main memory.  Conversely, when RE=0, the host read accesses are directed to DMI.
///  WE - Write Enable.  When WE=1, the host write accesses to the corresponding memory segment are claimed by the Uncore and directed to main memory.  Conversely, when WE=0, the host read accesses are directed to DMI.
///  The RE and WE attributes permit a memory segment to be Read Only, Write Only, Read/Write or Disabled.  For example, if a memory segment has RE=1 and WE=0, the segment is Read Only.
///
#define R_SA_PAM6  (0x86)
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
///
#define R_SA_REMAPBASE (0x90)
///
/// Description of LOCK (0:0)
///  This bit will lock all writeable settings in this register, including itself.
///
#define N_SA_REMAPBASE_LOCK_OFFSET   (0x0)
#define S_SA_REMAPBASE_LOCK_WIDTH    (0x1)
#define B_SA_REMAPBASE_LOCK_MASK     (0x1)
#define V_SA_REMAPBASE_LOCK_DEFAULT  (0x0)
///
/// Description of REMAPBASE (20:38)
///  The value in this register defines the lower boundary of the Remap window. The Remap window is inclusive of this address. In the decoder A[19:0] of the Remap Base Address are assumed to be 0's. Thus the bottom of the defined memory range will be aligned to a 1MB boundary.
///  When the value in this register is greater than the value programmed into the Remap Limit register, the Remap window is disabled.
///  These bits are LT lockable.
///
#define N_SA_REMAPBASE_REMAPBASE_OFFSET  (0x14)
#define S_SA_REMAPBASE_REMAPBASE_WIDTH   (0x13)
#define V_SA_REMAPBASE_REMAPBASE_DEFAULT (0x7ffff00000)
#define B_SA_REMAPBASE_REMAPBASE_MASK    (0x7ffff00000)

///
/// Description:
///
#define R_SA_REMAPLIMIT  (0x98)
///
/// Description of LOCK (0:0)
///  This bit will lock all writeable settings in this register, including itself.
///
#define N_SA_REMAPLIMIT_LOCK_OFFSET  (0x0)
#define S_SA_REMAPLIMIT_LOCK_WIDTH   (0x1)
#define B_SA_REMAPLIMIT_LOCK_MASK    (0x1)
#define V_SA_REMAPLIMIT_LOCK_DEFAULT (0x0)
///
/// Description of REMAPLMT (20:38)
///  The value in this register defines the upper boundary of the Remap window. The Remap window is inclusive of this address. In the decoder A[19:0] of the remap limit address are assumed to be F's. Thus the top of the defined range will be one byte less than a 1MB boundary.
///  When the value in this register is less than the value programmed into the Remap Base register, the Remap window is disabled.
///  These Bits are LT lockable.
///
#define N_SA_REMAPLIMIT_REMAPLMT_OFFSET  (0x14)
#define S_SA_REMAPLIMIT_REMAPLMT_WIDTH   (0x13)
#define V_SA_REMAPLIMIT_REMAPLMT_DEFAULT (0x0)
#define B_SA_REMAPLIMIT_REMAPLMT_MASK    (0x7ffff00000)

///
/// Description:
///  This Register contains the size of physical memory.  BIOS determines the memory size reported to the OS using this Register.
///
#define R_SA_TOM (0xa0)
///
/// Description of LOCK (0:0)
///  This bit will lock all writeable settings in this register, including itself.
///
#define N_SA_TOM_LOCK_OFFSET   (0x0)
#define S_SA_TOM_LOCK_WIDTH    (0x1)
#define B_SA_TOM_LOCK_MASK     (0x1)
#define V_SA_TOM_LOCK_DEFAULT  (0x0)

///
/// Description of TOM (20:38)
///  This register reflects the total amount of populated physical memory. This is NOT necessarily the highest main memory address (holes may exist in main memory address map due to addresses allocated for memory mapped IO). These bits correspond to address bits 38:20 (1MB granularity). Bits 19:0 are assumed to be 0. All the bits in this register are locked in LT mode.
///
#define N_SA_TOM_TOM_OFFSET  (0x14)
#define S_SA_TOM_TOM_WIDTH   (0x13)
#define B_SA_TOM_TOM_MASK    (0x7ffff00000)
#define V_SA_TOM_TOM_DEFAULT (0x7ffff00000)

///
/// Description:
///  This 64 bit register defines the Top of Upper Usable DRAM.
///  Configuration software must set this value to TOM minus all EP stolen memory if reclaim is disabled.  If reclaim is enabled, this value must be set to reclaim limit + 1byte, 1MB aligned, since reclaim limit is 1MB aligned. Address bits 19:0 are assumed to be 000_0000h for the purposes of address comparison. The Host interface positively decodes an address towards DRAM if the incoming address is less than the value programmed in this register and greater than or equal to 4GB.
///  BIOS Restriction: Minimum value for TOUUD is 4GB.
///  These bits are LT lockable.
///
#define R_SA_TOUUD (0xa8)
///
/// Description of LOCK (0:0)
///  This bit will lock all writeable settings in this register, including itself.
///
#define N_SA_TOUUD_LOCK_OFFSET   (0x0)
#define S_SA_TOUUD_LOCK_WIDTH    (0x1)
#define B_SA_TOUUD_LOCK_MASK     (0x1)
#define V_SA_TOUUD_LOCK_DEFAULT  (0x0)
///
/// Description of TOUUD (20:38)
///  This register contains bits 38  to 20 of an address one byte above the maximum DRAM memory above 4G that is usable by the operating system. Configuration software must set this value to TOM minus all EP stolen memory if reclaim is disabled. If reclaim is enabled, this value must be set to reclaim limit 1MB aligned since reclaim limit + 1byte is 1MB aligned. Address bits 19:0 are assumed to be 000_0000h for the purposes of address comparison. The Host interface positively decodes an address towards DRAM if the incoming address is less than the value programmed in this register and greater than 4GB.
///  All the bits in this register are locked in LT mode.
///
#define N_SA_TOUUD_TOUUD_OFFSET  (0x14)
#define S_SA_TOUUD_TOUUD_WIDTH   (0x13)
#define B_SA_TOUUD_TOUUD_MASK    (0x7ffff00000ULL)
#define V_SA_TOUUD_TOUUD_DEFAULT (0x0)

///
/// Description:
///  This register contains the base address of graphics data stolen DRAM memory. BIOS determines the base of graphics data stolen memory by subtracting the graphics data stolen memory size (PCI Device 0 offset 52 bits 7:4) from TOLUD (PCI Device 0 offset BC bits 31:20).
///
#define R_SA_BDSM  (0xb0)
///
/// Description of LOCK (0:0)
///  This bit will lock all writeable settings in this register, including itself.
///
#define N_SA_BDSM_LOCK_OFFSET  (0x0)
#define S_SA_BDSM_LOCK_WIDTH   (0x1)
#define B_SA_BDSM_LOCK_MASK    (0x1)
#define V_SA_BDSM_LOCK_DEFAULT (0x0)
///
/// Description of BDSM (20:31)
///  This register contains bits 31 to 20 of the base address of stolen DRAM memory. BIOS determines the base of graphics stolen memory by subtracting the graphics stolen memory size (PCI Device 0 offset 52 bits 6:4) from TOLUD (PCI Device 0 offset BC bits 31:20).
///
#define N_SA_BDSM_BDSM_OFFSET  (0x14)
#define S_SA_BDSM_BDSM_WIDTH   (0xc)
#define B_SA_BDSM_BDSM_MASK    (0xfff00000)
#define V_SA_BDSM_BDSM_DEFAULT (0x0)

///
/// Description:
///  This register contains the base address of stolen DRAM memory for the GTT. BIOS determines the base of GTT stolen memory by subtracting the GTT graphics stolen memory size (PCI Device 0 offset 52 bits 9:8) from the Graphics Base of  Data Stolen Memory (PCI Device 0 offset B0 bits 31:20).
///
#define R_SA_BGSM  (0xb4)
///
/// Description of LOCK (0:0)
///  This bit will lock all writeable settings in this register, including itself.
///
#define N_SA_BGSM_LOCK_OFFSET  (0x0)
#define S_SA_BGSM_LOCK_WIDTH   (0x1)
#define B_SA_BGSM_LOCK_MASK    (0x1)
#define V_SA_BGSM_LOCK_DEFAULT (0x0)
///
/// Description of BGSM (20:31)
///  This register contains the base address of stolen DRAM memory for the GTT. BIOS determines the base of GTT stolen memory by subtracting the GTT graphics stolen memory size (PCI Device 0 offset 52 bits 11:8) from the Graphics Base of  Data Stolen Memory (PCI Device 0 offset B0 bits 31:20).
///
#define N_SA_BGSM_BGSM_OFFSET  (0x14)
#define S_SA_BGSM_BGSM_WIDTH   (0xc)
#define B_SA_BGSM_BGSM_MASK    (0xfff00000)
#define V_SA_BGSM_BGSM_DEFAULT (0x0)

///
/// Description:
///  This register contains the base address of TSEG DRAM memory. BIOS determines the base of TSEG memory which must be at or below Graphics Base of GTT Stolen Memory (PCI Device 0 Offset B4 bits 31:20).
///
#define R_SA_TSEGMB  (0xb8)
///
/// Description of LOCK (0:0)
///  This bit will lock all writeable settings in this register, including itself.
///
#define N_SA_TSEGMB_LOCK_OFFSET  (0x0)
#define S_SA_TSEGMB_LOCK_WIDTH   (0x1)
#define B_SA_TSEGMB_LOCK_MASK    (0x1)
#define V_SA_TSEGMB_LOCK_DEFAULT (0x0)
///
/// Description of TSEGMB (20:31)
///  This register contains the base address of TSEG DRAM memory. BIOS determines the base of TSEG memory which must be at or below Graphics Base of GTT Stolen Memory (PCI Device 0 Offset B4 bits 31:20).
///
#define N_SA_TSEGMB_TSEGMB_OFFSET  (0x14)
#define S_SA_TSEGMB_TSEGMB_WIDTH   (0xc)
#define B_SA_TSEGMB_TSEGMB_MASK    (0xfff00000)
#define V_SA_TSEGMB_TSEGMB_DEFAULT (0x0)

///
/// Description:
///  This register contains the Top of low memory address.
///
#define R_SA_TOLUD (0xbc)
///
/// Description of LOCK (0:0)
///  This bit will lock all writeable settings in this register, including itself.
///
#define N_SA_TOLUD_LOCK_OFFSET   (0x0)
#define S_SA_TOLUD_LOCK_WIDTH    (0x1)
#define B_SA_TOLUD_LOCK_MASK     (0x1)
#define V_SA_TOLUD_LOCK_DEFAULT  (0x0)
///
/// Description of TOLUD (20:31)
///  This register contains bits 31 to 20 of an address one byte above the maximum DRAM memory below 4G that is usable by the operating system. Address bits 31 down to 20 programmed to 01h implies a minimum memory size of 1MB. Configuration software must set this value to the smaller of the following 2 choices: maximum amount memory in the system minus ME stolen memory plus one byte or the minimum address allocated for PCI memory. Address bits 19:0 are assumed to be 0_0000h for the purposes of address comparison. The Host interface positively decodes an address towards DRAM if the incoming address is less than the value programmed in this register.
///  The Top of Low Usable DRAM is the lowest address above both Graphics Stolen memory and Tseg. BIOS determines the base of Graphics Stolen Memory by subtracting the Graphics Stolen Memory Size from TOLUD and further decrements by Tseg size to determine base of Tseg. All the Bits in this register are locked in LT mode.
///  This register must be 1MB aligned when reclaim is enabled.
///
#define N_SA_TOLUD_TOLUD_OFFSET    (0x14)
#define S_SA_TOLUD_TOLUD_WIDTH     (0xc)
#define V_SA_TOLUD_TOLUD_DEFAULT   (0x100000)
#define B_SA_TOLUD_TOLUD_MASK      (0xfff00000)

#define R_SA_MC_CAPID0_A_OFFSET    0xE4

#define R_SA_MCHBAR_BIOS_RESET_CPL_OFFSET          0x5DA8
//
// Thermal Management Controls
//
///
/// Device 2 Register Equates
//
// The following equates must be reviewed and revised when the specification is ready.
//
#define SA_IGD_BUS           0x00
#define SA_IGD_DEV           0x02
#define SA_IGD_FUN_0         0x00
#define SA_IGD_FUN_1         0x01
#define SA_IGD_DEV_FUN       (SA_IGD_DEV << 3)
#define SA_IGD_BUS_DEV_FUN   (SA_MC_BUS << 8) + SA_IGD_DEV_FUN
#define V_SA_IGD_VID         0x8086
#define SA_GT_APERTURE_SIZE_256MB   1 ///< 256MB is the recommanded GT Aperture Size as per BWG.

///
/// For SKL IGD
///
#define V_SA_PCI_DEV_2_GT1_SULTM_ID    0x01906 ///< Dev2-SKL ULT GT1 (2+1F) Mobile
#define V_SA_PCI_DEV_2_GT15F_SULTM_ID  0x01913 ///< Dev2-SKL ULT GT1.5 (2+1.5F) Mobile
#define V_SA_PCI_DEV_2_GT2_SULTM_ID    0x01916 ///< Dev2-SKL ULT GT2 (2+2) Mobile
#define V_SA_PCI_DEV_2_GT2F_SULTM_ID   0x01921 ///< Dev2-SKL ULT GT2 (2+2F) Mobile
#define V_SA_PCI_DEV_2_GT3_SULTM_ID    0x01926 ///< Dev2-SKL ULT GT3 (3+3/3E) Mobile
#define V_SA_PCI_DEV_2_GT1_SHALM_ID    0x0190B ///< Dev2-SKL Halo GT1 (2+1)
#define V_SA_PCI_DEV_2_GT2_SHALM_ID    0x0191B ///< Dev2-SKL Halo GT2 (4/2+2)
#define V_SA_PCI_DEV_2_GT3_SHALM_ID    0x0192B ///< Dev2-SKL Halo GT3 (4+3FE)
#define V_SA_PCI_DEV_2_GT4_SHALM_ID    0x0193B ///< Dev2-SKL Halo GT4 (4+4E)
#define V_SA_PCI_DEV_2_GT3_SHALM_EMB_ID 0x0192D ///< Dev2-SKL Halo GT3 (4+3E) Embedded
#define V_SA_PCI_DEV_2_GT4_SHALM_EMB_ID 0x0193D ///< Dev2-SKL Halo GT4 (4+4E) Embedded
#define V_SA_PCI_DEV_2_GT1_SULXM_ID    0x0190E ///< Dev2-SKL ULX GT1(2+1F) Mobile
#define V_SA_PCI_DEV_2_GT15_SULXM_ID   0x01915 ///< Dev2-SKL ULX GT1.5(2+1.5F) Mobile
#define V_SA_PCI_DEV_2_GT2_SULXM_ID    0x0191E ///< Dev2-SKL ULX GT2 (2+2)Mobile
#define V_SA_PCI_DEV_2_GT1_SSR_ID      0x0190A ///< Dev2-SKL GT1 (4+1F) Server
#define V_SA_PCI_DEV_2_GT2_SSR_ID      0x0191A ///< Dev2-SKL GT2 (4/2+2) Server
#define V_SA_PCI_DEV_2_GT3_SSR_ID      0x0192A ///< Dev2-SKL GT3 (2+3E) Server
#define V_SA_PCI_DEV_2_GT4_SSR_ID      0x0193A ///< Dev2-SKL GT4 (4+4E) Server
#define V_SA_PCI_DEV_2_GT1_SDT_ID      0x01902 ///< Dev2-SKL GT1 (2+1F) Desktop
#define V_SA_PCI_DEV_2_GT2_SDT_ID      0x01912 ///< Dev2-SKL GT2 (4/2+2) Desktop
#define V_SA_PCI_DEV_2_GT15_SDT_ID     0x01917 ///< Dev2-SKL GT1.5 (2+1.5F) Desktop
#define V_SA_PCI_DEV_2_GT4_SDT_ID      0x01932 ///< Dev2-SKL GT4 (4+4E) Desktop
///
/// For KBL IGD
///
#define V_SA_PCI_DEV_2_GT1_KULTM_ID    0x05906 ///< Dev2-KBL ULT GT1 (2+1F) Mobile
#define V_SA_PCI_DEV_2_GT15F_KULTM_ID  0x05913 ///< Dev2-KBL ULT GT1.5 (2+1.5F) Mobile
#define V_SA_PCI_DEV_2_GT2_KULTM_ID    0x05916 ///< Dev2-KBL ULT GT2 (2+2) Mobile
#define V_SA_PCI_DEV_2_GT2F_KULTM_ID   0x05921 ///< Dev2-KBL ULT GT2 (2+2F) Mobile
#define V_SA_PCI_DEV_2_GT3_KULTM_ID    0x05926 ///< Dev2-KBL ULT GT3 (3+3/3E) Mobile
#define V_SA_PCI_DEV_2_GT3E_KULTM_ID   0x05927 ///< Dev2-KBL ULT GT3 (2+3/3E) Mobile
#define V_SA_PCI_DEV_2_GT1_KHALM_ID    0x0590B ///< Dev2-KBL Halo GT1 (2+1)
#define V_SA_PCI_DEV_2_GT2_KHALM_ID    0x0591B ///< Dev2-KBL Halo GT2 (4/2+2)
#define V_SA_PCI_DEV_2_GT3_KHALM_ID    0x0592B ///< Dev2-KBL Halo GT3 (4+3FE)
#define V_SA_PCI_DEV_2_GT4_KHALM_ID    0x0593B ///< Dev2-KBL Halo GT4 (4+4E)
#define V_SA_PCI_DEV_2_GT1_KULXM_ID    0x0590E ///< Dev2-KBL ULX GT1(2+1F) Mobile
#define V_SA_PCI_DEV_2_GT15_KULXM_ID   0x05915 ///< Dev2-KBL ULX GT1.5(2+1.5F) Mobile
#define V_SA_PCI_DEV_2_GT2_KULXM_ID    0x0591E ///< Dev2-KBL ULX GT2 (2+2)Mobile
#define V_SA_PCI_DEV_2_GT1_KSR_ID      0x0590A ///< Dev2-KBL GT1 (4+1F) Server
#define V_SA_PCI_DEV_2_GT2_KSR_ID      0x0591A ///< Dev2-KBL GT2 (4/2+2) Server
#define V_SA_PCI_DEV_2_GT3_KSR_ID      0x0592A ///< Dev2-KBL GT3 (2+3E) Server
#define V_SA_PCI_DEV_2_GT4_KSR_ID      0x0593A ///< Dev2-KBL GT4 (4+4E) Server
#define V_SA_PCI_DEV_2_GT1_KDT_ID      0x05902 ///< Dev2-KBL GT1 (2+1F) Desktop
#define V_SA_PCI_DEV_2_GT2_KDT_ID      0x05912 ///< Dev2-KBL GT2 (4/2+2) Desktop
#define V_SA_PCI_DEV_2_GT15_KDT_ID     0x05917 ///< Dev2-KBL GT1.5 (2+1.5F) Desktop
#define V_SA_PCI_DEV_2_GT4_KDT_ID      0x05932 ///< Dev2-KBL GT4 (4+4E) Desktop
#define V_SA_PCI_DEV_2_GT2_KWKS_ID     0x0591D ///< Dev2-KBL GT2 (4+2) WorkStation
#define V_SA_PCI_DEV_2_GT4_KWKS_ID     0x0593D ///< Dev2-KBL GT4 (4+4E) WorkStation
///
/// For CFL IGD
///
#define V_SA_PCI_DEV_2_GT2_CDT_ID      0x03E92 ///< Dev2-CFL GT2 (6+2) Desktop

#define R_SA_IGD_VID               0x00
#define R_SA_IGD_DID               0x02
#define R_SA_IGD_CMD               0x04

///
/// GTTMMADR for SKL is 16MB alignment (Base address = [38:24])
///
#define R_SA_IGD_GTTMMADR          0x10
#define R_SA_IGD_GMADR             0x18
#define R_SA_IGD_IOBAR             0x20
#define R_SA_IGD_BSM_OFFSET        0x005C  ///< Base of Stolen Memory
#define R_SA_IGD_MSAC_OFFSET       0x0062  ///< Multisize Aperture Control
#define R_SA_IGD_SWSCI_OFFSET      0x00E8
#define R_SA_IGD_ASLS_OFFSET       0x00FC  ///< ASL Storage
///
/// Maximum number of SDRAM channels supported by the memory controller
///
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
/// Define the SPD Address for DIMM 0
///
#define SA_MC_DIMM0_SPD_ADDRESS 0xA0

///
/// Define the maximum number of data bytes on a system with no ECC memory support.
///
#define SA_MC_MAX_BYTES_NO_ECC (8)

///
/// Define the maximum number of SPD data bytes on a DIMM.
///
#define SA_MC_MAX_SPD_SIZE (512)

///
/// Vt-d Engine base address.
///
#define R_SA_MCHBAR_VTD1_OFFSET  0x5400  ///< HW UNIT2 for IGD
#define R_SA_MCHBAR_VTD2_OFFSET  0x5410  ///< HW UNIT3 for all other - PEG, USB, SATA etc

#endif
