/** @file
  Register names for DMI and OP-DMI

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
    Three letter acronym of the generation is used .
    Register name without GenerationName applies to all generations.
  - [ComponentName]:
    This field indicates the component name that the register belongs to (e.g. PCH, SA etc.)
    Register name without ComponentName applies to all components.
    Register that is specific to -H denoted by "_PCH_H_" in component name.
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

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

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
#define B_PCH_DMI_PCR_V0CTL_EN               BIT31
#define B_PCH_DMI_PCR_V0CTL_ID               (7 << 24)                   ///< Bit[26:24]
#define N_PCH_DMI_PCR_V0CTL_ID               24
#define V_PCH_DMI_PCR_V0CTL_ETVM_MASK        0xFC00
#define V_PCH_DMI_PCR_V0CTL_TVM_MASK         0x7E
#define B_PCH_DMI_PCR_V0STS_NP               BIT1
#define B_PCH_DMI_PCR_V1CTL_EN               BIT31
#define B_PCH_DMI_PCR_V1CTL_ID               (0x0F << 24)                ///< Bit[27:24]
#define N_PCH_DMI_PCR_V1CTL_ID               24
#define V_PCH_DMI_PCR_V1CTL_ETVM_MASK        0xFC00
#define V_PCH_DMI_PCR_V1CTL_TVM_MASK         0xFE
#define B_PCH_DMI_PCR_V1STS_NP               BIT1


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
#define R_PCH_DMI_PCR_SEGIR             0x27BC                ///< Second ESPI Generic I/O Range
#define R_PCH_DMI_PCR_SEGMR             0x27C0                ///< Second ESPI Generic Memory Range
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
