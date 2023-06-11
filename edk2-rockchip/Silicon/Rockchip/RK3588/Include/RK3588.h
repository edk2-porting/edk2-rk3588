#ifndef __RK3588_H
#define __RK3588_H
#ifdef __cplusplus
  extern "C" {
#endif
/****************************************************************************************/
/*                                                                                      */
/*                               Module Structure Section                               */
/*                                                                                      */
/****************************************************************************************/
#ifndef __ASSEMBLY__
/* DCACHE Register Structure Define */
struct DCACHE_REG {
    __IO uint32_t CACHE_CTRL;                         /* Address Offset: 0x0000 */
    __IO uint32_t CACHE_MAINTAIN[2];                  /* Address Offset: 0x0004 */
    __IO uint32_t STB_TIMEOUT_CTRL;                   /* Address Offset: 0x000C */
         uint32_t RESERVED0010[4];                    /* Address Offset: 0x0010 */
    __IO uint32_t CACHE_INT_EN;                       /* Address Offset: 0x0020 */
    __IO uint32_t CACHE_INT_ST;                       /* Address Offset: 0x0024 */
    __IO uint32_t CACHE_ERR_HADDR;                    /* Address Offset: 0x0028 */
         uint32_t RESERVED002C;                       /* Address Offset: 0x002C */
    __I  uint32_t CACHE_STATUS;                       /* Address Offset: 0x0030 */
         uint32_t RESERVED0034[3];                    /* Address Offset: 0x0034 */
    __I  uint32_t PMU_RD_NUM_CNT;                     /* Address Offset: 0x0040 */
    __I  uint32_t PMU_WR_NUM_CNT;                     /* Address Offset: 0x0044 */
    __I  uint32_t PMU_SRAM_RD_HIT_CNT;                /* Address Offset: 0x0048 */
    __I  uint32_t PMU_HB_RD_HIT_CNT;                  /* Address Offset: 0x004C */
    __IO uint32_t PMU_STB_RD_HIT_CNT;                 /* Address Offset: 0x0050 */
    __I  uint32_t PMU_RD_HIT_CNT;                     /* Address Offset: 0x0054 */
    __I  uint32_t PMU_WR_HIT_CNT;                     /* Address Offset: 0x0058 */
    __I  uint32_t PMU_RD_MISS_PENALTY_CNT;            /* Address Offset: 0x005C */
    __I  uint32_t PMU_WR_MISS_PENALTY_CNT;            /* Address Offset: 0x0060 */
    __I  uint32_t PMU_RD_LAT_CNT;                     /* Address Offset: 0x0064 */
    __I  uint32_t PMU_WR_LAT_CNT;                     /* Address Offset: 0x0068 */
         uint32_t RESERVED006C[33];                   /* Address Offset: 0x006C */
    __IO uint32_t REVISION;                           /* Address Offset: 0x00F0 */
};
/* ICACHE Register Structure Define */
struct ICACHE_REG {
    __IO uint32_t CACHE_CTRL;                         /* Address Offset: 0x0000 */
    __IO uint32_t CACHE_MAINTAIN[2];                  /* Address Offset: 0x0004 */
    __IO uint32_t STB_TIMEOUT_CTRL;                   /* Address Offset: 0x000C */
         uint32_t RESERVED0010[4];                    /* Address Offset: 0x0010 */
    __IO uint32_t CACHE_INT_EN;                       /* Address Offset: 0x0020 */
    __IO uint32_t CACHE_INT_ST;                       /* Address Offset: 0x0024 */
    __IO uint32_t CACHE_ERR_HADDR;                    /* Address Offset: 0x0028 */
         uint32_t RESERVED002C;                       /* Address Offset: 0x002C */
    __I  uint32_t CACHE_STATUS;                       /* Address Offset: 0x0030 */
         uint32_t RESERVED0034[3];                    /* Address Offset: 0x0034 */
    __I  uint32_t PMU_RD_NUM_CNT;                     /* Address Offset: 0x0040 */
    __I  uint32_t PMU_WR_NUM_CNT;                     /* Address Offset: 0x0044 */
    __I  uint32_t PMU_SRAM_RD_HIT_CNT;                /* Address Offset: 0x0048 */
    __I  uint32_t PMU_HB_RD_HIT_CNT;                  /* Address Offset: 0x004C */
    __IO uint32_t PMU_STB_RD_HIT_CNT;                 /* Address Offset: 0x0050 */
    __I  uint32_t PMU_RD_HIT_CNT;                     /* Address Offset: 0x0054 */
    __I  uint32_t PMU_WR_HIT_CNT;                     /* Address Offset: 0x0058 */
    __I  uint32_t PMU_RD_MISS_PENALTY_CNT;            /* Address Offset: 0x005C */
    __I  uint32_t PMU_WR_MISS_PENALTY_CNT;            /* Address Offset: 0x0060 */
    __I  uint32_t PMU_RD_LAT_CNT;                     /* Address Offset: 0x0064 */
    __I  uint32_t PMU_WR_LAT_CNT;                     /* Address Offset: 0x0068 */
         uint32_t RESERVED006C[33];                   /* Address Offset: 0x006C */
    __IO uint32_t REVISION;                           /* Address Offset: 0x00F0 */
};
/* PMU1_IOC Register Structure Define */
struct PMU1_IOC_REG {
    __IO uint32_t GPIO0A_IOMUX_SEL_L;                 /* Address Offset: 0x0000 */
    __IO uint32_t GPIO0A_IOMUX_SEL_H;                 /* Address Offset: 0x0004 */
    __IO uint32_t GPIO0B_IOMUX_SEL_L;                 /* Address Offset: 0x0008 */
         uint32_t RESERVED000C;                       /* Address Offset: 0x000C */
    __IO uint32_t GPIO0A_DS_L;                        /* Address Offset: 0x0010 */
    __IO uint32_t GPIO0A_DS_H;                        /* Address Offset: 0x0014 */
    __IO uint32_t GPIO0B_DS_L;                        /* Address Offset: 0x0018 */
         uint32_t RESERVED001C;                       /* Address Offset: 0x001C */
    __IO uint32_t GPIO0A_P;                           /* Address Offset: 0x0020 */
    __IO uint32_t GPIO0B_P;                           /* Address Offset: 0x0024 */
    __IO uint32_t GPIO0A_IE;                          /* Address Offset: 0x0028 */
    __IO uint32_t GPIO0B_IE;                          /* Address Offset: 0x002C */
    __IO uint32_t GPIO0A_SMT;                         /* Address Offset: 0x0030 */
    __IO uint32_t GPIO0B_SMT;                         /* Address Offset: 0x0034 */
    __IO uint32_t GPIO0A_PDIS;                        /* Address Offset: 0x0038 */
    __IO uint32_t GPIO0B_PDIS;                        /* Address Offset: 0x003C */
    __IO uint32_t XIN_CON;                            /* Address Offset: 0x0040 */
};
/* PMU2_IOC Register Structure Define */
struct PMU2_IOC_REG {
    __IO uint32_t GPIO0B_IOMUX_SEL_H;                 /* Address Offset: 0x0000 */
    __IO uint32_t GPIO0C_IOMUX_SEL_L;                 /* Address Offset: 0x0004 */
    __IO uint32_t GPIO0C_IOMUX_SEL_H;                 /* Address Offset: 0x0008 */
    __IO uint32_t GPIO0D_IOMUX_SEL_L;                 /* Address Offset: 0x000C */
    __IO uint32_t GPIO0D_IOMUX_SEL_H;                 /* Address Offset: 0x0010 */
    __IO uint32_t GPIO0B_DS_H;                        /* Address Offset: 0x0014 */
    __IO uint32_t GPIO0C_DS_L;                        /* Address Offset: 0x0018 */
    __IO uint32_t GPIO0C_DS_H;                        /* Address Offset: 0x001C */
    __IO uint32_t GPIO0D_DS_L;                        /* Address Offset: 0x0020 */
    __IO uint32_t GPIO0D_DS_H;                        /* Address Offset: 0x0024 */
    __IO uint32_t GPIO0B_P;                           /* Address Offset: 0x0028 */
    __IO uint32_t GPIO0C_P;                           /* Address Offset: 0x002C */
    __IO uint32_t GPIO0D_P;                           /* Address Offset: 0x0030 */
    __IO uint32_t GPIO0B_IE;                          /* Address Offset: 0x0034 */
    __IO uint32_t GPIO0C_IE;                          /* Address Offset: 0x0038 */
    __IO uint32_t GPIO0D_IE;                          /* Address Offset: 0x003C */
    __IO uint32_t GPIO0B_SMT;                         /* Address Offset: 0x0040 */
    __IO uint32_t GPIO0C_SMT;                         /* Address Offset: 0x0044 */
    __IO uint32_t GPIO0D_SMT;                         /* Address Offset: 0x0048 */
    __IO uint32_t GPIO0B_PDIS;                        /* Address Offset: 0x004C */
    __IO uint32_t GPIO0C_PDIS;                        /* Address Offset: 0x0050 */
    __IO uint32_t GPIO0D_PDIS;                        /* Address Offset: 0x0054 */
};
/* BUS_IOC Register Structure Define */
struct BUS_IOC_REG {
         uint32_t RESERVED0000[3];                    /* Address Offset: 0x0000 */
    __IO uint32_t GPIO0B_IOMUX_SEL_H;                 /* Address Offset: 0x000C */
    __IO uint32_t GPIO0C_IOMUX_SEL_L;                 /* Address Offset: 0x0010 */
    __IO uint32_t GPIO0C_IOMUX_SEL_H;                 /* Address Offset: 0x0014 */
    __IO uint32_t GPIO0D_IOMUX_SEL_L;                 /* Address Offset: 0x0018 */
    __IO uint32_t GPIO0D_IOMUX_SEL_H;                 /* Address Offset: 0x001C */
    __IO uint32_t GPIO1A_IOMUX_SEL_L;                 /* Address Offset: 0x0020 */
    __IO uint32_t GPIO1A_IOMUX_SEL_H;                 /* Address Offset: 0x0024 */
    __IO uint32_t GPIO1B_IOMUX_SEL_L;                 /* Address Offset: 0x0028 */
    __IO uint32_t GPIO1B_IOMUX_SEL_H;                 /* Address Offset: 0x002C */
    __IO uint32_t GPIO1C_IOMUX_SEL_L;                 /* Address Offset: 0x0030 */
    __IO uint32_t GPIO1C_IOMUX_SEL_H;                 /* Address Offset: 0x0034 */
    __IO uint32_t GPIO1D_IOMUX_SEL_L;                 /* Address Offset: 0x0038 */
    __IO uint32_t GPIO1D_IOMUX_SEL_H;                 /* Address Offset: 0x003C */
    __IO uint32_t GPIO2A_IOMUX_SEL_L;                 /* Address Offset: 0x0040 */
    __IO uint32_t GPIO2A_IOMUX_SEL_H;                 /* Address Offset: 0x0044 */
    __IO uint32_t GPIO2B_IOMUX_SEL_L;                 /* Address Offset: 0x0048 */
    __IO uint32_t GPIO2B_IOMUX_SEL_H;                 /* Address Offset: 0x004C */
    __IO uint32_t GPIO2C_IOMUX_SEL_L;                 /* Address Offset: 0x0050 */
    __IO uint32_t GPIO2C_IOMUX_SEL_H;                 /* Address Offset: 0x0054 */
    __IO uint32_t GPIO2D_IOMUX_SEL_L;                 /* Address Offset: 0x0058 */
    __IO uint32_t GPIO2D_IOMUX_SEL_H;                 /* Address Offset: 0x005C */
    __IO uint32_t GPIO3A_IOMUX_SEL_L;                 /* Address Offset: 0x0060 */
    __IO uint32_t GPIO3A_IOMUX_SEL_H;                 /* Address Offset: 0x0064 */
    __IO uint32_t GPIO3B_IOMUX_SEL_L;                 /* Address Offset: 0x0068 */
    __IO uint32_t GPIO3B_IOMUX_SEL_H;                 /* Address Offset: 0x006C */
    __IO uint32_t GPIO3C_IOMUX_SEL_L;                 /* Address Offset: 0x0070 */
    __IO uint32_t GPIO3C_IOMUX_SEL_H;                 /* Address Offset: 0x0074 */
    __IO uint32_t GPIO3D_IOMUX_SEL_L;                 /* Address Offset: 0x0078 */
    __IO uint32_t GPIO3D_IOMUX_SEL_H;                 /* Address Offset: 0x007C */
    __IO uint32_t GPIO4A_IOMUX_SEL_L;                 /* Address Offset: 0x0080 */
    __IO uint32_t GPIO4A_IOMUX_SEL_H;                 /* Address Offset: 0x0084 */
    __IO uint32_t GPIO4B_IOMUX_SEL_L;                 /* Address Offset: 0x0088 */
    __IO uint32_t GPIO4B_IOMUX_SEL_H;                 /* Address Offset: 0x008C */
    __IO uint32_t GPIO4C_IOMUX_SEL_L;                 /* Address Offset: 0x0090 */
    __IO uint32_t GPIO4C_IOMUX_SEL_H;                 /* Address Offset: 0x0094 */
    __IO uint32_t GPIO4D_IOMUX_SEL_L;                 /* Address Offset: 0x0098 */
    __IO uint32_t GPIO4D_IOMUX_SEL_H;                 /* Address Offset: 0x009C */
};
/* UART Register Structure Define */
struct UART_REG {
    union {
        __I  uint32_t RBR;                                /* Address Offset: 0x0000 */
        __IO uint32_t DLL;                                /* Address Offset: 0x0000 */
        __O  uint32_t THR;                                /* Address Offset: 0x0000 */
    };
    union {
        __IO uint32_t DLH;                                /* Address Offset: 0x0004 */
        __IO uint32_t IER;                                /* Address Offset: 0x0004 */
    };
    union {
        __O  uint32_t FCR;                                /* Address Offset: 0x0008 */
        __I  uint32_t IIR;                                /* Address Offset: 0x0008 */
    };
    __IO uint32_t LCR;                                /* Address Offset: 0x000C */
    __IO uint32_t MCR;                                /* Address Offset: 0x0010 */
    __I  uint32_t LSR;                                /* Address Offset: 0x0014 */
    __I  uint32_t MSR;                                /* Address Offset: 0x0018 */
    __IO uint32_t SCR;                                /* Address Offset: 0x001C */
         uint32_t RESERVED0020[4];                    /* Address Offset: 0x0020 */
    union {
        __I  uint32_t SRBR;                               /* Address Offset: 0x0030 */
        __O  uint32_t STHR;                               /* Address Offset: 0x0030 */
    };
         uint32_t RESERVED0034[15];                   /* Address Offset: 0x0034 */
    __IO uint32_t FAR;                                /* Address Offset: 0x0070 */
    __I  uint32_t TFR;                                /* Address Offset: 0x0074 */
    __O  uint32_t RFW;                                /* Address Offset: 0x0078 */
    __I  uint32_t USR;                                /* Address Offset: 0x007C */
    __I  uint32_t TFL;                                /* Address Offset: 0x0080 */
    __I  uint32_t RFL;                                /* Address Offset: 0x0084 */
    __O  uint32_t SRR;                                /* Address Offset: 0x0088 */
    __IO uint32_t SRTS;                               /* Address Offset: 0x008C */
    __IO uint32_t SBCR;                               /* Address Offset: 0x0090 */
    __IO uint32_t SDMAM;                              /* Address Offset: 0x0094 */
    __IO uint32_t SFE;                                /* Address Offset: 0x0098 */
    __IO uint32_t SRT;                                /* Address Offset: 0x009C */
    __IO uint32_t STET;                               /* Address Offset: 0x00A0 */
    __IO uint32_t HTX;                                /* Address Offset: 0x00A4 */
    __O  uint32_t DMASA;                              /* Address Offset: 0x00A8 */
         uint32_t RESERVED00AC[18];                   /* Address Offset: 0x00AC */
    __I  uint32_t CPR;                                /* Address Offset: 0x00F4 */
    __I  uint32_t UCV;                                /* Address Offset: 0x00F8 */
    __I  uint32_t CTR;                                /* Address Offset: 0x00FC */
};
/* WDT Register Structure Define */
struct WDT_REG {
    __IO uint32_t CR;                                 /* Address Offset: 0x0000 */
    __IO uint32_t TORR;                               /* Address Offset: 0x0004 */
    __I  uint32_t CCVR;                               /* Address Offset: 0x0008 */
    __O  uint32_t CRR;                                /* Address Offset: 0x000C */
    __I  uint32_t STAT;                               /* Address Offset: 0x0010 */
    __I  uint32_t EOI;                                /* Address Offset: 0x0014 */
};
/* TIMER Register Structure Define */
struct TIMER_REG {
    __IO uint32_t LOAD_COUNT[2];                      /* Address Offset: 0x0000 */
    __I  uint32_t CURRENT_VALUE[2];                   /* Address Offset: 0x0008 */
    __IO uint32_t CONTROLREG;                         /* Address Offset: 0x0010 */
         uint32_t RESERVED0014;                       /* Address Offset: 0x0014 */
    __IO uint32_t INTSTATUS;                          /* Address Offset: 0x0018 */
};
/* FSPI Register Structure Define */
struct FSPI_REG {
    __IO uint32_t CTRL0;                              /* Address Offset: 0x0000 */
    __IO uint32_t IMR;                                /* Address Offset: 0x0004 */
    __IO uint32_t ICLR;                               /* Address Offset: 0x0008 */
    __IO uint32_t FTLR;                               /* Address Offset: 0x000C */
    __IO uint32_t RCVR;                               /* Address Offset: 0x0010 */
    __IO uint32_t AX0;                                /* Address Offset: 0x0014 */
    __IO uint32_t ABIT0;                              /* Address Offset: 0x0018 */
    __IO uint32_t ISR;                                /* Address Offset: 0x001C */
    __IO uint32_t FSR;                                /* Address Offset: 0x0020 */
    __I  uint32_t SR;                                 /* Address Offset: 0x0024 */
    __I  uint32_t RISR;                               /* Address Offset: 0x0028 */
    __I  uint32_t VER;                                /* Address Offset: 0x002C */
    __IO uint32_t QOP;                                /* Address Offset: 0x0030 */
    __IO uint32_t EXT_CTRL;                           /* Address Offset: 0x0034 */
         uint32_t RESERVED0038;                       /* Address Offset: 0x0038 */
    __IO uint32_t DLL_CTRL0;                          /* Address Offset: 0x003C */
         uint32_t RESERVED0040;                       /* Address Offset: 0x0040 */
    __IO uint32_t EXT_AX;                             /* Address Offset: 0x0044 */
    __IO uint32_t SCLK_INATM_CNT;                     /* Address Offset: 0x0048 */
         uint32_t RESERVED004C;                       /* Address Offset: 0x004C */
    __O  uint32_t XMMC_WCMD0;                         /* Address Offset: 0x0050 */
    __O  uint32_t XMMC_RCMD0;                         /* Address Offset: 0x0054 */
    __IO uint32_t XMMC_CTRL;                          /* Address Offset: 0x0058 */
    __IO uint32_t MODE;                               /* Address Offset: 0x005C */
    __IO uint32_t DEVRGN;                             /* Address Offset: 0x0060 */
    __IO uint32_t DEVSIZE0;                           /* Address Offset: 0x0064 */
    __IO uint32_t TME0;                               /* Address Offset: 0x0068 */
         uint32_t RESERVED006C;                       /* Address Offset: 0x006C */
    __IO uint32_t XMMC_RX_WTMRK;                      /* Address Offset: 0x0070 */
         uint32_t RESERVED0074[3];                    /* Address Offset: 0x0074 */
    __IO uint32_t DMATR;                              /* Address Offset: 0x0080 */
    __IO uint32_t DMAADDR;                            /* Address Offset: 0x0084 */
    __IO uint32_t LEN_CTRL;                           /* Address Offset: 0x0088 */
    __IO uint32_t LEN_EXT;                            /* Address Offset: 0x008C */
         uint32_t RESERVED0090;                       /* Address Offset: 0x0090 */
    __IO uint32_t XMMCSR;                             /* Address Offset: 0x0094 */
         uint32_t RESERVED0098[26];                   /* Address Offset: 0x0098 */
    __O  uint32_t CMD;                                /* Address Offset: 0x0100 */
    __O  uint32_t ADDR;                               /* Address Offset: 0x0104 */
    __IO uint32_t DATA;                               /* Address Offset: 0x0108 */
         uint32_t RESERVED010C[61];                   /* Address Offset: 0x010C */
    __IO uint32_t CTRL1;                              /* Address Offset: 0x0200 */
         uint32_t RESERVED0204[4];                    /* Address Offset: 0x0204 */
    __IO uint32_t AX1;                                /* Address Offset: 0x0214 */
    __IO uint32_t ABIT1;                              /* Address Offset: 0x0218 */
         uint32_t RESERVED021C[8];                    /* Address Offset: 0x021C */
    __IO uint32_t DLL_CTRL1;                          /* Address Offset: 0x023C */
         uint32_t RESERVED0240[4];                    /* Address Offset: 0x0240 */
    __O  uint32_t XMMC_WCMD1;                         /* Address Offset: 0x0250 */
    __O  uint32_t XMMC_RCMD1;                         /* Address Offset: 0x0254 */
         uint32_t RESERVED0258[3];                    /* Address Offset: 0x0258 */
    __IO uint32_t DEVSIZE1;                           /* Address Offset: 0x0264 */
    __IO uint32_t TME1;                               /* Address Offset: 0x0268 */
};
/* SPI Register Structure Define */
struct SPI_REG {
    __IO uint32_t CTRLR[2];                           /* Address Offset: 0x0000 */
    __IO uint32_t ENR;                                /* Address Offset: 0x0008 */
    __IO uint32_t SER;                                /* Address Offset: 0x000C */
    __IO uint32_t BAUDR;                              /* Address Offset: 0x0010 */
    __IO uint32_t TXFTLR;                             /* Address Offset: 0x0014 */
    __IO uint32_t RXFTLR;                             /* Address Offset: 0x0018 */
    __I  uint32_t TXFLR;                              /* Address Offset: 0x001C */
    __I  uint32_t RXFLR;                              /* Address Offset: 0x0020 */
    __I  uint32_t SR;                                 /* Address Offset: 0x0024 */
    __IO uint32_t IPR;                                /* Address Offset: 0x0028 */
    __IO uint32_t IMR;                                /* Address Offset: 0x002C */
    __IO uint32_t ISR;                                /* Address Offset: 0x0030 */
    __IO uint32_t RISR;                               /* Address Offset: 0x0034 */
    __O  uint32_t ICR;                                /* Address Offset: 0x0038 */
    __IO uint32_t DMACR;                              /* Address Offset: 0x003C */
    __IO uint32_t DMATDLR;                            /* Address Offset: 0x0040 */
    __IO uint32_t DMARDLR;                            /* Address Offset: 0x0044 */
         uint32_t RESERVED0048;                       /* Address Offset: 0x0048 */
    __IO uint32_t TIMEOUT;                            /* Address Offset: 0x004C */
    __IO uint32_t BYPASS;                             /* Address Offset: 0x0050 */
         uint32_t RESERVED0054[235];                  /* Address Offset: 0x0054 */
    __O  uint32_t TXDR;                               /* Address Offset: 0x0400 */
         uint32_t RESERVED0404[255];                  /* Address Offset: 0x0404 */
    __I  uint32_t RXDR;                               /* Address Offset: 0x0800 */
};
/* MBOX Register Structure Define */
struct MBOX_CMD_DAT {
    __IO uint32_t CMD;
    __IO uint32_t DATA;
};
struct MBOX_REG {
    __IO uint32_t A2B_INTEN;                          /* Address Offset: 0x0000 */
    __IO uint32_t A2B_STATUS;                         /* Address Offset: 0x0004 */
    struct MBOX_CMD_DAT A2B[4];                       /* Address Offset: 0x0008 */
    __IO uint32_t B2A_INTEN;                          /* Address Offset: 0x0028 */
    __IO uint32_t B2A_STATUS;                         /* Address Offset: 0x002C */
    struct MBOX_CMD_DAT B2A[4];                       /* Address Offset: 0x0030 */
         uint32_t RESERVED0050[44];                   /* Address Offset: 0x0050 */
    __IO uint32_t ATOMIC_LOCK[32];                    /* Address Offset: 0x0100 */
};
/* INTMUX Register Structure Define */
struct INTMUX_REG {
};
/* INTMUX0_ Register Structure Define */
struct INTMUX0__REG {
};
/* CRU Register Structure Define */
struct CRU_REG {
         uint32_t RESERVED0000[88];                   /* Address Offset: 0x0000 */
    __IO uint32_t V0PLL_CON[7];                       /* Address Offset: 0x0160 */
         uint32_t RESERVED017C;                       /* Address Offset: 0x017C */
    __IO uint32_t AUPLL_CON[7];                       /* Address Offset: 0x0180 */
         uint32_t RESERVED019C;                       /* Address Offset: 0x019C */
    __IO uint32_t CPLL_CON[7];                        /* Address Offset: 0x01A0 */
         uint32_t RESERVED01BC;                       /* Address Offset: 0x01BC */
    __IO uint32_t GPLL_CON[7];                        /* Address Offset: 0x01C0 */
         uint32_t RESERVED01DC;                       /* Address Offset: 0x01DC */
    __IO uint32_t NPLL_CON[7];                        /* Address Offset: 0x01E0 */
         uint32_t RESERVED01FC[33];                   /* Address Offset: 0x01FC */
    __IO uint32_t MODE_CON00;                         /* Address Offset: 0x0280 */
         uint32_t RESERVED0284[31];                   /* Address Offset: 0x0284 */
    __IO uint32_t CRU_CLKSEL_CON[178];                /* Address Offset: 0x0300 */
         uint32_t RESERVED05C8[142];                  /* Address Offset: 0x05C8 */
    __IO uint32_t CRU_CLKGATE_CON[78];                /* Address Offset: 0x0800 */
         uint32_t RESERVED0938[51];                   /* Address Offset: 0x0938 */
    __IO uint32_t CRU_SOFTRST_CON[77];                /* Address Offset: 0x0A04 */
         uint32_t RESERVED0B38[50];                   /* Address Offset: 0x0B38 */
    __IO uint32_t GLB_CNT_TH;                         /* Address Offset: 0x0C00 */
    __IO uint32_t GLBRST_ST;                          /* Address Offset: 0x0C04 */
    __IO uint32_t GLB_SRST_FST_VALUE;                 /* Address Offset: 0x0C08 */
    __IO uint32_t GLB_SRST_SND_VALUE;                 /* Address Offset: 0x0C0C */
    __IO uint32_t GLB_RST_CON;                        /* Address Offset: 0x0C10 */
         uint32_t RESERVED0C14[4];                    /* Address Offset: 0x0C14 */
    __IO uint32_t SDIO_CON[2];                        /* Address Offset: 0x0C24 */
         uint32_t RESERVED0C2C;                       /* Address Offset: 0x0C2C */
    __IO uint32_t SDMMC_CON[2];                       /* Address Offset: 0x0C30 */
    __IO uint32_t PHYREF_ALT_GATE_CON;                /* Address Offset: 0x0C38 */
    __IO uint32_t CM0_GATEMASK_CON;                   /* Address Offset: 0x0C3C */
         uint32_t RESERVED0C40[25];                   /* Address Offset: 0x0C40 */
    __IO uint32_t QCHANNEL_CON01;                     /* Address Offset: 0x0CA4 */
};
/* BIGCORE0CRU Register Structure Define */
struct BIGCORE0CRU_REG {
    __IO uint32_t B0PLL_CON[7];                       /* Address Offset: 0x0000 */
         uint32_t RESERVED001C[153];                  /* Address Offset: 0x001C */
    __IO uint32_t MODE_CON00;                         /* Address Offset: 0x0280 */
         uint32_t RESERVED0284[31];                   /* Address Offset: 0x0284 */
    __IO uint32_t CLKSEL_CON[3];                      /* Address Offset: 0x0300 */
         uint32_t RESERVED030C[317];                  /* Address Offset: 0x030C */
    __IO uint32_t GATE_CON[2];                        /* Address Offset: 0x0800 */
    __IO uint32_t SOFTRST_CON[2];                     /* Address Offset: 0x0A00 */
    __IO uint32_t SMOTH_DIVFREE_CON04;                /* Address Offset: 0x0CC0 */
    __IO uint32_t SMOTH_DIVFREE_CON05;                /* Address Offset: 0x0CC4 */
    __IO uint32_t AUTOCS_CLK_CORE_B01_I_CON0;         /* Address Offset: 0x0D00 */
    __IO uint32_t AUTOCS_CLK_CORE_B01_I_CON1;         /* Address Offset: 0x0D04 */
};
/* BIGCORE1CRU Register Structure Define */
struct BIGCORE1CRU_REG {
         uint32_t RESERVED0000[8];                    /* Address Offset: 0x0000 */
    __IO uint32_t B1PLL_CON[7];                       /* Address Offset: 0x0020 */
         uint32_t RESERVED003C[145];                  /* Address Offset: 0x003C */
    __IO uint32_t MODE_CON00;                         /* Address Offset: 0x0280 */
         uint32_t RESERVED0284[31];                   /* Address Offset: 0x0284 */
    __IO uint32_t CLKSEL_CON[3];                      /* Address Offset: 0x0300 */
         uint32_t RESERVED030C[317];                  /* Address Offset: 0x030C */
    __IO uint32_t GATE_CON[2];                        /* Address Offset: 0x0800 */
    __IO uint32_t SOFTRST_CON[2];                     /* Address Offset: 0x0A00 */
    __IO uint32_t SMOTH_DIVFREE_CON06;                /* Address Offset: 0x0CC0 */
    __IO uint32_t SMOTH_DIVFREE_CON07;                /* Address Offset: 0x0CC4 */
    __IO uint32_t AUTOCS_CLK_CORE_B23_I_CON0;         /* Address Offset: 0x0D00 */
    __IO uint32_t AUTOCS_CLK_CORE_B23_I_CON1;         /* Address Offset: 0x0D04 */
};
/* DSUCRU Register Structure Define */
struct DSUCRU_REG {
         uint32_t RESERVED0000[16];                   /* Address Offset: 0x0000 */
    __IO uint32_t LPLL_CON[7];                        /* Address Offset: 0x0040 */
         uint32_t RESERVED005C[137];                  /* Address Offset: 0x005C */
    __IO uint32_t MODE_CON00;                         /* Address Offset: 0x0280 */
         uint32_t RESERVED0284[31];                   /* Address Offset: 0x0284 */
    __IO uint32_t CLKSEL_CON[8];                      /* Address Offset: 0x0300 */
         uint32_t RESERVED0320[312];                  /* Address Offset: 0x0320 */
    __IO uint32_t GATE_CON[4];                        /* Address Offset: 0x0800 */
         uint32_t RESERVED0810[124];                  /* Address Offset: 0x0810 */
    __IO uint32_t SOFTRST_CON[4];                     /* Address Offset: 0x0A00 */
         uint32_t RESERVED0A10[188];                  /* Address Offset: 0x0A10 */
    __IO uint32_t AUTOCS_ACLK_M_DSU_BIU_CON0;         /* Address Offset: 0x0D00 */
    __IO uint32_t AUTOCS_ACLK_M_DSU_BIU_CON1;         /* Address Offset: 0x0D04 */
    __IO uint32_t AUTOCS_ACLK_S_DSU_BIU_CON0;         /* Address Offset: 0x0D08 */
    __IO uint32_t AUTOCS_ACLK_S_DSU_BIU_CON1;         /* Address Offset: 0x0D0C */
    __IO uint32_t AUTOCS_ACLK_MP_DSU_BIU_CON0;        /* Address Offset: 0x0D10 */
    __IO uint32_t AUTOCS_ACLK_MP_DSU_BIU_CON1;        /* Address Offset: 0x0D14 */
    __IO uint32_t AUTOCS_SCLK_DSU_SRC_CON0;           /* Address Offset: 0x0D18 */
    __IO uint32_t AUTOCS_SCLK_DSU_SRC_CON1;           /* Address Offset: 0x0D1C */
    __IO uint32_t AUTOCS_CLK_CORE_L_CON0;             /* Address Offset: 0x0D20 */
    __IO uint32_t AUTOCS_CLK_CORE_L_CON1;             /* Address Offset: 0x0D24 */
         uint32_t RESERVED0D28[118];                  /* Address Offset: 0x0D28 */
    __IO uint32_t QCHANNEL_CON00;                     /* Address Offset: 0x0F00 */
         uint32_t RESERVED0F04[3];                    /* Address Offset: 0x0F04 */
    __IO uint32_t SMOTH_DIVFREE_CON00;                /* Address Offset: 0x0F10 */
    __IO uint32_t SMOTH_DIVFREE_CON01;                /* Address Offset: 0x0F14 */
    __IO uint32_t SMOTH_DIVFREE_CON02;                /* Address Offset: 0x0F18 */
    __IO uint32_t SMOTH_DIVFREE_CON03;                /* Address Offset: 0x0F1C */
};
/* PHPTOPCRU Register Structure Define */
struct PHPTOPCRU_REG {
         uint32_t RESERVED0000[128];                  /* Address Offset: 0x0000 */
    __IO uint32_t PPLL_CON[7];                        /* Address Offset: 0x0200 */
         uint32_t RESERVED021C[377];                  /* Address Offset: 0x021C */
    __IO uint32_t GATE_CON00;                         /* Address Offset: 0x0800 */
         uint32_t RESERVED0804[127];                  /* Address Offset: 0x0804 */
    __IO uint32_t SOFTRST_CON00;                      /* Address Offset: 0x0A00 */
};
/* PMU1CRU Register Structure Define */
struct PMU1CRU_REG {
         uint32_t RESERVED0000[192];                  /* Address Offset: 0x0000 */
    __IO uint32_t PMU_CLKSEL_CON[18];                 /* Address Offset: 0x0300 */
         uint32_t RESERVED0348[302];                  /* Address Offset: 0x0348 */
    __IO uint32_t PMU_CLKGATE_CON[6];                 /* Address Offset: 0x0800 */
         uint32_t RESERVED0818[122];                  /* Address Offset: 0x0818 */
    __IO uint32_t PMU_SOFTRST_CON[6];                 /* Address Offset: 0x0A00 */
         uint32_t RESERVED0A18[186];                  /* Address Offset: 0x0A18 */
    __IO uint32_t AUTOCS_HCLK_PMU_CM0_ROOT_I_CON0;    /* Address Offset: 0x0D00 */
    __IO uint32_t AUTOCS_HCLK_PMU_CM0_ROOT_I_CON1;    /* Address Offset: 0x0D04 */
};
#endif /*  __ASSEMBLY__  */
/****************************************************************************************/
/*                                                                                      */
/*                                Module Address Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Memory Base */
#define DCACHE_BASE                    0xF6F10000U /* DCACHE base address */
#define ICACHE_BASE                    0xF6F10000U /* ICACHE base address */
#define PMU1_IOC_BASE                  0xFD5F0000U /* PMU1_IOC base address */
#define PMU2_IOC_BASE                  0xFD5F4000U /* PMU2_IOC base address */
#define BUS_IOC_BASE                   0xFD5F8000U /* BUS_IOC base address */
#define CRU_BASE                       0xFD7C0000U /* CRU base address */
#define BIGCORE0CRU_BASE               0xFD810000U /* BIGCORE0CRU base address */
#define BIGCORE1CRU_BASE               0xFD812000U /* BIGCORE1CRU base address */
#define DSUCRU_BASE                    0xFD818000U /* DSUCRU base address */
#define PHPTOPCRU_BASE                 0xFD7C8000U /* PHPTOPCRU base address */
#define PMU1CRU_BASE                   0xFD7E0000U /* PMU1CRU base address */
#define UART0_BASE                     0xFD890000U /* UART0 base address */
#define WDTPMU_BASE                    0xFD8E0000U /* WDTPMU base address */
#define TIMERPMU_BASE                  0xFD8F0000U /* TIMERPMU base address */
#define WDTNPU_BASE                    0xFDAF8000U /* WDTNPU base address */
#define TIMERNPU_BASE                  0xFDB00000U /* TIMERNPU base address */
#define WDTDDR_BASE                    0xFE110000U /* WDTDDR base address */
#define TIMERDDR_BASE                  0xFE118000U /* TIMERDDR base address */
#define FSPI_BASE                      0xFE2B0000U /* FSPI base address */
#define SPI0_BASE                      0xFEB00000U /* SPI0 base address */
#define SPI1_BASE                      0xFEB10000U /* SPI1 base address */
#define SPI2_BASE                      0xFEB20000U /* SPI2 base address */
#define UART1_BASE                     0xFEB40000U /* UART1 base address */
#define UART2_BASE                     0xFEB50000U /* UART2 base address */
#define UART3_BASE                     0xFEB60000U /* UART3 base address */
#define UART4_BASE                     0xFEB70000U /* UART4 base address */
#define UART5_BASE                     0xFEB80000U /* UART5 base address */
#define UART6_BASE                     0xFEB90000U /* UART6 base address */
#define UART7_BASE                     0xFEBA0000U /* UART7 base address */
#define UART8_BASE                     0xFEBB0000U /* UART8 base address */
#define UART9_BASE                     0xFEBC0000U /* UART9 base address */
#define MBOX0_BASE                     0xFEC60000U /* MBOX0 base address */
#define MBOX1_BASE                     0xFEC70000U /* MBOX1 base address */
#define MBOX2_BASE                     0xFECE0000U /* MBOX2 base address */
#define INTMUX0_PMU_BASE               0xFECF0000U /* INTMUX0_PMU base address */
#define INTMUX1_PMU_BASE               0xFECF4000U /* INTMUX1_PMU base address */
#define INTMUX0_DDR_BASE               0xFECF8000U /* INTMUX0_DDR base address */
#define INTMUX1_DDR_BASE               0xFECFC000U /* INTMUX1_DDR base address */
/****************************************************************************************/
/*                                                                                      */
/*                               Module Variable Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Module Variable Define */

#define DCACHE              ((struct DCACHE_REG *) DCACHE_BASE)
#define ICACHE              ((struct ICACHE_REG *) ICACHE_BASE)
#define PMU1_IOC            ((struct PMU1_IOC_REG *) PMU1_IOC_BASE)
#define PMU2_IOC            ((struct PMU2_IOC_REG *) PMU2_IOC_BASE)
#define BUS_IOC             ((struct BUS_IOC_REG *) BUS_IOC_BASE)
#define CRU                 ((struct CRU_REG *) CRU_BASE)
#define BIGCORE0CRU         ((struct BIGCORE0CRU_REG *) BIGCORE0CRU_BASE)
#define BIGCORE1CRU         ((struct BIGCORE1CRU_REG *) BIGCORE1CRU_BASE)
#define DSUCRU              ((struct DSUCRU_REG *) DSUCRU_BASE)
#define PHPTOPCRU           ((struct PHPTOPCRU_REG *) PHPTOPCRU_BASE)
#define PMU1CRU             ((struct PMU1CRU_REG *) PMU1CRU_BASE)
#define UART0               ((struct UART_REG *) UART0_BASE)
#define WDTPMU              ((struct WDTPMU_REG *) WDTPMU_BASE)
#define TIMERPMU            ((struct TIMERPMU_REG *) TIMERPMU_BASE)
#define WDTNPU              ((struct WDTNPU_REG *) WDTNPU_BASE)
#define TIMERNPU            ((struct TIMERNPU_REG *) TIMERNPU_BASE)
#define WDTDDR              ((struct WDTDDR_REG *) WDTDDR_BASE)
#define TIMERDDR            ((struct TIMERDDR_REG *) TIMERDDR_BASE)
#define FSPI                ((struct FSPI_REG *) FSPI_BASE)
#define SPI0                ((struct SPI_REG *) SPI0_BASE)
#define SPI1                ((struct SPI_REG *) SPI1_BASE)
#define SPI2                ((struct SPI_REG *) SPI2_BASE)
#define UART1               ((struct UART_REG *) UART1_BASE)
#define UART2               ((struct UART_REG *) UART2_BASE)
#define UART3               ((struct UART_REG *) UART3_BASE)
#define UART4               ((struct UART_REG *) UART4_BASE)
#define UART5               ((struct UART_REG *) UART5_BASE)
#define UART6               ((struct UART_REG *) UART6_BASE)
#define UART7               ((struct UART_REG *) UART7_BASE)
#define UART8               ((struct UART_REG *) UART8_BASE)
#define UART9               ((struct UART_REG *) UART9_BASE)
#define MBOX0               ((struct MBOX_REG *) MBOX0_BASE)
#define MBOX1               ((struct MBOX_REG *) MBOX1_BASE)
#define MBOX2               ((struct MBOX_REG *) MBOX2_BASE)
#define INTMUX0_PMU         ((struct INTMUX0_PMU_REG *) INTMUX0_PMU_BASE)
#define INTMUX1_PMU         ((struct INTMUX1_PMU_REG *) INTMUX1_PMU_BASE)
#define INTMUX0_DDR         ((struct INTMUX0_DDR_REG *) INTMUX0_DDR_BASE)
#define INTMUX1_DDR         ((struct INTMUX1_DDR_REG *) INTMUX1_DDR_BASE)

#define IS_DCACHE_INSTANCE(instance) ((instance) == DCACHE)
#define IS_ICACHE_INSTANCE(instance) ((instance) == ICACHE)
#define IS_PMU1_IOC_INSTANCE(instance) ((instance) == PMU1_IOC)
#define IS_PMU2_IOC_INSTANCE(instance) ((instance) == PMU2_IOC)
#define IS_BUS_IOC_INSTANCE(instance) ((instance) == BUS_IOC)
#define IS_CRU_INSTANCE(instance) ((instance) == CRU)
#define IS_BIGCORE0CRU_INSTANCE(instance) ((instance) == BIGCORE0CRU)
#define IS_BIGCORE1CRU_INSTANCE(instance) ((instance) == BIGCORE1CRU)
#define IS_DSUCRU_INSTANCE(instance) ((instance) == DSUCRU)
#define IS_PHPTOPCRU_INSTANCE(instance) ((instance) == PHPTOPCRU)
#define IS_PMU1CRU_INSTANCE(instance) ((instance) == PMU1CRU)
#define IS_WDTPMU_INSTANCE(instance) ((instance) == WDTPMU)
#define IS_TIMERPMU_INSTANCE(instance) ((instance) == TIMERPMU)
#define IS_WDTNPU_INSTANCE(instance) ((instance) == WDTNPU)
#define IS_TIMERNPU_INSTANCE(instance) ((instance) == TIMERNPU)
#define IS_WDTDDR_INSTANCE(instance) ((instance) == WDTDDR)
#define IS_TIMERDDR_INSTANCE(instance) ((instance) == TIMERDDR)
#define IS_FSPI_INSTANCE(instance) ((instance) == FSPI)
#define IS_INTMUX0_PMU_INSTANCE(instance) ((instance) == INTMUX0_PMU)
#define IS_INTMUX1_PMU_INSTANCE(instance) ((instance) == INTMUX1_PMU)
#define IS_INTMUX0_DDR_INSTANCE(instance) ((instance) == INTMUX0_DDR)
#define IS_INTMUX1_DDR_INSTANCE(instance) ((instance) == INTMUX1_DDR)
#define IS_UART_INSTANCE(instance) (((instance) == UART0) || ((instance) == UART1) || ((instance) == UART2) || ((instance) == UART3) || ((instance) == UART4) || ((instance) == UART5) || ((instance) == UART6) || ((instance) == UART7) || ((instance) == UART8) || ((instance) == UART9))
#define IS_SPI_INSTANCE(instance) (((instance) == SPI0) || ((instance) == SPI1) || ((instance) == SPI2))
#define IS_MBOX_INSTANCE(instance) (((instance) == MBOX0) || ((instance) == MBOX1) || ((instance) == MBOX2))
/****************************************************************************************/
/*                                                                                      */
/*                               Register Bitmap Section                                */
/*                                                                                      */
/****************************************************************************************/
/*****************************************DCACHE*****************************************/
/* CACHE_CTRL */
#define DCACHE_CACHE_CTRL_OFFSET                           (0x0U)
#define DCACHE_CACHE_CTRL_CACHE_EN_SHIFT                   (0U)
#define DCACHE_CACHE_CTRL_CACHE_EN_MASK                    (0x1U << DCACHE_CACHE_CTRL_CACHE_EN_SHIFT)                   /* 0x00000001 */
#define DCACHE_CACHE_CTRL_CACHE_WT_EN_SHIFT                (1U)
#define DCACHE_CACHE_CTRL_CACHE_WT_EN_MASK                 (0x1U << DCACHE_CACHE_CTRL_CACHE_WT_EN_SHIFT)                /* 0x00000002 */
#define DCACHE_CACHE_CTRL_CACHE_HB_EN_SHIFT                (2U)
#define DCACHE_CACHE_CTRL_CACHE_HB_EN_MASK                 (0x1U << DCACHE_CACHE_CTRL_CACHE_HB_EN_SHIFT)                /* 0x00000004 */
#define DCACHE_CACHE_CTRL_CACHE_STB_EN_SHIFT               (3U)
#define DCACHE_CACHE_CTRL_CACHE_STB_EN_MASK                (0x1U << DCACHE_CACHE_CTRL_CACHE_STB_EN_SHIFT)               /* 0x00000008 */
#define DCACHE_CACHE_CTRL_CACHE_FLUSH_SHIFT                (4U)
#define DCACHE_CACHE_CTRL_CACHE_FLUSH_MASK                 (0x1U << DCACHE_CACHE_CTRL_CACHE_FLUSH_SHIFT)                /* 0x00000010 */
#define DCACHE_CACHE_CTRL_CACHE_PMU_EN_SHIFT               (5U)
#define DCACHE_CACHE_CTRL_CACHE_PMU_EN_MASK                (0x1U << DCACHE_CACHE_CTRL_CACHE_PMU_EN_SHIFT)               /* 0x00000020 */
#define DCACHE_CACHE_CTRL_CACHE_BYPASS_SHIFT               (6U)
#define DCACHE_CACHE_CTRL_CACHE_BYPASS_MASK                (0x1U << DCACHE_CACHE_CTRL_CACHE_BYPASS_SHIFT)               /* 0x00000040 */
#define DCACHE_CACHE_CTRL_STB_TIMEOUT_EN_SHIFT             (7U)
#define DCACHE_CACHE_CTRL_STB_TIMEOUT_EN_MASK              (0x1U << DCACHE_CACHE_CTRL_STB_TIMEOUT_EN_SHIFT)             /* 0x00000080 */
#define DCACHE_CACHE_CTRL_CACHE_ENTRY_THRESH_SHIFT         (8U)
#define DCACHE_CACHE_CTRL_CACHE_ENTRY_THRESH_MASK          (0x7U << DCACHE_CACHE_CTRL_CACHE_ENTRY_THRESH_SHIFT)         /* 0x00000700 */
#define DCACHE_CACHE_CTRL_CACHE_MPU_MODE_SHIFT             (12U)
#define DCACHE_CACHE_CTRL_CACHE_MPU_MODE_MASK              (0x1U << DCACHE_CACHE_CTRL_CACHE_MPU_MODE_SHIFT)             /* 0x00001000 */
#define DCACHE_CACHE_CTRL_CACHE_PF_EN_SHIFT                (13U)
#define DCACHE_CACHE_CTRL_CACHE_PF_EN_MASK                 (0x1U << DCACHE_CACHE_CTRL_CACHE_PF_EN_SHIFT)                /* 0x00002000 */
/* CACHE_MAINTAIN0 */
#define DCACHE_CACHE_MAINTAIN0_OFFSET                      (0x4U)
#define DCACHE_CACHE_MAINTAIN0_CACHE_M_VALID_SHIFT         (0U)
#define DCACHE_CACHE_MAINTAIN0_CACHE_M_VALID_MASK          (0x1U << DCACHE_CACHE_MAINTAIN0_CACHE_M_VALID_SHIFT)         /* 0x00000001 */
#define DCACHE_CACHE_MAINTAIN0_CACHE_M_CMD_SHIFT           (1U)
#define DCACHE_CACHE_MAINTAIN0_CACHE_M_CMD_MASK            (0x3U << DCACHE_CACHE_MAINTAIN0_CACHE_M_CMD_SHIFT)           /* 0x00000006 */
#define DCACHE_CACHE_MAINTAIN0_CACHE_M_ADDR_SHIFT          (5U)
#define DCACHE_CACHE_MAINTAIN0_CACHE_M_ADDR_MASK           (0x7FFFFFFU << DCACHE_CACHE_MAINTAIN0_CACHE_M_ADDR_SHIFT)    /* 0xFFFFFFE0 */
/* CACHE_MAINTAIN1 */
#define DCACHE_CACHE_MAINTAIN1_OFFSET                      (0x8U)
#define DCACHE_CACHE_MAINTAIN1_CACHE_M_OFFSET_SHIFT        (0U)
#define DCACHE_CACHE_MAINTAIN1_CACHE_M_OFFSET_MASK         (0xFFFFU << DCACHE_CACHE_MAINTAIN1_CACHE_M_OFFSET_SHIFT)     /* 0x0000FFFF */
/* STB_TIMEOUT_CTRL */
#define DCACHE_STB_TIMEOUT_CTRL_OFFSET                     (0xCU)
#define DCACHE_STB_TIMEOUT_CTRL_STB_TIMEOUT_VALUE_SHIFT    (0U)
#define DCACHE_STB_TIMEOUT_CTRL_STB_TIMEOUT_VALUE_MASK     (0x7FFFFU << DCACHE_STB_TIMEOUT_CTRL_STB_TIMEOUT_VALUE_SHIFT) /* 0x0007FFFF */
/* CACHE_INT_EN */
#define DCACHE_CACHE_INT_EN_OFFSET                         (0x20U)
#define DCACHE_CACHE_INT_EN_ERR_RECORD_EN_SHIFT            (0U)
#define DCACHE_CACHE_INT_EN_ERR_RECORD_EN_MASK             (0x1U << DCACHE_CACHE_INT_EN_ERR_RECORD_EN_SHIFT)            /* 0x00000001 */
/* CACHE_INT_ST */
#define DCACHE_CACHE_INT_ST_OFFSET                         (0x24U)
#define DCACHE_CACHE_INT_ST_AHB_ERROR_STATUS_SHIFT         (0U)
#define DCACHE_CACHE_INT_ST_AHB_ERROR_STATUS_MASK          (0x1U << DCACHE_CACHE_INT_ST_AHB_ERROR_STATUS_SHIFT)         /* 0x00000001 */
/* CACHE_ERR_HADDR */
#define DCACHE_CACHE_ERR_HADDR_OFFSET                      (0x28U)
#define DCACHE_CACHE_ERR_HADDR_STATUS_HADDR_SHIFT          (0U)
#define DCACHE_CACHE_ERR_HADDR_STATUS_HADDR_MASK           (0x1U << DCACHE_CACHE_ERR_HADDR_STATUS_HADDR_SHIFT)          /* 0x00000001 */
/* CACHE_STATUS */
#define DCACHE_CACHE_STATUS_OFFSET                         (0x30U)
#define DCACHE_CACHE_STATUS                                (0x0U)
#define DCACHE_CACHE_STATUS_CACHE_INIT_FINISH_SHIFT        (0U)
#define DCACHE_CACHE_STATUS_CACHE_INIT_FINISH_MASK         (0x1U << DCACHE_CACHE_STATUS_CACHE_INIT_FINISH_SHIFT)        /* 0x00000001 */
#define DCACHE_CACHE_STATUS_CACHE_M_BUSY_SHIFT             (1U)
#define DCACHE_CACHE_STATUS_CACHE_M_BUSY_MASK              (0x1U << DCACHE_CACHE_STATUS_CACHE_M_BUSY_SHIFT)             /* 0x00000002 */
#define DCACHE_CACHE_STATUS_CACHE_FLUSH_DONE_SHIFT         (2U)
#define DCACHE_CACHE_STATUS_CACHE_FLUSH_DONE_MASK          (0x1U << DCACHE_CACHE_STATUS_CACHE_FLUSH_DONE_SHIFT)         /* 0x00000004 */
/* PMU_RD_NUM_CNT */
#define DCACHE_PMU_RD_NUM_CNT_OFFSET                       (0x40U)
#define DCACHE_PMU_RD_NUM_CNT                              (0x0U)
#define DCACHE_PMU_RD_NUM_CNT_PMU_RD_NUM_CNT_SHIFT         (0U)
#define DCACHE_PMU_RD_NUM_CNT_PMU_RD_NUM_CNT_MASK          (0xFFFFFFFFU << DCACHE_PMU_RD_NUM_CNT_PMU_RD_NUM_CNT_SHIFT)  /* 0xFFFFFFFF */
/* PMU_WR_NUM_CNT */
#define DCACHE_PMU_WR_NUM_CNT_OFFSET                       (0x44U)
#define DCACHE_PMU_WR_NUM_CNT                              (0x0U)
#define DCACHE_PMU_WR_NUM_CNT_PMU_WR_NUM_CNT_SHIFT         (0U)
#define DCACHE_PMU_WR_NUM_CNT_PMU_WR_NUM_CNT_MASK          (0xFFFFFFFFU << DCACHE_PMU_WR_NUM_CNT_PMU_WR_NUM_CNT_SHIFT)  /* 0xFFFFFFFF */
/* PMU_SRAM_RD_HIT_CNT */
#define DCACHE_PMU_SRAM_RD_HIT_CNT_OFFSET                  (0x48U)
#define DCACHE_PMU_SRAM_RD_HIT_CNT                         (0x0U)
#define DCACHE_PMU_SRAM_RD_HIT_CNT_PMU_SRAM_RD_HIT_CNT_SHIFT (0U)
#define DCACHE_PMU_SRAM_RD_HIT_CNT_PMU_SRAM_RD_HIT_CNT_MASK (0xFFFFFFFFU << DCACHE_PMU_SRAM_RD_HIT_CNT_PMU_SRAM_RD_HIT_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_HB_RD_HIT_CNT */
#define DCACHE_PMU_HB_RD_HIT_CNT_OFFSET                    (0x4CU)
#define DCACHE_PMU_HB_RD_HIT_CNT                           (0x0U)
#define DCACHE_PMU_HB_RD_HIT_CNT_PMU_HB_RD_HIT_CNT_SHIFT   (0U)
#define DCACHE_PMU_HB_RD_HIT_CNT_PMU_HB_RD_HIT_CNT_MASK    (0xFFFFFFFFU << DCACHE_PMU_HB_RD_HIT_CNT_PMU_HB_RD_HIT_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_STB_RD_HIT_CNT */
#define DCACHE_PMU_STB_RD_HIT_CNT_OFFSET                   (0x50U)
#define DCACHE_PMU_STB_RD_HIT_CNT_PMU_STB_RD_HIT_CNT_SHIFT (0U)
#define DCACHE_PMU_STB_RD_HIT_CNT_PMU_STB_RD_HIT_CNT_MASK  (0xFFFFFFFFU << DCACHE_PMU_STB_RD_HIT_CNT_PMU_STB_RD_HIT_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_RD_HIT_CNT */
#define DCACHE_PMU_RD_HIT_CNT_OFFSET                       (0x54U)
#define DCACHE_PMU_RD_HIT_CNT                              (0x0U)
#define DCACHE_PMU_RD_HIT_CNT_PMU_RD_HIT_CNT_SHIFT         (0U)
#define DCACHE_PMU_RD_HIT_CNT_PMU_RD_HIT_CNT_MASK          (0xFFFFFFFFU << DCACHE_PMU_RD_HIT_CNT_PMU_RD_HIT_CNT_SHIFT)  /* 0xFFFFFFFF */
/* PMU_WR_HIT_CNT */
#define DCACHE_PMU_WR_HIT_CNT_OFFSET                       (0x58U)
#define DCACHE_PMU_WR_HIT_CNT                              (0x0U)
#define DCACHE_PMU_WR_HIT_CNT_PMU_WR_HIT_CNT_SHIFT         (0U)
#define DCACHE_PMU_WR_HIT_CNT_PMU_WR_HIT_CNT_MASK          (0xFFFFFFFFU << DCACHE_PMU_WR_HIT_CNT_PMU_WR_HIT_CNT_SHIFT)  /* 0xFFFFFFFF */
/* PMU_RD_MISS_PENALTY_CNT */
#define DCACHE_PMU_RD_MISS_PENALTY_CNT_OFFSET              (0x5CU)
#define DCACHE_PMU_RD_MISS_PENALTY_CNT                     (0x0U)
#define DCACHE_PMU_RD_MISS_PENALTY_CNT_PMU_RD_MISS_PENALTY_CNT_SHIFT (0U)
#define DCACHE_PMU_RD_MISS_PENALTY_CNT_PMU_RD_MISS_PENALTY_CNT_MASK (0xFFFFFFFFU << DCACHE_PMU_RD_MISS_PENALTY_CNT_PMU_RD_MISS_PENALTY_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_WR_MISS_PENALTY_CNT */
#define DCACHE_PMU_WR_MISS_PENALTY_CNT_OFFSET              (0x60U)
#define DCACHE_PMU_WR_MISS_PENALTY_CNT                     (0x0U)
#define DCACHE_PMU_WR_MISS_PENALTY_CNT_PMU_WR_MISS_PENALTY_CNT_SHIFT (0U)
#define DCACHE_PMU_WR_MISS_PENALTY_CNT_PMU_WR_MISS_PENALTY_CNT_MASK (0xFFFFFFFFU << DCACHE_PMU_WR_MISS_PENALTY_CNT_PMU_WR_MISS_PENALTY_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_RD_LAT_CNT */
#define DCACHE_PMU_RD_LAT_CNT_OFFSET                       (0x64U)
#define DCACHE_PMU_RD_LAT_CNT                              (0x0U)
#define DCACHE_PMU_RD_LAT_CNT_PMU_RD_LAT_CNT_SHIFT         (0U)
#define DCACHE_PMU_RD_LAT_CNT_PMU_RD_LAT_CNT_MASK          (0xFFFFFFFFU << DCACHE_PMU_RD_LAT_CNT_PMU_RD_LAT_CNT_SHIFT)  /* 0xFFFFFFFF */
/* PMU_WR_LAT_CNT */
#define DCACHE_PMU_WR_LAT_CNT_OFFSET                       (0x68U)
#define DCACHE_PMU_WR_LAT_CNT                              (0x0U)
#define DCACHE_PMU_WR_LAT_CNT_PMU_RD_LAT_CNT_SHIFT         (0U)
#define DCACHE_PMU_WR_LAT_CNT_PMU_RD_LAT_CNT_MASK          (0xFFFFFFFFU << DCACHE_PMU_WR_LAT_CNT_PMU_RD_LAT_CNT_SHIFT)  /* 0xFFFFFFFF */
/* REVISION */
#define DCACHE_REVISION_OFFSET                             (0xF0U)
#define DCACHE_REVISION_REVISION_SHIFT                     (0U)
#define DCACHE_REVISION_REVISION_MASK                      (0xFFFFFFFFU << DCACHE_REVISION_REVISION_SHIFT)              /* 0xFFFFFFFF */
/*****************************************ICACHE*****************************************/
/* CACHE_CTRL */
#define ICACHE_CACHE_CTRL_OFFSET                           (0x0U)
#define ICACHE_CACHE_CTRL_CACHE_EN_SHIFT                   (0U)
#define ICACHE_CACHE_CTRL_CACHE_EN_MASK                    (0x1U << ICACHE_CACHE_CTRL_CACHE_EN_SHIFT)                   /* 0x00000001 */
#define ICACHE_CACHE_CTRL_CACHE_WT_EN_SHIFT                (1U)
#define ICACHE_CACHE_CTRL_CACHE_WT_EN_MASK                 (0x1U << ICACHE_CACHE_CTRL_CACHE_WT_EN_SHIFT)                /* 0x00000002 */
#define ICACHE_CACHE_CTRL_CACHE_HB_EN_SHIFT                (2U)
#define ICACHE_CACHE_CTRL_CACHE_HB_EN_MASK                 (0x1U << ICACHE_CACHE_CTRL_CACHE_HB_EN_SHIFT)                /* 0x00000004 */
#define ICACHE_CACHE_CTRL_CACHE_STB_EN_SHIFT               (3U)
#define ICACHE_CACHE_CTRL_CACHE_STB_EN_MASK                (0x1U << ICACHE_CACHE_CTRL_CACHE_STB_EN_SHIFT)               /* 0x00000008 */
#define ICACHE_CACHE_CTRL_CACHE_FLUSH_SHIFT                (4U)
#define ICACHE_CACHE_CTRL_CACHE_FLUSH_MASK                 (0x1U << ICACHE_CACHE_CTRL_CACHE_FLUSH_SHIFT)                /* 0x00000010 */
#define ICACHE_CACHE_CTRL_CACHE_PMU_EN_SHIFT               (5U)
#define ICACHE_CACHE_CTRL_CACHE_PMU_EN_MASK                (0x1U << ICACHE_CACHE_CTRL_CACHE_PMU_EN_SHIFT)               /* 0x00000020 */
#define ICACHE_CACHE_CTRL_CACHE_BYPASS_SHIFT               (6U)
#define ICACHE_CACHE_CTRL_CACHE_BYPASS_MASK                (0x1U << ICACHE_CACHE_CTRL_CACHE_BYPASS_SHIFT)               /* 0x00000040 */
#define ICACHE_CACHE_CTRL_STB_TIMEOUT_EN_SHIFT             (7U)
#define ICACHE_CACHE_CTRL_STB_TIMEOUT_EN_MASK              (0x1U << ICACHE_CACHE_CTRL_STB_TIMEOUT_EN_SHIFT)             /* 0x00000080 */
#define ICACHE_CACHE_CTRL_CACHE_ENTRY_THRESH_SHIFT         (8U)
#define ICACHE_CACHE_CTRL_CACHE_ENTRY_THRESH_MASK          (0x7U << ICACHE_CACHE_CTRL_CACHE_ENTRY_THRESH_SHIFT)         /* 0x00000700 */
#define ICACHE_CACHE_CTRL_CACHE_MPU_MODE_SHIFT             (12U)
#define ICACHE_CACHE_CTRL_CACHE_MPU_MODE_MASK              (0x1U << ICACHE_CACHE_CTRL_CACHE_MPU_MODE_SHIFT)             /* 0x00001000 */
#define ICACHE_CACHE_CTRL_CACHE_PF_EN_SHIFT                (13U)
#define ICACHE_CACHE_CTRL_CACHE_PF_EN_MASK                 (0x1U << ICACHE_CACHE_CTRL_CACHE_PF_EN_SHIFT)                /* 0x00002000 */
/* CACHE_MAINTAIN0 */
#define ICACHE_CACHE_MAINTAIN0_OFFSET                      (0x4U)
#define ICACHE_CACHE_MAINTAIN0_CACHE_M_VALID_SHIFT         (0U)
#define ICACHE_CACHE_MAINTAIN0_CACHE_M_VALID_MASK          (0x1U << ICACHE_CACHE_MAINTAIN0_CACHE_M_VALID_SHIFT)         /* 0x00000001 */
#define ICACHE_CACHE_MAINTAIN0_CACHE_M_CMD_SHIFT           (1U)
#define ICACHE_CACHE_MAINTAIN0_CACHE_M_CMD_MASK            (0x3U << ICACHE_CACHE_MAINTAIN0_CACHE_M_CMD_SHIFT)           /* 0x00000006 */
#define ICACHE_CACHE_MAINTAIN0_CACHE_M_ADDR_SHIFT          (5U)
#define ICACHE_CACHE_MAINTAIN0_CACHE_M_ADDR_MASK           (0x7FFFFFFU << ICACHE_CACHE_MAINTAIN0_CACHE_M_ADDR_SHIFT)    /* 0xFFFFFFE0 */
/* CACHE_MAINTAIN1 */
#define ICACHE_CACHE_MAINTAIN1_OFFSET                      (0x8U)
#define ICACHE_CACHE_MAINTAIN1_CACHE_M_OFFSET_SHIFT        (0U)
#define ICACHE_CACHE_MAINTAIN1_CACHE_M_OFFSET_MASK         (0xFFFFU << ICACHE_CACHE_MAINTAIN1_CACHE_M_OFFSET_SHIFT)     /* 0x0000FFFF */
/* STB_TIMEOUT_CTRL */
#define ICACHE_STB_TIMEOUT_CTRL_OFFSET                     (0xCU)
#define ICACHE_STB_TIMEOUT_CTRL_STB_TIMEOUT_VALUE_SHIFT    (0U)
#define ICACHE_STB_TIMEOUT_CTRL_STB_TIMEOUT_VALUE_MASK     (0x7FFFFU << ICACHE_STB_TIMEOUT_CTRL_STB_TIMEOUT_VALUE_SHIFT) /* 0x0007FFFF */
/* CACHE_INT_EN */
#define ICACHE_CACHE_INT_EN_OFFSET                         (0x20U)
#define ICACHE_CACHE_INT_EN_ERR_RECORD_EN_SHIFT            (0U)
#define ICACHE_CACHE_INT_EN_ERR_RECORD_EN_MASK             (0x1U << ICACHE_CACHE_INT_EN_ERR_RECORD_EN_SHIFT)            /* 0x00000001 */
/* CACHE_INT_ST */
#define ICACHE_CACHE_INT_ST_OFFSET                         (0x24U)
#define ICACHE_CACHE_INT_ST_AHB_ERROR_STATUS_SHIFT         (0U)
#define ICACHE_CACHE_INT_ST_AHB_ERROR_STATUS_MASK          (0x1U << ICACHE_CACHE_INT_ST_AHB_ERROR_STATUS_SHIFT)         /* 0x00000001 */
/* CACHE_ERR_HADDR */
#define ICACHE_CACHE_ERR_HADDR_OFFSET                      (0x28U)
#define ICACHE_CACHE_ERR_HADDR_STATUS_HADDR_SHIFT          (0U)
#define ICACHE_CACHE_ERR_HADDR_STATUS_HADDR_MASK           (0x1U << ICACHE_CACHE_ERR_HADDR_STATUS_HADDR_SHIFT)          /* 0x00000001 */
/* CACHE_STATUS */
#define ICACHE_CACHE_STATUS_OFFSET                         (0x30U)
#define ICACHE_CACHE_STATUS                                (0x0U)
#define ICACHE_CACHE_STATUS_CACHE_INIT_FINISH_SHIFT        (0U)
#define ICACHE_CACHE_STATUS_CACHE_INIT_FINISH_MASK         (0x1U << ICACHE_CACHE_STATUS_CACHE_INIT_FINISH_SHIFT)        /* 0x00000001 */
#define ICACHE_CACHE_STATUS_CACHE_M_BUSY_SHIFT             (1U)
#define ICACHE_CACHE_STATUS_CACHE_M_BUSY_MASK              (0x1U << ICACHE_CACHE_STATUS_CACHE_M_BUSY_SHIFT)             /* 0x00000002 */
#define ICACHE_CACHE_STATUS_CACHE_FLUSH_DONE_SHIFT         (2U)
#define ICACHE_CACHE_STATUS_CACHE_FLUSH_DONE_MASK          (0x1U << ICACHE_CACHE_STATUS_CACHE_FLUSH_DONE_SHIFT)         /* 0x00000004 */
/* PMU_RD_NUM_CNT */
#define ICACHE_PMU_RD_NUM_CNT_OFFSET                       (0x40U)
#define ICACHE_PMU_RD_NUM_CNT                              (0x0U)
#define ICACHE_PMU_RD_NUM_CNT_PMU_RD_NUM_CNT_SHIFT         (0U)
#define ICACHE_PMU_RD_NUM_CNT_PMU_RD_NUM_CNT_MASK          (0xFFFFFFFFU << ICACHE_PMU_RD_NUM_CNT_PMU_RD_NUM_CNT_SHIFT)  /* 0xFFFFFFFF */
/* PMU_WR_NUM_CNT */
#define ICACHE_PMU_WR_NUM_CNT_OFFSET                       (0x44U)
#define ICACHE_PMU_WR_NUM_CNT                              (0x0U)
#define ICACHE_PMU_WR_NUM_CNT_PMU_WR_NUM_CNT_SHIFT         (0U)
#define ICACHE_PMU_WR_NUM_CNT_PMU_WR_NUM_CNT_MASK          (0xFFFFFFFFU << ICACHE_PMU_WR_NUM_CNT_PMU_WR_NUM_CNT_SHIFT)  /* 0xFFFFFFFF */
/* PMU_SRAM_RD_HIT_CNT */
#define ICACHE_PMU_SRAM_RD_HIT_CNT_OFFSET                  (0x48U)
#define ICACHE_PMU_SRAM_RD_HIT_CNT                         (0x0U)
#define ICACHE_PMU_SRAM_RD_HIT_CNT_PMU_SRAM_RD_HIT_CNT_SHIFT (0U)
#define ICACHE_PMU_SRAM_RD_HIT_CNT_PMU_SRAM_RD_HIT_CNT_MASK (0xFFFFFFFFU << ICACHE_PMU_SRAM_RD_HIT_CNT_PMU_SRAM_RD_HIT_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_HB_RD_HIT_CNT */
#define ICACHE_PMU_HB_RD_HIT_CNT_OFFSET                    (0x4CU)
#define ICACHE_PMU_HB_RD_HIT_CNT                           (0x0U)
#define ICACHE_PMU_HB_RD_HIT_CNT_PMU_HB_RD_HIT_CNT_SHIFT   (0U)
#define ICACHE_PMU_HB_RD_HIT_CNT_PMU_HB_RD_HIT_CNT_MASK    (0xFFFFFFFFU << ICACHE_PMU_HB_RD_HIT_CNT_PMU_HB_RD_HIT_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_STB_RD_HIT_CNT */
#define ICACHE_PMU_STB_RD_HIT_CNT_OFFSET                   (0x50U)
#define ICACHE_PMU_STB_RD_HIT_CNT_PMU_STB_RD_HIT_CNT_SHIFT (0U)
#define ICACHE_PMU_STB_RD_HIT_CNT_PMU_STB_RD_HIT_CNT_MASK  (0xFFFFFFFFU << ICACHE_PMU_STB_RD_HIT_CNT_PMU_STB_RD_HIT_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_RD_HIT_CNT */
#define ICACHE_PMU_RD_HIT_CNT_OFFSET                       (0x54U)
#define ICACHE_PMU_RD_HIT_CNT                              (0x0U)
#define ICACHE_PMU_RD_HIT_CNT_PMU_RD_HIT_CNT_SHIFT         (0U)
#define ICACHE_PMU_RD_HIT_CNT_PMU_RD_HIT_CNT_MASK          (0xFFFFFFFFU << ICACHE_PMU_RD_HIT_CNT_PMU_RD_HIT_CNT_SHIFT)  /* 0xFFFFFFFF */
/* PMU_WR_HIT_CNT */
#define ICACHE_PMU_WR_HIT_CNT_OFFSET                       (0x58U)
#define ICACHE_PMU_WR_HIT_CNT                              (0x0U)
#define ICACHE_PMU_WR_HIT_CNT_PMU_WR_HIT_CNT_SHIFT         (0U)
#define ICACHE_PMU_WR_HIT_CNT_PMU_WR_HIT_CNT_MASK          (0xFFFFFFFFU << ICACHE_PMU_WR_HIT_CNT_PMU_WR_HIT_CNT_SHIFT)  /* 0xFFFFFFFF */
/* PMU_RD_MISS_PENALTY_CNT */
#define ICACHE_PMU_RD_MISS_PENALTY_CNT_OFFSET              (0x5CU)
#define ICACHE_PMU_RD_MISS_PENALTY_CNT                     (0x0U)
#define ICACHE_PMU_RD_MISS_PENALTY_CNT_PMU_RD_MISS_PENALTY_CNT_SHIFT (0U)
#define ICACHE_PMU_RD_MISS_PENALTY_CNT_PMU_RD_MISS_PENALTY_CNT_MASK (0xFFFFFFFFU << ICACHE_PMU_RD_MISS_PENALTY_CNT_PMU_RD_MISS_PENALTY_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_WR_MISS_PENALTY_CNT */
#define ICACHE_PMU_WR_MISS_PENALTY_CNT_OFFSET              (0x60U)
#define ICACHE_PMU_WR_MISS_PENALTY_CNT                     (0x0U)
#define ICACHE_PMU_WR_MISS_PENALTY_CNT_PMU_WR_MISS_PENALTY_CNT_SHIFT (0U)
#define ICACHE_PMU_WR_MISS_PENALTY_CNT_PMU_WR_MISS_PENALTY_CNT_MASK (0xFFFFFFFFU << ICACHE_PMU_WR_MISS_PENALTY_CNT_PMU_WR_MISS_PENALTY_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_RD_LAT_CNT */
#define ICACHE_PMU_RD_LAT_CNT_OFFSET                       (0x64U)
#define ICACHE_PMU_RD_LAT_CNT                              (0x0U)
#define ICACHE_PMU_RD_LAT_CNT_PMU_RD_LAT_CNT_SHIFT         (0U)
#define ICACHE_PMU_RD_LAT_CNT_PMU_RD_LAT_CNT_MASK          (0xFFFFFFFFU << ICACHE_PMU_RD_LAT_CNT_PMU_RD_LAT_CNT_SHIFT)  /* 0xFFFFFFFF */
/* PMU_WR_LAT_CNT */
#define ICACHE_PMU_WR_LAT_CNT_OFFSET                       (0x68U)
#define ICACHE_PMU_WR_LAT_CNT                              (0x0U)
#define ICACHE_PMU_WR_LAT_CNT_PMU_RD_LAT_CNT_SHIFT         (0U)
#define ICACHE_PMU_WR_LAT_CNT_PMU_RD_LAT_CNT_MASK          (0xFFFFFFFFU << ICACHE_PMU_WR_LAT_CNT_PMU_RD_LAT_CNT_SHIFT)  /* 0xFFFFFFFF */
/* REVISION */
#define ICACHE_REVISION_OFFSET                             (0xF0U)
#define ICACHE_REVISION_REVISION_SHIFT                     (0U)
#define ICACHE_REVISION_REVISION_MASK                      (0xFFFFFFFFU << ICACHE_REVISION_REVISION_SHIFT)              /* 0xFFFFFFFF */
/****************************************PMU1_IOC****************************************/
/* GPIO0A_IOMUX_SEL_L */
#define PMU1_IOC_GPIO0A_IOMUX_SEL_L_OFFSET                 (0x0U)
#define PMU1_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A0_SEL_SHIFT      (0U)
#define PMU1_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A0_SEL_MASK       (0xFU << PMU1_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A0_SEL_SHIFT)      /* 0x0000000F */
#define PMU1_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A1_SEL_SHIFT      (4U)
#define PMU1_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A1_SEL_MASK       (0xFU << PMU1_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A1_SEL_SHIFT)      /* 0x000000F0 */
#define PMU1_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A2_SEL_SHIFT      (8U)
#define PMU1_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A2_SEL_MASK       (0xFU << PMU1_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A2_SEL_SHIFT)      /* 0x00000F00 */
#define PMU1_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A3_SEL_SHIFT      (12U)
#define PMU1_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A3_SEL_MASK       (0xFU << PMU1_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A3_SEL_SHIFT)      /* 0x0000F000 */
/* GPIO0A_IOMUX_SEL_H */
#define PMU1_IOC_GPIO0A_IOMUX_SEL_H_OFFSET                 (0x4U)
#define PMU1_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A4_SEL_SHIFT      (0U)
#define PMU1_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A4_SEL_MASK       (0xFU << PMU1_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A4_SEL_SHIFT)      /* 0x0000000F */
#define PMU1_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A5_SEL_SHIFT      (4U)
#define PMU1_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A5_SEL_MASK       (0xFU << PMU1_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A5_SEL_SHIFT)      /* 0x000000F0 */
#define PMU1_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A6_SEL_SHIFT      (8U)
#define PMU1_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A6_SEL_MASK       (0xFU << PMU1_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A6_SEL_SHIFT)      /* 0x00000F00 */
#define PMU1_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A7_SEL_SHIFT      (12U)
#define PMU1_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A7_SEL_MASK       (0xFU << PMU1_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A7_SEL_SHIFT)      /* 0x0000F000 */
/* GPIO0B_IOMUX_SEL_L */
#define PMU1_IOC_GPIO0B_IOMUX_SEL_L_OFFSET                 (0x8U)
#define PMU1_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B0_SEL_SHIFT      (0U)
#define PMU1_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B0_SEL_MASK       (0xFU << PMU1_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B0_SEL_SHIFT)      /* 0x0000000F */
#define PMU1_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B1_SEL_SHIFT      (4U)
#define PMU1_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B1_SEL_MASK       (0xFU << PMU1_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B1_SEL_SHIFT)      /* 0x000000F0 */
#define PMU1_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B2_SEL_SHIFT      (8U)
#define PMU1_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B2_SEL_MASK       (0xFU << PMU1_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B2_SEL_SHIFT)      /* 0x00000F00 */
#define PMU1_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B3_SEL_SHIFT      (12U)
#define PMU1_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B3_SEL_MASK       (0xFU << PMU1_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B3_SEL_SHIFT)      /* 0x0000F000 */
/* GPIO0A_DS_L */
#define PMU1_IOC_GPIO0A_DS_L_OFFSET                        (0x10U)
#define PMU1_IOC_GPIO0A_DS_L_GPIO0A0_DS_SHIFT              (0U)
#define PMU1_IOC_GPIO0A_DS_L_GPIO0A0_DS_MASK               (0x3U << PMU1_IOC_GPIO0A_DS_L_GPIO0A0_DS_SHIFT)              /* 0x00000003 */
#define PMU1_IOC_GPIO0A_DS_L_GPIO0A1_DS_SHIFT              (4U)
#define PMU1_IOC_GPIO0A_DS_L_GPIO0A1_DS_MASK               (0x3U << PMU1_IOC_GPIO0A_DS_L_GPIO0A1_DS_SHIFT)              /* 0x00000030 */
#define PMU1_IOC_GPIO0A_DS_L_GPIO0A2_DS_SHIFT              (8U)
#define PMU1_IOC_GPIO0A_DS_L_GPIO0A2_DS_MASK               (0x3U << PMU1_IOC_GPIO0A_DS_L_GPIO0A2_DS_SHIFT)              /* 0x00000300 */
#define PMU1_IOC_GPIO0A_DS_L_GPIO0A3_DS_SHIFT              (12U)
#define PMU1_IOC_GPIO0A_DS_L_GPIO0A3_DS_MASK               (0x3U << PMU1_IOC_GPIO0A_DS_L_GPIO0A3_DS_SHIFT)              /* 0x00003000 */
/* GPIO0A_DS_H */
#define PMU1_IOC_GPIO0A_DS_H_OFFSET                        (0x14U)
#define PMU1_IOC_GPIO0A_DS_H_GPIO0A4_DS_SHIFT              (0U)
#define PMU1_IOC_GPIO0A_DS_H_GPIO0A4_DS_MASK               (0x3U << PMU1_IOC_GPIO0A_DS_H_GPIO0A4_DS_SHIFT)              /* 0x00000003 */
#define PMU1_IOC_GPIO0A_DS_H_GPIO0A5_DS_SHIFT              (4U)
#define PMU1_IOC_GPIO0A_DS_H_GPIO0A5_DS_MASK               (0x3U << PMU1_IOC_GPIO0A_DS_H_GPIO0A5_DS_SHIFT)              /* 0x00000030 */
#define PMU1_IOC_GPIO0A_DS_H_GPIO0A6_DS_SHIFT              (8U)
#define PMU1_IOC_GPIO0A_DS_H_GPIO0A6_DS_MASK               (0x3U << PMU1_IOC_GPIO0A_DS_H_GPIO0A6_DS_SHIFT)              /* 0x00000300 */
#define PMU1_IOC_GPIO0A_DS_H_GPIO0A7_DS_SHIFT              (12U)
#define PMU1_IOC_GPIO0A_DS_H_GPIO0A7_DS_MASK               (0x3U << PMU1_IOC_GPIO0A_DS_H_GPIO0A7_DS_SHIFT)              /* 0x00003000 */
/* GPIO0B_DS_L */
#define PMU1_IOC_GPIO0B_DS_L_OFFSET                        (0x18U)
#define PMU1_IOC_GPIO0B_DS_L_GPIO0B0_DS_SHIFT              (0U)
#define PMU1_IOC_GPIO0B_DS_L_GPIO0B0_DS_MASK               (0x3U << PMU1_IOC_GPIO0B_DS_L_GPIO0B0_DS_SHIFT)              /* 0x00000003 */
#define PMU1_IOC_GPIO0B_DS_L_GPIO0B1_DS_SHIFT              (4U)
#define PMU1_IOC_GPIO0B_DS_L_GPIO0B1_DS_MASK               (0x3U << PMU1_IOC_GPIO0B_DS_L_GPIO0B1_DS_SHIFT)              /* 0x00000030 */
#define PMU1_IOC_GPIO0B_DS_L_GPIO0B2_DS_SHIFT              (8U)
#define PMU1_IOC_GPIO0B_DS_L_GPIO0B2_DS_MASK               (0x3U << PMU1_IOC_GPIO0B_DS_L_GPIO0B2_DS_SHIFT)              /* 0x00000300 */
#define PMU1_IOC_GPIO0B_DS_L_GPIO0B3_DS_SHIFT              (12U)
#define PMU1_IOC_GPIO0B_DS_L_GPIO0B3_DS_MASK               (0x3U << PMU1_IOC_GPIO0B_DS_L_GPIO0B3_DS_SHIFT)              /* 0x00003000 */
/* GPIO0A_P */
#define PMU1_IOC_GPIO0A_P_OFFSET                           (0x20U)
#define PMU1_IOC_GPIO0A_P_GPIO0A0_PE_SHIFT                 (0U)
#define PMU1_IOC_GPIO0A_P_GPIO0A0_PE_MASK                  (0x1U << PMU1_IOC_GPIO0A_P_GPIO0A0_PE_SHIFT)                 /* 0x00000001 */
#define PMU1_IOC_GPIO0A_P_GPIO0A0_PS_SHIFT                 (1U)
#define PMU1_IOC_GPIO0A_P_GPIO0A0_PS_MASK                  (0x1U << PMU1_IOC_GPIO0A_P_GPIO0A0_PS_SHIFT)                 /* 0x00000002 */
#define PMU1_IOC_GPIO0A_P_GPIO0A1_PE_SHIFT                 (2U)
#define PMU1_IOC_GPIO0A_P_GPIO0A1_PE_MASK                  (0x1U << PMU1_IOC_GPIO0A_P_GPIO0A1_PE_SHIFT)                 /* 0x00000004 */
#define PMU1_IOC_GPIO0A_P_GPIO0A1_PS_SHIFT                 (3U)
#define PMU1_IOC_GPIO0A_P_GPIO0A1_PS_MASK                  (0x1U << PMU1_IOC_GPIO0A_P_GPIO0A1_PS_SHIFT)                 /* 0x00000008 */
#define PMU1_IOC_GPIO0A_P_GPIO0A2_PE_SHIFT                 (4U)
#define PMU1_IOC_GPIO0A_P_GPIO0A2_PE_MASK                  (0x1U << PMU1_IOC_GPIO0A_P_GPIO0A2_PE_SHIFT)                 /* 0x00000010 */
#define PMU1_IOC_GPIO0A_P_GPIO0A2_PS_SHIFT                 (5U)
#define PMU1_IOC_GPIO0A_P_GPIO0A2_PS_MASK                  (0x1U << PMU1_IOC_GPIO0A_P_GPIO0A2_PS_SHIFT)                 /* 0x00000020 */
#define PMU1_IOC_GPIO0A_P_GPIO0A3_PE_SHIFT                 (6U)
#define PMU1_IOC_GPIO0A_P_GPIO0A3_PE_MASK                  (0x1U << PMU1_IOC_GPIO0A_P_GPIO0A3_PE_SHIFT)                 /* 0x00000040 */
#define PMU1_IOC_GPIO0A_P_GPIO0A3_PS_SHIFT                 (7U)
#define PMU1_IOC_GPIO0A_P_GPIO0A3_PS_MASK                  (0x1U << PMU1_IOC_GPIO0A_P_GPIO0A3_PS_SHIFT)                 /* 0x00000080 */
#define PMU1_IOC_GPIO0A_P_GPIO0A4_PE_SHIFT                 (8U)
#define PMU1_IOC_GPIO0A_P_GPIO0A4_PE_MASK                  (0x1U << PMU1_IOC_GPIO0A_P_GPIO0A4_PE_SHIFT)                 /* 0x00000100 */
#define PMU1_IOC_GPIO0A_P_GPIO0A4_PS_SHIFT                 (9U)
#define PMU1_IOC_GPIO0A_P_GPIO0A4_PS_MASK                  (0x1U << PMU1_IOC_GPIO0A_P_GPIO0A4_PS_SHIFT)                 /* 0x00000200 */
#define PMU1_IOC_GPIO0A_P_GPIO0A5_PE_SHIFT                 (10U)
#define PMU1_IOC_GPIO0A_P_GPIO0A5_PE_MASK                  (0x1U << PMU1_IOC_GPIO0A_P_GPIO0A5_PE_SHIFT)                 /* 0x00000400 */
#define PMU1_IOC_GPIO0A_P_GPIO0A5_PS_SHIFT                 (11U)
#define PMU1_IOC_GPIO0A_P_GPIO0A5_PS_MASK                  (0x1U << PMU1_IOC_GPIO0A_P_GPIO0A5_PS_SHIFT)                 /* 0x00000800 */
#define PMU1_IOC_GPIO0A_P_GPIO0A6_PE_SHIFT                 (12U)
#define PMU1_IOC_GPIO0A_P_GPIO0A6_PE_MASK                  (0x1U << PMU1_IOC_GPIO0A_P_GPIO0A6_PE_SHIFT)                 /* 0x00001000 */
#define PMU1_IOC_GPIO0A_P_GPIO0A6_PS_SHIFT                 (13U)
#define PMU1_IOC_GPIO0A_P_GPIO0A6_PS_MASK                  (0x1U << PMU1_IOC_GPIO0A_P_GPIO0A6_PS_SHIFT)                 /* 0x00002000 */
#define PMU1_IOC_GPIO0A_P_GPIO0A7_PE_SHIFT                 (14U)
#define PMU1_IOC_GPIO0A_P_GPIO0A7_PE_MASK                  (0x1U << PMU1_IOC_GPIO0A_P_GPIO0A7_PE_SHIFT)                 /* 0x00004000 */
#define PMU1_IOC_GPIO0A_P_GPIO0A7_PS_SHIFT                 (15U)
#define PMU1_IOC_GPIO0A_P_GPIO0A7_PS_MASK                  (0x1U << PMU1_IOC_GPIO0A_P_GPIO0A7_PS_SHIFT)                 /* 0x00008000 */
/* GPIO0B_P */
#define PMU1_IOC_GPIO0B_P_OFFSET                           (0x24U)
#define PMU1_IOC_GPIO0B_P_GPIO0B0_PE_SHIFT                 (0U)
#define PMU1_IOC_GPIO0B_P_GPIO0B0_PE_MASK                  (0x1U << PMU1_IOC_GPIO0B_P_GPIO0B0_PE_SHIFT)                 /* 0x00000001 */
#define PMU1_IOC_GPIO0B_P_GPIO0B0_PS_SHIFT                 (1U)
#define PMU1_IOC_GPIO0B_P_GPIO0B0_PS_MASK                  (0x1U << PMU1_IOC_GPIO0B_P_GPIO0B0_PS_SHIFT)                 /* 0x00000002 */
#define PMU1_IOC_GPIO0B_P_GPIO0B1_PE_SHIFT                 (2U)
#define PMU1_IOC_GPIO0B_P_GPIO0B1_PE_MASK                  (0x1U << PMU1_IOC_GPIO0B_P_GPIO0B1_PE_SHIFT)                 /* 0x00000004 */
#define PMU1_IOC_GPIO0B_P_GPIO0B1_PS_SHIFT                 (3U)
#define PMU1_IOC_GPIO0B_P_GPIO0B1_PS_MASK                  (0x1U << PMU1_IOC_GPIO0B_P_GPIO0B1_PS_SHIFT)                 /* 0x00000008 */
#define PMU1_IOC_GPIO0B_P_GPIO0B2_PE_SHIFT                 (4U)
#define PMU1_IOC_GPIO0B_P_GPIO0B2_PE_MASK                  (0x1U << PMU1_IOC_GPIO0B_P_GPIO0B2_PE_SHIFT)                 /* 0x00000010 */
#define PMU1_IOC_GPIO0B_P_GPIO0B2_PS_SHIFT                 (5U)
#define PMU1_IOC_GPIO0B_P_GPIO0B2_PS_MASK                  (0x1U << PMU1_IOC_GPIO0B_P_GPIO0B2_PS_SHIFT)                 /* 0x00000020 */
#define PMU1_IOC_GPIO0B_P_GPIO0B3_PE_SHIFT                 (6U)
#define PMU1_IOC_GPIO0B_P_GPIO0B3_PE_MASK                  (0x1U << PMU1_IOC_GPIO0B_P_GPIO0B3_PE_SHIFT)                 /* 0x00000040 */
#define PMU1_IOC_GPIO0B_P_GPIO0B3_PS_SHIFT                 (7U)
#define PMU1_IOC_GPIO0B_P_GPIO0B3_PS_MASK                  (0x1U << PMU1_IOC_GPIO0B_P_GPIO0B3_PS_SHIFT)                 /* 0x00000080 */
/* GPIO0A_IE */
#define PMU1_IOC_GPIO0A_IE_OFFSET                          (0x28U)
#define PMU1_IOC_GPIO0A_IE_GPIO0A0_IE_SHIFT                (0U)
#define PMU1_IOC_GPIO0A_IE_GPIO0A0_IE_MASK                 (0x1U << PMU1_IOC_GPIO0A_IE_GPIO0A0_IE_SHIFT)                /* 0x00000001 */
#define PMU1_IOC_GPIO0A_IE_GPIO0A1_IE_SHIFT                (1U)
#define PMU1_IOC_GPIO0A_IE_GPIO0A1_IE_MASK                 (0x1U << PMU1_IOC_GPIO0A_IE_GPIO0A1_IE_SHIFT)                /* 0x00000002 */
#define PMU1_IOC_GPIO0A_IE_GPIO0A2_IE_SHIFT                (2U)
#define PMU1_IOC_GPIO0A_IE_GPIO0A2_IE_MASK                 (0x1U << PMU1_IOC_GPIO0A_IE_GPIO0A2_IE_SHIFT)                /* 0x00000004 */
#define PMU1_IOC_GPIO0A_IE_GPIO0A3_IE_SHIFT                (3U)
#define PMU1_IOC_GPIO0A_IE_GPIO0A3_IE_MASK                 (0x1U << PMU1_IOC_GPIO0A_IE_GPIO0A3_IE_SHIFT)                /* 0x00000008 */
#define PMU1_IOC_GPIO0A_IE_GPIO0A4_IE_SHIFT                (4U)
#define PMU1_IOC_GPIO0A_IE_GPIO0A4_IE_MASK                 (0x1U << PMU1_IOC_GPIO0A_IE_GPIO0A4_IE_SHIFT)                /* 0x00000010 */
#define PMU1_IOC_GPIO0A_IE_GPIO0A5_IE_SHIFT                (5U)
#define PMU1_IOC_GPIO0A_IE_GPIO0A5_IE_MASK                 (0x1U << PMU1_IOC_GPIO0A_IE_GPIO0A5_IE_SHIFT)                /* 0x00000020 */
#define PMU1_IOC_GPIO0A_IE_GPIO0A6_IE_SHIFT                (6U)
#define PMU1_IOC_GPIO0A_IE_GPIO0A6_IE_MASK                 (0x1U << PMU1_IOC_GPIO0A_IE_GPIO0A6_IE_SHIFT)                /* 0x00000040 */
#define PMU1_IOC_GPIO0A_IE_GPIO0A7_IE_SHIFT                (7U)
#define PMU1_IOC_GPIO0A_IE_GPIO0A7_IE_MASK                 (0x1U << PMU1_IOC_GPIO0A_IE_GPIO0A7_IE_SHIFT)                /* 0x00000080 */
/* GPIO0B_IE */
#define PMU1_IOC_GPIO0B_IE_OFFSET                          (0x2CU)
#define PMU1_IOC_GPIO0B_IE_GPIO0B0_IE_SHIFT                (0U)
#define PMU1_IOC_GPIO0B_IE_GPIO0B0_IE_MASK                 (0x1U << PMU1_IOC_GPIO0B_IE_GPIO0B0_IE_SHIFT)                /* 0x00000001 */
#define PMU1_IOC_GPIO0B_IE_GPIO0B1_IE_SHIFT                (1U)
#define PMU1_IOC_GPIO0B_IE_GPIO0B1_IE_MASK                 (0x1U << PMU1_IOC_GPIO0B_IE_GPIO0B1_IE_SHIFT)                /* 0x00000002 */
#define PMU1_IOC_GPIO0B_IE_GPIO0B2_IE_SHIFT                (2U)
#define PMU1_IOC_GPIO0B_IE_GPIO0B2_IE_MASK                 (0x1U << PMU1_IOC_GPIO0B_IE_GPIO0B2_IE_SHIFT)                /* 0x00000004 */
#define PMU1_IOC_GPIO0B_IE_GPIO0B3_IE_SHIFT                (3U)
#define PMU1_IOC_GPIO0B_IE_GPIO0B3_IE_MASK                 (0x1U << PMU1_IOC_GPIO0B_IE_GPIO0B3_IE_SHIFT)                /* 0x00000008 */
/* GPIO0A_SMT */
#define PMU1_IOC_GPIO0A_SMT_OFFSET                         (0x30U)
#define PMU1_IOC_GPIO0A_SMT_GPIO0A0_SMT_SHIFT              (0U)
#define PMU1_IOC_GPIO0A_SMT_GPIO0A0_SMT_MASK               (0x1U << PMU1_IOC_GPIO0A_SMT_GPIO0A0_SMT_SHIFT)              /* 0x00000001 */
#define PMU1_IOC_GPIO0A_SMT_GPIO0A1_SMT_SHIFT              (1U)
#define PMU1_IOC_GPIO0A_SMT_GPIO0A1_SMT_MASK               (0x1U << PMU1_IOC_GPIO0A_SMT_GPIO0A1_SMT_SHIFT)              /* 0x00000002 */
#define PMU1_IOC_GPIO0A_SMT_GPIO0A2_SMT_SHIFT              (2U)
#define PMU1_IOC_GPIO0A_SMT_GPIO0A2_SMT_MASK               (0x1U << PMU1_IOC_GPIO0A_SMT_GPIO0A2_SMT_SHIFT)              /* 0x00000004 */
#define PMU1_IOC_GPIO0A_SMT_GPIO0A3_SMT_SHIFT              (3U)
#define PMU1_IOC_GPIO0A_SMT_GPIO0A3_SMT_MASK               (0x1U << PMU1_IOC_GPIO0A_SMT_GPIO0A3_SMT_SHIFT)              /* 0x00000008 */
#define PMU1_IOC_GPIO0A_SMT_GPIO0A4_SMT_SHIFT              (4U)
#define PMU1_IOC_GPIO0A_SMT_GPIO0A4_SMT_MASK               (0x1U << PMU1_IOC_GPIO0A_SMT_GPIO0A4_SMT_SHIFT)              /* 0x00000010 */
#define PMU1_IOC_GPIO0A_SMT_GPIO0A5_SMT_SHIFT              (5U)
#define PMU1_IOC_GPIO0A_SMT_GPIO0A5_SMT_MASK               (0x1U << PMU1_IOC_GPIO0A_SMT_GPIO0A5_SMT_SHIFT)              /* 0x00000020 */
#define PMU1_IOC_GPIO0A_SMT_GPIO0A6_SMT_SHIFT              (6U)
#define PMU1_IOC_GPIO0A_SMT_GPIO0A6_SMT_MASK               (0x1U << PMU1_IOC_GPIO0A_SMT_GPIO0A6_SMT_SHIFT)              /* 0x00000040 */
#define PMU1_IOC_GPIO0A_SMT_GPIO0A7_SMT_SHIFT              (7U)
#define PMU1_IOC_GPIO0A_SMT_GPIO0A7_SMT_MASK               (0x1U << PMU1_IOC_GPIO0A_SMT_GPIO0A7_SMT_SHIFT)              /* 0x00000080 */
/* GPIO0B_SMT */
#define PMU1_IOC_GPIO0B_SMT_OFFSET                         (0x34U)
#define PMU1_IOC_GPIO0B_SMT_GPIO0B0_SMT_SHIFT              (0U)
#define PMU1_IOC_GPIO0B_SMT_GPIO0B0_SMT_MASK               (0x1U << PMU1_IOC_GPIO0B_SMT_GPIO0B0_SMT_SHIFT)              /* 0x00000001 */
#define PMU1_IOC_GPIO0B_SMT_GPIO0B1_SMT_SHIFT              (1U)
#define PMU1_IOC_GPIO0B_SMT_GPIO0B1_SMT_MASK               (0x1U << PMU1_IOC_GPIO0B_SMT_GPIO0B1_SMT_SHIFT)              /* 0x00000002 */
#define PMU1_IOC_GPIO0B_SMT_GPIO0B2_SMT_SHIFT              (2U)
#define PMU1_IOC_GPIO0B_SMT_GPIO0B2_SMT_MASK               (0x1U << PMU1_IOC_GPIO0B_SMT_GPIO0B2_SMT_SHIFT)              /* 0x00000004 */
#define PMU1_IOC_GPIO0B_SMT_GPIO0B3_SMT_SHIFT              (3U)
#define PMU1_IOC_GPIO0B_SMT_GPIO0B3_SMT_MASK               (0x1U << PMU1_IOC_GPIO0B_SMT_GPIO0B3_SMT_SHIFT)              /* 0x00000008 */
/* GPIO0A_PDIS */
#define PMU1_IOC_GPIO0A_PDIS_OFFSET                        (0x38U)
#define PMU1_IOC_GPIO0A_PDIS_GPIO0A0_PULL_DIS_SHIFT        (0U)
#define PMU1_IOC_GPIO0A_PDIS_GPIO0A0_PULL_DIS_MASK         (0x1U << PMU1_IOC_GPIO0A_PDIS_GPIO0A0_PULL_DIS_SHIFT)        /* 0x00000001 */
#define PMU1_IOC_GPIO0A_PDIS_GPIO0A1_PULL_DIS_SHIFT        (1U)
#define PMU1_IOC_GPIO0A_PDIS_GPIO0A1_PULL_DIS_MASK         (0x1U << PMU1_IOC_GPIO0A_PDIS_GPIO0A1_PULL_DIS_SHIFT)        /* 0x00000002 */
#define PMU1_IOC_GPIO0A_PDIS_GPIO0A2_PULL_DIS_SHIFT        (2U)
#define PMU1_IOC_GPIO0A_PDIS_GPIO0A2_PULL_DIS_MASK         (0x1U << PMU1_IOC_GPIO0A_PDIS_GPIO0A2_PULL_DIS_SHIFT)        /* 0x00000004 */
#define PMU1_IOC_GPIO0A_PDIS_GPIO0A3_PULL_DIS_SHIFT        (3U)
#define PMU1_IOC_GPIO0A_PDIS_GPIO0A3_PULL_DIS_MASK         (0x1U << PMU1_IOC_GPIO0A_PDIS_GPIO0A3_PULL_DIS_SHIFT)        /* 0x00000008 */
#define PMU1_IOC_GPIO0A_PDIS_GPIO0A4_PULL_DIS_SHIFT        (4U)
#define PMU1_IOC_GPIO0A_PDIS_GPIO0A4_PULL_DIS_MASK         (0x1U << PMU1_IOC_GPIO0A_PDIS_GPIO0A4_PULL_DIS_SHIFT)        /* 0x00000010 */
#define PMU1_IOC_GPIO0A_PDIS_GPIO0A5_PULL_DIS_SHIFT        (5U)
#define PMU1_IOC_GPIO0A_PDIS_GPIO0A5_PULL_DIS_MASK         (0x1U << PMU1_IOC_GPIO0A_PDIS_GPIO0A5_PULL_DIS_SHIFT)        /* 0x00000020 */
#define PMU1_IOC_GPIO0A_PDIS_GPIO0A6_PULL_DIS_SHIFT        (6U)
#define PMU1_IOC_GPIO0A_PDIS_GPIO0A6_PULL_DIS_MASK         (0x1U << PMU1_IOC_GPIO0A_PDIS_GPIO0A6_PULL_DIS_SHIFT)        /* 0x00000040 */
#define PMU1_IOC_GPIO0A_PDIS_GPIO0A7_PULL_DIS_SHIFT        (7U)
#define PMU1_IOC_GPIO0A_PDIS_GPIO0A7_PULL_DIS_MASK         (0x1U << PMU1_IOC_GPIO0A_PDIS_GPIO0A7_PULL_DIS_SHIFT)        /* 0x00000080 */
/* GPIO0B_PDIS */
#define PMU1_IOC_GPIO0B_PDIS_OFFSET                        (0x3CU)
#define PMU1_IOC_GPIO0B_PDIS_GPIO0B0_PULL_DIS_SHIFT        (0U)
#define PMU1_IOC_GPIO0B_PDIS_GPIO0B0_PULL_DIS_MASK         (0x1U << PMU1_IOC_GPIO0B_PDIS_GPIO0B0_PULL_DIS_SHIFT)        /* 0x00000001 */
#define PMU1_IOC_GPIO0B_PDIS_GPIO0B1_PULL_DIS_SHIFT        (1U)
#define PMU1_IOC_GPIO0B_PDIS_GPIO0B1_PULL_DIS_MASK         (0x1U << PMU1_IOC_GPIO0B_PDIS_GPIO0B1_PULL_DIS_SHIFT)        /* 0x00000002 */
#define PMU1_IOC_GPIO0B_PDIS_GPIO0B2_PULL_DIS_SHIFT        (2U)
#define PMU1_IOC_GPIO0B_PDIS_GPIO0B2_PULL_DIS_MASK         (0x1U << PMU1_IOC_GPIO0B_PDIS_GPIO0B2_PULL_DIS_SHIFT)        /* 0x00000004 */
#define PMU1_IOC_GPIO0B_PDIS_GPIO0B3_PULL_DIS_SHIFT        (3U)
#define PMU1_IOC_GPIO0B_PDIS_GPIO0B3_PULL_DIS_MASK         (0x1U << PMU1_IOC_GPIO0B_PDIS_GPIO0B3_PULL_DIS_SHIFT)        /* 0x00000008 */
/* XIN_CON */
#define PMU1_IOC_XIN_CON_OFFSET                            (0x40U)
#define PMU1_IOC_XIN_CON_XIN_OSC_EN_SHIFT                  (0U)
#define PMU1_IOC_XIN_CON_XIN_OSC_EN_MASK                   (0x1U << PMU1_IOC_XIN_CON_XIN_OSC_EN_SHIFT)                  /* 0x00000001 */
#define PMU1_IOC_XIN_CON_XIN_OSC_SF_SHIFT                  (2U)
#define PMU1_IOC_XIN_CON_XIN_OSC_SF_MASK                   (0x3U << PMU1_IOC_XIN_CON_XIN_OSC_SF_SHIFT)                  /* 0x0000000C */
/****************************************PMU2_IOC****************************************/
/* GPIO0B_IOMUX_SEL_H */
#define PMU2_IOC_GPIO0B_IOMUX_SEL_H_OFFSET                 (0x0U)
#define PMU2_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B5_SEL_SHIFT      (4U)
#define PMU2_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B5_SEL_MASK       (0xFU << PMU2_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B5_SEL_SHIFT)      /* 0x000000F0 */
#define PMU2_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B6_SEL_SHIFT      (8U)
#define PMU2_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B6_SEL_MASK       (0xFU << PMU2_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B6_SEL_SHIFT)      /* 0x00000F00 */
#define PMU2_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B7_SEL_SHIFT      (12U)
#define PMU2_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B7_SEL_MASK       (0xFU << PMU2_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B7_SEL_SHIFT)      /* 0x0000F000 */
/* GPIO0C_IOMUX_SEL_L */
#define PMU2_IOC_GPIO0C_IOMUX_SEL_L_OFFSET                 (0x4U)
#define PMU2_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C0_SEL_SHIFT      (0U)
#define PMU2_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C0_SEL_MASK       (0xFU << PMU2_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C0_SEL_SHIFT)      /* 0x0000000F */
#define PMU2_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C1_SEL_SHIFT      (4U)
#define PMU2_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C1_SEL_MASK       (0xFU << PMU2_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C1_SEL_SHIFT)      /* 0x000000F0 */
#define PMU2_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C2_SEL_SHIFT      (8U)
#define PMU2_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C2_SEL_MASK       (0xFU << PMU2_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C2_SEL_SHIFT)      /* 0x00000F00 */
#define PMU2_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C3_SEL_SHIFT      (12U)
#define PMU2_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C3_SEL_MASK       (0xFU << PMU2_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C3_SEL_SHIFT)      /* 0x0000F000 */
/* GPIO0C_IOMUX_SEL_H */
#define PMU2_IOC_GPIO0C_IOMUX_SEL_H_OFFSET                 (0x8U)
#define PMU2_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C4_SEL_SHIFT      (0U)
#define PMU2_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C4_SEL_MASK       (0xFU << PMU2_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C4_SEL_SHIFT)      /* 0x0000000F */
#define PMU2_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C5_SEL_SHIFT      (4U)
#define PMU2_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C5_SEL_MASK       (0xFU << PMU2_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C5_SEL_SHIFT)      /* 0x000000F0 */
#define PMU2_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C6_SEL_SHIFT      (8U)
#define PMU2_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C6_SEL_MASK       (0xFU << PMU2_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C6_SEL_SHIFT)      /* 0x00000F00 */
#define PMU2_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C7_SEL_SHIFT      (12U)
#define PMU2_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C7_SEL_MASK       (0xFU << PMU2_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C7_SEL_SHIFT)      /* 0x0000F000 */
/* GPIO0D_IOMUX_SEL_L */
#define PMU2_IOC_GPIO0D_IOMUX_SEL_L_OFFSET                 (0xCU)
#define PMU2_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D0_SEL_SHIFT      (0U)
#define PMU2_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D0_SEL_MASK       (0xFU << PMU2_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D0_SEL_SHIFT)      /* 0x0000000F */
#define PMU2_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D1_SEL_SHIFT      (4U)
#define PMU2_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D1_SEL_MASK       (0xFU << PMU2_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D1_SEL_SHIFT)      /* 0x000000F0 */
#define PMU2_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D2_SEL_SHIFT      (8U)
#define PMU2_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D2_SEL_MASK       (0xFU << PMU2_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D2_SEL_SHIFT)      /* 0x00000F00 */
#define PMU2_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D3_SEL_SHIFT      (12U)
#define PMU2_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D3_SEL_MASK       (0xFU << PMU2_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D3_SEL_SHIFT)      /* 0x0000F000 */
/* GPIO0D_IOMUX_SEL_H */
#define PMU2_IOC_GPIO0D_IOMUX_SEL_H_OFFSET                 (0x10U)
#define PMU2_IOC_GPIO0D_IOMUX_SEL_H_GPIO0D4_SEL_SHIFT      (0U)
#define PMU2_IOC_GPIO0D_IOMUX_SEL_H_GPIO0D4_SEL_MASK       (0xFU << PMU2_IOC_GPIO0D_IOMUX_SEL_H_GPIO0D4_SEL_SHIFT)      /* 0x0000000F */
#define PMU2_IOC_GPIO0D_IOMUX_SEL_H_GPIO0D5_SEL_SHIFT      (4U)
#define PMU2_IOC_GPIO0D_IOMUX_SEL_H_GPIO0D5_SEL_MASK       (0xFU << PMU2_IOC_GPIO0D_IOMUX_SEL_H_GPIO0D5_SEL_SHIFT)      /* 0x000000F0 */
#define PMU2_IOC_GPIO0D_IOMUX_SEL_H_GPIO0D6_SEL_SHIFT      (8U)
#define PMU2_IOC_GPIO0D_IOMUX_SEL_H_GPIO0D6_SEL_MASK       (0xFU << PMU2_IOC_GPIO0D_IOMUX_SEL_H_GPIO0D6_SEL_SHIFT)      /* 0x00000F00 */
/* GPIO0B_DS_H */
#define PMU2_IOC_GPIO0B_DS_H_OFFSET                        (0x14U)
#define PMU2_IOC_GPIO0B_DS_H_GPIO0B5_DS_SHIFT              (4U)
#define PMU2_IOC_GPIO0B_DS_H_GPIO0B5_DS_MASK               (0x7U << PMU2_IOC_GPIO0B_DS_H_GPIO0B5_DS_SHIFT)              /* 0x00000070 */
#define PMU2_IOC_GPIO0B_DS_H_GPIO0B6_DS_SHIFT              (8U)
#define PMU2_IOC_GPIO0B_DS_H_GPIO0B6_DS_MASK               (0x7U << PMU2_IOC_GPIO0B_DS_H_GPIO0B6_DS_SHIFT)              /* 0x00000700 */
#define PMU2_IOC_GPIO0B_DS_H_GPIO0B7_DS_SHIFT              (12U)
#define PMU2_IOC_GPIO0B_DS_H_GPIO0B7_DS_MASK               (0x7U << PMU2_IOC_GPIO0B_DS_H_GPIO0B7_DS_SHIFT)              /* 0x00007000 */
/* GPIO0C_DS_L */
#define PMU2_IOC_GPIO0C_DS_L_OFFSET                        (0x18U)
#define PMU2_IOC_GPIO0C_DS_L_GPIO0C0_DS_SHIFT              (0U)
#define PMU2_IOC_GPIO0C_DS_L_GPIO0C0_DS_MASK               (0x7U << PMU2_IOC_GPIO0C_DS_L_GPIO0C0_DS_SHIFT)              /* 0x00000007 */
#define PMU2_IOC_GPIO0C_DS_L_GPIO0C1_DS_SHIFT              (4U)
#define PMU2_IOC_GPIO0C_DS_L_GPIO0C1_DS_MASK               (0x7U << PMU2_IOC_GPIO0C_DS_L_GPIO0C1_DS_SHIFT)              /* 0x00000070 */
#define PMU2_IOC_GPIO0C_DS_L_GPIO0C2_DS_SHIFT              (8U)
#define PMU2_IOC_GPIO0C_DS_L_GPIO0C2_DS_MASK               (0x7U << PMU2_IOC_GPIO0C_DS_L_GPIO0C2_DS_SHIFT)              /* 0x00000700 */
#define PMU2_IOC_GPIO0C_DS_L_GPIO0C3_DS_SHIFT              (12U)
#define PMU2_IOC_GPIO0C_DS_L_GPIO0C3_DS_MASK               (0x7U << PMU2_IOC_GPIO0C_DS_L_GPIO0C3_DS_SHIFT)              /* 0x00007000 */
/* GPIO0C_DS_H */
#define PMU2_IOC_GPIO0C_DS_H_OFFSET                        (0x1CU)
#define PMU2_IOC_GPIO0C_DS_H_GPIO0C4_DS_SHIFT              (0U)
#define PMU2_IOC_GPIO0C_DS_H_GPIO0C4_DS_MASK               (0x7U << PMU2_IOC_GPIO0C_DS_H_GPIO0C4_DS_SHIFT)              /* 0x00000007 */
#define PMU2_IOC_GPIO0C_DS_H_GPIO0C5_DS_SHIFT              (4U)
#define PMU2_IOC_GPIO0C_DS_H_GPIO0C5_DS_MASK               (0x7U << PMU2_IOC_GPIO0C_DS_H_GPIO0C5_DS_SHIFT)              /* 0x00000070 */
#define PMU2_IOC_GPIO0C_DS_H_GPIO0C6_DS_SHIFT              (8U)
#define PMU2_IOC_GPIO0C_DS_H_GPIO0C6_DS_MASK               (0x7U << PMU2_IOC_GPIO0C_DS_H_GPIO0C6_DS_SHIFT)              /* 0x00000700 */
#define PMU2_IOC_GPIO0C_DS_H_GPIO0C7_DS_SHIFT              (12U)
#define PMU2_IOC_GPIO0C_DS_H_GPIO0C7_DS_MASK               (0x7U << PMU2_IOC_GPIO0C_DS_H_GPIO0C7_DS_SHIFT)              /* 0x00007000 */
/* GPIO0D_DS_L */
#define PMU2_IOC_GPIO0D_DS_L_OFFSET                        (0x20U)
#define PMU2_IOC_GPIO0D_DS_L_GPIO0D0_DS_SHIFT              (0U)
#define PMU2_IOC_GPIO0D_DS_L_GPIO0D0_DS_MASK               (0x7U << PMU2_IOC_GPIO0D_DS_L_GPIO0D0_DS_SHIFT)              /* 0x00000007 */
#define PMU2_IOC_GPIO0D_DS_L_GPIO0D1_DS_SHIFT              (4U)
#define PMU2_IOC_GPIO0D_DS_L_GPIO0D1_DS_MASK               (0x7U << PMU2_IOC_GPIO0D_DS_L_GPIO0D1_DS_SHIFT)              /* 0x00000070 */
#define PMU2_IOC_GPIO0D_DS_L_GPIO0D2_DS_SHIFT              (8U)
#define PMU2_IOC_GPIO0D_DS_L_GPIO0D2_DS_MASK               (0x7U << PMU2_IOC_GPIO0D_DS_L_GPIO0D2_DS_SHIFT)              /* 0x00000700 */
#define PMU2_IOC_GPIO0D_DS_L_GPIO0D3_DS_SHIFT              (12U)
#define PMU2_IOC_GPIO0D_DS_L_GPIO0D3_DS_MASK               (0x7U << PMU2_IOC_GPIO0D_DS_L_GPIO0D3_DS_SHIFT)              /* 0x00007000 */
/* GPIO0D_DS_H */
#define PMU2_IOC_GPIO0D_DS_H_OFFSET                        (0x24U)
#define PMU2_IOC_GPIO0D_DS_H_GPIO0D4_DS_SHIFT              (0U)
#define PMU2_IOC_GPIO0D_DS_H_GPIO0D4_DS_MASK               (0x7U << PMU2_IOC_GPIO0D_DS_H_GPIO0D4_DS_SHIFT)              /* 0x00000007 */
#define PMU2_IOC_GPIO0D_DS_H_GPIO0D5_DS_SHIFT              (4U)
#define PMU2_IOC_GPIO0D_DS_H_GPIO0D5_DS_MASK               (0x7U << PMU2_IOC_GPIO0D_DS_H_GPIO0D5_DS_SHIFT)              /* 0x00000070 */
#define PMU2_IOC_GPIO0D_DS_H_GPIO0D6_DS_SHIFT              (8U)
#define PMU2_IOC_GPIO0D_DS_H_GPIO0D6_DS_MASK               (0x7U << PMU2_IOC_GPIO0D_DS_H_GPIO0D6_DS_SHIFT)              /* 0x00000700 */
/* GPIO0B_P */
#define PMU2_IOC_GPIO0B_P_OFFSET                           (0x28U)
#define PMU2_IOC_GPIO0B_P_GPIO0B5_PE_SHIFT                 (10U)
#define PMU2_IOC_GPIO0B_P_GPIO0B5_PE_MASK                  (0x1U << PMU2_IOC_GPIO0B_P_GPIO0B5_PE_SHIFT)                 /* 0x00000400 */
#define PMU2_IOC_GPIO0B_P_GPIO0B5_PS_SHIFT                 (11U)
#define PMU2_IOC_GPIO0B_P_GPIO0B5_PS_MASK                  (0x1U << PMU2_IOC_GPIO0B_P_GPIO0B5_PS_SHIFT)                 /* 0x00000800 */
#define PMU2_IOC_GPIO0B_P_GPIO0B6_PE_SHIFT                 (12U)
#define PMU2_IOC_GPIO0B_P_GPIO0B6_PE_MASK                  (0x1U << PMU2_IOC_GPIO0B_P_GPIO0B6_PE_SHIFT)                 /* 0x00001000 */
#define PMU2_IOC_GPIO0B_P_GPIO0B6_PS_SHIFT                 (13U)
#define PMU2_IOC_GPIO0B_P_GPIO0B6_PS_MASK                  (0x1U << PMU2_IOC_GPIO0B_P_GPIO0B6_PS_SHIFT)                 /* 0x00002000 */
#define PMU2_IOC_GPIO0B_P_GPIO0B7_PE_SHIFT                 (14U)
#define PMU2_IOC_GPIO0B_P_GPIO0B7_PE_MASK                  (0x1U << PMU2_IOC_GPIO0B_P_GPIO0B7_PE_SHIFT)                 /* 0x00004000 */
#define PMU2_IOC_GPIO0B_P_GPIO0B7_PS_SHIFT                 (15U)
#define PMU2_IOC_GPIO0B_P_GPIO0B7_PS_MASK                  (0x1U << PMU2_IOC_GPIO0B_P_GPIO0B7_PS_SHIFT)                 /* 0x00008000 */
/* GPIO0C_P */
#define PMU2_IOC_GPIO0C_P_OFFSET                           (0x2CU)
#define PMU2_IOC_GPIO0C_P_GPIO0C0_PE_SHIFT                 (0U)
#define PMU2_IOC_GPIO0C_P_GPIO0C0_PE_MASK                  (0x1U << PMU2_IOC_GPIO0C_P_GPIO0C0_PE_SHIFT)                 /* 0x00000001 */
#define PMU2_IOC_GPIO0C_P_GPIO0C0_PS_SHIFT                 (1U)
#define PMU2_IOC_GPIO0C_P_GPIO0C0_PS_MASK                  (0x1U << PMU2_IOC_GPIO0C_P_GPIO0C0_PS_SHIFT)                 /* 0x00000002 */
#define PMU2_IOC_GPIO0C_P_GPIO0C1_PE_SHIFT                 (2U)
#define PMU2_IOC_GPIO0C_P_GPIO0C1_PE_MASK                  (0x1U << PMU2_IOC_GPIO0C_P_GPIO0C1_PE_SHIFT)                 /* 0x00000004 */
#define PMU2_IOC_GPIO0C_P_GPIO0C1_PS_SHIFT                 (3U)
#define PMU2_IOC_GPIO0C_P_GPIO0C1_PS_MASK                  (0x1U << PMU2_IOC_GPIO0C_P_GPIO0C1_PS_SHIFT)                 /* 0x00000008 */
#define PMU2_IOC_GPIO0C_P_GPIO0C2_PE_SHIFT                 (4U)
#define PMU2_IOC_GPIO0C_P_GPIO0C2_PE_MASK                  (0x1U << PMU2_IOC_GPIO0C_P_GPIO0C2_PE_SHIFT)                 /* 0x00000010 */
#define PMU2_IOC_GPIO0C_P_GPIO0C2_PS_SHIFT                 (5U)
#define PMU2_IOC_GPIO0C_P_GPIO0C2_PS_MASK                  (0x1U << PMU2_IOC_GPIO0C_P_GPIO0C2_PS_SHIFT)                 /* 0x00000020 */
#define PMU2_IOC_GPIO0C_P_GPIO0C3_PE_SHIFT                 (6U)
#define PMU2_IOC_GPIO0C_P_GPIO0C3_PE_MASK                  (0x1U << PMU2_IOC_GPIO0C_P_GPIO0C3_PE_SHIFT)                 /* 0x00000040 */
#define PMU2_IOC_GPIO0C_P_GPIO0C3_PS_SHIFT                 (7U)
#define PMU2_IOC_GPIO0C_P_GPIO0C3_PS_MASK                  (0x1U << PMU2_IOC_GPIO0C_P_GPIO0C3_PS_SHIFT)                 /* 0x00000080 */
#define PMU2_IOC_GPIO0C_P_GPIO0C4_PE_SHIFT                 (8U)
#define PMU2_IOC_GPIO0C_P_GPIO0C4_PE_MASK                  (0x1U << PMU2_IOC_GPIO0C_P_GPIO0C4_PE_SHIFT)                 /* 0x00000100 */
#define PMU2_IOC_GPIO0C_P_GPIO0C4_PS_SHIFT                 (9U)
#define PMU2_IOC_GPIO0C_P_GPIO0C4_PS_MASK                  (0x1U << PMU2_IOC_GPIO0C_P_GPIO0C4_PS_SHIFT)                 /* 0x00000200 */
#define PMU2_IOC_GPIO0C_P_GPIO0C5_PE_SHIFT                 (10U)
#define PMU2_IOC_GPIO0C_P_GPIO0C5_PE_MASK                  (0x1U << PMU2_IOC_GPIO0C_P_GPIO0C5_PE_SHIFT)                 /* 0x00000400 */
#define PMU2_IOC_GPIO0C_P_GPIO0C5_PS_SHIFT                 (11U)
#define PMU2_IOC_GPIO0C_P_GPIO0C5_PS_MASK                  (0x1U << PMU2_IOC_GPIO0C_P_GPIO0C5_PS_SHIFT)                 /* 0x00000800 */
#define PMU2_IOC_GPIO0C_P_GPIO0C6_PE_SHIFT                 (12U)
#define PMU2_IOC_GPIO0C_P_GPIO0C6_PE_MASK                  (0x1U << PMU2_IOC_GPIO0C_P_GPIO0C6_PE_SHIFT)                 /* 0x00001000 */
#define PMU2_IOC_GPIO0C_P_GPIO0C6_PS_SHIFT                 (13U)
#define PMU2_IOC_GPIO0C_P_GPIO0C6_PS_MASK                  (0x1U << PMU2_IOC_GPIO0C_P_GPIO0C6_PS_SHIFT)                 /* 0x00002000 */
#define PMU2_IOC_GPIO0C_P_GPIO0C7_PE_SHIFT                 (14U)
#define PMU2_IOC_GPIO0C_P_GPIO0C7_PE_MASK                  (0x1U << PMU2_IOC_GPIO0C_P_GPIO0C7_PE_SHIFT)                 /* 0x00004000 */
#define PMU2_IOC_GPIO0C_P_GPIO0C7_PS_SHIFT                 (15U)
#define PMU2_IOC_GPIO0C_P_GPIO0C7_PS_MASK                  (0x1U << PMU2_IOC_GPIO0C_P_GPIO0C7_PS_SHIFT)                 /* 0x00008000 */
/* GPIO0D_P */
#define PMU2_IOC_GPIO0D_P_OFFSET                           (0x30U)
#define PMU2_IOC_GPIO0D_P_GPIO0D0_PE_SHIFT                 (0U)
#define PMU2_IOC_GPIO0D_P_GPIO0D0_PE_MASK                  (0x1U << PMU2_IOC_GPIO0D_P_GPIO0D0_PE_SHIFT)                 /* 0x00000001 */
#define PMU2_IOC_GPIO0D_P_GPIO0D0_PS_SHIFT                 (1U)
#define PMU2_IOC_GPIO0D_P_GPIO0D0_PS_MASK                  (0x1U << PMU2_IOC_GPIO0D_P_GPIO0D0_PS_SHIFT)                 /* 0x00000002 */
#define PMU2_IOC_GPIO0D_P_GPIO0D1_PE_SHIFT                 (2U)
#define PMU2_IOC_GPIO0D_P_GPIO0D1_PE_MASK                  (0x1U << PMU2_IOC_GPIO0D_P_GPIO0D1_PE_SHIFT)                 /* 0x00000004 */
#define PMU2_IOC_GPIO0D_P_GPIO0D1_PS_SHIFT                 (3U)
#define PMU2_IOC_GPIO0D_P_GPIO0D1_PS_MASK                  (0x1U << PMU2_IOC_GPIO0D_P_GPIO0D1_PS_SHIFT)                 /* 0x00000008 */
#define PMU2_IOC_GPIO0D_P_GPIO0D2_PE_SHIFT                 (4U)
#define PMU2_IOC_GPIO0D_P_GPIO0D2_PE_MASK                  (0x1U << PMU2_IOC_GPIO0D_P_GPIO0D2_PE_SHIFT)                 /* 0x00000010 */
#define PMU2_IOC_GPIO0D_P_GPIO0D2_PS_SHIFT                 (5U)
#define PMU2_IOC_GPIO0D_P_GPIO0D2_PS_MASK                  (0x1U << PMU2_IOC_GPIO0D_P_GPIO0D2_PS_SHIFT)                 /* 0x00000020 */
#define PMU2_IOC_GPIO0D_P_GPIO0D3_PE_SHIFT                 (6U)
#define PMU2_IOC_GPIO0D_P_GPIO0D3_PE_MASK                  (0x1U << PMU2_IOC_GPIO0D_P_GPIO0D3_PE_SHIFT)                 /* 0x00000040 */
#define PMU2_IOC_GPIO0D_P_GPIO0D3_PS_SHIFT                 (7U)
#define PMU2_IOC_GPIO0D_P_GPIO0D3_PS_MASK                  (0x1U << PMU2_IOC_GPIO0D_P_GPIO0D3_PS_SHIFT)                 /* 0x00000080 */
#define PMU2_IOC_GPIO0D_P_GPIO0D4_PE_SHIFT                 (8U)
#define PMU2_IOC_GPIO0D_P_GPIO0D4_PE_MASK                  (0x1U << PMU2_IOC_GPIO0D_P_GPIO0D4_PE_SHIFT)                 /* 0x00000100 */
#define PMU2_IOC_GPIO0D_P_GPIO0D4_PS_SHIFT                 (9U)
#define PMU2_IOC_GPIO0D_P_GPIO0D4_PS_MASK                  (0x1U << PMU2_IOC_GPIO0D_P_GPIO0D4_PS_SHIFT)                 /* 0x00000200 */
#define PMU2_IOC_GPIO0D_P_GPIO0D5_PE_SHIFT                 (10U)
#define PMU2_IOC_GPIO0D_P_GPIO0D5_PE_MASK                  (0x1U << PMU2_IOC_GPIO0D_P_GPIO0D5_PE_SHIFT)                 /* 0x00000400 */
#define PMU2_IOC_GPIO0D_P_GPIO0D5_PS_SHIFT                 (11U)
#define PMU2_IOC_GPIO0D_P_GPIO0D5_PS_MASK                  (0x1U << PMU2_IOC_GPIO0D_P_GPIO0D5_PS_SHIFT)                 /* 0x00000800 */
#define PMU2_IOC_GPIO0D_P_GPIO0D6_PE_SHIFT                 (12U)
#define PMU2_IOC_GPIO0D_P_GPIO0D6_PE_MASK                  (0x1U << PMU2_IOC_GPIO0D_P_GPIO0D6_PE_SHIFT)                 /* 0x00001000 */
#define PMU2_IOC_GPIO0D_P_GPIO0D6_PS_SHIFT                 (13U)
#define PMU2_IOC_GPIO0D_P_GPIO0D6_PS_MASK                  (0x1U << PMU2_IOC_GPIO0D_P_GPIO0D6_PS_SHIFT)                 /* 0x00002000 */
/* GPIO0B_IE */
#define PMU2_IOC_GPIO0B_IE_OFFSET                          (0x34U)
#define PMU2_IOC_GPIO0B_IE_GPIO0B5_IE_SHIFT                (5U)
#define PMU2_IOC_GPIO0B_IE_GPIO0B5_IE_MASK                 (0x1U << PMU2_IOC_GPIO0B_IE_GPIO0B5_IE_SHIFT)                /* 0x00000020 */
#define PMU2_IOC_GPIO0B_IE_GPIO0B6_IE_SHIFT                (6U)
#define PMU2_IOC_GPIO0B_IE_GPIO0B6_IE_MASK                 (0x1U << PMU2_IOC_GPIO0B_IE_GPIO0B6_IE_SHIFT)                /* 0x00000040 */
#define PMU2_IOC_GPIO0B_IE_GPIO0B7_IE_SHIFT                (7U)
#define PMU2_IOC_GPIO0B_IE_GPIO0B7_IE_MASK                 (0x1U << PMU2_IOC_GPIO0B_IE_GPIO0B7_IE_SHIFT)                /* 0x00000080 */
/* GPIO0C_IE */
#define PMU2_IOC_GPIO0C_IE_OFFSET                          (0x38U)
#define PMU2_IOC_GPIO0C_IE_GPIO0C0_IE_SHIFT                (0U)
#define PMU2_IOC_GPIO0C_IE_GPIO0C0_IE_MASK                 (0x1U << PMU2_IOC_GPIO0C_IE_GPIO0C0_IE_SHIFT)                /* 0x00000001 */
#define PMU2_IOC_GPIO0C_IE_GPIO0C1_IE_SHIFT                (1U)
#define PMU2_IOC_GPIO0C_IE_GPIO0C1_IE_MASK                 (0x1U << PMU2_IOC_GPIO0C_IE_GPIO0C1_IE_SHIFT)                /* 0x00000002 */
#define PMU2_IOC_GPIO0C_IE_GPIO0C2_IE_SHIFT                (2U)
#define PMU2_IOC_GPIO0C_IE_GPIO0C2_IE_MASK                 (0x1U << PMU2_IOC_GPIO0C_IE_GPIO0C2_IE_SHIFT)                /* 0x00000004 */
#define PMU2_IOC_GPIO0C_IE_GPIO0C3_IE_SHIFT                (3U)
#define PMU2_IOC_GPIO0C_IE_GPIO0C3_IE_MASK                 (0x1U << PMU2_IOC_GPIO0C_IE_GPIO0C3_IE_SHIFT)                /* 0x00000008 */
#define PMU2_IOC_GPIO0C_IE_GPIO0C4_IE_SHIFT                (4U)
#define PMU2_IOC_GPIO0C_IE_GPIO0C4_IE_MASK                 (0x1U << PMU2_IOC_GPIO0C_IE_GPIO0C4_IE_SHIFT)                /* 0x00000010 */
#define PMU2_IOC_GPIO0C_IE_GPIO0C5_IE_SHIFT                (5U)
#define PMU2_IOC_GPIO0C_IE_GPIO0C5_IE_MASK                 (0x1U << PMU2_IOC_GPIO0C_IE_GPIO0C5_IE_SHIFT)                /* 0x00000020 */
#define PMU2_IOC_GPIO0C_IE_GPIO0C6_IE_SHIFT                (6U)
#define PMU2_IOC_GPIO0C_IE_GPIO0C6_IE_MASK                 (0x1U << PMU2_IOC_GPIO0C_IE_GPIO0C6_IE_SHIFT)                /* 0x00000040 */
#define PMU2_IOC_GPIO0C_IE_GPIO0C7_IE_SHIFT                (7U)
#define PMU2_IOC_GPIO0C_IE_GPIO0C7_IE_MASK                 (0x1U << PMU2_IOC_GPIO0C_IE_GPIO0C7_IE_SHIFT)                /* 0x00000080 */
/* GPIO0D_IE */
#define PMU2_IOC_GPIO0D_IE_OFFSET                          (0x3CU)
#define PMU2_IOC_GPIO0D_IE_GPIO0D0_IE_SHIFT                (0U)
#define PMU2_IOC_GPIO0D_IE_GPIO0D0_IE_MASK                 (0x1U << PMU2_IOC_GPIO0D_IE_GPIO0D0_IE_SHIFT)                /* 0x00000001 */
#define PMU2_IOC_GPIO0D_IE_GPIO0D1_IE_SHIFT                (1U)
#define PMU2_IOC_GPIO0D_IE_GPIO0D1_IE_MASK                 (0x1U << PMU2_IOC_GPIO0D_IE_GPIO0D1_IE_SHIFT)                /* 0x00000002 */
#define PMU2_IOC_GPIO0D_IE_GPIO0D2_IE_SHIFT                (2U)
#define PMU2_IOC_GPIO0D_IE_GPIO0D2_IE_MASK                 (0x1U << PMU2_IOC_GPIO0D_IE_GPIO0D2_IE_SHIFT)                /* 0x00000004 */
#define PMU2_IOC_GPIO0D_IE_GPIO0D3_IE_SHIFT                (3U)
#define PMU2_IOC_GPIO0D_IE_GPIO0D3_IE_MASK                 (0x1U << PMU2_IOC_GPIO0D_IE_GPIO0D3_IE_SHIFT)                /* 0x00000008 */
#define PMU2_IOC_GPIO0D_IE_GPIO0D4_IE_SHIFT                (4U)
#define PMU2_IOC_GPIO0D_IE_GPIO0D4_IE_MASK                 (0x1U << PMU2_IOC_GPIO0D_IE_GPIO0D4_IE_SHIFT)                /* 0x00000010 */
#define PMU2_IOC_GPIO0D_IE_GPIO0D5_IE_SHIFT                (5U)
#define PMU2_IOC_GPIO0D_IE_GPIO0D5_IE_MASK                 (0x1U << PMU2_IOC_GPIO0D_IE_GPIO0D5_IE_SHIFT)                /* 0x00000020 */
#define PMU2_IOC_GPIO0D_IE_GPIO0D6_IE_SHIFT                (6U)
#define PMU2_IOC_GPIO0D_IE_GPIO0D6_IE_MASK                 (0x1U << PMU2_IOC_GPIO0D_IE_GPIO0D6_IE_SHIFT)                /* 0x00000040 */
/* GPIO0B_SMT */
#define PMU2_IOC_GPIO0B_SMT_OFFSET                         (0x40U)
#define PMU2_IOC_GPIO0B_SMT_GPIO0B5_SMT_SHIFT              (5U)
#define PMU2_IOC_GPIO0B_SMT_GPIO0B5_SMT_MASK               (0x1U << PMU2_IOC_GPIO0B_SMT_GPIO0B5_SMT_SHIFT)              /* 0x00000020 */
#define PMU2_IOC_GPIO0B_SMT_GPIO0B6_SMT_SHIFT              (6U)
#define PMU2_IOC_GPIO0B_SMT_GPIO0B6_SMT_MASK               (0x1U << PMU2_IOC_GPIO0B_SMT_GPIO0B6_SMT_SHIFT)              /* 0x00000040 */
#define PMU2_IOC_GPIO0B_SMT_GPIO0B7_SMT_SHIFT              (7U)
#define PMU2_IOC_GPIO0B_SMT_GPIO0B7_SMT_MASK               (0x1U << PMU2_IOC_GPIO0B_SMT_GPIO0B7_SMT_SHIFT)              /* 0x00000080 */
/* GPIO0C_SMT */
#define PMU2_IOC_GPIO0C_SMT_OFFSET                         (0x44U)
#define PMU2_IOC_GPIO0C_SMT_GPIO0C0_SMT_SHIFT              (0U)
#define PMU2_IOC_GPIO0C_SMT_GPIO0C0_SMT_MASK               (0x1U << PMU2_IOC_GPIO0C_SMT_GPIO0C0_SMT_SHIFT)              /* 0x00000001 */
#define PMU2_IOC_GPIO0C_SMT_GPIO0C1_SMT_SHIFT              (1U)
#define PMU2_IOC_GPIO0C_SMT_GPIO0C1_SMT_MASK               (0x1U << PMU2_IOC_GPIO0C_SMT_GPIO0C1_SMT_SHIFT)              /* 0x00000002 */
#define PMU2_IOC_GPIO0C_SMT_GPIO0C2_SMT_SHIFT              (2U)
#define PMU2_IOC_GPIO0C_SMT_GPIO0C2_SMT_MASK               (0x1U << PMU2_IOC_GPIO0C_SMT_GPIO0C2_SMT_SHIFT)              /* 0x00000004 */
#define PMU2_IOC_GPIO0C_SMT_GPIO0C3_SMT_SHIFT              (3U)
#define PMU2_IOC_GPIO0C_SMT_GPIO0C3_SMT_MASK               (0x1U << PMU2_IOC_GPIO0C_SMT_GPIO0C3_SMT_SHIFT)              /* 0x00000008 */
#define PMU2_IOC_GPIO0C_SMT_GPIO0C4_SMT_SHIFT              (4U)
#define PMU2_IOC_GPIO0C_SMT_GPIO0C4_SMT_MASK               (0x1U << PMU2_IOC_GPIO0C_SMT_GPIO0C4_SMT_SHIFT)              /* 0x00000010 */
#define PMU2_IOC_GPIO0C_SMT_GPIO0C5_SMT_SHIFT              (5U)
#define PMU2_IOC_GPIO0C_SMT_GPIO0C5_SMT_MASK               (0x1U << PMU2_IOC_GPIO0C_SMT_GPIO0C5_SMT_SHIFT)              /* 0x00000020 */
#define PMU2_IOC_GPIO0C_SMT_GPIO0C6_SMT_SHIFT              (6U)
#define PMU2_IOC_GPIO0C_SMT_GPIO0C6_SMT_MASK               (0x1U << PMU2_IOC_GPIO0C_SMT_GPIO0C6_SMT_SHIFT)              /* 0x00000040 */
#define PMU2_IOC_GPIO0C_SMT_GPIO0C7_SMT_SHIFT              (7U)
#define PMU2_IOC_GPIO0C_SMT_GPIO0C7_SMT_MASK               (0x1U << PMU2_IOC_GPIO0C_SMT_GPIO0C7_SMT_SHIFT)              /* 0x00000080 */
/* GPIO0D_SMT */
#define PMU2_IOC_GPIO0D_SMT_OFFSET                         (0x48U)
#define PMU2_IOC_GPIO0D_SMT_GPIO0D0_SMT_SHIFT              (0U)
#define PMU2_IOC_GPIO0D_SMT_GPIO0D0_SMT_MASK               (0x1U << PMU2_IOC_GPIO0D_SMT_GPIO0D0_SMT_SHIFT)              /* 0x00000001 */
#define PMU2_IOC_GPIO0D_SMT_GPIO0D1_SMT_SHIFT              (1U)
#define PMU2_IOC_GPIO0D_SMT_GPIO0D1_SMT_MASK               (0x1U << PMU2_IOC_GPIO0D_SMT_GPIO0D1_SMT_SHIFT)              /* 0x00000002 */
#define PMU2_IOC_GPIO0D_SMT_GPIO0D2_SMT_SHIFT              (2U)
#define PMU2_IOC_GPIO0D_SMT_GPIO0D2_SMT_MASK               (0x1U << PMU2_IOC_GPIO0D_SMT_GPIO0D2_SMT_SHIFT)              /* 0x00000004 */
#define PMU2_IOC_GPIO0D_SMT_GPIO0D3_SMT_SHIFT              (3U)
#define PMU2_IOC_GPIO0D_SMT_GPIO0D3_SMT_MASK               (0x1U << PMU2_IOC_GPIO0D_SMT_GPIO0D3_SMT_SHIFT)              /* 0x00000008 */
#define PMU2_IOC_GPIO0D_SMT_GPIO0D4_SMT_SHIFT              (4U)
#define PMU2_IOC_GPIO0D_SMT_GPIO0D4_SMT_MASK               (0x1U << PMU2_IOC_GPIO0D_SMT_GPIO0D4_SMT_SHIFT)              /* 0x00000010 */
#define PMU2_IOC_GPIO0D_SMT_GPIO0D5_SMT_SHIFT              (5U)
#define PMU2_IOC_GPIO0D_SMT_GPIO0D5_SMT_MASK               (0x1U << PMU2_IOC_GPIO0D_SMT_GPIO0D5_SMT_SHIFT)              /* 0x00000020 */
#define PMU2_IOC_GPIO0D_SMT_GPIO0D6_SMT_SHIFT              (6U)
#define PMU2_IOC_GPIO0D_SMT_GPIO0D6_SMT_MASK               (0x1U << PMU2_IOC_GPIO0D_SMT_GPIO0D6_SMT_SHIFT)              /* 0x00000040 */
/* GPIO0B_PDIS */
#define PMU2_IOC_GPIO0B_PDIS_OFFSET                        (0x4CU)
#define PMU2_IOC_GPIO0B_PDIS_GPIO0B5_PULL_DIS_SHIFT        (5U)
#define PMU2_IOC_GPIO0B_PDIS_GPIO0B5_PULL_DIS_MASK         (0x1U << PMU2_IOC_GPIO0B_PDIS_GPIO0B5_PULL_DIS_SHIFT)        /* 0x00000020 */
#define PMU2_IOC_GPIO0B_PDIS_GPIO0B6_PULL_DIS_SHIFT        (6U)
#define PMU2_IOC_GPIO0B_PDIS_GPIO0B6_PULL_DIS_MASK         (0x1U << PMU2_IOC_GPIO0B_PDIS_GPIO0B6_PULL_DIS_SHIFT)        /* 0x00000040 */
#define PMU2_IOC_GPIO0B_PDIS_GPIO0B7_PULL_DIS_SHIFT        (7U)
#define PMU2_IOC_GPIO0B_PDIS_GPIO0B7_PULL_DIS_MASK         (0x1U << PMU2_IOC_GPIO0B_PDIS_GPIO0B7_PULL_DIS_SHIFT)        /* 0x00000080 */
/* GPIO0C_PDIS */
#define PMU2_IOC_GPIO0C_PDIS_OFFSET                        (0x50U)
#define PMU2_IOC_GPIO0C_PDIS_GPIO0C0_PULL_DIS_SHIFT        (0U)
#define PMU2_IOC_GPIO0C_PDIS_GPIO0C0_PULL_DIS_MASK         (0x1U << PMU2_IOC_GPIO0C_PDIS_GPIO0C0_PULL_DIS_SHIFT)        /* 0x00000001 */
#define PMU2_IOC_GPIO0C_PDIS_GPIO0C1_PULL_DIS_SHIFT        (1U)
#define PMU2_IOC_GPIO0C_PDIS_GPIO0C1_PULL_DIS_MASK         (0x1U << PMU2_IOC_GPIO0C_PDIS_GPIO0C1_PULL_DIS_SHIFT)        /* 0x00000002 */
#define PMU2_IOC_GPIO0C_PDIS_GPIO0C2_PULL_DIS_SHIFT        (2U)
#define PMU2_IOC_GPIO0C_PDIS_GPIO0C2_PULL_DIS_MASK         (0x1U << PMU2_IOC_GPIO0C_PDIS_GPIO0C2_PULL_DIS_SHIFT)        /* 0x00000004 */
#define PMU2_IOC_GPIO0C_PDIS_GPIO0C3_PULL_DIS_SHIFT        (3U)
#define PMU2_IOC_GPIO0C_PDIS_GPIO0C3_PULL_DIS_MASK         (0x1U << PMU2_IOC_GPIO0C_PDIS_GPIO0C3_PULL_DIS_SHIFT)        /* 0x00000008 */
#define PMU2_IOC_GPIO0C_PDIS_GPIO0C4_PULL_DIS_SHIFT        (4U)
#define PMU2_IOC_GPIO0C_PDIS_GPIO0C4_PULL_DIS_MASK         (0x1U << PMU2_IOC_GPIO0C_PDIS_GPIO0C4_PULL_DIS_SHIFT)        /* 0x00000010 */
#define PMU2_IOC_GPIO0C_PDIS_GPIO0C5_PULL_DIS_SHIFT        (5U)
#define PMU2_IOC_GPIO0C_PDIS_GPIO0C5_PULL_DIS_MASK         (0x1U << PMU2_IOC_GPIO0C_PDIS_GPIO0C5_PULL_DIS_SHIFT)        /* 0x00000020 */
#define PMU2_IOC_GPIO0C_PDIS_GPIO0C6_PULL_DIS_SHIFT        (6U)
#define PMU2_IOC_GPIO0C_PDIS_GPIO0C6_PULL_DIS_MASK         (0x1U << PMU2_IOC_GPIO0C_PDIS_GPIO0C6_PULL_DIS_SHIFT)        /* 0x00000040 */
#define PMU2_IOC_GPIO0C_PDIS_GPIO0C7_PULL_DIS_SHIFT        (7U)
#define PMU2_IOC_GPIO0C_PDIS_GPIO0C7_PULL_DIS_MASK         (0x1U << PMU2_IOC_GPIO0C_PDIS_GPIO0C7_PULL_DIS_SHIFT)        /* 0x00000080 */
/* GPIO0D_PDIS */
#define PMU2_IOC_GPIO0D_PDIS_OFFSET                        (0x54U)
#define PMU2_IOC_GPIO0D_PDIS_GPIO0D0_PULL_DIS_SHIFT        (0U)
#define PMU2_IOC_GPIO0D_PDIS_GPIO0D0_PULL_DIS_MASK         (0x1U << PMU2_IOC_GPIO0D_PDIS_GPIO0D0_PULL_DIS_SHIFT)        /* 0x00000001 */
#define PMU2_IOC_GPIO0D_PDIS_GPIO0D1_PULL_DIS_SHIFT        (1U)
#define PMU2_IOC_GPIO0D_PDIS_GPIO0D1_PULL_DIS_MASK         (0x1U << PMU2_IOC_GPIO0D_PDIS_GPIO0D1_PULL_DIS_SHIFT)        /* 0x00000002 */
#define PMU2_IOC_GPIO0D_PDIS_GPIO0D2_PULL_DIS_SHIFT        (2U)
#define PMU2_IOC_GPIO0D_PDIS_GPIO0D2_PULL_DIS_MASK         (0x1U << PMU2_IOC_GPIO0D_PDIS_GPIO0D2_PULL_DIS_SHIFT)        /* 0x00000004 */
#define PMU2_IOC_GPIO0D_PDIS_GPIO0D3_PULL_DIS_SHIFT        (3U)
#define PMU2_IOC_GPIO0D_PDIS_GPIO0D3_PULL_DIS_MASK         (0x1U << PMU2_IOC_GPIO0D_PDIS_GPIO0D3_PULL_DIS_SHIFT)        /* 0x00000008 */
#define PMU2_IOC_GPIO0D_PDIS_GPIO0D4_PULL_DIS_SHIFT        (4U)
#define PMU2_IOC_GPIO0D_PDIS_GPIO0D4_PULL_DIS_MASK         (0x1U << PMU2_IOC_GPIO0D_PDIS_GPIO0D4_PULL_DIS_SHIFT)        /* 0x00000010 */
#define PMU2_IOC_GPIO0D_PDIS_GPIO0D5_PULL_DIS_SHIFT        (5U)
#define PMU2_IOC_GPIO0D_PDIS_GPIO0D5_PULL_DIS_MASK         (0x1U << PMU2_IOC_GPIO0D_PDIS_GPIO0D5_PULL_DIS_SHIFT)        /* 0x00000020 */
#define PMU2_IOC_GPIO0D_PDIS_GPIO0D6_PULL_DIS_SHIFT        (6U)
#define PMU2_IOC_GPIO0D_PDIS_GPIO0D6_PULL_DIS_MASK         (0x1U << PMU2_IOC_GPIO0D_PDIS_GPIO0D6_PULL_DIS_SHIFT)        /* 0x00000040 */
/****************************************BUS_IOC*****************************************/
/* GPIO0B_IOMUX_SEL_H */
#define BUS_IOC_GPIO0B_IOMUX_SEL_H_OFFSET                  (0xCU)
#define BUS_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B5_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B5_SEL_MASK        (0xFU << BUS_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B5_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B6_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B6_SEL_MASK        (0xFU << BUS_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B6_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B7_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B7_SEL_MASK        (0xFU << BUS_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B7_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO0C_IOMUX_SEL_L */
#define BUS_IOC_GPIO0C_IOMUX_SEL_L_OFFSET                  (0x10U)
#define BUS_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C0_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C0_SEL_MASK        (0xFU << BUS_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C0_SEL_SHIFT)       /* 0x0000000F */
/* GPIO0C_IOMUX_SEL_H */
#define BUS_IOC_GPIO0C_IOMUX_SEL_H_OFFSET                  (0x14U)
#define BUS_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C4_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C4_SEL_MASK        (0xFU << BUS_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C4_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C5_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C5_SEL_MASK        (0xFU << BUS_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C5_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C6_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C6_SEL_MASK        (0xFU << BUS_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C6_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C7_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C7_SEL_MASK        (0xFU << BUS_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C7_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO0D_IOMUX_SEL_L */
#define BUS_IOC_GPIO0D_IOMUX_SEL_L_OFFSET                  (0x18U)
#define BUS_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D0_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D0_SEL_MASK        (0xFU << BUS_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D0_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D1_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D1_SEL_MASK        (0xFU << BUS_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D1_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D2_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D2_SEL_MASK        (0xFU << BUS_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D2_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D3_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D3_SEL_MASK        (0xFU << BUS_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D3_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO0D_IOMUX_SEL_H */
#define BUS_IOC_GPIO0D_IOMUX_SEL_H_OFFSET                  (0x1CU)
#define BUS_IOC_GPIO0D_IOMUX_SEL_H_GPIO0D4_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO0D_IOMUX_SEL_H_GPIO0D4_SEL_MASK        (0xFU << BUS_IOC_GPIO0D_IOMUX_SEL_H_GPIO0D4_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO0D_IOMUX_SEL_H_GPIO0D5_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO0D_IOMUX_SEL_H_GPIO0D5_SEL_MASK        (0xFU << BUS_IOC_GPIO0D_IOMUX_SEL_H_GPIO0D5_SEL_SHIFT)       /* 0x000000F0 */
/* GPIO1A_IOMUX_SEL_L */
#define BUS_IOC_GPIO1A_IOMUX_SEL_L_OFFSET                  (0x20U)
#define BUS_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A0_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A0_SEL_MASK        (0xFU << BUS_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A0_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A1_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A1_SEL_MASK        (0xFU << BUS_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A1_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A2_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A2_SEL_MASK        (0xFU << BUS_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A2_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A3_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A3_SEL_MASK        (0xFU << BUS_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A3_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO1A_IOMUX_SEL_H */
#define BUS_IOC_GPIO1A_IOMUX_SEL_H_OFFSET                  (0x24U)
#define BUS_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A4_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A4_SEL_MASK        (0xFU << BUS_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A4_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A5_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A5_SEL_MASK        (0xFU << BUS_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A5_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A6_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A6_SEL_MASK        (0xFU << BUS_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A6_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A7_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A7_SEL_MASK        (0xFU << BUS_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A7_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO1B_IOMUX_SEL_L */
#define BUS_IOC_GPIO1B_IOMUX_SEL_L_OFFSET                  (0x28U)
#define BUS_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B0_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B0_SEL_MASK        (0xFU << BUS_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B0_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B1_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B1_SEL_MASK        (0xFU << BUS_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B1_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B2_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B2_SEL_MASK        (0xFU << BUS_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B2_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B3_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B3_SEL_MASK        (0xFU << BUS_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B3_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO1B_IOMUX_SEL_H */
#define BUS_IOC_GPIO1B_IOMUX_SEL_H_OFFSET                  (0x2CU)
#define BUS_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B4_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B4_SEL_MASK        (0xFU << BUS_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B4_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B5_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B5_SEL_MASK        (0xFU << BUS_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B5_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B6_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B6_SEL_MASK        (0xFU << BUS_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B6_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B7_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B7_SEL_MASK        (0xFU << BUS_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B7_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO1C_IOMUX_SEL_L */
#define BUS_IOC_GPIO1C_IOMUX_SEL_L_OFFSET                  (0x30U)
#define BUS_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C0_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C0_SEL_MASK        (0xFU << BUS_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C0_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C1_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C1_SEL_MASK        (0xFU << BUS_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C1_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C2_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C2_SEL_MASK        (0xFU << BUS_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C2_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C3_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C3_SEL_MASK        (0xFU << BUS_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C3_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO1C_IOMUX_SEL_H */
#define BUS_IOC_GPIO1C_IOMUX_SEL_H_OFFSET                  (0x34U)
#define BUS_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C4_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C4_SEL_MASK        (0xFU << BUS_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C4_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C5_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C5_SEL_MASK        (0xFU << BUS_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C5_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C6_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C6_SEL_MASK        (0xFU << BUS_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C6_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C7_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C7_SEL_MASK        (0xFU << BUS_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C7_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO1D_IOMUX_SEL_L */
#define BUS_IOC_GPIO1D_IOMUX_SEL_L_OFFSET                  (0x38U)
#define BUS_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D0_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D0_SEL_MASK        (0xFU << BUS_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D0_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D1_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D1_SEL_MASK        (0xFU << BUS_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D1_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D2_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D2_SEL_MASK        (0xFU << BUS_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D2_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D3_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D3_SEL_MASK        (0xFU << BUS_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D3_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO1D_IOMUX_SEL_H */
#define BUS_IOC_GPIO1D_IOMUX_SEL_H_OFFSET                  (0x3CU)
#define BUS_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D4_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D4_SEL_MASK        (0xFU << BUS_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D4_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D5_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D5_SEL_MASK        (0xFU << BUS_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D5_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D6_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D6_SEL_MASK        (0xFU << BUS_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D6_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D7_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D7_SEL_MASK        (0xFU << BUS_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D7_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO2A_IOMUX_SEL_L */
#define BUS_IOC_GPIO2A_IOMUX_SEL_L_OFFSET                  (0x40U)
#define BUS_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A0_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A0_SEL_MASK        (0xFU << BUS_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A0_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A1_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A1_SEL_MASK        (0xFU << BUS_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A1_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A2_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A2_SEL_MASK        (0xFU << BUS_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A2_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A3_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A3_SEL_MASK        (0xFU << BUS_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A3_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO2A_IOMUX_SEL_H */
#define BUS_IOC_GPIO2A_IOMUX_SEL_H_OFFSET                  (0x44U)
#define BUS_IOC_GPIO2A_IOMUX_SEL_H_GPIO2A6_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO2A_IOMUX_SEL_H_GPIO2A6_SEL_MASK        (0xFU << BUS_IOC_GPIO2A_IOMUX_SEL_H_GPIO2A6_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO2A_IOMUX_SEL_H_GPIO2A7_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO2A_IOMUX_SEL_H_GPIO2A7_SEL_MASK        (0xFU << BUS_IOC_GPIO2A_IOMUX_SEL_H_GPIO2A7_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO2B_IOMUX_SEL_L */
#define BUS_IOC_GPIO2B_IOMUX_SEL_L_OFFSET                  (0x48U)
#define BUS_IOC_GPIO2B_IOMUX_SEL_L_GPIO2B0_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO2B_IOMUX_SEL_L_GPIO2B0_SEL_MASK        (0xFU << BUS_IOC_GPIO2B_IOMUX_SEL_L_GPIO2B0_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO2B_IOMUX_SEL_L_GPIO2B1_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO2B_IOMUX_SEL_L_GPIO2B1_SEL_MASK        (0xFU << BUS_IOC_GPIO2B_IOMUX_SEL_L_GPIO2B1_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO2B_IOMUX_SEL_L_GPIO2B2_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO2B_IOMUX_SEL_L_GPIO2B2_SEL_MASK        (0xFU << BUS_IOC_GPIO2B_IOMUX_SEL_L_GPIO2B2_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO2B_IOMUX_SEL_L_GPIO2B3_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO2B_IOMUX_SEL_L_GPIO2B3_SEL_MASK        (0xFU << BUS_IOC_GPIO2B_IOMUX_SEL_L_GPIO2B3_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO2B_IOMUX_SEL_H */
#define BUS_IOC_GPIO2B_IOMUX_SEL_H_OFFSET                  (0x4CU)
#define BUS_IOC_GPIO2B_IOMUX_SEL_H_GPIO2B4_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO2B_IOMUX_SEL_H_GPIO2B4_SEL_MASK        (0xFU << BUS_IOC_GPIO2B_IOMUX_SEL_H_GPIO2B4_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO2B_IOMUX_SEL_H_GPIO2B5_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO2B_IOMUX_SEL_H_GPIO2B5_SEL_MASK        (0xFU << BUS_IOC_GPIO2B_IOMUX_SEL_H_GPIO2B5_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO2B_IOMUX_SEL_H_GPIO2B6_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO2B_IOMUX_SEL_H_GPIO2B6_SEL_MASK        (0xFU << BUS_IOC_GPIO2B_IOMUX_SEL_H_GPIO2B6_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO2B_IOMUX_SEL_H_GPIO2B7_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO2B_IOMUX_SEL_H_GPIO2B7_SEL_MASK        (0xFU << BUS_IOC_GPIO2B_IOMUX_SEL_H_GPIO2B7_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO2C_IOMUX_SEL_L */
#define BUS_IOC_GPIO2C_IOMUX_SEL_L_OFFSET                  (0x50U)
#define BUS_IOC_GPIO2C_IOMUX_SEL_L_GPIO2C0_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO2C_IOMUX_SEL_L_GPIO2C0_SEL_MASK        (0xFU << BUS_IOC_GPIO2C_IOMUX_SEL_L_GPIO2C0_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO2C_IOMUX_SEL_L_GPIO2C1_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO2C_IOMUX_SEL_L_GPIO2C1_SEL_MASK        (0xFU << BUS_IOC_GPIO2C_IOMUX_SEL_L_GPIO2C1_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO2C_IOMUX_SEL_L_GPIO2C2_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO2C_IOMUX_SEL_L_GPIO2C2_SEL_MASK        (0xFU << BUS_IOC_GPIO2C_IOMUX_SEL_L_GPIO2C2_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO2C_IOMUX_SEL_L_GPIO2C3_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO2C_IOMUX_SEL_L_GPIO2C3_SEL_MASK        (0xFU << BUS_IOC_GPIO2C_IOMUX_SEL_L_GPIO2C3_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO2C_IOMUX_SEL_H */
#define BUS_IOC_GPIO2C_IOMUX_SEL_H_OFFSET                  (0x54U)
#define BUS_IOC_GPIO2C_IOMUX_SEL_H_GPIO2C4_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO2C_IOMUX_SEL_H_GPIO2C4_SEL_MASK        (0xFU << BUS_IOC_GPIO2C_IOMUX_SEL_H_GPIO2C4_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO2C_IOMUX_SEL_H_GPIO2C5_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO2C_IOMUX_SEL_H_GPIO2C5_SEL_MASK        (0xFU << BUS_IOC_GPIO2C_IOMUX_SEL_H_GPIO2C5_SEL_SHIFT)       /* 0x000000F0 */
/* GPIO2D_IOMUX_SEL_L */
#define BUS_IOC_GPIO2D_IOMUX_SEL_L_OFFSET                  (0x58U)
#define BUS_IOC_GPIO2D_IOMUX_SEL_L_GPIO2D0_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO2D_IOMUX_SEL_L_GPIO2D0_SEL_MASK        (0xFU << BUS_IOC_GPIO2D_IOMUX_SEL_L_GPIO2D0_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO2D_IOMUX_SEL_L_GPIO2D1_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO2D_IOMUX_SEL_L_GPIO2D1_SEL_MASK        (0xFU << BUS_IOC_GPIO2D_IOMUX_SEL_L_GPIO2D1_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO2D_IOMUX_SEL_L_GPIO2D2_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO2D_IOMUX_SEL_L_GPIO2D2_SEL_MASK        (0xFU << BUS_IOC_GPIO2D_IOMUX_SEL_L_GPIO2D2_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO2D_IOMUX_SEL_L_GPIO2D3_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO2D_IOMUX_SEL_L_GPIO2D3_SEL_MASK        (0xFU << BUS_IOC_GPIO2D_IOMUX_SEL_L_GPIO2D3_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO2D_IOMUX_SEL_H */
#define BUS_IOC_GPIO2D_IOMUX_SEL_H_OFFSET                  (0x5CU)
#define BUS_IOC_GPIO2D_IOMUX_SEL_H_GPIO2D4_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO2D_IOMUX_SEL_H_GPIO2D4_SEL_MASK        (0xFU << BUS_IOC_GPIO2D_IOMUX_SEL_H_GPIO2D4_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO2D_IOMUX_SEL_H_GPIO2D5_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO2D_IOMUX_SEL_H_GPIO2D5_SEL_MASK        (0xFU << BUS_IOC_GPIO2D_IOMUX_SEL_H_GPIO2D5_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO2D_IOMUX_SEL_H_GPIO2D6_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO2D_IOMUX_SEL_H_GPIO2D6_SEL_MASK        (0xFU << BUS_IOC_GPIO2D_IOMUX_SEL_H_GPIO2D6_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO2D_IOMUX_SEL_H_GPIO2D7_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO2D_IOMUX_SEL_H_GPIO2D7_SEL_MASK        (0xFU << BUS_IOC_GPIO2D_IOMUX_SEL_H_GPIO2D7_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO3A_IOMUX_SEL_L */
#define BUS_IOC_GPIO3A_IOMUX_SEL_L_OFFSET                  (0x60U)
#define BUS_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A0_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A0_SEL_MASK        (0xFU << BUS_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A0_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A1_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A1_SEL_MASK        (0xFU << BUS_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A1_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A2_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A2_SEL_MASK        (0xFU << BUS_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A2_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A3_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A3_SEL_MASK        (0xFU << BUS_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A3_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO3A_IOMUX_SEL_H */
#define BUS_IOC_GPIO3A_IOMUX_SEL_H_OFFSET                  (0x64U)
#define BUS_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A4_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A4_SEL_MASK        (0xFU << BUS_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A4_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A5_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A5_SEL_MASK        (0xFU << BUS_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A5_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A6_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A6_SEL_MASK        (0xFU << BUS_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A6_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A7_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A7_SEL_MASK        (0xFU << BUS_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A7_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO3B_IOMUX_SEL_L */
#define BUS_IOC_GPIO3B_IOMUX_SEL_L_OFFSET                  (0x68U)
#define BUS_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B0_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B0_SEL_MASK        (0xFU << BUS_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B0_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B1_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B1_SEL_MASK        (0xFU << BUS_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B1_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B2_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B2_SEL_MASK        (0xFU << BUS_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B2_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B3_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B3_SEL_MASK        (0xFU << BUS_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B3_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO3B_IOMUX_SEL_H */
#define BUS_IOC_GPIO3B_IOMUX_SEL_H_OFFSET                  (0x6CU)
#define BUS_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B4_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B4_SEL_MASK        (0xFU << BUS_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B4_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B5_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B5_SEL_MASK        (0xFU << BUS_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B5_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B6_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B6_SEL_MASK        (0xFU << BUS_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B6_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B7_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B7_SEL_MASK        (0xFU << BUS_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B7_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO3C_IOMUX_SEL_L */
#define BUS_IOC_GPIO3C_IOMUX_SEL_L_OFFSET                  (0x70U)
#define BUS_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C0_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C0_SEL_MASK        (0xFU << BUS_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C0_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C1_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C1_SEL_MASK        (0xFU << BUS_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C1_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C2_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C2_SEL_MASK        (0xFU << BUS_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C2_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C3_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C3_SEL_MASK        (0xFU << BUS_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C3_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO3C_IOMUX_SEL_H */
#define BUS_IOC_GPIO3C_IOMUX_SEL_H_OFFSET                  (0x74U)
#define BUS_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C4_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C4_SEL_MASK        (0xFU << BUS_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C4_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C5_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C5_SEL_MASK        (0xFU << BUS_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C5_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C6_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C6_SEL_MASK        (0xFU << BUS_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C6_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C7_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C7_SEL_MASK        (0xFU << BUS_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C7_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO3D_IOMUX_SEL_L */
#define BUS_IOC_GPIO3D_IOMUX_SEL_L_OFFSET                  (0x78U)
#define BUS_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D0_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D0_SEL_MASK        (0xFU << BUS_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D0_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D1_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D1_SEL_MASK        (0xFU << BUS_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D1_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D2_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D2_SEL_MASK        (0xFU << BUS_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D2_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D3_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D3_SEL_MASK        (0xFU << BUS_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D3_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO3D_IOMUX_SEL_H */
#define BUS_IOC_GPIO3D_IOMUX_SEL_H_OFFSET                  (0x7CU)
#define BUS_IOC_GPIO3D_IOMUX_SEL_H_GPIO3D4_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO3D_IOMUX_SEL_H_GPIO3D4_SEL_MASK        (0xFU << BUS_IOC_GPIO3D_IOMUX_SEL_H_GPIO3D4_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO3D_IOMUX_SEL_H_GPIO3D5_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO3D_IOMUX_SEL_H_GPIO3D5_SEL_MASK        (0xFU << BUS_IOC_GPIO3D_IOMUX_SEL_H_GPIO3D5_SEL_SHIFT)       /* 0x000000F0 */
/* GPIO4A_IOMUX_SEL_L */
#define BUS_IOC_GPIO4A_IOMUX_SEL_L_OFFSET                  (0x80U)
#define BUS_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A0_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A0_SEL_MASK        (0xFU << BUS_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A0_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A1_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A1_SEL_MASK        (0xFU << BUS_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A1_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A2_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A2_SEL_MASK        (0xFU << BUS_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A2_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A3_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A3_SEL_MASK        (0xFU << BUS_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A3_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO4A_IOMUX_SEL_H */
#define BUS_IOC_GPIO4A_IOMUX_SEL_H_OFFSET                  (0x84U)
#define BUS_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A4_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A4_SEL_MASK        (0xFU << BUS_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A4_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A5_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A5_SEL_MASK        (0xFU << BUS_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A5_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A6_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A6_SEL_MASK        (0xFU << BUS_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A6_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A7_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A7_SEL_MASK        (0xFU << BUS_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A7_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO4B_IOMUX_SEL_L */
#define BUS_IOC_GPIO4B_IOMUX_SEL_L_OFFSET                  (0x88U)
#define BUS_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B0_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B0_SEL_MASK        (0xFU << BUS_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B0_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B1_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B1_SEL_MASK        (0xFU << BUS_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B1_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B2_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B2_SEL_MASK        (0xFU << BUS_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B2_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B3_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B3_SEL_MASK        (0xFU << BUS_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B3_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO4B_IOMUX_SEL_H */
#define BUS_IOC_GPIO4B_IOMUX_SEL_H_OFFSET                  (0x8CU)
#define BUS_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B4_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B4_SEL_MASK        (0xFU << BUS_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B4_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B5_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B5_SEL_MASK        (0xFU << BUS_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B5_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B6_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B6_SEL_MASK        (0xFU << BUS_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B6_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B7_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B7_SEL_MASK        (0xFU << BUS_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B7_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO4C_IOMUX_SEL_L */
#define BUS_IOC_GPIO4C_IOMUX_SEL_L_OFFSET                  (0x90U)
#define BUS_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C0_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C0_SEL_MASK        (0xFU << BUS_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C0_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C1_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C1_SEL_MASK        (0xFU << BUS_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C1_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C2_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C2_SEL_MASK        (0xFU << BUS_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C2_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C3_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C3_SEL_MASK        (0xFU << BUS_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C3_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO4C_IOMUX_SEL_H */
#define BUS_IOC_GPIO4C_IOMUX_SEL_H_OFFSET                  (0x94U)
#define BUS_IOC_GPIO4C_IOMUX_SEL_H_GPIO4C4_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO4C_IOMUX_SEL_H_GPIO4C4_SEL_MASK        (0xFU << BUS_IOC_GPIO4C_IOMUX_SEL_H_GPIO4C4_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO4C_IOMUX_SEL_H_GPIO4C5_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO4C_IOMUX_SEL_H_GPIO4C5_SEL_MASK        (0xFU << BUS_IOC_GPIO4C_IOMUX_SEL_H_GPIO4C5_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO4C_IOMUX_SEL_H_GPIO4C6_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO4C_IOMUX_SEL_H_GPIO4C6_SEL_MASK        (0xFU << BUS_IOC_GPIO4C_IOMUX_SEL_H_GPIO4C6_SEL_SHIFT)       /* 0x00000F00 */
/* GPIO4D_IOMUX_SEL_L */
#define BUS_IOC_GPIO4D_IOMUX_SEL_L_OFFSET                  (0x98U)
#define BUS_IOC_GPIO4D_IOMUX_SEL_L_GPIO4D0_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO4D_IOMUX_SEL_L_GPIO4D0_SEL_MASK        (0xFU << BUS_IOC_GPIO4D_IOMUX_SEL_L_GPIO4D0_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO4D_IOMUX_SEL_L_GPIO4D1_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO4D_IOMUX_SEL_L_GPIO4D1_SEL_MASK        (0xFU << BUS_IOC_GPIO4D_IOMUX_SEL_L_GPIO4D1_SEL_SHIFT)       /* 0x000000F0 */
#define BUS_IOC_GPIO4D_IOMUX_SEL_L_GPIO4D2_SEL_SHIFT       (8U)
#define BUS_IOC_GPIO4D_IOMUX_SEL_L_GPIO4D2_SEL_MASK        (0xFU << BUS_IOC_GPIO4D_IOMUX_SEL_L_GPIO4D2_SEL_SHIFT)       /* 0x00000F00 */
#define BUS_IOC_GPIO4D_IOMUX_SEL_L_GPIO4D3_SEL_SHIFT       (12U)
#define BUS_IOC_GPIO4D_IOMUX_SEL_L_GPIO4D3_SEL_MASK        (0xFU << BUS_IOC_GPIO4D_IOMUX_SEL_L_GPIO4D3_SEL_SHIFT)       /* 0x0000F000 */
/* GPIO4D_IOMUX_SEL_H */
#define BUS_IOC_GPIO4D_IOMUX_SEL_H_OFFSET                  (0x9CU)
#define BUS_IOC_GPIO4D_IOMUX_SEL_H_GPIO4D4_SEL_SHIFT       (0U)
#define BUS_IOC_GPIO4D_IOMUX_SEL_H_GPIO4D4_SEL_MASK        (0xFU << BUS_IOC_GPIO4D_IOMUX_SEL_H_GPIO4D4_SEL_SHIFT)       /* 0x0000000F */
#define BUS_IOC_GPIO4D_IOMUX_SEL_H_GPIO4D5_SEL_SHIFT       (4U)
#define BUS_IOC_GPIO4D_IOMUX_SEL_H_GPIO4D5_SEL_MASK        (0xFU << BUS_IOC_GPIO4D_IOMUX_SEL_H_GPIO4D5_SEL_SHIFT)       /* 0x000000F0 */
/******************************************UART******************************************/
/* RBR */
#define UART_RBR_OFFSET                                    (0x0U)
#define UART_RBR                                           (0x0U)
#define UART_RBR_DATA_INPUT_SHIFT                          (0U)
#define UART_RBR_DATA_INPUT_MASK                           (0xFFU << UART_RBR_DATA_INPUT_SHIFT)                         /* 0x000000FF */
/* DLL */
#define UART_DLL_OFFSET                                    (0x0U)
#define UART_DLL_BAUD_RATE_DIVISOR_L_SHIFT                 (0U)
#define UART_DLL_BAUD_RATE_DIVISOR_L_MASK                  (0xFFU << UART_DLL_BAUD_RATE_DIVISOR_L_SHIFT)                /* 0x000000FF */
/* THR */
#define UART_THR_OFFSET                                    (0x0U)
#define UART_THR_DATA_OUTPUT_SHIFT                         (0U)
#define UART_THR_DATA_OUTPUT_MASK                          (0xFFU << UART_THR_DATA_OUTPUT_SHIFT)                        /* 0x000000FF */
/* DLH */
#define UART_DLH_OFFSET                                    (0x4U)
#define UART_DLH_BAUD_RATE_DIVISOR_H_SHIFT                 (0U)
#define UART_DLH_BAUD_RATE_DIVISOR_H_MASK                  (0xFFU << UART_DLH_BAUD_RATE_DIVISOR_H_SHIFT)                /* 0x000000FF */
/* IER */
#define UART_IER_OFFSET                                    (0x4U)
#define UART_IER_RECEIVE_DATA_AVAILABLE_INT_EN_SHIFT       (0U)
#define UART_IER_RECEIVE_DATA_AVAILABLE_INT_EN_MASK        (0x1U << UART_IER_RECEIVE_DATA_AVAILABLE_INT_EN_SHIFT)       /* 0x00000001 */
#define UART_IER_TRANS_HOLD_EMPTY_INT_EN_SHIFT             (1U)
#define UART_IER_TRANS_HOLD_EMPTY_INT_EN_MASK              (0x1U << UART_IER_TRANS_HOLD_EMPTY_INT_EN_SHIFT)             /* 0x00000002 */
#define UART_IER_RECEIVE_LINE_STATUS_INT_EN_SHIFT          (2U)
#define UART_IER_RECEIVE_LINE_STATUS_INT_EN_MASK           (0x1U << UART_IER_RECEIVE_LINE_STATUS_INT_EN_SHIFT)          /* 0x00000004 */
#define UART_IER_MODEM_STATUS_INT_EN_SHIFT                 (3U)
#define UART_IER_MODEM_STATUS_INT_EN_MASK                  (0x1U << UART_IER_MODEM_STATUS_INT_EN_SHIFT)                 /* 0x00000008 */
#define UART_IER_PROG_THRE_INT_EN_SHIFT                    (7U)
#define UART_IER_PROG_THRE_INT_EN_MASK                     (0x1U << UART_IER_PROG_THRE_INT_EN_SHIFT)                    /* 0x00000080 */
/* FCR */
#define UART_FCR_OFFSET                                    (0x8U)
#define UART_FCR_FIFO_EN_SHIFT                             (0U)
#define UART_FCR_FIFO_EN_MASK                              (0x1U << UART_FCR_FIFO_EN_SHIFT)                             /* 0x00000001 */
#define UART_FCR_RCVR_FIFO_RESET_SHIFT                     (1U)
#define UART_FCR_RCVR_FIFO_RESET_MASK                      (0x1U << UART_FCR_RCVR_FIFO_RESET_SHIFT)                     /* 0x00000002 */
#define UART_FCR_XMIT_FIFO_RESET_SHIFT                     (2U)
#define UART_FCR_XMIT_FIFO_RESET_MASK                      (0x1U << UART_FCR_XMIT_FIFO_RESET_SHIFT)                     /* 0x00000004 */
#define UART_FCR_DMA_MODE_SHIFT                            (3U)
#define UART_FCR_DMA_MODE_MASK                             (0x1U << UART_FCR_DMA_MODE_SHIFT)                            /* 0x00000008 */
#define UART_FCR_TX_EMPTY_TRIGGER_SHIFT                    (4U)
#define UART_FCR_TX_EMPTY_TRIGGER_MASK                     (0x3U << UART_FCR_TX_EMPTY_TRIGGER_SHIFT)                    /* 0x00000030 */
#define UART_FCR_RCVR_TRIGGER_SHIFT                        (6U)
#define UART_FCR_RCVR_TRIGGER_MASK                         (0x3U << UART_FCR_RCVR_TRIGGER_SHIFT)                        /* 0x000000C0 */
/* IIR */
#define UART_IIR_OFFSET                                    (0x8U)
#define UART_IIR                                           (0x1U)
#define UART_IIR_INT_ID_SHIFT                              (0U)
#define UART_IIR_INT_ID_MASK                               (0xFU << UART_IIR_INT_ID_SHIFT)                              /* 0x0000000F */
#define UART_IIR_FIFOS_EN_SHIFT                            (6U)
#define UART_IIR_FIFOS_EN_MASK                             (0x3U << UART_IIR_FIFOS_EN_SHIFT)                            /* 0x000000C0 */
/* LCR */
#define UART_LCR_OFFSET                                    (0xCU)
#define UART_LCR_DATA_LENGTH_SEL_SHIFT                     (0U)
#define UART_LCR_DATA_LENGTH_SEL_MASK                      (0x3U << UART_LCR_DATA_LENGTH_SEL_SHIFT)                     /* 0x00000003 */
#define UART_LCR_STOP_BITS_NUM_SHIFT                       (2U)
#define UART_LCR_STOP_BITS_NUM_MASK                        (0x1U << UART_LCR_STOP_BITS_NUM_SHIFT)                       /* 0x00000004 */
#define UART_LCR_PARITY_EN_SHIFT                           (3U)
#define UART_LCR_PARITY_EN_MASK                            (0x1U << UART_LCR_PARITY_EN_SHIFT)                           /* 0x00000008 */
#define UART_LCR_EVEN_PARITY_SEL_SHIFT                     (4U)
#define UART_LCR_EVEN_PARITY_SEL_MASK                      (0x1U << UART_LCR_EVEN_PARITY_SEL_SHIFT)                     /* 0x00000010 */
#define UART_LCR_STICK_PARITY_SHIFT                        (5U)
#define UART_LCR_STICK_PARITY_MASK                         (0x1U << UART_LCR_STICK_PARITY_SHIFT)                        /* 0x00000020 */
#define UART_LCR_BREAK_CTRL_SHIFT                          (6U)
#define UART_LCR_BREAK_CTRL_MASK                           (0x1U << UART_LCR_BREAK_CTRL_SHIFT)                          /* 0x00000040 */
#define UART_LCR_DIV_LAT_ACCESS_SHIFT                      (7U)
#define UART_LCR_DIV_LAT_ACCESS_MASK                       (0x1U << UART_LCR_DIV_LAT_ACCESS_SHIFT)                      /* 0x00000080 */
/* MCR */
#define UART_MCR_OFFSET                                    (0x10U)
#define UART_MCR_DATA_TERMINAL_READY_SHIFT                 (0U)
#define UART_MCR_DATA_TERMINAL_READY_MASK                  (0x1U << UART_MCR_DATA_TERMINAL_READY_SHIFT)                 /* 0x00000001 */
#define UART_MCR_REQ_TO_SEND_SHIFT                         (1U)
#define UART_MCR_REQ_TO_SEND_MASK                          (0x1U << UART_MCR_REQ_TO_SEND_SHIFT)                         /* 0x00000002 */
#define UART_MCR_OUT1_SHIFT                                (2U)
#define UART_MCR_OUT1_MASK                                 (0x1U << UART_MCR_OUT1_SHIFT)                                /* 0x00000004 */
#define UART_MCR_OUT2_SHIFT                                (3U)
#define UART_MCR_OUT2_MASK                                 (0x1U << UART_MCR_OUT2_SHIFT)                                /* 0x00000008 */
#define UART_MCR_LOOPBACK_SHIFT                            (4U)
#define UART_MCR_LOOPBACK_MASK                             (0x1U << UART_MCR_LOOPBACK_SHIFT)                            /* 0x00000010 */
#define UART_MCR_AUTO_FLOW_CTRL_EN_SHIFT                   (5U)
#define UART_MCR_AUTO_FLOW_CTRL_EN_MASK                    (0x1U << UART_MCR_AUTO_FLOW_CTRL_EN_SHIFT)                   /* 0x00000020 */
#define UART_MCR_SIR_MODE_EN_SHIFT                         (6U)
#define UART_MCR_SIR_MODE_EN_MASK                          (0x1U << UART_MCR_SIR_MODE_EN_SHIFT)                         /* 0x00000040 */
/* LSR */
#define UART_LSR_OFFSET                                    (0x14U)
#define UART_LSR                                           (0x60U)
#define UART_LSR_DATA_READY_SHIFT                          (0U)
#define UART_LSR_DATA_READY_MASK                           (0x1U << UART_LSR_DATA_READY_SHIFT)                          /* 0x00000001 */
#define UART_LSR_OVERRUN_ERROR_SHIFT                       (1U)
#define UART_LSR_OVERRUN_ERROR_MASK                        (0x1U << UART_LSR_OVERRUN_ERROR_SHIFT)                       /* 0x00000002 */
#define UART_LSR_PARITY_ERROR_SHIFT                        (2U)
#define UART_LSR_PARITY_ERROR_MASK                         (0x1U << UART_LSR_PARITY_ERROR_SHIFT)                        /* 0x00000004 */
#define UART_LSR_FRAMING_ERROR_SHIFT                       (3U)
#define UART_LSR_FRAMING_ERROR_MASK                        (0x1U << UART_LSR_FRAMING_ERROR_SHIFT)                       /* 0x00000008 */
#define UART_LSR_BREAK_INT_SHIFT                           (4U)
#define UART_LSR_BREAK_INT_MASK                            (0x1U << UART_LSR_BREAK_INT_SHIFT)                           /* 0x00000010 */
#define UART_LSR_TRANS_HOLD_REG_EMPTY_SHIFT                (5U)
#define UART_LSR_TRANS_HOLD_REG_EMPTY_MASK                 (0x1U << UART_LSR_TRANS_HOLD_REG_EMPTY_SHIFT)                /* 0x00000020 */
#define UART_LSR_TRANS_EMPTY_SHIFT                         (6U)
#define UART_LSR_TRANS_EMPTY_MASK                          (0x1U << UART_LSR_TRANS_EMPTY_SHIFT)                         /* 0x00000040 */
#define UART_LSR_RECEIVER_FIFO_ERROR_SHIFT                 (7U)
#define UART_LSR_RECEIVER_FIFO_ERROR_MASK                  (0x1U << UART_LSR_RECEIVER_FIFO_ERROR_SHIFT)                 /* 0x00000080 */
/* MSR */
#define UART_MSR_OFFSET                                    (0x18U)
#define UART_MSR                                           (0x0U)
#define UART_MSR_DELTA_CLEAR_TO_SEND_SHIFT                 (0U)
#define UART_MSR_DELTA_CLEAR_TO_SEND_MASK                  (0x1U << UART_MSR_DELTA_CLEAR_TO_SEND_SHIFT)                 /* 0x00000001 */
#define UART_MSR_DELTA_DATA_SET_READY_SHIFT                (1U)
#define UART_MSR_DELTA_DATA_SET_READY_MASK                 (0x1U << UART_MSR_DELTA_DATA_SET_READY_SHIFT)                /* 0x00000002 */
#define UART_MSR_TRAILING_EDGE_RING_INDICATOR_SHIFT        (2U)
#define UART_MSR_TRAILING_EDGE_RING_INDICATOR_MASK         (0x1U << UART_MSR_TRAILING_EDGE_RING_INDICATOR_SHIFT)        /* 0x00000004 */
#define UART_MSR_DELTA_DATA_CARRIER_DETECT_SHIFT           (3U)
#define UART_MSR_DELTA_DATA_CARRIER_DETECT_MASK            (0x1U << UART_MSR_DELTA_DATA_CARRIER_DETECT_SHIFT)           /* 0x00000008 */
#define UART_MSR_CLEAR_TO_SEND_SHIFT                       (4U)
#define UART_MSR_CLEAR_TO_SEND_MASK                        (0x1U << UART_MSR_CLEAR_TO_SEND_SHIFT)                       /* 0x00000010 */
#define UART_MSR_DATA_SET_READY_SHIFT                      (5U)
#define UART_MSR_DATA_SET_READY_MASK                       (0x1U << UART_MSR_DATA_SET_READY_SHIFT)                      /* 0x00000020 */
#define UART_MSR_RING_INDICATOR_SHIFT                      (6U)
#define UART_MSR_RING_INDICATOR_MASK                       (0x1U << UART_MSR_RING_INDICATOR_SHIFT)                      /* 0x00000040 */
#define UART_MSR_DATA_CARRIOR_DETECT_SHIFT                 (7U)
#define UART_MSR_DATA_CARRIOR_DETECT_MASK                  (0x1U << UART_MSR_DATA_CARRIOR_DETECT_SHIFT)                 /* 0x00000080 */
/* SCR */
#define UART_SCR_OFFSET                                    (0x1CU)
#define UART_SCR_TEMP_STORE_SPACE_SHIFT                    (0U)
#define UART_SCR_TEMP_STORE_SPACE_MASK                     (0xFFU << UART_SCR_TEMP_STORE_SPACE_SHIFT)                   /* 0x000000FF */
/* SRBR */
#define UART_SRBR_OFFSET                                   (0x30U)
#define UART_SRBR                                          (0x0U)
#define UART_SRBR_SHADOW_RBR_SHIFT                         (0U)
#define UART_SRBR_SHADOW_RBR_MASK                          (0xFFU << UART_SRBR_SHADOW_RBR_SHIFT)                        /* 0x000000FF */
/* STHR */
#define UART_STHR_OFFSET                                   (0x30U)
#define UART_STHR_SHADOW_THR_SHIFT                         (0U)
#define UART_STHR_SHADOW_THR_MASK                          (0xFFU << UART_STHR_SHADOW_THR_SHIFT)                        /* 0x000000FF */
/* FAR */
#define UART_FAR_OFFSET                                    (0x70U)
#define UART_FAR_FIFO_ACCESS_TEST_EN_SHIFT                 (0U)
#define UART_FAR_FIFO_ACCESS_TEST_EN_MASK                  (0x1U << UART_FAR_FIFO_ACCESS_TEST_EN_SHIFT)                 /* 0x00000001 */
/* TFR */
#define UART_TFR_OFFSET                                    (0x74U)
#define UART_TFR                                           (0x0U)
#define UART_TFR_TRANS_FIFO_READ_SHIFT                     (0U)
#define UART_TFR_TRANS_FIFO_READ_MASK                      (0xFFU << UART_TFR_TRANS_FIFO_READ_SHIFT)                    /* 0x000000FF */
/* RFW */
#define UART_RFW_OFFSET                                    (0x78U)
#define UART_RFW_RECEIVE_FIFO_WRITE_SHIFT                  (0U)
#define UART_RFW_RECEIVE_FIFO_WRITE_MASK                   (0xFFU << UART_RFW_RECEIVE_FIFO_WRITE_SHIFT)                 /* 0x000000FF */
#define UART_RFW_RECEIVE_FIFO_PARITY_ERROR_SHIFT           (8U)
#define UART_RFW_RECEIVE_FIFO_PARITY_ERROR_MASK            (0x1U << UART_RFW_RECEIVE_FIFO_PARITY_ERROR_SHIFT)           /* 0x00000100 */
#define UART_RFW_RECEIVE_FIFO_FRAMING_ERROR_SHIFT          (9U)
#define UART_RFW_RECEIVE_FIFO_FRAMING_ERROR_MASK           (0x1U << UART_RFW_RECEIVE_FIFO_FRAMING_ERROR_SHIFT)          /* 0x00000200 */
/* USR */
#define UART_USR_OFFSET                                    (0x7CU)
#define UART_USR                                           (0x6U)
#define UART_USR_UART_BUSY_SHIFT                           (0U)
#define UART_USR_UART_BUSY_MASK                            (0x1U << UART_USR_UART_BUSY_SHIFT)                           /* 0x00000001 */
#define UART_USR_TRANS_FIFO_NOT_FULL_SHIFT                 (1U)
#define UART_USR_TRANS_FIFO_NOT_FULL_MASK                  (0x1U << UART_USR_TRANS_FIFO_NOT_FULL_SHIFT)                 /* 0x00000002 */
#define UART_USR_TRASN_FIFO_EMPTY_SHIFT                    (2U)
#define UART_USR_TRASN_FIFO_EMPTY_MASK                     (0x1U << UART_USR_TRASN_FIFO_EMPTY_SHIFT)                    /* 0x00000004 */
#define UART_USR_RECEIVE_FIFO_NOT_EMPTY_SHIFT              (3U)
#define UART_USR_RECEIVE_FIFO_NOT_EMPTY_MASK               (0x1U << UART_USR_RECEIVE_FIFO_NOT_EMPTY_SHIFT)              /* 0x00000008 */
#define UART_USR_RECEIVE_FIFO_FULL_SHIFT                   (4U)
#define UART_USR_RECEIVE_FIFO_FULL_MASK                    (0x1U << UART_USR_RECEIVE_FIFO_FULL_SHIFT)                   /* 0x00000010 */
/* TFL */
#define UART_TFL_OFFSET                                    (0x80U)
#define UART_TFL                                           (0x0U)
#define UART_TFL_TRANS_FIFO_LEVEL_SHIFT                    (0U)
#define UART_TFL_TRANS_FIFO_LEVEL_MASK                     (0x3FU << UART_TFL_TRANS_FIFO_LEVEL_SHIFT)                   /* 0x0000003F */
/* RFL */
#define UART_RFL_OFFSET                                    (0x84U)
#define UART_RFL                                           (0x0U)
#define UART_RFL_RECEIVE_FIFO_LEVEL_SHIFT                  (0U)
#define UART_RFL_RECEIVE_FIFO_LEVEL_MASK                   (0x3FU << UART_RFL_RECEIVE_FIFO_LEVEL_SHIFT)                 /* 0x0000003F */
/* SRR */
#define UART_SRR_OFFSET                                    (0x88U)
#define UART_SRR_UART_RESET_SHIFT                          (0U)
#define UART_SRR_UART_RESET_MASK                           (0x1U << UART_SRR_UART_RESET_SHIFT)                          /* 0x00000001 */
#define UART_SRR_RCVR_FIFO_RESET_SHIFT                     (1U)
#define UART_SRR_RCVR_FIFO_RESET_MASK                      (0x1U << UART_SRR_RCVR_FIFO_RESET_SHIFT)                     /* 0x00000002 */
#define UART_SRR_XMIT_FIFO_RESET_SHIFT                     (2U)
#define UART_SRR_XMIT_FIFO_RESET_MASK                      (0x1U << UART_SRR_XMIT_FIFO_RESET_SHIFT)                     /* 0x00000004 */
/* SRTS */
#define UART_SRTS_OFFSET                                   (0x8CU)
#define UART_SRTS_SHADOW_REQ_TO_SEND_SHIFT                 (0U)
#define UART_SRTS_SHADOW_REQ_TO_SEND_MASK                  (0x1U << UART_SRTS_SHADOW_REQ_TO_SEND_SHIFT)                 /* 0x00000001 */
/* SBCR */
#define UART_SBCR_OFFSET                                   (0x90U)
#define UART_SBCR_SHADOW_BREAK_CTRL_SHIFT                  (0U)
#define UART_SBCR_SHADOW_BREAK_CTRL_MASK                   (0x1U << UART_SBCR_SHADOW_BREAK_CTRL_SHIFT)                  /* 0x00000001 */
/* SDMAM */
#define UART_SDMAM_OFFSET                                  (0x94U)
#define UART_SDMAM_SHADOW_DMA_MODE_SHIFT                   (0U)
#define UART_SDMAM_SHADOW_DMA_MODE_MASK                    (0x1U << UART_SDMAM_SHADOW_DMA_MODE_SHIFT)                   /* 0x00000001 */
/* SFE */
#define UART_SFE_OFFSET                                    (0x98U)
#define UART_SFE_SHADOW_FIFO_EN_SHIFT                      (0U)
#define UART_SFE_SHADOW_FIFO_EN_MASK                       (0x1U << UART_SFE_SHADOW_FIFO_EN_SHIFT)                      /* 0x00000001 */
/* SRT */
#define UART_SRT_OFFSET                                    (0x9CU)
#define UART_SRT_SHADOW_RCVR_TRIGGER_SHIFT                 (0U)
#define UART_SRT_SHADOW_RCVR_TRIGGER_MASK                  (0x3U << UART_SRT_SHADOW_RCVR_TRIGGER_SHIFT)                 /* 0x00000003 */
/* STET */
#define UART_STET_OFFSET                                   (0xA0U)
#define UART_STET_SHADOW_TX_EMPTY_TRIGGER_SHIFT            (0U)
#define UART_STET_SHADOW_TX_EMPTY_TRIGGER_MASK             (0x3U << UART_STET_SHADOW_TX_EMPTY_TRIGGER_SHIFT)            /* 0x00000003 */
/* HTX */
#define UART_HTX_OFFSET                                    (0xA4U)
#define UART_HTX_HALT_TX_EN_SHIFT                          (0U)
#define UART_HTX_HALT_TX_EN_MASK                           (0x1U << UART_HTX_HALT_TX_EN_SHIFT)                          /* 0x00000001 */
/* DMASA */
#define UART_DMASA_OFFSET                                  (0xA8U)
#define UART_DMASA_DMA_SOFTWARE_ACK_SHIFT                  (0U)
#define UART_DMASA_DMA_SOFTWARE_ACK_MASK                   (0x1U << UART_DMASA_DMA_SOFTWARE_ACK_SHIFT)                  /* 0x00000001 */
/* CPR */
#define UART_CPR_OFFSET                                    (0xF4U)
#define UART_CPR                                           (0x43FF2U)
#define UART_CPR_APB_DATA_WIDTH_SHIFT                      (0U)
#define UART_CPR_APB_DATA_WIDTH_MASK                       (0x3U << UART_CPR_APB_DATA_WIDTH_SHIFT)                      /* 0x00000003 */
#define UART_CPR_AFCE_MODE_SHIFT                           (4U)
#define UART_CPR_AFCE_MODE_MASK                            (0x1U << UART_CPR_AFCE_MODE_SHIFT)                           /* 0x00000010 */
#define UART_CPR_THRE_MODE_SHIFT                           (5U)
#define UART_CPR_THRE_MODE_MASK                            (0x1U << UART_CPR_THRE_MODE_SHIFT)                           /* 0x00000020 */
#define UART_CPR_SIR_MODE_SHIFT                            (6U)
#define UART_CPR_SIR_MODE_MASK                             (0x1U << UART_CPR_SIR_MODE_SHIFT)                            /* 0x00000040 */
#define UART_CPR_SIR_LP_MODE_SHIFT                         (7U)
#define UART_CPR_SIR_LP_MODE_MASK                          (0x1U << UART_CPR_SIR_LP_MODE_SHIFT)                         /* 0x00000080 */
#define UART_CPR_NEW_FEAT_SHIFT                            (8U)
#define UART_CPR_NEW_FEAT_MASK                             (0x1U << UART_CPR_NEW_FEAT_SHIFT)                            /* 0x00000100 */
#define UART_CPR_FIFO_ACCESS_SHIFT                         (9U)
#define UART_CPR_FIFO_ACCESS_MASK                          (0x1U << UART_CPR_FIFO_ACCESS_SHIFT)                         /* 0x00000200 */
#define UART_CPR_FIFO_STAT_SHIFT                           (10U)
#define UART_CPR_FIFO_STAT_MASK                            (0x1U << UART_CPR_FIFO_STAT_SHIFT)                           /* 0x00000400 */
#define UART_CPR_SHADOW_SHIFT                              (11U)
#define UART_CPR_SHADOW_MASK                               (0x1U << UART_CPR_SHADOW_SHIFT)                              /* 0x00000800 */
#define UART_CPR_UART_ADD_ENCODED_PARAMS_SHIFT             (12U)
#define UART_CPR_UART_ADD_ENCODED_PARAMS_MASK              (0x1U << UART_CPR_UART_ADD_ENCODED_PARAMS_SHIFT)             /* 0x00001000 */
#define UART_CPR_DMA_EXTRA_SHIFT                           (13U)
#define UART_CPR_DMA_EXTRA_MASK                            (0x1U << UART_CPR_DMA_EXTRA_SHIFT)                           /* 0x00002000 */
#define UART_CPR_FIFO_MODE_SHIFT                           (16U)
#define UART_CPR_FIFO_MODE_MASK                            (0xFFU << UART_CPR_FIFO_MODE_SHIFT)                          /* 0x00FF0000 */
/* UCV */
#define UART_UCV_OFFSET                                    (0xF8U)
#define UART_UCV                                           (0x3430322AU)
#define UART_UCV_VER_SHIFT                                 (0U)
#define UART_UCV_VER_MASK                                  (0xFFFFFFFFU << UART_UCV_VER_SHIFT)                          /* 0xFFFFFFFF */
/* CTR */
#define UART_CTR_OFFSET                                    (0xFCU)
#define UART_CTR                                           (0x44570110U)
#define UART_CTR_PERIPHERAL_ID_SHIFT                       (0U)
#define UART_CTR_PERIPHERAL_ID_MASK                        (0xFFFFFFFFU << UART_CTR_PERIPHERAL_ID_SHIFT)                /* 0xFFFFFFFF */
/******************************************WDT*******************************************/
/* CR */
#define WDT_CR_OFFSET                                      (0x0U)
#define WDT_CR_EN_SHIFT                                    (0U)
#define WDT_CR_EN_MASK                                     (0x1U << WDT_CR_EN_SHIFT)                                    /* 0x00000001 */
#define WDT_CR_RESP_MODE_SHIFT                             (1U)
#define WDT_CR_RESP_MODE_MASK                              (0x1U << WDT_CR_RESP_MODE_SHIFT)                             /* 0x00000002 */
#define WDT_CR_RST_PLUSE_LENGTH_SHIFT                      (2U)
#define WDT_CR_RST_PLUSE_LENGTH_MASK                       (0x7U << WDT_CR_RST_PLUSE_LENGTH_SHIFT)                      /* 0x0000001C */
/* TORR */
#define WDT_TORR_OFFSET                                    (0x4U)
#define WDT_TORR_TIMEOUT_PERIOD_SHIFT                      (0U)
#define WDT_TORR_TIMEOUT_PERIOD_MASK                       (0xFU << WDT_TORR_TIMEOUT_PERIOD_SHIFT)                      /* 0x0000000F */
/* CCVR */
#define WDT_CCVR_OFFSET                                    (0x8U)
#define WDT_CCVR                                           (0xFFFFU)
#define WDT_CCVR_CUR_CNT_SHIFT                             (0U)
#define WDT_CCVR_CUR_CNT_MASK                              (0xFFFFFFFFU << WDT_CCVR_CUR_CNT_SHIFT)                      /* 0xFFFFFFFF */
/* CRR */
#define WDT_CRR_OFFSET                                     (0xCU)
#define WDT_CRR_CNT_RESTART_SHIFT                          (0U)
#define WDT_CRR_CNT_RESTART_MASK                           (0xFFU << WDT_CRR_CNT_RESTART_SHIFT)                         /* 0x000000FF */
/* STAT */
#define WDT_STAT_OFFSET                                    (0x10U)
#define WDT_STAT                                           (0x0U)
#define WDT_STAT_STATUS_SHIFT                              (0U)
#define WDT_STAT_STATUS_MASK                               (0x1U << WDT_STAT_STATUS_SHIFT)                              /* 0x00000001 */
/* EOI */
#define WDT_EOI_OFFSET                                     (0x14U)
#define WDT_EOI                                            (0x0U)
#define WDT_EOI_INT_CLR_SHIFT                              (0U)
#define WDT_EOI_INT_CLR_MASK                               (0x1U << WDT_EOI_INT_CLR_SHIFT)                              /* 0x00000001 */
/*****************************************TIMER******************************************/
/* LOAD_COUNT0 */
#define TIMER_LOAD_COUNT0_OFFSET                           (0x0U)
#define TIMER_LOAD_COUNT0_COUNT0_SHIFT                     (0U)
#define TIMER_LOAD_COUNT0_COUNT0_MASK                      (0xFFFFFFFFU << TIMER_LOAD_COUNT0_COUNT0_SHIFT)              /* 0xFFFFFFFF */
/* LOAD_COUNT1 */
#define TIMER_LOAD_COUNT1_OFFSET                           (0x4U)
#define TIMER_LOAD_COUNT1_COUNT1_SHIFT                     (0U)
#define TIMER_LOAD_COUNT1_COUNT1_MASK                      (0xFFFFFFFFU << TIMER_LOAD_COUNT1_COUNT1_SHIFT)              /* 0xFFFFFFFF */
/* CURRENT_VALUE0 */
#define TIMER_CURRENT_VALUE0_OFFSET                        (0x8U)
#define TIMER_CURRENT_VALUE0                               (0x0U)
#define TIMER_CURRENT_VALUE0_CURRENT_VALUE0_SHIFT          (0U)
#define TIMER_CURRENT_VALUE0_CURRENT_VALUE0_MASK           (0xFFFFFFFFU << TIMER_CURRENT_VALUE0_CURRENT_VALUE0_SHIFT)   /* 0xFFFFFFFF */
/* CURRENT_VALUE1 */
#define TIMER_CURRENT_VALUE1_OFFSET                        (0xCU)
#define TIMER_CURRENT_VALUE1                               (0x0U)
#define TIMER_CURRENT_VALUE1_CURRENT_VALUE1_SHIFT          (0U)
#define TIMER_CURRENT_VALUE1_CURRENT_VALUE1_MASK           (0xFFFFFFFFU << TIMER_CURRENT_VALUE1_CURRENT_VALUE1_SHIFT)   /* 0xFFFFFFFF */
/* CONTROLREG */
#define TIMER_CONTROLREG_OFFSET                            (0x10U)
#define TIMER_CONTROLREG_TIMER_ENABLE_SHIFT                (0U)
#define TIMER_CONTROLREG_TIMER_ENABLE_MASK                 (0x1U << TIMER_CONTROLREG_TIMER_ENABLE_SHIFT)                /* 0x00000001 */
#define TIMER_CONTROLREG_TIMER_MODE_SHIFT                  (1U)
#define TIMER_CONTROLREG_TIMER_MODE_MASK                   (0x1U << TIMER_CONTROLREG_TIMER_MODE_SHIFT)                  /* 0x00000002 */
#define TIMER_CONTROLREG_TIMER_INT_MASK_SHIFT              (2U)
#define TIMER_CONTROLREG_TIMER_INT_MASK_MASK               (0x1U << TIMER_CONTROLREG_TIMER_INT_MASK_SHIFT)              /* 0x00000004 */
/* INTSTATUS */
#define TIMER_INTSTATUS_OFFSET                             (0x18U)
#define TIMER_INTSTATUS_INT_PD_SHIFT                       (0U)
#define TIMER_INTSTATUS_INT_PD_MASK                        (0x1U << TIMER_INTSTATUS_INT_PD_SHIFT)                       /* 0x00000001 */
/******************************************FSPI******************************************/
/* CTRL0 */
#define FSPI_CTRL0_OFFSET                                  (0x0U)
#define FSPI_CTRL0_SPIM_SHIFT                              (0U)
#define FSPI_CTRL0_SPIM_MASK                               (0x1U << FSPI_CTRL0_SPIM_SHIFT)                              /* 0x00000001 */
#define FSPI_CTRL0_SHIFTPHASE_SHIFT                        (1U)
#define FSPI_CTRL0_SHIFTPHASE_MASK                         (0x1U << FSPI_CTRL0_SHIFTPHASE_SHIFT)                        /* 0x00000002 */
#define FSPI_CTRL0_IDLE_CYCLE_SHIFT                        (4U)
#define FSPI_CTRL0_IDLE_CYCLE_MASK                         (0xFU << FSPI_CTRL0_IDLE_CYCLE_SHIFT)                        /* 0x000000F0 */
#define FSPI_CTRL0_CMDB_SHIFT                              (8U)
#define FSPI_CTRL0_CMDB_MASK                               (0x3U << FSPI_CTRL0_CMDB_SHIFT)                              /* 0x00000300 */
#define FSPI_CTRL0_ADRB_SHIFT                              (10U)
#define FSPI_CTRL0_ADRB_MASK                               (0x3U << FSPI_CTRL0_ADRB_SHIFT)                              /* 0x00000C00 */
#define FSPI_CTRL0_DATB_SHIFT                              (12U)
#define FSPI_CTRL0_DATB_MASK                               (0x3U << FSPI_CTRL0_DATB_SHIFT)                              /* 0x00003000 */
/* IMR */
#define FSPI_IMR_OFFSET                                    (0x4U)
#define FSPI_IMR_RXFM_SHIFT                                (0U)
#define FSPI_IMR_RXFM_MASK                                 (0x1U << FSPI_IMR_RXFM_SHIFT)                                /* 0x00000001 */
#define FSPI_IMR_RXUM_SHIFT                                (1U)
#define FSPI_IMR_RXUM_MASK                                 (0x1U << FSPI_IMR_RXUM_SHIFT)                                /* 0x00000002 */
#define FSPI_IMR_TXOM_SHIFT                                (2U)
#define FSPI_IMR_TXOM_MASK                                 (0x1U << FSPI_IMR_TXOM_SHIFT)                                /* 0x00000004 */
#define FSPI_IMR_TXEM_SHIFT                                (3U)
#define FSPI_IMR_TXEM_MASK                                 (0x1U << FSPI_IMR_TXEM_SHIFT)                                /* 0x00000008 */
#define FSPI_IMR_TRANSM_SHIFT                              (4U)
#define FSPI_IMR_TRANSM_MASK                               (0x1U << FSPI_IMR_TRANSM_SHIFT)                              /* 0x00000010 */
#define FSPI_IMR_AHBM_SHIFT                                (5U)
#define FSPI_IMR_AHBM_MASK                                 (0x1U << FSPI_IMR_AHBM_SHIFT)                                /* 0x00000020 */
#define FSPI_IMR_NSPIM_SHIFT                               (6U)
#define FSPI_IMR_NSPIM_MASK                                (0x1U << FSPI_IMR_NSPIM_SHIFT)                               /* 0x00000040 */
#define FSPI_IMR_DMAM_SHIFT                                (7U)
#define FSPI_IMR_DMAM_MASK                                 (0x1U << FSPI_IMR_DMAM_SHIFT)                                /* 0x00000080 */
/* ICLR */
#define FSPI_ICLR_OFFSET                                   (0x8U)
#define FSPI_ICLR_RXFC_SHIFT                               (0U)
#define FSPI_ICLR_RXFC_MASK                                (0x1U << FSPI_ICLR_RXFC_SHIFT)                               /* 0x00000001 */
#define FSPI_ICLR_RXUC_SHIFT                               (1U)
#define FSPI_ICLR_RXUC_MASK                                (0x1U << FSPI_ICLR_RXUC_SHIFT)                               /* 0x00000002 */
#define FSPI_ICLR_TXOC_SHIFT                               (2U)
#define FSPI_ICLR_TXOC_MASK                                (0x1U << FSPI_ICLR_TXOC_SHIFT)                               /* 0x00000004 */
#define FSPI_ICLR_TXEC_SHIFT                               (3U)
#define FSPI_ICLR_TXEC_MASK                                (0x1U << FSPI_ICLR_TXEC_SHIFT)                               /* 0x00000008 */
#define FSPI_ICLR_TRANSC_SHIFT                             (4U)
#define FSPI_ICLR_TRANSC_MASK                              (0x1U << FSPI_ICLR_TRANSC_SHIFT)                             /* 0x00000010 */
#define FSPI_ICLR_AHBC_SHIFT                               (5U)
#define FSPI_ICLR_AHBC_MASK                                (0x1U << FSPI_ICLR_AHBC_SHIFT)                               /* 0x00000020 */
#define FSPI_ICLR_NSPIC_SHIFT                              (6U)
#define FSPI_ICLR_NSPIC_MASK                               (0x1U << FSPI_ICLR_NSPIC_SHIFT)                              /* 0x00000040 */
#define FSPI_ICLR_DMAC_SHIFT                               (7U)
#define FSPI_ICLR_DMAC_MASK                                (0x1U << FSPI_ICLR_DMAC_SHIFT)                               /* 0x00000080 */
/* FTLR */
#define FSPI_FTLR_OFFSET                                   (0xCU)
#define FSPI_FTLR_TXFTLR_SHIFT                             (0U)
#define FSPI_FTLR_TXFTLR_MASK                              (0xFFU << FSPI_FTLR_TXFTLR_SHIFT)                            /* 0x000000FF */
#define FSPI_FTLR_RXFTLR_SHIFT                             (8U)
#define FSPI_FTLR_RXFTLR_MASK                              (0xFFU << FSPI_FTLR_RXFTLR_SHIFT)                            /* 0x0000FF00 */
/* RCVR */
#define FSPI_RCVR_OFFSET                                   (0x10U)
#define FSPI_RCVR_RCVR_SHIFT                               (0U)
#define FSPI_RCVR_RCVR_MASK                                (0x1U << FSPI_RCVR_RCVR_SHIFT)                               /* 0x00000001 */
/* AX0 */
#define FSPI_AX0_OFFSET                                    (0x14U)
#define FSPI_AX0_AX_SHIFT                                  (0U)
#define FSPI_AX0_AX_MASK                                   (0xFFU << FSPI_AX0_AX_SHIFT)                                 /* 0x000000FF */
/* ABIT0 */
#define FSPI_ABIT0_OFFSET                                  (0x18U)
#define FSPI_ABIT0_ABIT_SHIFT                              (0U)
#define FSPI_ABIT0_ABIT_MASK                               (0x1FU << FSPI_ABIT0_ABIT_SHIFT)                             /* 0x0000001F */
/* ISR */
#define FSPI_ISR_OFFSET                                    (0x1CU)
#define FSPI_ISR_RXFS_SHIFT                                (0U)
#define FSPI_ISR_RXFS_MASK                                 (0x1U << FSPI_ISR_RXFS_SHIFT)                                /* 0x00000001 */
#define FSPI_ISR_RXUS_SHIFT                                (1U)
#define FSPI_ISR_RXUS_MASK                                 (0x1U << FSPI_ISR_RXUS_SHIFT)                                /* 0x00000002 */
#define FSPI_ISR_TXOS_SHIFT                                (2U)
#define FSPI_ISR_TXOS_MASK                                 (0x1U << FSPI_ISR_TXOS_SHIFT)                                /* 0x00000004 */
#define FSPI_ISR_TXES_SHIFT                                (3U)
#define FSPI_ISR_TXES_MASK                                 (0x1U << FSPI_ISR_TXES_SHIFT)                                /* 0x00000008 */
#define FSPI_ISR_TRANSS_SHIFT                              (4U)
#define FSPI_ISR_TRANSS_MASK                               (0x1U << FSPI_ISR_TRANSS_SHIFT)                              /* 0x00000010 */
#define FSPI_ISR_AHBS_SHIFT                                (5U)
#define FSPI_ISR_AHBS_MASK                                 (0x1U << FSPI_ISR_AHBS_SHIFT)                                /* 0x00000020 */
#define FSPI_ISR_NSPIS_SHIFT                               (6U)
#define FSPI_ISR_NSPIS_MASK                                (0x1U << FSPI_ISR_NSPIS_SHIFT)                               /* 0x00000040 */
#define FSPI_ISR_DMAS_SHIFT                                (7U)
#define FSPI_ISR_DMAS_MASK                                 (0x1U << FSPI_ISR_DMAS_SHIFT)                                /* 0x00000080 */
/* FSR */
#define FSPI_FSR_OFFSET                                    (0x20U)
#define FSPI_FSR_TXFS_SHIFT                                (0U)
#define FSPI_FSR_TXFS_MASK                                 (0x1U << FSPI_FSR_TXFS_SHIFT)                                /* 0x00000001 */
#define FSPI_FSR_TXES_SHIFT                                (1U)
#define FSPI_FSR_TXES_MASK                                 (0x1U << FSPI_FSR_TXES_SHIFT)                                /* 0x00000002 */
#define FSPI_FSR_RXES_SHIFT                                (2U)
#define FSPI_FSR_RXES_MASK                                 (0x1U << FSPI_FSR_RXES_SHIFT)                                /* 0x00000004 */
#define FSPI_FSR_RXFS_SHIFT                                (3U)
#define FSPI_FSR_RXFS_MASK                                 (0x1U << FSPI_FSR_RXFS_SHIFT)                                /* 0x00000008 */
#define FSPI_FSR_TXWLVL_SHIFT                              (8U)
#define FSPI_FSR_TXWLVL_MASK                               (0x1FU << FSPI_FSR_TXWLVL_SHIFT)                             /* 0x00001F00 */
#define FSPI_FSR_RXWLVL_SHIFT                              (16U)
#define FSPI_FSR_RXWLVL_MASK                               (0x1FU << FSPI_FSR_RXWLVL_SHIFT)                             /* 0x001F0000 */
/* SR */
#define FSPI_SR_OFFSET                                     (0x24U)
#define FSPI_SR                                            (0x0U)
#define FSPI_SR_SR_SHIFT                                   (0U)
#define FSPI_SR_SR_MASK                                    (0x1U << FSPI_SR_SR_SHIFT)                                   /* 0x00000001 */
/* RISR */
#define FSPI_RISR_OFFSET                                   (0x28U)
#define FSPI_RISR                                          (0x0U)
#define FSPI_RISR_RXFS_SHIFT                               (0U)
#define FSPI_RISR_RXFS_MASK                                (0x1U << FSPI_RISR_RXFS_SHIFT)                               /* 0x00000001 */
#define FSPI_RISR_RXUS_SHIFT                               (1U)
#define FSPI_RISR_RXUS_MASK                                (0x1U << FSPI_RISR_RXUS_SHIFT)                               /* 0x00000002 */
#define FSPI_RISR_TXOS_SHIFT                               (2U)
#define FSPI_RISR_TXOS_MASK                                (0x1U << FSPI_RISR_TXOS_SHIFT)                               /* 0x00000004 */
#define FSPI_RISR_TXES_SHIFT                               (3U)
#define FSPI_RISR_TXES_MASK                                (0x1U << FSPI_RISR_TXES_SHIFT)                               /* 0x00000008 */
#define FSPI_RISR_TRANSS_SHIFT                             (4U)
#define FSPI_RISR_TRANSS_MASK                              (0x1U << FSPI_RISR_TRANSS_SHIFT)                             /* 0x00000010 */
#define FSPI_RISR_AHBS_SHIFT                               (5U)
#define FSPI_RISR_AHBS_MASK                                (0x1U << FSPI_RISR_AHBS_SHIFT)                               /* 0x00000020 */
#define FSPI_RISR_NSPIS_SHIFT                              (6U)
#define FSPI_RISR_NSPIS_MASK                               (0x1U << FSPI_RISR_NSPIS_SHIFT)                              /* 0x00000040 */
#define FSPI_RISR_DMAS_SHIFT                               (7U)
#define FSPI_RISR_DMAS_MASK                                (0x1U << FSPI_RISR_DMAS_SHIFT)                               /* 0x00000080 */
/* VER */
#define FSPI_VER_OFFSET                                    (0x2CU)
#define FSPI_VER                                           (0x5U)
#define FSPI_VER_VER_SHIFT                                 (0U)
#define FSPI_VER_VER_MASK                                  (0xFFFFU << FSPI_VER_VER_SHIFT)                              /* 0x0000FFFF */
/* QOP */
#define FSPI_QOP_OFFSET                                    (0x30U)
#define FSPI_QOP_SO123_SHIFT                               (0U)
#define FSPI_QOP_SO123_MASK                                (0x1U << FSPI_QOP_SO123_SHIFT)                               /* 0x00000001 */
#define FSPI_QOP_SO123BP_SHIFT                             (1U)
#define FSPI_QOP_SO123BP_MASK                              (0x1U << FSPI_QOP_SO123BP_SHIFT)                             /* 0x00000002 */
/* EXT_CTRL */
#define FSPI_EXT_CTRL_OFFSET                               (0x34U)
#define FSPI_EXT_CTRL_CS_DESEL_CTRL_SHIFT                  (0U)
#define FSPI_EXT_CTRL_CS_DESEL_CTRL_MASK                   (0xFU << FSPI_EXT_CTRL_CS_DESEL_CTRL_SHIFT)                  /* 0x0000000F */
#define FSPI_EXT_CTRL_SWITCH_IO_DUMM_CNT_SHIFT             (4U)
#define FSPI_EXT_CTRL_SWITCH_IO_DUMM_CNT_MASK              (0xFU << FSPI_EXT_CTRL_SWITCH_IO_DUMM_CNT_SHIFT)             /* 0x000000F0 */
#define FSPI_EXT_CTRL_SWITCH_IO_O2I_CNT_SHIFT              (8U)
#define FSPI_EXT_CTRL_SWITCH_IO_O2I_CNT_MASK               (0xFU << FSPI_EXT_CTRL_SWITCH_IO_O2I_CNT_SHIFT)              /* 0x00000F00 */
#define FSPI_EXT_CTRL_TRANS_INT_MODE_SHIFT                 (13U)
#define FSPI_EXT_CTRL_TRANS_INT_MODE_MASK                  (0x1U << FSPI_EXT_CTRL_TRANS_INT_MODE_SHIFT)                 /* 0x00002000 */
#define FSPI_EXT_CTRL_SR_GEN_MODE_SHIFT                    (14U)
#define FSPI_EXT_CTRL_SR_GEN_MODE_MASK                     (0x1U << FSPI_EXT_CTRL_SR_GEN_MODE_SHIFT)                    /* 0x00004000 */
/* DLL_CTRL0 */
#define FSPI_DLL_CTRL0_OFFSET                              (0x3CU)
#define FSPI_DLL_CTRL0_SMP_DLL_CFG_SHIFT                   (0U)
#define FSPI_DLL_CTRL0_SMP_DLL_CFG_MASK                    (0x1FFU << FSPI_DLL_CTRL0_SMP_DLL_CFG_SHIFT)                 /* 0x000001FF */
#define FSPI_DLL_CTRL0_SCLK_SMP_SEL_SHIFT                  (15U)
#define FSPI_DLL_CTRL0_SCLK_SMP_SEL_MASK                   (0x1U << FSPI_DLL_CTRL0_SCLK_SMP_SEL_SHIFT)                  /* 0x00008000 */
/* EXT_AX */
#define FSPI_EXT_AX_OFFSET                                 (0x44U)
#define FSPI_EXT_AX_AX_CANCEL_PAT_SHIFT                    (0U)
#define FSPI_EXT_AX_AX_CANCEL_PAT_MASK                     (0xFFU << FSPI_EXT_AX_AX_CANCEL_PAT_SHIFT)                   /* 0x000000FF */
#define FSPI_EXT_AX_AX_SETUP_PAT_SHIFT                     (8U)
#define FSPI_EXT_AX_AX_SETUP_PAT_MASK                      (0xFFU << FSPI_EXT_AX_AX_SETUP_PAT_SHIFT)                    /* 0x0000FF00 */
/* SCLK_INATM_CNT */
#define FSPI_SCLK_INATM_CNT_OFFSET                         (0x48U)
#define FSPI_SCLK_INATM_CNT_SCLK_INATM_CNT_SHIFT           (0U)
#define FSPI_SCLK_INATM_CNT_SCLK_INATM_CNT_MASK            (0xFFFFFFFFU << FSPI_SCLK_INATM_CNT_SCLK_INATM_CNT_SHIFT)    /* 0xFFFFFFFF */
/* XMMC_WCMD0 */
#define FSPI_XMMC_WCMD0_OFFSET                             (0x50U)
#define FSPI_XMMC_WCMD0_CMD_SHIFT                          (0U)
#define FSPI_XMMC_WCMD0_CMD_MASK                           (0xFFU << FSPI_XMMC_WCMD0_CMD_SHIFT)                         /* 0x000000FF */
#define FSPI_XMMC_WCMD0_DUMM_SHIFT                         (8U)
#define FSPI_XMMC_WCMD0_DUMM_MASK                          (0xFU << FSPI_XMMC_WCMD0_DUMM_SHIFT)                         /* 0x00000F00 */
#define FSPI_XMMC_WCMD0_CONT_SHIFT                         (13U)
#define FSPI_XMMC_WCMD0_CONT_MASK                          (0x1U << FSPI_XMMC_WCMD0_CONT_SHIFT)                         /* 0x00002000 */
#define FSPI_XMMC_WCMD0_ADDRB_SHIFT                        (14U)
#define FSPI_XMMC_WCMD0_ADDRB_MASK                         (0x3U << FSPI_XMMC_WCMD0_ADDRB_SHIFT)                        /* 0x0000C000 */
/* XMMC_RCMD0 */
#define FSPI_XMMC_RCMD0_OFFSET                             (0x54U)
#define FSPI_XMMC_RCMD0_CMD_SHIFT                          (0U)
#define FSPI_XMMC_RCMD0_CMD_MASK                           (0xFFU << FSPI_XMMC_RCMD0_CMD_SHIFT)                         /* 0x000000FF */
#define FSPI_XMMC_RCMD0_DUMM_SHIFT                         (8U)
#define FSPI_XMMC_RCMD0_DUMM_MASK                          (0xFU << FSPI_XMMC_RCMD0_DUMM_SHIFT)                         /* 0x00000F00 */
#define FSPI_XMMC_RCMD0_CONT_SHIFT                         (13U)
#define FSPI_XMMC_RCMD0_CONT_MASK                          (0x1U << FSPI_XMMC_RCMD0_CONT_SHIFT)                         /* 0x00002000 */
#define FSPI_XMMC_RCMD0_ADDRB_SHIFT                        (14U)
#define FSPI_XMMC_RCMD0_ADDRB_MASK                         (0x3U << FSPI_XMMC_RCMD0_ADDRB_SHIFT)                        /* 0x0000C000 */
/* XMMC_CTRL */
#define FSPI_XMMC_CTRL_OFFSET                              (0x58U)
#define FSPI_XMMC_CTRL_DEV_HWEN_SHIFT                      (5U)
#define FSPI_XMMC_CTRL_DEV_HWEN_MASK                       (0x1U << FSPI_XMMC_CTRL_DEV_HWEN_SHIFT)                      /* 0x00000020 */
#define FSPI_XMMC_CTRL_PFT_EN_SHIFT                        (6U)
#define FSPI_XMMC_CTRL_PFT_EN_MASK                         (0x1U << FSPI_XMMC_CTRL_PFT_EN_SHIFT)                        /* 0x00000040 */
/* MODE */
#define FSPI_MODE_OFFSET                                   (0x5CU)
#define FSPI_MODE_XMMC_MODE_EN_SHIFT                       (0U)
#define FSPI_MODE_XMMC_MODE_EN_MASK                        (0x1U << FSPI_MODE_XMMC_MODE_EN_SHIFT)                       /* 0x00000001 */
/* DEVRGN */
#define FSPI_DEVRGN_OFFSET                                 (0x60U)
#define FSPI_DEVRGN_RSIZE_SHIFT                            (0U)
#define FSPI_DEVRGN_RSIZE_MASK                             (0x1FU << FSPI_DEVRGN_RSIZE_SHIFT)                           /* 0x0000001F */
#define FSPI_DEVRGN_DEC_CTRL_SHIFT                         (8U)
#define FSPI_DEVRGN_DEC_CTRL_MASK                          (0x3U << FSPI_DEVRGN_DEC_CTRL_SHIFT)                         /* 0x00000300 */
/* DEVSIZE0 */
#define FSPI_DEVSIZE0_OFFSET                               (0x64U)
#define FSPI_DEVSIZE0_DSIZE_SHIFT                          (0U)
#define FSPI_DEVSIZE0_DSIZE_MASK                           (0x1FU << FSPI_DEVSIZE0_DSIZE_SHIFT)                         /* 0x0000001F */
/* TME0 */
#define FSPI_TME0_OFFSET                                   (0x68U)
#define FSPI_TME0_SCLK_INATM_EN_SHIFT                      (1U)
#define FSPI_TME0_SCLK_INATM_EN_MASK                       (0x1U << FSPI_TME0_SCLK_INATM_EN_SHIFT)                      /* 0x00000002 */
/* XMMC_RX_WTMRK */
#define FSPI_XMMC_RX_WTMRK_OFFSET                          (0x70U)
#define FSPI_XMMC_RX_WTMRK_RX_FULL_WTMRK_SHIFT             (0U)
#define FSPI_XMMC_RX_WTMRK_RX_FULL_WTMRK_MASK              (0xFFU << FSPI_XMMC_RX_WTMRK_RX_FULL_WTMRK_SHIFT)            /* 0x000000FF */
/* DMATR */
#define FSPI_DMATR_OFFSET                                  (0x80U)
#define FSPI_DMATR_DMATR_SHIFT                             (0U)
#define FSPI_DMATR_DMATR_MASK                              (0x1U << FSPI_DMATR_DMATR_SHIFT)                             /* 0x00000001 */
/* DMAADDR */
#define FSPI_DMAADDR_OFFSET                                (0x84U)
#define FSPI_DMAADDR_DMAADDR_SHIFT                         (0U)
#define FSPI_DMAADDR_DMAADDR_MASK                          (0xFFFFFFFFU << FSPI_DMAADDR_DMAADDR_SHIFT)                  /* 0xFFFFFFFF */
/* LEN_CTRL */
#define FSPI_LEN_CTRL_OFFSET                               (0x88U)
#define FSPI_LEN_CTRL_TRB_SEL_SHIFT                        (0U)
#define FSPI_LEN_CTRL_TRB_SEL_MASK                         (0x1U << FSPI_LEN_CTRL_TRB_SEL_SHIFT)                        /* 0x00000001 */
/* LEN_EXT */
#define FSPI_LEN_EXT_OFFSET                                (0x8CU)
#define FSPI_LEN_EXT_TRB_EXT_SHIFT                         (0U)
#define FSPI_LEN_EXT_TRB_EXT_MASK                          (0xFFFFFFFFU << FSPI_LEN_EXT_TRB_EXT_SHIFT)                  /* 0xFFFFFFFF */
/* XMMCSR */
#define FSPI_XMMCSR_OFFSET                                 (0x94U)
#define FSPI_XMMCSR_SLOPOVER0_SHIFT                        (0U)
#define FSPI_XMMCSR_SLOPOVER0_MASK                         (0x1U << FSPI_XMMCSR_SLOPOVER0_SHIFT)                        /* 0x00000001 */
#define FSPI_XMMCSR_SLOPOVER1_SHIFT                        (1U)
#define FSPI_XMMCSR_SLOPOVER1_MASK                         (0x1U << FSPI_XMMCSR_SLOPOVER1_SHIFT)                        /* 0x00000002 */
/* CMD */
#define FSPI_CMD_OFFSET                                    (0x100U)
#define FSPI_CMD_CMD_SHIFT                                 (0U)
#define FSPI_CMD_CMD_MASK                                  (0xFFU << FSPI_CMD_CMD_SHIFT)                                /* 0x000000FF */
#define FSPI_CMD_DUMM_SHIFT                                (8U)
#define FSPI_CMD_DUMM_MASK                                 (0xFU << FSPI_CMD_DUMM_SHIFT)                                /* 0x00000F00 */
#define FSPI_CMD_WR_SHIFT                                  (12U)
#define FSPI_CMD_WR_MASK                                   (0x1U << FSPI_CMD_WR_SHIFT)                                  /* 0x00001000 */
#define FSPI_CMD_CONT_SHIFT                                (13U)
#define FSPI_CMD_CONT_MASK                                 (0x1U << FSPI_CMD_CONT_SHIFT)                                /* 0x00002000 */
#define FSPI_CMD_ADDRB_SHIFT                               (14U)
#define FSPI_CMD_ADDRB_MASK                                (0x3U << FSPI_CMD_ADDRB_SHIFT)                               /* 0x0000C000 */
#define FSPI_CMD_TRB_SHIFT                                 (16U)
#define FSPI_CMD_TRB_MASK                                  (0x3FFFU << FSPI_CMD_TRB_SHIFT)                              /* 0x3FFF0000 */
#define FSPI_CMD_CS_SHIFT                                  (30U)
#define FSPI_CMD_CS_MASK                                   (0x3U << FSPI_CMD_CS_SHIFT)                                  /* 0xC0000000 */
/* ADDR */
#define FSPI_ADDR_OFFSET                                   (0x104U)
#define FSPI_ADDR_ADDR_SHIFT                               (0U)
#define FSPI_ADDR_ADDR_MASK                                (0xFFFFFFFFU << FSPI_ADDR_ADDR_SHIFT)                        /* 0xFFFFFFFF */
/* DATA */
#define FSPI_DATA_OFFSET                                   (0x108U)
#define FSPI_DATA_DATA_SHIFT                               (0U)
#define FSPI_DATA_DATA_MASK                                (0xFFFFFFFFU << FSPI_DATA_DATA_SHIFT)                        /* 0xFFFFFFFF */
/* CTRL1 */
#define FSPI_CTRL1_OFFSET                                  (0x200U)
#define FSPI_CTRL1_SPIM_SHIFT                              (0U)
#define FSPI_CTRL1_SPIM_MASK                               (0x1U << FSPI_CTRL1_SPIM_SHIFT)                              /* 0x00000001 */
#define FSPI_CTRL1_SHIFTPHASE_SHIFT                        (1U)
#define FSPI_CTRL1_SHIFTPHASE_MASK                         (0x1U << FSPI_CTRL1_SHIFTPHASE_SHIFT)                        /* 0x00000002 */
#define FSPI_CTRL1_IDLE_CYCLE_SHIFT                        (4U)
#define FSPI_CTRL1_IDLE_CYCLE_MASK                         (0xFU << FSPI_CTRL1_IDLE_CYCLE_SHIFT)                        /* 0x000000F0 */
#define FSPI_CTRL1_CMDB_SHIFT                              (8U)
#define FSPI_CTRL1_CMDB_MASK                               (0x3U << FSPI_CTRL1_CMDB_SHIFT)                              /* 0x00000300 */
#define FSPI_CTRL1_ADRB_SHIFT                              (10U)
#define FSPI_CTRL1_ADRB_MASK                               (0x3U << FSPI_CTRL1_ADRB_SHIFT)                              /* 0x00000C00 */
#define FSPI_CTRL1_DATB_SHIFT                              (12U)
#define FSPI_CTRL1_DATB_MASK                               (0x3U << FSPI_CTRL1_DATB_SHIFT)                              /* 0x00003000 */
/* AX1 */
#define FSPI_AX1_OFFSET                                    (0x214U)
#define FSPI_AX1_AX_SHIFT                                  (0U)
#define FSPI_AX1_AX_MASK                                   (0xFFU << FSPI_AX1_AX_SHIFT)                                 /* 0x000000FF */
/* ABIT1 */
#define FSPI_ABIT1_OFFSET                                  (0x218U)
#define FSPI_ABIT1_ABIT_SHIFT                              (0U)
#define FSPI_ABIT1_ABIT_MASK                               (0x1FU << FSPI_ABIT1_ABIT_SHIFT)                             /* 0x0000001F */
/* DLL_CTRL1 */
#define FSPI_DLL_CTRL1_OFFSET                              (0x23CU)
#define FSPI_DLL_CTRL1_SMP_DLL_CFG_SHIFT                   (0U)
#define FSPI_DLL_CTRL1_SMP_DLL_CFG_MASK                    (0x1FFU << FSPI_DLL_CTRL1_SMP_DLL_CFG_SHIFT)                 /* 0x000001FF */
#define FSPI_DLL_CTRL1_SCLK_SMP_SEL_SHIFT                  (15U)
#define FSPI_DLL_CTRL1_SCLK_SMP_SEL_MASK                   (0x1U << FSPI_DLL_CTRL1_SCLK_SMP_SEL_SHIFT)                  /* 0x00008000 */
/* XMMC_WCMD1 */
#define FSPI_XMMC_WCMD1_OFFSET                             (0x250U)
#define FSPI_XMMC_WCMD1_CMD_SHIFT                          (0U)
#define FSPI_XMMC_WCMD1_CMD_MASK                           (0xFFU << FSPI_XMMC_WCMD1_CMD_SHIFT)                         /* 0x000000FF */
#define FSPI_XMMC_WCMD1_DUMM_SHIFT                         (8U)
#define FSPI_XMMC_WCMD1_DUMM_MASK                          (0xFU << FSPI_XMMC_WCMD1_DUMM_SHIFT)                         /* 0x00000F00 */
#define FSPI_XMMC_WCMD1_CONT_SHIFT                         (13U)
#define FSPI_XMMC_WCMD1_CONT_MASK                          (0x1U << FSPI_XMMC_WCMD1_CONT_SHIFT)                         /* 0x00002000 */
#define FSPI_XMMC_WCMD1_ADDRB_SHIFT                        (14U)
#define FSPI_XMMC_WCMD1_ADDRB_MASK                         (0x3U << FSPI_XMMC_WCMD1_ADDRB_SHIFT)                        /* 0x0000C000 */
/* XMMC_RCMD1 */
#define FSPI_XMMC_RCMD1_OFFSET                             (0x254U)
#define FSPI_XMMC_RCMD1_CMD_SHIFT                          (0U)
#define FSPI_XMMC_RCMD1_CMD_MASK                           (0xFFU << FSPI_XMMC_RCMD1_CMD_SHIFT)                         /* 0x000000FF */
#define FSPI_XMMC_RCMD1_DUMM_SHIFT                         (8U)
#define FSPI_XMMC_RCMD1_DUMM_MASK                          (0xFU << FSPI_XMMC_RCMD1_DUMM_SHIFT)                         /* 0x00000F00 */
#define FSPI_XMMC_RCMD1_CONT_SHIFT                         (13U)
#define FSPI_XMMC_RCMD1_CONT_MASK                          (0x1U << FSPI_XMMC_RCMD1_CONT_SHIFT)                         /* 0x00002000 */
#define FSPI_XMMC_RCMD1_ADDRB_SHIFT                        (14U)
#define FSPI_XMMC_RCMD1_ADDRB_MASK                         (0x3U << FSPI_XMMC_RCMD1_ADDRB_SHIFT)                        /* 0x0000C000 */
/* DEVSIZE1 */
#define FSPI_DEVSIZE1_OFFSET                               (0x264U)
#define FSPI_DEVSIZE1_DSIZE_SHIFT                          (0U)
#define FSPI_DEVSIZE1_DSIZE_MASK                           (0x1FU << FSPI_DEVSIZE1_DSIZE_SHIFT)                         /* 0x0000001F */
/* TME1 */
#define FSPI_TME1_OFFSET                                   (0x268U)
#define FSPI_TME1_SCLK_INATM_EN_SHIFT                      (1U)
#define FSPI_TME1_SCLK_INATM_EN_MASK                       (0x1U << FSPI_TME1_SCLK_INATM_EN_SHIFT)                      /* 0x00000002 */
/******************************************SPI*******************************************/
/* CTRLR0 */
#define SPI_CTRLR0_OFFSET                                  (0x0U)
#define SPI_CTRLR0_DFS_SHIFT                               (0U)
#define SPI_CTRLR0_DFS_MASK                                (0x3U << SPI_CTRLR0_DFS_SHIFT)                               /* 0x00000003 */
#define SPI_CTRLR0_CFS_SHIFT                               (2U)
#define SPI_CTRLR0_CFS_MASK                                (0xFU << SPI_CTRLR0_CFS_SHIFT)                               /* 0x0000003C */
#define SPI_CTRLR0_SCPH_SHIFT                              (6U)
#define SPI_CTRLR0_SCPH_MASK                               (0x1U << SPI_CTRLR0_SCPH_SHIFT)                              /* 0x00000040 */
#define SPI_CTRLR0_SCPOL_SHIFT                             (7U)
#define SPI_CTRLR0_SCPOL_MASK                              (0x1U << SPI_CTRLR0_SCPOL_SHIFT)                             /* 0x00000080 */
#define SPI_CTRLR0_CSM_SHIFT                               (8U)
#define SPI_CTRLR0_CSM_MASK                                (0x3U << SPI_CTRLR0_CSM_SHIFT)                               /* 0x00000300 */
#define SPI_CTRLR0_SSD_SHIFT                               (10U)
#define SPI_CTRLR0_SSD_MASK                                (0x1U << SPI_CTRLR0_SSD_SHIFT)                               /* 0x00000400 */
#define SPI_CTRLR0_EM_SHIFT                                (11U)
#define SPI_CTRLR0_EM_MASK                                 (0x1U << SPI_CTRLR0_EM_SHIFT)                                /* 0x00000800 */
#define SPI_CTRLR0_FBM_SHIFT                               (12U)
#define SPI_CTRLR0_FBM_MASK                                (0x1U << SPI_CTRLR0_FBM_SHIFT)                               /* 0x00001000 */
#define SPI_CTRLR0_BHT_SHIFT                               (13U)
#define SPI_CTRLR0_BHT_MASK                                (0x1U << SPI_CTRLR0_BHT_SHIFT)                               /* 0x00002000 */
#define SPI_CTRLR0_RSD_SHIFT                               (14U)
#define SPI_CTRLR0_RSD_MASK                                (0x3U << SPI_CTRLR0_RSD_SHIFT)                               /* 0x0000C000 */
#define SPI_CTRLR0_FRF_SHIFT                               (16U)
#define SPI_CTRLR0_FRF_MASK                                (0x3U << SPI_CTRLR0_FRF_SHIFT)                               /* 0x00030000 */
#define SPI_CTRLR0_XFM_SHIFT                               (18U)
#define SPI_CTRLR0_XFM_MASK                                (0x3U << SPI_CTRLR0_XFM_SHIFT)                               /* 0x000C0000 */
#define SPI_CTRLR0_OPM_SHIFT                               (20U)
#define SPI_CTRLR0_OPM_MASK                                (0x1U << SPI_CTRLR0_OPM_SHIFT)                               /* 0x00100000 */
#define SPI_CTRLR0_MTM_SHIFT                               (21U)
#define SPI_CTRLR0_MTM_MASK                                (0x1U << SPI_CTRLR0_MTM_SHIFT)                               /* 0x00200000 */
#define SPI_CTRLR0_SOI_SHIFT                               (23U)
#define SPI_CTRLR0_SOI_MASK                                (0x3U << SPI_CTRLR0_SOI_SHIFT)                               /* 0x01800000 */
#define SPI_CTRLR0_LBK_SHIFT                               (25U)
#define SPI_CTRLR0_LBK_MASK                                (0x1U << SPI_CTRLR0_LBK_SHIFT)                               /* 0x02000000 */
/* CTRLR1 */
#define SPI_CTRLR1_OFFSET                                  (0x4U)
#define SPI_CTRLR1_NDM_SHIFT                               (0U)
#define SPI_CTRLR1_NDM_MASK                                (0xFFFFFFFFU << SPI_CTRLR1_NDM_SHIFT)                        /* 0xFFFFFFFF */
/* ENR */
#define SPI_ENR_OFFSET                                     (0x8U)
#define SPI_ENR_ENR_SHIFT                                  (0U)
#define SPI_ENR_ENR_MASK                                   (0x1U << SPI_ENR_ENR_SHIFT)                                  /* 0x00000001 */
/* SER */
#define SPI_SER_OFFSET                                     (0xCU)
#define SPI_SER_SER_SHIFT                                  (0U)
#define SPI_SER_SER_MASK                                   (0x3U << SPI_SER_SER_SHIFT)                                  /* 0x00000003 */
/* BAUDR */
#define SPI_BAUDR_OFFSET                                   (0x10U)
#define SPI_BAUDR_BAUDR_SHIFT                              (0U)
#define SPI_BAUDR_BAUDR_MASK                               (0xFFFFU << SPI_BAUDR_BAUDR_SHIFT)                           /* 0x0000FFFF */
/* TXFTLR */
#define SPI_TXFTLR_OFFSET                                  (0x14U)
#define SPI_TXFTLR_XFTLR_SHIFT                             (0U)
#define SPI_TXFTLR_XFTLR_MASK                              (0x3FU << SPI_TXFTLR_XFTLR_SHIFT)                            /* 0x0000003F */
/* RXFTLR */
#define SPI_RXFTLR_OFFSET                                  (0x18U)
#define SPI_RXFTLR_RXFTLR_SHIFT                            (0U)
#define SPI_RXFTLR_RXFTLR_MASK                             (0x3FU << SPI_RXFTLR_RXFTLR_SHIFT)                           /* 0x0000003F */
/* TXFLR */
#define SPI_TXFLR_OFFSET                                   (0x1CU)
#define SPI_TXFLR                                          (0x0U)
#define SPI_TXFLR_TXFLR_SHIFT                              (0U)
#define SPI_TXFLR_TXFLR_MASK                               (0x7FU << SPI_TXFLR_TXFLR_SHIFT)                             /* 0x0000007F */
/* RXFLR */
#define SPI_RXFLR_OFFSET                                   (0x20U)
#define SPI_RXFLR                                          (0x0U)
#define SPI_RXFLR_RXFLR_SHIFT                              (0U)
#define SPI_RXFLR_RXFLR_MASK                               (0x7FU << SPI_RXFLR_RXFLR_SHIFT)                             /* 0x0000007F */
/* SR */
#define SPI_SR_OFFSET                                      (0x24U)
#define SPI_SR                                             (0x4CU)
#define SPI_SR_BSF_SHIFT                                   (0U)
#define SPI_SR_BSF_MASK                                    (0x1U << SPI_SR_BSF_SHIFT)                                   /* 0x00000001 */
#define SPI_SR_TFF_SHIFT                                   (1U)
#define SPI_SR_TFF_MASK                                    (0x1U << SPI_SR_TFF_SHIFT)                                   /* 0x00000002 */
#define SPI_SR_TFE_SHIFT                                   (2U)
#define SPI_SR_TFE_MASK                                    (0x1U << SPI_SR_TFE_SHIFT)                                   /* 0x00000004 */
#define SPI_SR_RFE_SHIFT                                   (3U)
#define SPI_SR_RFE_MASK                                    (0x1U << SPI_SR_RFE_SHIFT)                                   /* 0x00000008 */
#define SPI_SR_RFF_SHIFT                                   (4U)
#define SPI_SR_RFF_MASK                                    (0x1U << SPI_SR_RFF_SHIFT)                                   /* 0x00000010 */
#define SPI_SR_STB_SHIFT                                   (5U)
#define SPI_SR_STB_MASK                                    (0x1U << SPI_SR_STB_SHIFT)                                   /* 0x00000020 */
#define SPI_SR_SSI_SHIFT                                   (6U)
#define SPI_SR_SSI_MASK                                    (0x1U << SPI_SR_SSI_SHIFT)                                   /* 0x00000040 */
/* IPR */
#define SPI_IPR_OFFSET                                     (0x28U)
#define SPI_IPR_IPR_SHIFT                                  (0U)
#define SPI_IPR_IPR_MASK                                   (0x1U << SPI_IPR_IPR_SHIFT)                                  /* 0x00000001 */
/* IMR */
#define SPI_IMR_OFFSET                                     (0x2CU)
#define SPI_IMR_TFEIM_SHIFT                                (0U)
#define SPI_IMR_TFEIM_MASK                                 (0x1U << SPI_IMR_TFEIM_SHIFT)                                /* 0x00000001 */
#define SPI_IMR_TFOIM_SHIFT                                (1U)
#define SPI_IMR_TFOIM_MASK                                 (0x1U << SPI_IMR_TFOIM_SHIFT)                                /* 0x00000002 */
#define SPI_IMR_RFUIM_SHIFT                                (2U)
#define SPI_IMR_RFUIM_MASK                                 (0x1U << SPI_IMR_RFUIM_SHIFT)                                /* 0x00000004 */
#define SPI_IMR_RFOIM_SHIFT                                (3U)
#define SPI_IMR_RFOIM_MASK                                 (0x1U << SPI_IMR_RFOIM_SHIFT)                                /* 0x00000008 */
#define SPI_IMR_RFFIM_SHIFT                                (4U)
#define SPI_IMR_RFFIM_MASK                                 (0x1U << SPI_IMR_RFFIM_SHIFT)                                /* 0x00000010 */
#define SPI_IMR_TOIM_SHIFT                                 (5U)
#define SPI_IMR_TOIM_MASK                                  (0x1U << SPI_IMR_TOIM_SHIFT)                                 /* 0x00000020 */
#define SPI_IMR_SSPIM_SHIFT                                (6U)
#define SPI_IMR_SSPIM_MASK                                 (0x1U << SPI_IMR_SSPIM_SHIFT)                                /* 0x00000040 */
#define SPI_IMR_TXFIM_SHIFT                                (7U)
#define SPI_IMR_TXFIM_MASK                                 (0x1U << SPI_IMR_TXFIM_SHIFT)                                /* 0x00000080 */
/* ISR */
#define SPI_ISR_OFFSET                                     (0x30U)
#define SPI_ISR_TFEIS_SHIFT                                (0U)
#define SPI_ISR_TFEIS_MASK                                 (0x1U << SPI_ISR_TFEIS_SHIFT)                                /* 0x00000001 */
#define SPI_ISR_TFOIS_SHIFT                                (1U)
#define SPI_ISR_TFOIS_MASK                                 (0x1U << SPI_ISR_TFOIS_SHIFT)                                /* 0x00000002 */
#define SPI_ISR_RFUIS_SHIFT                                (2U)
#define SPI_ISR_RFUIS_MASK                                 (0x1U << SPI_ISR_RFUIS_SHIFT)                                /* 0x00000004 */
#define SPI_ISR_RFOIS_SHIFT                                (3U)
#define SPI_ISR_RFOIS_MASK                                 (0x1U << SPI_ISR_RFOIS_SHIFT)                                /* 0x00000008 */
#define SPI_ISR_RFFIS_SHIFT                                (4U)
#define SPI_ISR_RFFIS_MASK                                 (0x1U << SPI_ISR_RFFIS_SHIFT)                                /* 0x00000010 */
#define SPI_ISR_TOIS_SHIFT                                 (5U)
#define SPI_ISR_TOIS_MASK                                  (0x1U << SPI_ISR_TOIS_SHIFT)                                 /* 0x00000020 */
#define SPI_ISR_SSPIS_SHIFT                                (6U)
#define SPI_ISR_SSPIS_MASK                                 (0x1U << SPI_ISR_SSPIS_SHIFT)                                /* 0x00000040 */
#define SPI_ISR_TXFIS_SHIFT                                (7U)
#define SPI_ISR_TXFIS_MASK                                 (0x1U << SPI_ISR_TXFIS_SHIFT)                                /* 0x00000080 */
/* RISR */
#define SPI_RISR_OFFSET                                    (0x34U)
#define SPI_RISR_TFERIS_SHIFT                              (0U)
#define SPI_RISR_TFERIS_MASK                               (0x1U << SPI_RISR_TFERIS_SHIFT)                              /* 0x00000001 */
#define SPI_RISR_TFORIS_SHIFT                              (1U)
#define SPI_RISR_TFORIS_MASK                               (0x1U << SPI_RISR_TFORIS_SHIFT)                              /* 0x00000002 */
#define SPI_RISR_RFURIS_SHIFT                              (2U)
#define SPI_RISR_RFURIS_MASK                               (0x1U << SPI_RISR_RFURIS_SHIFT)                              /* 0x00000004 */
#define SPI_RISR_RFORIS_SHIFT                              (3U)
#define SPI_RISR_RFORIS_MASK                               (0x1U << SPI_RISR_RFORIS_SHIFT)                              /* 0x00000008 */
#define SPI_RISR_RFFRIS_SHIFT                              (4U)
#define SPI_RISR_RFFRIS_MASK                               (0x1U << SPI_RISR_RFFRIS_SHIFT)                              /* 0x00000010 */
#define SPI_RISR_TORIS_SHIFT                               (5U)
#define SPI_RISR_TORIS_MASK                                (0x1U << SPI_RISR_TORIS_SHIFT)                               /* 0x00000020 */
#define SPI_RISR_SSPRIS_SHIFT                              (6U)
#define SPI_RISR_SSPRIS_MASK                               (0x1U << SPI_RISR_SSPRIS_SHIFT)                              /* 0x00000040 */
#define SPI_RISR_TXFRIS_SHIFT                              (7U)
#define SPI_RISR_TXFRIS_MASK                               (0x1U << SPI_RISR_TXFRIS_SHIFT)                              /* 0x00000080 */
/* ICR */
#define SPI_ICR_OFFSET                                     (0x38U)
#define SPI_ICR_CCI_SHIFT                                  (0U)
#define SPI_ICR_CCI_MASK                                   (0x1U << SPI_ICR_CCI_SHIFT)                                  /* 0x00000001 */
#define SPI_ICR_CRFUI_SHIFT                                (1U)
#define SPI_ICR_CRFUI_MASK                                 (0x1U << SPI_ICR_CRFUI_SHIFT)                                /* 0x00000002 */
#define SPI_ICR_CRFOI_SHIFT                                (2U)
#define SPI_ICR_CRFOI_MASK                                 (0x1U << SPI_ICR_CRFOI_SHIFT)                                /* 0x00000004 */
#define SPI_ICR_CTFOI_SHIFT                                (3U)
#define SPI_ICR_CTFOI_MASK                                 (0x1U << SPI_ICR_CTFOI_SHIFT)                                /* 0x00000008 */
#define SPI_ICR_CTOI_SHIFT                                 (4U)
#define SPI_ICR_CTOI_MASK                                  (0x1U << SPI_ICR_CTOI_SHIFT)                                 /* 0x00000010 */
#define SPI_ICR_CSSPI_SHIFT                                (5U)
#define SPI_ICR_CSSPI_MASK                                 (0x1U << SPI_ICR_CSSPI_SHIFT)                                /* 0x00000020 */
#define SPI_ICR_CTXFI_SHIFT                                (6U)
#define SPI_ICR_CTXFI_MASK                                 (0x1U << SPI_ICR_CTXFI_SHIFT)                                /* 0x00000040 */
/* DMACR */
#define SPI_DMACR_OFFSET                                   (0x3CU)
#define SPI_DMACR_RDE_SHIFT                                (0U)
#define SPI_DMACR_RDE_MASK                                 (0x1U << SPI_DMACR_RDE_SHIFT)                                /* 0x00000001 */
#define SPI_DMACR_TDE_SHIFT                                (1U)
#define SPI_DMACR_TDE_MASK                                 (0x1U << SPI_DMACR_TDE_SHIFT)                                /* 0x00000002 */
/* DMATDLR */
#define SPI_DMATDLR_OFFSET                                 (0x40U)
#define SPI_DMATDLR_TDL_SHIFT                              (0U)
#define SPI_DMATDLR_TDL_MASK                               (0x3FU << SPI_DMATDLR_TDL_SHIFT)                             /* 0x0000003F */
/* DMARDLR */
#define SPI_DMARDLR_OFFSET                                 (0x44U)
#define SPI_DMARDLR_RDL_SHIFT                              (0U)
#define SPI_DMARDLR_RDL_MASK                               (0x3FU << SPI_DMARDLR_RDL_SHIFT)                             /* 0x0000003F */
/* TIMEOUT */
#define SPI_TIMEOUT_OFFSET                                 (0x4CU)
#define SPI_TIMEOUT_TOV_SHIFT                              (0U)
#define SPI_TIMEOUT_TOV_MASK                               (0xFFFFU << SPI_TIMEOUT_TOV_SHIFT)                           /* 0x0000FFFF */
#define SPI_TIMEOUT_TOE_SHIFT                              (16U)
#define SPI_TIMEOUT_TOE_MASK                               (0x1U << SPI_TIMEOUT_TOE_SHIFT)                              /* 0x00010000 */
/* BYPASS */
#define SPI_BYPASS_OFFSET                                  (0x50U)
#define SPI_BYPASS_BYEN_SHIFT                              (0U)
#define SPI_BYPASS_BYEN_MASK                               (0x1U << SPI_BYPASS_BYEN_SHIFT)                              /* 0x00000001 */
#define SPI_BYPASS_FBM_SHIFT                               (1U)
#define SPI_BYPASS_FBM_MASK                                (0x1U << SPI_BYPASS_FBM_SHIFT)                               /* 0x00000002 */
#define SPI_BYPASS_END_SHIFT                               (2U)
#define SPI_BYPASS_END_MASK                                (0x1U << SPI_BYPASS_END_SHIFT)                               /* 0x00000004 */
#define SPI_BYPASS_RXCP_SHIFT                              (3U)
#define SPI_BYPASS_RXCP_MASK                               (0x1U << SPI_BYPASS_RXCP_SHIFT)                              /* 0x00000008 */
#define SPI_BYPASS_TXCP_SHIFT                              (4U)
#define SPI_BYPASS_TXCP_MASK                               (0x1U << SPI_BYPASS_TXCP_SHIFT)                              /* 0x00000010 */
/* TXDR */
#define SPI_TXDR_OFFSET                                    (0x400U)
#define SPI_TXDR_TXDR_SHIFT                                (0U)
#define SPI_TXDR_TXDR_MASK                                 (0xFFFFU << SPI_TXDR_TXDR_SHIFT)                             /* 0x0000FFFF */
/* RXDR */
#define SPI_RXDR_OFFSET                                    (0x800U)
#define SPI_RXDR                                           (0x0U)
#define SPI_RXDR_RXDR_SHIFT                                (0U)
#define SPI_RXDR_RXDR_MASK                                 (0xFFFFU << SPI_RXDR_RXDR_SHIFT)                             /* 0x0000FFFF */
/******************************************MBOX******************************************/
/* A2B_INTEN */
#define MBOX_A2B_INTEN_OFFSET                              (0x0U)
#define MBOX_A2B_INTEN_INT0_SHIFT                          (0U)
#define MBOX_A2B_INTEN_INT0_MASK                           (0x1U << MBOX_A2B_INTEN_INT0_SHIFT)                          /* 0x00000001 */
#define MBOX_A2B_INTEN_INT1_SHIFT                          (1U)
#define MBOX_A2B_INTEN_INT1_MASK                           (0x1U << MBOX_A2B_INTEN_INT1_SHIFT)                          /* 0x00000002 */
#define MBOX_A2B_INTEN_INT2_SHIFT                          (2U)
#define MBOX_A2B_INTEN_INT2_MASK                           (0x1U << MBOX_A2B_INTEN_INT2_SHIFT)                          /* 0x00000004 */
#define MBOX_A2B_INTEN_INT3_SHIFT                          (3U)
#define MBOX_A2B_INTEN_INT3_MASK                           (0x1U << MBOX_A2B_INTEN_INT3_SHIFT)                          /* 0x00000008 */
/* A2B_STATUS */
#define MBOX_A2B_STATUS_OFFSET                             (0x4U)
#define MBOX_A2B_STATUS_INT0_SHIFT                         (0U)
#define MBOX_A2B_STATUS_INT0_MASK                          (0x1U << MBOX_A2B_STATUS_INT0_SHIFT)                         /* 0x00000001 */
#define MBOX_A2B_STATUS_INT1_SHIFT                         (1U)
#define MBOX_A2B_STATUS_INT1_MASK                          (0x1U << MBOX_A2B_STATUS_INT1_SHIFT)                         /* 0x00000002 */
#define MBOX_A2B_STATUS_INT2_SHIFT                         (2U)
#define MBOX_A2B_STATUS_INT2_MASK                          (0x1U << MBOX_A2B_STATUS_INT2_SHIFT)                         /* 0x00000004 */
#define MBOX_A2B_STATUS_INT3_SHIFT                         (3U)
#define MBOX_A2B_STATUS_INT3_MASK                          (0x1U << MBOX_A2B_STATUS_INT3_SHIFT)                         /* 0x00000008 */
/* A2B_CMD_0 */
#define MBOX_A2B_CMD_0_OFFSET                              (0x8U)
#define MBOX_A2B_CMD_0_COMMAND_SHIFT                       (0U)
#define MBOX_A2B_CMD_0_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_A2B_CMD_0_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* A2B_DAT_0 */
#define MBOX_A2B_DAT_0_OFFSET                              (0xCU)
#define MBOX_A2B_DAT_0_DATA_SHIFT                          (0U)
#define MBOX_A2B_DAT_0_DATA_MASK                           (0xFFFFFFFFU << MBOX_A2B_DAT_0_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* A2B_CMD_1 */
#define MBOX_A2B_CMD_1_OFFSET                              (0x10U)
#define MBOX_A2B_CMD_1_COMMAND_SHIFT                       (0U)
#define MBOX_A2B_CMD_1_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_A2B_CMD_1_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* A2B_DAT_1 */
#define MBOX_A2B_DAT_1_OFFSET                              (0x14U)
#define MBOX_A2B_DAT_1_DATA_SHIFT                          (0U)
#define MBOX_A2B_DAT_1_DATA_MASK                           (0xFFFFFFFFU << MBOX_A2B_DAT_1_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* A2B_CMD_2 */
#define MBOX_A2B_CMD_2_OFFSET                              (0x18U)
#define MBOX_A2B_CMD_2_COMMAND_SHIFT                       (0U)
#define MBOX_A2B_CMD_2_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_A2B_CMD_2_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* A2B_DAT_2 */
#define MBOX_A2B_DAT_2_OFFSET                              (0x1CU)
#define MBOX_A2B_DAT_2_DATA_SHIFT                          (0U)
#define MBOX_A2B_DAT_2_DATA_MASK                           (0xFFFFFFFFU << MBOX_A2B_DAT_2_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* A2B_CMD_3 */
#define MBOX_A2B_CMD_3_OFFSET                              (0x20U)
#define MBOX_A2B_CMD_3_COMMAND_SHIFT                       (0U)
#define MBOX_A2B_CMD_3_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_A2B_CMD_3_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* A2B_DAT_3 */
#define MBOX_A2B_DAT_3_OFFSET                              (0x24U)
#define MBOX_A2B_DAT_3_DATA_SHIFT                          (0U)
#define MBOX_A2B_DAT_3_DATA_MASK                           (0xFFFFFFFFU << MBOX_A2B_DAT_3_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* B2A_INTEN */
#define MBOX_B2A_INTEN_OFFSET                              (0x28U)
#define MBOX_B2A_INTEN_INT0_SHIFT                          (0U)
#define MBOX_B2A_INTEN_INT0_MASK                           (0x1U << MBOX_B2A_INTEN_INT0_SHIFT)                          /* 0x00000001 */
#define MBOX_B2A_INTEN_INT1_SHIFT                          (1U)
#define MBOX_B2A_INTEN_INT1_MASK                           (0x1U << MBOX_B2A_INTEN_INT1_SHIFT)                          /* 0x00000002 */
#define MBOX_B2A_INTEN_INT2_SHIFT                          (2U)
#define MBOX_B2A_INTEN_INT2_MASK                           (0x1U << MBOX_B2A_INTEN_INT2_SHIFT)                          /* 0x00000004 */
#define MBOX_B2A_INTEN_INT3_SHIFT                          (3U)
#define MBOX_B2A_INTEN_INT3_MASK                           (0x1U << MBOX_B2A_INTEN_INT3_SHIFT)                          /* 0x00000008 */
/* B2A_STATUS */
#define MBOX_B2A_STATUS_OFFSET                             (0x2CU)
#define MBOX_B2A_STATUS_INT0_SHIFT                         (0U)
#define MBOX_B2A_STATUS_INT0_MASK                          (0x1U << MBOX_B2A_STATUS_INT0_SHIFT)                         /* 0x00000001 */
#define MBOX_B2A_STATUS_INT1_SHIFT                         (1U)
#define MBOX_B2A_STATUS_INT1_MASK                          (0x1U << MBOX_B2A_STATUS_INT1_SHIFT)                         /* 0x00000002 */
#define MBOX_B2A_STATUS_INT2_SHIFT                         (2U)
#define MBOX_B2A_STATUS_INT2_MASK                          (0x1U << MBOX_B2A_STATUS_INT2_SHIFT)                         /* 0x00000004 */
#define MBOX_B2A_STATUS_INT3_SHIFT                         (3U)
#define MBOX_B2A_STATUS_INT3_MASK                          (0x1U << MBOX_B2A_STATUS_INT3_SHIFT)                         /* 0x00000008 */
/* B2A_CMD_0 */
#define MBOX_B2A_CMD_0_OFFSET                              (0x30U)
#define MBOX_B2A_CMD_0_COMMAND_SHIFT                       (0U)
#define MBOX_B2A_CMD_0_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_B2A_CMD_0_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* B2A_DAT_0 */
#define MBOX_B2A_DAT_0_OFFSET                              (0x34U)
#define MBOX_B2A_DAT_0_DATA_SHIFT                          (0U)
#define MBOX_B2A_DAT_0_DATA_MASK                           (0xFFFFFFFFU << MBOX_B2A_DAT_0_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* B2A_CMD_1 */
#define MBOX_B2A_CMD_1_OFFSET                              (0x38U)
#define MBOX_B2A_CMD_1_COMMAND_SHIFT                       (0U)
#define MBOX_B2A_CMD_1_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_B2A_CMD_1_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* B2A_DAT_1 */
#define MBOX_B2A_DAT_1_OFFSET                              (0x3CU)
#define MBOX_B2A_DAT_1_DATA_SHIFT                          (0U)
#define MBOX_B2A_DAT_1_DATA_MASK                           (0xFFFFFFFFU << MBOX_B2A_DAT_1_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* B2A_CMD_2 */
#define MBOX_B2A_CMD_2_OFFSET                              (0x40U)
#define MBOX_B2A_CMD_2_COMMAND_SHIFT                       (0U)
#define MBOX_B2A_CMD_2_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_B2A_CMD_2_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* B2A_DAT_2 */
#define MBOX_B2A_DAT_2_OFFSET                              (0x44U)
#define MBOX_B2A_DAT_2_DATA_SHIFT                          (0U)
#define MBOX_B2A_DAT_2_DATA_MASK                           (0xFFFFFFFFU << MBOX_B2A_DAT_2_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* B2A_CMD_3 */
#define MBOX_B2A_CMD_3_OFFSET                              (0x48U)
#define MBOX_B2A_CMD_3_COMMAND_SHIFT                       (0U)
#define MBOX_B2A_CMD_3_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_B2A_CMD_3_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* B2A_DAT_3 */
#define MBOX_B2A_DAT_3_OFFSET                              (0x4CU)
#define MBOX_B2A_DAT_3_DATA_SHIFT                          (0U)
#define MBOX_B2A_DAT_3_DATA_MASK                           (0xFFFFFFFFU << MBOX_B2A_DAT_3_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* ATOMIC_LOCK_00 */
#define MBOX_ATOMIC_LOCK_00_OFFSET                         (0x100U)
#define MBOX_ATOMIC_LOCK_00_ATOMIC_LOCK_00_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_00_ATOMIC_LOCK_00_MASK            (0x1U << MBOX_ATOMIC_LOCK_00_ATOMIC_LOCK_00_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_01 */
#define MBOX_ATOMIC_LOCK_01_OFFSET                         (0x104U)
#define MBOX_ATOMIC_LOCK_01_ATOMIC_LOCK_01_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_01_ATOMIC_LOCK_01_MASK            (0x1U << MBOX_ATOMIC_LOCK_01_ATOMIC_LOCK_01_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_02 */
#define MBOX_ATOMIC_LOCK_02_OFFSET                         (0x108U)
#define MBOX_ATOMIC_LOCK_02_ATOMIC_LOCK_02_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_02_ATOMIC_LOCK_02_MASK            (0x1U << MBOX_ATOMIC_LOCK_02_ATOMIC_LOCK_02_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_03 */
#define MBOX_ATOMIC_LOCK_03_OFFSET                         (0x10CU)
#define MBOX_ATOMIC_LOCK_03_ATOMIC_LOCK_03_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_03_ATOMIC_LOCK_03_MASK            (0x1U << MBOX_ATOMIC_LOCK_03_ATOMIC_LOCK_03_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_04 */
#define MBOX_ATOMIC_LOCK_04_OFFSET                         (0x110U)
#define MBOX_ATOMIC_LOCK_04_ATOMIC_LOCK_04_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_04_ATOMIC_LOCK_04_MASK            (0x1U << MBOX_ATOMIC_LOCK_04_ATOMIC_LOCK_04_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_05 */
#define MBOX_ATOMIC_LOCK_05_OFFSET                         (0x114U)
#define MBOX_ATOMIC_LOCK_05_ATOMIC_LOCK_05_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_05_ATOMIC_LOCK_05_MASK            (0x1U << MBOX_ATOMIC_LOCK_05_ATOMIC_LOCK_05_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_06 */
#define MBOX_ATOMIC_LOCK_06_OFFSET                         (0x118U)
#define MBOX_ATOMIC_LOCK_06_ATOMIC_LOCK_06_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_06_ATOMIC_LOCK_06_MASK            (0x1U << MBOX_ATOMIC_LOCK_06_ATOMIC_LOCK_06_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_07 */
#define MBOX_ATOMIC_LOCK_07_OFFSET                         (0x11CU)
#define MBOX_ATOMIC_LOCK_07_ATOMIC_LOCK_07_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_07_ATOMIC_LOCK_07_MASK            (0x1U << MBOX_ATOMIC_LOCK_07_ATOMIC_LOCK_07_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_08 */
#define MBOX_ATOMIC_LOCK_08_OFFSET                         (0x120U)
#define MBOX_ATOMIC_LOCK_08_ATOMIC_LOCK_08_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_08_ATOMIC_LOCK_08_MASK            (0x1U << MBOX_ATOMIC_LOCK_08_ATOMIC_LOCK_08_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_09 */
#define MBOX_ATOMIC_LOCK_09_OFFSET                         (0x124U)
#define MBOX_ATOMIC_LOCK_09_ATOMIC_LOCK_09_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_09_ATOMIC_LOCK_09_MASK            (0x1U << MBOX_ATOMIC_LOCK_09_ATOMIC_LOCK_09_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_10 */
#define MBOX_ATOMIC_LOCK_10_OFFSET                         (0x128U)
#define MBOX_ATOMIC_LOCK_10_ATOMIC_LOCK_10_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_10_ATOMIC_LOCK_10_MASK            (0x1U << MBOX_ATOMIC_LOCK_10_ATOMIC_LOCK_10_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_11 */
#define MBOX_ATOMIC_LOCK_11_OFFSET                         (0x12CU)
#define MBOX_ATOMIC_LOCK_11_ATOMIC_LOCK_11_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_11_ATOMIC_LOCK_11_MASK            (0x1U << MBOX_ATOMIC_LOCK_11_ATOMIC_LOCK_11_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_12 */
#define MBOX_ATOMIC_LOCK_12_OFFSET                         (0x130U)
#define MBOX_ATOMIC_LOCK_12_ATOMIC_LOCK_12_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_12_ATOMIC_LOCK_12_MASK            (0x1U << MBOX_ATOMIC_LOCK_12_ATOMIC_LOCK_12_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_13 */
#define MBOX_ATOMIC_LOCK_13_OFFSET                         (0x134U)
#define MBOX_ATOMIC_LOCK_13_ATOMIC_LOCK_13_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_13_ATOMIC_LOCK_13_MASK            (0x1U << MBOX_ATOMIC_LOCK_13_ATOMIC_LOCK_13_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_14 */
#define MBOX_ATOMIC_LOCK_14_OFFSET                         (0x138U)
#define MBOX_ATOMIC_LOCK_14_ATOMIC_LOCK_14_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_14_ATOMIC_LOCK_14_MASK            (0x1U << MBOX_ATOMIC_LOCK_14_ATOMIC_LOCK_14_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_15 */
#define MBOX_ATOMIC_LOCK_15_OFFSET                         (0x13CU)
#define MBOX_ATOMIC_LOCK_15_ATOMIC_LOCK_15_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_15_ATOMIC_LOCK_15_MASK            (0x1U << MBOX_ATOMIC_LOCK_15_ATOMIC_LOCK_15_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_16 */
#define MBOX_ATOMIC_LOCK_16_OFFSET                         (0x140U)
#define MBOX_ATOMIC_LOCK_16_ATOMIC_LOCK_16_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_16_ATOMIC_LOCK_16_MASK            (0x1U << MBOX_ATOMIC_LOCK_16_ATOMIC_LOCK_16_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_17 */
#define MBOX_ATOMIC_LOCK_17_OFFSET                         (0x144U)
#define MBOX_ATOMIC_LOCK_17_ATOMIC_LOCK_17_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_17_ATOMIC_LOCK_17_MASK            (0x1U << MBOX_ATOMIC_LOCK_17_ATOMIC_LOCK_17_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_18 */
#define MBOX_ATOMIC_LOCK_18_OFFSET                         (0x148U)
#define MBOX_ATOMIC_LOCK_18_ATOMIC_LOCK_18_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_18_ATOMIC_LOCK_18_MASK            (0x1U << MBOX_ATOMIC_LOCK_18_ATOMIC_LOCK_18_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_19 */
#define MBOX_ATOMIC_LOCK_19_OFFSET                         (0x14CU)
#define MBOX_ATOMIC_LOCK_19_ATOMIC_LOCK_19_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_19_ATOMIC_LOCK_19_MASK            (0x1U << MBOX_ATOMIC_LOCK_19_ATOMIC_LOCK_19_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_20 */
#define MBOX_ATOMIC_LOCK_20_OFFSET                         (0x150U)
#define MBOX_ATOMIC_LOCK_20_ATOMIC_LOCK_20_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_20_ATOMIC_LOCK_20_MASK            (0x1U << MBOX_ATOMIC_LOCK_20_ATOMIC_LOCK_20_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_21 */
#define MBOX_ATOMIC_LOCK_21_OFFSET                         (0x154U)
#define MBOX_ATOMIC_LOCK_21_ATOMIC_LOCK_21_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_21_ATOMIC_LOCK_21_MASK            (0x1U << MBOX_ATOMIC_LOCK_21_ATOMIC_LOCK_21_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_22 */
#define MBOX_ATOMIC_LOCK_22_OFFSET                         (0x158U)
#define MBOX_ATOMIC_LOCK_22_ATOMIC_LOCK_22_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_22_ATOMIC_LOCK_22_MASK            (0x1U << MBOX_ATOMIC_LOCK_22_ATOMIC_LOCK_22_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_23 */
#define MBOX_ATOMIC_LOCK_23_OFFSET                         (0x15CU)
#define MBOX_ATOMIC_LOCK_23_ATOMIC_LOCK_23_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_23_ATOMIC_LOCK_23_MASK            (0x1U << MBOX_ATOMIC_LOCK_23_ATOMIC_LOCK_23_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_24 */
#define MBOX_ATOMIC_LOCK_24_OFFSET                         (0x160U)
#define MBOX_ATOMIC_LOCK_24_ATOMIC_LOCK_24_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_24_ATOMIC_LOCK_24_MASK            (0x1U << MBOX_ATOMIC_LOCK_24_ATOMIC_LOCK_24_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_25 */
#define MBOX_ATOMIC_LOCK_25_OFFSET                         (0x164U)
#define MBOX_ATOMIC_LOCK_25_ATOMIC_LOCK_25_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_25_ATOMIC_LOCK_25_MASK            (0x1U << MBOX_ATOMIC_LOCK_25_ATOMIC_LOCK_25_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_26 */
#define MBOX_ATOMIC_LOCK_26_OFFSET                         (0x168U)
#define MBOX_ATOMIC_LOCK_26_ATOMIC_LOCK_26_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_26_ATOMIC_LOCK_26_MASK            (0x1U << MBOX_ATOMIC_LOCK_26_ATOMIC_LOCK_26_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_27 */
#define MBOX_ATOMIC_LOCK_27_OFFSET                         (0x16CU)
#define MBOX_ATOMIC_LOCK_27_ATOMIC_LOCK_27_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_27_ATOMIC_LOCK_27_MASK            (0x1U << MBOX_ATOMIC_LOCK_27_ATOMIC_LOCK_27_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_28 */
#define MBOX_ATOMIC_LOCK_28_OFFSET                         (0x170U)
#define MBOX_ATOMIC_LOCK_28_ATOMIC_LOCK_28_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_28_ATOMIC_LOCK_28_MASK            (0x1U << MBOX_ATOMIC_LOCK_28_ATOMIC_LOCK_28_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_29 */
#define MBOX_ATOMIC_LOCK_29_OFFSET                         (0x174U)
#define MBOX_ATOMIC_LOCK_29_ATOMIC_LOCK_29_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_29_ATOMIC_LOCK_29_MASK            (0x1U << MBOX_ATOMIC_LOCK_29_ATOMIC_LOCK_29_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_30 */
#define MBOX_ATOMIC_LOCK_30_OFFSET                         (0x178U)
#define MBOX_ATOMIC_LOCK_30_ATOMIC_LOCK_30_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_30_ATOMIC_LOCK_30_MASK            (0x1U << MBOX_ATOMIC_LOCK_30_ATOMIC_LOCK_30_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_31 */
#define MBOX_ATOMIC_LOCK_31_OFFSET                         (0x17CU)
#define MBOX_ATOMIC_LOCK_31_ATOMIC_LOCK_31_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_31_ATOMIC_LOCK_31_MASK            (0x1U << MBOX_ATOMIC_LOCK_31_ATOMIC_LOCK_31_SHIFT)           /* 0x00000001 */
/*****************************************INTMUX*****************************************/

/********Name=SOFTRST_CON01,Offset=0xA04********/
#define SRST_A_TOP_NIU 19U
#define SRST_P_TOP_NIU 20U
#define SRST_A_LOW_TOP_NIU 21U
#define SRST_P_CSIPHY0 22U
#define SRST_SCAN_CSIPHY0 23U
#define SRST_P_CSIPHY1 24U
#define SRST_SCAN_CSIPHY1 25U
#define SRST_A_TOP_M500_NIU 31U
/********Name=SOFTRST_CON02,Offset=0xA08********/
#define SRST_A_TOP_M400_NIU 32U
#define SRST_A_TOP_S200_NIU 33U
#define SRST_A_TOP_S400_NIU 34U
#define SRST_A_TOP_M300_NIU 35U
#define SRST_USBDP_COMBO_PHY0_INIT 40U
#define SRST_USBDP_COMBO_PHY0_CMN 41U
#define SRST_USBDP_COMBO_PHY0_LANE 42U
#define SRST_USBDP_COMBO_PHY0_PCS 43U
#define SRST_USBDP_COMBO_PHY1_INIT 47U
/********Name=SOFTRST_CON03,Offset=0xA0C********/
#define SRST_USBDP_COMBO_PHY1_CMN 48U
#define SRST_USBDP_COMBO_PHY1_LANE 49U
#define SRST_USBDP_COMBO_PHY1_PCS 50U
#define SRST_SCAN_DCPHY0 59U
#define SRST_P_MIPI_DCPHY0 62U
#define SRST_P_MIPI_DCPHY0_GRF 63U
/********Name=SOFTRST_CON04,Offset=0xA10********/
#define SRST_SCAN_DCPHY1 64U
#define SRST_P_MIPI_DCPHY1 67U
#define SRST_P_MIPI_DCPHY1_GRF 68U
#define SRST_P_APB2ASB_SLV_CDPHY 69U
#define SRST_P_APB2ASB_SLV_CSIPHY 70U
#define SRST_P_APB2ASB_SLV_VCCIO3_5 71U
#define SRST_P_APB2ASB_SLV_VCCIO6 72U
#define SRST_P_APB2ASB_SLV_EMMCIO 73U
#define SRST_P_APB2ASB_SLV_IOC_TOP 74U
#define SRST_P_APB2ASB_SLV_IOC_RIGHT 75U
/********Name=SOFTRST_CON05,Offset=0xA14********/
#define SRST_P_CRU 80U
#define SRST_NPOR_PAD 81U
#define SRST_NPOR_TM_DECODER 82U
#define SRST_A_CHANNEL_SECURE2VO1USB 87U
#define SRST_A_CHANNEL_SECURE2CENTER 88U
#define SRST_H_CHANNEL_SECURE2VO1USB 94U
#define SRST_H_CHANNEL_SECURE2CENTER 95U
/********Name=SOFTRST_CON06,Offset=0xA18********/
#define SRST_P_CHANNEL_SECURE2VO1USB 96U
#define SRST_P_CHANNEL_SECURE2CENTER 97U
/********Name=SOFTRST_CON07,Offset=0xA1C********/
#define SRST_H_AUDIO_NIU 114U
#define SRST_P_AUDIO_NIU 115U
#define SRST_H_I2S0_8CH 116U
#define SRST_M_I2S0_8CH_TX 119U
#define SRST_M_I2S0_8CH_RX 122U
#define SRST_P_ACDCDIG 123U
#define SRST_H_I2S2_2CH 124U
#define SRST_H_I2S3_2CH 125U
/********Name=SOFTRST_CON08,Offset=0xA20********/
#define SRST_M_I2S2_2CH 128U
#define SRST_M_I2S3_2CH 131U
#define SRST_DAC_ACDCDIG 132U
#define SRST_H_SPDIF0 142U
/********Name=SOFTRST_CON09,Offset=0xA24********/
#define SRST_M_SPDIF0 145U
#define SRST_H_SPDIF1 146U
#define SRST_M_SPDIF1 149U
#define SRST_H_PDM1 150U
#define SRST_PDM1 151U
/********Name=SOFTRST_CON10,Offset=0xA28********/
#define SRST_A_BUS_NIU 161U
#define SRST_P_BUS_NIU 162U
#define SRST_A_GIC 163U
#define SRST_A_GIC_DBG 164U
#define SRST_A_DMAC0 165U
#define SRST_A_DMAC1 166U
#define SRST_A_DMAC2 167U
#define SRST_P_I2C1 168U
#define SRST_P_I2C2 169U
#define SRST_P_I2C3 170U
#define SRST_P_I2C4 171U
#define SRST_P_I2C5 172U
#define SRST_P_I2C6 173U
#define SRST_P_I2C7 174U
#define SRST_P_I2C8 175U
/********Name=SOFTRST_CON11,Offset=0xA2C********/
#define SRST_I2C1 176U
#define SRST_I2C2 177U
#define SRST_I2C3 178U
#define SRST_I2C4 179U
#define SRST_I2C5 180U
#define SRST_I2C6 181U
#define SRST_I2C7 182U
#define SRST_I2C8 183U
#define SRST_P_CAN0 184U
#define SRST_CAN0 185U
#define SRST_P_CAN1 186U
#define SRST_CAN1 187U
#define SRST_P_CAN2 188U
#define SRST_CAN2 189U
#define SRST_P_SARADC 190U
/********Name=SOFTRST_CON12,Offset=0xA30********/
#define SRST_P_TSADC 192U
#define SRST_TSADC 193U
#define SRST_P_UART1 194U
#define SRST_P_UART2 195U
#define SRST_P_UART3 196U
#define SRST_P_UART4 197U
#define SRST_P_UART5 198U
#define SRST_P_UART6 199U
#define SRST_P_UART7 200U
#define SRST_P_UART8 201U
#define SRST_P_UART9 202U
#define SRST_S_UART1 205U
/********Name=SOFTRST_CON13,Offset=0xA34********/
#define SRST_S_UART2 208U
#define SRST_S_UART3 211U
#define SRST_S_UART4 214U
#define SRST_S_UART5 217U
#define SRST_S_UART6 220U
#define SRST_S_UART7 223U
/********Name=SOFTRST_CON14,Offset=0xA38********/
#define SRST_S_UART8 226U
#define SRST_S_UART9 229U
#define SRST_P_SPI0 230U
#define SRST_P_SPI1 231U
#define SRST_P_SPI2 232U
#define SRST_P_SPI3 233U
#define SRST_P_SPI4 234U
#define SRST_SPI0 235U
#define SRST_SPI1 236U
#define SRST_SPI2 237U
#define SRST_SPI3 238U
#define SRST_SPI4 239U
/********Name=SOFTRST_CON15,Offset=0xA3C********/
#define SRST_P_WDT0 240U
#define SRST_T_WDT0 241U
#define SRST_P_SYS_GRF 242U
#define SRST_P_PWM1 243U
#define SRST_PWM1 244U
#define SRST_P_PWM2 246U
#define SRST_PWM2 247U
#define SRST_P_PWM3 249U
#define SRST_PWM3 250U
#define SRST_P_BUSTIMER0 252U
#define SRST_P_BUSTIMER1 253U
#define SRST_BUSTIMER0 255U
/********Name=SOFTRST_CON16,Offset=0xA40********/
#define SRST_BUSTIMER1 256U
#define SRST_BUSTIMER2 257U
#define SRST_BUSTIMER3 258U
#define SRST_BUSTIMER4 259U
#define SRST_BUSTIMER5 260U
#define SRST_BUSTIMER6 261U
#define SRST_BUSTIMER7 262U
#define SRST_BUSTIMER8 263U
#define SRST_BUSTIMER9 264U
#define SRST_BUSTIMER10 265U
#define SRST_BUSTIMER11 266U
#define SRST_P_MAILBOX0 267U
#define SRST_P_MAILBOX1 268U
#define SRST_P_MAILBOX2 269U
#define SRST_P_GPIO1 270U
#define SRST_DB_GPIO1 271U
/********Name=SOFTRST_CON17,Offset=0xA44********/
#define SRST_P_GPIO2 272U
#define SRST_DB_GPIO2 273U
#define SRST_P_GPIO3 274U
#define SRST_DB_GPIO3 275U
#define SRST_P_GPIO4 276U
#define SRST_DB_GPIO4 277U
#define SRST_A_DECOM 278U
#define SRST_P_DECOM 279U
#define SRST_D_DECOM 280U
#define SRST_P_TOP 281U
#define SRST_A_GICADB_CORE2GIC_BUS 282U
#define SRST_A_GICADB_GIC2CORE_BUS 283U
#define SRST_P_DFT2APB 284U
#define SRST_P_APB2ASB_MST_TOP 285U
#define SRST_P_APB2ASB_MST_CDPHY 286U
#define SRST_P_APB2ASB_MST_BOT_RIGHT 287U
/********Name=SOFTRST_CON18,Offset=0xA48********/
#define SRST_P_APB2ASB_MST_IOC_TOP 288U
#define SRST_P_APB2ASB_MST_IOC_RIGHT 289U
#define SRST_P_APB2ASB_MST_CSIPHY 290U
#define SRST_P_APB2ASB_MST_VCCIO3_5 291U
#define SRST_P_APB2ASB_MST_VCCIO6 292U
#define SRST_P_APB2ASB_MST_EMMCIO 293U
#define SRST_A_SPINLOCK 294U
#define SRST_P_OTPC_TM 295U
#define SRST_OTPC_TM 296U
#define SRST_P_OTPC_NS 297U
#define SRST_OTPC_NS 298U
#define SRST_OTPC_ARB 299U
#define SRST_P_JDBCK_DFT 302U
#define SRST_JDBCK_DFT 303U
/********Name=SOFTRST_CON19,Offset=0xA4C********/
#define SRST_P_BUSIOC 304U
#define SRST_BISRINTF 306U
#define SRST_P_PMUCM0_INTMUX 308U
#define SRST_P_DDRCM0_INTMUX 309U
/********Name=SOFTRST_CON20,Offset=0xA50********/
#define SRST_P_DDR_DFICTL_CH0 320U
#define SRST_P_DDR_MON_CH0 321U
#define SRST_P_DDR_STANDBY_CH0 322U
#define SRST_P_DDR_UPCTL_CH0 323U
#define SRST_TM_DDR_MON_CH0 324U
#define SRST_P_DDR_GRF_CH01 325U
#define SRST_DFI_CH0 326U
#define SRST_SBR_CH0 327U
#define SRST_DDR_UPCTL_CH0 328U
#define SRST_DDR_DFICTL_CH0 329U
#define SRST_DDR_MON_CH0 330U
#define SRST_DDR_STANDBY_CH0 331U
#define SRST_A_DDR_UPCTL_CH0 332U
#define SRST_P_DDR_DFICTL_CH1 333U
#define SRST_P_DDR_MON_CH1 334U
#define SRST_P_DDR_STANDBY_CH1 335U
/********Name=SOFTRST_CON21,Offset=0xA54********/
#define SRST_P_DDR_UPCTL_CH1 336U
#define SRST_TM_DDR_MON_CH1 337U
#define SRST_DFI_CH1 338U
#define SRST_SBR_CH1 339U
#define SRST_DDR_UPCTL_CH1 340U
#define SRST_DDR_DFICTL_CH1 341U
#define SRST_DDR_MON_CH1 342U
#define SRST_DDR_STANDBY_CH1 343U
#define SRST_A_DDR_UPCTL_CH1 344U
#define SRST_A_DDR01_MSCH0 349U
#define SRST_A_DDR01_RS_MSCH0 350U
#define SRST_A_DDR01_FRS_MSCH0 351U
/********Name=SOFTRST_CON22,Offset=0xA58********/
#define SRST_A_DDR01_SCRAMBLE0 352U
#define SRST_A_DDR01_FRS_SCRAMBLE0 353U
#define SRST_A_DDR01_MSCH1 354U
#define SRST_A_DDR01_RS_MSCH1 355U
#define SRST_A_DDR01_FRS_MSCH1 356U
#define SRST_A_DDR01_SCRAMBLE1 357U
#define SRST_A_DDR01_FRS_SCRAMBLE1 358U
#define SRST_P_DDR01_MSCH0 359U
#define SRST_P_DDR01_MSCH1 360U
/********Name=SOFTRST_CON23,Offset=0xA5C********/
#define SRST_P_DDR_DFICTL_CH2 368U
#define SRST_P_DDR_MON_CH2 369U
#define SRST_P_DDR_STANDBY_CH2 370U
#define SRST_P_DDR_UPCTL_CH2 371U
#define SRST_TM_DDR_MON_CH2 372U
#define SRST_P_DDR_GRF_CH23 373U
#define SRST_DFI_CH2 374U
#define SRST_SBR_CH2 375U
#define SRST_DDR_UPCTL_CH2 376U
#define SRST_DDR_DFICTL_CH2 377U
#define SRST_DDR_MON_CH2 378U
#define SRST_DDR_STANDBY_CH2 379U
#define SRST_A_DDR_UPCTL_CH2 380U
#define SRST_P_DDR_DFICTL_CH3 381U
#define SRST_P_DDR_MON_CH3 382U
#define SRST_P_DDR_STANDBY_CH3 383U
/********Name=SOFTRST_CON24,Offset=0xA60********/
#define SRST_P_DDR_UPCTL_CH3 384U
#define SRST_TM_DDR_MON_CH3 385U
#define SRST_DFI_CH3 386U
#define SRST_SBR_CH3 387U
#define SRST_DDR_UPCTL_CH3 388U
#define SRST_DDR_DFICTL_CH3 389U
#define SRST_DDR_MON_CH3 390U
#define SRST_DDR_STANDBY_CH3 391U
#define SRST_A_DDR_UPCTL_CH3 392U
#define SRST_A_DDR23_MSCH2 397U
#define SRST_A_DDR23_RS_MSCH2 398U
#define SRST_A_DDR23_FRS_MSCH2 399U
/********Name=SOFTRST_CON25,Offset=0xA64********/
#define SRST_A_DDR23_SCRAMBLE2 400U
#define SRST_A_DDR23_FRS_SCRAMBLE2 401U
#define SRST_A_DDR23_MSCH3 402U
#define SRST_A_DDR23_RS_MSCH3 403U
#define SRST_A_DDR23_FRS_MSCH3 404U
#define SRST_A_DDR23_SCRAMBLE3 405U
#define SRST_A_DDR23_FRS_SCRAMBLE3 406U
#define SRST_P_DDR23_MSCH2 407U
#define SRST_P_DDR23_MSCH3 408U
/********Name=SOFTRST_CON26,Offset=0xA68********/
#define SRST_ISP1 419U
#define SRST_ISP1_VICAP 420U
#define SRST_A_ISP1_NIU 422U
#define SRST_H_ISP1_NIU 424U
/********Name=SOFTRST_CON27,Offset=0xA6C********/
#define SRST_A_RKNN1 432U
#define SRST_A_RKNN1_NIU 433U
#define SRST_H_RKNN1 434U
#define SRST_H_RKNN1_NIU 435U
/********Name=SOFTRST_CON28,Offset=0xA70********/
#define SRST_A_RKNN2 448U
#define SRST_A_RKNN2_NIU 449U
#define SRST_H_RKNN2 450U
#define SRST_H_RKNN2_NIU 451U
/********Name=SOFTRST_CON29,Offset=0xA74********/
#define SRST_A_RKNN_DSU0 467U
#define SRST_P_NPUTOP_NIU 469U
#define SRST_P_NPU_TIMER 470U
#define SRST_NPUTIMER0 472U
#define SRST_NPUTIMER1 473U
#define SRST_P_NPU_WDT 474U
#define SRST_T_NPU_WDT 475U
#define SRST_P_PVTM1 476U
#define SRST_P_NPU_GRF 477U
#define SRST_PVTM1 478U
/********Name=SOFTRST_CON30,Offset=0xA78********/
#define SRST_NPU_PVTPLL 480U
#define SRST_H_NPU_CM0_NIU 482U
#define SRST_F_NPU_CM0_CORE 483U
#define SRST_T_NPU_CM0_JTAG 484U
#define SRST_A_RKNN0 486U
#define SRST_A_RKNN0_NIU 487U
#define SRST_H_RKNN0 488U
#define SRST_H_RKNN0_NIU 489U
/********Name=SOFTRST_CON31,Offset=0xA7C********/
#define SRST_H_NVM_NIU 498U
#define SRST_A_NVM_NIU 499U
#define SRST_H_EMMC 500U
#define SRST_A_EMMC 501U
#define SRST_C_EMMC 502U
#define SRST_B_EMMC 503U
#define SRST_T_EMMC 504U
#define SRST_S_SFC 505U
#define SRST_H_SFC 506U
#define SRST_H_SFC_XIP 507U
/********Name=SOFTRST_CON32,Offset=0xA80********/
#define SRST_P_GRF 513U
#define SRST_P_DEC_NIU 514U
#define SRST_P_PHP_NIU 517U
#define SRST_A_PCIE_GRIDGE 520U
#define SRST_A_PHP_NIU 521U
#define SRST_A_GMAC0 522U
#define SRST_A_GMAC1 523U
#define SRST_A_PCIE_NIU 524U
#define SRST_PCIE0_POWER_UP 525U
#define SRST_PCIE1_POWER_UP 526U
#define SRST_PCIE2_POWER_UP 527U
/********Name=SOFTRST_CON33,Offset=0xA84********/
#define SRST_PCIE3_POWER_UP 528U
#define SRST_PCIE4_POWER_UP 529U
#define SRST_P_PCIE0 540U
#define SRST_P_PCIE1 541U
#define SRST_P_PCIE2 542U
#define SRST_P_PCIE3 543U
/********Name=SOFTRST_CON34,Offset=0xA88********/
#define SRST_P_PCIE4 544U
#define SRST_A_PHP_GIC_ITS 550U
#define SRST_A_MMU_PCIE 551U
#define SRST_A_MMU_PHP 552U
#define SRST_A_MMU_NIU 553U
/********Name=SOFTRST_CON35,Offset=0xA8C********/
#define SRST_A_USB3OTG2 567U
/********Name=SOFTRST_CON37,Offset=0xA94********/
#define SRST_PMALIVE0 596U
#define SRST_PMALIVE1 597U
#define SRST_PMALIVE2 598U
#define SRST_A_SATA0 599U
#define SRST_A_SATA1 600U
#define SRST_A_SATA2 601U
#define SRST_RXOOB0 602U
#define SRST_RXOOB1 603U
#define SRST_RXOOB2 604U
#define SRST_ASIC0 605U
#define SRST_ASIC1 606U
#define SRST_ASIC2 607U
/********Name=SOFTRST_CON40,Offset=0xAA0********/
#define SRST_A_RKVDEC_CCU 642U
#define SRST_H_RKVDEC0 643U
#define SRST_A_RKVDEC0 644U
#define SRST_H_RKVDEC0_NIU 645U
#define SRST_A_RKVDEC0_NIU 646U
#define SRST_RKVDEC0_CA 647U
#define SRST_RKVDEC0_HEVC_CA 648U
#define SRST_RKVDEC0_CORE 649U
/********Name=SOFTRST_CON41,Offset=0xAA4********/
#define SRST_H_RKVDEC1 658U
#define SRST_A_RKVDEC1 659U
#define SRST_H_RKVDEC1_NIU 660U
#define SRST_A_RKVDEC1_NIU 661U
#define SRST_RKVDEC1_CA 662U
#define SRST_RKVDEC1_HEVC_CA 663U
#define SRST_RKVDEC1_CORE 664U
/********Name=SOFTRST_CON42,Offset=0xAA8********/
#define SRST_A_USB_NIU 674U
#define SRST_H_USB_NIU 675U
#define SRST_A_USB3OTG0 676U
#define SRST_A_USB3OTG1 679U
#define SRST_H_HOST0 682U
#define SRST_H_HOST_ARB0 683U
#define SRST_H_HOST1 684U
#define SRST_H_HOST_ARB1 685U
#define SRST_A_USB_GRF 686U
#define SRST_C_USB2P0_HOST0 687U
/********Name=SOFTRST_CON43,Offset=0xAAC********/
#define SRST_C_USB2P0_HOST1 688U
#define SRST_HOST_UTMI0 689U
#define SRST_HOST_UTMI1 690U
/********Name=SOFTRST_CON44,Offset=0xAB0********/
#define SRST_A_VDPU_NIU 708U
#define SRST_A_VDPU_LOW_NIU 709U
#define SRST_H_VDPU_NIU 710U
#define SRST_A_JPEG_DECODER_NIU 711U
#define SRST_A_VPU 712U
#define SRST_H_VPU 713U
#define SRST_A_JPEG_ENCODER0 714U
#define SRST_H_JPEG_ENCODER0 715U
#define SRST_A_JPEG_ENCODER1 716U
#define SRST_H_JPEG_ENCODER1 717U
#define SRST_A_JPEG_ENCODER2 718U
#define SRST_H_JPEG_ENCODER2 719U
/********Name=SOFTRST_CON45,Offset=0xAB4********/
#define SRST_A_JPEG_ENCODER3 720U
#define SRST_H_JPEG_ENCODER3 721U
#define SRST_A_JPEG_DECODER 722U
#define SRST_H_JPEG_DECODER 723U
#define SRST_H_IEP2P0 724U
#define SRST_A_IEP2P0 725U
#define SRST_IEP2P0_CORE 726U
#define SRST_H_RGA2 727U
#define SRST_A_RGA2 728U
#define SRST_RGA2_CORE 729U
#define SRST_H_RGA3_0 730U
#define SRST_A_RGA3_0 731U
#define SRST_RGA3_0_CORE 732U
/********Name=SOFTRST_CON47,Offset=0xABC********/
#define SRST_H_RKVENC0_NIU 754U
#define SRST_A_RKVENC0_NIU 755U
#define SRST_H_RKVENC0 756U
#define SRST_A_RKVENC0 757U
#define SRST_RKVENC0_CORE 758U
/********Name=SOFTRST_CON48,Offset=0xAC0********/
#define SRST_H_RKVENC1_NIU 770U
#define SRST_A_RKVENC1_NIU 771U
#define SRST_H_RKVENC1 772U
#define SRST_A_RKVENC1 773U
#define SRST_RKVENC1_CORE 774U
/********Name=SOFTRST_CON49,Offset=0xAC4********/
#define SRST_A_VI_NIU 787U
#define SRST_H_VI_NIU 788U
#define SRST_P_VI_NIU 789U
#define SRST_D_VICAP 790U
#define SRST_A_VICAP 791U
#define SRST_H_VICAP 792U
#define SRST_ISP0 794U
#define SRST_ISP0_VICAP 795U
/********Name=SOFTRST_CON50,Offset=0xAC8********/
#define SRST_FISHEYE0 800U
#define SRST_FISHEYE1 803U
#define SRST_P_CSI_HOST_0 804U
#define SRST_P_CSI_HOST_1 805U
#define SRST_P_CSI_HOST_2 806U
#define SRST_P_CSI_HOST_3 807U
#define SRST_P_CSI_HOST_4 808U
#define SRST_P_CSI_HOST_5 809U
/********Name=SOFTRST_CON51,Offset=0xACC********/
#define SRST_CSIHOST0_VICAP 820U
#define SRST_CSIHOST1_VICAP 821U
#define SRST_CSIHOST2_VICAP 822U
#define SRST_CSIHOST3_VICAP 823U
#define SRST_CSIHOST4_VICAP 824U
#define SRST_CSIHOST5_VICAP 825U
#define SRST_CIFIN 829U
/********Name=SOFTRST_CON52,Offset=0xAD0********/
#define SRST_A_VOP_NIU 836U
#define SRST_A_VOP_LOW_NIU 837U
#define SRST_H_VOP_NIU 838U
#define SRST_P_VOP_NIU 839U
#define SRST_H_VOP 840U
#define SRST_A_VOP 841U
#define SRST_D_VOP0 845U
#define SRST_D_VOP2HDMI_BRIDGE0 846U
#define SRST_D_VOP2HDMI_BRIDGE1 847U
/********Name=SOFTRST_CON53,Offset=0xAD4********/
#define SRST_D_VOP1 848U
#define SRST_D_VOP2 849U
#define SRST_D_VOP3 850U
#define SRST_P_VOPGRF 851U
#define SRST_P_DSIHOST0 852U
#define SRST_P_DSIHOST1 853U
#define SRST_DSIHOST0 854U
#define SRST_DSIHOST1 855U
#define SRST_VOP_PMU 856U
#define SRST_P_VOP_CHANNEL_NIU 857U
/********Name=SOFTRST_CON55,Offset=0xADC********/
#define SRST_H_VO0_NIU 885U
#define SRST_H_VO0_S_NIU 886U
#define SRST_P_VO0_NIU 887U
#define SRST_P_VO0_S_NIU 888U
#define SRST_A_HDCP0_NIU 889U
#define SRST_P_VO0GRF 890U
#define SRST_H_HDCP_KEY0 891U
#define SRST_A_HDCP0 892U
#define SRST_H_HDCP0 893U
#define SRST_P_HDCP0 894U
#define SRST_HDCP0 895U
/********Name=SOFTRST_CON56,Offset=0xAE0********/
#define SRST_A_TRNG0 896U
#define SRST_P_TRNG0 897U
#define SRST_DP0 904U
#define SRST_DP1 905U
#define SRST_H_I2S4_8CH 906U
#define SRST_M_I2S4_8CH_TX 909U
#define SRST_H_I2S8_8CH 910U
/********Name=SOFTRST_CON57,Offset=0xAE4********/
#define SRST_M_I2S8_8CH_TX 913U
#define SRST_H_SPDIF2_DP0 914U
#define SRST_M_SPDIF2_DP0 918U
#define SRST_H_SPDIF5_DP1 919U
#define SRST_M_SPDIF5_DP1 923U
/********Name=SOFTRST_CON59,Offset=0xAEC********/
#define SRST_A_HDCP1_NIU 950U
#define SRST_A_HDMIRX_NIU 951U
#define SRST_A_VO1_NIU 952U
#define SRST_H_VOP1_NIU 953U
#define SRST_H_VOP1_S_NIU 954U
#define SRST_P_VOP1_NIU 955U
#define SRST_P_VO1GRF 956U
#define SRST_P_VO1_S_NIU 957U
/********Name=SOFTRST_CON60,Offset=0xAF0********/
#define SRST_H_I2S7_8CH 960U
#define SRST_M_I2S7_8CH_RX 963U
#define SRST_H_HDCP_KEY1 964U
#define SRST_A_HDCP1 965U
#define SRST_H_HDCP1 966U
#define SRST_HDCP1 968U
#define SRST_P_TRNG1 970U
#define SRST_P_HDMITX0 971U
/********Name=SOFTRST_CON61,Offset=0xAF4********/
#define SRST_HDMITX0_REF 976U
#define SRST_P_HDMITX1 978U
#define SRST_HDMITX1_REF 983U
#define SRST_A_HDMIRX 985U
#define SRST_P_HDMIRX 986U
#define SRST_HDMIRX_REF 987U
/********Name=SOFTRST_CON62,Offset=0xAF8********/
#define SRST_P_EDP0 992U
#define SRST_EDP0_24M 993U
#define SRST_P_EDP1 995U
#define SRST_EDP1_24M 996U
#define SRST_M_I2S5_8CH_TX 1000U
#define SRST_H_I2S5_8CH 1004U
#define SRST_M_I2S6_8CH_TX 1007U
/********Name=SOFTRST_CON63,Offset=0xAFC********/
#define SRST_M_I2S6_8CH_RX 1010U
#define SRST_H_I2S6_8CH 1011U
#define SRST_H_SPDIF3 1012U
#define SRST_M_SPDIF3 1015U
#define SRST_H_SPDIF4 1016U
#define SRST_M_SPDIF4 1019U
#define SRST_H_SPDIFRX0 1020U
#define SRST_M_SPDIFRX0 1021U
#define SRST_H_SPDIFRX1 1022U
#define SRST_M_SPDIFRX1 1023U
/********Name=SOFTRST_CON64,Offset=0xB00********/
#define SRST_H_SPDIFRX2 1024U
#define SRST_M_SPDIFRX2 1025U
#define SRST_LINKSYM_HDMITXPHY0 1036U
#define SRST_LINKSYM_HDMITXPHY1 1037U
#define SRST_VO1_BRIDGE0 1038U
#define SRST_VO1_BRIDGE1 1039U
/********Name=SOFTRST_CON65,Offset=0xB04********/
#define SRST_H_I2S9_8CH 1040U
#define SRST_M_I2S9_8CH_RX 1043U
#define SRST_H_I2S10_8CH 1044U
#define SRST_M_I2S10_8CH_RX 1047U
#define SRST_P_S_HDMIRX 1048U
/********Name=SOFTRST_CON66,Offset=0xB08********/
#define SRST_GPU 1060U
#define SRST_SYS_GPU 1061U
#define SRST_A_S_GPU_NIU 1064U
#define SRST_A_M0_GPU_NIU 1065U
#define SRST_A_M1_GPU_NIU 1066U
#define SRST_A_M2_GPU_NIU 1067U
#define SRST_A_M3_GPU_NIU 1068U
#define SRST_P_GPU_NIU 1070U
#define SRST_P_PVTM2 1071U
/********Name=SOFTRST_CON67,Offset=0xB0C********/
#define SRST_PVTM2 1072U
#define SRST_P_GPU_GRF 1074U
#define SRST_GPU_PVTPLL 1075U
#define SRST_PO_GPU_JTAG 1076U
/********Name=SOFTRST_CON68,Offset=0xB10********/
#define SRST_A_AV1_NIU 1089U
#define SRST_A_AV1 1090U
#define SRST_P_AV1_NIU 1092U
#define SRST_P_AV1 1093U
/********Name=SOFTRST_CON69,Offset=0xB14********/
#define SRST_A_DDR_NIU 1108U
#define SRST_A_DMA2DDR 1109U
#define SRST_A_DDR_SHAREMEM 1110U
#define SRST_A_DDR_SHAREMEM_NIU 1111U
#define SRST_A_CENTER_S200_NIU 1114U
#define SRST_A_CENTER_S400_NIU 1115U
#define SRST_H_AHB2APB 1116U
#define SRST_H_CENTER_NIU 1117U
#define SRST_F_DDR_CM0_CORE 1118U
/********Name=SOFTRST_CON70,Offset=0xB18********/
#define SRST_DDR_TIMER0 1120U
#define SRST_DDR_TIMER1 1121U
#define SRST_T_WDT_DDR 1122U
#define SRST_T_DDR_CM0_JTAG 1123U
#define SRST_P_CENTER_GRF 1125U
#define SRST_P_AHB2APB 1126U
#define SRST_P_WDT 1127U
#define SRST_P_TIMER 1128U
#define SRST_P_DMA2DDR 1129U
#define SRST_P_SHAREMEM 1130U
#define SRST_P_CENTER_NIU 1131U
#define SRST_P_CENTER_CHANNEL_NIU 1132U
/********Name=SOFTRST_CON72,Offset=0xB20********/
#define SRST_P_USBDPGRF0 1153U
#define SRST_P_USBDPPHY0 1154U
#define SRST_P_USBDPGRF1 1155U
#define SRST_P_USBDPPHY1 1156U
#define SRST_P_HDPTX0 1157U
#define SRST_P_HDPTX1 1158U
#define SRST_P_APB2ASB_SLV_BOT_RIGHT 1159U
#define SRST_P_USB2PHY_U3_0_GRF0 1160U
#define SRST_P_USB2PHY_U3_1_GRF0 1161U
#define SRST_P_USB2PHY_U2_0_GRF0 1162U
#define SRST_P_USB2PHY_U2_1_GRF0 1163U
#define SRST_SCAN_HDPTX0_ROPLL 1164U
#define SRST_SCAN_HDPTX0_LCPLL 1165U
#define SRST_SCAN_HDPTX0 1166U
#define SRST_SCAN_HDPTX1_ROPLL 1167U
/********Name=SOFTRST_CON73,Offset=0xB24********/
#define SRST_SCAN_HDPTX1_LCPLL 1168U
#define SRST_SCAN_HDPTX1 1169U
#define SRST_SCAN_HDPTX0_HDMIRXPHY_SET 1170U
#define SRST_SCAN_USBDP_COMBO_PHY0 1171U
#define SRST_SCAN_USBDP_COMBO_PHY0_LCPLL 1172U
#define SRST_SCAN_USBDP_COMBO_PHY0_ROPLL 1173U
#define SRST_SCAN_USBDP_COMBO_PHY0_PCS_HS 1174U
#define SRST_SCAN_USBDP_COMBO_PHY1 1175U
#define SRST_SCAN_USBDP_COMBO_PHY1_LCPLL 1176U
#define SRST_SCAN_USBDP_COMBO_PHY1_ROPLL 1177U
#define SRST_SCAN_USBDP_COMBO_PHY1_PCS_HS 1178U
#define SRST_HDMIHDP0 1180U
#define SRST_HDMIHDP1 1181U
/********Name=SOFTRST_CON74,Offset=0xB28********/
#define SRST_A_VO1USB_TOP_NIU 1185U
#define SRST_H_VO1USB_TOP_NIU 1187U
/********Name=SOFTRST_CON75,Offset=0xB2C********/
#define SRST_H_SDIO_NIU 1201U
#define SRST_H_SDIO 1202U
#define SRST_SDIO 1203U
/********Name=SOFTRST_CON76,Offset=0xB30********/
#define SRST_H_RGA3_NIU 1218U
#define SRST_A_RGA3_NIU 1219U
#define SRST_H_RGA3_1 1220U
#define SRST_A_RGA3_1 1221U
#define SRST_RGA3_1_CORE 1222U
/********Name=SOFTRST_CON77,Offset=0xB34********/
#define SRST_REF_PIPE_PHY0 1238U
#define SRST_REF_PIPE_PHY1 1239U
#define SRST_REF_PIPE_PHY2 1240U
/********Name=PMU1SOFTRST_CON00,Offset=0xA00********/
#define SRST_H_PMU1_NIU 26U
#define SRST_P_PMU1_NIU 27U
#define SRST_H_PMU_CM0_NIU 28U
#define SRST_F_PMU_CM0_CORE 29U
#define SRST_T_PMU1_CM0_JTAG 30U
/********Name=PMU1SOFTRST_CON01,Offset=0xA04********/
#define SRST_DDR_FAIL_SAFE 17U
#define SRST_P_CRU_PMU1 18U
#define SRST_P_PMU1_GRF 20U
#define SRST_P_PMU1_IOC 21U
#define SRST_P_PMU1WDT 22U
#define SRST_T_PMU1WDT 23U
#define SRST_P_PMU1TIMER 24U
#define SRST_PMU1TIMER0 26U
#define SRST_PMU1TIMER1 27U
#define SRST_P_PMU1PWM 28U
#define SRST_PMU1PWM 29U
/********Name=PMU1SOFTRST_CON02,Offset=0xA08********/
#define SRST_P_I2C0 17U
#define SRST_I2C0 18U
#define SRST_S_UART0 21U
#define SRST_P_UART0 22U
#define SRST_H_I2S1_8CH 23U
#define SRST_M_I2S1_8CH_TX 26U
#define SRST_M_I2S1_8CH_RX 29U
#define SRST_H_PDM0 30U
#define SRST_PDM0 31U
/********Name=PMU1SOFTRST_CON03,Offset=0xA0C********/
#define SRST_H_VAD 16U
#define SRST_HDPTX0_INIT 27U
#define SRST_HDPTX0_CMN 28U
#define SRST_HDPTX0_LANE 29U
#define SRST_HDPTX1_INIT 31U
/********Name=PMU1SOFTRST_CON04,Offset=0xA10********/
#define SRST_HDPTX1_CMN 16U
#define SRST_HDPTX1_LANE 17U
#define SRST_M_MIPI_DCPHY0 19U
#define SRST_S_MIPI_DCPHY0 20U
#define SRST_M_MIPI_DCPHY1 21U
#define SRST_S_MIPI_DCPHY1 22U
#define SRST_OTGPHY_U3_0 23U
#define SRST_OTGPHY_U3_1 24U
#define SRST_OTGPHY_U2_0 25U
#define SRST_OTGPHY_U2_1 26U
/********Name=PMU1SOFTRST_CON05,Offset=0xA14********/
#define SRST_P_PMU0GRF 19U
#define SRST_P_PMU0IOC 20U
#define SRST_P_GPIO0 21U
#define SRST_DB_GPIO0 22U

/********Name=GATE_CON00,Offset=0x800********/
#define CLK_MATRIX_50M_SRC_GATE 0U
#define CLK_MATRIX_100M_SRC_GATE 1U
#define CLK_MATRIX_150M_SRC_GATE 2U
#define CLK_MATRIX_200M_SRC_GATE 3U
#define CLK_MATRIX_250M_SRC_GATE 4U
#define CLK_MATRIX_300M_SRC_GATE 5U
#define CLK_MATRIX_350M_SRC_GATE 6U
#define CLK_MATRIX_400M_SRC_GATE 7U
#define CLK_MATRIX_450M_SRC_GATE 8U
#define CLK_MATRIX_500M_SRC_GATE 9U
#define CLK_MATRIX_600M_SRC_GATE 10U
#define CLK_MATRIX_650M_SRC_GATE 11U
#define CLK_MATRIX_700M_SRC_GATE 12U
#define CLK_MATRIX_800M_SRC_GATE 13U
#define CLK_MATRIX_1000M_SRC_GATE 14U
#define CLK_MATRIX_1200M_SRC_GATE 15U
/********Name=GATE_CON01,Offset=0x804********/
#define ACLK_TOP_ROOT_GATE 16U
#define PCLK_TOP_ROOT_GATE 17U
#define ACLK_LOW_TOP_ROOT_GATE 18U
#define ACLK_TOP_NIU_GATE 19U
#define PCLK_TOP_NIU_GATE 20U
#define ACLK_LOW_TOP_NIU_GATE 21U
#define PCLK_CSIPHY0_GATE 22U
#define PCLK_CSIPHY1_GATE 24U
#define ACLK_TOP_M300_ROOT_GATE 26U
#define ACLK_TOP_M500_ROOT_GATE 27U
#define ACLK_TOP_M400_ROOT_GATE 28U
#define ACLK_TOP_S200_ROOT_GATE 29U
#define ACLK_TOP_S400_ROOT_GATE 30U
#define ACLK_TOP_M500_NIU_GATE 31U
/********Name=GATE_CON02,Offset=0x808********/
#define ACLK_TOP_M400_NIU_GATE 32U
#define ACLK_TOP_S200_NIU_GATE 33U
#define ACLK_TOP_S400_NIU_GATE 34U
#define ACLK_TOP_M300_NIU_GATE 35U
#define CLK_TESTOUT_TOP_GATE 36U
#define CLK_TESTOUT_GRP0_GATE 38U
#define CLK_USBDP_COMBO_PHY0_IMMORTAL_GATE 40U
#define CLK_USBDP_COMBO_PHY1_IMMORTAL_GATE 47U
/********Name=GATE_CON03,Offset=0x80C********/
#define PCLK_MIPI_DCPHY0_GATE 62U
#define PCLK_MIPI_DCPHY0_GRF_GATE 63U
/********Name=GATE_CON04,Offset=0x810********/
#define PCLK_MIPI_DCPHY1_GATE 67U
#define PCLK_MIPI_DCPHY1_GRF_GATE 68U
#define PCLK_APB2ASB_SLV_CDPHY_GATE 69U
#define PCLK_APB2ASB_SLV_CSIPHY_GATE 70U
#define PCLK_APB2ASB_SLV_VCCIO3_5_GATE 71U
#define PCLK_APB2ASB_SLV_VCCIO6_GATE 72U
#define PCLK_APB2ASB_SLV_EMMCIO_GATE 73U
#define PCLK_APB2ASB_SLV_IOC_TOP_GATE 74U
#define PCLK_APB2ASB_SLV_IOC_RIGHT_GATE 75U
/********Name=GATE_CON05,Offset=0x814********/
#define PCLK_CRU_GATE 80U
#define XIN_OSC0_PAD_GATE 81U
#define XIN_OSC0_TMDECODER_GATE 82U
#define MCLK_GMAC0_OUT_GATE 83U
#define REFCLKO25M_ETH0_OUT_GATE 84U
#define REFCLKO25M_ETH1_OUT_GATE 85U
#define CLK_CIFOUT_OUT_GATE 86U
#define ACLK_CHANNEL_SECURE2VO1USB_GATE 87U
#define ACLK_CHANNEL_SECURE2CENTER_GATE 88U
#define CLK_MIPI_CAMERAOUT_M0_GATE 89U
#define CLK_MIPI_CAMERAOUT_M1_GATE 90U
#define CLK_MIPI_CAMERAOUT_M2_GATE 91U
#define CLK_MIPI_CAMERAOUT_M3_GATE 92U
#define CLK_MIPI_CAMERAOUT_M4_GATE 93U
#define HCLK_CHANNEL_SECURE2VO1USB_GATE 94U
#define HCLK_CHANNEL_SECURE2CENTER_GATE 95U
/********Name=GATE_CON06,Offset=0x818********/
#define PCLK_CHANNEL_SECURE2VO1USB_GATE 96U
#define PCLK_CHANNEL_SECURE2CENTER_GATE 97U
/********Name=GATE_CON07,Offset=0x81C********/
#define HCLK_AUDIO_ROOT_GATE 112U
#define PCLK_AUDIO_ROOT_GATE 113U
#define HCLK_AUDIO_NIU_GATE 114U
#define PCLK_AUDIO_NIU_GATE 115U
#define HCLK_I2S0_8CH_GATE 116U
#define CLK_I2S0_8CH_TX_GATE 117U
#define CLK_I2S0_8CH_FRAC_TX_GATE 118U
#define MCLK_I2S0_8CH_TX_GATE 119U
#define CLK_I2S0_8CH_RX_GATE 120U
#define CLK_I2S0_8CH_FRAC_RX_GATE 121U
#define MCLK_I2S0_8CH_RX_GATE 122U
#define PCLK_ACDCDIG_GATE 123U
#define HCLK_I2S2_2CH_GATE 124U
#define HCLK_I2S3_2CH_GATE 125U
#define CLK_I2S2_2CH_GATE 126U
#define CLK_I2S2_2CH_FRAC_GATE 127U
/********Name=GATE_CON08,Offset=0x820********/
#define MCLK_I2S2_2CH_GATE 128U
#define CLK_I2S3_2CH_GATE 129U
#define CLK_I2S3_2CH_FRAC_GATE 130U
#define MCLK_I2S3_2CH_GATE 131U
#define CLK_DAC_ACDCDIG_GATE 132U
#define HCLK_SPDIF0_GATE 142U
#define CLK_SPDIF0_GATE 143U
/********Name=GATE_CON09,Offset=0x824********/
#define CLK_SPDIF0_FRAC_GATE 144U
#define MCLK_SPDIF0_GATE 145U
#define HCLK_SPDIF1_GATE 146U
#define CLK_SPDIF1_GATE 147U
#define CLK_SPDIF1_FRAC_GATE 148U
#define MCLK_SPDIF1_GATE 149U
#define HCLK_PDM1_GATE 150U
#define MCLK_PDM1_GATE 151U
#define MBIST_CLK_MCLK_PDM1_GATE 152U
#define MBIST_CLK_CLK_ACDCDIG_GATE 153U
/********Name=GATE_CON10,Offset=0x828********/
#define ACLK_BUS_ROOT_GATE 160U
#define ACLK_BUS_NIU_GATE 161U
#define PCLK_BUS_NIU_GATE 162U
#define ACLK_GIC_GATE 163U
#define ACLK_DMAC0_GATE 165U
#define ACLK_DMAC1_GATE 166U
#define ACLK_DMAC2_GATE 167U
#define PCLK_I2C1_GATE 168U
#define PCLK_I2C2_GATE 169U
#define PCLK_I2C3_GATE 170U
#define PCLK_I2C4_GATE 171U
#define PCLK_I2C5_GATE 172U
#define PCLK_I2C6_GATE 173U
#define PCLK_I2C7_GATE 174U
#define PCLK_I2C8_GATE 175U
/********Name=GATE_CON11,Offset=0x82C********/
#define CLK_I2C1_GATE 176U
#define CLK_I2C2_GATE 177U
#define CLK_I2C3_GATE 178U
#define CLK_I2C4_GATE 179U
#define CLK_I2C5_GATE 180U
#define CLK_I2C6_GATE 181U
#define CLK_I2C7_GATE 182U
#define CLK_I2C8_GATE 183U
#define PCLK_CAN0_GATE 184U
#define CLK_CAN0_GATE 185U
#define PCLK_CAN1_GATE 186U
#define CLK_CAN1_GATE 187U
#define PCLK_CAN2_GATE 188U
#define CLK_CAN2_GATE 189U
#define PCLK_SARADC_GATE 190U
#define CLK_SARADC_GATE 191U
/********Name=GATE_CON12,Offset=0x830********/
#define PCLK_TSADC_GATE 192U
#define CLK_TSADC_GATE 193U
#define PCLK_UART1_GATE 194U
#define PCLK_UART2_GATE 195U
#define PCLK_UART3_GATE 196U
#define PCLK_UART4_GATE 197U
#define PCLK_UART5_GATE 198U
#define PCLK_UART6_GATE 199U
#define PCLK_UART7_GATE 200U
#define PCLK_UART8_GATE 201U
#define PCLK_UART9_GATE 202U
#define CLK_UART1_GATE 203U
#define CLK_UART1_FRAC_GATE 204U
#define SCLK_UART1_GATE 205U
#define CLK_UART2_GATE 206U
#define CLK_UART2_FRAC_GATE 207U
/********Name=GATE_CON13,Offset=0x834********/
#define SCLK_UART2_GATE 208U
#define CLK_UART3_GATE 209U
#define CLK_UART3_FRAC_GATE 210U
#define SCLK_UART3_GATE 211U
#define CLK_UART4_GATE 212U
#define CLK_UART4_FRAC_GATE 213U
#define SCLK_UART4_GATE 214U
#define CLK_UART5_GATE 215U
#define CLK_UART5_FRAC_GATE 216U
#define SCLK_UART5_GATE 217U
#define CLK_UART6_GATE 218U
#define CLK_UART6_FRAC_GATE 219U
#define SCLK_UART6_GATE 220U
#define CLK_UART7_GATE 221U
#define CLK_UART7_FRAC_GATE 222U
#define SCLK_UART7_GATE 223U
/********Name=GATE_CON14,Offset=0x838********/
#define CLK_UART8_GATE 224U
#define CLK_UART8_FRAC_GATE 225U
#define SCLK_UART8_GATE 226U
#define CLK_UART9_GATE 227U
#define CLK_UART9_FRAC_GATE 228U
#define SCLK_UART9_GATE 229U
#define PCLK_SPI0_GATE 230U
#define PCLK_SPI1_GATE 231U
#define PCLK_SPI2_GATE 232U
#define PCLK_SPI3_GATE 233U
#define PCLK_SPI4_GATE 234U
#define CLK_SPI0_GATE 235U
#define CLK_SPI1_GATE 236U
#define CLK_SPI2_GATE 237U
#define CLK_SPI3_GATE 238U
#define CLK_SPI4_GATE 239U
/********Name=GATE_CON15,Offset=0x83C********/
#define PCLK_WDT0_GATE 240U
#define TCLK_WDT0_GATE 241U
#define PCLK_SYS_GRF_GATE 242U
#define PCLK_PWM1_GATE 243U
#define CLK_PWM1_GATE 244U
#define CLK_PWM1_CAPTURE_GATE 245U
#define PCLK_PWM2_GATE 246U
#define CLK_PWM2_GATE 247U
#define CLK_PWM2_CAPTURE_GATE 248U
#define PCLK_PWM3_GATE 249U
#define CLK_PWM3_GATE 250U
#define CLK_PWM3_CAPTURE_GATE 251U
#define PCLK_BUSTIMER0_GATE 252U
#define PCLK_BUSTIMER1_GATE 253U
#define CLK_BUSTIMER_ROOT_GATE 254U
#define CLK_BUSTIMER0_GATE 255U
/********Name=GATE_CON16,Offset=0x840********/
#define CLK_BUSTIMER1_GATE 256U
#define CLK_BUSTIMER2_GATE 257U
#define CLK_BUSTIMER3_GATE 258U
#define CLK_BUSTIMER4_GATE 259U
#define CLK_BUSTIMER5_GATE 260U
#define CLK_BUSTIMER6_GATE 261U
#define CLK_BUSTIMER7_GATE 262U
#define CLK_BUSTIMER8_GATE 263U
#define CLK_BUSTIMER9_GATE 264U
#define CLK_BUSTIMER10_GATE 265U
#define CLK_BUSTIMER11_GATE 266U
#define PCLK_MAILBOX0_GATE 267U
#define PCLK_MAILBOX1_GATE 268U
#define PCLK_MAILBOX2_GATE 269U
#define PCLK_GPIO1_GATE 270U
#define DBCLK_GPIO1_GATE 271U
/********Name=GATE_CON17,Offset=0x844********/
#define PCLK_GPIO2_GATE 272U
#define DBCLK_GPIO2_GATE 273U
#define PCLK_GPIO3_GATE 274U
#define DBCLK_GPIO3_GATE 275U
#define PCLK_GPIO4_GATE 276U
#define DBCLK_GPIO4_GATE 277U
#define ACLK_DECOM_GATE 278U
#define PCLK_DECOM_GATE 279U
#define DCLK_DECOM_GATE 280U
#define PCLK_TOP_GATE 281U
#define ACLK_GICADB_CORE2GIC_BUS_GATE 282U
#define ACLK_GICADB_GIC2CORE_BUS_GATE 283U
#define PCLK_DFT2APB_GATE 284U
#define PCLK_APB2ASB_MST_TOP_GATE 285U
#define PCLK_APB2ASB_MST_CDPHY_GATE 286U
#define PCLK_APB2ASB_MST_BOT_RIGHT_GATE 287U
/********Name=GATE_CON18,Offset=0x848********/
#define PCLK_APB2ASB_MST_IOC_TOP_GATE 288U
#define PCLK_APB2ASB_MST_IOC_RIGHT_GATE 289U
#define PCLK_APB2ASB_MST_CSIPHY_GATE 290U
#define PCLK_APB2ASB_MST_VCCIO3_5_GATE 291U
#define PCLK_APB2ASB_MST_VCCIO6_GATE 292U
#define PCLK_APB2ASB_MST_EMMCIO_GATE 293U
#define ACLK_SPINLOCK_GATE 294U
#define PCLK_OTPC_TM_GATE 295U
#define CLK_OTPC_TM_GATE 296U
#define PCLK_OTPC_NS_GATE 297U
#define CLK_OTPC_NS_GATE 298U
#define CLK_OTPC_ARB_GATE 299U
#define CLK_OTPC_AUTO_RD_GATE 300U
#define CLK_OTP_PHY_GATE 301U
#define PCLK_JDBCK_DFT_GATE 302U
#define CLK_JDBCK_DFT_GATE 303U
/********Name=GATE_CON19,Offset=0x84C********/
#define PCLK_BUSIOC_GATE 304U
#define CLK_BISRINTF_PLLSRC_GATE 305U
#define CLK_BISRINTF_GATE 306U
#define PCLK_PMU2_GATE 307U
#define PCLK_PMUCM0_INTMUX_GATE 308U
#define PCLK_DDRCM0_INTMUX_GATE 309U
#define MBIST_CLK_ACLK_DECOM_GATE 310U
/********Name=GATE_CON20,Offset=0x850********/
#define PCLK_DDR_DFICTL_CH0_GATE 320U
#define PCLK_DDR_MON_CH0_GATE 321U
#define PCLK_DDR_STANDBY_CH0_GATE 322U
#define PCLK_DDR_UPCTL_CH0_GATE 323U
#define TMCLK_DDR_MON_CH0_GATE 324U
#define PCLK_DDR_GRF_CH01_GATE 325U
#define CLK_DFI_CH0_GATE 326U
#define CLK_SBR_CH0_GATE 327U
#define CLK_DDR_UPCTL_CH0_GATE 328U
#define CLK_DDR_DFICTL_CH0_GATE 329U
#define CLK_DDR_MON_CH0_GATE 330U
#define CLK_DDR_STANDBY_CH0_GATE 331U
#define ACLK_DDR_UPCTL_CH0_GATE 332U
#define PCLK_DDR_DFICTL_CH1_GATE 333U
#define PCLK_DDR_MON_CH1_GATE 334U
#define PCLK_DDR_STANDBY_CH1_GATE 335U
/********Name=GATE_CON21,Offset=0x854********/
#define PCLK_DDR_UPCTL_CH1_GATE 336U
#define TMCLK_DDR_MON_CH1_GATE 337U
#define CLK_DFI_CH1_GATE 338U
#define CLK_SBR_CH1_GATE 339U
#define CLK_DDR_UPCTL_CH1_GATE 340U
#define CLK_DDR_DFICTL_CH1_GATE 341U
#define CLK_DDR_MON_CH1_GATE 342U
#define CLK_DDR_STANDBY_CH1_GATE 343U
#define ACLK_DDR_UPCTL_CH1_GATE 344U
#define MBIST_CLK_CLK_DDR_UPCTRL_CH0_GATE 345U
#define MBIST_CLK_CLK_DDR_UPCTRL_CH1_GATE 346U
#define MBIST_CLK_ACLK_DDR01_MSCH_CH0_GATE 347U
#define MBIST_CLK_ACLK_DDR01_MSCH_CH1_GATE 348U
#define ACLK_DDR01_MSCH0_GATE 349U
#define ACLK_DDR01_RS_MSCH0_GATE 350U
#define ACLK_DDR01_FRS_MSCH0_GATE 351U
/********Name=GATE_CON22,Offset=0x858********/
#define ACLK_DDR01_SCRAMBLE0_GATE 352U
#define ACLK_DDR01_FRS_SCRAMBLE0_GATE 353U
#define ACLK_DDR01_MSCH1_GATE 354U
#define ACLK_DDR01_RS_MSCH1_GATE 355U
#define ACLK_DDR01_FRS_MSCH1_GATE 356U
#define ACLK_DDR01_SCRAMBLE1_GATE 357U
#define ACLK_DDR01_FRS_SCRAMBLE1_GATE 358U
#define PCLK_DDR01_MSCH0_GATE 359U
#define PCLK_DDR01_MSCH1_GATE 360U
#define CLK_TESTOUT_DDR01_GATE 361U
/********Name=GATE_CON23,Offset=0x85C********/
#define PCLK_DDR_DFICTL_CH2_GATE 368U
#define PCLK_DDR_MON_CH2_GATE 369U
#define PCLK_DDR_STANDBY_CH2_GATE 370U
#define PCLK_DDR_UPCTL_CH2_GATE 371U
#define TMCLK_DDR_MON_CH2_GATE 372U
#define PCLK_DDR_GRF_CH23_GATE 373U
#define CLK_DFI_CH2_GATE 374U
#define CLK_SBR_CH2_GATE 375U
#define CLK_DDR_UPCTL_CH2_GATE 376U
#define CLK_DDR_DFICTL_CH2_GATE 377U
#define CLK_DDR_MON_CH2_GATE 378U
#define CLK_DDR_STANDBY_CH2_GATE 379U
#define ACLK_DDR_UPCTL_CH2_GATE 380U
#define PCLK_DDR_DFICTL_CH3_GATE 381U
#define PCLK_DDR_MON_CH3_GATE 382U
#define PCLK_DDR_STANDBY_CH3_GATE 383U
/********Name=GATE_CON24,Offset=0x860********/
#define PCLK_DDR_UPCTL_CH3_GATE 384U
#define TMCLK_DDR_MON_CH3_GATE 385U
#define CLK_DFI_CH3_GATE 386U
#define CLK_SBR_CH3_GATE 387U
#define CLK_DDR_UPCTL_CH3_GATE 388U
#define CLK_DDR_DFICTL_CH3_GATE 389U
#define CLK_DDR_MON_CH3_GATE 390U
#define CLK_DDR_STANDBY_CH3_GATE 391U
#define ACLK_DDR_UPCTL_CH3_GATE 392U
#define MBIST_CLK_CLK_DDR_UPCTRL_CH2_GATE 393U
#define MBIST_CLK_CLK_DDR_UPCTRL_CH3_GATE 394U
#define MBIST_CLK_ACLK_DDR23_MSCH_CH2_GATE 395U
#define MBIST_CLK_ACLK_DDR23_MSCH_CH3_GATE 396U
#define ACLK_DDR23_MSCH2_GATE 397U
#define ACLK_DDR23_RS_MSCH2_GATE 398U
#define ACLK_DDR23_FRS_MSCH2_GATE 399U
/********Name=GATE_CON25,Offset=0x864********/
#define ACLK_DDR23_SCRAMBLE2_GATE 400U
#define ACLK_DDR23_FRS_SCRAMBLE2_GATE 401U
#define ACLK_DDR23_MSCH3_GATE 402U
#define ACLK_DDR23_RS_MSCH3_GATE 403U
#define ACLK_DDR23_FRS_MSCH3_GATE 404U
#define ACLK_DDR23_SCRAMBLE3_GATE 405U
#define ACLK_DDR23_FRS_SCRAMBLE3_GATE 406U
#define PCLK_DDR23_MSCH2_GATE 407U
#define PCLK_DDR23_MSCH3_GATE 408U
#define CLK_TESTOUT_DDR23_GATE 409U
/********Name=GATE_CON26,Offset=0x868********/
#define ACLK_ISP1_ROOT_GATE 416U
#define HCLK_ISP1_ROOT_GATE 417U
#define CLK_ISP1_CORE_GATE 418U
#define CLK_ISP1_CORE_MARVIN_GATE 419U
#define CLK_ISP1_CORE_VICAP_GATE 420U
#define ACLK_ISP1_GATE 421U
#define ACLK_ISP1_NIU_GATE 422U
#define HCLK_ISP1_GATE 423U
#define HCLK_ISP1_NIU_GATE 424U
#define MBIST_CLK_CLK_ISP1_CORE_GATE 425U
#define MBIST_CLK_ACLK_ISP1_GATE 426U
#define MBIST_CLK_HCLK_ISP1_GATE 427U
/********Name=GATE_CON27,Offset=0x86C********/
#define ACLK_RKNN1_GATE 432U
#define ACLK_RKNN1_NIU_GATE 433U
#define HCLK_RKNN1_GATE 434U
#define HCLK_RKNN1_NIU_GATE 435U
/********Name=GATE_CON28,Offset=0x870********/
#define ACLK_RKNN2_GATE 448U
#define ACLK_RKNN2_NIU_GATE 449U
#define HCLK_RKNN2_GATE 450U
#define HCLK_RKNN2_NIU_GATE 451U
/********Name=GATE_CON29,Offset=0x874********/
#define HCLK_RKNN_ROOT_GATE 464U
#define CLK_RKNN_DSU0_DF_GATE 465U
#define CLK_TESTOUT_NPU_GATE 466U
#define CLK_RKNN_DSU0_GATE 467U
#define PCLK_NPUTOP_ROOT_GATE 468U
#define PCLK_NPUTOP_NIU_GATE 469U
#define PCLK_NPU_TIMER_GATE 470U
#define CLK_NPUTIMER_ROOT_GATE 471U
#define CLK_NPUTIMER0_GATE 472U
#define CLK_NPUTIMER1_GATE 473U
#define PCLK_NPU_WDT_GATE 474U
#define TCLK_NPU_WDT_GATE 475U
#define PCLK_PVTM1_GATE 476U
#define PCLK_NPU_GRF_GATE 477U
#define CLK_PVTM1_GATE 478U
#define CLK_NPU_PVTM_GATE 479U
/********Name=GATE_CON30,Offset=0x878********/
#define CLK_NPU_PVTPLL_GATE 480U
#define HCLK_NPU_CM0_ROOT_GATE 481U
#define HCLK_NPU_CM0_NIU_GATE 482U
#define FCLK_NPU_CM0_CORE_GATE 483U
#define CLK_NPU_CM0_RTC_GATE 485U
#define ACLK_RKNN0_GATE 486U
#define ACLK_RKNN0_NIU_GATE 487U
#define HCLK_RKNN0_GATE 488U
#define HCLK_RKNN0_NIU_GATE 489U
/********Name=GATE_CON31,Offset=0x87C********/
#define HCLK_NVM_ROOT_GATE 496U
#define ACLK_NVM_ROOT_GATE 497U
#define HCLK_NVM_NIU_GATE 498U
#define ACLK_NVM_NIU_GATE 499U
#define HCLK_EMMC_GATE 500U
#define ACLK_EMMC_GATE 501U
#define CCLK_EMMC_GATE 502U
#define BCLK_EMMC_GATE 503U
#define TMCLK_EMMC_GATE 504U
#define SCLK_SFC_GATE 505U
#define HCLK_SFC_GATE 506U
#define HCLK_SFC_XIP_GATE 507U
#define MBIST_CLK_EMMC_GATE 508U
/********Name=GATE_CON32,Offset=0x880********/
#define PCLK_PHP_ROOT_GATE 512U
#define PCLK_GRF_GATE 513U
#define PCLK_DEC_NIU_GATE 514U
#define PCLK_GMAC0_GATE 515U
#define PCLK_GMAC1_GATE 516U
#define PCLK_PHP_NIU_GATE 517U
#define ACLK_PCIE_ROOT_GATE 518U
#define ACLK_PHP_ROOT_GATE 519U
#define ACLK_PCIE_GRIDGE_GATE 520U
#define ACLK_PHP_NIU_GATE 521U
#define ACLK_GMAC0_GATE 522U
#define ACLK_GMAC1_GATE 523U
#define ACLK_PCIE_NIU_GATE 524U
#define ACLK_PCIE_DBI0_GATE 525U
#define ACLK_PCIE_DBI1_GATE 526U
#define ACLK_PCIE_DBI2_GATE 527U
/********Name=GATE_CON33,Offset=0x884********/
#define ACLK_PCIE_DBI3_GATE 528U
#define ACLK_PCIE_DBI4_GATE 529U
#define ACLK_PCIE_MSTR0_GATE 530U
#define ACLK_PCIE_MSTR1_GATE 531U
#define ACLK_PCIE_MSTR2_GATE 532U
#define ACLK_PCIE_MSTR3_GATE 533U
#define ACLK_PCIE_MSTR4_GATE 534U
#define ACLK_PCIE_SLV0_GATE 535U
#define ACLK_PCIE_SLV1_GATE 536U
#define ACLK_PCIE_SLV2_GATE 537U
#define ACLK_PCIE_SLV3_GATE 538U
#define ACLK_PCIE_SLV4_GATE 539U
#define PCLK_PCIE0_GATE 540U
#define PCLK_PCIE1_GATE 541U
#define PCLK_PCIE2_GATE 542U
#define PCLK_PCIE3_GATE 543U
/********Name=GATE_CON34,Offset=0x888********/
#define PCLK_PCIE4_GATE 544U
#define CLK_PCIE_AUX0_GATE 545U
#define CLK_PCIE_AUX1_GATE 546U
#define CLK_PCIE_AUX2_GATE 547U
#define CLK_PCIE_AUX3_GATE 548U
#define CLK_PCIE_AUX4_GATE 549U
#define ACLK_PHP_GIC_ITS_GATE 550U
#define ACLK_MMU_PCIE_GATE 551U
#define ACLK_MMU_PHP_GATE 552U
#define ACLK_MMU_NIU_GATE 553U
#define CLK_GMAC0_PTP_REF_GATE 554U
#define CLK_GMAC1_PTP_REF_GATE 555U
/********Name=GATE_CON35,Offset=0x88C********/
#define CLK_GMAC_125M_CRU_GATE 565U
#define CLK_GMAC_50M_CRU_GATE 566U
#define ACLK_USB3OTG2_GATE 567U
#define SUSPEND_CLK_USB3OTG2_GATE 568U
#define REF_CLK_USB3OTG2_GATE 569U
#define CLK_UTMI_OTG2_GATE 570U
#define MBIST_CLK_ACLK_SATA0_GATE 571U
#define MBIST_CLK_ACLK_SATA1_GATE 572U
#define MBIST_CLK_ACLK_SATA2_GATE 573U
#define MBIST_CLK_CLK_PCIE4L_PIPE_GATE 574U
#define MBIST_CLK_ACLK_PCIE_MSTR0_GATE 575U
/********Name=GATE_CON36,Offset=0x890********/
#define MBIST_CLK_ACLK_PCIE_SLV0_GATE 576U
#define MBIST_CLK_CLK_PCIE2L_PIPE_GATE 577U
#define MBIST_CLK_ACLK_PCIE_MSTR1_GATE 578U
#define MBIST_CLK_ACLK_PCIE_SLV1_GATE 579U
#define MBIST_CLK_CLK_PCIE1L0_PIPE_GATE 580U
#define MBIST_CLK_ACLK_PCIE_MSTR2_GATE 581U
#define MBIST_CLK_ACLK_PCIE_SLV2_GATE 582U
#define MBIST_CLK_CLK_PCIE1L1_PIPE_GATE 583U
#define MBIST_CLK_ACLK_PCIE_MSTR3_GATE 584U
#define MBIST_CLK_ACLK_PCIE_SLV3_GATE 585U
#define MBIST_CLK_CLK_PCIE1L2_PIPE_GATE 586U
#define MBIST_CLK_ACLK_PCIE_MSTR4_GATE 587U
#define MBIST_CLK_ACLK_PCIE_SLV4_GATE 588U
#define MBIST_CLK_ACLK_GMAC0_GATE 589U
#define MBIST_CLK_ACLK_GMAC1_GATE 590U
#define MBIST_CLK_ACLK_USB3OTG2_GATE 591U
/********Name=GATE_CON37,Offset=0x894********/
#define CLK_PIPEPHY0_REF_GATE 592U
#define CLK_PIPEPHY1_REF_GATE 593U
#define CLK_PIPEPHY2_REF_GATE 594U
#define CLK_PMALIVE0_GATE 596U
#define CLK_PMALIVE1_GATE 597U
#define CLK_PMALIVE2_GATE 598U
#define ACLK_SATA0_GATE 599U
#define ACLK_SATA1_GATE 600U
#define ACLK_SATA2_GATE 601U
#define CLK_RXOOB0_GATE 602U
#define CLK_RXOOB1_GATE 603U
#define CLK_RXOOB2_GATE 604U
/********Name=GATE_CON38,Offset=0x898********/
#define CLK_PIPEPHY0_PIPE_G_GATE 611U
#define CLK_PIPEPHY1_PIPE_G_GATE 612U
#define CLK_PIPEPHY2_PIPE_G_GATE 613U
#define CLK_PIPEPHY0_PIPE_ASIC_G_GATE 614U
#define CLK_PIPEPHY1_PIPE_ASIC_G_GATE 615U
#define CLK_PIPEPHY2_PIPE_ASIC_G_GATE 616U
#define CLK_PIPEPHY2_PIPE_U3_G_GATE 617U
#define CLK_PCIE1L2_PIPE_GATE 621U
#define CLK_PCIE1L0_PIPE_GATE 622U
#define CLK_PCIE1L1_PIPE_GATE 623U
/********Name=GATE_CON39,Offset=0x89C********/
#define CLK_PCIE4L_PIPE_GATE 624U
#define CLK_PCIE2L_PIPE_GATE 625U
/********Name=GATE_CON40,Offset=0x8A0********/
#define HCLK_RKVDEC0_ROOT_GATE 640U
#define ACLK_RKVDEC0_ROOT_GATE 641U
#define ACLK_RKVDEC_CCU_GATE 642U
#define HCLK_RKVDEC0_GATE 643U
#define ACLK_RKVDEC0_GATE 644U
#define HCLK_RKVDEC0_NIU_GATE 645U
#define ACLK_RKVDEC0_NIU_GATE 646U
#define CLK_RKVDEC0_CA_GATE 647U
#define CLK_RKVDEC0_HEVC_CA_GATE 648U
#define CLK_RKVDEC0_CORE_GATE 649U
#define MBIST_CLK_ACLK_RKVDEC0_GATE 650U
#define MBIST_CLK_CLK_RKVDEC0_HEVC_CA_GATE 651U
#define MBIST_CLK_CLK_RKVDEC0_CORE_GATE 652U
#define MBIST_CLK_CLK_RKVDEC0_CA_GATE 653U
/********Name=GATE_CON41,Offset=0x8A4********/
#define HCLK_RKVDEC1_ROOT_GATE 656U
#define ACLK_RKVDEC1_ROOT_GATE 657U
#define HCLK_RKVDEC1_GATE 658U
#define ACLK_RKVDEC1_GATE 659U
#define HCLK_RKVDEC1_NIU_GATE 660U
#define ACLK_RKVDEC1_NIU_GATE 661U
#define CLK_RKVDEC1_CA_GATE 662U
#define CLK_RKVDEC1_HEVC_CA_GATE 663U
#define CLK_RKVDEC1_CORE_GATE 664U
#define MBIST_CLK_ACLK_RKVDEC1_GATE 665U
#define MBIST_CLK_CLK_RKVDEC1_HEVC_CA_GATE 666U
#define MBIST_CLK_CLK_RKVDEC1_CORE_GATE 667U
#define MBIST_CLK_CLK_RKVDEC1_CA_GATE 668U
/********Name=GATE_CON42,Offset=0x8A8********/
#define ACLK_USB_ROOT_GATE 672U
#define HCLK_USB_ROOT_GATE 673U
#define ACLK_USB_NIU_GATE 674U
#define HCLK_USB_NIU_GATE 675U
#define ACLK_USB3OTG0_GATE 676U
#define SUSPEND_CLK_USB3OTG0_GATE 677U
#define REF_CLK_USB3OTG0_GATE 678U
#define ACLK_USB3OTG1_GATE 679U
#define SUSPEND_CLK_USB3OTG1_GATE 680U
#define REF_CLK_USB3OTG1_GATE 681U
#define HCLK_HOST0_GATE 682U
#define HCLK_HOST_ARB0_GATE 683U
#define HCLK_HOST1_GATE 684U
#define HCLK_HOST_ARB1_GATE 685U
#define ACLK_USB_GRF_GATE 686U
#define UTMI_OHCI_CLK48_HOST0_GATE 687U
/********Name=GATE_CON43,Offset=0x8AC********/
#define UTMI_OHCI_CLK48_HOST1_GATE 688U
#define MBIST_CLK_HCLK_HOST0_GATE 691U
#define MBIST_CLK_HCLK_HOST1_GATE 692U
#define MBIST_CLK_ACLK_USB3OTG0_GATE 693U
#define MBIST_CLK_ACLK_USB3OTG1_GATE 694U
/********Name=GATE_CON44,Offset=0x8B0********/
#define ACLK_VDPU_ROOT_GATE 704U
#define ACLK_VDPU_LOW_ROOT_GATE 705U
#define HCLK_VDPU_ROOT_GATE 706U
#define ACLK_JPEG_DECODER_ROOT_GATE 707U
#define ACLK_VDPU_NIU_GATE 708U
#define ACLK_VDPU_LOW_NIU_GATE 709U
#define HCLK_VDPU_NIU_GATE 710U
#define ACLK_JPEG_DECODER_NIU_GATE 711U
#define ACLK_VPU_GATE 712U
#define HCLK_VPU_GATE 713U
#define ACLK_JPEG_ENCODER0_GATE 714U
#define HCLK_JPEG_ENCODER0_GATE 715U
#define ACLK_JPEG_ENCODER1_GATE 716U
#define HCLK_JPEG_ENCODER1_GATE 717U
#define ACLK_JPEG_ENCODER2_GATE 718U
#define HCLK_JPEG_ENCODER2_GATE 719U
/********Name=GATE_CON45,Offset=0x8B4********/
#define ACLK_JPEG_ENCODER3_GATE 720U
#define HCLK_JPEG_ENCODER3_GATE 721U
#define ACLK_JPEG_DECODER_GATE 722U
#define HCLK_JPEG_DECODER_GATE 723U
#define HCLK_IEP2P0_GATE 724U
#define ACLK_IEP2P0_GATE 725U
#define CLK_IEP2P0_CORE_GATE 726U
#define HCLK_RGA2_GATE 727U
#define ACLK_RGA2_GATE 728U
#define CLK_RGA2_CORE_GATE 729U
#define HCLK_RGA3_0_GATE 730U
#define ACLK_RGA3_0_GATE 731U
#define CLK_RGA3_0_CORE_GATE 732U
#define MBIST_CLK_ACLK_RGA3_0_GATE 733U
#define MBIST_CLK_CLK_RGA3_0_CORE_GATE 734U
#define MBIST_CLK_CLK_RGA2_GATE 735U
/********Name=GATE_CON46,Offset=0x8B8********/
#define MBIST_CLK_ACLK_IEP2P0_GATE 736U
#define MBIST_CLK_CLK_IEP2P0_CORE_GATE 737U
#define MBIST_CLK_ACLK_JPEG_ENCODER0_GATE 738U
#define MBIST_CLK_ACLK_JPEG_ENCODER1_GATE 739U
#define MBIST_CLK_ACLK_JPEG_ENCODER2_GATE 740U
#define MBIST_CLK_ACLK_JPEG_ENCODER3_GATE 741U
#define MBIST_CLK_ACLK_JPEG_DECODER_GATE 742U
#define MBIST_CLK_ACLK_VPU_GATE 743U
/********Name=GATE_CON47,Offset=0x8BC********/
#define HCLK_RKVENC0_ROOT_GATE 752U
#define ACLK_RKVENC0_ROOT_GATE 753U
#define HCLK_RKVENC0_NIU_GATE 754U
#define ACLK_RKVENC0_NIU_GATE 755U
#define HCLK_RKVENC0_GATE 756U
#define ACLK_RKVENC0_GATE 757U
#define CLK_RKVENC0_CORE_GATE 758U
/********Name=GATE_CON48,Offset=0x8C0********/
#define HCLK_RKVENC1_ROOT_GATE 768U
#define ACLK_RKVENC1_ROOT_GATE 769U
#define HCLK_RKVENC1_NIU_GATE 770U
#define ACLK_RKVENC1_NIU_GATE 771U
#define HCLK_RKVENC1_GATE 772U
#define ACLK_RKVENC1_GATE 773U
#define CLK_RKVENC1_CORE_GATE 774U
/********Name=GATE_CON49,Offset=0x8C4********/
#define ACLK_VI_ROOT_GATE 784U
#define HCLK_VI_ROOT_GATE 785U
#define PCLK_VI_ROOT_GATE 786U
#define ACLK_VI_NIU_GATE 787U
#define HCLK_VI_NIU_GATE 788U
#define PCLK_VI_NIU_GATE 789U
#define DCLK_VICAP_GATE 790U
#define ACLK_VICAP_GATE 791U
#define HCLK_VICAP_GATE 792U
#define CLK_ISP0_CORE_GATE 793U
#define CLK_ISP0_CORE_MARVIN_GATE 794U
#define CLK_ISP0_CORE_VICAP_GATE 795U
#define ACLK_ISP0_GATE 796U
#define HCLK_ISP0_GATE 797U
#define ACLK_FISHEYE0_GATE 798U
#define HCLK_FISHEYE0_GATE 799U
/********Name=GATE_CON50,Offset=0x8C8********/
#define CLK_FISHEYE0_CORE_GATE 800U
#define ACLK_FISHEYE1_GATE 801U
#define HCLK_FISHEYE1_GATE 802U
#define CLK_FISHEYE1_CORE_GATE 803U
#define PCLK_CSI_HOST_0_GATE 804U
#define PCLK_CSI_HOST_1_GATE 805U
#define PCLK_CSI_HOST_2_GATE 806U
#define PCLK_CSI_HOST_3_GATE 807U
#define PCLK_CSI_HOST_4_GATE 808U
#define PCLK_CSI_HOST_5_GATE 809U
#define MBIST_CLK_CLK_ISP0_CORE_GATE 810U
#define MBIST_CLK_ACLK_ISP0_GATE 811U
#define MBIST_CLK_ACLK_VICAP_GATE 812U
#define MBIST_CLK_VICAP_CLK_ISP0_CORE_GATE 813U
#define MBIST_CLK_VICAP_CLK_ISP1_CORE_GATE 814U
#define MBIST_CLK_ACLK_FISHEYE0_GATE 815U
/********Name=GATE_CON51,Offset=0x8CC********/
#define MBIST_CLK_CLK_FISHEYE0_CORE_GATE 816U
#define MBIST_CLK_ACLK_FISHEYE1_GATE 817U
#define MBIST_CLK_CLK_FISHEYE1_CORE_GATE 818U
#define MBIST_CLK_DCLK_VICAP_GATE 819U
#define CLK_CSIHOST0_VICAP_GATE 820U
#define CLK_CSIHOST1_VICAP_GATE 821U
#define CLK_CSIHOST2_VICAP_GATE 822U
#define CLK_CSIHOST3_VICAP_GATE 823U
#define CLK_CSIHOST4_VICAP_GATE 824U
#define CLK_CSIHOST5_VICAP_GATE 825U
#define ICLK_CSIHOST01_GATE 826U
#define ICLK_CSIHOST0_GATE 827U
#define ICLK_CSIHOST1_GATE 828U
/********Name=GATE_CON52,Offset=0x8D0********/
#define ACLK_VOP_ROOT_GATE 832U
#define ACLK_VOP_LOW_ROOT_GATE 833U
#define HCLK_VOP_ROOT_GATE 834U
#define PCLK_VOP_ROOT_GATE 835U
#define ACLK_VOP_NIU_GATE 836U
#define ACLK_VOP_LOW_NIU_GATE 837U
#define HCLK_VOP_NIU_GATE 838U
#define PCLK_VOP_NIU_GATE 839U
#define HCLK_VOP_GATE 840U
#define ACLK_VOP_GATE 841U
#define DCLK_VOP0_SRC_GATE 842U
#define DCLK_VOP1_SRC_GATE 843U
#define DCLK_VOP2_SRC_GATE 844U
#define DCLK_VOP0_GATE 845U
/********Name=GATE_CON53,Offset=0x8D4********/
#define DCLK_VOP1_GATE 848U
#define DCLK_VOP2_GATE 849U
#define DCLK_VOP3_GATE 850U
#define PCLK_VOPGRF_GATE 851U
#define PCLK_DSIHOST0_GATE 852U
#define PCLK_DSIHOST1_GATE 853U
#define CLK_DSIHOST0_GATE 854U
#define CLK_DSIHOST1_GATE 855U
#define CLK_VOP_PMU_GATE 856U
#define PCLK_VOP_CHANNEL_NIU_GATE 857U
#define ACLK_VOP_DOBY_GATE 858U
#define MBIST_CLK_DCLK_VOP0_GATE 859U
#define MBIST_CLK_DCLK_VOP1_GATE 860U
#define MBIST_CLK_DCLK_VOP2_GATE 861U
#define MBIST_CLK_DCLK_VOP3_GATE 862U
#define MBIST_CLK_ACLK_VOP_GATE 863U
/********Name=GATE_CON54,Offset=0x8D8********/
#define MBIST_CLK_CLK_DSIHOST0_GATE 864U
#define MBIST_CLK_CLK_DSIHOST1_GATE 865U
#define MBIST_CLK_VOP_DSC_GATE 866U
/********Name=GATE_CON55,Offset=0x8DC********/
#define ACLK_VO0_ROOT_GATE 880U
#define HCLK_VO0_ROOT_GATE 881U
#define HCLK_VO0_S_ROOT_GATE 882U
#define PCLK_VO0_ROOT_GATE 883U
#define PCLK_VO0_S_ROOT_GATE 884U
#define HCLK_VO0_NIU_GATE 885U
#define HCLK_VO0_S_NIU_GATE 886U
#define PCLK_VO0_NIU_GATE 887U
#define PCLK_VO0_S_NIU_GATE 888U
#define ACLK_HDCP0_NIU_GATE 889U
#define PCLK_VO0GRF_GATE 890U
#define HCLK_HDCP_KEY0_GATE 891U
#define ACLK_HDCP0_GATE 892U
#define HCLK_HDCP0_GATE 893U
#define PCLK_HDCP0_GATE 894U
/********Name=GATE_CON56,Offset=0x8E0********/
#define ACLK_TRNG0_GATE 896U
#define PCLK_TRNG0_GATE 897U
#define CLK_AUX16MHZ_0_GATE 898U
#define CLK_AUX16MHZ_1_GATE 899U
#define PCLK_DP0_GATE 900U
#define PCLK_DP1_GATE 901U
#define PCLK_S_DP0_GATE 902U
#define PCLK_S_DP1_GATE 903U
#define CLK_DP0_GATE 904U
#define CLK_DP1_GATE 905U
#define HCLK_I2S4_8CH_GATE 906U
#define CLK_I2S4_8CH_TX_GATE 907U
#define CLK_I2S4_8CH_FRAC_TX_GATE 908U
#define MCLK_I2S4_8CH_TX_GATE 909U
#define HCLK_I2S8_8CH_GATE 910U
#define CLK_I2S8_8CH_TX_GATE 911U
/********Name=GATE_CON57,Offset=0x8E4********/
#define CLK_I2S8_8CH_FRAC_TX_GATE 912U
#define MCLK_I2S8_8CH_TX_GATE 913U
#define HCLK_SPDIF2_DP0_GATE 914U
#define CLK_SPDIF2_DP0_GATE 915U
#define CLK_SPDIF2_DP0_FRAC_GATE 916U
#define MCLK_SPDIF2_DP0_GATE 917U
#define MCLK_SPDIF2_GATE 918U
#define HCLK_SPDIF5_DP1_GATE 919U
#define CLK_SPDIF5_DP1_GATE 920U
#define CLK_SPDIF5_DP1_FRAC_GATE 921U
#define MCLK_SPDIF5_DP1_GATE 922U
#define MCLK_SPDIF5_GATE 923U
#define MBIST_CLK_CLK_DP0_600M_GATE 924U
#define MBIST_CLK_CLK_DP0_300M_GATE 925U
#define MBIST_CLK_CLK_DP0_100M_GATE 926U
#define MBIST_CLK_CLK_DP1_600M_GATE 927U
/********Name=GATE_CON58,Offset=0x8E8********/
#define MBIST_CLK_CLK_DP1_300M_GATE 928U
#define MBIST_CLK_CLK_DP1_100M_GATE 929U
#define MBIST_CLK_ACLK_HDCP0_GATE 930U
/********Name=GATE_CON59,Offset=0x8EC********/
#define ACLK_HDCP1_ROOT_GATE 944U
#define ACLK_HDMIRX_ROOT_GATE 945U
#define HCLK_VO1_ROOT_GATE 946U
#define HCLK_VO1_S_ROOT_GATE 947U
#define PCLK_VO1_ROOT_GATE 948U
#define PCLK_VO1_S_ROOT_GATE 949U
#define ACLK_HDCP1_NIU_GATE 950U
#define ACLK_HDMIRX_NIU_GATE 951U
#define ACLK_VO1_NIU_GATE 952U
#define HCLK_VO1_NIU_GATE 953U
#define HCLK_VO1_S_NIU_GATE 954U
#define PCLK_VO1_NIU_GATE 955U
#define PCLK_VO1GRF_GATE 956U
#define PCLK_VO1_S_NIU_GATE 957U
#define PCLK_S_EDP0_GATE 958U
#define PCLK_S_EDP1_GATE 959U
/********Name=GATE_CON60,Offset=0x8F0********/
#define HCLK_I2S7_8CH_GATE 960U
#define CLK_I2S7_8CH_RX_GATE 961U
#define CLK_I2S7_8CH_FRAC_RX_GATE 962U
#define MCLK_I2S7_8CH_RX_GATE 963U
#define HCLK_HDCP_KEY1_GATE 964U
#define ACLK_HDCP1_GATE 965U
#define HCLK_HDCP1_GATE 966U
#define PCLK_HDCP1_GATE 967U
#define ACLK_TRNG1_GATE 969U
#define PCLK_TRNG1_GATE 970U
#define PCLK_HDMITX0_GATE 971U
#define CLK_HDMITX0_EARC_GATE 975U
/********Name=GATE_CON61,Offset=0x8F4********/
#define CLK_HDMITX0_REF_GATE 976U
#define PCLK_HDMITX1_GATE 978U
#define CLK_HDMITX1_EARC_GATE 982U
#define CLK_HDMITX1_REF_GATE 983U
#define ACLK_HDMIRX_GATE 985U
#define PCLK_HDMIRX_GATE 986U
#define CLK_HDMIRX_REF_GATE 987U
#define CLK_HDMIRX_AUD_SRC_GATE 988U
#define CLK_HDMIRX_AUD_FRAC_GATE 989U
#define CLK_HDMIRX_AUD_GATE 990U
#define CLK_HDMIRX_TMDSQP_GATE 991U
/********Name=GATE_CON62,Offset=0x8F8********/
#define PCLK_EDP0_GATE 992U
#define CLK_EDP0_24M_GATE 993U
#define CLK_EDP0_200M_GATE 994U
#define PCLK_EDP1_GATE 995U
#define CLK_EDP1_24M_GATE 996U
#define CLK_EDP1_200M_GATE 997U
#define CLK_I2S5_8CH_TX_GATE 998U
#define CLK_I2S5_8CH_FRAC_TX_GATE 999U
#define MCLK_I2S5_8CH_TX_GATE 1000U
#define HCLK_I2S5_8CH_GATE 1004U
#define CLK_I2S6_8CH_TX_GATE 1005U
#define CLK_I2S6_8CH_FRAC_TX_GATE 1006U
#define MCLK_I2S6_8CH_TX_GATE 1007U
/********Name=GATE_CON63,Offset=0x8FC********/
#define CLK_I2S6_8CH_RX_GATE 1008U
#define CLK_I2S6_8CH_FRAC_RX_GATE 1009U
#define MCLK_I2S6_8CH_RX_GATE 1010U
#define HCLK_I2S6_8CH_GATE 1011U
#define HCLK_SPDIF3_GATE 1012U
#define CLK_SPDIF3_GATE 1013U
#define CLK_SPDIF3_FRAC_GATE 1014U
#define MCLK_SPDIF3_GATE 1015U
#define HCLK_SPDIF4_GATE 1016U
#define CLK_SPDIF4_GATE 1017U
#define CLK_SPDIF4_FRAC_GATE 1018U
#define MCLK_SPDIF4_GATE 1019U
#define HCLK_SPDIFRX0_GATE 1020U
#define MCLK_SPDIFRX0_GATE 1021U
#define HCLK_SPDIFRX1_GATE 1022U
#define MCLK_SPDIFRX1_GATE 1023U
/********Name=GATE_CON64,Offset=0x900********/
#define HCLK_SPDIFRX2_GATE 1024U
#define MCLK_SPDIFRX2_GATE 1025U
#define MBIST_CLK_ACLK_HDCP1_GATE 1026U
#define MBIST_CLK_CLK_HDMITX0_VID_GATE 1027U
#define MBIST_CLK_CLK_HDMITX0_LINK_GATE 1028U
#define MBIST_CLK_CLK_HDMITX1_VID_GATE 1029U
#define MBIST_CLK_CLK_HDMITX1_LINK_GATE 1030U
#define MBIST_CLK_CLK_HDMIRX_TMDSQP_GATE 1031U
#define MBIST_CLK_CLK_HDMIRX_REF_GATE 1032U
#define MBIST_CLK_ACLK_HDMIRX_GATE 1033U
#define MBIST_CLK_PCLK_EDP0_GATE 1034U
#define MBIST_CLK_PCLK_EDP1_GATE 1035U
#define DCLK_VOP2HDMI_BRIDGE0_VO1_GATE 1038U
#define DCLK_VOP2HDMI_BRIDGE1_VO1_GATE 1039U
/********Name=GATE_CON65,Offset=0x904********/
#define HCLK_I2S9_8CH_GATE 1040U
#define CLK_I2S9_8CH_RX_GATE 1041U
#define CLK_I2S9_8CH_FRAC_RX_GATE 1042U
#define MCLK_I2S9_8CH_RX_GATE 1043U
#define HCLK_I2S10_8CH_GATE 1044U
#define CLK_I2S10_8CH_RX_GATE 1045U
#define CLK_I2S10_8CH_FRAC_RX_GATE 1046U
#define MCLK_I2S10_8CH_RX_GATE 1047U
#define PCLK_S_HDMIRX_GATE 1048U
#define CLK_HDMITRX_REFSRC_GATE 1049U
/********Name=GATE_CON66,Offset=0x908********/
#define CLK_GPU_SRC_DF_GATE 1057U
#define CLK_TESTOUT_GPU_GATE 1058U
#define CLK_GPU_SRC_GATE 1059U
#define CLK_GPU_GATE 1060U
#define CLK_GPU_COREGROUP_GATE 1062U
#define CLK_GPU_STACKS_GATE 1063U
#define ACLK_S_GPU_NIU_GATE 1064U
#define ACLK_M0_GPU_NIU_GATE 1065U
#define ACLK_M1_GPU_NIU_GATE 1066U
#define ACLK_M2_GPU_NIU_GATE 1067U
#define ACLK_M3_GPU_NIU_GATE 1068U
#define PCLK_GPU_ROOT_GATE 1069U
#define PCLK_GPU_NIU_GATE 1070U
#define PCLK_PVTM2_GATE 1071U
/********Name=GATE_CON67,Offset=0x90C********/
#define CLK_PVTM2_GATE 1072U
#define CLK_GPU_PVTM_GATE 1073U
#define PCLK_GPU_GRF_GATE 1074U
#define CLK_GPU_PVTPLL_GATE 1075U
/********Name=GATE_CON68,Offset=0x910********/
#define ACLK_AV1_ROOT_GATE 1088U
#define ACLK_AV1_NIU_GATE 1089U
#define ACLK_AV1_GATE 1090U
#define PCLK_AV1_ROOT_GATE 1091U
#define PCLK_AV1_NIU_GATE 1092U
#define PCLK_AV1_GATE 1093U
/********Name=GATE_CON69,Offset=0x914********/
#define ACLK_CENTER_ROOT_GATE 1104U
#define ACLK_CENTER_LOW_ROOT_GATE 1105U
#define HCLK_CENTER_ROOT_GATE 1106U
#define PCLK_CENTER_ROOT_GATE 1107U
#define ACLK_DDR_NIU_GATE 1108U
#define ACLK_DMA2DDR_GATE 1109U
#define ACLK_DDR_SHAREMEM_GATE 1110U
#define ACLK_DDR_SHAREMEM_NIU_GATE 1111U
#define ACLK_CENTER_S200_ROOT_GATE 1112U
#define ACLK_CENTER_S400_ROOT_GATE 1113U
#define ACLK_CENTER_S200_NIU_GATE 1114U
#define ACLK_CENTER_S400_NIU_GATE 1115U
#define HCLK_AHB2APB_GATE 1116U
#define HCLK_CENTER_NIU_GATE 1117U
#define FCLK_DDR_CM0_CORE_GATE 1118U
#define CLK_DDR_TIMER_ROOT_GATE 1119U
/********Name=GATE_CON70,Offset=0x918********/
#define CLK_DDR_TIMER0_GATE 1120U
#define CLK_DDR_TIMER1_GATE 1121U
#define TCLK_WDT_DDR_GATE 1122U
#define CLK_DDR_CM0_RTC_GATE 1124U
#define PCLK_CENTER_GRF_GATE 1125U
#define PCLK_AHB2APB_GATE 1126U
#define PCLK_WDT_GATE 1127U
#define PCLK_TIMER_GATE 1128U
#define PCLK_DMA2DDR_GATE 1129U
#define PCLK_SHAREMEM_GATE 1130U
#define PCLK_CENTER_NIU_GATE 1131U
#define PCLK_CENTER_CHANNEL_NIU_GATE 1132U
#define MBIST_CLK_CLK_DDR_CM0_CORE_GATE 1133U
#define MBIST_CLK_SCLK_DDR_SHAREMEM_GATE 1134U
#define MBIST_CLK_ACLK_DMA2DDR_GATE 1135U
/********Name=GATE_CON71,Offset=0x91C********/
#define MBIST_CLK_ACLK_NSPI_GATE 1136U
/********Name=GATE_CON72,Offset=0x920********/
#define PCLK_USBDPGRF0_GATE 1153U
#define PCLK_USBDPPHY0_GATE 1154U
#define PCLK_USBDPGRF1_GATE 1155U
#define PCLK_USBDPPHY1_GATE 1156U
#define PCLK_HDPTX0_GATE 1157U
#define PCLK_HDPTX1_GATE 1158U
#define PCLK_APB2ASB_SLV_BOT_RIGHT_GATE 1159U
#define PCLK_USB2PHY_U3_0_GRF0_GATE 1160U
#define PCLK_USB2PHY_U3_1_GRF0_GATE 1161U
#define PCLK_USB2PHY_U2_0_GRF0_GATE 1162U
#define PCLK_USB2PHY_U2_1_GRF0_GATE 1163U
/********Name=GATE_CON73,Offset=0x924********/
#define MBIST_CLK_HDMIRXPHY_GATE 1179U
#define CLK_HDMIHDP0_GATE 1180U
#define CLK_HDMIHDP1_GATE 1181U
/********Name=GATE_CON74,Offset=0x928********/
#define ACLK_VO1USB_TOP_ROOT_GATE 1184U
#define ACLK_VO1USB_TOP_NIU_GATE 1185U
#define HCLK_VO1USB_TOP_ROOT_GATE 1186U
#define HCLK_VO1USB_TOP_NIU_GATE 1187U
/********Name=GATE_CON75,Offset=0x92C********/
#define HCLK_SDIO_ROOT_GATE 1200U
#define HCLK_SDIO_NIU_GATE 1201U
#define HCLK_SDIO_GATE 1202U
#define CCLK_SRC_SDIO_GATE 1203U
#define MBIST_CLK_SDIO_GATE 1204U
/********Name=GATE_CON76,Offset=0x930********/
#define ACLK_RGA3_ROOT_GATE 1216U
#define HCLK_RGA3_ROOT_GATE 1217U
#define HCLK_RGA3_NIU_GATE 1218U
#define ACLK_RGA3_NIU_GATE 1219U
#define HCLK_RGA3_1_GATE 1220U
#define ACLK_RGA3_1_GATE 1221U
#define CLK_RGA3_1_CORE_GATE 1222U
#define MBIST_CLK_ACLK_RGA3_1_GATE 1223U
#define MBIST_CLK_CLK_RGA3_1_CORE_GATE 1224U
/********Name=GATE_CON77,Offset=0x934********/
#define CLK_REF_PIPE_PHY0_OSC_SRC_GATE 1232U
#define CLK_REF_PIPE_PHY1_OSC_SRC_GATE 1233U
#define CLK_REF_PIPE_PHY2_OSC_SRC_GATE 1234U
#define CLK_REF_PIPE_PHY0_PLL_SRC_GATE 1235U
#define CLK_REF_PIPE_PHY1_PLL_SRC_GATE 1236U
#define CLK_REF_PIPE_PHY2_PLL_SRC_GATE 1237U

/********Name=PMU1GATE_CON00,Offset=0x800********/
#define CLK_MATRIX_PMU1_50M_SRC_GATE 16U
#define CLK_MATRIX_PMU1_100M_SRC_GATE 17U
#define CLK_MATRIX_PMU1_200M_SRC_GATE 18U
#define CLK_MATRIX_PMU1_300M_SRC_GATE 19U
#define CLK_MATRIX_PMU1_400M_SRC_GATE 20U
#define HCLK_PMU1_ROOT_I_GATE 21U
#define HCLK_PMU1_ROOT_GATE 22U
#define PCLK_PMU1_ROOT_I_GATE 23U
#define HCLK_PMU_CM0_ROOT_I_GATE 24U
#define HCLK_PMU_CM0_ROOT_GATE 25U
#define HCLK_PMU1_NIU_GATE 26U
#define PCLK_PMU1_NIU_GATE 27U
#define HCLK_PMU_CM0_NIU_GATE 28U
#define FCLK_PMU_CM0_CORE_GATE 29U
#define CLK_PMU_CM0_RTC_GATE 31U
/********Name=PMU1GATE_CON01,Offset=0x804********/
#define PCLK_PMU1_GATE 16U
#define CLK_DDR_FAIL_SAFE_GATE 17U
#define PCLK_PMU1_CRU_GATE 18U
#define CLK_PMU1_GATE 19U
#define PCLK_PMU1_GRF_GATE 20U
#define PCLK_PMU1_IOC_GATE 21U
#define PCLK_PMU1WDT_GATE 22U
#define TCLK_PMU1WDT_GATE 23U
#define PCLK_PMU1TIMER_GATE 24U
#define CLK_PMU1TIMER_ROOT_GATE 25U
#define CLK_PMU1TIMER0_GATE 26U
#define CLK_PMU1TIMER1_GATE 27U
#define PCLK_PMU1PWM_GATE 28U
#define CLK_PMU1PWM_GATE 29U
#define CLK_PMU1PWM_CAPTURE_GATE 30U
/********Name=PMU1GATE_CON02,Offset=0x808********/
#define PCLK_I2C0_GATE 17U
#define CLK_I2C0_GATE 18U
#define CLK_UART0_GATE 19U
#define CLK_UART0_FRAC_GATE 20U
#define SCLK_UART0_GATE 21U
#define PCLK_UART0_GATE 22U
#define HCLK_I2S1_8CH_GATE 23U
#define CLK_I2S1_8CH_TX_GATE 24U
#define CLK_I2S1_8CH_FRAC_TX_GATE 25U
#define MCLK_I2S1_8CH_TX_GATE 26U
#define CLK_I2S1_8CH_RX_GATE 27U
#define CLK_I2S1_8CH_FRAC_RX_GATE 28U
#define MCLK_I2S1_8CH_RX_GATE 29U
#define HCLK_PDM0_GATE 30U
#define MCLK_PDM0_GATE 31U
/********Name=PMU1GATE_CON03,Offset=0x80C********/
#define HCLK_VAD_GATE 16U
#define CLK_USBDP_COMBO_PHY0_REF_XTAL_GATE 21U
#define CLK_HDPTX0_REF_XTAL_GATE 27U
/********Name=PMU1GATE_CON04,Offset=0x810********/
#define CLK_REF_MIPI_DCPHY0_GATE 19U
#define CLK_OTGPHY_U3_0_GATE 23U
#define CLK_CR_PARA_GATE 27U
/********Name=PMU1GATE_CON05,Offset=0x814********/
#define PCLK_PMU0_ROOT_GATE 16U
#define CLK_PMU0_GATE 17U
#define PCLK_PMU0_GATE 18U
#define PCLK_PMU0GRF_GATE 19U
#define PCLK_PMU0IOC_GATE 20U
#define PCLK_GPIO0_GATE 21U
#define DBCLK_GPIO0_GATE 22U

/********Name=CLKSEL_CON00,Offset=0x300********/
#define CLK_MATRIX_50M_SRC_DIV 0x05000000U
#define CLK_MATRIX_100M_SRC_DIV 0x05060000U
/********Name=CLKSEL_CON01,Offset=0x304********/
#define CLK_MATRIX_150M_SRC_DIV 0x05000001U
#define CLK_MATRIX_200M_SRC_DIV 0x05060001U
/********Name=CLKSEL_CON02,Offset=0x308********/
#define CLK_MATRIX_250M_SRC_DIV 0x05000002U
#define CLK_MATRIX_300M_SRC_DIV 0x05060002U
/********Name=CLKSEL_CON03,Offset=0x30C********/
#define CLK_MATRIX_350M_SRC_DIV 0x05000003U
#define CLK_MATRIX_400M_SRC_DIV 0x05060003U
/********Name=CLKSEL_CON04,Offset=0x310********/
#define CLK_MATRIX_450M_SRC_DIV 0x05000004U
#define CLK_MATRIX_500M_SRC_DIV 0x05060004U
/********Name=CLKSEL_CON05,Offset=0x314********/
#define CLK_MATRIX_600M_SRC_DIV 0x05000005U
#define CLK_MATRIX_650M_SRC_DIV 0x05060005U
/********Name=CLKSEL_CON06,Offset=0x318********/
#define CLK_MATRIX_700M_SRC_DIV 0x05000006U
#define CLK_MATRIX_800M_SRC_DIV 0x05060006U
/********Name=CLKSEL_CON07,Offset=0x31C********/
#define CLK_MATRIX_1000M_SRC_DIV 0x05000007U
#define CLK_MATRIX_1200M_SRC_DIV 0x05070007U
/********Name=CLKSEL_CON08,Offset=0x320********/
#define ACLK_TOP_ROOT_DIV 0x05000008U
#define ACLK_LOW_TOP_ROOT_DIV 0x05090008U
/********Name=CLKSEL_CON09,Offset=0x324********/
/********Name=CLKSEL_CON10,Offset=0x328********/
#define CLK_TESTOUT_TOP_DIV 0x0600000AU
/********Name=CLKSEL_CON100,Offset=0x490********/
#define CLK_RGA2_CORE_DIV 0x05000064U
#define CLK_RGA3_0_CORE_DIV 0x05080064U
/********Name=CLKSEL_CON102,Offset=0x498********/
#define ACLK_RKVENC0_ROOT_DIV 0x05020066U
#define CLK_RKVENC0_CORE_DIV 0x05090066U
/********Name=CLKSEL_CON104,Offset=0x4A0********/
#define ACLK_RKVENC1_ROOT_DIV 0x05020068U
#define CLK_RKVENC1_CORE_DIV 0x05090068U
/********Name=CLKSEL_CON106,Offset=0x4A8********/
#define ACLK_VI_ROOT_DIV 0x0500006AU
/********Name=CLKSEL_CON107,Offset=0x4AC********/
#define DCLK_VICAP_DIV 0x0500006BU
#define CLK_ISP0_CORE_DIV 0x0506006BU
/********Name=CLKSEL_CON108,Offset=0x4B0********/
#define CLK_FISHEYE0_CORE_DIV 0x0500006CU
#define CLK_FISHEYE1_CORE_DIV 0x0507006CU
/********Name=CLKSEL_CON110,Offset=0x4B8********/
#define ACLK_VOP_ROOT_DIV 0x0500006EU
/********Name=CLKSEL_CON111,Offset=0x4BC********/
#define DCLK_VOP0_SRC_DIV 0x0700006FU
#define DCLK_VOP1_SRC_DIV 0x0509006FU
/********Name=CLKSEL_CON112,Offset=0x4C0********/
#define DCLK_VOP2_SRC_DIV 0x05000070U
/********Name=CLKSEL_CON113,Offset=0x4C4********/
#define DCLK_VOP3_DIV 0x07000071U
/********Name=CLKSEL_CON114,Offset=0x4C8********/
#define CLK_DSIHOST0_DIV 0x07000072U
/********Name=CLKSEL_CON115,Offset=0x4CC********/
#define CLK_DSIHOST1_DIV 0x07000073U
/********Name=CLKSEL_CON116,Offset=0x4D0********/
#define ACLK_VO0_ROOT_DIV 0x05000074U
/********Name=CLKSEL_CON117,Offset=0x4D4********/
#define CLK_AUX16MHZ_0_DIV 0x08000075U
#define CLK_AUX16MHZ_1_DIV 0x08080075U
/********Name=CLKSEL_CON118,Offset=0x4D8********/
#define CLK_I2S4_8CH_TX_SRC_DIV 0x05000076U
/********Name=CLKSEL_CON119,Offset=0x4DC********/
#define CLK_I2S4_8CH_TX_FRAC_DIV 0x20000077U
/********Name=CLKSEL_CON120,Offset=0x4E0********/
#define CLK_I2S8_8CH_TX_SRC_DIV 0x05030078U
/********Name=CLKSEL_CON121,Offset=0x4E4********/
#define CLK_I2S8_8CH_TX_FRAC_DIV 0x20000079U
/********Name=CLKSEL_CON122,Offset=0x4E8********/
#define CLK_SPDIF2_DP0_SRC_DIV 0x0503007AU
/********Name=CLKSEL_CON123,Offset=0x4EC********/
#define CLK_SPDIF2_DP0_FRAC_DIV 0x2000007BU
/********Name=CLKSEL_CON124,Offset=0x4F0********/
#define CLK_SPDIF5_DP1_SRC_DIV 0x0502007CU
/********Name=CLKSEL_CON125,Offset=0x4F4********/
#define CLK_SPDIF5_DP1_FRAC_DIV 0x2000007DU
/********Name=CLKSEL_CON126,Offset=0x4F8********/
/********Name=CLKSEL_CON128,Offset=0x500********/
#define ACLK_HDCP1_ROOT_DIV 0x05000080U
#define ACLK_HDMIRX_ROOT_DIV 0x05070080U
/********Name=CLKSEL_CON129,Offset=0x504********/
#define CLK_I2S7_8CH_RX_SRC_DIV 0x05060081U
/********Name=CLKSEL_CON130,Offset=0x508********/
#define CLK_I2S7_8CH_RX_FRAC_DIV 0x20000082U
/********Name=CLKSEL_CON131,Offset=0x50C********/
/********Name=CLKSEL_CON133,Offset=0x514********/
#define CLK_HDMITX0_EARC_DIV 0x05010085U
/********Name=CLKSEL_CON136,Offset=0x520********/
#define CLK_HDMITX1_EARC_DIV 0x05010088U
/********Name=CLKSEL_CON138,Offset=0x528********/
#define CLK_HDMIRX_AUD_SRC_DIV 0x0800008AU
/********Name=CLKSEL_CON139,Offset=0x52C********/
#define CLK_HDMIRX_AUD_FRAC_DIV 0x2000008BU
/********Name=CLKSEL_CON140,Offset=0x530********/
#define CLK_I2S5_8CH_TX_SRC_DIV 0x0505008CU
/********Name=CLKSEL_CON141,Offset=0x534********/
#define CLK_I2S5_8CH_TX_FRAC_DIV 0x2000008DU
/********Name=CLKSEL_CON142,Offset=0x538********/
/********Name=CLKSEL_CON144,Offset=0x540********/
#define CLK_I2S6_8CH_TX_SRC_DIV 0x05030090U
/********Name=CLKSEL_CON145,Offset=0x544********/
#define CLK_I2S6_8CH_TX_FRAC_DIV 0x20000091U
/********Name=CLKSEL_CON146,Offset=0x548********/
#define CLK_I2S6_8CH_RX_SRC_DIV 0x05020092U
/********Name=CLKSEL_CON147,Offset=0x54C********/
#define CLK_I2S6_8CH_RX_FRAC_DIV 0x20000093U
/********Name=CLKSEL_CON148,Offset=0x550********/
#define CLK_SPDIF3_SRC_DIV 0x05040094U
/********Name=CLKSEL_CON149,Offset=0x554********/
#define CLK_SPDIF3_FRAC_DIV 0x20000095U
/********Name=CLKSEL_CON15,Offset=0x33C********/
#define MCLK_GMAC0_OUT_DIV 0x0700000FU
#define REFCLKO25M_ETH0_OUT_DIV 0x0708000FU
/********Name=CLKSEL_CON150,Offset=0x558********/
#define CLK_SPDIF4_SRC_DIV 0x05020096U
/********Name=CLKSEL_CON151,Offset=0x55C********/
#define CLK_SPDIF4_FRAC_DIV 0x20000097U
/********Name=CLKSEL_CON152,Offset=0x560********/
#define MCLK_SPDIFRX0_DIV 0x05020098U
#define MCLK_SPDIFRX1_DIV 0x05090098U
/********Name=CLKSEL_CON153,Offset=0x564********/
#define MCLK_SPDIFRX2_DIV 0x05000099U
#define CLK_I2S9_8CH_RX_SRC_DIV 0x05070099U
/********Name=CLKSEL_CON154,Offset=0x568********/
#define CLK_I2S9_8CH_RX_FRAC_DIV 0x2000009AU
/********Name=CLKSEL_CON155,Offset=0x56C********/
#define CLK_I2S10_8CH_RX_SRC_DIV 0x0503009BU
/********Name=CLKSEL_CON156,Offset=0x570********/
#define CLK_I2S10_8CH_RX_FRAC_DIV 0x2000009CU
/********Name=CLKSEL_CON157,Offset=0x574********/
#define CLK_HDMITRX_REFSRC_DIV 0x0502009DU
/********Name=CLKSEL_CON158,Offset=0x578********/
#define CLK_GPU_SRC_T_DIV 0x0500009EU
#define CLK_TESTOUT_GPU_DIV 0x0508009EU
/********Name=CLKSEL_CON159,Offset=0x57C********/
#define CLK_GPU_STACKS_DIV 0x0500009FU
#define ACLK_S_GPU_NIU_DIV 0x0505009FU
#define ACLK_M0_GPU_NIU_DIV 0x050A009FU
/********Name=CLKSEL_CON16,Offset=0x340********/
#define REFCLKO25M_ETH1_OUT_DIV 0x07000010U
/********Name=CLKSEL_CON160,Offset=0x580********/
#define ACLK_M1_GPU_NIU_DIV 0x050000A0U
#define ACLK_M2_GPU_NIU_DIV 0x050500A0U
#define ACLK_M3_GPU_NIU_DIV 0x050A00A0U
/********Name=CLKSEL_CON161,Offset=0x584********/
/********Name=CLKSEL_CON163,Offset=0x58C********/
#define ACLK_AV1_ROOT_DIV 0x050000A3U
/********Name=CLKSEL_CON165,Offset=0x594********/
/********Name=CLKSEL_CON166,Offset=0x598********/
#define CLK_DDR_CM0_RTC_DIV 0x050000A6U
/********Name=CLKSEL_CON17,Offset=0x344********/
#define CLK_CIFOUT_OUT_DIV 0x08000011U
/********Name=CLKSEL_CON170,Offset=0x5A8********/
#define ACLK_VO1USB_TOP_ROOT_DIV 0x050000AAU
/********Name=CLKSEL_CON172,Offset=0x5B0********/
#define CCLK_SRC_SDIO_DIV 0x060200ACU
/********Name=CLKSEL_CON174,Offset=0x5B8********/
#define ACLK_RGA3_ROOT_DIV 0x050000AEU
#define CLK_RGA3_1_CORE_DIV 0x050900AEU
/********Name=CLKSEL_CON176,Offset=0x5C0********/
#define CLK_REF_PIPE_PHY0_PLL_SRC_DIV 0x060000B0U
#define CLK_REF_PIPE_PHY1_PLL_SRC_DIV 0x060600B0U
/********Name=CLKSEL_CON177,Offset=0x5C4********/
#define CLK_REF_PIPE_PHY2_PLL_SRC_DIV 0x060000B1U
/********Name=CLKSEL_CON18,Offset=0x348********/
#define CLK_MIPI_CAMARAOUT_M0_DIV 0x08000012U
/********Name=CLKSEL_CON19,Offset=0x34C********/
#define CLK_MIPI_CAMARAOUT_M1_DIV 0x08000013U
/********Name=CLKSEL_CON20,Offset=0x350********/
#define CLK_MIPI_CAMARAOUT_M2_DIV 0x08000014U
/********Name=CLKSEL_CON21,Offset=0x354********/
#define CLK_MIPI_CAMARAOUT_M3_DIV 0x08000015U
/********Name=CLKSEL_CON22,Offset=0x358********/
#define CLK_MIPI_CAMARAOUT_M4_DIV 0x08000016U
/********Name=CLKSEL_CON24,Offset=0x360********/
#define CLK_I2S0_8CH_TX_SRC_DIV 0x05040018U
/********Name=CLKSEL_CON25,Offset=0x364********/
#define CLK_I2S0_8CH_TX_FRAC_DIV 0x20000019U
/********Name=CLKSEL_CON26,Offset=0x368********/
#define CLK_I2S0_8CH_RX_SRC_DIV 0x0502001AU
/********Name=CLKSEL_CON27,Offset=0x36C********/
#define CLK_I2S0_8CH_RX_FRAC_DIV 0x2000001BU
/********Name=CLKSEL_CON28,Offset=0x370********/
#define CLK_I2S2_2CH_SRC_DIV 0x0504001CU
/********Name=CLKSEL_CON29,Offset=0x374********/
#define CLK_I2S2_2CH_FRAC_DIV 0x2000001DU
/********Name=CLKSEL_CON30,Offset=0x378********/
#define CLK_I2S3_2CH_SRC_DIV 0x0503001EU
/********Name=CLKSEL_CON31,Offset=0x37C********/
#define CLK_I2S3_2CH_FRAC_DIV 0x2000001FU
/********Name=CLKSEL_CON32,Offset=0x380********/
#define CLK_SPDIF0_SRC_DIV 0x05030020U
/********Name=CLKSEL_CON33,Offset=0x384********/
#define CLK_SPDIF0_FRAC_DIV 0x20000021U
/********Name=CLKSEL_CON34,Offset=0x388********/
#define CLK_SPDIF1_SRC_DIV 0x05020022U
/********Name=CLKSEL_CON35,Offset=0x38C********/
#define CLK_SPDIF1_FRAC_DIV 0x20000023U
/********Name=CLKSEL_CON36,Offset=0x390********/
#define MCLK_PDM1_DIV 0x05020024U
/********Name=CLKSEL_CON38,Offset=0x398********/
#define ACLK_BUS_ROOT_DIV 0x05000026U
/********Name=CLKSEL_CON39,Offset=0x39C********/
#define CLK_CAN0_DIV 0x05000027U
#define CLK_CAN1_DIV 0x05060027U
/********Name=CLKSEL_CON40,Offset=0x3A0********/
#define CLK_CAN2_DIV 0x05000028U
#define CLK_SARADC_DIV 0x08060028U
/********Name=CLKSEL_CON41,Offset=0x3A4********/
#define CLK_TSADC_DIV 0x08000029U
#define CLK_UART1_SRC_DIV 0x05090029U
/********Name=CLKSEL_CON42,Offset=0x3A8********/
#define CLK_UART1_FRAC_DIV 0x2000002AU
/********Name=CLKSEL_CON43,Offset=0x3AC********/
#define CLK_UART2_SRC_DIV 0x0502002BU
/********Name=CLKSEL_CON44,Offset=0x3B0********/
#define CLK_UART2_FRAC_DIV 0x2000002CU
/********Name=CLKSEL_CON45,Offset=0x3B4********/
#define CLK_UART3_SRC_DIV 0x0502002DU
/********Name=CLKSEL_CON46,Offset=0x3B8********/
#define CLK_UART3_FRAC_DIV 0x2000002EU
/********Name=CLKSEL_CON47,Offset=0x3BC********/
#define CLK_UART4_SRC_DIV 0x0502002FU
/********Name=CLKSEL_CON48,Offset=0x3C0********/
#define CLK_UART4_FRAC_DIV 0x20000030U
/********Name=CLKSEL_CON49,Offset=0x3C4********/
#define CLK_UART5_SRC_DIV 0x05020031U
/********Name=CLKSEL_CON50,Offset=0x3C8********/
#define CLK_UART5_FRAC_DIV 0x20000032U
/********Name=CLKSEL_CON51,Offset=0x3CC********/
#define CLK_UART6_SRC_DIV 0x05020033U
/********Name=CLKSEL_CON52,Offset=0x3D0********/
#define CLK_UART6_FRAC_DIV 0x20000034U
/********Name=CLKSEL_CON53,Offset=0x3D4********/
#define CLK_UART7_SRC_DIV 0x05020035U
/********Name=CLKSEL_CON54,Offset=0x3D8********/
#define CLK_UART7_FRAC_DIV 0x20000036U
/********Name=CLKSEL_CON55,Offset=0x3DC********/
#define CLK_UART8_SRC_DIV 0x05020037U
/********Name=CLKSEL_CON56,Offset=0x3E0********/
#define CLK_UART8_FRAC_DIV 0x20000038U
/********Name=CLKSEL_CON57,Offset=0x3E4********/
#define CLK_UART9_SRC_DIV 0x05020039U
/********Name=CLKSEL_CON58,Offset=0x3E8********/
#define CLK_UART9_FRAC_DIV 0x2000003AU
/********Name=CLKSEL_CON59,Offset=0x3EC********/
/********Name=CLKSEL_CON60,Offset=0x3F0********/
#define DBCLK_GPIO1_DIV 0x0503003CU
#define DBCLK_GPIO2_DIV 0x0509003CU
/********Name=CLKSEL_CON61,Offset=0x3F4********/
#define DBCLK_GPIO3_DIV 0x0500003DU
#define DBCLK_GPIO4_DIV 0x0506003DU
/********Name=CLKSEL_CON62,Offset=0x3F8********/
#define DCLK_DECOM_DIV 0x0500003EU
#define CLK_BISRINTF_PLLSRC_DIV 0x0506003EU
/********Name=CLKSEL_CON63,Offset=0x3FC********/
#define CLK_TESTOUT_DDR01_DIV 0x0600003FU
/********Name=CLKSEL_CON65,Offset=0x404********/
#define CLK_TESTOUT_DDR23_DIV 0x06000041U
/********Name=CLKSEL_CON67,Offset=0x40C********/
#define ACLK_ISP1_ROOT_DIV 0x05000043U
#define CLK_ISP1_CORE_DIV 0x05090043U
/********Name=CLKSEL_CON73,Offset=0x424********/
#define CLK_RKNN_DSU0_SRC_T_DIV 0x05020049U
#define CLK_TESTOUT_NPU_DIV 0x050A0049U
/********Name=CLKSEL_CON74,Offset=0x428********/
#define CLK_NPU_CM0_RTC_DIV 0x0507004AU
/********Name=CLKSEL_CON77,Offset=0x434********/
#define ACLK_NVM_ROOT_DIV 0x0502004DU
#define CCLK_EMMC_DIV 0x0608004DU
/********Name=CLKSEL_CON78,Offset=0x438********/
#define BCLK_EMMC_DIV 0x0500004EU
#define SCLK_SFC_DIV 0x0606004EU
/********Name=CLKSEL_CON80,Offset=0x440********/
#define ACLK_PCIE_ROOT_DIV 0x05020050U
#define ACLK_PHP_ROOT_DIV 0x05080050U
/********Name=CLKSEL_CON81,Offset=0x444********/
#define CLK_GMAC0_PTP_REF_DIV 0x06000051U
#define CLK_GMAC1_PTP_REF_DIV 0x06070051U
/********Name=CLKSEL_CON82,Offset=0x448********/
#define CLK_RXOOB0_DIV 0x07000052U
#define CLK_RXOOB1_DIV 0x07080052U
/********Name=CLKSEL_CON83,Offset=0x44C********/
#define CLK_RXOOB2_DIV 0x07000053U
#define CLK_GMAC_125M_CRU_I_DIV 0x07080053U
/********Name=CLKSEL_CON84,Offset=0x450********/
#define CLK_GMAC_50M_CRU_I_DIV 0x07000054U
#define CLK_UTMI_OTG2_DIV 0x04080054U
/********Name=CLKSEL_CON85,Offset=0x454********/
#define CLK_GMAC0_TX_125M_O_DIV 0x06000055U
#define CLK_GMAC1_TX_125M_O_DIV 0x06060055U
/********Name=CLKSEL_CON89,Offset=0x464********/
#define ACLK_RKVDEC0_ROOT_DIV 0x05020059U
#define ACLK_RKVDEC_CCU_DIV 0x05090059U
/********Name=CLKSEL_CON90,Offset=0x468********/
#define CLK_RKVDEC0_CA_DIV 0x0500005AU
#define CLK_RKVDEC0_HEVC_CA_DIV 0x0506005AU
/********Name=CLKSEL_CON91,Offset=0x46C********/
#define CLK_RKVDEC0_CORE_DIV 0x0500005BU
/********Name=CLKSEL_CON93,Offset=0x474********/
#define ACLK_RKVDEC1_ROOT_DIV 0x0502005DU
#define CLK_RKVDEC1_CA_DIV 0x0509005DU
/********Name=CLKSEL_CON94,Offset=0x478********/
#define CLK_RKVDEC1_HEVC_CA_DIV 0x0500005EU
#define CLK_RKVDEC1_CORE_DIV 0x0507005EU
/********Name=CLKSEL_CON96,Offset=0x480********/
#define ACLK_USB_ROOT_DIV 0x05000060U
/********Name=CLKSEL_CON98,Offset=0x488********/
#define ACLK_VDPU_ROOT_DIV 0x05000062U
/********Name=CLKSEL_CON99,Offset=0x48C********/
#define ACLK_JPEG_DECODER_ROOT_DIV 0x05000063U
#define CLK_IEP2P0_CORE_DIV 0x05070063U
/********Name=PMU1CLKSEL_CON00,Offset=0x300********/
#define CLK_MATRIX_PMU1_50M_SRC_DIV 0x04000001U
#define CLK_MATRIX_PMU1_100M_SRC_DIV 0x03040001U
#define CLK_MATRIX_PMU1_200M_SRC_DIV 0x03070001U
#define CLK_MATRIX_PMU1_300M_SRC_DIV 0x050A0001U
/********Name=PMU1CLKSEL_CON01,Offset=0x304********/
#define CLK_MATRIX_PMU1_400M_SRC_DIV 0x05000001U
/********Name=PMU1CLKSEL_CON02,Offset=0x308********/
#define CLK_PMU_CM0_RTC_DIV 0x05000001U
/********Name=PMU1CLKSEL_CON03,Offset=0x30C********/
#define CLK_UART0_SRC_DIV 0x05070001U
/********Name=PMU1CLKSEL_CON04,Offset=0x310********/
#define CLK_UART0_FRAC_DIV 0x20000001U
/********Name=PMU1CLKSEL_CON05,Offset=0x314********/
#define CLK_I2S1_8CH_TX_SRC_DIV 0x05020001U
/********Name=PMU1CLKSEL_CON06,Offset=0x318********/
#define CLK_I2S1_8CH_TX_FRAC_DIV 0x20000001U
/********Name=PMU1CLKSEL_CON07,Offset=0x31C********/
#define CLK_I2S1_8CH_RX_SRC_DIV 0x05020001U
/********Name=PMU1CLKSEL_CON08,Offset=0x320********/
#define CLK_I2S1_8CH_RX_FRAC_DIV 0x20000001U
/********Name=PMU1CLKSEL_CON09,Offset=0x324********/
#define CLK_USBDP_COMBO_PHY0_REF_XTAL_DIV 0x05050001U
/********Name=PMU1CLKSEL_CON12,Offset=0x330********/
#define CLK_HDPTX0_REF_XTAL_DIV 0x05060001U
/********Name=PMU1CLKSEL_CON14,Offset=0x338********/
#define CLK_REF_MIPI_DCPHY0_DIV 0x07000001U
#define CLK_OTGPHY_U3_0_DIV 0x05090001U
/********Name=PMU1CLKSEL_CON15,Offset=0x33C********/
#define CLK_CR_PARA_DIV 0x05000001U
/********Name=PMU1CLKSEL_CON17,Offset=0x344********/
/********Name=PMU1SCLKSEL_CON00,Offset=0x300********/
/********Name=PMU1SCLKSEL_CON02,Offset=0x308********/
#define XIN_OSC0_DIV_DIV 0x20000001U

/********Name=CLKSEL_CON00,Offset=0x300********/
#define CLK_MATRIX_50M_SRC_SEL 0x01050000U
#define CLK_MATRIX_50M_SRC_SEL_CLK_CPLL_MUX 0U
#define CLK_MATRIX_100M_SRC_SEL 0x010B0000U
#define CLK_MATRIX_100M_SRC_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON01,Offset=0x304********/
#define CLK_MATRIX_150M_SRC_SEL 0x01050001U
#define CLK_MATRIX_150M_SRC_SEL_CLK_CPLL_MUX 0U
#define CLK_MATRIX_200M_SRC_SEL 0x010B0001U
#define CLK_MATRIX_200M_SRC_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON02,Offset=0x308********/
#define CLK_MATRIX_250M_SRC_SEL 0x01050002U
#define CLK_MATRIX_250M_SRC_SEL_CLK_CPLL_MUX 0U
#define CLK_MATRIX_300M_SRC_SEL 0x010B0002U
#define CLK_MATRIX_300M_SRC_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON03,Offset=0x30C********/
#define CLK_MATRIX_350M_SRC_SEL 0x01050003U
#define CLK_MATRIX_350M_SRC_SEL_CLK_SPLL_MUX 0U
#define CLK_MATRIX_400M_SRC_SEL 0x010B0003U
#define CLK_MATRIX_400M_SRC_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON04,Offset=0x310********/
#define CLK_MATRIX_450M_SRC_SEL 0x01050004U
#define CLK_MATRIX_450M_SRC_SEL_CLK_CPLL_MUX 0U
#define CLK_MATRIX_500M_SRC_SEL 0x010B0004U
#define CLK_MATRIX_500M_SRC_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON05,Offset=0x314********/
#define CLK_MATRIX_600M_SRC_SEL 0x01050005U
#define CLK_MATRIX_600M_SRC_SEL_CLK_CPLL_MUX 0U
#define CLK_MATRIX_650M_SRC_SEL 0x010B0005U
#define CLK_MATRIX_650M_SRC_SEL_CLK_LPLL_MUX 0U
/********Name=CLKSEL_CON06,Offset=0x318********/
#define CLK_MATRIX_700M_SRC_SEL 0x01050006U
#define CLK_MATRIX_700M_SRC_SEL_CLK_SPLL_MUX 0U
#define CLK_MATRIX_800M_SRC_SEL 0x010B0006U
#define CLK_MATRIX_800M_SRC_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON07,Offset=0x31C********/
#define CLK_MATRIX_1000M_SRC_SEL 0x02050007U
#define CLK_MATRIX_1000M_SRC_SEL_CLK_V0PLL_MUX 0U
#define CLK_MATRIX_1200M_SRC_SEL 0x010C0007U
#define CLK_MATRIX_1200M_SRC_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON08,Offset=0x320********/
#define ACLK_TOP_ROOT_SEL 0x02050008U
#define ACLK_TOP_ROOT_SEL_CLK_AUPLL_MUX 0U
#define PCLK_TOP_ROOT_SEL 0x02070008U
#define PCLK_TOP_ROOT_SEL_XIN_OSC0_FUNC 0U
#define ACLK_LOW_TOP_ROOT_SEL 0x010E0008U
#define ACLK_LOW_TOP_ROOT_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON09,Offset=0x324********/
#define ACLK_TOP_M300_ROOT_SEL 0x02000009U
#define ACLK_TOP_M300_ROOT_SEL_XIN_OSC0_FUNC 0U
#define ACLK_TOP_M500_ROOT_SEL 0x02020009U
#define ACLK_TOP_M500_ROOT_SEL_XIN_OSC0_FUNC 0U
#define ACLK_TOP_M400_ROOT_SEL 0x02040009U
#define ACLK_TOP_M400_ROOT_SEL_XIN_OSC0_FUNC 0U
#define ACLK_TOP_S200_ROOT_SEL 0x02060009U
#define ACLK_TOP_S200_ROOT_SEL_XIN_OSC0_FUNC 0U
#define ACLK_TOP_S400_ROOT_SEL 0x02080009U
#define ACLK_TOP_S400_ROOT_SEL_XIN_OSC0_FUNC 0U
/********Name=CLKSEL_CON10,Offset=0x328********/
#define CLK_TESTOUT_TOP_SEL 0x0306000AU
#define CLK_TESTOUT_TOP_SEL_CLK_DEEPSLOW 0U
#define CLK_TESTOUT_SEL 0x0309000AU
#define CLK_TESTOUT_SEL_CLK_TESTOUT_DDR23 0U
#define CLK_TESTOUT_GRP0_SEL 0x030C000AU
#define CLK_TESTOUT_GRP0_SEL_CLK_TESTOUT_NPU 0U
/********Name=CLKSEL_CON100,Offset=0x490********/
#define CLK_RGA2_CORE_SEL 0x03050064U
#define CLK_RGA2_CORE_SEL_CLK_SPLL_MUX 0U
#define CLK_RGA3_0_CORE_SEL 0x030D0064U
#define CLK_RGA3_0_CORE_SEL_CLK_SPLL_MUX 0U
/********Name=CLKSEL_CON102,Offset=0x498********/
#define HCLK_RKVENC0_ROOT_SEL 0x02000066U
#define HCLK_RKVENC0_ROOT_SEL_XIN_OSC0_FUNC 0U
#define ACLK_RKVENC0_ROOT_SEL 0x02070066U
#define ACLK_RKVENC0_ROOT_SEL_CLK_NPLL_MUX 0U
#define CLK_RKVENC0_CORE_SEL 0x020E0066U
#define CLK_RKVENC0_CORE_SEL_CLK_NPLL_MUX 0U
/********Name=CLKSEL_CON104,Offset=0x4A0********/
#define HCLK_RKVENC1_ROOT_SEL 0x02000068U
#define HCLK_RKVENC1_ROOT_SEL_XIN_OSC0_FUNC 0U
#define ACLK_RKVENC1_ROOT_SEL 0x02070068U
#define ACLK_RKVENC1_ROOT_SEL_CLK_NPLL_MUX 0U
#define CLK_RKVENC1_CORE_SEL 0x020E0068U
#define CLK_RKVENC1_CORE_SEL_CLK_NPLL_MUX 0U
/********Name=CLKSEL_CON106,Offset=0x4A8********/
#define ACLK_VI_ROOT_SEL 0x0305006AU
#define ACLK_VI_ROOT_SEL_CLK_SPLL_MUX 0U
#define HCLK_VI_ROOT_SEL 0x0208006AU
#define HCLK_VI_ROOT_SEL_XIN_OSC0_FUNC 0U
#define PCLK_VI_ROOT_SEL 0x020A006AU
#define PCLK_VI_ROOT_SEL_XIN_OSC0_FUNC 0U
/********Name=CLKSEL_CON107,Offset=0x4AC********/
#define DCLK_VICAP_SEL 0x0105006BU
#define DCLK_VICAP_SEL_CLK_CPLL_MUX 0U
#define CLK_ISP0_CORE_SEL 0x020B006BU
#define CLK_ISP0_CORE_SEL_CLK_SPLL_MUX 0U
/********Name=CLKSEL_CON108,Offset=0x4B0********/
#define CLK_FISHEYE0_CORE_SEL 0x0205006CU
#define CLK_FISHEYE0_CORE_SEL_CLK_SPLL_MUX 0U
#define CLK_FISHEYE1_CORE_SEL 0x020C006CU
#define CLK_FISHEYE1_CORE_SEL_CLK_SPLL_MUX 0U
#define ICLK_CSIHOST01_SEL 0x020E006CU
#define ICLK_CSIHOST01_SEL_XIN_OSC0_FUNC 0U
/********Name=CLKSEL_CON110,Offset=0x4B8********/
#define ACLK_VOP_ROOT_SEL 0x0305006EU
#define ACLK_VOP_ROOT_SEL_CLK_SPLL_MUX 0U
#define ACLK_VOP_LOW_ROOT_SEL 0x0208006EU
#define ACLK_VOP_LOW_ROOT_SEL_XIN_OSC0_FUNC 0U
#define HCLK_VOP_ROOT_SEL 0x020A006EU
#define HCLK_VOP_ROOT_SEL_XIN_OSC0_FUNC 0U
#define PCLK_VOP_ROOT_SEL 0x020C006EU
#define PCLK_VOP_ROOT_SEL_XIN_OSC0_FUNC 0U
/********Name=CLKSEL_CON111,Offset=0x4BC********/
#define DCLK_VOP0_SRC_SEL 0x0207006FU
#define DCLK_VOP0_SRC_SEL_CLK_AUPLL_MUX 0U
#define DCLK_VOP1_SRC_SEL 0x020E006FU
#define DCLK_VOP1_SRC_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON112,Offset=0x4C0********/
#define DCLK_VOP2_SRC_SEL 0x02050070U
#define DCLK_VOP2_SRC_SEL_CLK_AUPLL_MUX 0U
#define DCLK_VOP0_SEL 0x02070070U
#define DCLK_VOP0_SEL_CLK_HDMIPHY_PIXEL1_O 0U
#define DCLK_VOP1_SEL 0x02090070U
#define DCLK_VOP1_SEL_CLK_HDMIPHY_PIXEL1_O 0U
#define DCLK_VOP2_SEL 0x020B0070U
#define DCLK_VOP2_SEL_CLK_HDMIPHY_PIXEL1_O 0U
/********Name=CLKSEL_CON113,Offset=0x4C4********/
#define DCLK_VOP3_SEL 0x02070071U
#define DCLK_VOP3_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON114,Offset=0x4C8********/
#define CLK_DSIHOST0_SEL 0x02070072U
#define CLK_DSIHOST0_SEL_CLK_SPLL_MUX 0U
/********Name=CLKSEL_CON115,Offset=0x4CC********/
#define CLK_DSIHOST1_SEL 0x02070073U
#define CLK_DSIHOST1_SEL_CLK_SPLL_MUX 0U
#define ACLK_VOP_SUB_SRC_SEL 0x01090073U
#define ACLK_VOP_SUB_SRC_SEL_ACLK_VOP_DIV2_SRC 0U
/********Name=CLKSEL_CON116,Offset=0x4D0********/
#define ACLK_VO0_ROOT_SEL 0x01050074U
#define ACLK_VO0_ROOT_SEL_CLK_CPLL_MUX 0U
#define HCLK_VO0_ROOT_SEL 0x02060074U
#define HCLK_VO0_ROOT_SEL_XIN_OSC0_FUNC 0U
#define HCLK_VO0_S_ROOT_SEL 0x02080074U
#define HCLK_VO0_S_ROOT_SEL_XIN_OSC0_FUNC 0U
#define PCLK_VO0_ROOT_SEL 0x020A0074U
#define PCLK_VO0_ROOT_SEL_XIN_OSC0_FUNC 0U
#define PCLK_VO0_S_ROOT_SEL 0x020C0074U
#define PCLK_VO0_S_ROOT_SEL_XIN_OSC0_FUNC 0U
/********Name=CLKSEL_CON117,Offset=0x4D4********/
/********Name=CLKSEL_CON118,Offset=0x4D8********/
#define CLK_I2S4_8CH_TX_SRC_SEL 0x01050076U
#define CLK_I2S4_8CH_TX_SRC_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON119,Offset=0x4DC********/
/********Name=CLKSEL_CON120,Offset=0x4E0********/
#define MCLK_I2S4_8CH_TX_SEL 0x02000078U
#define MCLK_I2S4_8CH_TX_SEL_XIN_OSC0_HALF 0U
#define I2S4_8CH_MCLKOUT_SEL 0x01020078U
#define I2S4_8CH_MCLKOUT_SEL_XIN_OSC0_HALF 0U
#define CLK_I2S8_8CH_TX_SRC_SEL 0x01080078U
#define CLK_I2S8_8CH_TX_SRC_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON121,Offset=0x4E4********/
/********Name=CLKSEL_CON122,Offset=0x4E8********/
#define MCLK_I2S8_8CH_TX_SEL 0x0200007AU
#define MCLK_I2S8_8CH_TX_SEL_XIN_OSC0_HALF 0U
#define I2S8_8CH_MCLKOUT_SEL 0x0102007AU
#define I2S8_8CH_MCLKOUT_SEL_XIN_OSC0_HALF 0U
#define CLK_SPDIF2_DP0_SRC_SEL 0x0108007AU
#define CLK_SPDIF2_DP0_SRC_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON123,Offset=0x4EC********/
/********Name=CLKSEL_CON124,Offset=0x4F0********/
#define MCLK_4X_SPDIF2_DP0_SEL 0x0200007CU
#define MCLK_4X_SPDIF2_DP0_SEL_XIN_OSC0_HALF 0U
#define CLK_SPDIF5_DP1_SRC_SEL 0x0107007CU
#define CLK_SPDIF5_DP1_SRC_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON125,Offset=0x4F4********/
/********Name=CLKSEL_CON126,Offset=0x4F8********/
#define MCLK_4X_SPDIF5_DP1_SEL 0x0200007EU
#define MCLK_4X_SPDIF5_DP1_SEL_XIN_OSC0_HALF 0U
/********Name=CLKSEL_CON128,Offset=0x500********/
#define ACLK_HDCP1_ROOT_SEL 0x02050080U
#define ACLK_HDCP1_ROOT_SEL_CLK_HDMITRX_REFSRC 0U
#define ACLK_HDMIRX_ROOT_SEL 0x010C0080U
#define ACLK_HDMIRX_ROOT_SEL_CLK_CPLL_MUX 0U
#define HCLK_VO1_ROOT_SEL 0x020D0080U
#define HCLK_VO1_ROOT_SEL_XIN_OSC0_FUNC 0U
/********Name=CLKSEL_CON129,Offset=0x504********/
#define HCLK_VO1_S_ROOT_SEL 0x02000081U
#define HCLK_VO1_S_ROOT_SEL_XIN_OSC0_FUNC 0U
#define PCLK_VO1_ROOT_SEL 0x02020081U
#define PCLK_VO1_ROOT_SEL_XIN_OSC0_FUNC 0U
#define PCLK_VO1_S_ROOT_SEL 0x02040081U
#define PCLK_VO1_S_ROOT_SEL_XIN_OSC0_FUNC 0U
#define CLK_I2S7_8CH_RX_SRC_SEL 0x010B0081U
#define CLK_I2S7_8CH_RX_SRC_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON130,Offset=0x508********/
/********Name=CLKSEL_CON131,Offset=0x50C********/
#define MCLK_I2S7_8CH_RX_SEL 0x02000083U
#define MCLK_I2S7_8CH_RX_SEL_XIN_OSC0_HALF 0U
#define I2S7_8CH_MCLKOUT_SEL 0x01020083U
#define I2S7_8CH_MCLKOUT_SEL_XIN_OSC0_HALF 0U
/********Name=CLKSEL_CON133,Offset=0x514********/
#define CLK_HDMITX0_EARC_SEL 0x01060085U
#define CLK_HDMITX0_EARC_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON136,Offset=0x520********/
#define CLK_HDMITX1_EARC_SEL 0x01060088U
#define CLK_HDMITX1_EARC_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON138,Offset=0x528********/
#define CLK_HDMIRX_AUD_SRC_SEL 0x0108008AU
#define CLK_HDMIRX_AUD_SRC_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON139,Offset=0x52C********/
/********Name=CLKSEL_CON140,Offset=0x530********/
#define CLK_HDMIRX_AUD_SEL 0x0100008CU
#define CLK_HDMIRX_AUD_SEL_CLK_HDMIRX_AUD_FRAC 0U
#define CLK_EDP0_200M_SEL 0x0201008CU
#define CLK_EDP0_200M_SEL_XIN_OSC0_FUNC 0U
#define CLK_EDP1_200M_SEL 0x0203008CU
#define CLK_EDP1_200M_SEL_XIN_OSC0_FUNC 0U
#define CLK_I2S5_8CH_TX_SRC_SEL 0x010A008CU
#define CLK_I2S5_8CH_TX_SRC_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON141,Offset=0x534********/
/********Name=CLKSEL_CON142,Offset=0x538********/
#define MCLK_I2S5_8CH_TX_SEL 0x0200008EU
#define MCLK_I2S5_8CH_TX_SEL_XIN_OSC0_HALF 0U
/********Name=CLKSEL_CON144,Offset=0x540********/
#define I2S5_8CH_MCLKOUT_SEL 0x01020090U
#define I2S5_8CH_MCLKOUT_SEL_XIN_OSC0_HALF 0U
#define CLK_I2S6_8CH_TX_SRC_SEL 0x01080090U
#define CLK_I2S6_8CH_TX_SRC_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON145,Offset=0x544********/
/********Name=CLKSEL_CON146,Offset=0x548********/
#define MCLK_I2S6_8CH_TX_SEL 0x02000092U
#define MCLK_I2S6_8CH_TX_SEL_XIN_OSC0_HALF 0U
#define CLK_I2S6_8CH_RX_SRC_SEL 0x01070092U
#define CLK_I2S6_8CH_RX_SRC_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON147,Offset=0x54C********/
/********Name=CLKSEL_CON148,Offset=0x550********/
#define MCLK_I2S6_8CH_RX_SEL 0x02000094U
#define MCLK_I2S6_8CH_RX_SEL_XIN_OSC0_HALF 0U
#define I2S6_8CH_MCLKOUT_SEL 0x02020094U
#define I2S6_8CH_MCLKOUT_SEL_XIN_OSC0_HALF 0U
#define CLK_SPDIF3_SRC_SEL 0x01090094U
#define CLK_SPDIF3_SRC_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON149,Offset=0x554********/
/********Name=CLKSEL_CON15,Offset=0x33C********/
#define MCLK_GMAC0_OUT_SEL 0x0107000FU
#define MCLK_GMAC0_OUT_SEL_CLK_CPLL_MUX 0U
#define REFCLKO25M_ETH0_OUT_SEL 0x010F000FU
#define REFCLKO25M_ETH0_OUT_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON150,Offset=0x558********/
#define MCLK_SPDIF3_SEL 0x02000096U
#define MCLK_SPDIF3_SEL_XIN_OSC0_HALF 0U
#define CLK_SPDIF4_SRC_SEL 0x01070096U
#define CLK_SPDIF4_SRC_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON151,Offset=0x55C********/
/********Name=CLKSEL_CON152,Offset=0x560********/
#define MCLK_SPDIF4_SEL 0x02000098U
#define MCLK_SPDIF4_SEL_XIN_OSC0_HALF 0U
#define MCLK_SPDIFRX0_SEL 0x02070098U
#define MCLK_SPDIFRX0_SEL_CLK_AUPLL_MUX 0U
#define MCLK_SPDIFRX1_SEL 0x020E0098U
#define MCLK_SPDIFRX1_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON153,Offset=0x564********/
#define MCLK_SPDIFRX2_SEL 0x02050099U
#define MCLK_SPDIFRX2_SEL_CLK_AUPLL_MUX 0U
#define CLK_I2S9_8CH_RX_SRC_SEL 0x010C0099U
#define CLK_I2S9_8CH_RX_SRC_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON154,Offset=0x568********/
/********Name=CLKSEL_CON155,Offset=0x56C********/
#define MCLK_I2S9_8CH_RX_SEL 0x0200009BU
#define MCLK_I2S9_8CH_RX_SEL_XIN_OSC0_HALF 0U
#define CLK_I2S10_8CH_RX_SRC_SEL 0x0108009BU
#define CLK_I2S10_8CH_RX_SRC_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON156,Offset=0x570********/
/********Name=CLKSEL_CON157,Offset=0x574********/
#define MCLK_I2S10_8CH_RX_SEL 0x0200009DU
#define MCLK_I2S10_8CH_RX_SEL_XIN_OSC0_HALF 0U
#define CLK_HDMITRX_REFSRC_SEL 0x0107009DU
#define CLK_HDMITRX_REFSRC_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON158,Offset=0x578********/
#define CLK_GPU_SRC_T_SEL 0x0305009EU
#define CLK_GPU_SRC_T_SEL_CLK_SPLL_MUX 0U
#define CLK_TESTOUT_GPU_SEL 0x010D009EU
#define CLK_TESTOUT_GPU_SEL_CLK_GPU_PVTPLL 0U
#define CLK_GPU_SRC_SEL 0x010E009EU
#define CLK_GPU_SRC_SEL_CLK_GPU_PVTPLL 0U
/********Name=CLKSEL_CON159,Offset=0x57C********/
/********Name=CLKSEL_CON16,Offset=0x340********/
#define REFCLKO25M_ETH1_OUT_SEL 0x01070010U
#define REFCLKO25M_ETH1_OUT_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON160,Offset=0x580********/
/********Name=CLKSEL_CON161,Offset=0x584********/
#define PCLK_GPU_ROOT_SEL 0x020000A1U
#define PCLK_GPU_ROOT_SEL_XIN_OSC0_FUNC 0U
#define CLK_GPU_PVTPLL_SEL 0x010200A1U
#define CLK_GPU_PVTPLL_SEL_XIN_OSC0_FUNC 0U
/********Name=CLKSEL_CON163,Offset=0x58C********/
#define ACLK_AV1_ROOT_SEL 0x020500A3U
#define ACLK_AV1_ROOT_SEL_CLK_AUPLL_MUX 0U
#define PCLK_AV1_ROOT_SEL 0x020700A3U
#define PCLK_AV1_ROOT_SEL_XIN_OSC0_FUNC 0U
/********Name=CLKSEL_CON165,Offset=0x594********/
#define ACLK_CENTER_ROOT_SEL 0x020000A5U
#define ACLK_CENTER_ROOT_SEL_XIN_OSC0_FUNC 0U
#define ACLK_CENTER_LOW_ROOT_SEL 0x020200A5U
#define ACLK_CENTER_LOW_ROOT_SEL_XIN_OSC0_FUNC 0U
#define HCLK_CENTER_ROOT_SEL 0x020400A5U
#define HCLK_CENTER_ROOT_SEL_XIN_OSC0_FUNC 0U
#define PCLK_CENTER_ROOT_SEL 0x020600A5U
#define PCLK_CENTER_ROOT_SEL_XIN_OSC0_FUNC 0U
#define ACLK_CENTER_S200_ROOT_SEL 0x020800A5U
#define ACLK_CENTER_S200_ROOT_SEL_XIN_OSC0_FUNC 0U
#define ACLK_CENTER_S400_ROOT_SEL 0x020A00A5U
#define ACLK_CENTER_S400_ROOT_SEL_XIN_OSC0_FUNC 0U
#define CLK_DDR_TIMER_ROOT_SEL 0x010C00A5U
#define CLK_DDR_TIMER_ROOT_SEL_CLK_MATRIX_100M_SRC 0U
/********Name=CLKSEL_CON166,Offset=0x598********/
#define CLK_DDR_CM0_RTC_SEL 0x010500A6U
#define CLK_DDR_CM0_RTC_SEL_CLK_DEEPSLOW 0U
/********Name=CLKSEL_CON17,Offset=0x344********/
#define CLK_CIFOUT_OUT_SEL 0x02080011U
#define CLK_CIFOUT_OUT_SEL_CLK_SPLL_MUX 0U
/********Name=CLKSEL_CON170,Offset=0x5A8********/
#define ACLK_VO1USB_TOP_ROOT_SEL 0x010500AAU
#define ACLK_VO1USB_TOP_ROOT_SEL_CLK_CPLL_MUX 0U
#define HCLK_VO1USB_TOP_ROOT_SEL 0x020600AAU
#define HCLK_VO1USB_TOP_ROOT_SEL_XIN_OSC0_FUNC 0U
/********Name=CLKSEL_CON172,Offset=0x5B0********/
#define HCLK_SDIO_ROOT_SEL 0x020000ACU
#define HCLK_SDIO_ROOT_SEL_XIN_OSC0_FUNC 0U
#define CCLK_SRC_SDIO_SEL 0x020800ACU
#define CCLK_SRC_SDIO_SEL_XIN_OSC0_FUNC 0U
/********Name=CLKSEL_CON174,Offset=0x5B8********/
#define ACLK_RGA3_ROOT_SEL 0x020500AEU
#define ACLK_RGA3_ROOT_SEL_CLK_AUPLL_MUX 0U
#define HCLK_RGA3_ROOT_SEL 0x020700AEU
#define HCLK_RGA3_ROOT_SEL_XIN_OSC0_FUNC 0U
#define CLK_RGA3_1_CORE_SEL 0x020E00AEU
#define CLK_RGA3_1_CORE_SEL_CLK_SPLL_MUX 0U
/********Name=CLKSEL_CON176,Offset=0x5C0********/
/********Name=CLKSEL_CON177,Offset=0x5C4********/
#define CLK_REF_PIPE_PHY0_SEL 0x010600B1U
#define CLK_REF_PIPE_PHY0_SEL_CLK_REF_PIPE_PHY0_PLL_SRC 0U
#define CLK_REF_PIPE_PHY1_SEL 0x010700B1U
#define CLK_REF_PIPE_PHY1_SEL_CLK_REF_PIPE_PHY1_PLL_SRC 0U
#define CLK_REF_PIPE_PHY2_SEL 0x010800B1U
#define CLK_REF_PIPE_PHY2_SEL_CLK_REF_PIPE_PHY2_PLL_SRC 0U
/********Name=CLKSEL_CON18,Offset=0x348********/
#define CLK_MIPI_CAMARAOUT_M0_SEL 0x02080012U
#define CLK_MIPI_CAMARAOUT_M0_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON19,Offset=0x34C********/
#define CLK_MIPI_CAMARAOUT_M1_SEL 0x02080013U
#define CLK_MIPI_CAMARAOUT_M1_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON20,Offset=0x350********/
#define CLK_MIPI_CAMARAOUT_M2_SEL 0x02080014U
#define CLK_MIPI_CAMARAOUT_M2_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON21,Offset=0x354********/
#define CLK_MIPI_CAMARAOUT_M3_SEL 0x02080015U
#define CLK_MIPI_CAMARAOUT_M3_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON22,Offset=0x358********/
#define CLK_MIPI_CAMARAOUT_M4_SEL 0x02080016U
#define CLK_MIPI_CAMARAOUT_M4_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON24,Offset=0x360********/
#define HCLK_AUDIO_ROOT_SEL 0x02000018U
#define HCLK_AUDIO_ROOT_SEL_XIN_OSC0_FUNC 0U
#define PCLK_AUDIO_ROOT_SEL 0x02020018U
#define PCLK_AUDIO_ROOT_SEL_XIN_OSC0_FUNC 0U
#define CLK_I2S0_8CH_TX_SRC_SEL 0x01090018U
#define CLK_I2S0_8CH_TX_SRC_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON25,Offset=0x364********/
/********Name=CLKSEL_CON26,Offset=0x368********/
#define MCLK_I2S0_8CH_TX_SEL 0x0200001AU
#define MCLK_I2S0_8CH_TX_SEL_XIN_OSC0_HALF 0U
#define CLK_I2S0_8CH_RX_SRC_SEL 0x0107001AU
#define CLK_I2S0_8CH_RX_SRC_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON27,Offset=0x36C********/
/********Name=CLKSEL_CON28,Offset=0x370********/
#define MCLK_I2S0_8CH_RX_SEL 0x0200001CU
#define MCLK_I2S0_8CH_RX_SEL_XIN_OSC0_HALF 0U
#define I2S0_8CH_MCLKOUT_SEL 0x0202001CU
#define I2S0_8CH_MCLKOUT_SEL_XIN_OSC0_HALF 0U
#define CLK_I2S2_2CH_SRC_SEL 0x0109001CU
#define CLK_I2S2_2CH_SRC_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON29,Offset=0x374********/
/********Name=CLKSEL_CON30,Offset=0x378********/
#define MCLK_I2S2_2CH_SEL 0x0200001EU
#define MCLK_I2S2_2CH_SEL_XIN_OSC0_HALF 0U
#define I2S2_2CH_MCLKOUT_SEL 0x0102001EU
#define I2S2_2CH_MCLKOUT_SEL_XIN_OSC0_HALF 0U
#define CLK_I2S3_2CH_SRC_SEL 0x0108001EU
#define CLK_I2S3_2CH_SRC_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON31,Offset=0x37C********/
/********Name=CLKSEL_CON32,Offset=0x380********/
#define MCLK_I2S3_2CH_SEL 0x02000020U
#define MCLK_I2S3_2CH_SEL_XIN_OSC0_HALF 0U
#define I2S3_2CH_MCLKOUT_SEL 0x01020020U
#define I2S3_2CH_MCLKOUT_SEL_XIN_OSC0_HALF 0U
#define CLK_SPDIF0_SRC_SEL 0x01080020U
#define CLK_SPDIF0_SRC_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON33,Offset=0x384********/
/********Name=CLKSEL_CON34,Offset=0x388********/
#define MCLK_SPDIF0_SEL 0x02000022U
#define MCLK_SPDIF0_SEL_XIN_OSC0_HALF 0U
#define CLK_SPDIF1_SRC_SEL 0x01070022U
#define CLK_SPDIF1_SRC_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON35,Offset=0x38C********/
/********Name=CLKSEL_CON36,Offset=0x390********/
#define MCLK_SPDIF1_SEL 0x02000024U
#define MCLK_SPDIF1_SEL_XIN_OSC0_HALF 0U
#define MCLK_PDM1_SEL 0x02070024U
#define MCLK_PDM1_SEL_CLK_AUPLL_MUX 0U
/********Name=CLKSEL_CON38,Offset=0x398********/
#define ACLK_BUS_ROOT_SEL 0x01050026U
#define ACLK_BUS_ROOT_SEL_CLK_CPLL_MUX 0U
#define CLK_I2C1_SEL 0x01060026U
#define CLK_I2C1_SEL_CLK_MATRIX_100M_SRC 0U
#define CLK_I2C2_SEL 0x01070026U
#define CLK_I2C2_SEL_CLK_MATRIX_100M_SRC 0U
#define CLK_I2C3_SEL 0x01080026U
#define CLK_I2C3_SEL_CLK_MATRIX_100M_SRC 0U
#define CLK_I2C4_SEL 0x01090026U
#define CLK_I2C4_SEL_CLK_MATRIX_100M_SRC 0U
#define CLK_I2C5_SEL 0x010A0026U
#define CLK_I2C5_SEL_CLK_MATRIX_100M_SRC 0U
#define CLK_I2C6_SEL 0x010B0026U
#define CLK_I2C6_SEL_CLK_MATRIX_100M_SRC 0U
#define CLK_I2C7_SEL 0x010C0026U
#define CLK_I2C7_SEL_CLK_MATRIX_100M_SRC 0U
#define CLK_I2C8_SEL 0x010D0026U
#define CLK_I2C8_SEL_CLK_MATRIX_100M_SRC 0U
/********Name=CLKSEL_CON39,Offset=0x39C********/
#define CLK_CAN0_SEL 0x01050027U
#define CLK_CAN0_SEL_CLK_CPLL_MUX 0U
#define CLK_CAN1_SEL 0x010B0027U
#define CLK_CAN1_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON40,Offset=0x3A0********/
#define CLK_CAN2_SEL 0x01050028U
#define CLK_CAN2_SEL_CLK_CPLL_MUX 0U
#define CLK_SARADC_SEL 0x010E0028U
#define CLK_SARADC_SEL_XIN_OSC0_FUNC 0U
/********Name=CLKSEL_CON41,Offset=0x3A4********/
#define CLK_TSADC_SEL 0x01080029U
#define CLK_TSADC_SEL_XIN_OSC0_FUNC 0U
#define CLK_UART1_SRC_SEL 0x010E0029U
#define CLK_UART1_SRC_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON42,Offset=0x3A8********/
/********Name=CLKSEL_CON43,Offset=0x3AC********/
#define SCLK_UART1_SEL 0x0200002BU
#define SCLK_UART1_SEL_XIN_OSC0_FUNC 0U
#define CLK_UART2_SRC_SEL 0x0107002BU
#define CLK_UART2_SRC_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON44,Offset=0x3B0********/
/********Name=CLKSEL_CON45,Offset=0x3B4********/
#define SCLK_UART2_SEL 0x0200002DU
#define SCLK_UART2_SEL_XIN_OSC0_FUNC 0U
#define CLK_UART3_SRC_SEL 0x0107002DU
#define CLK_UART3_SRC_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON46,Offset=0x3B8********/
/********Name=CLKSEL_CON47,Offset=0x3BC********/
#define SCLK_UART3_SEL 0x0200002FU
#define SCLK_UART3_SEL_XIN_OSC0_FUNC 0U
#define CLK_UART4_SRC_SEL 0x0107002FU
#define CLK_UART4_SRC_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON48,Offset=0x3C0********/
/********Name=CLKSEL_CON49,Offset=0x3C4********/
#define SCLK_UART4_SEL 0x02000031U
#define SCLK_UART4_SEL_XIN_OSC0_FUNC 0U
#define CLK_UART5_SRC_SEL 0x01070031U
#define CLK_UART5_SRC_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON50,Offset=0x3C8********/
/********Name=CLKSEL_CON51,Offset=0x3CC********/
#define SCLK_UART5_SEL 0x02000033U
#define SCLK_UART5_SEL_XIN_OSC0_FUNC 0U
#define CLK_UART6_SRC_SEL 0x01070033U
#define CLK_UART6_SRC_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON52,Offset=0x3D0********/
/********Name=CLKSEL_CON53,Offset=0x3D4********/
#define SCLK_UART6_SEL 0x02000035U
#define SCLK_UART6_SEL_XIN_OSC0_FUNC 0U
#define CLK_UART7_SRC_SEL 0x01070035U
#define CLK_UART7_SRC_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON54,Offset=0x3D8********/
/********Name=CLKSEL_CON55,Offset=0x3DC********/
#define SCLK_UART7_SEL 0x02000037U
#define SCLK_UART7_SEL_XIN_OSC0_FUNC 0U
#define CLK_UART8_SRC_SEL 0x01070037U
#define CLK_UART8_SRC_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON56,Offset=0x3E0********/
/********Name=CLKSEL_CON57,Offset=0x3E4********/
#define SCLK_UART8_SEL 0x02000039U
#define SCLK_UART8_SEL_XIN_OSC0_FUNC 0U
#define CLK_UART9_SRC_SEL 0x01070039U
#define CLK_UART9_SRC_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON58,Offset=0x3E8********/
/********Name=CLKSEL_CON59,Offset=0x3EC********/
#define SCLK_UART9_SEL 0x0200003BU
#define SCLK_UART9_SEL_XIN_OSC0_FUNC 0U
#define CLK_SPI0_SEL 0x0202003BU
#define CLK_SPI0_SEL_XIN_OSC0_FUNC 0U
#define CLK_SPI1_SEL 0x0204003BU
#define CLK_SPI1_SEL_XIN_OSC0_FUNC 0U
#define CLK_SPI2_SEL 0x0206003BU
#define CLK_SPI2_SEL_XIN_OSC0_FUNC 0U
#define CLK_SPI3_SEL 0x0208003BU
#define CLK_SPI3_SEL_XIN_OSC0_FUNC 0U
#define CLK_SPI4_SEL 0x020A003BU
#define CLK_SPI4_SEL_XIN_OSC0_FUNC 0U
#define CLK_PWM1_SEL 0x020C003BU
#define CLK_PWM1_SEL_XIN_OSC0_FUNC 0U
#define CLK_PWM2_SEL 0x020E003BU
#define CLK_PWM2_SEL_XIN_OSC0_FUNC 0U
/********Name=CLKSEL_CON60,Offset=0x3F0********/
#define CLK_PWM3_SEL 0x0200003CU
#define CLK_PWM3_SEL_XIN_OSC0_FUNC 0U
#define CLK_BUS_TIMER_ROOT_SEL 0x0102003CU
#define CLK_BUS_TIMER_ROOT_SEL_CLK_MATRIX_100M_SRC 0U
#define DBCLK_GPIO1_SEL 0x0108003CU
#define DBCLK_GPIO1_SEL_CLK_DEEPSLOW 0U
#define DBCLK_GPIO2_SEL 0x010E003CU
#define DBCLK_GPIO2_SEL_CLK_DEEPSLOW 0U
/********Name=CLKSEL_CON61,Offset=0x3F4********/
#define DBCLK_GPIO3_SEL 0x0105003DU
#define DBCLK_GPIO3_SEL_CLK_DEEPSLOW 0U
#define DBCLK_GPIO4_SEL 0x010B003DU
#define DBCLK_GPIO4_SEL_CLK_DEEPSLOW 0U
/********Name=CLKSEL_CON62,Offset=0x3F8********/
#define DCLK_DECOM_SEL 0x0105003EU
#define DCLK_DECOM_SEL_CLK_SPLL_MUX 0U
/********Name=CLKSEL_CON63,Offset=0x3FC********/
#define CLK_TESTOUT_DDR01_SEL 0x0106003FU
#define CLK_TESTOUT_DDR01_SEL_CLK_DFI_CH1 0U
/********Name=CLKSEL_CON65,Offset=0x404********/
#define CLK_TESTOUT_DDR23_SEL 0x01060041U
#define CLK_TESTOUT_DDR23_SEL_CLK_DFI_CH3 0U
/********Name=CLKSEL_CON67,Offset=0x40C********/
#define ACLK_ISP1_ROOT_SEL 0x02050043U
#define ACLK_ISP1_ROOT_SEL_CLK_SPLL_MUX 0U
#define HCLK_ISP1_ROOT_SEL 0x02070043U
#define HCLK_ISP1_ROOT_SEL_XIN_OSC0_FUNC 0U
#define CLK_ISP1_CORE_SEL 0x020E0043U
#define CLK_ISP1_CORE_SEL_CLK_SPLL_MUX 0U
/********Name=CLKSEL_CON73,Offset=0x424********/
#define HCLK_RKNN_ROOT_SEL 0x02000049U
#define HCLK_RKNN_ROOT_SEL_XIN_OSC0_FUNC 0U
#define CLK_RKNN_DSU0_SRC_T_SEL 0x03070049U
#define CLK_RKNN_DSU0_SRC_T_SEL_CLK_SPLL_MUX 0U
#define CLK_TESTOUT_NPU_SEL 0x010F0049U
#define CLK_TESTOUT_NPU_SEL_CLK_NPU_PVTPLL 0U
/********Name=CLKSEL_CON74,Offset=0x428********/
#define CLK_RKNN_DSU0_SEL 0x0100004AU
#define CLK_RKNN_DSU0_SEL_CLK_NPU_PVTPLL 0U
#define PCLK_NPUTOP_ROOT_SEL 0x0201004AU
#define PCLK_NPUTOP_ROOT_SEL_XIN_OSC0_FUNC 0U
#define CLK_NPUTIMER_ROOT_SEL 0x0103004AU
#define CLK_NPUTIMER_ROOT_SEL_CLK_MATRIX_100M_SRC 0U
#define CLK_NPU_PVTPLL_SEL 0x0104004AU
#define CLK_NPU_PVTPLL_SEL_XIN_OSC0_FUNC 0U
#define HCLK_NPU_CM0_ROOT_SEL 0x0205004AU
#define HCLK_NPU_CM0_ROOT_SEL_XIN_OSC0_FUNC 0U
#define CLK_NPU_CM0_RTC_SEL 0x010C004AU
#define CLK_NPU_CM0_RTC_SEL_CLK_DEEPSLOW 0U
/********Name=CLKSEL_CON77,Offset=0x434********/
#define HCLK_NVM_ROOT_SEL 0x0200004DU
#define HCLK_NVM_ROOT_SEL_XIN_OSC0_FUNC 0U
#define ACLK_NVM_ROOT_SEL 0x0107004DU
#define ACLK_NVM_ROOT_SEL_CLK_CPLL_MUX 0U
#define CCLK_EMMC_SEL 0x020E004DU
#define CCLK_EMMC_SEL_XIN_OSC0_FUNC 0U
/********Name=CLKSEL_CON78,Offset=0x438********/
#define BCLK_EMMC_SEL 0x0105004EU
#define BCLK_EMMC_SEL_CLK_CPLL_MUX 0U
#define SCLK_SFC_SEL 0x020C004EU
#define SCLK_SFC_SEL_XIN_OSC0_FUNC 0U
/********Name=CLKSEL_CON80,Offset=0x440********/
#define PCLK_PHP_ROOT_SEL 0x02000050U
#define PCLK_PHP_ROOT_SEL_XIN_OSC0_FUNC 0U
#define ACLK_PCIE_ROOT_SEL 0x01070050U
#define ACLK_PCIE_ROOT_SEL_CLK_CPLL_MUX 0U
#define ACLK_PHP_ROOT_SEL 0x010D0050U
#define ACLK_PHP_ROOT_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON81,Offset=0x444********/
#define CLK_GMAC0_PTP_REF_SEL 0x01060051U
#define CLK_GMAC0_PTP_REF_SEL_CLK_GMAC0_PTPREFO 0U
#define CLK_GMAC1_PTP_REF_SEL 0x010D0051U
#define CLK_GMAC1_PTP_REF_SEL_CLK_GMAC1_PTPREFO 0U
/********Name=CLKSEL_CON82,Offset=0x448********/
#define CLK_RXOOB0_SEL 0x01070052U
#define CLK_RXOOB0_SEL_CLK_CPLL_MUX 0U
#define CLK_RXOOB1_SEL 0x010F0052U
#define CLK_RXOOB1_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON83,Offset=0x44C********/
#define CLK_RXOOB2_SEL 0x01070053U
#define CLK_RXOOB2_SEL_CLK_CPLL_MUX 0U
#define CLK_GMAC_125M_CRU_I_SEL 0x010F0053U
#define CLK_GMAC_125M_CRU_I_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON84,Offset=0x450********/
#define CLK_GMAC_50M_CRU_I_SEL 0x01070054U
#define CLK_GMAC_50M_CRU_I_SEL_CLK_CPLL_MUX 0U
#define CLK_UTMI_OTG2_SEL 0x020C0054U
#define CLK_UTMI_OTG2_SEL_XIN_OSC0_FUNC 0U
/********Name=CLKSEL_CON85,Offset=0x454********/
/********Name=CLKSEL_CON89,Offset=0x464********/
#define HCLK_RKVDEC0_ROOT_SEL 0x02000059U
#define HCLK_RKVDEC0_ROOT_SEL_XIN_OSC0_FUNC 0U
#define ACLK_RKVDEC0_ROOT_SEL 0x02070059U
#define ACLK_RKVDEC0_ROOT_SEL_CLK_SPLL_MUX 0U
#define ACLK_RKVDEC_CCU_SEL 0x020E0059U
#define ACLK_RKVDEC_CCU_SEL_CLK_SPLL_MUX 0U
/********Name=CLKSEL_CON90,Offset=0x468********/
#define CLK_RKVDEC0_CA_SEL 0x0105005AU
#define CLK_RKVDEC0_CA_SEL_CLK_CPLL_MUX 0U
#define CLK_RKVDEC0_HEVC_CA_SEL 0x020B005AU
#define CLK_RKVDEC0_HEVC_CA_SEL_CLK_MATRIX_1000M_SRC 0U
/********Name=CLKSEL_CON91,Offset=0x46C********/
#define CLK_RKVDEC0_CORE_SEL 0x0105005BU
#define CLK_RKVDEC0_CORE_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON93,Offset=0x474********/
#define HCLK_RKVDEC1_ROOT_SEL 0x0200005DU
#define HCLK_RKVDEC1_ROOT_SEL_XIN_OSC0_FUNC 0U
#define ACLK_RKVDEC1_ROOT_SEL 0x0207005DU
#define ACLK_RKVDEC1_ROOT_SEL_CLK_NPLL_MUX 0U
#define CLK_RKVDEC1_CA_SEL 0x010E005DU
#define CLK_RKVDEC1_CA_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON94,Offset=0x478********/
#define CLK_RKVDEC1_HEVC_CA_SEL 0x0205005EU
#define CLK_RKVDEC1_HEVC_CA_SEL_CLK_MATRIX_1000M_SRC 0U
#define CLK_RKVDEC1_CORE_SEL 0x010C005EU
#define CLK_RKVDEC1_CORE_SEL_CLK_CPLL_MUX 0U
/********Name=CLKSEL_CON96,Offset=0x480********/
#define ACLK_USB_ROOT_SEL 0x01050060U
#define ACLK_USB_ROOT_SEL_CLK_CPLL_MUX 0U
#define HCLK_USB_ROOT_SEL 0x02060060U
#define HCLK_USB_ROOT_SEL_XIN_OSC0_FUNC 0U
/********Name=CLKSEL_CON98,Offset=0x488********/
#define ACLK_VDPU_ROOT_SEL 0x02050062U
#define ACLK_VDPU_ROOT_SEL_CLK_AUPLL_MUX 0U
#define ACLK_VDPU_LOW_ROOT_SEL 0x02070062U
#define ACLK_VDPU_LOW_ROOT_SEL_XIN_OSC0_FUNC 0U
#define HCLK_VDPU_ROOT_SEL 0x02090062U
#define HCLK_VDPU_ROOT_SEL_XIN_OSC0_FUNC 0U
/********Name=CLKSEL_CON99,Offset=0x48C********/
#define ACLK_JPEG_DECODER_ROOT_SEL 0x02050063U
#define ACLK_JPEG_DECODER_ROOT_SEL_CLK_SPLL_MUX 0U
#define CLK_IEP2P0_CORE_SEL 0x010C0063U
#define CLK_IEP2P0_CORE_SEL_CLK_CPLL_MUX 0U

/********Name=PMU1CLKSEL_CON00,Offset=0x300********/
#define CLK_MATRIX_PMU1_300M_SRC_SEL 0x010F0001U
#define CLK_MATRIX_PMU1_300M_SRC_SEL_XIN_OSC0_FUNC 0U
/********Name=PMU1CLKSEL_CON01,Offset=0x304********/
#define CLK_MATRIX_PMU1_400M_SRC_SEL 0x01050001U
#define CLK_MATRIX_PMU1_400M_SRC_SEL_XIN_OSC0_FUNC 0U
#define HCLK_PMU1_ROOT_I_SEL 0x02060001U
#define HCLK_PMU1_ROOT_I_SEL_XIN_OSC0_FUNC 0U
#define PCLK_PMU1_ROOT_I_SEL 0x02080001U
#define PCLK_PMU1_ROOT_I_SEL_XIN_OSC0_FUNC 0U
#define HCLK_PMU_CM0_ROOT_I_SEL 0x020A0001U
#define HCLK_PMU_CM0_ROOT_I_SEL_XIN_OSC0_FUNC 0U
/********Name=PMU1CLKSEL_CON02,Offset=0x308********/
#define CLK_PMU_CM0_RTC_SEL 0x01050001U
#define CLK_PMU_CM0_RTC_SEL_CLK_DEEPSLOW 0U
#define TCLK_PMU1WDT_SEL 0x01060001U
#define TCLK_PMU1WDT_SEL_CLK_DEEPSLOW 0U
#define CLK_PMU1TIMER_ROOT_SEL 0x02070001U
#define CLK_PMU1TIMER_ROOT_SEL_CLK_MATRIX_PMU1_100M_SRC 0U
#define CLK_PMU1PWM_SEL 0x02090001U
#define CLK_PMU1PWM_SEL_XIN_OSC0_FUNC 0U
/********Name=PMU1CLKSEL_CON03,Offset=0x30C********/
#define CLK_I2C0_SEL 0x01060001U
#define CLK_I2C0_SEL_CLK_MATRIX_PMU1_100M_SRC 0U
/********Name=PMU1CLKSEL_CON04,Offset=0x310********/
/********Name=PMU1CLKSEL_CON05,Offset=0x314********/
#define SCLK_UART0_SEL 0x02000001U
#define SCLK_UART0_SEL_XIN_OSC0_FUNC 0U
/********Name=PMU1CLKSEL_CON06,Offset=0x318********/
/********Name=PMU1CLKSEL_CON07,Offset=0x31C********/
#define MCLK_I2S1_8CH_TX_SEL 0x02000001U
#define MCLK_I2S1_8CH_TX_SEL_XIN_OSC0_HALF 0U
/********Name=PMU1CLKSEL_CON08,Offset=0x320********/
/********Name=PMU1CLKSEL_CON09,Offset=0x324********/
#define MCLK_I2S1_8CH_RX_SEL 0x02000001U
#define MCLK_I2S1_8CH_RX_SEL_XIN_OSC0_HALF 0U
#define I2S1_8CH_MCLKOUT_SEL 0x02020001U
#define I2S1_8CH_MCLKOUT_SEL_XIN_OSC0_HALF 0U
#define MCLK_PDM0_SEL 0x01040001U
#define MCLK_PDM0_SEL_CLK_MATRIX_PMU1_200M_SRC 0U
#define CLK_USBDP_COMBO_PHY0_REF_XTAL_SEL 0x010A0001U
#define CLK_USBDP_COMBO_PHY0_REF_XTAL_SEL_CLK_PPLL 0U
/********Name=PMU1CLKSEL_CON12,Offset=0x330********/
#define CLK_HDPTX0_REF_XTAL_SEL 0x010B0001U
#define CLK_HDPTX0_REF_XTAL_SEL_CLK_PPLL 0U
/********Name=PMU1CLKSEL_CON14,Offset=0x338********/
#define CLK_REF_MIPI_DCPHY0_SEL 0x02070001U
#define CLK_REF_MIPI_DCPHY0_SEL_CLK_SPLL_MUX 0U
#define CLK_OTGPHY_U3_0_SEL 0x010E0001U
#define CLK_OTGPHY_U3_0_SEL_CLK_PPLL 0U
/********Name=PMU1CLKSEL_CON15,Offset=0x33C********/
#define CLK_CR_PARA_SEL 0x02050001U
#define CLK_CR_PARA_SEL_CLK_SPLL_MUX 0U
/********Name=PMU1CLKSEL_CON17,Offset=0x344********/
#define DBCLK_GPIO0_SEL 0x01000001U
#define DBCLK_GPIO0_SEL_CLK_DEEPSLOW 0U

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __RK3588_H */
