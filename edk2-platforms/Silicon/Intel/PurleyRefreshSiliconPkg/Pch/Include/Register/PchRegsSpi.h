/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PCH_REGS_SPI_H_
#define _PCH_REGS_SPI_H_

//
// SPI Registers (D31:F5)
//

#define PCI_DEVICE_NUMBER_PCH_SPI           31
#define PCI_FUNCTION_NUMBER_PCH_SPI         5
#define V_PCH_SPI_VENDOR_ID                 V_PCH_INTEL_VENDOR_ID
#define V_PCH_H_SPI_DEVICE_ID               0xA124
//
// LBG PRODUCTION SPI Device ID
//
#define V_PCH_LBG_PROD_SPI_DEVICE_ID        0xA1A4
//
// LBG SSX (Super SKU) SPI Device ID
//
#define V_PCH_LBG_SPI_DEVICE_ID             0xA224
#define V_PCH_LP_SPI_DEVICE_ID              0x9D24
#define R_PCH_SPI_BAR0                      0x10
#define B_PCH_SPI_BAR0_MASK                 0x0FFF

#define R_PCH_SPI_BDE                       0xD8
#define B_PCH_SPI_BDE_F8                    0x8000
#define B_PCH_SPI_BDE_F0                    0x4000
#define B_PCH_SPI_BDE_E8                    0x2000
#define B_PCH_SPI_BDE_E0                    0x1000
#define B_PCH_SPI_BDE_D8                    0x0800
#define B_PCH_SPI_BDE_D0                    0x0400
#define B_PCH_SPI_BDE_C8                    0x0200
#define B_PCH_SPI_BDE_C0                    0x0100
#define B_PCH_SPI_BDE_LEG_F                 0x0080
#define B_PCH_SPI_BDE_LEG_E                 0x0040
#define B_PCH_SPI_BDE_70                    0x0008
#define B_PCH_SPI_BDE_60                    0x0004
#define B_PCH_SPI_BDE_50                    0x0002
#define B_PCH_SPI_BDE_40                    0x0001

#define R_PCH_SPI_BC                        0xDC
#define S_PCH_SPI_BC                        4
#define N_PCH_SPI_BC_ASE_BWP                11
#define B_PCH_SPI_BC_ASE_BWP                BIT11
#define N_PCH_SPI_BC_ASYNC_SS               10
#define B_PCH_SPI_BC_ASYNC_SS               BIT10
#define B_PCH_SPI_BC_OSFH                   BIT9            ///< OS Function Hide
#define N_PCH_SPI_BC_SYNC_SS                8
#define B_PCH_SPI_BC_SYNC_SS                BIT8
#define B_PCH_SPI_BC_BILD                   BIT7
#define B_PCH_SPI_BC_BBS                    BIT6            ///< Boot BIOS strap
#define N_PCH_SPI_BC_BBS                    6
#define V_PCH_SPI_BC_BBS_SPI                0               ///< Boot BIOS strapped to SPI
#define V_PCH_SPI_BC_BBS_LPC                1               ///< Boot BIOS strapped to LPC
#define B_PCH_SPI_BC_EISS                   BIT5            ///< Enable InSMM.STS
#define B_PCH_SPI_BC_TSS                    BIT4
#define B_PCH_SPI_BC_SRC                    (BIT3 | BIT2)
#define N_PCH_SPI_BC_SRC                    2
#define V_PCH_SPI_BC_SRC_PREF_EN_CACHE_EN   0x02            ///< Prefetching and Caching enabled
#define V_PCH_SPI_BC_SRC_PREF_DIS_CACHE_DIS 0x01            ///< No prefetching and no caching
#define V_PCH_SPI_BC_SRC_PREF_DIS_CACHE_EN  0x00            ///< No prefetching, but caching enabled
#define B_PCH_SPI_BC_LE                     BIT1            ///< Lock Enable
#define N_PCH_SPI_BC_BLE                    1
#define B_PCH_SPI_BC_WPD                    BIT0            ///< Write Protect Disable

//
// BIOS Flash Program Registers (based on SPI_BAR0)
//
#define R_PCH_SPI_BFPR                      0x00                          ///< BIOS Flash Primary Region Register(32bits), which is RO and contains the same value from FREG1
#define B_PCH_SPI_BFPR_PRL                  0x7FFF0000                    ///< BIOS Flash Primary Region Limit mask
#define N_PCH_SPI_BFPR_PRL                  16                            ///< BIOS Flash Primary Region Limit bit position
#define B_PCH_SPI_BFPR_PRB                  0x00007FFF                    ///< BIOS Flash Primary Region Base mask
#define N_PCH_SPI_BFPR_PRB                  0                             ///< BIOS Flash Primary Region Base bit position
#define R_PCH_SPI_HSFSC                     0x04                          ///< Hardware Sequencing Flash Status and Control Register(32bits)
#define B_PCH_SPI_HSFSC_FSMIE               BIT31                         ///< Flash SPI SMI# Enable
#define B_PCH_SPI_HSFSC_FDBC_MASK           0x3F000000                    ///< Flash Data Byte Count ( <= 64), Count = (Value in this field) + 1.
#define N_PCH_SPI_HSFSC_FDBC                24
#define B_PCH_SPI_HSFSC_CYCLE_MASK          0x001E0000                    ///< Flash Cycle.
#define N_PCH_SPI_HSFSC_CYCLE               17
#define V_PCH_SPI_HSFSC_CYCLE_READ          0                             ///< Flash Cycle Read
#define V_PCH_SPI_HSFSC_CYCLE_WRITE         2                             ///< Flash Cycle Write
#define V_PCH_SPI_HSFSC_CYCLE_4K_ERASE      3                             ///< Flash Cycle 4K Block Erase
#define V_PCH_SPI_HSFSC_CYCLE_64K_ERASE     4                             ///< Flash Cycle 64K Sector Erase
#define V_PCH_SPI_HSFSC_CYCLE_READ_SFDP     5                             ///< Flash Cycle Read SFDP
#define V_PCH_SPI_HSFSC_CYCLE_READ_JEDEC_ID 6                             ///< Flash Cycle Read JEDEC ID
#define V_PCH_SPI_HSFSC_CYCLE_WRITE_STATUS  7                             ///< Flash Cycle Write Status
#define V_PCH_SPI_HSFSC_CYCLE_READ_STATUS   8                             ///< Flash Cycle Read Status
#define B_PCH_SPI_HSFSC_CYCLE_FGO           BIT16                         ///< Flash Cycle Go.
#define B_PCH_SPI_HSFSC_FLOCKDN             BIT15                         ///< Flash Configuration Lock-Down
#define B_PCH_SPI_HSFSC_FDV                 BIT14                         ///< Flash Descriptor Valid, once valid software can use hareware sequencing regs
#define B_PCH_SPI_HSFSC_FDOPSS              BIT13                         ///< Flash Descriptor Override Pin-Strap Status
#define B_PCH_SPI_HSFSC_PRR34_LOCKDN        BIT12                         ///< PRR3 PRR4 Lock-Down
#define B_PCH_SPI_HSFSC_SAF_CE              BIT8                          ///< SAF ctype error
#define B_PCH_SPI_HSFSC_SAF_MODE_ACTIVE     BIT7                          ///< Indicates flash is attached either directly to the PCH via the SPI bus or EC/BMC
#define B_PCH_SPI_HSFSC_SAF_LE              BIT6                          ///< SAF link error
#define B_PCH_SPI_HSFSC_SCIP                BIT5                          ///< SPI cycle in progress
#define B_PCH_SPI_HSFSC_SAF_DLE             BIT4                          ///< SAF Data length error
#define B_PCH_SPI_HSFSC_SAF_ERROR           BIT3                          ///< SAF Error
#define B_PCH_SPI_HSFSC_AEL                 BIT2                          ///< Access Error Log
#define B_PCH_SPI_HSFSC_FCERR               BIT1                          ///< Flash Cycle Error
#define B_PCH_SPI_HSFSC_FDONE               BIT0                          ///< Flash Cycle Done
#define R_PCH_SPI_FADDR                     0x08                          ///< SPI Flash Address
#define B_PCH_SPI_FADDR_MASK                0x07FFFFFF                    ///< SPI Flash Address Mask (0~26bit)
#define R_PCH_SPI_DLOCK                     0x0C                          ///< Discrete Lock Bits
#define B_PCH_SPI_DLOCK_PR0LOCKDN           BIT8                          ///< PR0LOCKDN
#define R_PCH_SPI_FDATA00                   0x10                          ///< SPI Data 00 (32 bits)
#define R_PCH_SPI_FDATA01                   0x14                          ///< SPI Data 01
#define R_PCH_SPI_FDATA02                   0x18                          ///< SPI Data 02
#define R_PCH_SPI_FDATA03                   0x1C                          ///< SPI Data 03
#define R_PCH_SPI_FDATA04                   0x20                          ///< SPI Data 04
#define R_PCH_SPI_FDATA05                   0x24                          ///< SPI Data 05
#define R_PCH_SPI_FDATA06                   0x28                          ///< SPI Data 06
#define R_PCH_SPI_FDATA07                   0x2C                          ///< SPI Data 07
#define R_PCH_SPI_FDATA08                   0x30                          ///< SPI Data 08
#define R_PCH_SPI_FDATA09                   0x34                          ///< SPI Data 09
#define R_PCH_SPI_FDATA10                   0x38                          ///< SPI Data 10
#define R_PCH_SPI_FDATA11                   0x3C                          ///< SPI Data 11
#define R_PCH_SPI_FDATA12                   0x40                          ///< SPI Data 12
#define R_PCH_SPI_FDATA13                   0x44                          ///< SPI Data 13
#define R_PCH_SPI_FDATA14                   0x48                          ///< SPI Data 14
#define R_PCH_SPI_FDATA15                   0x4C                          ///< SPI Data 15
#define R_PCH_SPI_FRAP                      0x50                          ///< Flash Region Access Permisions Register
#define B_PCH_SPI_FRAP_BRWA_MASK            0x0000FF00                    ///< BIOS Region Write Access MASK, Region0~7 - 0: Flash Descriptor; 1: BIOS; 2: ME; 3: GbE; 4: PlatformData
#define N_PCH_SPI_FRAP_BRWA                 8                             ///< BIOS Region Write Access bit position
#define B_PCH_SPI_FRAP_BRRA_MASK            0x000000FF                    ///< BIOS Region Read Access MASK, Region0~7 - 0: Flash Descriptor; 1: BIOS; 2: ME; 3: GbE; 4: PlatformData
#define B_PCH_SPI_FRAP_BMRAG_MASK           0x00FF0000                    ///< BIOS Master Read Access Grant
#define B_PCH_SPI_FRAP_BMWAG_MASK           0xFF000000                    ///< BIOS Master Write Access Grant
#define R_PCH_SPI_FREG0_FLASHD              0x54                          ///< Flash Region 0(Flash Descriptor)(32bits)
#define R_PCH_SPI_FREG1_BIOS                0x58                          ///< Flash Region 1(BIOS)(32bits)
#define R_PCH_SPI_FREG2_ME                  0x5C                          ///< Flash Region 2(ME)(32bits)
#define R_PCH_SPI_FREG3_GBE                 0x60                          ///< Flash Region 3(GbE)(32bits)
#define R_PCH_SPI_FREG4_PLATFORM_DATA       0x64                          ///< Flash Region 4(Platform Data)(32bits)
#define R_PCH_SPI_FREG5_DER                 0x68                          ///< Flash Region 5(Device Expansion Region)(32bits)
#define S_PCH_SPI_FREGX                     4                             ///< Size of Flash Region register
#define B_PCH_SPI_FREGX_LIMIT_MASK          0x7FFF0000                    ///< Flash Region Limit [30:16] represents [26:12], [11:0] are assumed to be FFFh
#define N_PCH_SPI_FREGX_LIMIT               16                            ///< Region limit bit position
#define N_PCH_SPI_FREGX_LIMIT_REPR          12                            ///< Region limit bit represents position
#define B_PCH_SPI_FREGX_BASE_MASK           0x00007FFF                    ///< Flash Region Base, [14:0] represents [26:12]
#define N_PCH_SPI_FREGX_BASE                0                             ///< Region base bit position
#define N_PCH_SPI_FREGX_BASE_REPR           12                            ///< Region base bit represents position
#define R_PCH_SPI_PR0                       0x84                          ///< Protected Region 0 Register
#define R_PCH_SPI_PR1                       0x88                          ///< Protected Region 1 Register
#define R_PCH_SPI_PR2                       0x8C                          ///< Protected Region 2 Register
#define R_PCH_SPI_PR3                       0x90                          ///< Protected Region 3 Register
#define R_PCH_SPI_PR4                       0x94                          ///< Protected Region 4 Register
#define S_PCH_SPI_PRX                       4                             ///< Protected Region X Register size
#define B_PCH_SPI_PRX_WPE                   BIT31                         ///< Write Protection Enable
#define B_PCH_SPI_PRX_PRL_MASK              0x7FFF0000                    ///< Protected Range Limit Mask, [30:16] here represents upper limit of address [26:12]
#define N_PCH_SPI_PRX_PRL                   16                            ///< Protected Range Limit bit position
#define B_PCH_SPI_PRX_RPE                   BIT15                         ///< Read Protection Enable
#define B_PCH_SPI_PRX_PRB_MASK              0x00007FFF                    ///< Protected Range Base Mask, [14:0] here represents base limit of address [26:12]
#define N_PCH_SPI_PRX_PRB                   0                             ///< Protected Range Base bit position
#define R_PCH_SPI_SFRAP                     0xB0                          ///< Secondary Flash Regions Access Permisions Register
#define R_PCH_SPI_FDOC                      0xB4                          ///< Flash Descriptor Observability Control Register(32 bits)
#define B_PCH_SPI_FDOC_FDSS_MASK            (BIT14 | BIT13 | BIT12)       ///< Flash Descritor Section Select
#define V_PCH_SPI_FDOC_FDSS_FSDM            0x0000                        ///< Flash Signature and Descriptor Map
#define V_PCH_SPI_FDOC_FDSS_COMP            0x1000                        ///< Component
#define V_PCH_SPI_FDOC_FDSS_REGN            0x2000                        ///< Region
#define V_PCH_SPI_FDOC_FDSS_MSTR            0x3000                        ///< Master
#define V_PCH_SPI_FDOC_FDSS_PCHS            0x4000                        ///< PCH soft straps
#define V_PCH_SPI_FDOC_FDSS_SFDP            0x5000                        ///< SFDP Parameter Table
#define B_PCH_SPI_FDOC_FDSI_MASK            0x0FFC                        ///< Flash Descriptor Section Index
#define R_PCH_SPI_FDOD                      0xB8                          ///< Flash Descriptor Observability Data Register(32 bits)
#define R_PCH_SPI_SFDP0_VSCC0               0xC4                          ///< Vendor Specific Component Capabilities Register(32 bits)
#define B_PCH_SPI_SFDPX_VSCCX_CPPTV         BIT31                         ///< Component Property Parameter Table Valid
#define B_PCH_SPI_SFDP0_VSCC0_VCL           BIT30                         ///< Vendor Component Lock
#define B_PCH_SPI_SFDPX_VSCCX_EO_64K        BIT29                         ///< 64k Erase valid (EO_64k_valid)
#define B_PCH_SPI_SFDPX_VSCCX_EO_4K         BIT28                         ///< 4k Erase valid (EO_4k_valid)
#define B_PCH_SPI_SFDPX_VSCCX_RPMC          BIT27                         ///< RPMC Supported
#define B_PCH_SPI_SFDPX_VSCCX_DPD           BIT26                         ///< Deep Powerdown Supported
#define B_PCH_SPI_SFDPX_VSCCX_SUSRES        BIT25                         ///< Suspend/Resume Supported
#define B_PCH_SPI_SFDPX_VSCCX_SOFTRES       BIT24                         ///< Soft Reset Supported
#define B_PCH_SPI_SFDPX_VSCCX_64k_EO_MASK   0x00FF0000                    ///< 64k Erase Opcode (EO_64k)
#define B_PCH_SPI_SFDPX_VSCCX_4k_EO_MASK    0x0000FF00                    ///< 4k Erase Opcode (EO_4k)
#define B_PCH_SPI_SFDPX_VSCCX_QER           (BIT7 | BIT6 | BIT5)          ///< Quad Enable Requirements
#define B_PCH_SPI_SFDPX_VSCCX_WEWS          BIT4                          ///< Write Enable on Write Status
#define B_PCH_SPI_SFDPX_VSCCX_WSR           BIT3                          ///< Write Status Required
#define B_PCH_SPI_SFDPX_VSCCX_WG_64B        BIT2                          ///< Write Granularity, 0: 1 Byte; 1: 64 Bytes
#define R_PCH_SPI_SFDP1_VSCC1               0xC8                          ///< Vendor Specific Component Capabilities Register(32 bits)
#define R_PCH_SPI_PINTX                     0xCC                          ///< Parameter Table Index
#define N_PCH_SPI_PINTX_SPT                 14
#define V_PCH_SPI_PINTX_SPT_CPT0            0x0                           ///< Component 0 Property Parameter Table
#define V_PCH_SPI_PINTX_SPT_CPT1            0x1                           ///< Component 1 Property Parameter Table
#define N_PCH_SPI_PINTX_HORD                12
#define V_PCH_SPI_PINTX_HORD_SFDP           0x0                           ///< SFDP Header
#define V_PCH_SPI_PINTX_HORD_PT             0x1                           ///< Parameter Table Header
#define V_PCH_SPI_PINTX_HORD_DATA           0x2                           ///< Data
#define R_PCH_SPI_PTDATA                    0xD0                          ///< Parameter Table Data
#define R_PCH_SPI_SBRS                      0xD4                          ///< SPI Bus Requester Status
#define R_PCH_SPI_SSML                      0xF0                          ///< Set Strap Msg Lock
#define B_PCH_SPI_SSML_SSL                  BIT0                          ///< Set_Strap Lock
#define R_PCH_SPI_SSMC                      0xF4                          ///< Set Strap Msg Control
#define B_PCH_SPI_SSMC_SSMS                 BIT0                          ///< Set_Strap Mux Select
#define R_PCH_SPI_SSMD                      0xF8                          ///< Set Strap Msg Data
//
// @todo Follow up with EDS owner if it should be 3FFF or FFFF.
//
#define B_PCH_SPI_SRD_SSD                   0x0000FFFF                    ///< Set_Strap Data
//
// Flash Descriptor Base Address Region (FDBAR) from Flash Region 0
//
#define R_PCH_SPI_FDBAR_FLVALSIG            0x00                          ///< Flash Valid Signature
#define V_PCH_SPI_FDBAR_FLVALSIG            0x0FF0A55A
#define R_PCH_SPI_FDBAR_FLASH_MAP0          0x04
#define B_PCH_SPI_FDBAR_FCBA                0x000000FF                    ///< Flash Component Base Address
#define B_PCH_SPI_FDBAR_NC                  0x00000300                    ///< Number Of Components
#define N_PCH_SPI_FDBAR_NC                  8                             ///< Number Of Components
#define V_PCH_SPI_FDBAR_NC_1                0x00000000
#define V_PCH_SPI_FDBAR_NC_2                0x00000100
#define B_PCH_SPI_FDBAR_FRBA                0x00FF0000                    ///< Flash Region Base Address
#define B_PCH_SPI_FDBAR_NR                  0x07000000                    ///< Number Of Regions
#define R_PCH_SPI_FDBAR_FLASH_MAP1          0x08
#define B_PCH_SPI_FDBAR_FMBA                0x000000FF                    ///< Flash Master Base Address
#define B_PCH_SPI_FDBAR_NM                  0x00000700                    ///< Number Of Masters
#define B_PCH_SPI_FDBAR_FPSBA               0x00FF0000                    ///< PCH Strap Base Address, [23:16] represents [11:4]
#define N_PCH_SPI_FDBAR_FPSBA               16                            ///< PCH Strap base Address bit position
#define N_PCH_SPI_FDBAR_FPSBA_REPR          4                             ///< PCH Strap base Address bit represents position
#define B_PCH_SPI_FDBAR_PCHSL               0xFF000000                    ///< PCH Strap Length, [31:24] represents number of Dwords
#define N_PCH_SPI_FDBAR_PCHSL               24                            ///< PCH Strap Length bit position
#define R_PCH_SPI_FDBAR_FLASH_MAP2          0x0C
#define B_PCH_SPI_FDBAR_FCPUSBA             0x000000FF                    ///< CPU Strap Base Address, [7:0] represents [11:4]
#define N_PCH_SPI_FDBAR_FCPUSBA             0                             ///< CPU Strap Base Address bit position
#define N_PCH_SPI_FDBAR_FCPUSBA_REPR        4                             ///< CPU Strap Base Address bit represents position
#define B_PCH_SPI_FDBAR_CPUSL               0x0000FF00                    ///< CPU Strap Length, [15:8] represents number of Dwords
#define N_PCH_SPI_FDBAR_CPUSL               8                             ///< CPU Strap Length bit position
//
// Flash Component Base Address (FCBA) from Flash Region 0
//
#define R_PCH_SPI_FCBA_FLCOMP               0x00                          ///< Flash Components Register
#define B_PCH_SPI_FLCOMP_RIDS_FREQ          (BIT29 | BIT28 | BIT27)       ///< Read ID and Read Status Clock Frequency
#define B_PCH_SPI_FLCOMP_WE_FREQ            (BIT26 | BIT25 | BIT24)       ///< Write and Erase Clock Frequency
#define B_PCH_SPI_FLCOMP_FRCF_FREQ          (BIT23 | BIT22 | BIT21)       ///< Fast Read Clock Frequency
#define B_PCH_SPI_FLCOMP_FR_SUP             BIT20                         ///< Fast Read Support.
#define B_PCH_SPI_FLCOMP_RC_FREQ            (BIT19 | BIT18 | BIT17)       ///< Read Clock Frequency.
#define V_PCH_SPI_FLCOMP_FREQ_48MHZ         0x02
#define V_PCH_SPI_FLCOMP_FREQ_30MHZ         0x04
#define V_PCH_SPI_FLCOMP_FREQ_17MHZ         0x06
#define B_PCH_SPI_FLCOMP_COMP1_MASK         0xF0                          ///< Flash Component 1 Size MASK
#define N_PCH_SPI_FLCOMP_COMP1              4                             ///< Flash Component 1 Size bit position
#define B_PCH_SPI_FLCOMP_COMP0_MASK         0x0F                          ///< Flash Component 0 Size MASK
#define V_PCH_SPI_FLCOMP_COMP_512KB         0x80000
//
// Descriptor Upper Map Section from Flash Region 0
//
#define R_PCH_SPI_FLASH_UMAP1               0xEFC                         ///< Flash Upper Map 1
#define B_PCH_SPI_FLASH_UMAP1_VTBA          0x000000FF                    ///< VSCC Table Base Address
#define B_PCH_SPI_FLASH_UMAP1_VTL           0x0000FF00                    ///< VSCC Table Length

#define R_PCH_SPI_VTBA_JID0                 0x00                          ///< JEDEC-ID 0 Register
#define S_PCH_SPI_VTBA_JID0                 0x04
#define B_PCH_SPI_VTBA_JID0_VID             0x000000FF
#define B_PCH_SPI_VTBA_JID0_DID0            0x0000FF00
#define B_PCH_SPI_VTBA_JID0_DID1            0x00FF0000
#define N_PCH_SPI_VTBA_JID0_DID0            0x08
#define N_PCH_SPI_VTBA_JID0_DID1            0x10
#define R_PCH_SPI_VTBA_VSCC0                0x04
#define S_PCH_SPI_VTBA_VSCC0                0x04


//
// SPI Private Configuration Space Registers
//
#define R_PCH_PCR_SPI_CLK_CTL               0xC004
#define R_PCH_PCR_SPI_PWR_CTL               0xC008

//
// MMP0
//
#define R_PCH_SPI_STRP_MMP0                 0xC4    ///< MMP0 Soft strap offset
#define B_PCH_SPI_STRP_MMP0                 0x10    ///< MMP0 Soft strap bit


#define R_PCH_SPI_STRP_SFDP                 0xF0    ///< PCH Soft Strap SFDP
#define B_PCH_SPI_STRP_SFDP_QIORE           BIT3                          ///< Quad IO Read Enable
#define B_PCH_SPI_STRP_SFDP_QORE            BIT2                          ///< Quad Output Read Enable
#define B_PCH_SPI_STRP_SFDP_DIORE           BIT1                          ///< Dual IO Read Enable
#define B_PCH_SPI_STRP_SFDP_DORE            BIT0                          ///< Dual Output Read Enable

//
// Descriptor Record 0
//
#define R_PCH_SPI_STRP_DSCR_0               0x00    ///< PCH Soft Strap 0
#define B_PCH_SPI_STRP_DSCR_0_PTT_SUPP      BIT22   ///< PTT Supported

#endif
