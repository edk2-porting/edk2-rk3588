/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _IIO_REGS_H_
#define _IIO_REGS_H_

/**
==================================================================================================
==================================  General Defintions          ==================================
==================================================================================================
**/

#define NUMBER_PORTS_PER_SOCKET       21
#define IIO_UPLINK_PORT_INDEX         5    //Port 2A is the uplink port in Neon-City ///TODO Check if this is required for SKX/Purley SKX_TTEST
#define MaxIIO                        MAX_SOCKET

#if MAX_SOCKET > 4
#define TOTAL_CB3_DEVICES             64   // Todo Check SKX CB3 devices (IOAT_TOTAL_FUNCS * MAX_SOCKET). Note: this covers up to 8S.
#define MAX_TOTAL_PORTS               168   //NUMBER_PORTS_PER_SOCKET * MaxIIO. As now, treats setup S0-S3 = S4_S7 as optimal
#else
#define TOTAL_CB3_DEVICES             32   // Todo Check SKX CB3 devices.
#define MAX_TOTAL_PORTS               84   //NUMBER_PORTS_PER_SOCKET * MaxIIO
#endif

#if MAX_SOCKET > 4
#define TOTAL_IIO_STACKS              48   // MAX_SOCKET * MAX_IIO_STACK. Not reflect architecture but only sysHost structure!
#define TOTAL_SYSTEM_IIO_STACKS       32   // In term of system architecture support
#else
#define TOTAL_IIO_STACKS              24   // MAX_SOCKET * MAX_IIO_STACK
#define TOTAL_SYSTEM_IIO_STACKS       24   // In term of system architecture support
#endif

#define NUMBER_NTB_PORTS_PER_SOCKET   3
#if MAX_SOCKET > 4
#define MAX_NTB_PORTS                 24   // NUMBER_NTB_PORTS_PER_SOCKET * MAX_SOCKET
#else
#define MAX_NTB_PORTS                 12   // NUMBER_NTB_PORTS_PER_SOCKET * MAX_SOCKET
#endif
#define VMD_STACK_PER_SOCKET          3
#define VMD_PORT_PER_STACK            4
#define VMD_PORTS_PER_SOCKET          12
#if MAX_SOCKET > 4
#define MAX_VMD_PORTS                 96   // VMD_PORTS_PER_SOCKET * MAX_SOCKET
#define MAX_VMD_STACKS                24   // VMD_STACK_PER_SOCKET * MAX_SOCKET
#else
#define MAX_VMD_PORTS                 48   // VMD_PORTS_PER_SOCKET * MAX_SOCKET
#define MAX_VMD_STACKS                12   // VMD_STACK_PER_SOCKET * MAX_SOCKET
#endif


#define VARIABLE_FUNC3_ELEMENTS       4
#if MAX_SOCKET > 4
#define MAX_TOTAL_CORE_HIDE           32   //(MAX_SOCKET * VARIABLE_FUNC3_ELEMENTS)
#else
#define MAX_TOTAL_CORE_HIDE           16   //(MAX_SOCKET * VARIABLE_FUNC3_ELEMENTS)
#endif

#define MAX_IOU_PORT_DEVICES          4


/**
==================================================================================================
==================================  IIO Root Port Defintions              ====================
==================================================================================================
**/
typedef enum {
  IioPortA = 0,
  IioPortB = 1,
  IioPortC = 2,
  IioPortD = 3
}IIOPORTS;

typedef enum {
 IioIou0 = 0,
 IioIou1,
 IioIou2,
 IioMcp0,
 IioMcp1,
 IioIouMax
} IIOIOUS;

//
// Bifurcation control register shorthand
//
#define IIO_BIFURCATE_AUTO      0xFF

// Ports 1D-1A, 2D-2A, 3D-3A
//
#define IIO_BIFURCATE_x4x4x4x4  0
#define IIO_BIFURCATE_x4x4xxx8  1
#define IIO_BIFURCATE_xxx8x4x4  2
#define IIO_BIFURCATE_xxx8xxx8  3
#define IIO_BIFURCATE_xxxxxx16  4
#define IIO_BIFURCATE_xxxxxxxx  0xF

#define PORT_0_INDEX             0
#define PCIE_PORT_2_DEV          0x02

// IOU2
#define PORT_1A_INDEX            1
#define PORT_1B_INDEX            2
#define PORT_1C_INDEX            3
#define PORT_1D_INDEX            4
// IOU0
#define PORT_2A_INDEX            5
#define PORT_2B_INDEX            6
#define PORT_2C_INDEX            7
#define PORT_2D_INDEX            8
// IOU1
#define PORT_3A_INDEX            9
#define PORT_3B_INDEX            10
#define PORT_3C_INDEX            11
#define PORT_3D_INDEX            12
//MCP0
#define PORT_4A_INDEX            13
#define PORT_4B_INDEX            14
#define PORT_4C_INDEX            15
#define PORT_4D_INDEX            16
//MCP1
#define PORT_5A_INDEX            17
#define PORT_5B_INDEX            18
#define PORT_5C_INDEX            19
#define PORT_5D_INDEX            20

//
#define SOCKET_0_INDEX           0
#define SOCKET_1_INDEX           21
#define SOCKET_2_INDEX           42
#define SOCKET_3_INDEX           63
#define SOCKET_4_INDEX           84
#define SOCKET_5_INDEX           105
#define SOCKET_6_INDEX           126
#define SOCKET_7_INDEX           147

#define PCIE_PORT_0_DEV     0x00
#define PCIE_PORT_0_FUNC    0x00

#define PCIE_PORT_1A_DEV    0x00
#define PCIE_PORT_1B_DEV    0x01
#define PCIE_PORT_1C_DEV    0x02
#define PCIE_PORT_1D_DEV    0x03
#define PCIE_PORT_1A_FUNC   0x00
#define PCIE_PORT_1B_FUNC   0x00
#define PCIE_PORT_1C_FUNC   0x00
#define PCIE_PORT_1D_FUNC   0x00

#define PCIE_PORT_2A_DEV    0x00
#define PCIE_PORT_2B_DEV    0x01
#define PCIE_PORT_2C_DEV    0x02
#define PCIE_PORT_2D_DEV    0x03
#define PCIE_PORT_2A_FUNC   0x00
#define PCIE_PORT_2B_FUNC   0x00
#define PCIE_PORT_2C_FUNC   0x00
#define PCIE_PORT_2D_FUNC   0x00

#define PCIE_PORT_3A_DEV    0x00
#define PCIE_PORT_3B_DEV    0x01
#define PCIE_PORT_3C_DEV    0x02
#define PCIE_PORT_3D_DEV    0x03
#define PCIE_PORT_3A_FUNC   0x00
#define PCIE_PORT_3B_FUNC   0x00
#define PCIE_PORT_3C_FUNC   0x00
#define PCIE_PORT_3D_FUNC   0x00

#define PCIE_PORT_4A_DEV    0x00
#define PCIE_PORT_4B_DEV    0x01
#define PCIE_PORT_4C_DEV    0x02
#define PCIE_PORT_4D_DEV    0x03
#define PCIE_PORT_4A_FUNC   0x00
#define PCIE_PORT_4B_FUNC   0x00
#define PCIE_PORT_4C_FUNC   0x00
#define PCIE_PORT_4D_FUNC   0x00

#define PCIE_PORT_5A_DEV    0x00
#define PCIE_PORT_5B_DEV    0x01
#define PCIE_PORT_5C_DEV    0x02
#define PCIE_PORT_5D_DEV    0x03
#define PCIE_PORT_5A_FUNC   0x00
#define PCIE_PORT_5B_FUNC   0x00
#define PCIE_PORT_5C_FUNC   0x00
#define PCIE_PORT_5D_FUNC   0x00

#define PCIE_PORT_GLOBAL_RTO_DEV     0x07
#define PCIE_PORT_GLOBAL_RTO_FUNC    0x07

#define PCIE_PORT_0_RTO_DEV     0x07
#define PCIE_PORT_0_RTO_FUNC    0x00

#define PCIE_PORT_1A_RTO_DEV    0x07
#define PCIE_PORT_1A_RTO_FUNC   0x00
#define PCIE_PORT_1B_RTO_DEV    0x07
#define PCIE_PORT_1B_RTO_FUNC   0x01
#define PCIE_PORT_1C_RTO_DEV    0x07
#define PCIE_PORT_1C_RTO_FUNC   0x02
#define PCIE_PORT_1D_RTO_DEV    0x07
#define PCIE_PORT_1D_RTO_FUNC   0x03


#define PCIE_PORT_2A_RTO_DEV    0x07
#define PCIE_PORT_2A_RTO_FUNC   0x00
#define PCIE_PORT_2B_RTO_DEV    0x07
#define PCIE_PORT_2B_RTO_FUNC   0x01
#define PCIE_PORT_2C_RTO_DEV    0x07
#define PCIE_PORT_2C_RTO_FUNC   0x02
#define PCIE_PORT_2D_RTO_DEV    0x07
#define PCIE_PORT_2D_RTO_FUNC   0x03

#define PCIE_PORT_3A_RTO_DEV    0x07
#define PCIE_PORT_3A_RTO_FUNC   0x00
#define PCIE_PORT_3B_RTO_DEV    0x07
#define PCIE_PORT_3B_RTO_FUNC   0x01
#define PCIE_PORT_3C_RTO_DEV    0x07
#define PCIE_PORT_3C_RTO_FUNC   0x02
#define PCIE_PORT_3D_RTO_DEV    0x07
#define PCIE_PORT_3D_RTO_FUNC   0x03

#define PCIE_PORT_4A_RTO_DEV    0x07
#define PCIE_PORT_4A_RTO_FUNC   0x00
#define PCIE_PORT_4B_RTO_DEV    0x07
#define PCIE_PORT_4B_RTO_FUNC   0x01
#define PCIE_PORT_4C_RTO_DEV    0x07
#define PCIE_PORT_4C_RTO_FUNC   0x02
#define PCIE_PORT_4D_RTO_DEV    0x07
#define PCIE_PORT_4D_RTO_FUNC   0x03

#define PCIE_PORT_5A_RTO_DEV    0x07
#define PCIE_PORT_5A_RTO_FUNC   0x00
#define PCIE_PORT_5B_RTO_DEV    0x07
#define PCIE_PORT_5B_RTO_FUNC   0x01
#define PCIE_PORT_5C_RTO_DEV    0x07
#define PCIE_PORT_5C_RTO_FUNC   0x02
#define PCIE_PORT_5D_RTO_DEV    0x07
#define PCIE_PORT_5D_RTO_FUNC   0x03

#define PCIE_PORT_0_LINK_SEL  0x00
#define PCIE_PORT_1A_LINK_SEL  0x00
#define PCIE_PORT_1B_LINK_SEL  0x01
#define PCIE_PORT_1C_LINK_SEL  0x02
#define PCIE_PORT_1D_LINK_SEL  0x03
#define PCIE_PORT_2A_LINK_SEL  0x00
#define PCIE_PORT_2B_LINK_SEL  0x01
#define PCIE_PORT_2C_LINK_SEL  0x02
#define PCIE_PORT_2D_LINK_SEL  0x03
#define PCIE_PORT_3A_LINK_SEL  0x00
#define PCIE_PORT_3B_LINK_SEL  0x01
#define PCIE_PORT_3C_LINK_SEL  0x02
#define PCIE_PORT_3D_LINK_SEL  0x03
#define PCIE_PORT_4A_LINK_SEL  0x00
#define PCIE_PORT_4B_LINK_SEL  0x01
#define PCIE_PORT_4C_LINK_SEL  0x02
#define PCIE_PORT_4D_LINK_SEL  0x03
#define PCIE_PORT_5A_LINK_SEL  0x00
#define PCIE_PORT_5B_LINK_SEL  0x01
#define PCIE_PORT_5C_LINK_SEL  0x02
#define PCIE_PORT_5D_LINK_SEL  0x03

#define PCIE_PORT_0_SUPER_CLUSTER_PORT  0x00
#define PCIE_PORT_1A_SUPER_CLUSTER_PORT  0x01
#define PCIE_PORT_1B_SUPER_CLUSTER_PORT  0x01
#define PCIE_PORT_1C_SUPER_CLUSTER_PORT  0x01
#define PCIE_PORT_1D_SUPER_CLUSTER_PORT  0x01
#define PCIE_PORT_2A_SUPER_CLUSTER_PORT  0x05
#define PCIE_PORT_2B_SUPER_CLUSTER_PORT  0x05
#define PCIE_PORT_2C_SUPER_CLUSTER_PORT  0x05
#define PCIE_PORT_2D_SUPER_CLUSTER_PORT  0x05
#define PCIE_PORT_3A_SUPER_CLUSTER_PORT  0x09
#define PCIE_PORT_3B_SUPER_CLUSTER_PORT  0x09
#define PCIE_PORT_3C_SUPER_CLUSTER_PORT  0x09
#define PCIE_PORT_3D_SUPER_CLUSTER_PORT  0x09
#define PCIE_PORT_4A_SUPER_CLUSTER_PORT  0x0D
#define PCIE_PORT_4B_SUPER_CLUSTER_PORT  0x0D
#define PCIE_PORT_4C_SUPER_CLUSTER_PORT  0x0D
#define PCIE_PORT_4D_SUPER_CLUSTER_PORT  0x0D
#define PCIE_PORT_5A_SUPER_CLUSTER_PORT  0x11
#define PCIE_PORT_5B_SUPER_CLUSTER_PORT  0x11
#define PCIE_PORT_5C_SUPER_CLUSTER_PORT  0x11
#define PCIE_PORT_5D_SUPER_CLUSTER_PORT  0x11

#define PORT_LINK_WIDTH_x16     16
#define PORT_LINK_WIDTH_x8      8
#define PORT_LINK_WIDTH_x4      4
#define PORT_LINK_WIDTH_x2      2
#define PORT_LINK_WIDTH_x1      1

//
// Port Config Mode
//
#define REGULAR_PCIE_OWNERSHIP        0
#define PCIE_PORT_REGULAR_MODE        1
#define PCIE_PORT_NTB_MODE            2
#define VMD_OWNERSHIP                 3
#define PCIEAIC_OCL_OWNERSHIP         4


/**
==================================================================================================
================================== Devide Hide Definitions =======================================
==================================================================================================
**/

#define NUM_DEVHIDE_REGS                              8
// Hide all 8 Devices for every Stack
#define MAX_DEVHIDE_REGS                             (MAX_IIO_STACK * NUM_DEVHIDE_REGS)
#if MaxIIO > 4
#define MAX_DEVHIDE_REGS_PER_SYSTEM                  384 //(MAX_DEVHIDE_REGS * MaxIIO)
#else
#define MAX_DEVHIDE_REGS_PER_SYSTEM                  192 //(MAX_DEVHIDE_REGS * MaxIIO)
#endif

#endif //_IIO_REGS_H_
