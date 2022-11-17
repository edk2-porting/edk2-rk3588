/** @file

  @copyright
  Copyright 2015 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _ONBOARD_NIC_STRUCTS_H
#define _ONBOARD_NIC_STRUCTS_H

#include <Protocol/UbaDevsUpdateProtocol.h>

#pragma pack(1)
typedef struct {

  NIC_TYPE    NicType;               //Onboard or IO modue?
  UINT8       NicIndex;              //Nic index in setup page, 1 for Nic 1, 2 for Nic 2.....
  //
  //Parent
  //
  UINT8      RootPortBusNo;         //Who is connected to?
  UINT8      RootPortDevNo;
  UINT8      RootPortFunNo;

  //
  //Nic controller for Onboard Nic
  //
  UINT16     NicVID;                //Vendor ID
  UINT16     NicDID;                //Device ID
  UINT8      PortNumbers;           //How many ports inside NIC?
  UINT8      DescriptionIndex;      //Index to Nic description list

  //
  //Disable Method for Onboard Nic
  //
  UINT8      IsGpioCtrl;            //Disable by Gpio?
  UINT32     GpioForDev;            //Ctrl Gpio Pin number for device
  UINT8      GpioForDevValue;       //what value is used to enable?

  UINT32     PortEnableGPIO[4];     //Ctrl Gpio Pin number for this port
  UINT8      PortEnableGPIOValue[4];//what value is used to enable the port
  UINT8      IsPchNIC;

} NIC_SETUP_CONFIGURATION_STUCT;

typedef struct {
  UINT16     NicDID;              // Device ID
  UINT16     SubDID;              // Subsystem ID, if preset to 0xFFFF, then negore this field during detection
  UINT8      NIC10Gb;             // 10Gbe Pxe Seupported
  UINT8      PXE_Support;         // Pxe supported?
  UINT8      iSCSI_Support;       // iScsi supported?
  UINT8      FCoE_Support;        // FCoe supported?
  UINT8      InfB_Support;        // InfiniBand supported?
  UINT8      PchNIC;              //PCH integrated NIC?
} NIC_OPTIONROM_CAPBILITY_STRUCT;

typedef struct {
  UINT16    IOM1DID;              //Device ID for IOM1
  UINT16    IOM1SubDID;          //Subsystem ID for IOM1
  UINT16    IOM2DID;              //Device ID for IOM2
  UINT16    IOM2SubDID;          //Subsystem ID for IOM2
  UINT16    IOM3DID;              //Device ID for IOM3
  UINT16    IOM3SubDID;          //Subsystem ID for IOM3
  UINT16    IOM4DID;              //Device ID for IOM4
  UINT16    IOM4SubDID;          //Subsystem ID for IOM4
} IOMS_NV_VARIABLE;

typedef struct {
  UINT8    PXE10GPreValue;
  UINT8    PXE1GPreValue;
  UINT8    Reserved[6];
} PXE_PREVIOUS_SETTINGS;

#pragma pack()

#define NIC_CHARACTER_NUMBER_FOR_VALUE   30

#define CPU0_IIO_BUS                     0x00
#define IIO_PCIE_PORT_1A_DEV           0x1
#define IIO_PCIE_PORT_1A_FUN           0x0
#define IIO_PCIE_PORT_1B_DEV           0x1
#define IIO_PCIE_PORT_1B_FUN           0x1
#define IIO_PCIE_PORT_2A_DEV           0x2
#define IIO_PCIE_PORT_2A_FUN           0x0
#define IIO_PCIE_PORT_3A_DEV           0x3
#define IIO_PCIE_PORT_3A_FUN           0x0
#define IIO_PCIE_PORT_3C_DEV           0x3
#define IIO_PCIE_PORT_3C_FUN           0x2

#define OB_NIC_POWERVILLE_DID            0x1521

#define INTEL_MAC_ADDRESS_REG            0x5400

#define VENDOR_ID_MELLANOX               0x15B3
#define DEVICE_ID_MELLANOX               0x1003

#endif
