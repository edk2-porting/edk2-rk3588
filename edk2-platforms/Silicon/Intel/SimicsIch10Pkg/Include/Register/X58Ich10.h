/** @file
  Various register numbers and value bits based on the following publications:
  - Intel(R) datasheet 319973-003
  - Intel(R) datasheet 319974-017US

  Copyright (C) 2015, Red Hat, Inc.
  Copyright (c) 2014, Gabriel L. Somlo <somlo@cmu.edu>
  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __X58_ICH10_H__
#define __X58_ICH10_H__

#include <Library/PciLib.h>
#include <IndustryStandard/Pci22.h>

//
// Simics Host Bridge DID Address
//
#define SIMICS_HOSTBRIDGE_DID \
  PCI_LIB_ADDRESS (0, 0, 0, PCI_DEVICE_ID_OFFSET)

//
// Host Bridge Device ID (DID) value for ICH10
//
#define INTEL_ICH10_DEVICE_ID 0x3400

//
// B/D/F/Type: 0/0/0/PCI
//
#define DRAMC_REGISTER_Q35(Offset) PCI_LIB_ADDRESS (0, 0, 0, (Offset))
#define DRAMC_REGISTER_X58(Offset) PCI_LIB_ADDRESS (0, 20, 0, (Offset))
#define MCH_GGC                 0x52
#define MCH_GGC_IVD             BIT1

#define MCH_PCIEXBAR_LOW        0x10C
#define MCH_PCIEXBAR_LID        0x10E
#define MCH_PCIEXBAR_SHIFT      16
#define MCH_PCIEXBAR_LOWMASK    0x0FFFFFFF
#define MCH_PCIEXBAR_BUS_FF     0
#define MCH_PCIEXBAR_EN         BIT0

#define MCH_PCIEXBAR_HIGH       0x64
#define MCH_PCIEXBAR_HIGHMASK   0xFFFFFFF0

#define MCH_SMRAM             0x9D
#define MCH_SMRAM_D_LCK         BIT4
#define MCH_SMRAM_G_SMRAME      BIT3

#define MCH_ESMRAMC           0x9E
#define MCH_ESMRAMC_H_SMRAME    BIT7
#define MCH_ESMRAMC_E_SMERR     BIT6
#define MCH_ESMRAMC_SM_CACHE    BIT5
#define MCH_ESMRAMC_SM_L1       BIT4
#define MCH_ESMRAMC_SM_L2       BIT3
#define MCH_ESMRAMC_TSEG_8MB    BIT3
#define MCH_ESMRAMC_TSEG_2MB    BIT2
#define MCH_ESMRAMC_TSEG_1MB    BIT1
#define MCH_ESMRAMC_TSEG_MASK   (BIT3 | BIT2 | BIT1)
#define MCH_ESMRAMC_T_EN        BIT0

#define MCH_GBSM              0xA4
#define MCH_GBSM_MB_SHIFT       20

#define MCH_BGSM              0xA8
#define MCH_BGSM_MB_SHIFT       20

#define MCH_TSEGMB            0xA8
#define MCH_TSEGMB_MB_SHIFT     20

#define MCH_TOLUD             0xD0

//
// B/D/F/Type: 0/0x1f/0/PCI
//
#define POWER_MGMT_REGISTER_ICH10(Offset) \
  PCI_LIB_ADDRESS (0, 0x1f, 0, (Offset))

#define ICH10_PMBASE               0x40
#define ICH10_PMBASE_MASK            (BIT15 | BIT14 | BIT13 | BIT12 | BIT11 | \
                                     BIT10 | BIT9  | BIT8  | BIT7)

#define ICH10_ACPI_CNTL            0x44
#define ICH10_ACPI_CNTL_ACPI_EN      BIT7

#define ICH10_GEN_PMCON_1          0xA0
#define ICH10_GEN_PMCON_1_SMI_LOCK   BIT4

#define ICH10_RCBA                 0xF0
#define ICH10_RCBA_EN                BIT0

#define ICH10_PMBASE_IO            0x400
//
// IO ports
//
#define ICH10_APM_CNT 0xB2
#define ICH10_APM_STS 0xB3

//
// IO ports relative to PMBASE
//
#define ICH10_PMBASE_OFS_SMI_EN   0x30
#define ICH10_SMI_EN_APMC_EN      BIT5
#define ICH10_SMI_EN_GBL_SMI_EN   BIT0
#define ICH10_SMI_EN_EOS          BIT1  // End of SMI

#define ICH10_PMBASE_OFS_SMI_STS  0x34
#define ICH10_SMI_STS_APM         BIT5  // APM Status

#define ICH10_ROOT_COMPLEX_BASE 0xFED1C000

#endif
