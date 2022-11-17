/** @file

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SIO_REG_H_
#define _SIO_REG_H_

typedef struct {
  UINT8 Index;
  UINT8 Value;
} SIO_INDEX_DATA;

#define REG_LOGICAL_DEVICE        0x07
#define ACTIVATE                  0x30

//
// COM (Serial) Port Base address
//
#define SIO_BASE_COM1             0x3F8
#define SIO_BASE_COM2             0x2F8

#define BASE_ADDRESS_HIGH0        0x60
#define BASE_ADDRESS_LOW0         0x61
#define PRIMARY_INTERRUPT_SELECT  0x70
#define INTERRUPT_TYPE            0x71

//
//ASPEED AST2500 register
//
#define ASPEED2500_SIO_INDEX_PORT  0x2E
#define ASPEED2500_SIO_DATA_PORT   (ASPEED2500_SIO_INDEX_PORT+1)

#define ASPEED2500_SIO_UART1       0x02
#define ASPEED2500_SIO_UART2       0x03
#define ASPEED2500_SIO_SMI         0x0D
#define ASPEED2500_SIO_MAILBOX     0x0E

#define SCU7C                      0x1e6e207c

#define ASPEED2500_SIO_UNLOCK      0xA5
#define ASPEED2500_SIO_LOCK        0xAA

//
// Port address for PILOT-IV
//
#define PILOTIV_CHIP_ID         0x03
#define PILOTIV_SIO_INDEX_PORT  0x2E
#define PILOTIV_SIO_DATA_PORT   (PILOTIV_SIO_INDEX_PORT+1)

#define PILOTIV_SIO_UNLOCK      0x5A
#define PILOTIV_SIO_LOCK        0xA5
#define PILOTIV_UNLOCK      0x5A
#define PILOTIV_LOCK        0xA5

#define PILOTIV_SIO_PSR     0x00
#define PILOTIV_SIO_COM2    0x01
#define PILOTIV_SIO_COM1    0x02
#define PILOTIV_SIO_SWCP    0x03
#define PILOTIV_SIO_GPIO    0x04
#define PILOTIV_SIO_WDT     0x05

#define PILOTIV_SIO_KCS3    0x08
#define PILOTIV_SIO_KCS4    0x09
#define PILOTIV_SIO_KCS5    0x0A
#define PILOTIV_SIO_BT      0x0B
#define PILOTIV_SIO_SMIC    0x0C
#define PILOTIV_SIO_MAILBOX 0x0D
#define PILOTIV_SIO_RTC     0x0E
#define PILOTIV_SIO_SPI     0x0F
#define PILOTIV_SIO_TAP     0x10

//
// Register for Pilot IV
//
#define PILOTIV_CHIP_ID_REG               0x20
#define PILOTIV_LOGICAL_DEVICE            REG_LOGICAL_DEVICE
#define PILOTIV_ACTIVATE                  ACTIVATE
#define PILOTIV_BASE_ADDRESS_HIGH0        BASE_ADDRESS_HIGH0
#define PILOTIV_BASE_ADDRESS_LOW0         BASE_ADDRESS_LOW0
#define PILOTIV_BASE_ADDRESS_HIGH1        BASE_ADDRESS_HIGH1
#define PILOTIV_BASE_ADDRESS_LOW1         BASE_ADDRESS_LOW1
#define PILOTIV_PRIMARY_INTERRUPT_SELECT  PRIMARY_INTERRUPT_SELECT


//
// Port address for PC8374
//
#define PC8374_CHIP_ID         0xF1
#define PC8374_SIO_INDEX_PORT  0x02E
#define PC8374_SIO_DATA_PORT   (PC8374_SIO_INDEX_PORT+1)

//
// Logical device in PC8374
//
#define PC8374_SIO_FLOPPY  0x00
#define PC8374_SIO_PARA    0x01
#define PC8374_SIO_COM2    0x02
#define PC8374_SIO_COM1    0x03
#define PC8374_SIO_MOUSE   0x05
#define PC8374_SIO_KYBD    0x06
#define PC8374_SIO_GPIO    0x07

//
// Registers specific for PC8374
//
#define PC8374_CLOCK_SELECT  0x2D
#define PC8374_CLOCK_CONFIG  0x29

//
// Registers for PC8374
//
#define PC8374_LOGICAL_DEVICE            REG_LOGICAL_DEVICE
#define PC8374_ACTIVATE                  ACTIVATE
#define PC8374_BASE_ADDRESS_HIGH0        BASE_ADDRESS_HIGH0
#define PC8374_BASE_ADDRESS_LOW0         BASE_ADDRESS_LOW0
#define PC8374_PRIMARY_INTERRUPT_SELECT  PRIMARY_INTERRUPT_SELECT
#define PC8374_DMA_CHANNEL_SELECT        DMA_CHANNEL_SELECT0
#define PC8374_CHIP_ID_REG               0x20

#define PC87427_SERVERIO_CNF2           0x22



//
// Port address for NCT5104D
//
#define NCT5104D_SIO_INDEX_PORT  0x4E
#define NCT5104D_SIO_DATA_PORT   (NCT5104D_SIO_INDEX_PORT+1)

//
// Registers for NCT5104D
//
#define NCT5104D_CHIP_ID_REG          0x20
#define NCT5104D_CHIP_ID              0xC4
#define NCT5104D_LOGICAL_DEVICE       REG_LOGICAL_DEVICE
#define NCT5104D_ACTIVATE             ACTIVATE
#define NCT5104D_SIO_UARTA            2
#define NCT5104D_SIO_COM1             3
#define NCT5104D_BASE_ADDRESS_HIGH0        BASE_ADDRESS_HIGH0
#define NCT5104D_BASE_ADDRESS_LOW0         BASE_ADDRESS_LOW0
#define NCT5104D_WAKEUP_ON_IRQ_EN     0x70
#define NCT5104D_ENTER_THE_EXTENDED_FUNCTION_MODE       0x87
#define NCT5104D_EXIT_THE_EXTENDED_FUNCTION_MODE        0xAA
//
// Port address for W83527
//
#define W83527_SIO_INDEX_PORT  0x02E
#define W83527_SIO_DATA_PORT   (W83527_SIO_INDEX_PORT+1)

//
// Logical device in W83527
//
#define W83527_SIO_KYBD       0x05
#define W83527_SIO_WDTO       0x08
#define W83527_SIO_GPIO       0x09
#define W83527_SIO_ACPI       0x0A
#define W83527_SIO_HWM        0x0B
#define W83527_SIO_PCEI       0x0C

//
// Registers for W83527
//
#define W83527_EXT_MODE_START                   0x87
#define W83527_EXT_MODE_STOP                    0xAA
#define W83527_LOGICAL_DEVICE                   REG_LOGICAL_DEVICE
#define W83527_ACTIVATE_REG                     0x30
#define W83527_ACTIVATE                         ACTIVATE
#define W83527_CHIP_ID_REG                      0x20
#define W83527_CHIP_ID                          0xB0
#define W83527_CLOCK_REG                        0x24
#define W83527_KBC_BASE1_HI_ADDR_REG            0x60
#define W83527_KBC_BASE1_LO_ADDR_REG            0x61
#define W83527_KBC_BASE2_HI_ADDR_REG            0x62
#define W83527_KBC_BASE2_LO_ADDR_REG            0x63
#define W83527_KBC_BASE1_HI_ADDR                0x00
#define W83527_KBC_BASE1_LO_ADDR                0x60
#define W83527_KBC_BASE2_HI_ADDR                0x00
#define W83527_KBC_BASE2_LO_ADDR                0x64
#define W83527_KBC_KB_IRQ_REG                   0x70
#define W83527_KBC_KB_IRQ                       0x01
#define W83527_KBC_MS_IRQ_REG                   0x72
#define W83527_KBC_MS_IRQ                       0x0C
#define W83527_KBC_CFG_REG                      0xF0
#define W83527_KBC_CFG                          0x83
#define W83527_KBC_CLOCK                        0x01
#define W83527_EXT_MODE_START                   0x87
#define W83527_EXT_MODE_END                     0xAA


//
// Select Clock for W83527, 0 / 1 for 24MHz / 48MHz
//
#define W83527_CLOCK_BIT                          0x06
#define W83527_CLOCK                              0x01

//
// Initialize Key Board Controller
//
#define W83527_KeyBoard   1


//
// Pilot II Mailbox Data Register definitions
//
#define MBDAT00_OFFSET                  0x00
#define MBDAT01_OFFSET                  0x01
#define MBDAT02_OFFSET                  0x02
#define MBDAT03_OFFSET                  0x03
#define MBDAT04_OFFSET                  0x04
#define MBDAT05_OFFSET                  0x05
#define MBDAT06_OFFSET                  0x06
#define MBDAT07_OFFSET                  0x07
#define MBDAT08_OFFSET                  0x08
#define MBDAT09_OFFSET                  0x09
#define MBDAT10_OFFSET                  0x0A
#define MBDAT11_OFFSET                  0x0B
#define MBDAT12_OFFSET                  0x0C
#define MBDAT13_OFFSET                  0x0D
#define MBDAT14_OFFSET                  0x0E
#define MBDAT15_OFFSET                  0x0F
#define MBST0_OFFSET                    0x10
#define MBST1_OFFSET                    0x11

//
// If both are there, use the default one
//

#define  ASPEED_EXIST     BIT4
#define  NCT5104D_EXIST   BIT3
#define  W83527_EXIST     BIT2
#define  PC8374_EXIST     BIT1
#define  PILOTIV_EXIST    BIT0
#define  DEFAULT_SIO      PILOTIV_EXIST
#define  DEFAULT_KDB      PC8374_EXIST

#define IPMI_DEFAULT_SMM_IO_BASE  0xca2
#define PILOTIV_SWC_BASE_ADDRESS       0xA00
#define PILOTIV_PM1b_EVT_BLK_BASE_ADDRESS 0x0A80
#define PILOTIV_PM1b_CNT_BLK_BASE_ADDRESS 0x0A84
#define PILOTIV_GPE1_BLK_BASE_ADDRESS     0x0A86
#define PILOTIV_KCS3_DATA_BASE_ADDRESS    0x0CA4
#define PILOTIV_KCS3_CMD_BASE_ADDRESS     0x0CA5
#define PILOTIV_KCS4_DATA_BASE_ADDRESS    0x0CA2
#define PILOTIV_KCS4_CMD_BASE_ADDRESS     0x0CA3
#define PILOTIV_MAILBOX_BASE_ADDRESS      0x0600
#define PILOTIV_MAILBOX_MASK              0xFFE0
#endif
