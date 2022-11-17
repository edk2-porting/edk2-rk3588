/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PCH_REGS_DMI_H_
#define _PCH_REGS_DMI_H_

//
// DMI Chipset Configuration Registers (PID:DMI)
//

//
// VC Configuration (Common)
//
#define R_PCH_PCR_DMI_V0CTL                  0x2014                      ///< Virtual channel 0 resource control
#define B_PCH_PCR_DMI_V0CTL_EN               BIT31
#define B_PCH_PCR_DMI_V0CTL_ID               (7 << 24)                   ///< Bit[26:24]
#define N_PCH_PCR_DMI_V0CTL_ID               24
#define V_PCH_PCR_DMI_V0CTL_ETVM_MASK        0xFC00
#define V_PCH_PCR_DMI_V0CTL_TVM_MASK         0x7E
#define R_PCH_PCR_DMI_V0STS                  0x201A                      ///< Virtual channel 0 status
#define B_PCH_PCR_DMI_V0STS_NP               BIT1
#define R_PCH_PCR_DMI_V1CTL                  0x2020                      ///< Virtual channel 1 resource control
#define B_PCH_PCR_DMI_V1CTL_EN               BIT31
#define B_PCH_PCR_DMI_V1CTL_ID               (0x0F << 24)                ///< Bit[27:24]
#define N_PCH_PCR_DMI_V1CTL_ID               24
#define V_PCH_PCR_DMI_V1CTL_ETVM_MASK        0xFC00
#define V_PCH_PCR_DMI_V1CTL_TVM_MASK         0xFE
#define R_PCH_PCR_DMI_V1STS                  0x2026                      ///< Virtual channel 1 status
#define B_PCH_PCR_DMI_V1STS_NP               BIT1
#define R_PCH_PCR_DMI_VMCTL                  0x2040                      ///< ME Virtual Channel (VCm) resource control
#define R_PCH_PCR_DMI_VMSTS                  0x2046                      ///< ME Virtual Channel Resource Status
#define R_PCH_PCR_DMI_UEM                    0x2088                      ///< Uncorrectable Error Mask
#define R_PCH_PCR_DMI_REC                    0x20AC                      ///< Root Error Command

//
// Internal Link Configuration (DMI Only)
//
#define R_PCH_PCR_DMI_LCAP                   0x21A4                      ///< Link Capabilities
#define B_PCH_PCR_DMI_LCAP_EL1               (BIT17 | BIT16 | BIT15)
#define B_PCH_PCR_DMI_LCAP_EL0               (BIT14 | BIT13 | BIT12)
#define B_PCH_PCR_DMI_LCAP_APMS              (BIT11 | BIT10)             ///< L0 is supported on DMI
#define B_PCH_PCR_DMI_LCAP_MLW               0x000003F0
#define B_PCH_PCR_DMI_LCAP_MLS               0x0000000F
#define R_PCH_PCR_DMI_LCTL                   0x21A8                      ///< Link Control
#define B_PCH_PCR_DMI_LCTL_ES                BIT7
#define B_PCH_PCR_DMI_LCTL_ASPM              (BIT1 | BIT0)               ///< Link ASPM
#define R_PCH_PCR_DMI_LSTS                   0x21AA                      ///< Link Status
#define R_PCH_PCR_DMI_LCTL2                  0x21B0                      ///< Link Control 2
#define R_PCH_PCR_DMI_LSTS2                  0x21B2                      ///< Link Status 2
#define R_PCH_PCR_DMI_L01EC                  0x21BC                      ///< Lane 0 and Lane 1 Equalization Control
#define R_PCH_PCR_DMI_L23EC                  0x21C0                      ///< Lane 2 and Lane 3 Equalization Control
#define B_PCH_PCR_DMI_UPL13RPH               0x0F000000                  ///< Upstream Port Lane 1/3 Transmitter Preset Hint mask
#define N_PCH_PCR_DMI_UPL13RPH               24                          ///< Upstream Port Lane 1/3 Transmitter Preset Hint value offset
#define B_PCH_PCR_DMI_UPL02RPH               0x000000F0                  ///< Upstream Port Lane 0/2 Transmitter Preset Hint mask
#define N_PCH_PCR_DMI_UPL02RPH               8                           ///< Upstream Port Lane 0/2 Transmitter Preset Hint value offset
#define V_PCH_PCR_DMI_UPL0RPH                7                           ///< Upstream Port Lane 0 Transmitter Preset Hint value
#define V_PCH_PCR_DMI_UPL1RPH                7                           ///< Upstream Port Lane 1 Transmitter Preset Hint value
#define V_PCH_PCR_DMI_UPL2RPH                7                           ///< Upstream Port Lane 2 Transmitter Preset Hint value
#define V_PCH_PCR_DMI_UPL3RPH                7                           ///< Upstream Port Lane 3 Transmitter Preset Hint value


//
// North Port Error Injection Configuration (DMI Only)
//
#define R_PCH_PCR_DMI_DMIEN                  0x2230                      ///< DMI Error Injection Enable

//
// DMI Control
//
#define R_PCH_PCR_DMI_DMIC                   0x2234                              ///< DMI Control
#define B_PCH_PCR_DMI_DMIC_SRL               BIT31                               ///< Secured register lock
#define B_PCH_PCR_DMI_DMIC_ORCE              (BIT25 | BIT24)                     ///< Offset Re-Calibration Enable
#define N_PCH_PCR_DMI_DMIC_ORCE              24
#define V_PCH_PCR_DMI_DMIC_ORCE_EN_GEN2_GEN3 1                                   ///< Enable offset re-calibration for Gen 2 and Gen 3 data rate only.
#define B_PCH_PCR_DMI_DMIC_DMICGEN           (BIT4 | BIT3 | BIT2 | BIT1 | BIT0)  ///< DMI Clock Gate Enable
#define R_PCH_PCR_DMI_DMIHWAWC               0x2238                              ///< DMI HW Autonomus Width Control
#define R_PCH_PCR_DMI_IOSFSBCS               0x223E                              ///< IOSF Sideband Control and Status
#define B_PCH_PCR_DMI_IOSFSBCS_DMICGEN       (BIT6 | BIT5 | BIT3 | BIT2)         ///< DMI Clock Gate Enable

#define R_PCH_PCR_DMI_2300                   0x2300
#define R_PCH_PCR_DMI_2304                   0x2304
#define R_PCH_PCR_DMI_2310                   0x2310
#define R_PCH_PCR_DMI_2314                   0x2314
#define R_PCH_PCR_DMI_2320                   0x2320
#define R_PCH_PCR_DMI_2324                   0x2324
#define R_PCH_PCR_DMI_232C                   0x232C
#define R_PCH_PCR_DMI_2334                   0x2334
#define R_PCH_PCR_DMI_2338                   0x2338
#define R_PCH_PCR_DMI_2340                   0x2340
#define R_PCH_PCR_DMI_2344                   0x2344
#define R_PCH_PCR_DMI_2348                   0x2348
#define R_PCH_PCR_DMI_234C                   0x234C

//
// Port Configuration Extension(DMI Only)
//
#define R_PCH_PCR_DMI_EQCFG1                 0x2450
#define B_PCH_PCR_DMI_EQCFG1_RTLEPCEB        BIT16
#define R_PCH_PCR_DMI_LTCO1                  0x2470                      ///< Local Transmitter Coefficient Override 1
#define R_PCH_PCR_DMI_LTCO2                  0x2474                      ///< Local Transmitter Coefficient Override 2
#define B_PCH_PCR_DMI_L13TCOE                BIT25                       ///< Lane 1/3 Transmitter Coefficient Override Enable
#define B_PCH_PCR_DMI_L02TCOE                BIT24                       ///< Lane 0/2 Transmitter Coefficient Override Enable
#define B_PCH_PCR_DMI_L13TPOSTCO             0x00fc0000                  ///< Lane 1/3 Transmitter Post-Cursor Coefficient Override mask
#define N_PCH_PCR_DMI_L13TPOSTCO             18                          ///< Lane 1/3 Transmitter Post-Cursor Coefficient Override value offset
#define B_PCH_PCR_DMI_L13TPRECO              0x0003f000                  ///< Lane 1/3 Transmitter Pre-Cursor Coefficient Override mask
#define N_PCH_PCR_DMI_L13TPRECO              12                          ///< Lane 1/3 Transmitter Pre-Cursor Coefficient Override value offset
#define B_PCH_PCR_DMI_L02TPOSTCO             0x00000fc0                  ///< Lane 0/2 Transmitter Post-Cursor Coefficient Override mask
#define N_PCH_PCR_DMI_L02TPOSTCO             6                           ///< Lane 0/2 Transmitter Post-Cursor Coefficient Override value offset
#define B_PCH_PCR_DMI_L02TPRECO              0x0000003f                  ///< Lane 0/2 Transmitter Pre-Cursor Coefficient Override mask
#define N_PCH_PCR_DMI_L02TPRECO              0                           ///< Lane 0/2 Transmitter Pre-Cursor Coefficient Override value offset
#define R_PCH_PCR_DMI_G3L0SCTL               0x2478                      ///< GEN3 L0s Control

//
// OP-DMI Specific Registers (OP-DMI Only)
//
#define R_PCH_PCR_OPDMI_LCTL                  0x2600                      ///< Link Control
#define R_PCH_PCR_OPDMI_STC                   0x260C                      ///< Sideband Timing Control
#define R_PCH_PCR_OPDMI_LPMC                  0x2614                      ///< Link Power Management Control
#define R_PCH_PCR_OPDMI_LCFG                  0x2618                      ///< Link Configuration

//
// DMI Source Decode PCRs (Common)
//
#define R_PCH_PCR_DMI_PCIEPAR1E         0x2700                ///< PCIE Port IOxAPIC Range 1 Enable
#define R_PCH_PCR_DMI_PCIEPAR2E         0x2704                ///< PCIE Port IOxAPIC Range 2 Enable
#define R_PCH_PCR_DMI_PCIEPAR3E         0x2708                ///< PCIE Port IOxAPIC Range 3 Enable
#define R_PCH_PCR_DMI_PCIEPAR4E         0x270C                ///< PCIE Port IOxAPIC Range 4 Enable
#define R_PCH_PCR_DMI_PCIEPAR1DID       0x2710                ///< PCIE Port IOxAPIC Range 1 Destination ID
#define R_PCH_PCR_DMI_PCIEPAR2DID       0x2714                ///< PCIE Port IOxAPIC Range 2 Destination ID
#define R_PCH_PCR_DMI_PCIEPAR3DID       0x2718                ///< PCIE Port IOxAPIC Range 3 Destination ID
#define R_PCH_PCR_DMI_PCIEPAR4DID       0x271C                ///< PCIE Port IOxAPIC Range 4 Destination ID
#define R_PCH_PCR_DMI_P2SBIOR           0x2720                ///< P2SB IO Range
#define R_PCH_PCR_DMI_TTTBARB           0x2724                ///< Thermal Throttling BIOS Assigned Thermal Base Address
#define R_PCH_PCR_DMI_TTTBARBH          0x2728                ///< Thermal Throttling BIOS Assigned Thermal Base High Address
#define R_PCH_PCR_DMI_LPCLGIR1          0x2730                ///< LPC Generic I/O Range 1
#define R_PCH_PCR_DMI_LPCLGIR2          0x2734                ///< LPC Generic I/O Range 2
#define R_PCH_PCR_DMI_LPCLGIR3          0x2738                ///< LPC Generic I/O Range 3
#define R_PCH_PCR_DMI_LPCLGIR4          0x273C                ///< LPC Generic I/O Range 4
#define R_PCH_PCR_DMI_LPCGMR            0x2740                ///< LPC Generic Memory Range
#define R_PCH_PCR_DMI_LPCBDE            0x2744                ///< LPC BIOS Decode Enable
#define R_PCH_PCR_DMI_UCPR              0x2748                ///< uCode Patch Region
#define B_PCH_PCR_DMI_UCPR_UPRE         BIT0                  ///< uCode Patch Region Enable
#define R_PCH_PCR_DMI_GCS               0x274C                ///< Generic Control and Status
#define B_PCH_PCR_DMI_RPRDID            0xFFFF0000            ///< RPR Destination ID
#define B_PCH_PCR_DMI_BBS               BIT10                 ///< Boot BIOS Strap
#define B_PCH_PCR_DMI_RPR               BIT11                 ///< Reserved Page Route
#define B_PCH_PCR_DMI_BILD              BIT0                  ///< BIOS Interface Lock-Down
#define R_PCH_PCR_DMI_IOT1              0x2750                ///< I/O Trap Register 1
#define R_PCH_PCR_DMI_IOT2              0x2758                ///< I/O Trap Register 2
#define R_PCH_PCR_DMI_IOT3              0x2760                ///< I/O Trap Register 3
#define R_PCH_PCR_DMI_IOT4              0x2768                ///< I/O Trap Register 4
#define R_PCH_PCR_DMI_LPCIOD            0x2770                ///< LPC I/O Decode Ranges
#define R_PCH_PCR_DMI_LPCIOE            0x2774                ///< LPC I/O Enables
#define R_PCH_PCR_DMI_TCOBASE           0x2778                ///< TCO Base Address
#define B_PCH_PCR_DMI_TCOBASE_TCOBA     0xFFE0                ///< TCO Base Address Mask
#define R_PCH_PCR_DMI_GPMR1             0x277C                ///< General Purpose Memory Range 1
#define R_PCH_PCR_DMI_GPMR1DID          0x2780                ///< General Purpose Memory Range 1 Destination ID
#define R_PCH_PCR_DMI_GPMR2             0x2784                ///< General Purpose Memory Range 2
#define R_PCH_PCR_DMI_GPMR2DID          0x2788                ///< General Purpose Memory Range 2 Destination ID
#define R_PCH_PCR_DMI_GPMR3             0x278C                ///< General Purpose Memory Range 3
#define R_PCH_PCR_DMI_GPMR3DID          0x2790                ///< General Purpose Memory Range 3 Destination ID
#define R_PCH_PCR_DMI_GPIOR1            0x2794                ///< General Purpose I/O Range 1
#define R_PCH_PCR_DMI_GPIOR1DID         0x2798                ///< General Purpose I/O Range 1 Destination ID
#define R_PCH_PCR_DMI_GPIOR2            0x279C                ///< General Purpose I/O Range 2
#define R_PCH_PCR_DMI_GPIOR2DID         0x27A0                ///< General Purpose I/O Range 2 Destination ID
#define R_PCH_PCR_DMI_GPIOR3            0x27A4                ///< General Purpose I/O Range 3
#define R_PCH_PCR_DMI_GPIOR3DID         0x27A8                ///< General Purpose I/O Range 3 Destination ID
#define R_PCH_PCR_DMI_PMBASEA           0x27AC                ///< PM Base Address
#define R_PCH_PCR_DMI_PMBASEC           0x27B0                ///< PM Base Control
#define R_PCH_PCR_DMI_ACPIBA            0x27B4                ///< ACPI Base Address
#define R_PCH_PCR_DMI_ACPIBDID          0x27B8                ///< ACPI Base Destination ID


//
// Opi PHY registers
//
#define R_PCH_PCR_OPIPHY_0110           0x0110
#define R_PCH_PCR_OPIPHY_0118           0x0118
#define R_PCH_PCR_OPIPHY_011C           0x011C
#define R_PCH_PCR_OPIPHY_0354           0x0354
#define R_PCH_PCR_OPIPHY_B104           0xB104
#define R_PCH_PCR_OPIPHY_B10C           0xB10C

#endif
