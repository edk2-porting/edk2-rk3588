/** @file
*
*  Copyright (c) 2016, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2016, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _PLATFORM_PCI_LIB_H_
#define _PLATFORM_PCI_LIB_H_

#define PCIE_MAX_HOSTBRIDGE      2
#define PCIE_MAX_ROOTBRIDGE      8
//The extern pcie addresses will be initialized by oemmisclib
extern UINT64 pcie_subctrl_base_1610[PCIE_MAX_HOSTBRIDGE][PCIE_MAX_ROOTBRIDGE];
extern UINT64 PCIE_APB_SLAVE_BASE_1610[PCIE_MAX_HOSTBRIDGE][PCIE_MAX_ROOTBRIDGE];
extern UINT64 PCIE_PHY_BASE_1610[PCIE_MAX_HOSTBRIDGE][PCIE_MAX_ROOTBRIDGE];
extern UINT64 PCIE_ITS_1610[PCIE_MAX_HOSTBRIDGE][PCIE_MAX_ROOTBRIDGE];


#define PCI_HB0RB0_PCI_BASE        FixedPcdGet64(PciHb0Rb0Base)
#define PCI_HB0RB1_PCI_BASE        FixedPcdGet64(PciHb0Rb1Base)
#define PCI_HB0RB2_PCI_BASE        FixedPcdGet64(PciHb0Rb2Base)
#define PCI_HB0RB3_PCI_BASE        FixedPcdGet64(PciHb0Rb3Base)
#define PCI_HB0RB4_PCI_BASE        FixedPcdGet64(PciHb0Rb4Base)
#define PCI_HB0RB5_PCI_BASE        FixedPcdGet64(PciHb0Rb5Base)
#define PCI_HB0RB6_PCI_BASE        FixedPcdGet64(PciHb0Rb6Base)
#define PCI_HB0RB7_PCI_BASE        FixedPcdGet64(PciHb0Rb7Base)

#define PCI_HB1RB0_PCI_BASE        FixedPcdGet64(PciHb1Rb0Base)
#define PCI_HB1RB1_PCI_BASE        FixedPcdGet64(PciHb1Rb1Base)
#define PCI_HB1RB2_PCI_BASE        FixedPcdGet64(PciHb1Rb2Base)
#define PCI_HB1RB3_PCI_BASE        FixedPcdGet64(PciHb1Rb3Base)
#define PCI_HB1RB4_PCI_BASE        FixedPcdGet64(PciHb1Rb4Base)
#define PCI_HB1RB5_PCI_BASE        FixedPcdGet64(PciHb1Rb5Base)
#define PCI_HB1RB6_PCI_BASE        FixedPcdGet64(PciHb1Rb6Base)
#define PCI_HB1RB7_PCI_BASE        FixedPcdGet64(PciHb1Rb7Base)

#define PCI_HB0RB0_ECAM_BASE      FixedPcdGet64 (PcdHb0Rb0PciConfigurationSpaceBaseAddress)
#define PCI_HB0RB0_ECAM_SIZE      FixedPcdGet64 (PcdHb0Rb0PciConfigurationSpaceSize)
#define PCI_HB0RB1_ECAM_BASE      FixedPcdGet64 (PcdHb0Rb1PciConfigurationSpaceBaseAddress)
#define PCI_HB0RB1_ECAM_SIZE      FixedPcdGet64 (PcdHb0Rb1PciConfigurationSpaceSize)
#define PCI_HB0RB2_ECAM_BASE      FixedPcdGet64 (PcdHb0Rb2PciConfigurationSpaceBaseAddress)
#define PCI_HB0RB2_ECAM_SIZE      FixedPcdGet64 (PcdHb0Rb2PciConfigurationSpaceSize)
#define PCI_HB0RB3_ECAM_BASE      FixedPcdGet64 (PcdHb0Rb3PciConfigurationSpaceBaseAddress)
#define PCI_HB0RB3_ECAM_SIZE      FixedPcdGet64 (PcdHb0Rb3PciConfigurationSpaceSize)
#define PCI_HB0RB4_ECAM_BASE      FixedPcdGet64 (PcdHb0Rb4PciConfigurationSpaceBaseAddress)
#define PCI_HB0RB4_ECAM_SIZE      FixedPcdGet64 (PcdHb0Rb4PciConfigurationSpaceSize)
#define PCI_HB0RB5_ECAM_BASE      FixedPcdGet64 (PcdHb0Rb5PciConfigurationSpaceBaseAddress)
#define PCI_HB0RB5_ECAM_SIZE      FixedPcdGet64 (PcdHb0Rb5PciConfigurationSpaceSize)
#define PCI_HB0RB6_ECAM_BASE      FixedPcdGet64 (PcdHb0Rb6PciConfigurationSpaceBaseAddress)
#define PCI_HB0RB6_ECAM_SIZE      FixedPcdGet64 (PcdHb0Rb6PciConfigurationSpaceSize)
#define PCI_HB0RB7_ECAM_BASE      FixedPcdGet64 (PcdHb0Rb7PciConfigurationSpaceBaseAddress)
#define PCI_HB0RB7_ECAM_SIZE      FixedPcdGet64 (PcdHb0Rb7PciConfigurationSpaceSize)

#define PCI_HB1RB0_ECAM_BASE      FixedPcdGet64 (PcdHb1Rb0PciConfigurationSpaceBaseAddress)
#define PCI_HB1RB0_ECAM_SIZE      FixedPcdGet64 (PcdHb1Rb0PciConfigurationSpaceSize)
#define PCI_HB1RB1_ECAM_BASE      FixedPcdGet64 (PcdHb1Rb1PciConfigurationSpaceBaseAddress)
#define PCI_HB1RB1_ECAM_SIZE      FixedPcdGet64 (PcdHb1Rb1PciConfigurationSpaceSize)
#define PCI_HB1RB2_ECAM_BASE      FixedPcdGet64 (PcdHb1Rb2PciConfigurationSpaceBaseAddress)
#define PCI_HB1RB2_ECAM_SIZE      FixedPcdGet64 (PcdHb1Rb2PciConfigurationSpaceSize)
#define PCI_HB1RB3_ECAM_BASE      FixedPcdGet64 (PcdHb1Rb3PciConfigurationSpaceBaseAddress)
#define PCI_HB1RB3_ECAM_SIZE      FixedPcdGet64 (PcdHb1Rb3PciConfigurationSpaceSize)
#define PCI_HB1RB4_ECAM_BASE      FixedPcdGet64 (PcdHb1Rb4PciConfigurationSpaceBaseAddress)
#define PCI_HB1RB4_ECAM_SIZE      FixedPcdGet64 (PcdHb1Rb4PciConfigurationSpaceSize)
#define PCI_HB1RB5_ECAM_BASE      FixedPcdGet64 (PcdHb1Rb5PciConfigurationSpaceBaseAddress)
#define PCI_HB1RB5_ECAM_SIZE      FixedPcdGet64 (PcdHb1Rb5PciConfigurationSpaceSize)
#define PCI_HB1RB6_ECAM_BASE      FixedPcdGet64 (PcdHb1Rb6PciConfigurationSpaceBaseAddress)
#define PCI_HB1RB6_ECAM_SIZE      FixedPcdGet64 (PcdHb1Rb6PciConfigurationSpaceSize)
#define PCI_HB1RB7_ECAM_BASE      FixedPcdGet64 (PcdHb1Rb7PciConfigurationSpaceBaseAddress)
#define PCI_HB1RB7_ECAM_SIZE      FixedPcdGet64 (PcdHb1Rb7PciConfigurationSpaceSize)

#define PCI_HB0RB0_PCIREGION_BASE (FixedPcdGet64 (PcdHb0Rb0PciRegionBaseAddress))
#define PCI_HB0RB0_PCIREGION_SIZE (FixedPcdGet64 (PcdHb0Rb0PciRegionSize))
#define PCI_HB0RB1_PCIREGION_BASE (FixedPcdGet64 (PcdHb0Rb1PciRegionBaseAddress))
#define PCI_HB0RB1_PCIREGION_SIZE (FixedPcdGet64 (PcdHb0Rb1PciRegionSize))
#define PCI_HB0RB2_PCIREGION_BASE (FixedPcdGet64 (PcdHb0Rb2PciRegionBaseAddress))
#define PCI_HB0RB2_PCIREGION_SIZE (FixedPcdGet64 (PcdHb0Rb2PciRegionSize))
#define PCI_HB0RB3_PCIREGION_BASE (FixedPcdGet64 (PcdHb0Rb3PciRegionBaseAddress))
#define PCI_HB0RB3_PCIREGION_SIZE (FixedPcdGet64 (PcdHb0Rb3PciRegionSize))
#define PCI_HB0RB4_PCIREGION_BASE (FixedPcdGet64 (PcdHb0Rb4PciRegionBaseAddress))
#define PCI_HB0RB4_PCIREGION_SIZE (FixedPcdGet64 (PcdHb0Rb4PciRegionSize))
#define PCI_HB0RB5_PCIREGION_BASE (FixedPcdGet64 (PcdHb0Rb5PciRegionBaseAddress))
#define PCI_HB0RB5_PCIREGION_SIZE (FixedPcdGet64 (PcdHb0Rb5PciRegionSize))
#define PCI_HB0RB6_PCIREGION_BASE (FixedPcdGet64 (PcdHb0Rb6PciRegionBaseAddress))
#define PCI_HB0RB6_PCIREGION_SIZE (FixedPcdGet64 (PcdHb0Rb6PciRegionSize))
#define PCI_HB0RB7_PCIREGION_BASE (FixedPcdGet64 (PcdHb0Rb7PciRegionBaseAddress))
#define PCI_HB0RB7_PCIREGION_SIZE (FixedPcdGet64 (PcdHb0Rb7PciRegionSize))

#define PCI_HB1RB0_PCIREGION_BASE (FixedPcdGet64 (PcdHb1Rb0PciRegionBaseAddress))
#define PCI_HB1RB0_PCIREGION_SIZE (FixedPcdGet64 (PcdHb1Rb0PciRegionSize))
#define PCI_HB1RB1_PCIREGION_BASE (FixedPcdGet64 (PcdHb1Rb1PciRegionBaseAddress))
#define PCI_HB1RB1_PCIREGION_SIZE (FixedPcdGet64 (PcdHb1Rb1PciRegionSize))
#define PCI_HB1RB2_PCIREGION_BASE (FixedPcdGet64 (PcdHb1Rb2PciRegionBaseAddress))
#define PCI_HB1RB2_PCIREGION_SIZE (FixedPcdGet64 (PcdHb1Rb2PciRegionSize))
#define PCI_HB1RB3_PCIREGION_BASE (FixedPcdGet64 (PcdHb1Rb3PciRegionBaseAddress))
#define PCI_HB1RB3_PCIREGION_SIZE (FixedPcdGet64 (PcdHb1Rb3PciRegionSize))
#define PCI_HB1RB4_PCIREGION_BASE (FixedPcdGet64 (PcdHb1Rb4PciRegionBaseAddress))
#define PCI_HB1RB4_PCIREGION_SIZE (FixedPcdGet64 (PcdHb1Rb4PciRegionSize))
#define PCI_HB1RB5_PCIREGION_BASE (FixedPcdGet64 (PcdHb1Rb5PciRegionBaseAddress))
#define PCI_HB1RB5_PCIREGION_SIZE (FixedPcdGet64 (PcdHb1Rb5PciRegionSize))
#define PCI_HB1RB6_PCIREGION_BASE (FixedPcdGet64 (PcdHb1Rb6PciRegionBaseAddress))
#define PCI_HB1RB6_PCIREGION_SIZE (FixedPcdGet64 (PcdHb1Rb6PciRegionSize))
#define PCI_HB1RB7_PCIREGION_BASE (FixedPcdGet64 (PcdHb1Rb7PciRegionBaseAddress))
#define PCI_HB1RB7_PCIREGION_SIZE (FixedPcdGet64 (PcdHb1Rb7PciRegionSize))


#define PCI_HB0RB0_CPUMEMREGIONBASE (FixedPcdGet64 (PcdHb0Rb0CpuMemRegionBase))
#define PCI_HB0RB1_CPUMEMREGIONBASE (FixedPcdGet64 (PcdHb0Rb1CpuMemRegionBase))
#define PCI_HB0RB2_CPUMEMREGIONBASE (FixedPcdGet64 (PcdHb0Rb2CpuMemRegionBase))
#define PCI_HB0RB3_CPUMEMREGIONBASE (FixedPcdGet64 (PcdHb0Rb3CpuMemRegionBase))
#define PCI_HB0RB4_CPUMEMREGIONBASE (FixedPcdGet64 (PcdHb0Rb4CpuMemRegionBase))
#define PCI_HB0RB5_CPUMEMREGIONBASE (FixedPcdGet64 (PcdHb0Rb5CpuMemRegionBase))
#define PCI_HB0RB6_CPUMEMREGIONBASE (FixedPcdGet64 (PcdHb0Rb6CpuMemRegionBase))
#define PCI_HB0RB7_CPUMEMREGIONBASE (FixedPcdGet64 (PcdHb0Rb7CpuMemRegionBase))

#define PCI_HB1RB0_CPUMEMREGIONBASE (FixedPcdGet64 (PcdHb1Rb0CpuMemRegionBase))
#define PCI_HB1RB1_CPUMEMREGIONBASE (FixedPcdGet64 (PcdHb1Rb1CpuMemRegionBase))
#define PCI_HB1RB2_CPUMEMREGIONBASE (FixedPcdGet64 (PcdHb1Rb2CpuMemRegionBase))
#define PCI_HB1RB3_CPUMEMREGIONBASE (FixedPcdGet64 (PcdHb1Rb3CpuMemRegionBase))
#define PCI_HB1RB4_CPUMEMREGIONBASE (FixedPcdGet64 (PcdHb1Rb4CpuMemRegionBase))
#define PCI_HB1RB5_CPUMEMREGIONBASE (FixedPcdGet64 (PcdHb1Rb5CpuMemRegionBase))
#define PCI_HB1RB6_CPUMEMREGIONBASE (FixedPcdGet64 (PcdHb1Rb6CpuMemRegionBase))
#define PCI_HB1RB7_CPUMEMREGIONBASE (FixedPcdGet64 (PcdHb1Rb7CpuMemRegionBase))


#define PCI_HB0RB0_CPUIOREGIONBASE (FixedPcdGet64 (PcdHb0Rb0CpuIoRegionBase))
#define PCI_HB0RB1_CPUIOREGIONBASE (FixedPcdGet64 (PcdHb0Rb1CpuIoRegionBase))
#define PCI_HB0RB2_CPUIOREGIONBASE (FixedPcdGet64 (PcdHb0Rb2CpuIoRegionBase))
#define PCI_HB0RB3_CPUIOREGIONBASE (FixedPcdGet64 (PcdHb0Rb3CpuIoRegionBase))
#define PCI_HB0RB4_CPUIOREGIONBASE (FixedPcdGet64 (PcdHb0Rb4CpuIoRegionBase))
#define PCI_HB0RB5_CPUIOREGIONBASE (FixedPcdGet64 (PcdHb0Rb5CpuIoRegionBase))
#define PCI_HB0RB6_CPUIOREGIONBASE (FixedPcdGet64 (PcdHb0Rb6CpuIoRegionBase))
#define PCI_HB0RB7_CPUIOREGIONBASE (FixedPcdGet64 (PcdHb0Rb7CpuIoRegionBase))

#define PCI_HB1RB0_CPUIOREGIONBASE (FixedPcdGet64 (PcdHb1Rb0CpuIoRegionBase))
#define PCI_HB1RB1_CPUIOREGIONBASE (FixedPcdGet64 (PcdHb1Rb1CpuIoRegionBase))
#define PCI_HB1RB2_CPUIOREGIONBASE (FixedPcdGet64 (PcdHb1Rb2CpuIoRegionBase))
#define PCI_HB1RB3_CPUIOREGIONBASE (FixedPcdGet64 (PcdHb1Rb3CpuIoRegionBase))
#define PCI_HB1RB4_CPUIOREGIONBASE (FixedPcdGet64 (PcdHb1Rb4CpuIoRegionBase))
#define PCI_HB1RB5_CPUIOREGIONBASE (FixedPcdGet64 (PcdHb1Rb5CpuIoRegionBase))
#define PCI_HB1RB6_CPUIOREGIONBASE (FixedPcdGet64 (PcdHb1Rb6CpuIoRegionBase))
#define PCI_HB1RB7_CPUIOREGIONBASE (FixedPcdGet64 (PcdHb1Rb7CpuIoRegionBase))



#define PCI_HB0RB0_IO_BASE (FixedPcdGet64 (PcdHb0Rb0IoBase))
#define PCI_HB0RB1_IO_BASE (FixedPcdGet64 (PcdHb0Rb1IoBase))
#define PCI_HB0RB2_IO_BASE (FixedPcdGet64 (PcdHb0Rb2IoBase))
#define PCI_HB0RB3_IO_BASE (FixedPcdGet64 (PcdHb0Rb3IoBase))
#define PCI_HB0RB4_IO_BASE (FixedPcdGet64 (PcdHb0Rb4IoBase))
#define PCI_HB0RB5_IO_BASE (FixedPcdGet64 (PcdHb0Rb5IoBase))
#define PCI_HB0RB6_IO_BASE (FixedPcdGet64 (PcdHb0Rb6IoBase))
#define PCI_HB0RB7_IO_BASE (FixedPcdGet64 (PcdHb0Rb7IoBase))

#define PCI_HB1RB0_IO_BASE (FixedPcdGet64 (PcdHb1Rb0IoBase))
#define PCI_HB1RB1_IO_BASE (FixedPcdGet64 (PcdHb1Rb1IoBase))
#define PCI_HB1RB2_IO_BASE (FixedPcdGet64 (PcdHb1Rb2IoBase))
#define PCI_HB1RB3_IO_BASE (FixedPcdGet64 (PcdHb1Rb3IoBase))
#define PCI_HB1RB4_IO_BASE (FixedPcdGet64 (PcdHb1Rb4IoBase))
#define PCI_HB1RB5_IO_BASE (FixedPcdGet64 (PcdHb1Rb5IoBase))
#define PCI_HB1RB6_IO_BASE (FixedPcdGet64 (PcdHb1Rb6IoBase))
#define PCI_HB1RB7_IO_BASE (FixedPcdGet64 (PcdHb1Rb7IoBase))

#define PCI_HB0RB0_IO_SIZE (FixedPcdGet64 (PcdHb0Rb0IoSize))
#define PCI_HB0RB1_IO_SIZE (FixedPcdGet64 (PcdHb0Rb1IoSize))
#define PCI_HB0RB2_IO_SIZE (FixedPcdGet64 (PcdHb0Rb2IoSize))
#define PCI_HB0RB3_IO_SIZE (FixedPcdGet64 (PcdHb0Rb3IoSize))
#define PCI_HB0RB4_IO_SIZE (FixedPcdGet64 (PcdHb0Rb4IoSize))
#define PCI_HB0RB5_IO_SIZE (FixedPcdGet64 (PcdHb0Rb5IoSize))
#define PCI_HB0RB6_IO_SIZE (FixedPcdGet64 (PcdHb0Rb6IoSize))
#define PCI_HB0RB7_IO_SIZE (FixedPcdGet64 (PcdHb0Rb7IoSize))

#define PCI_HB1RB0_IO_SIZE (FixedPcdGet64 (PcdHb1Rb0IoSize))
#define PCI_HB1RB1_IO_SIZE (FixedPcdGet64 (PcdHb1Rb1IoSize))
#define PCI_HB1RB2_IO_SIZE (FixedPcdGet64 (PcdHb1Rb2IoSize))
#define PCI_HB1RB3_IO_SIZE (FixedPcdGet64 (PcdHb1Rb3IoSize))
#define PCI_HB1RB4_IO_SIZE (FixedPcdGet64 (PcdHb1Rb4IoSize))
#define PCI_HB1RB5_IO_SIZE (FixedPcdGet64 (PcdHb1Rb5IoSize))
#define PCI_HB1RB6_IO_SIZE (FixedPcdGet64 (PcdHb1Rb6IoSize))
#define PCI_HB1RB7_IO_SIZE (FixedPcdGet64 (PcdHb1Rb7IoSize))



typedef struct {
  UINT32          Segment;
  UINT64          Ecam;
  UINT64          BusBase;
  UINT64          BusLimit;
  UINT64          IoBase;
  UINT64          IoLimit;
  UINT64          CpuMemRegionBase;
  UINT64          CpuIoRegionBase;
  UINT64          RbPciBar;
  UINT64          PciRegionBase;
  UINT64          PciRegionLimit;
} PCI_ROOT_BRIDGE_RESOURCE_APPETURE;

extern PCI_ROOT_BRIDGE_RESOURCE_APPETURE  mResAppeture[PCIE_MAX_HOSTBRIDGE][PCIE_MAX_ROOTBRIDGE];
#endif

