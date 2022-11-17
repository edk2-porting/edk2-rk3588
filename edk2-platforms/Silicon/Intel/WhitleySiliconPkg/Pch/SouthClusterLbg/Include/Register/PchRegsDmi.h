/** @file
  Register names for DMI and OP-DMI
  Conventions:
  Prefixes:
  Definitions beginning with "R_" are registers
  Definitions beginning with "B_" are bits within registers
  Definitions beginning with "V_" are meaningful values within the bits
  Definitions beginning with "S_" are register sizes
  Definitions beginning with "N_" are the bit position
  In general, PCH registers are denoted by "_PCH_" in register names
  Registers / bits that are different between PCH generations are denoted by
  _PCH_[generation_name]_" in register/bit names.
  Registers / bits that are specific to PCH-H denoted by "_H_" in register/bit names.
  Registers / bits that are specific to PCH-LP denoted by "_LP_" in register/bit names.
  e.g., "_PCH_H_", "_PCH_LP_"
  Registers / bits names without _H_ or _LP_ apply for both H and LP.
  Registers / bits that are different between SKUs are denoted by "_[SKU_name]"
  at the end of the register/bit names
  Registers / bits of new devices introduced in a PCH generation will be just named
  as "_PCH_" without [generation_name] inserted.

  @copyright
  Copyright 2014 - 2021 Intel Corporation. <BR>

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
#define R_PCH_DMI_PCR_V0CTL                  0x2014                      ///< Virtual channel 0 resource control
#define B_PCH_DMI_PCR_V0CTL_EN               BIT31
#define B_PCH_DMI_PCR_V0CTL_ID               (7 << 24)                   ///< Bit[26:24]
#define N_PCH_DMI_PCR_V0CTL_ID               24
#define V_PCH_DMI_PCR_V0CTL_ETVM_MASK        0xFC00
#define V_PCH_DMI_PCR_V0CTL_TVM_MASK         0x7E
#define R_PCH_DMI_PCR_V0STS                  0x201A                      ///< Virtual channel 0 status
#define B_PCH_DMI_PCR_V0STS_NP               BIT1
#define R_PCH_DMI_PCR_V1CTL                  0x2020                      ///< Virtual channel 1 resource control
#define B_PCH_DMI_PCR_V1CTL_EN               BIT31
#define B_PCH_DMI_PCR_V1CTL_ID               (0x0F << 24)                ///< Bit[27:24]
#define N_PCH_DMI_PCR_V1CTL_ID               24
#define V_PCH_DMI_PCR_V1CTL_ETVM_MASK        0xFC00
#define V_PCH_DMI_PCR_V1CTL_TVM_MASK         0xFE
#define R_PCH_DMI_PCR_V1STS                  0x2026                      ///< Virtual channel 1 status
#define B_PCH_DMI_PCR_V1STS_NP               BIT1
#define R_PCH_DMI_PCR_VMCTL                  0x2040                      ///< ME Virtual Channel (VCm) resource control
#define R_PCH_DMI_PCR_VMSTS                  0x2046                      ///< ME Virtual Channel Resource Status
#define R_PCH_DMI_PCR_IOSFC1TC               0x2054                      ///< Offset of credits for VC1 register
#define R_PCH_DMI_PCR_IOSFC2TC               0x2058                      ///< Offset of credits for VCm register
#define V_PCH_DMI_PCR_IOSFC1TC_ICX           0x00021002                  ///< Credits for VC1 - values for ICX
#define V_PCH_DMI_PCR_IOSFC2TC_ICX           0x00082005                  ///< Credits for VCm - values for ICX
#define R_PCH_DMI_PCR_UEM                    0x2088                      ///< Uncorrectable Error Mask
#define R_PCH_DMI_PCR_REC                    0x20AC                      ///< Root Error Command

//
// DMI Error Reporting
//
#define R_PCH_DMI_PCR_UES                     0x2084                      ///< Uncorrectable Error Status
#define R_PCH_DMI_PCR_UEM                     0x2088                      ///< Uncorrectable Error Mask
#define B_PCH_DMI_UE_DLPE                     BIT4                        // Data Link Protocol Error
#define B_PCH_DMI_UE_PT                       BIT12                       // Poisoned TLP
#define B_PCH_DMI_UE_CA                       BIT15                       // Completer Abort
#define B_PCH_DMI_UE_RO                       BIT17                       // Receiver Overflow
#define B_PCH_DMI_UE_MT                       BIT18                       // Malformed TLP
#define R_PCH_DMI_PCR_CES                     0x2090                      ///< Correctable Error Status
#define R_PCH_DMI_PCR_CEM                     0x2094                      ///< Correctable Error Mask
#define B_PCH_DMI_CE_RE                       BIT0                        // Indicates a receiver error
#define B_PCH_DMI_CE_BT                       BIT6                        // Bad TLP
#define B_PCH_DMI_CE_BD                       BIT7                        // Bad DLLP
#define B_PCH_DMI_CE_RNR                      BIT8                        // Replay Number Rollover
#define R_PCH_DMI_PCR_RES                     0x20B0                      ///< Root Error Status
#define B_PCH_DMI_RES_CR                      BIT0                        // correctable error message is received or an internal correctable error is detected
#define B_PCH_DMI_RES_ENR                     BIT2                        // either afatal or a non-fatal error message is received or an internal fatal error is detected

//
// Internal Link Configuration (DMI Only)
//
#define R_PCH_DMI_PCR_LCAP                   0x21A4                      ///< Link Capabilities
#define B_PCH_DMI_PCR_LCAP_EL1               (BIT17 | BIT16 | BIT15)
#define B_PCH_DMI_PCR_LCAP_EL0               (BIT14 | BIT13 | BIT12)
#define B_PCH_DMI_PCR_LCAP_APMS              (BIT11 | BIT10)             ///< L0 is supported on DMI
#define B_PCH_DMI_PCR_LCAP_MLW               0x000003F0
#define B_PCH_DMI_PCR_LCAP_MLS               0x0000000F

#define R_PCH_DMI_PCR_LCTL                   0x21A8                      ///< Link Control
#define B_PCH_DMI_PCR_LCTL_ES                BIT7
#define B_PCH_DMI_PCR_LCTL_ASPM              (BIT1 | BIT0)               ///< Link ASPM
#define R_PCH_DMI_PCR_LSTS                   0x21AA                      ///< Link Status

#define R_PCH_DMI_PCR_LCAP2                  0x21AC                      ///< Link Control 2
typedef union {
  UINT32   Dword;
  struct {
    UINT32 Rsrvd0   : 1,
           SLSV     : 7, // Supported Link Speed Vector
           CS       : 1, // Crosslink Supported
           LSOSGSSV : 7, // Lower SKP OS Generation Supported Speeds Vector
           LSOSRSS  : 7, // Lower SKP OS Reception Supported Speeds Vector
           Rsrvd1   : 9;
  } Bits;
} PCH_DMI_PCR_LCAP2;

#define R_PCH_DMI_PCR_LCTL2                  0x21B0                      ///< Link Control 2
typedef union {
  UINT32   Dword;
  struct {
    UINT32 TLS    : 4, // 0:3 Target Link Speed
           EC     : 1, // 4 Enter Compliance
           HASD   : 1, // 5 Hardware Autonomous Speed Disable
           SD     : 1, // 6 Selectable De-emphasis
           TM     : 3, // 9:7 Transmit Margin
           EMC    : 1, // 10 Enter Modified Compliance
           CSOS   : 1, // 11 Compliance SOS
           CD     : 4, // 15:12 Compliance Preset/De-emphasis
           CDL    : 1, // 16 Current De-emphasis Level
           EqC    : 1, // 17 Equalization Complete
           EQP1S  : 1, // 18 Equalization Phase 1 Successful
           EQP2S  : 1, // 19 Equalization Phase 2 Successful
           EQP3S  : 1, // 20 Equalization Phase 3 Successful
           LER    : 1, // 21 Link Equalization Request
           Rsrvd0 :10; // 31:22
  } Bits;
} PCH_DMI_PCR_LCTL2;

#define R_PCH_DMI_PCR_LSTS2                  0x21B2                      ///< Link Status 2
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
#define R_PCH_DMI_PCR_DMIEN                  0x2230                      ///< DMI Error Injection Enable

//
// DMI Control
//
#define R_PCH_DMI_PCR_DMIC                   0x2234                              ///< DMI Control
#define B_PCH_DMI_PCR_DMIC_SRL               BIT31                               ///< Secured register lock
#define B_PCH_DMI_PCR_DMIC_ORCE              (BIT25 | BIT24)                     ///< Offset Re-Calibration Enable
#define N_PCH_DMI_PCR_DMIC_ORCE              24
#define V_PCH_DMI_PCR_DMIC_ORCE_EN_GEN2_GEN3 1                                   ///< Enable offset re-calibration for Gen 2 and Gen 3 data rate only.
#define B_PCH_DMI_PCR_DMIC_DMICGEN           (BIT4 | BIT3 | BIT2 | BIT1 | BIT0)  ///< DMI Clock Gate Enable
#define R_PCH_DMI_PCR_DMIHWAWC               0x2238                              ///< DMI HW Autonomus Width Control
#define R_PCH_DMI_PCR_IOSFSBCS               0x223E                              ///< IOSF Sideband Control and Status
#define B_PCH_DMI_PCR_IOSFSBCS_DMICGEN       (BIT6 | BIT5 | BIT3 | BIT2)         ///< DMI Clock Gate Enable
#define B_PCH_PCR_DMI_DMIC_DNPRL             BIT19

#define R_PCH_DMI_PCR_2300                   0x2300
#define R_PCH_DMI_PCR_2304                   0x2304
#define R_PCH_DMI_PCR_2310                   0x2310
#define B_PCH_PCR_DMI_2310_HALEP             BIT22
#define R_PCH_DMI_PCR_2314                   0x2314
#define R_PCH_DMI_PCR_2320                   0x2320
#define R_PCH_DMI_PCR_2324                   0x2324
#define R_PCH_DMI_PCR_232C                   0x232C
#define R_PCH_DMI_PCR_2334                   0x2334
#define R_PCH_DMI_PCR_2338                   0x2338
#define R_PCH_DMI_PCR_2340                   0x2340
#define R_PCH_DMI_PCR_2344                   0x2344
#define R_PCH_DMI_PCR_2348                   0x2348
#define R_PCH_PCR_DMI_234C                   0x234C

//
// Port Configuration Extension(DMI Only)
//
#define R_PCH_DMI_PCR_EQCFG1                 0x2450
#define B_PCH_DMI_PCR_EQCFG1_RTLEPCEB        BIT16
#define B_PCH_PCR_DMI_EQCFG1_RTPCOE          BIT15                       ///< Remote Transmitter Preset Coefficient Override Enable

#define R_PCH_PCR_DMI_RTPCL1                 0x2454                      ///< Remote Transmitter Preset Coefficient List 1

#define N_PCH_PCR_DMI_RTPCL1_RTPRECL2PL4    24
#define N_PCH_PCR_DMI_RTPCL1_RTPOSTCL1PL3   18
#define N_PCH_PCR_DMI_RTPCL1_RTPRECL1PL2    12
#define N_PCH_PCR_DMI_RTPCL1_RTPOSTCL0PL1   6
#define N_PCH_PCR_DMI_RTPCL1_RTPRECL0PL0    0

#define B_PCH_PCR_DMI_RTPCL1_PCM            BIT31
#define B_PCH_PCR_DMI_RTPCL1_RTPRECL2PL4    (0X3F << N_PCH_PCR_DMI_RTPCL1_RTPRECL2PL4)
#define B_PCH_PCR_DMI_RTPCL1_RTPOSTCL1PL3   (0X3F << N_PCH_PCR_DMI_RTPCL1_RTPOSTCL1PL3)
#define B_PCH_PCR_DMI_RTPCL1_RTPRECL1PL2    (0X3F << N_PCH_PCR_DMI_RTPCL1_RTPRECL1PL2)
#define B_PCH_PCR_DMI_RTPCL1_RTPOSTCL0PL1   (0X3F << N_PCH_PCR_DMI_RTPCL1_RTPOSTCL0PL1)
#define B_PCH_PCR_DMI_RTPCL1_RTPRECL0PL0    (0X3F << N_PCH_PCR_DMI_RTPCL1_RTPRECL0PL0)
#define R_PCH_PCR_DMI_RTPCL2                 0x2458                      ///< Remote Transmitter Preset Coefficient List 2

#define N_PCH_PCR_DMI_RTPCL2_RTPOSTCL4PL9   24
#define N_PCH_PCR_DMI_RTPCL2_RTPRECL4PL8    18
#define N_PCH_PCR_DMI_RTPCL2_RTPOSTCL3PL7   12
#define N_PCH_PCR_DMI_RTPCL2_RTPRECL3PL6    6
#define N_PCH_PCR_DMI_RTPCL2_RTPOSTCL2PL5   0

#define B_PCH_PCR_DMI_RTPCL2_RTPOSTCL4PL9   (0X3F << N_PCH_PCR_DMI_RTPCL2_RTPOSTCL4PL9)
#define B_PCH_PCR_DMI_RTPCL2_RTPRECL4PL8    (0X3F << N_PCH_PCR_DMI_RTPCL2_RTPRECL4PL8)
#define B_PCH_PCR_DMI_RTPCL2_RTPOSTCL3PL7   (0X3F << N_PCH_PCR_DMI_RTPCL2_RTPOSTCL3PL7)
#define B_PCH_PCR_DMI_RTPCL2_RTPRECL3PL6    (0X3F << N_PCH_PCR_DMI_RTPCL2_RTPRECL3PL6)
#define B_PCH_PCR_DMI_RTPCL2_RTPOSTCL2PL5   (0X3F << N_PCH_PCR_DMI_RTPCL2_RTPOSTCL2PL5)


#define R_PCH_DMI_PCR_LTCO1                  0x2470                      ///< Local Transmitter Coefficient Override 1
#define R_PCH_DMI_PCR_LTCO2                  0x2474                      ///< Local Transmitter Coefficient Override 2
#define B_PCH_DMI_PCR_L13TCOE                BIT25                       ///< Lane 1/3 Transmitter Coefficient Override Enable
#define B_PCH_DMI_PCR_L02TCOE                BIT24                       ///< Lane 0/2 Transmitter Coefficient Override Enable
#define B_PCH_DMI_PCR_L13TPOSTCO             0x00fc0000                  ///< Lane 1/3 Transmitter Post-Cursor Coefficient Override mask
#define N_PCH_DMI_PCR_L13TPOSTCO             18                          ///< Lane 1/3 Transmitter Post-Cursor Coefficient Override value offset
#define B_PCH_DMI_PCR_L13TPRECO              0x0003f000                  ///< Lane 1/3 Transmitter Pre-Cursor Coefficient Override mask
#define N_PCH_DMI_PCR_L13TPRECO              12                          ///< Lane 1/3 Transmitter Pre-Cursor Coefficient Override value offset
#define B_PCH_DMI_PCR_L02TPOSTCO             0x00000fc0                  ///< Lane 0/2 Transmitter Post-Cursor Coefficient Override mask
#define N_PCH_DMI_PCR_L02TPOSTCO             6                           ///< Lane 0/2 Transmitter Post-Cursor Coefficient Override value offset
#define B_PCH_DMI_PCR_L02TPRECO              0x0000003f                  ///< Lane 0/2 Transmitter Pre-Cursor Coefficient Override mask
#define N_PCH_DMI_PCR_L02TPRECO              0                           ///< Lane 0/2 Transmitter Pre-Cursor Coefficient Override value offset
#define R_PCH_DMI_PCR_G3L0SCTL               0x2478                      ///< GEN3 L0s Control

//
// OP-DMI Specific Registers (OP-DMI Only)
//
#define R_PCH_OPDMI_PCR_LCTL                  0x2600                      ///< Link Control
#define R_PCH_OPDMI_PCR_STC                   0x260C                      ///< Sideband Timing Control
#define R_PCH_OPDMI_PCR_LPMC                  0x2614                      ///< Link Power Management Control
#define R_PCH_OPDMI_PCR_LCFG                  0x2618                      ///< Link Configuration

//
// DMI Source Decode PCRs (Common)
//
#define R_PCH_DMI_PCR_PCIEPAR1E         0x2700                ///< PCIE Port IOxAPIC Range 1 Enable
#define R_PCH_DMI_PCR_PCIEPAR2E         0x2704                ///< PCIE Port IOxAPIC Range 2 Enable
#define R_PCH_DMI_PCR_PCIEPAR3E         0x2708                ///< PCIE Port IOxAPIC Range 3 Enable
#define R_PCH_DMI_PCR_PCIEPAR4E         0x270C                ///< PCIE Port IOxAPIC Range 4 Enable
#define R_PCH_DMI_PCR_PCIEPAR1DID       0x2710                ///< PCIE Port IOxAPIC Range 1 Destination ID
#define R_PCH_DMI_PCR_PCIEPAR2DID       0x2714                ///< PCIE Port IOxAPIC Range 2 Destination ID
#define R_PCH_DMI_PCR_PCIEPAR3DID       0x2718                ///< PCIE Port IOxAPIC Range 3 Destination ID
#define R_PCH_DMI_PCR_PCIEPAR4DID       0x271C                ///< PCIE Port IOxAPIC Range 4 Destination ID
#define R_PCH_DMI_PCR_P2SBIOR           0x2720                ///< P2SB IO Range
#define R_PCH_DMI_PCR_TTTBARB           0x2724                ///< Thermal Throttling BIOS Assigned Thermal Base Address
#define R_PCH_DMI_PCR_TTTBARBH          0x2728                ///< Thermal Throttling BIOS Assigned Thermal Base High Address
#define R_PCH_DMI_PCR_LPCLGIR1          0x2730                ///< LPC Generic I/O Range 1
#define R_PCH_DMI_PCR_LPCLGIR2          0x2734                ///< LPC Generic I/O Range 2
#define R_PCH_DMI_PCR_LPCLGIR3          0x2738                ///< LPC Generic I/O Range 3
#define R_PCH_DMI_PCR_LPCLGIR4          0x273C                ///< LPC Generic I/O Range 4
#define R_PCH_DMI_PCR_LPCGMR            0x2740                ///< LPC Generic Memory Range
#define R_PCH_DMI_PCR_LPCBDE            0x2744                ///< LPC BIOS Decode Enable
#define R_PCH_DMI_PCR_UCPR              0x2748                ///< uCode Patch Region
#define B_PCH_DMI_PCR_UCPR_UPRE         BIT0                  ///< uCode Patch Region Enable
#define R_PCH_DMI_PCR_GCS               0x274C                ///< Generic Control and Status
#define B_PCH_DMI_PCR_RPRDID            0xFFFF0000            ///< RPR Destination ID
#define B_PCH_DMI_PCR_BBS               BIT10                 ///< Boot BIOS Strap
#define B_PCH_DMI_PCR_RPR               BIT11                 ///< Reserved Page Route
#define B_PCH_DMI_PCR_BILD              BIT0                  ///< BIOS Interface Lock-Down
#define R_PCH_DMI_PCR_IOT1              0x2750                ///< I/O Trap Register 1
#define R_PCH_DMI_PCR_IOT2              0x2758                ///< I/O Trap Register 2
#define R_PCH_DMI_PCR_IOT3              0x2760                ///< I/O Trap Register 3
#define R_PCH_DMI_PCR_IOT4              0x2768                ///< I/O Trap Register 4
#define R_PCH_DMI_PCR_LPCIOD            0x2770                ///< LPC I/O Decode Ranges
#define R_PCH_DMI_PCR_LPCIOE            0x2774                ///< LPC I/O Enables
#define R_PCH_DMI_PCR_TCOBASE           0x2778                ///< TCO Base Address
#define B_PCH_DMI_PCR_TCOBASE_TCOBA     0xFFE0                ///< TCO Base Address Mask
#define R_PCH_DMI_PCR_GPMR1             0x277C                ///< General Purpose Memory Range 1
#define R_PCH_DMI_PCR_GPMR1DID          0x2780                ///< General Purpose Memory Range 1 Destination ID
#define R_PCH_DMI_PCR_GPMR2             0x2784                ///< General Purpose Memory Range 2
#define R_PCH_DMI_PCR_GPMR2DID          0x2788                ///< General Purpose Memory Range 2 Destination ID
#define R_PCH_DMI_PCR_GPMR3             0x278C                ///< General Purpose Memory Range 3
#define R_PCH_DMI_PCR_GPMR3DID          0x2790                ///< General Purpose Memory Range 3 Destination ID
#define R_PCH_DMI_PCR_GPIOR1            0x2794                ///< General Purpose I/O Range 1
#define R_PCH_DMI_PCR_GPIOR1DID         0x2798                ///< General Purpose I/O Range 1 Destination ID
#define R_PCH_DMI_PCR_GPIOR2            0x279C                ///< General Purpose I/O Range 2
#define R_PCH_DMI_PCR_GPIOR2DID         0x27A0                ///< General Purpose I/O Range 2 Destination ID
#define R_PCH_DMI_PCR_GPIOR3            0x27A4                ///< General Purpose I/O Range 3
#define R_PCH_DMI_PCR_GPIOR3DID         0x27A8                ///< General Purpose I/O Range 3 Destination ID
#define R_PCH_PCR_DMI_PMBASEA           0x27AC                ///< PM Base Address
#define R_PCH_PCR_DMI_PMBASEC           0x27B0                ///< PM Base Control
#define R_PCH_PCR_DMI_ACPIBA            0x27B4                ///< ACPI Base Address
#define R_PCH_PCR_DMI_ACPIBDID          0x27B8                ///< ACPI Base Destination ID
#define R_PCH_DMI_PCR_SEGMR             0x27C0                ///< Second eSPI Generic Memory Range
#define R_PCH_DMI_PCR_SEGIR             0x27BC                ///< Second eSPI Generic I/O Range

//
// Opi PHY registers
//
#define R_PCH_OPIPHY_PCR_0110           0x0110
#define R_PCH_OPIPHY_PCR_0118           0x0118
#define R_PCH_OPIPHY_PCR_011C           0x011C
#define R_PCH_OPIPHY_PCR_0354           0x0354
#define R_PCH_OPIPHY_PCR_B104           0xB104
#define R_PCH_OPIPHY_PCR_B10C           0xB10C

#endif
