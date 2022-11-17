/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _USB_DEBUG_PORT_INTERNAL_H
#define _USB_DEBUG_PORT_INTERNAL_H

#include "DataTypes.h"

#define PCI_VENDOR_ID_OFFSET                        0x00
#define PCI_DEVICE_ID_OFFSET                        0x02
#define PCI_COMMAND_OFFSET                          0x04
#define PCI_PRIMARY_STATUS_OFFSET                   0x06
#define PCI_REVISION_ID_OFFSET                      0x08
#define PCI_CLASSCODE_OFFSET                        0x09
#define PCI_SUBCLASSCODE_OFFSET                     0x0A
#define PCI_BASECLASSCODE_OFFSET                    0x0B // Base Class Code Register
#define PCI_CACHELINE_SIZE_OFFSET                   0x0C
#define PCI_LATENCY_TIMER_OFFSET                    0x0D
#define PCI_HEADER_TYPE_OFFSET                      0x0E
#define PCI_BIST_OFFSET                             0x0F
#define PCI_BASE_ADDRESSREG_OFFSET                  0x10
#define PCI_CARDBUS_CIS_OFFSET                      0x28
#define PCI_SVID_OFFSET                             0x2C // SubSystem Vendor id
#define PCI_SUBSYSTEM_VENDOR_ID_OFFSET              0x2C
#define PCI_SID_OFFSET                              0x2E // SubSystem ID
#define PCI_SUBSYSTEM_ID_OFFSET                     0x2E
#define PCI_EXPANSION_ROM_BASE                      0x30
#define PCI_CAPBILITY_POINTER_OFFSET                0x34
#define PCI_INT_LINE_OFFSET                         0x3C // Interrupt Line Register
#define PCI_INT_PIN_OFFSET                          0x3D // Interrupt Pin Register
#define PCI_MAXGNT_OFFSET                           0x3E // Max Grant Register
#define PCI_MAXLAT_OFFSET                           0x3F // Max Latency Register

#define PCI_CLASS_SERIAL              0x0C
#define PCI_CLASS_SERIAL_USB          0x03

#define IS_BIT_SET(Register, BitMask)  (((*(volatile UINT32 *)(Register)) & (BitMask)) != 0)
#define IS_BIT_CLEAR(Register, BitMask)  (((*(volatile UINT32 *)(Register)) & (BitMask)) == 0)

#define SET_R32_BIT(Register, BitMask)                \
  {                                                   \
    UINT32    RegisterValue = *(volatile UINT32 *)(Register);  \
    RegisterValue |= (UINT32)(BitMask);               \
    *(volatile UINT32 *)(Register) = RegisterValue;            \
  }

#define CLR_R32_BIT(Register, BitMask)                \
  {                                                   \
    UINT32    RegisterValue = *(volatile UINT32 *)(Register);  \
    RegisterValue &= (UINT32)(~(BitMask));            \
    *(volatile UINT32 *)(Register) = RegisterValue;            \
  }

#define CLR_AND_SET_R32_BIT(Register, BitMask, Value) \
  {                                                   \
    UINT32    RegisterValue = *(volatile UINT32 *)(Register);  \
    RegisterValue &= (UINT32)(~(BitMask));            \
    RegisterValue |= (UINT32)(Value);                 \
    *(volatile UINT32 *)(Register) = RegisterValue;            \
  }

#define SET_R16_BIT(Register, BitMask)                \
  {                                                   \
    UINT16    RegisterValue = *(volatile UINT16 *)(Register);  \
    RegisterValue |= (UINT16)(BitMask);               \
    *(volatile UINT16 *)(Register) = RegisterValue;            \
  }

#define CLR_R16_BIT(Register, BitMask)                \
  {                                                   \
    UINT16    RegisterValue = *(volatile UINT16 *)(Register);  \
    RegisterValue &= (UINT16)(~(BitMask));            \
    *(volatile UINT16 *)(Register) = RegisterValue;            \
  }

#define SET_R8_BIT(Register, BitMask)                 \
  {                                                   \
    UINT8    RegisterValue = *(volatile UINT8 *)(Register);    \
    RegisterValue |= (UINT8)(BitMask);                \
    *(volatile UINT8 *)(Register) = RegisterValue;             \
  }

#define CLR_R8_BIT(Register, BitMask)                 \
  {                                                   \
    UINT8    RegisterValue = *(volatile UINT8 *)(Register);    \
    RegisterValue &= (UINT8)(~(BitMask));             \
    *(volatile UINT8 *)(Register) = RegisterValue;             \
  }

#define PCI_CLASS_SERIAL_USB_EHCI                 0x20
#define PCI_CAPABILITY_ID_DEBUG_PORT              0x0A

#define PCI_USB2_SBRN_OFFSET                      0x60
#define PCI_PRIMARY_BUS_NUMBER_OFFSET             0x18
#define PCI_SECONDARY_BUS_NUMBER_OFFSET           0x19
#define PCI_SUBORDINATE_BUS_NUMBER_OFFSET         0x1A

#define PCI_BRIDGE_MBASE_OFFSET                   0x20
#define PCI_BRIDGE_MLIMIT_OFFSET                  0x22

#define PCI_EHCI_DEFAULT_BUS_NUMBER               0x00
#define PCI_EHCI_DEFAULT_DEVICE_NUMBER            0x1D
#define PCI_EHCI_DEFAULT_FUNCTION_NUMBER          0x00
#define PCI_EHCI_DEFAULT_DEBUG_CAPID_OFFSET       0x58
#define PCI_EHCI_DEFAULT_DEBUG_BASE_OFFSET        0x5A

//
// USB PIDs
//
#define USB2_PID_TOKEN_OUT                        0xE1
#define USB2_PID_TOKEN_IN                         0x69
#define USB2_PID_TOKEN_SOF                        0xA5
#define USB2_PID_TOKEN_SETUP                      0x2D

#define USB2_PID_DATA0                            0xC3
#define USB2_PID_DATA1                            0x4B
#define USB2_PID_DATA2                            0x87
#define USB2_PID_MDATA                            0x0F

#define USB2_PID_HANDSHAKE_ACK                    0xD2
#define USB2_PID_HANDSHAKE_NAK                    0x5A
#define USB2_PID_HANDSHAKE_STALL                  0x1E
#define USB2_PID_HANDSHAKE_NYET                   0x96

#define USB2_PID_SPECIAL_PRE                      0x3C
#define USB2_PID_SPECIAL_ERR                      0x3C
#define USB2_PID_SPECIAL_SPLIT                    0x78
#define USB2_PID_SPECIAL_PING                     0xB4
#define USB2_PID_SPECIAL_RESERVED                 0xF0

//
// USB2 Debug Port Register
//
#define USB2_DEBUG_PORT_STATUS_OWNER              0x40000000
#define USB2_DEBUG_PORT_STATUS_ENABLED            0x10000000
#define USB2_DEBUG_PORT_STATUS_DONE               0x00010000
#define USB2_DEBUG_PORT_STATUS_INUSE              0x00000400
#define USB2_DEBUG_PORT_STATUS_EXCEPTION          0x00000380
#define USB2_DEBUG_PORT_STATUS_ERROR              0x00000040
#define USB2_DEBUG_PORT_STATUS_GO                 0x00000020
#define USB2_DEBUG_PORT_STATUS_WRITE              0x00000010
#define USB2_DEBUG_PORT_STATUS_LENGTH             0x0000000F

#define USB2_DEBUG_PORT_DEFAULT_ADDRESS           127

#define USB2_DEBUG_PORT_DEVICE_BUFFER_MAX         8

typedef struct _USB2_DEBUG_PORT_REGISTER {
  UINT32                ControlStatus;
  UINT8                 TokenPid;
  UINT8                 SendPid;
  UINT8                 ReceivedPid;
  UINT8                 Reserved1;
  UINT8                 DataBuffer[USB2_DEBUG_PORT_DEVICE_BUFFER_MAX];
  UINT8                 UsbEndPoint;
  UINT8                 UsbAddress;
  UINT8                 Reserved2;
  UINT8                 Reserved3;
}USB2_DEBUG_PORT_REGISTER;

typedef struct _USB2_EHCI_CAPABILITY_REGISTER {
  UINT8                 CapLength;
  UINT8                 Reserved;
  UINT16                HciVersion;
  UINT32                HcsParams;
  UINT32                HccParams;
  UINT32                HcspPortRoute;
}USB2_EHCI_CAPABILITY_REGISTER;

#define USB2_EHCI_USBCMD_RUN                      0x00000001
#define USB2_EHCI_USBCMD_RESET                    0x00000002

#define USB2_EHCI_USBSTS_HC_HALTED                0x00001000

#define USB2_EHCI_PORTSC_PORT_OWNER               0x00002000
#define USB2_EHCI_PORTSC_PORT_POWER               0x00001000
#define USB2_EHCI_PORTSC_PORT_RESET               0x00000100
#define USB2_EHCI_PORTSC_PORT_SUSPEND             0x00000080
#define USB2_EHCI_PORTSC_PORT_ENABLED             0x00000004

typedef struct _USB2_EHCI_OPERATIONAL_REGISTER {
  UINT32                UsbCommand;
  UINT32                UsbStatus;
  UINT32                UsbInterruptEnable;
  UINT32                UsbFrameIndex;
  UINT32                SegmentSelector;
  UINT32                FrameListBaseAddress;
  UINT32                NextAsyncListAddress;
  UINT32                Reserved[9];
  UINT32                ConfigFlag;
  UINT32                PortSc[0x0F];
}USB2_EHCI_OPERATIONAL_REGISTER;

#define USB2_DEBUG_PORT_DRIVER_BUFFER_MAX     USB2_DEBUG_PORT_DEVICE_BUFFER_MAX * 2

typedef struct _USB2_DEBUG_PORT_INSTANCE {

  UINT32                              EhciCapRegister;
  UINT32                              EhciOpRegister;
  UINT32                              PortSc;
  UINT32                              DebugRegister;

  BOOLEAN                             Ready;

  UINT8                               PciBusNumber;
  UINT8                               PciDeviceNumber;
  UINT8                               PciDeviceFunction;

  UINT8                               Reserved1;
  UINT8                               BarIndex;
  UINT16                              BarOffset;

  UINT32                              PortBase;
  UINT8                               PortNumber;
  UINT8                               PortAddress;
  UINT8                               ReadEndpoint;
  UINT8                               WriteEndpoint;

  UINT8                               ReadEndpointDataToggle;
  UINT8                               WriteEndpointDataToggle;
  UINT8                               Reserved2[2];

  INT32                                TempDataLength;
  INT32                                TempDataIndex;
  UINT8                               TempData[USB2_DEBUG_PORT_DRIVER_BUFFER_MAX];
}USB2_DEBUG_PORT_INSTANCE;

//
// Setup Packet
//
// Data phase transfer direction
//
#define USB2_REQUEST_TYPE_HOST_TO_DEVICE          0x00
#define USB2_REQUEST_TYPE_DEVICE_TO_HOST          0x80

//
// Type
//
#define USB2_REQUEST_TYPE_STANDARD                0x00
#define USB2_REQUEST_TYPE_CLASS                   0x20
#define USB2_REQUEST_TYPE_VENDOR                  0x40

//
// Recipient
//
#define USB2_REQUEST_TYPE_DEVICE                  0x00
#define USB2_REQUEST_TYPE_INTERFACE               0x01
#define USB2_REQUEST_TYPE_ENDPOINT                0x02
#define USB2_REQUEST_TYPE_OTHER                   0x03

//
// Request
//
#define USB2_REQUEST_GET_STATUS                   0x00
#define USB2_REQUEST_CLEAR_FEATURE                0x01
#define USB2_REQUEST_SET_FEATURE                  0x03
#define USB2_REQUEST_SET_ADDRESS                  0x05
#define USB2_REQUEST_GET_DESCRIPTOR               0x06
#define USB2_REQUEST_SET_DESCRIPTOR               0x07
#define USB2_REQUEST_GET_CONFIGURATION            0x08
#define USB2_REQUEST_SET_CONFIGURATION            0x09
#define USB2_REQUEST_GET_INTERFACE                0x0A
#define USB2_REQUEST_SET_INTERFACE                0x11

//
// Descriptor Types
//
#define USB2_DESCRIPTOR_TYPE_DEVICE                         0x01
#define USB2_DESCRIPTOR_TYPE_CONFIGURATION                  0x02
#define USB2_DESCRIPTOR_TYPE_STRING                         0x03
#define USB2_DESCRIPTOR_TYPE_INTERFACE                      0x04
#define USB2_DESCRIPTOR_TYPE_ENDPOINT                       0x05
#define USB2_DESCRIPTOR_TYPE_DEVICE_QUALIFIER               0x06
#define USB2_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIGURATION      0x07
#define USB2_DESCRIPTOR_TYPE_INTERFACE_POWER                0x08
#define USB2_DESCRIPTOR_TYPE_OTG                            0x09
#define USB2_DESCRIPTOR_TYPE_DEBUG                          0x0A

//
// Standard Feature Selectors
//
#define USB2_FEATURE_DEVICE_REMOTE_WAKEUP                   0x01
#define USB2_FEATURE_ENDPOINT_HALT                          0x00
#define USB2_FEATURE_TEST_MODE                              0x02
#define USB2_FEATURE_OTG_B_HNP_ENABLE                       0x03
#define USB2_FEATURE_OTG_A_HNP_SUPPORT                      0x04
#define USB2_FEATURE_OTG_A_ALT_HNP_SUPPORT                  0x05
#define USB2_FEATURE_DEBUG_MODE                             0x06

typedef struct _USB2_SETUP_PACKET {
    UINT8                         RequestType;
    UINT8                         Request;
    UINT8                         Value[2];
    UINT16                        Index;
    UINT16                        Length_;
}USB2_SETUP_PACKET;

typedef struct _USB2_DEBUG_DESCRIPTOR_TYPE {
  UINT8                           Length_;
  UINT8                           DescriptorType;
  UINT8                           DebugInEndpoint;
  UINT8                           DebugOutEndpoint;
}USB2_DEBUG_DESCRIPTOR_TYPE;

typedef struct _USB2_ENDPOINT_DESCRIPTOR_TYPE {
  UINT8                           Length_;
  UINT8                           DescriptorType;
  UINT8                           EndpointAddress;
  UINT8                           Attributes;
  UINT16                          MaxPacketSize;
  UINT8                           Interval;
}USB2_ENDPOINT_DESCRIPTOR_TYPE;

#endif /* _USB_DEBUG_PORT_H */
