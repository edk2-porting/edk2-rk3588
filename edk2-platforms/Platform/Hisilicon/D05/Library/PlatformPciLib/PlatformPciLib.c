/** @file

  Copyright (c) 2016, Hisilicon Limited. All rights reserved.<BR>
  Copyright (c) 2016, Linaro Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/PcdLib.h>
#include <Library/PlatformPciLib.h>

UINT64 pcie_subctrl_base_1610[PCIE_MAX_HOSTBRIDGE][PCIE_MAX_ROOTBRIDGE] = {{0xa0000000, 0xa0000000,0xa0000000,0xa0000000,0x8a0000000,0x8a0000000,0x8a0000000,0x8a0000000},
                                        {0x600a0000000,0x600a0000000,0x600a0000000,0x600a0000000, 0x700a0000000,0x700a0000000,0x700a0000000,0x700a0000000}};
UINT64 PCIE_APB_SLAVE_BASE_1610[PCIE_MAX_HOSTBRIDGE][PCIE_MAX_ROOTBRIDGE] = {{0xa0090000, 0xa0200000, 0xa00a0000, 0xa00b0000, 0x8a0090000, 0x8a0200000, 0x8a00a0000, 0x8a00b0000},
                                         {0x600a0090000, 0x600a0200000, 0x600a00a0000, 0x600a00b0000, 0x700a0090000, 0x700a0200000, 0x700a00a0000, 0x700a00b0000}};
UINT64 PCIE_PHY_BASE_1610 [PCIE_MAX_HOSTBRIDGE][PCIE_MAX_ROOTBRIDGE] = {{0xa00c0000, 0xa00d0000, 0xa00e0000, 0xa00f0000, 0x8a00c0000, 0x8a00d0000, 0x8a00e0000, 0x8a00f0000},
                                 {0x600a00c0000, 0x600a00d0000, 0x600a00e0000, 0x600a00f0000, 0x700a00c0000, 0x700a00d0000, 0x700a00e0000, 0x700a00f0000}};
UINT64 PCIE_ITS_1610[PCIE_MAX_HOSTBRIDGE][PCIE_MAX_ROOTBRIDGE] = {{0xc6010040, 0xc6010040, 0xc6010040, 0xc6010040, 0x8c6010040, 0x8c6010040, 0x8c6010040, 0x8c6010040},
                           {0x400C6010040, 0x400C6010040, 0x400C6010040, 0x400C6010040, 0x408C6010040, 0x408C6010040, 0x408C6010040, 0x408C6010040}};

PCI_ROOT_BRIDGE_RESOURCE_APPETURE  mResAppeture[PCIE_MAX_HOSTBRIDGE][PCIE_MAX_ROOTBRIDGE] = {
 {// HostBridge 0
  /* Port 0 */
  {
      0,                    //Segment
      PCI_HB0RB0_ECAM_BASE, //ecam
      0x80,  //BusBase
      0x87, //BusLimit
      PCI_HB0RB0_IO_BASE,  //IoBase
      (PCI_HB0RB0_CPUIOREGIONBASE + PCI_HB0RB0_IO_SIZE - 1), //IoLimit
      PCI_HB0RB0_CPUMEMREGIONBASE, //CpuMemRegionBase
      PCI_HB0RB0_CPUIOREGIONBASE,  //CpuIoRegionBase
      (PCI_HB0RB0_PCI_BASE),//RbPciBar
      PCI_HB0RB0_PCIREGION_BASE, //PciRegionbase
      PCI_HB0RB0_PCIREGION_BASE + PCI_HB0RB0_PCIREGION_SIZE - 1 //PciRegionlimit
  },
  /* Port 1 */
  {
      1,                   //Segment
      PCI_HB0RB1_ECAM_BASE,//ecam
      0x90,  //BusBase
      0x97, //BusLimit
      (PCI_HB0RB1_IO_BASE),  //IoBase
      (PCI_HB0RB1_CPUIOREGIONBASE + PCI_HB0RB1_IO_SIZE - 1), //IoLimit
      PCI_HB0RB1_CPUMEMREGIONBASE, //CpuMemRegionBase
      PCI_HB0RB1_CPUIOREGIONBASE,  //CpuIoRegionBase
      (PCI_HB0RB1_PCI_BASE),  //RbPciBar
      PCI_HB0RB1_PCIREGION_BASE, //PciRegionbase
      PCI_HB0RB1_PCIREGION_BASE + PCI_HB0RB1_PCIREGION_SIZE - 1 //PciRegionlimit
  },
  /* Port 2 */
  {
      2,                   //Segment
      PCI_HB0RB2_ECAM_BASE,
      0xF8,  //BusBase
      0xFF, //BusLimit
      (PCI_HB0RB2_IO_BASE),  //IOBase
      (PCI_HB0RB2_CPUIOREGIONBASE + PCI_HB0RB2_IO_SIZE - 1), //IoLimit
      PCI_HB0RB2_CPUMEMREGIONBASE, //CpuMemRegionBase
      PCI_HB0RB2_CPUIOREGIONBASE,  //CpuIoRegionBase
      (PCI_HB0RB2_PCI_BASE),  //RbPciBar
      PCI_HB0RB2_PCIREGION_BASE, //PciRegionbase
      PCI_HB0RB2_PCIREGION_BASE + PCI_HB0RB2_PCIREGION_SIZE - 1 //PciRegionlimit
  },

  /* Port 3 */
  {
      3,                   //Segment
      PCI_HB0RB3_ECAM_BASE,
      0xb0,  //BusBase
      0xb7, //BusLimit
      (PCI_HB0RB3_IO_BASE), //IoBase
      (PCI_HB0RB3_CPUIOREGIONBASE + PCI_HB0RB3_IO_SIZE - 1),  //IoLimit
      PCI_HB0RB3_CPUMEMREGIONBASE,
      PCI_HB0RB3_CPUIOREGIONBASE,
      (PCI_HB0RB3_PCI_BASE),  //RbPciBar
      PCI_HB0RB3_PCIREGION_BASE, //PciRegionbase
      PCI_HB0RB3_PCIREGION_BASE + PCI_HB0RB3_PCIREGION_SIZE - 1 //PciRegionlimit
  },
  /* Port 4 */
  {
      4,                   //Segment
      PCI_HB0RB4_ECAM_BASE, //ecam
      0x88,  //BusBase
      0x8f, //BusLimit
      PCI_HB0RB4_IO_BASE,  //IoBase
      (PCI_HB0RB4_CPUIOREGIONBASE + PCI_HB0RB4_IO_SIZE - 1), //IoLimit
      PCI_HB0RB4_CPUMEMREGIONBASE, //CpuMemRegionBase
      PCI_HB0RB4_CPUIOREGIONBASE,  //CpuIoRegionBase
      (PCI_HB0RB4_PCI_BASE),  //RbPciBar
      PCI_HB0RB4_PCIREGION_BASE, //PciRegionbase
      PCI_HB0RB4_PCIREGION_BASE + PCI_HB0RB4_PCIREGION_SIZE - 1 //PciRegionlimit
  },
  /* Port 5 */
  {
      5,                   //Segment
      PCI_HB0RB5_ECAM_BASE,//ecam
      0x78,  //BusBase
      0x7F, //BusLimit
      (PCI_HB0RB5_IO_BASE),  //IoBase
      (PCI_HB0RB5_CPUIOREGIONBASE + PCI_HB0RB5_IO_SIZE - 1), //IoLimit
      PCI_HB0RB5_CPUMEMREGIONBASE, //CpuMemRegionBase
      PCI_HB0RB5_CPUIOREGIONBASE,  //CpuIoRegionBase
      (PCI_HB0RB5_PCI_BASE),  //RbPciBar
      PCI_HB0RB5_PCIREGION_BASE, //PciRegionbase
      PCI_HB0RB5_PCIREGION_BASE + PCI_HB0RB5_PCIREGION_SIZE - 1 //PciRegionlimit
  },
  /* Port 6 */
  {
      6,                   //Segment
      PCI_HB0RB6_ECAM_BASE,
      0xC0,  //BusBase
      0xC7, //BusLimit
      (PCI_HB0RB6_IO_BASE),  //IOBase
      (PCI_HB0RB6_CPUIOREGIONBASE + PCI_HB0RB6_IO_SIZE - 1), //IoLimit
      PCI_HB0RB6_CPUMEMREGIONBASE, //CpuMemRegionBase
      PCI_HB0RB6_CPUIOREGIONBASE,  //CpuIoRegionBase
      (PCI_HB0RB6_PCI_BASE),  //RbPciBar
      PCI_HB0RB6_PCIREGION_BASE, //PciRegionbase
      PCI_HB0RB6_PCIREGION_BASE + PCI_HB0RB6_PCIREGION_SIZE - 1 //PciRegionlimit
  },

  /* Port 7 */
  {
      7,                   //Segment
      PCI_HB0RB7_ECAM_BASE,
      0x90,  //BusBase
      0x97, //BusLimit
      (PCI_HB0RB7_IO_BASE), //IoBase
      (PCI_HB0RB7_CPUIOREGIONBASE + PCI_HB0RB7_IO_SIZE - 1),  //IoLimit
      PCI_HB0RB7_CPUMEMREGIONBASE,
      PCI_HB0RB7_CPUIOREGIONBASE,
      (PCI_HB0RB7_PCI_BASE),  //RbPciBar
      PCI_HB0RB7_PCIREGION_BASE, //PciRegionbase
      PCI_HB0RB7_PCIREGION_BASE + PCI_HB0RB7_PCIREGION_SIZE - 1 //PciRegionlimit
  }
 },
{// HostBridge 1
  /* Port 0 */
  {
      8,                   //Segment
      PCI_HB1RB0_ECAM_BASE,
      0x80,  //BusBase
      0x87, //BusLimit
      PCI_HB1RB0_IO_BASE, //IoBase
      (PCI_HB1RB0_CPUIOREGIONBASE + PCI_HB1RB0_IO_SIZE - 1), //IoLimit
      PCI_HB1RB0_CPUMEMREGIONBASE, //CpuMemRegionBase
      PCI_HB1RB0_CPUIOREGIONBASE,  //CpuIoRegionBase
      (PCI_HB1RB0_PCI_BASE),  //RbPciBar
      PCI_HB1RB0_PCIREGION_BASE, //PciRegionbase
      PCI_HB1RB0_PCIREGION_BASE + PCI_HB1RB0_PCIREGION_SIZE - 1 //PciRegionlimit
  },
  /* Port 1 */
  {
      9,                   //Segment
      PCI_HB1RB1_ECAM_BASE,
      0x90,  //BusBase
      0x97, //BusLimit
      PCI_HB1RB1_IO_BASE, //IoBase
      (PCI_HB1RB1_CPUIOREGIONBASE + PCI_HB1RB1_IO_SIZE - 1), //IoLimit
      PCI_HB1RB1_CPUMEMREGIONBASE, //CpuMemRegionBase
      PCI_HB1RB1_CPUIOREGIONBASE,  //CpuIoRegionBase
      (PCI_HB1RB1_PCI_BASE),  //RbPciBar
      PCI_HB1RB1_PCIREGION_BASE, //PciRegionbase
      PCI_HB1RB1_PCIREGION_BASE + PCI_HB1RB1_PCIREGION_SIZE - 1 //PciRegionlimit
  },
  /* Port 2 */
  {
      0xa,                  //Segment
      PCI_HB1RB2_ECAM_BASE,
      0x10,  //BusBase
      0x1f, //BusLimit
      PCI_HB1RB2_IO_BASE, //IoBase
      (PCI_HB1RB2_CPUIOREGIONBASE + PCI_HB1RB2_IO_SIZE - 1), //IoLimit
      PCI_HB1RB2_CPUMEMREGIONBASE, //CpuMemRegionBase
      PCI_HB1RB2_CPUIOREGIONBASE,  //CpuIoRegionBase
      (PCI_HB1RB2_PCI_BASE),  //RbPciBar
      PCI_HB1RB2_PCIREGION_BASE, //PciRegionbase
      PCI_HB1RB2_PCIREGION_BASE + PCI_HB1RB2_PCIREGION_SIZE - 1 //PciRegionlimit
  },

  /* Port 3 */
  {
      0xb,                  //Segment
      PCI_HB1RB3_ECAM_BASE,
      0xb0,  //BusBase
      0xb7, //BusLimit
      PCI_HB1RB3_IO_BASE, //IoBase
      (PCI_HB1RB3_CPUIOREGIONBASE + PCI_HB1RB3_IO_SIZE - 1), //IoLimit
      PCI_HB1RB3_CPUMEMREGIONBASE, //CpuMemRegionBase
      PCI_HB1RB3_CPUIOREGIONBASE,  //CpuIoRegionBase
      (PCI_HB1RB3_PCI_BASE),  //RbPciBar
      PCI_HB1RB3_PCIREGION_BASE, //PciRegionbase
      PCI_HB1RB3_PCIREGION_BASE + PCI_HB1RB3_PCIREGION_SIZE - 1 //PciRegionlimit
  },
  /* Port 4 */
  {
      0xc,                  //Segment
      PCI_HB1RB4_ECAM_BASE,
      0x20,  //BusBase
      0x2f, //BusLimit
      PCI_HB1RB4_IO_BASE, //IoBase
      (PCI_HB1RB4_CPUIOREGIONBASE + PCI_HB1RB4_IO_SIZE - 1), //IoLimit
      PCI_HB1RB4_CPUMEMREGIONBASE, //CpuMemRegionBase
      PCI_HB1RB4_CPUIOREGIONBASE,  //CpuIoRegionBase
      (PCI_HB1RB4_PCI_BASE),  //RbPciBar
      PCI_HB1RB4_PCIREGION_BASE, //PciRegionbase
      PCI_HB1RB4_PCIREGION_BASE + PCI_HB1RB4_PCIREGION_SIZE - 1 //PciRegionlimit
  },
  /* Port 5 */
  {
      0xd,                  //Segment
      PCI_HB1RB5_ECAM_BASE,
      0x30,  //BusBase
      0x3f, //BusLimit
      PCI_HB1RB5_IO_BASE, //IoBase
      (PCI_HB1RB5_CPUIOREGIONBASE + PCI_HB1RB5_IO_SIZE - 1), //IoLimit
      PCI_HB1RB5_CPUMEMREGIONBASE, //CpuMemRegionBase
      PCI_HB1RB5_CPUIOREGIONBASE,  //CpuIoRegionBase
      (PCI_HB1RB5_PCI_BASE),  //RbPciBar
      PCI_HB1RB5_PCIREGION_BASE, //PciRegionbase
      PCI_HB1RB5_PCIREGION_BASE + PCI_HB1RB5_PCIREGION_SIZE - 1 //PciRegionlimit
  },
  /* Port 6 */
  {
      0xe,                  //Segment
      PCI_HB1RB6_ECAM_BASE,
      0xa8,  //BusBase
      0xaf, //BusLimit
      PCI_HB1RB6_IO_BASE, //IoBase
      (PCI_HB1RB6_CPUIOREGIONBASE + PCI_HB1RB6_IO_SIZE - 1), //IoLimit
      PCI_HB1RB6_CPUMEMREGIONBASE, //CpuMemRegionBase
      PCI_HB1RB6_CPUIOREGIONBASE,  //CpuIoRegionBase
      (PCI_HB1RB6_PCI_BASE),  //RbPciBar
      PCI_HB1RB6_PCIREGION_BASE, //PciRegionbase
      PCI_HB1RB6_PCIREGION_BASE + PCI_HB1RB6_PCIREGION_SIZE - 1 //PciRegionlimit
  },

  /* Port 7 */
  {
      0xf,                  //Segment
      PCI_HB1RB7_ECAM_BASE,
      0xb8,  //BusBase
      0xbf, //BusLimit
      PCI_HB1RB7_IO_BASE, //IoBase
      (PCI_HB1RB7_CPUIOREGIONBASE + PCI_HB1RB7_IO_SIZE - 1), //IoLimit
      PCI_HB1RB7_CPUMEMREGIONBASE, //CpuMemRegionBase
      PCI_HB1RB7_CPUIOREGIONBASE,  //CpuIoRegionBase
      (PCI_HB1RB7_PCI_BASE),  //RbPciBar
      PCI_HB1RB7_PCIREGION_BASE, //PciRegionbase
      PCI_HB1RB7_PCIREGION_BASE + PCI_HB1RB7_PCIREGION_SIZE - 1 //PciRegionlimit
  }

 }
};

