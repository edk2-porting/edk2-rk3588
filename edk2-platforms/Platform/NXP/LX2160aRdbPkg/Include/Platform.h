/** @file
 *  Platform headers
 *
 *  Copyright 2020 NXP
 *  Copyright 2020 Puresoftware Ltd
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
**/


#ifndef LX2160ARDB_PLATFORM_H
#define LX2160ARDB_PLATFORM_H

#define EFI_ACPI_ARM_OEM_REVISION       0x00000000

// Soc defines
#define SVR_SOC_VER(svr)        (((svr) >> 8) & 0xFFFFFE)
#define SVR_MAJOR(svr)          (((svr) >> 4) & 0xf)
#define SVR_MINOR(svr)          (((svr) >> 0) & 0xf)

#define SVR_LX2160A             0x873600

// PCLK
#define DCFG_BASE   0x1E00000
#define DCFG_LEN    0x1FFFF

// Gic
#define GIC_VERSION 3
#define GICD_BASE   0x6000000
#define GICI_BASE   0x6020000
#define GICR_BASE   0x06200000
#define GICR_LEN    0x200000
#define GICC_BASE   0x0c0c0000
#define GICH_BASE   0x0c0d0000
#define GICV_BASE   0x0c0e0000

// UART
#define UART0_BASE               0x21C0000
#define UART1_BASE               0x21D0000
#define UART2_BASE               0x21E0000
#define UART3_BASE               0x21F0000
#define UART0_IT                 64
#define UART1_IT                 65
#define UART2_IT                 104
#define UART3_IT                 105
#define UART_LEN                 0x10000
#define SPCR_FLOW_CONTROL_NONE   0

// Timer
#define TIMER_BLOCK_COUNT            1
#define TIMER_FRAME_COUNT            4
#define TIMER_WATCHDOG_COUNT         1
#define TIMER_BASE_ADDRESS           0x23E0000 // a.k.a CNTControlBase
#define TIMER_READ_BASE_ADDRESS      0x23F0000 // a.k.a CNTReadBase
#define TIMER_GT_BLOCK_0_ADDRESS     0x2890000 // a.k.a CNTCTLBase (Secure)
#define TIMER_GT_BASE_0_ADDRESS      0x28A0000  // a.k.a CNTBase0
#define TIMER_GT_BASE_1_ADDRESS      0x28B0000  // a.k.a CNTBase1
#define TIMER_GT_BASE_2_ADDRESS      0x28C0000  // a.k.a CNTBase2
#define TIMER_GT_BASE_3_ADDRESS      0x28D0000  // a.k.a CNTBase3
#define TIMER_GT_BASE_0_EL0_ADDRESS  0x28E0000  // a.k.a CNTBase0EL0
#define TIMER_GT_BASE_2_EL0_ADDRESS  0x28F0000  // a.k.a CNTBase2EL0
#define TIMER_WDT0_REFRESH_BASE      0x2390000
#define TIMER_WDT0_CONTROL_BASE      0x23A0000
#define TIMER_SEC_IT                 29
#define TIMER_NON_SEC_IT             30
#define TIMER_VIRT_IT                27
#define TIMER_HYP_IT                 26
#define TIMER_FRAME0_IT              78
#define TIMER_FRAME1_IT              79
#define TIMER_FRAME2_IT              92
#define TIMER_FRAME3_IT              93
#define TIMER_WDT0_IT                91

#define DEFAULT_PLAT_FREQ            700000000

// Mcfg
#define LX2160A_PCI_SEG0_CONFIG_BASE 0x9000000000
#define LX2160A_PCI_SEG0             0x2
#define LX2160A_PCI_SEG_BUSNUM_MIN   0x0
#define LX2160A_PCI_SEG_BUSNUM_MAX   0xff
#define LX2160A_PCI_SEG1_CONFIG_BASE 0xA000000000
#define LX2160A_PCI_SEG1             0x4

// Platform specific info needed by Configuration Manager

#define OEM_ACPI_TABLES   1 // OEM defined DSDT

#define CFG_MGR_TABLE_ID  SIGNATURE_64 ('L','X','2','1','6','0',' ',' ')

#define PLAT_PCI_SEG0_CONFIG_BASE   LX2160A_PCI_SEG0_CONFIG_BASE
#define PLAT_PCI_SEG0               LX2160A_PCI_SEG0
#define PLAT_PCI_SEG_BUSNUM_MIN     LX2160A_PCI_SEG_BUSNUM_MIN
#define PLAT_PCI_SEG_BUSNUM_MAX     LX2160A_PCI_SEG_BUSNUM_MAX
#define PLAT_PCI_SEG1_CONFIG_BASE   LX2160A_PCI_SEG1_CONFIG_BASE
#define PLAT_PCI_SEG1               LX2160A_PCI_SEG1

#define PLAT_GIC_VERSION            GIC_VERSION
#define PLAT_GICD_BASE              GICD_BASE
#define PLAT_GICI_BASE              GICI_BASE
#define PLAT_GICR_BASE              GICR_BASE
#define PLAT_GICR_LEN               GICR_LEN
#define PLAT_GICC_BASE              GICC_BASE
#define PLAT_GICH_BASE              GICH_BASE
#define PLAT_GICV_BASE              GICV_BASE

#define PLAT_CPU_COUNT          16
#define PLAT_GTBLOCK_COUNT      1
#define PLAT_GTFRAME_COUNT      4
#define PLAT_PCI_CONFG_COUNT    2

#define PLAT_WATCHDOG_COUNT           1
#define PLAT_GIC_REDISTRIBUTOR_COUNT  1
#define PLAT_GIC_ITS_COUNT            1

/* GIC CPU Interface information
   GIC_ENTRY (CPUInterfaceNumber, Mpidr, PmuIrq, VGicIrq, EnergyEfficiency)
 */
#define PLAT_GIC_CPU_INTERFACE    {                         \
             GICC_ENTRY (0,  GET_MPID (0, 0), 23, 0x19, 0), \
             GICC_ENTRY (1,  GET_MPID (0, 1), 23, 0x19, 0), \
             GICC_ENTRY (2,  GET_MPID (1, 0), 23, 0x19, 0), \
             GICC_ENTRY (3,  GET_MPID (1, 1), 23, 0x19, 0), \
             GICC_ENTRY (4,  GET_MPID (2, 0), 23, 0x19, 0), \
             GICC_ENTRY (5,  GET_MPID (2, 1), 23, 0x19, 0), \
             GICC_ENTRY (6,  GET_MPID (3, 0), 23, 0x19, 0), \
             GICC_ENTRY (7,  GET_MPID (3, 1), 23, 0x19, 0), \
             GICC_ENTRY (8,  GET_MPID (4, 0), 23, 0x19, 0), \
             GICC_ENTRY (9,  GET_MPID (4, 1), 23, 0x19, 0), \
             GICC_ENTRY (10, GET_MPID (5, 0), 23, 0x19, 0), \
             GICC_ENTRY (11, GET_MPID (5, 1), 23, 0x19, 0), \
             GICC_ENTRY (12, GET_MPID (6, 0), 23, 0x19, 0), \
             GICC_ENTRY (13, GET_MPID (6, 1), 23, 0x19, 0), \
             GICC_ENTRY (14, GET_MPID (7, 0), 23, 0x19, 0), \
             GICC_ENTRY (15, GET_MPID (7, 1), 23, 0x19, 0)  \
}

// watchdogs
#define PLAT_WATCHDOG_INFO                    \
  {                                           \
      TIMER_WDT0_CONTROL_BASE,                \
      TIMER_WDT0_REFRESH_BASE,                \
      TIMER_WDT0_IT,                          \
      SBSA_WATCHDOG_FLAGS                     \
  }                                           \

#define PLAT_TIMER_BLOCK_INFO                                           \
  {                                                                     \
    {                                                                   \
      TIMER_GT_BLOCK_0_ADDRESS,                                         \
      PLAT_GTFRAME_COUNT,                                               \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo +            \
        OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, GTBlock0TimerInfo))  \
    }                                                                   \
  }                                                                     \

#define PLAT_TIMER_FRAME_INFO                                            \
  {                                                                      \
    {                                                                    \
      0,                             /* UINT8 GTFrameNumber */           \
      TIMER_GT_BASE_0_ADDRESS,       /* UINT64 CntBaseX */               \
      TIMER_GT_BASE_0_EL0_ADDRESS,   /* UINT64 CntEL0BaseX */            \
      TIMER_FRAME0_IT,               /* UINT32 GTxPhysicalTimerGSIV */   \
      GTDT_FRAME_FLAGS,              /* UINT32 GTxPhysicalTimerFlags */  \
      TIMER_FRAME0_IT,               /* UINT32 GTxVirtualTimerGSIV */    \
      GTDT_FRAME_FLAGS,              /* UINT32 GTxVirtualTimerFlags */   \
      0                              /* UINT32 GTxCommonFlags */         \
    }, /* Gtdt.Frames[0] */                                              \
    {                                                                    \
      1,                             /* UINT8 GTFrameNumber */           \
      TIMER_GT_BASE_1_ADDRESS,       /* UINT64 CntBaseX */               \
      GT_BLOCK_FRAME_RES_BASE,       /* UINT64 CntEL0BaseX */            \
      TIMER_FRAME1_IT,               /* UINT32 GTxPhysicalTimerGSIV */   \
      GTDT_FRAME_FLAGS,              /* UINT32 GTxPhysicalTimerFlags */  \
      0,                             /* UINT32 GTxVirtualTimerGSIV */    \
      0,                             /* UINT32 GTxVirtualTimerFlags */   \
      GTDT_FRAME_COMMON_FLAGS        /* UINT32 GTxCommonFlags */         \
    }, /* Gtdt.Frames[1] */                                              \
    {                                                                    \
      2,                              /* UINT8 GTFrameNumber */          \
      TIMER_GT_BASE_2_ADDRESS,        /* UINT64 CntBaseX */              \
      TIMER_GT_BASE_2_EL0_ADDRESS,    /* UINT64 CntEL0BaseX */           \
      TIMER_FRAME2_IT,                /* UINT32 GTxPhysicalTimerGSIV */  \
      GTDT_FRAME_FLAGS,               /* UINT32 GTxPhysicalTimerFlags */ \
      0,                              /* UINT32 GTxVirtualTimerGSIV */   \
      0,                              /* UINT32 GTxVirtualTimerFlags */  \
      GTDT_FRAME_COMMON_FLAGS         /* UINT32 GTxCommonFlags */        \
    },/* Gtdt.Frames[2] */                                               \
    {                                                                   \
      3,                             /* UINT8 GTFrameNumber */          \
      TIMER_GT_BASE_3_ADDRESS,       /* UINT64 CntBaseX */              \
      GT_BLOCK_FRAME_RES_BASE,       /* UINT64 CntEL0BaseX */           \
      TIMER_FRAME3_IT,               /* UINT32 GTxPhysicalTimerGSIV */  \
      GTDT_FRAME_FLAGS,              /* UINT32 GTxPhysicalTimerFlags */ \
      0,                             /* UINT32 GTxVirtualTimerGSIV */   \
      0,                             /* UINT32 GTxVirtualTimerFlags */  \
      GTDT_FRAME_COMMON_FLAGS        /* UINT32 GTxCommonFlags */        \
    }, /* Gtdt.Frames[3] */                                             \
  }                                                                     \

#define PLAT_GIC_DISTRIBUTOR_INFO                                      \
  {                                                                    \
    PLAT_GICD_BASE,                  /* UINT64  PhysicalBaseAddress */ \
    0,                               /* UINT32  SystemVectorBase */    \
    PLAT_GIC_VERSION                 /* UINT8   GicVersion */          \
  }                                                                    \

#define PLAT_GIC_REDISTRIBUTOR_INFO                                    \
  {                                                                    \
    PLAT_GICR_BASE,      /* UINT64 DiscoveryRangeBaseAddress */        \
    PLAT_GICR_LEN        /* UINT32 DiscoveryRangeLength */             \
  }                                                                    \

#define PLAT_GIC_ITS_INFO                                                    \
  {                                                                          \
    0,                   /* UINT32 GIC ITS ID */                             \
    PLAT_GICI_BASE,      /* UINT64 The 64-bit physical address for ITS */    \
    0                    /* UINT32 Populate the GIC ITS affinity in SRAT. */ \
  }                                                                          \

#define PLAT_MCFG_INFO                \
  {                                   \
    {                                 \
      PLAT_PCI_SEG0_CONFIG_BASE,      \
      PLAT_PCI_SEG0,                  \
      PLAT_PCI_SEG_BUSNUM_MIN,        \
      PLAT_PCI_SEG_BUSNUM_MAX,        \
    },                                \
    {                                 \
      PLAT_PCI_SEG1_CONFIG_BASE,      \
      PLAT_PCI_SEG1,                  \
      PLAT_PCI_SEG_BUSNUM_MIN,        \
      PLAT_PCI_SEG_BUSNUM_MAX,        \
    }                                 \
  }                                   \

#define PLAT_SPCR_INFO                                                            \
  {                                                                               \
    UART0_BASE,                                                                   \
    UART0_IT,                                                                     \
    115200,                                                                       \
    0,                                                                            \
    EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_INTERFACE_TYPE_ARM_PL011_UART  \
  }                                                                               \

#endif // LX2160ARDB_PLATFORM_H
