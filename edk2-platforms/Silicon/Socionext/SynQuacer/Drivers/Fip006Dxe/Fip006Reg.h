/** @file
  Socionext FIP006 Register List

  Copyright (c) 2017, Socionext Inc. All rights reserved.<BR>
  Copyright (c) 2017, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __EFI_FIP006_REG_H__
#define __EFI_FIP006_REG_H__

#define FIP006_REG_MCTRL      0x00    // Module Control
typedef union {
  UINT32          Raw       : 32;
  struct {
    BOOLEAN       MEN       : 1;
    BOOLEAN       CSEN      : 1;
#define MCTRL_CSEN_DIRECT     0
#define MCTRL_CSEN_CS         1
    BOOLEAN       CDSS      : 1;
#define MCTRL_CDSS_IHCLK      0
#define MCTRL_CDSS_IPCLK      1
    BOOLEAN       MES       : 1;
#define MCTRL_MES_READY       1
    UINT8                   : 4;
    UINT8                   : 8;
    UINT8                   : 8;
    UINT8                   : 8;
  };
} FIP006_MCTRL;

#define FIP006_REG_PCC0       0x04    // Peripheral Communication Control 0
#define FIP006_REG_PCC1       0x08    // Peripheral Communication Control 1
#define FIP006_REG_PCC2       0x0C    // Peripheral Communication Control 2
#define FIP006_REG_PCC3       0x10    // Peripheral Communication Control 3
typedef union {
  UINT32          Raw       : 32;
  struct {
    BOOLEAN       CPHA      : 1;
    BOOLEAN       CPOL      : 1;
    BOOLEAN       ACES      : 1;
    BOOLEAN       RTM       : 1;
    BOOLEAN       SSPOL     : 1;
    UINT8         SS2CD     : 2;
    BOOLEAN       SDIR      : 1;
#define PCC_SDIR_MS_BIT       0
#define PCC_SDIR_LS_BIT       1
    BOOLEAN       SENDIAN   : 1;
#define PCC_SENDIAN_BIG       0
#define PCC_SENDIAN_LITTLE    1
    UINT8         CDRS      : 7;
    BOOLEAN       SAFESYNC  : 1;
    UINT8         WRDSEL    : 4;
    UINT8         RDDSEL    : 2;
    UINT8                   : 1;
    UINT8                   : 8;
  } Reg;
} FIP006_PCC;
typedef FIP006_PCC          FIP006_PCC0, FIP006_PCC1, FIP006_PCC2, FIP006_PCC3;

#define FIP006_REG_TXF        0x14    // Tx Interrupt Flag
#define TXF_TSSRS             BIT6
#define TXF_TFMTS             BIT5
#define TXF_TFLETS            BIT4
#define TXF_TFUS              BIT3
#define TXF_TFOS              BIT2
#define TXF_TFES              BIT1
#define TXF_TFFS              BIT0

#define FIP006_REG_TXE        0x18    // Tx Interrupt Enable
#define TXE_TSSRE             BIT6
#define TXE_TFMTE             BIT5
#define TXE_TFLETE            BIT4
#define TXE_TFUE              BIT3
#define TXE_TFOE              BIT2
#define TXE_TFEE              BIT1
#define TXE_TFFE              BIT0

#define FIP006_REG_TXC        0x1C    // Tx Interrupt Clear
#define TXC_TSSRC             BIT6
#define TXC_TFMTC             BIT5
#define TXC_TFLETC            BIT4
#define TXC_TFUC              BIT3
#define TXC_TFOC              BIT2
#define TXC_TFEC              BIT1
#define TXC_TFFC              BIT0

#define FIP006_REG_RXF        0x20    // Rx Interrupt Flag
#define RXF_RSSRS             BIT6
#define RXF_RFMTS             BIT5
#define RXF_RFLETS            BIT4
#define RXF_RFUS              BIT3
#define RXF_RFOS              BIT2
#define RXF_RFES              BIT1
#define RXF_RFFS              BIT0

#define FIP006_REG_RXE        0x24    // Rx Interrupt Enable
#define RXE_RSSRE             BIT6
#define RXE_RFMTE             BIT5
#define RXE_RFLETE            BIT4
#define RXE_RFUE              BIT3
#define RXE_RFOE              BIT2
#define RXE_RFEE              BIT1
#define RXE_RFFE              BIT0

#define FIP006_REG_RXC        0x28    // Rx Interrupt Clear
#define RXC_RSSRC             BIT6
#define RXC_RFMTC             BIT5
#define RXC_RFLETC            BIT4
#define RXC_RFUC              BIT3
#define RXC_RFOC              BIT2
#define RXC_RFEC              BIT1
#define RXC_RFFC              BIT0

#define FIP006_REG_FAULTF     0x2C    // Error Interrupt Status
#define FAULTF_DRCBSFS        BIT4
#define FAULTF_DWCBSFS        BIT3
#define FAULTF_PVFS           BIT2
#define FAULTF_WAFS           BIT1
#define FAULTF_UMAFS          BIT0

#define FIP006_REG_FAULTC     0x30    // Error Interrupt Clear
#define FAULTC_DRCBSFC        BIT4
#define FAULTC_DWCBSFC        BIT3
#define FAULTC_PVFC           BIT2
#define FAULTC_WAFC           BIT1
#define FAULTC_UMAFC          BIT0

#define FIP006_REG_DM_CFG     0x34    // Direct Mode DMA Configuration
#define DM_CFG_MSTARTEN       BIT2
#define DM_CFG_SSDC           BIT1

#define FIP006_REG_DM_DMA     0x35    // Direct Mode DMA Enable
#define DM_DMA_TXDMAEN        BIT1
#define DM_DMA_RXDMAEN        BIT0

#define FIP006_REG_DM_START   0x38    // Direct Mode Start Transmission
#define DM_START              BIT0
#define FIP006_REG_DM_STOP    0x39    // Direct Mode Stop Transmission
#define DM_STOP               BIT0

#define FIP006_REG_DM_PSEL    0x3A    // Direct Mode Peripheral Select
#define DM_PSEL               (BIT1 | BIT0)

#define FIP006_REG_DM_TRP     0x3B    // Direct Mode Transmission Protocol
#define DM_TRP                (BIT3 | BIT2 | BIT1 | BIT0)

#define FIP006_REG_DM_BCC     0x3C    // Direct Mode Byte Count Control
#define FIP006_REG_DM_BCS     0x3E    // Direct Mode Byte Count Status

#define FIP006_REG_DM_STATUS  0x40    // Direct Mode Status
#define DM_STATUS_TXFLEVEL    (BIT20 | BIT19 | BIT18 | BIT17 | BIT16)
#define DM_STATUS_RXFLEVEL    (BIT20 | BIT19 | BIT18 | BIT17 | BIT16)
#define DM_STATUS_TXACTIVE    BIT1
#define DM_STATUS_RXACTIVE    BIT0

#define FIP006_REG_FIFO_CFG   0x4C    // FIFO Configuration
#define FIFO_CFG_TXFLSH       BIT12
#define FIFO_CFG_RXFLSH       BIT11
#define FIFO_CFG_TXCTRL       BIT10
#define FIFO_CFG_FWIDTH       (BIT9 | BIT8)
#define FIFO_CFG_TXFTH        (BIT7 | BIT6 | BIT5 | BIT4)
#define FIFO_CFG_RXFTH        (BIT3 | BIT2 | BIT1 | BIT0)

#define FIP006_REG_FIFO_TX    0x50    // 16 32-bit Tx FIFO
#define FIP006_REG_FIFO_RX    0x90    // 16 32-bit Rx FIFO

#define FIP006_REG_CS_CFG     0xD0    // Command Sequencer Configuration
typedef union {
  UINT32          Raw       : 32;
  struct {
    BOOLEAN       SRAM      : 1;
#define CS_CFG_SRAM_RO        0
#define CS_CFG_SRAM_RW        1
    UINT8         MBM       : 2;
#define CS_CFG_MBM_SINGLE     0
#define CS_CFG_MBM_DUAL       1
#define CS_CFG_MBM_QUAD       2
    BOOLEAN       SPICHNG   : 1;
    BOOLEAN       BOOTEN    : 1;
    BOOLEAN       BSEL      : 1;
    UINT8                   : 2;
    BOOLEAN       SSEL0EN   : 1;
    BOOLEAN       SSEL1EN   : 1;
    BOOLEAN       SSEL2EN   : 1;
    BOOLEAN       SSEL3EN   : 1;
    UINT8                   : 4;
    BOOLEAN       MSEL      : 4;
    UINT8                   : 4;
    UINT8                   : 8;
  } Reg;
} FIP006_CS_CFG;

#define FIP006_REG_CS_ITIME   0xD4    // Command Sequencer Idle Timer
typedef union {
  UINT32          Raw       : 32;
  struct {
    UINT16        ITIME     : 16;
    UINT16                  : 16;
  } Reg;
} FIP006_CS_ITIME;
#define FIP006_REG_CS_AEXT    0xD8    // Command Sequencer Address Extension
typedef union {
  UINT32          Raw       : 32;
  struct {
    UINT16                  : 13;
    UINT32        AEXT      : 19;
  } Reg;
} FIP006_CS_AEXT;

#define FIP006_REG_CS_RD      0xDC    // Command Sequencer Read Control
#define CS_RD_DEPTH           8
#define FIP006_REG_CS_WR      0xEC    // Command Sequencer Write Control
#define CS_WR_DEPTH           8
typedef union {
  UINT16          Raw       : 16;
  struct {
    BOOLEAN       DEC       : 1;
    UINT8         TRP       : 2;
    BOOLEAN       CONT      : 1;
    UINT8                   : 4;
    union {
      UINT8       Data      : 8;
      struct {
        UINT8     Data      : 3;
        UINT8               : 5;
      } Cmd;
    } Payload;
  } Reg;
} FIP006_CS_CMD;
typedef FIP006_CS_CMD       FIP006_CS_RD, FIP006_CS_WR;

#define FIP006_REG_MID        0xFC    // Command Sequencer Module ID
typedef UINT32    FIP006_MID;

#endif
