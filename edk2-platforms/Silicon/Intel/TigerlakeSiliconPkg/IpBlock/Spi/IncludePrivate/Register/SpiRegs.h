/** @file
  Register names for PCH SPI device.

  Conventions:

  - Register definition format:
    Prefix_[GenerationName]_[ComponentName]_SubsystemName_RegisterSpace_RegisterName
  - Prefix:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values within the bits
    Definitions beginning with "S_" are register size
    Definitions beginning with "N_" are the bit position
  - [GenerationName]:
    Three letter acronym of the generation is used (e.g. SKL,KBL,CNL etc.).
    Register name without GenerationName applies to all generations.
  - [ComponentName]:
    This field indicates the component name that the register belongs to (e.g. PCH, SA etc.)
    Register name without ComponentName applies to all components.
    Register that is specific to -LP denoted by "_PCH_LP_" in component name.
  - SubsystemName:
    This field indicates the subsystem name of the component that the register belongs to
    (e.g. PCIE, USB, SATA, GPIO, PMC etc.).
  - RegisterSpace:
    MEM - MMIO space register of subsystem.
    IO  - IO space register of subsystem.
    PCR - Private configuration register of subsystem.
    CFG - PCI configuration space register of subsystem.
  - RegisterName:
    Full register name.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _SPI_REGS_H_
#define _SPI_REGS_H_

//
// SPI Registers
//
#define R_SPI_CFG_BAR0                      0x10
#define B_SPI_CFG_BAR0_MASK                 0x0FFF

#define R_SPI_CFG_BDE                       0xD8

#define R_SPI_CFG_BC                        0xDC
#define S_SPI_CFG_BC                        4
#define N_SPI_CFG_BC_ASE_BWP                11
#define B_SPI_CFG_BC_ASE_BWP                BIT11
#define N_SPI_CFG_BC_ASYNC_SS               10
#define B_SPI_CFG_BC_ASYNC_SS               BIT10
#define N_SPI_CFG_BC_SYNC_SS                8
#define B_SPI_CFG_BC_SYNC_SS                BIT8
#define B_SPI_CFG_BC_BILD                   BIT7
#define B_SPI_CFG_BC_BBS                    BIT6            ///< Boot BIOS strap
#define N_SPI_CFG_BC_BBS                    6
#define V_SPI_CFG_BC_BBS_SPI                0               ///< Boot BIOS strapped to SPI
#define B_SPI_CFG_BC_EISS                   BIT5            ///< Enable InSMM.STS
#define B_SPI_CFG_BC_TSS                    BIT4
#define B_SPI_CFG_BC_SRC                    (BIT3 | BIT2)
#define N_SPI_CFG_BC_SRC                    2
#define V_SPI_CFG_BC_SRC_PREF_DIS_CACHE_DIS 0x01            ///< No prefetching and no caching
#define B_SPI_CFG_BC_LE                     BIT1            ///< Lock Enable
#define N_SPI_CFG_BC_BLE                    1
#define B_SPI_CFG_BC_WPD                    BIT0            ///< Write Protect Disable

//
// BIOS Flash Program Registers (based on SPI_BAR0)
//
#define R_SPI_MEM_HSFSC                     0x04                          ///< Hardware Sequencing Flash Status and Control Register(32bits)
#define B_SPI_MEM_HSFSC_FDBC_MASK           0x3F000000                    ///< Flash Data Byte Count ( <= 64), Count = (Value in this field) + 1.
#define N_SPI_MEM_HSFSC_FDBC                24
#define B_SPI_MEM_HSFSC_CYCLE_MASK          0x001E0000                    ///< Flash Cycle.
#define N_SPI_MEM_HSFSC_CYCLE               17
#define V_SPI_MEM_HSFSC_CYCLE_READ          0                             ///< Flash Cycle Read
#define V_SPI_MEM_HSFSC_CYCLE_WRITE         2                             ///< Flash Cycle Write
#define V_SPI_MEM_HSFSC_CYCLE_4K_ERASE      3                             ///< Flash Cycle 4K Block Erase
#define V_SPI_MEM_HSFSC_CYCLE_64K_ERASE     4                             ///< Flash Cycle 64K Sector Erase
#define V_SPI_MEM_HSFSC_CYCLE_READ_SFDP     5                             ///< Flash Cycle Read SFDP
#define V_SPI_MEM_HSFSC_CYCLE_READ_JEDEC_ID 6                             ///< Flash Cycle Read JEDEC ID
#define V_SPI_MEM_HSFSC_CYCLE_WRITE_STATUS  7                             ///< Flash Cycle Write Status
#define V_SPI_MEM_HSFSC_CYCLE_READ_STATUS   8                             ///< Flash Cycle Read Status
#define B_SPI_MEM_HSFSC_CYCLE_FGO           BIT16                         ///< Flash Cycle Go.
#define B_SPI_MEM_HSFSC_FLOCKDN             BIT15                         ///< Flash Configuration Lock-Down
#define B_SPI_MEM_HSFSC_FDV                 BIT14                         ///< Flash Descriptor Valid, once valid software can use hareware sequencing regs
#define B_SPI_MEM_HSFSC_FDOPSS              BIT13                         ///< Flash Descriptor Override Pin-Strap Status
#define B_SPI_MEM_HSFSC_WRSDIS              BIT11                         ///< Write Status Disable
#define B_SPI_MEM_HSFSC_SAF_MODE_ACTIVE     BIT7                          ///< Indicates flash is attached either directly to the PCH via the SPI bus or EC/BMC
#define B_SPI_MEM_HSFSC_SCIP                BIT5                          ///< SPI cycle in progress
#define B_SPI_MEM_HSFSC_SAF_DLE             BIT4                          ///< SAF Data length error
#define B_SPI_MEM_HSFSC_SAF_ERROR           BIT3                          ///< SAF Error
#define B_SPI_MEM_HSFSC_AEL                 BIT2                          ///< Access Error Log
#define B_SPI_MEM_HSFSC_FCERR               BIT1                          ///< Flash Cycle Error
#define B_SPI_MEM_HSFSC_FDONE               BIT0                          ///< Flash Cycle Done
#define R_SPI_MEM_FADDR                     0x08                          ///< SPI Flash Address
#define B_SPI_MEM_FADDR_MASK                0x07FFFFFF                    ///< SPI Flash Address Mask (0~26bit)
#define R_SPI_MEM_DLOCK                     0x0C                          ///< Discrete Lock Bits
#define B_SPI_MEM_DLOCK_PR0LOCKDN           BIT8                          ///< PR0LOCKDN
#define R_SPI_MEM_FDATA00                   0x10                          ///< SPI Data 00 (32 bits)
#define R_SPI_MEM_FRAP                      0x50                          ///< Flash Region Access Permissions Register
#define B_SPI_MEM_FRAP_BRWA_MASK            0x0000FF00                    ///< BIOS Region Write Access MASK, Region0~7 - 0: Flash Descriptor; 1: BIOS; 2: ME; 3: GbE; 4: PlatformData
#define N_SPI_MEM_FRAP_BRWA                 8                             ///< BIOS Region Write Access bit position
#define B_SPI_MEM_FRAP_BRRA_MASK            0x000000FF                    ///< BIOS Region Read Access MASK, Region0~7 - 0: Flash Descriptor; 1: BIOS; 2: ME; 3: GbE; 4: PlatformData
#define B_SPI_MEM_FRAP_BMRAG_MASK           0x00FF0000                    ///< BIOS Master Read Access Grant
#define B_SPI_MEM_FRAP_BMWAG_MASK           0xFF000000                    ///< BIOS Master Write Access Grant
#define R_SPI_MEM_FREG0_FLASHD              0x54                          ///< Flash Region 0(Flash Descriptor)(32bits)
#define R_SPI_MEM_FREG3_GBE                 0x60                          ///< Flash Region 3(GbE)(32bits)
#define S_SPI_MEM_FREGX                     4                             ///< Size of Flash Region register
#define B_SPI_MEM_FREGX_LIMIT_MASK          0x7FFF0000                    ///< Flash Region Limit [30:16] represents [26:12], [11:0] are assumed to be FFFh
#define N_SPI_MEM_FREGX_LIMIT               16                            ///< Region limit bit position
#define N_SPI_MEM_FREGX_LIMIT_REPR          12                            ///< Region limit bit represents position
#define B_SPI_MEM_FREGX_BASE_MASK           0x00007FFF                    ///< Flash Region Base, [14:0] represents [26:12]
#define N_SPI_MEM_FREGX_BASE                0                             ///< Region base bit position
#define N_SPI_MEM_FREGX_BASE_REPR           12                            ///< Region base bit represents position
#define R_SPI_MEM_PR0                       0x84                          ///< Protected Region 0 Register
#define S_SPI_MEM_PRX                       4                             ///< Protected Region X Register size
#define B_SPI_MEM_PRX_WPE                   BIT31                         ///< Write Protection Enable
#define B_SPI_MEM_PRX_PRL_MASK              0x7FFF0000                    ///< Protected Range Limit Mask, [30:16] here represents upper limit of address [26:12]
#define N_SPI_MEM_PRX_PRL                   16                            ///< Protected Range Limit bit position
#define B_SPI_MEM_PRX_RPE                   BIT15                         ///< Read Protection Enable
#define B_SPI_MEM_PRX_PRB_MASK              0x00007FFF                    ///< Protected Range Base Mask, [14:0] here represents base limit of address [26:12]
#define N_SPI_MEM_PRX_PRB                   0                             ///< Protected Range Base bit position
#define R_SPI_MEM_FDOC                      0xB4                          ///< Flash Descriptor Observability Control Register(32 bits)
#define B_SPI_MEM_FDOC_FDSS_MASK            (BIT14 | BIT13 | BIT12)       ///< Flash Descritor Section Select
#define V_SPI_MEM_FDOC_FDSS_FSDM            0x0000                        ///< Flash Signature and Descriptor Map
#define V_SPI_MEM_FDOC_FDSS_COMP            0x1000                        ///< Component
#define V_SPI_MEM_FDOC_FDSS_MSTR            0x3000                        ///< Master
#define B_SPI_MEM_FDOC_FDSI_MASK            0x0FFC                        ///< Flash Descriptor Section Index
#define R_SPI_MEM_FDOD                      0xB8                          ///< Flash Descriptor Observability Data Register(32 bits)
#define R_SPI_MEM_SFDP0_VSCC0               0xC4                          ///< Vendor Specific Component Capabilities Register(32 bits)
#define B_SPI_MEM_SFDPX_VSCCX_CPPTV         BIT31                         ///< Component Property Parameter Table Valid
#define B_SPI_MEM_SFDP0_VSCC0_VCL           BIT30                         ///< Vendor Component Lock
#define B_SPI_MEM_SFDPX_VSCCX_EO_64K        BIT29                         ///< 64k Erase valid (EO_64k_valid)
#define R_SPI_MEM_SFDP1_VSCC1               0xC8                          ///< Vendor Specific Component Capabilities Register(32 bits)

#endif
