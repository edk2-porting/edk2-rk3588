/** @file
  Register names for PCH Serial IO Controllers

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

#ifndef _PCH_REGS_SERIAL_IO_
#define _PCH_REGS_SERIAL_IO_

//
// Serial IO Controllers General PCI Configuration Registers
// registers accessed using PciD21FxRegBase + offset
//
#define R_SERIAL_IO_CFG_BAR0_LOW                        0x10
#define B_SERIAL_IO_CFG_BAR0_LOW_BAR                    0xFFFFF000
#define R_SERIAL_IO_CFG_BAR0_HIGH                       0x14
#define R_SERIAL_IO_CFG_BAR1_LOW                        0x18
#define B_SERIAL_IO_CFG_BAR1_LOW_BAR                    0xFFFFF000
#define R_SERIAL_IO_CFG_BAR1_HIGH                       0x1C
#define V_SERIAL_IO_CFG_BAR_SIZE                        (4 * 1024)
#define N_SERIAL_IO_CFG_BAR_ALIGNMENT                   12

#define R_SERIAL_IO_CFG_PME_CTRL_STS                    0x84
#define B_SERIAL_IO_CFG_PME_CTRL_STS_PWR_ST             (BIT1| BIT0)

#define R_SERIAL_IO_CFG_D0I3MAXDEVPG                    0xA0
#define B_SERIAL_IO_CFG_D0I3MAXDEVPG_PMCRE              BIT16
#define B_SERIAL_IO_CFG_D0I3MAXDEVPG_I3E                BIT17
#define B_SERIAL_IO_CFG_D0I3MAXDEVPG_PGE                BIT18

#define R_SERIAL_IO_CFG_INTERRUPTREG                    0x3C
#define B_SERIAL_IO_CFG_INTERRUPTREG_INTLINE            0x000000FF

//
// Serial IO Controllers MMIO Registers
// registers accessed : BAR0 + offset
//
#define R_SERIAL_IO_MEM_SSCR1                           0x4
#define B_SERIAL_IO_MEM_SSCR1_IFS                       BIT16

#define R_SERIAL_IO_MEM_PPR_CLK                         0x200
#define B_SERIAL_IO_MEM_PPR_CLK_EN                      BIT0
#define B_SERIAL_IO_MEM_PPR_CLK_UPDATE                  BIT31
#define V_SERIAL_IO_MEM_PPR_CLK_M_DIV                   0x30
#define V_SERIAL_IO_MEM_PPR_CLK_N_DIV                   0xC35

#define R_SERIAL_IO_MEM_PPR_RESETS                      0x204
#define B_SERIAL_IO_MEM_PPR_RESETS_FUNC                 BIT0
#define B_SERIAL_IO_MEM_PPR_RESETS_APB                  BIT1
#define B_SERIAL_IO_MEM_PPR_RESETS_IDMA                 BIT2

#define R_SERIAL_IO_MEM_ACTIVE_LTR                      0x210
#define R_SERIAL_IO_MEM_IDLE_LTR                        0x214
#define B_SERIAL_IO_MEM_LTR_SNOOP_VALUE                 0x000003FF
#define B_SERIAL_IO_MEM_LTR_SNOOP_SCALE                 0x00001C00
#define B_SERIAL_IO_MEM_LTR_SNOOP_REQUIREMENT           BIT15

#define R_SERIAL_IO_MEM_SPI_CS_CONTROL                  0x224
#define B_SERIAL_IO_MEM_SPI_CS_CONTROL_STATE            BIT1
#define B_SERIAL_IO_MEM_SPI_CS_CONTROL_MODE             BIT0

#define R_SERIAL_IO_MEM_REMAP_ADR_LOW                   0x240
#define R_SERIAL_IO_MEM_REMAP_ADR_HIGH                  0x244

#define R_SERIAL_IO_MEM_I2C_SDA_HOLD                    0x7C
#define V_SERIAL_IO_MEM_I2C_SDA_HOLD_VALUE              0x002C002C

//
// I2C Controller
// Registers accessed through BAR0 + offset
//
#define    R_IC_CON                            0x00 // I2c Control
#define    B_IC_MASTER_MODE                    BIT0
#define    B_IC_RESTART_EN                     BIT5
#define    B_IC_SLAVE_DISABLE                  BIT6
#define    V_IC_SPEED_STANDARD                 0x02
#define    V_IC_SPEED_FAST                     0x04
#define    V_IC_SPEED_HIGH                     0x06

#define    R_IC_TAR                            0x04 // I2c Target Address
#define    B_IC_TAR_10BITADDR_MASTER           BIT12

#define    R_IC_DATA_CMD                       0x10  // I2c Rx/Tx Data Buffer and Command
#define    B_IC_CMD_READ                       BIT8    // 1 = read, 0 = write
#define    B_IC_CMD_STOP                       BIT9    // 1 = STOP
#define    B_IC_CMD_RESTART                    BIT10   // 1 = IC_RESTART_EN
#define    V_IC_WRITE_CMD_MASK                 0xFF

#define    R_IC_SS_SCL_HCNT                    0x14 // Standard Speed I2c Clock SCL High Count
#define    R_IC_SS_SCL_LCNT                    0x18 // Standard Speed I2c Clock SCL Low Count
#define    R_IC_FS_SCL_HCNT                    0x1C // Full Speed I2c Clock SCL High Count
#define    R_IC_FS_SCL_LCNT                    0x20 // Full Speed I2c Clock SCL Low Count
#define    R_IC_HS_SCL_HCNT                    0x24 // High Speed I2c Clock SCL High Count
#define    R_IC_HS_SCL_LCNT                    0x28 // High Speed I2c Clock SCL Low Count
#define    R_IC_INTR_STAT                      0x2C // I2c Inetrrupt Status
#define    R_IC_INTR_MASK                      0x30 // I2c Interrupt Mask
#define    B_IC_INTR_GEN_CALL                  BIT11  // General call received
#define    B_IC_INTR_START_DET                 BIT10
#define    B_IC_INTR_STOP_DET                  BIT9
#define    B_IC_INTR_ACTIVITY                  BIT8
#define    B_IC_INTR_TX_ABRT                   BIT6   // Set on NACK
#define    B_IC_INTR_TX_EMPTY                  BIT4
#define    B_IC_INTR_TX_OVER                   BIT3
#define    B_IC_INTR_RX_FULL                   BIT2   // Data bytes in RX FIFO over threshold
#define    B_IC_INTR_RX_OVER                   BIT1
#define    B_IC_INTR_RX_UNDER                  BIT0
#define    R_IC_RAW_INTR_STAT                ( 0x34) // I2c Raw Interrupt Status
#define    R_IC_RX_TL                        ( 0x38) // I2c Receive FIFO Threshold
#define    R_IC_TX_TL                        ( 0x3C) // I2c Transmit FIFO Threshold
#define    R_IC_CLR_INTR                     ( 0x40) // Clear Combined and Individual Interrupts
#define    R_IC_CLR_RX_UNDER                 ( 0x44) // Clear RX_UNDER Interrupt
#define    R_IC_CLR_RX_OVER                  ( 0x48) // Clear RX_OVERinterrupt
#define    R_IC_CLR_TX_OVER                  ( 0x4C) // Clear TX_OVER interrupt
#define    R_IC_CLR_RD_REQ                   ( 0x50) // Clear RD_REQ interrupt
#define    R_IC_CLR_TX_ABRT                  ( 0x54) // Clear TX_ABRT interrupt
#define    R_IC_CLR_RX_DONE                  ( 0x58) // Clear RX_DONE interrupt
#define    R_IC_CLR_ACTIVITY                 ( 0x5C) // Clear ACTIVITY interrupt
#define    R_IC_CLR_STOP_DET                 ( 0x60) // Clear STOP_DET interrupt
#define    R_IC_CLR_START_DET                ( 0x64) // Clear START_DET interrupt
#define    R_IC_CLR_GEN_CALL                 ( 0x68) // Clear GEN_CALL interrupt
#define    R_IC_ENABLE                       ( 0x6C) // I2c Enable

#define    R_IC_STATUS                         0x70  // I2c Status
#define    B_IC_STATUS_RFF                     BIT4   // RX FIFO is completely full
#define    B_IC_STATUS_RFNE                    BIT3   // RX FIFO is not empty
#define    B_IC_STATUS_TFE                     BIT2   // TX FIFO is completely empty
#define    B_IC_STATUS_TFNF                    BIT1   // TX FIFO is not full
#define    B_IC_STATUS_ACTIVITY                BIT0   // Controller Activity Status.

#define    R_IC_TXFL R                       ( 0x74) // Transmit FIFO Level Register
#define    R_IC_RXFLR                        ( 0x78) // Receive FIFO Level Register
#define    R_IC_SDA_HOLD                     ( 0x7C)
#define    R_IC_TX_ABRT_SOURCE               ( 0x80) // I2c Transmit Abort Status Register
#define    B_IC_TX_ABRT_7B_ADDR_NACK          BIT0 // NACK on 7-bit address

#define    R_IC_SDA_SETUP                    ( 0x94) // I2c SDA Setup Register
#define    R_IC_ACK_GENERAL_CALL             ( 0x98) // I2c ACK General Call Register
#define    R_IC_ENABLE_STATUS                ( 0x9C) // I2c Enable Status Register
#define    B_IC_EN                            BIT0   // I2c enable status

#define    R_IC_CLK_GATE                     ( 0xC0)
#define    R_IC_COMP_PARAM                   ( 0xF4) // Component Parameter Register
#define    R_IC_COMP_VERSION                 ( 0xF8) // Component Version ID
#define    R_IC_COMP_TYPE                    ( 0xFC) // Component Type

//
// Bridge Private Configuration Registers
// accessed only through SB messaging. SB access = SerialIo IOSF2OCP Bridge Port ID + offset
//
#define R_SERIAL_IO_PCR_PMCTL                        0x1D0
#define V_SERIAL_IO_PCR_PMCTL_PWR_GATING             0x3F

#define R_SERIAL_IO_PCR_PCICFGCTRLx                 0x200
#define V_SERIAL_IO_PCR_PCICFGCTRL_N_OFFS           0x04
#define R_SERIAL_IO_PCR_PCICFGCTRL1                 0x200 //I2C0
#define R_SERIAL_IO_PCR_PCICFGCTRL2                 0x204 //I2C1
#define R_SERIAL_IO_PCR_PCICFGCTRL3                 0x208 //I2C2
#define R_SERIAL_IO_PCR_PCICFGCTRL4                 0x20C //I2C3
#define R_SERIAL_IO_PCR_PCICFGCTRL5                 0x210 //I2C4
#define R_SERIAL_IO_PCR_PCICFGCTRL6                 0x214 //I2C5
#define R_SERIAL_IO_PCR_PCICFGCTRL9                 0x218 //UA00
#define R_SERIAL_IO_PCR_PCICFGCTRL10                0x21C //UA01
#define R_SERIAL_IO_PCR_PCICFGCTRL11                0x220 //UA02
#define R_SERIAL_IO_PCR_PCICFGCTRL13                0x224 //SPI0
#define R_SERIAL_IO_PCR_PCICFGCTRL14                0x228 //SPI1

#define B_SERIAL_IO_PCR_PCICFGCTRL_PCI_CFG_DIS      BIT0
#define B_SERIAL_IO_PCR_PCICFGCTRL_ACPI_INTR_EN     BIT1
#define B_SERIAL_IO_PCR_PCICFGCTRL_BAR1_DIS         BIT7
#define B_SERIAL_IO_PCR_PCICFGCTRL_INT_PIN          (BIT11 | BIT10 | BIT9 | BIT8)
#define N_SERIAL_IO_PCR_PCICFGCTRL_INT_PIN          8
#define V_SERIAL_IO_PCR_PCICFGCTRL_INTA             0x01
#define V_SERIAL_IO_PCR_PCICFGCTRL_INTB             0x02
#define V_SERIAL_IO_PCR_PCICFGCTRL_INTC             0x03
#define V_SERIAL_IO_PCR_PCICFGCTRL_INTD             0x04
#define B_SERIAL_IO_PCR_PCICFGCTRL_ACPI_IRQ         0x000FF000
#define N_SERIAL_IO_PCR_PCICFGCTRL_ACPI_IRQ         12
#define B_SERIAL_IO_PCR_PCICFGCTRL_PCI_IRQ          0x0FF00000
#define N_SERIAL_IO_PCR_PCICFGCTRL_PCI_IRQ          20

#define R_SERIAL_IO_PCR_GPPRVRW2                            0x604
#define B_SERIAL_IO_PCR_GPPRVRW2_PGCB_FRC_CLK_CP_EN         BIT1
#define B_SERIAL_IO_PCR_GPPRVRW2_CDC_SIDE_CFG_CG_EN         BIT5
#define B_SERIAL_IO_PCR_GPPRVRW2_CDC_SIDE_CFG_CLKREQ_CTL_EN BIT11
#define V_SERIAL_IO_PCR_GPPRVRW2_CLK_GATING                 (B_SERIAL_IO_PCR_GPPRVRW2_PGCB_FRC_CLK_CP_EN | B_SERIAL_IO_PCR_GPPRVRW2_CDC_SIDE_CFG_CG_EN | B_SERIAL_IO_PCR_GPPRVRW2_CDC_SIDE_CFG_CLKREQ_CTL_EN)


#define R_SERIAL_IO_PCR_GPPRVRW7                    0x618
#define B_SERIAL_IO_PCR_GPPRVRW7_UART0_BYTE_ADDR_EN BIT0
#define B_SERIAL_IO_PCR_GPPRVRW7_UART1_BYTE_ADDR_EN BIT1
#define B_SERIAL_IO_PCR_GPPRVRW7_UART2_BYTE_ADDR_EN BIT2

//
// Number of pins used by SerialIo controllers
//
#define PCH_SERIAL_IO_PINS_PER_I2C_CONTROLLER               2
#define PCH_SERIAL_IO_PINS_PER_UART_CONTROLLER              4
#define PCH_SERIAL_IO_PINS_PER_UART_CONTROLLER_NO_FLOW_CTRL 2
#define PCH_SERIAL_IO_PINS_PER_SPI_CONTROLLER               4

#endif
