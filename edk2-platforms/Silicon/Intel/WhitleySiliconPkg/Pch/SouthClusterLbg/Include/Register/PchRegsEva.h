/** @file
  Register names for PCH Eva devices.
  Conventions:
  Prefixes:
  Definitions beginning with "R_" are registers
  Definitions beginning with "B_" are bits within registers
  Definitions beginning with "V_" are meaningful values of bits within the registers
  Definitions beginning with "S_" are register sizes
  Definitions beginning with "N_" are the bit position
  In general, PCH registers are denoted by "_PCH_" in register names
  Registers / bits that are different between PCH generations are denoted by
  _PCH_<generation_name>_" in register/bit names. e.g., "_PCH_LPT_"
  Registers / bits that are different between SKUs are denoted by "_<SKU_name>"
  at the end of the register/bit names
  Registers / bits of new devices introduced in a PCH generation will be just named
  as "_PCH_" without <generation_name> inserted.

  @copyright
  Copyright 2014 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_REGS_EVA_H_
#define _PCH_REGS_EVA_H_

#define PCI_DEVICE_NUMBER_EVA 17
#define PCI_FUNCTION_NUMBER_EVA_MROM0  0
#define PCI_FUNCTION_NUMBER_EVA_MROM1  1
#define PCI_FUNCTION_NUMBER_EVA_SSATA  5

///
/// Lewisburg SKUs
///
#define LBG_SKU_G    1
#define LBG_SKU_X    2
#define LBG_SKU_A    3

#define PCI_DEVICE_NUMBER_PCH_SSATA     17
#define PCI_FUNCTION_NUMBER_PCH_SSATA   5

#define R_PCH_LBG_SSATA_DEVICE_ID      0x02

///
///  LBG Production sSATA Controller DID definition
///
#define V_PCH_LBG_PROD_SSATA_DEVICE_ID_D_AHCI                     0xA1D2      // LBG Production Server Secondary AHCI Mode (Ports 0-4)
#define V_PCH_LBG_PROD_SSATA_DEVICE_ID_D_RAID                     0xA1D4      // LBG Production Server RAID 0/1/5/10 - NOT premium
#define V_PCH_LBG_PROD_SSATA_DEVICE_ID_D_RAID_PREMIUM             0xA1D6      // LBG Production Server RAID 0/1/5/10 - premium
#define V_PCH_LBG_PROD_SSATA_DEVICE_ID_D_RAID1                    0xA1DE      // LBG Production Server RAID 1/RRT

///
/// LBG Production (PRQ) MSUint SMBUS DID definition
///
#define V_PCH_LBG_PROD_MROM_DEVICE_ID_0                      0xA1F0       // LBG MS Unit MROM 0 PRQ DID
#define V_PCH_LBG_PROD_MROM_DEVICE_ID_1                      0xA1F1       // LBG MS Unit MROM 1 PRQ DID


///
///  LBG SSX (Super SKUs and Pre Production) sSATA Controller DID definition
///
#define V_PCH_LBG_SSATA_DEVICE_ID_D_AHCI                  0xA252      // LBG SSX Server Secondary AHCI Mode (Ports 0-4)
#define V_PCH_LBG_SSATA_DEVICE_ID_D_RAID                  0xA254      // LBG SSX Server RAID 0/1/5/10 - NOT premium
#define V_PCH_LBG_SSATA_DEVICE_ID_D_RAID_PREMIUM          0xA256      // LBG SSX Server RAID 0/1/5/10 - premium
#define V_PCH_LBG_SSATA_DEVICE_ID_D_RAID1                 0xA25E      // LBG SSX Server RAID 1/RRT

#define V_PCH_LBG_SSATA_DEVICE_ID_D_RAID_PREMIUM_DSEL0       0x2823      // Server RAID 0/1/5/10 - premium - Alternate ID for RST
#define V_PCH_LBG_SSATA_DEVICE_ID_D_RAID_PREMIUM_DSEL1       0x2827      // Server RAID 0/1/5/10 - premium - Alternate ID for RSTe

///
/// LBG Super SKU (SSX) MSUint DID definition
///
#define V_PCH_LBG_MROM_DEVICE_ID_0                        0xA270       // LBG NS MS Unit MROM 0 Super SKU DID
#define V_PCH_LBG_MROM_DEVICE_ID_1                        0xA271       // LBG NS MS Unit MROM 1 Super SKU DID

#define R_PCH_LBG_MROM_DEVCLKGCTL     0xE4

#define R_PCH_LBG_MROM_PLKCTL         0xE8
#define B_PCH_LBG_MROM_PLKCTL_CL      BIT0

#define ADR_TMR_HELD_OFF_SETUP_OPTION 2
#define R_PCH_LBG_MROM_ADRTIMERCTRL   0x180
#define B_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_MASK (BIT27|BIT26|BIT25|BIT24)
#define N_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT      24
#define V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_1    0x0
#define V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_8    0x1
#define V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_24   0x2
#define V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_40   0x3
#define V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_56   0x4
#define V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_64   0x5
#define V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_72   0x6
#define V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_80   0x7
#define V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_88   0x8
#define V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_96   0x9
#define V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_MAX  (V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_96)
#define ADR_MULT_SETUP_DEFAULT_POR 99
#define B_PCH_LBG_MROM_ADRTIMERCTRL_ADR_DBG_DIS   BIT28
#define B_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR_DIS   BIT29
#define B_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR_MASK  (BIT30|BIT31)
#define N_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR       30
#define V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR_25US  0x0
#define V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR_50US  0x1
#define V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR_100US 0x2
#define V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR_0US   0x3
#define V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR_MAX   (V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR_0US)
#define ADR_TMR_SETUP_DEFAULT_POR 4

///
///  MS Unit Hide Control Register
///
#define PCH_LBG_MSUINT_FUNCS              3
#define R_PCH_LBG_MSUINT_MSDEVFUNCHIDE          0xD4
#define B_PCH_LBG_MSUINT_MSDEVFUNCHIDE_RSVD    (BIT30|BIT29|BIT28|BIT27|BIT26|BIT25|BIT24|\
                                            BIT23|BIT22|BIT21|BIT20|BIT19|BIT18|BIT17|\
                                            BIT16|BIT15|BIT14|BIT13|BIT12|BIT11|BIT10|\
                                            BIT9|BIT8|BIT7|BIT6|BIT4|BIT3|BIT2)

#define B_PCH_EVA_MSUNIT_MSDEVFUNCHIDE_SSATA  (BIT5)

#define B_PCH_EVA_MSUNIT_MSDEVFUNCHIDE_MROM1   BIT1
#define B_PCH_EVA_MSUNIT_MSDEVFUNCHIDE_MROM0   BIT0
#define B_PCH_EVA_MSUNIT_MSDEVFUNCHIDE_REGLOCK BIT31

#endif
