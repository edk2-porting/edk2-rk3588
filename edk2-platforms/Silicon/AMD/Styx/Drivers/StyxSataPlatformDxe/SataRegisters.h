/** @file
  Header file for AHCI mode of ATA host controller.

  Copyright (c) 2010 - 2011, Intel Corporation. All rights reserved.<BR>
  Copyright (c) 2014 - 2016, AMD Inc. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef __SATA_REGISTERS_H__
#define __SATA_REGISTERS_H__

#define EFI_AHCI_BAR_INDEX                     0x05

#define EFI_AHCI_CAPABILITY_OFFSET             0x0000
#define   EFI_AHCI_CAP_SSS                     BIT27
#define   EFI_AHCI_CAP_SMPS                    BIT28
#define   EFI_AHCI_CAP_S64A                    BIT31
#define EFI_AHCI_GHC_OFFSET                    0x0004
#define   EFI_AHCI_GHC_RESET                   BIT0
#define   EFI_AHCI_GHC_IE                      BIT1
#define   EFI_AHCI_GHC_ENABLE                  BIT31
#define EFI_AHCI_IS_OFFSET                     0x0008
#define EFI_AHCI_PI_OFFSET                     0x000C

#define EFI_AHCI_MAX_PORTS                     32

//
// Refer SATA1.0a spec section 5.2, the Phy detection time should be less than 10ms.
//
#define  EFI_AHCI_BUS_PHY_DETECT_TIMEOUT       10
//
// Refer SATA1.0a spec, the FIS enable time should be less than 500ms.
//
#define  EFI_AHCI_PORT_CMD_FR_CLEAR_TIMEOUT    EFI_TIMER_PERIOD_MILLISECONDS(500)
//
// Refer SATA1.0a spec, the bus reset time should be less than 1s.
//
#define  EFI_AHCI_BUS_RESET_TIMEOUT            EFI_TIMER_PERIOD_SECONDS(1)

#define  EFI_AHCI_ATAPI_DEVICE_SIG             0xEB140000
#define  EFI_AHCI_ATA_DEVICE_SIG               0x00000000
#define  EFI_AHCI_PORT_MULTIPLIER_SIG          0x96690000
#define  EFI_AHCI_ATAPI_SIG_MASK               0xFFFF0000

//
// Each PRDT entry can point to a memory block up to 4M byte
//
#define EFI_AHCI_MAX_DATA_PER_PRDT             0x400000

#define EFI_AHCI_FIS_REGISTER_H2D              0x27      //Register FIS - Host to Device
#define   EFI_AHCI_FIS_REGISTER_H2D_LENGTH     20
#define EFI_AHCI_FIS_REGISTER_D2H              0x34      //Register FIS - Device to Host
#define   EFI_AHCI_FIS_REGISTER_D2H_LENGTH     20
#define EFI_AHCI_FIS_DMA_ACTIVATE              0x39      //DMA Activate FIS - Device to Host
#define   EFI_AHCI_FIS_DMA_ACTIVATE_LENGTH     4
#define EFI_AHCI_FIS_DMA_SETUP                 0x41      //DMA Setup FIS - Bi-directional
#define   EFI_AHCI_FIS_DMA_SETUP_LENGTH        28
#define EFI_AHCI_FIS_DATA                      0x46      //Data FIS - Bi-directional
#define EFI_AHCI_FIS_BIST                      0x58      //BIST Activate FIS - Bi-directional
#define   EFI_AHCI_FIS_BIST_LENGTH             12
#define EFI_AHCI_FIS_PIO_SETUP                 0x5F      //PIO Setup FIS - Device to Host
#define   EFI_AHCI_FIS_PIO_SETUP_LENGTH        20
#define EFI_AHCI_FIS_SET_DEVICE                0xA1      //Set Device Bits FIS - Device to Host
#define   EFI_AHCI_FIS_SET_DEVICE_LENGTH       8

#define EFI_AHCI_D2H_FIS_OFFSET                0x40
#define EFI_AHCI_DMA_FIS_OFFSET                0x00
#define EFI_AHCI_PIO_FIS_OFFSET                0x20
#define EFI_AHCI_SDB_FIS_OFFSET                0x58
#define EFI_AHCI_FIS_TYPE_MASK                 0xFF
#define EFI_AHCI_U_FIS_OFFSET                  0x60

//
// Port register
//
#define EFI_AHCI_PORT_START                    0x0100
#define EFI_AHCI_PORT_REG_WIDTH                0x0080
#define EFI_AHCI_PORT_CLB                      0x0000
#define EFI_AHCI_PORT_CLBU                     0x0004
#define EFI_AHCI_PORT_FB                       0x0008
#define EFI_AHCI_PORT_FBU                      0x000C
#define EFI_AHCI_PORT_IS                       0x0010
#define   EFI_AHCI_PORT_IS_DHRS                BIT0
#define   EFI_AHCI_PORT_IS_PSS                 BIT1
#define   EFI_AHCI_PORT_IS_SSS                 BIT2
#define   EFI_AHCI_PORT_IS_SDBS                BIT3
#define   EFI_AHCI_PORT_IS_UFS                 BIT4
#define   EFI_AHCI_PORT_IS_DPS                 BIT5
#define   EFI_AHCI_PORT_IS_PCS                 BIT6
#define   EFI_AHCI_PORT_IS_DIS                 BIT7
#define   EFI_AHCI_PORT_IS_PRCS                BIT22
#define   EFI_AHCI_PORT_IS_IPMS                BIT23
#define   EFI_AHCI_PORT_IS_OFS                 BIT24
#define   EFI_AHCI_PORT_IS_INFS                BIT26
#define   EFI_AHCI_PORT_IS_IFS                 BIT27
#define   EFI_AHCI_PORT_IS_HBDS                BIT28
#define   EFI_AHCI_PORT_IS_HBFS                BIT29
#define   EFI_AHCI_PORT_IS_TFES                BIT30
#define   EFI_AHCI_PORT_IS_CPDS                BIT31
#define   EFI_AHCI_PORT_IS_CLEAR               0xFFFFFFFF
#define   EFI_AHCI_PORT_IS_FIS_CLEAR           0x0000000F

#define EFI_AHCI_PORT_OFFSET(PortNum) \
        (EFI_AHCI_PORT_START + ((PortNum) * EFI_AHCI_PORT_REG_WIDTH))

#define EFI_AHCI_PORT_IE                       0x0014
#define EFI_AHCI_PORT_CMD                      0x0018
#define   EFI_AHCI_PORT_CMD_ST_MASK            0xFFFFFFFE
#define   EFI_AHCI_PORT_CMD_ST                 BIT0
#define   EFI_AHCI_PORT_CMD_SUD                BIT1
#define   EFI_AHCI_PORT_CMD_POD                BIT2
#define   EFI_AHCI_PORT_CMD_CLO                BIT3
#define   EFI_AHCI_PORT_CMD_CR                 BIT15
#define   EFI_AHCI_PORT_CMD_FRE                BIT4
#define   EFI_AHCI_PORT_CMD_FR                 BIT14
#define   EFI_AHCI_PORT_CMD_MASK               ~(EFI_AHCI_PORT_CMD_ST | EFI_AHCI_PORT_CMD_FRE | EFI_AHCI_PORT_CMD_COL)
#define   EFI_AHCI_PORT_CMD_PMA                BIT17
#define   EFI_AHCI_PORT_CMD_HPCP               BIT18
#define   EFI_AHCI_PORT_CMD_MPSP               BIT19
#define   EFI_AHCI_PORT_CMD_CPD                BIT20
#define   EFI_AHCI_PORT_CMD_ESP                BIT21
#define   EFI_AHCI_PORT_CMD_ATAPI              BIT24
#define   EFI_AHCI_PORT_CMD_DLAE               BIT25
#define   EFI_AHCI_PORT_CMD_ALPE               BIT26
#define   EFI_AHCI_PORT_CMD_ASP                BIT27
#define   EFI_AHCI_PORT_CMD_ICC_MASK           (BIT28 | BIT29 | BIT30 | BIT31)
#define   EFI_AHCI_PORT_CMD_ACTIVE             (1 << 28 )
#define EFI_AHCI_PORT_TFD                      0x0020
#define   EFI_AHCI_PORT_TFD_MASK               (BIT7 | BIT3 | BIT0)
#define   EFI_AHCI_PORT_TFD_BSY                BIT7
#define   EFI_AHCI_PORT_TFD_DRQ                BIT3
#define   EFI_AHCI_PORT_TFD_ERR                BIT0
#define   EFI_AHCI_PORT_TFD_ERR_MASK           0x00FF00
#define EFI_AHCI_PORT_SIG                      0x0024
#define EFI_AHCI_PORT_SSTS                     0x0028
#define   EFI_AHCI_PORT_SSTS_DET_MASK          0x000F
#define   EFI_AHCI_PORT_SSTS_DET               0x0001
#define   EFI_AHCI_PORT_SSTS_DET_PCE           0x0003
#define   EFI_AHCI_PORT_SSTS_SPD_MASK          0x00F0
#define EFI_AHCI_PORT_SCTL                     0x002C
#define   EFI_AHCI_PORT_SCTL_DET_MASK          0x000F
#define   EFI_AHCI_PORT_SCTL_MASK              (~EFI_AHCI_PORT_SCTL_DET_MASK)
#define   EFI_AHCI_PORT_SCTL_DET_INIT          0x0001
#define   EFI_AHCI_PORT_SCTL_DET_PHYCOMM       0x0003
#define   EFI_AHCI_PORT_SCTL_SPD_MASK          0x00F0
#define   EFI_AHCI_PORT_SCTL_IPM_MASK          0x0F00
#define   EFI_AHCI_PORT_SCTL_IPM_INIT          0x0300
#define   EFI_AHCI_PORT_SCTL_IPM_PSD           0x0100
#define   EFI_AHCI_PORT_SCTL_IPM_SSD           0x0200
#define EFI_AHCI_PORT_SERR                     0x0030
#define   EFI_AHCI_PORT_SERR_RDIE              BIT0
#define   EFI_AHCI_PORT_SERR_RCE               BIT1
#define   EFI_AHCI_PORT_SERR_TDIE              BIT8
#define   EFI_AHCI_PORT_SERR_PCDIE             BIT9
#define   EFI_AHCI_PORT_SERR_PE                BIT10
#define   EFI_AHCI_PORT_SERR_IE                BIT11
#define   EFI_AHCI_PORT_SERR_PRC               BIT16
#define   EFI_AHCI_PORT_SERR_PIE               BIT17
#define   EFI_AHCI_PORT_SERR_CW                BIT18
#define   EFI_AHCI_PORT_SERR_BDE               BIT19
#define   EFI_AHCI_PORT_SERR_DE                BIT20
#define   EFI_AHCI_PORT_SERR_CRCE              BIT21
#define   EFI_AHCI_PORT_SERR_HE                BIT22
#define   EFI_AHCI_PORT_SERR_LSE               BIT23
#define   EFI_AHCI_PORT_SERR_TSTE              BIT24
#define   EFI_AHCI_PORT_SERR_UFT               BIT25
#define   EFI_AHCI_PORT_SERR_EX                BIT26
#define   EFI_AHCI_PORT_ERR_CLEAR              0xFFFFFFFF
#define EFI_AHCI_PORT_SACT                     0x0034
#define EFI_AHCI_PORT_CI                       0x0038
#define EFI_AHCI_PORT_SNTF                     0x003C

#endif
