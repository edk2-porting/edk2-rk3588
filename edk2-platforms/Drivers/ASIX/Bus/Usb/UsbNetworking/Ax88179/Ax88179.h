/** @file
  Definitions for ASIX AX88179 Ethernet adapter.

  Copyright (c) 2011, Intel Corporation
  Copyright (c) 2020, ARM Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef AX88179_H_
#define AX88179_H_

#include <Uefi.h>

#include <Guid/EventGroup.h>

#include <IndustryStandard/Pci.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/NetLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeLib.h>

#include <Protocol/DevicePath.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/NetworkInterfaceIdentifier.h>
#include <Protocol/SimpleNetwork.h>
#include <Protocol/UsbIo.h>


#define FORCE_100Mbps 0
#define REPORTLINK    1


//------------------------------------------------------------------------------
//  Macros
//------------------------------------------------------------------------------

#define USB_IS_IN_ENDPOINT(EndPointAddr)      (((EndPointAddr) & BIT7) != 0)  ///<  Return TRUE/FALSE for IN direction
#define USB_IS_OUT_ENDPOINT(EndPointAddr)     (((EndPointAddr) & BIT7) == 0)  ///<  Return TRUE/FALSE for OUT direction
#define USB_IS_BULK_ENDPOINT(Attribute)       (((Attribute) & (BIT0 | BIT1)) == USB_ENDPOINT_BULK)      ///<  Return TRUE/FALSE for BULK type
#define USB_IS_INTERRUPT_ENDPOINT(Attribute)  (((Attribute) & (BIT0 | BIT1)) == USB_ENDPOINT_INTERRUPT) ///<  Return TRUE/FALSE for INTERRUPT type

//------------------------------------------------------------------------------
//  Constants
//------------------------------------------------------------------------------

#define DEBUG_RX_BROADCAST  0x40000000  ///<  Display RX broadcast messages
#define DEBUG_RX_MULTICAST  0x20000000  ///<  Display RX multicast messages
#define DEBUG_RX_UNICAST    0x10000000  ///<  Display RX unicast messages
#define DEBUG_MAC_ADDRESS   0x08000000  ///<  Display the MAC address
#define DEBUG_LINK          0x04000000  ///<  Display the link status
#define DEBUG_TX            0x02000000  ///<  Display the TX messages
#define DEBUG_PHY           0x01000000  ///<  Display the PHY register values
#define DEBUG_SROM          0x00800000  ///<  Display the SROM contents
#define DEBUG_TIMER         0x00400000  ///<  Display the timer routine entry/exit
#define DEBUG_TPL           0x00200000  ///<  Display the timer routine entry/exit


#define ETHERNET_HEADER_SIZE  sizeof (ETHERNET_HEADER)  ///<  Size in bytes of the Ethernet header
#define MIN_ETHERNET_PKT_SIZE 60    ///<  Minimum packet size including Ethernet header
#define MAX_ETHERNET_PKT_SIZE 1500

#define USB_NETWORK_CLASS   0x09    ///<  USB Network class code
#define USB_BUS_TIMEOUT     1000    ///<  USB timeout in milliseconds

#define AX88179_BULKIN_SIZE_INK     2
#define AX88179_MAX_BULKIN_SIZE    (1024 * AX88179_BULKIN_SIZE_INK)
#define AX88179_MAX_PKT_SIZE  2048

#define HC_DEBUG        0
#define ADD_MACPATHNOD  1
#define BULKIN_TIMEOUT  3 //5000
#define TX_RETRY        0
#define AUTONEG_DELAY   1000000

/**
  Verify new TPL value

  This macro which is enabled when debug is enabled verifies that
  the new TPL value is >= the current TPL value.
**/
#ifdef VERIFY_TPL
#undef VERIFY_TPL
#endif  //  VERIFY_TPL

#if !defined(MDEPKG_NDEBUG)

#define VERIFY_TPL(tpl)                         \
{                                               \
  EFI_TPL PreviousTpl;                          \
                                                \
  PreviousTpl = gBS->RaiseTPL (TPL_HIGH_LEVEL); \
  gBS->RestoreTPL (PreviousTpl);                \
  if (PreviousTpl > tpl) {                      \
    DEBUG ((DEBUG_ERROR, "Current TPL: %d, New TPL: %d\r\n", PreviousTpl, tpl));  \
    ASSERT (PreviousTpl <= tpl);                \
  }                                             \
}

#else   //  MDEPKG_NDEBUG

#define VERIFY_TPL(tpl)

#endif  //  MDEPKG_NDEBUG

//------------------------------------------------------------------------------
//  Hardware Definition
//------------------------------------------------------------------------------

#define DEV_SIGNATURE     SIGNATURE_32 ('A','X','8','8')  ///<  Signature of data structures in memory

#define VENDOR_ID         0x0B95  ///<  Vendor ID for Asix
#define PRODUCT_ID        0x1790  ///<  Product ID for the AX88179 USB 10/100 Ethernet controller
#define PRODUCT_ID_178A   0x178A

#define RESET_MSEC        1000    ///<  Reset duration
#define PHY_RESET_MSEC    100     ///<  PHY reset duration

//
//  RX Control register
//


//
//  Medium Status register
//



//
//  Software PHY Select register
//

#define SPHY_PSEL         0x01    ///<  Select internal PHY
#define SPHY_ASEL         0x02    ///<  1=Auto select, 0=Manual select

//
//  Software Reset register
//

#define SRR_RR            0x01    ///<  Clear receive frame length error
#define SRR_RT            0x02    ///<  Clear transmit frame length error
#define SRR_PRTE          0x04    ///<  External PHY reset pin tri-state enable
#define SRR_PRL           0x08    ///<  External PHY reset pin level
#define SRR_BZ            0x10    ///<  Force Bulk to return zero length packet
#define SRR_IPRL          0x20    ///<  Internal PHY reset control
#define SRR_IPPD          0x40    ///<  Internal PHY power down

//
//  PHY ID values
//

#define PHY_ID_INTERNAL   0x0003  ///<  Internal PHY

//
//  USB Commands
//

#define CMD_PHY_ACCESS_SOFTWARE   0x06  ///<  Software in control of PHY
#define CMD_PHY_REG_READ          0x07  ///<  Read PHY register, Value: PHY, Index: Register, Data: Register value
#define CMD_PHY_REG_WRITE         0x08  ///<  Write PHY register, Value: PHY, Index: Register, Data: New 16-bit value
#define CMD_PHY_ACCESS_HARDWARE   0x0a  ///<  Hardware in control of PHY
#define CMD_SROM_READ             0x0b  ///<  Read SROM register: Value: Address, Data: Value
#define CMD_RX_CONTROL_WRITE      0x10  ///<  Set the RX control register, Value: New value
#define CMD_GAPS_WRITE            0x12  ///<  Write the gaps register, Value: New value
#define CMD_MAC_ADDRESS_READ      0x11  ///<  Read the MAC address, Data: 6 byte MAC address
#define CMD_MAC_ADDRESS_WRITE     0x14  ///<  Set the MAC address, Data: New 6 byte MAC address

#define CMD_MEDIUM_STATUS_READ    0x1a  ///<  Read medium status register, Data: Register value
#define CMD_MEDIUM_STATUS_WRITE   0x1b  ///<  Write medium status register, Value: New value
#define CMD_RESET                 0x20  ///<  Reset register, Value: New value
#define CMD_PHY_SELECT            0x22  ///<  PHY select register, Value: New value

//------------------------------
//  USB Endpoints
//------------------------------

#define CONTROL_ENDPOINT                0       ///<  Control endpoint
#define INTERRUPT_ENDPOINT              1       ///<  Interrupt endpoint
#define BULK_IN_ENDPOINT                2       ///<  Receive endpoint
#define BULK_OUT_ENDPOINT               3       ///<  Transmit endpoint

//------------------------------
//  PHY Registers
//------------------------------

#define PHY_ANER                        6       ///<  Autonegotiation expansion register
//  BSMR - Register 1
//  ANAR and ANLPAR Registers 4, 5
#define RXHDR_DROP 0x8000
#define RXHDR_CRCERR 0x2000


//------------------------------------------------------------------------------
//  Data Types
//------------------------------------------------------------------------------

/**
  Ethernet header layout

  IEEE 802.3-2002 Part 3 specification, section 3.1.1.
**/
#pragma pack(1)
typedef struct {
  UINT8  DestAddr[PXE_HWADDR_LEN_ETHER];  ///<  Destination LAN address
  UINT8  SrcAddr[PXE_HWADDR_LEN_ETHER];   ///<  Source LAN address
  UINT16 Type;                            ///<  Protocol or length
} ETHERNET_HEADER;
#pragma pack()

/**
  Receive and Transmit packet structure
**/
#pragma pack(1)
typedef struct _TX_PACKET {
  UINT32  TxHdr1;
  UINT32  TxHdr2;
  UINT8   Data[AX88179_MAX_PKT_SIZE]; ///<  Received packet data
} TX_PACKET;
#pragma pack()

#pragma pack(1)
typedef struct _RX_PACKET {
  struct _RX_PACKET *Next;
  UINT16            Length;
  UINT16            EEEE;
  UINT8             Data[AX88179_MAX_PKT_SIZE];
} RX_PACKET;
#pragma pack()

/**
  AX88179 control structure

  The driver uses this structure to manage the Asix AX88179 10/100
  Ethernet controller.
**/
typedef struct {
  UINTN                     Signature;         ///<  Structure identification

  //
  //  USB data
  //
  EFI_HANDLE                Controller;        ///<  Controller handle
  EFI_USB_IO_PROTOCOL       *UsbIo;            ///<  USB driver interface

  //
  //  Simple network protocol data
  //
  EFI_SIMPLE_NETWORK_PROTOCOL SimpleNetwork;     ///<  Driver's network stack interface
  EFI_SIMPLE_NETWORK_MODE     SimpleNetworkData; ///<  Data for simple network

  //
  // Ethernet controller data
  //
  BOOLEAN                   Initialized;       ///<  Controller initialized
  UINT16                    PhyId;             ///<  PHY ID

  //
  //  Link state
  //
  BOOLEAN                   LinkSpeed100Mbps;   ///<  Current link speed, FALSE = 10 Mbps
  BOOLEAN                   LinkSpeed1000Mbps;  ///<  Current link speed, FALSE = 100/10 bps
  BOOLEAN                   Complete;           ///<  Current state of auto-negotiation
  BOOLEAN                   FullDuplex;         ///<  Current duplex
  BOOLEAN                   LinkUp;             ///<  Current link state
  BOOLEAN                   LinkIdle;           ///<  TRUE = No received traffic
  EFI_EVENT                 Timer;              ///<  Timer to monitor link state and receive packets
  UINTN                     PollCount;          ///<  Number of times the autonegotiation status was polled
  UINTN                     SkipRXCnt;

  UINT8                     *BulkInbuf;
  UINT16                    PktCnt;
  UINT8                     *CurPktHdrOff;
  UINT8                     *CurPktOff;

  TX_PACKET                 *TxTest;

  INT8                      MulticastHash[8];
  EFI_MAC_ADDRESS           MAC;

  UINT16                    CurMediumStatus;
  UINT16                    CurRxControl;
  VOID *                    TxBuffer;

  EFI_DEVICE_PATH_PROTOCOL  *MyDevPath;
  BOOLEAN                   Grub_f;
  BOOLEAN                   FirstRst;
  BOOLEAN                   SetZeroLen;
  UINT8                     RxBurst;
  UINTN                     UsbMaxPktSize;
} NIC_DEVICE;

#define DEV_FROM_SIMPLE_NETWORK(a)  CR (a, NIC_DEVICE, SimpleNetwork, DEV_SIGNATURE)  ///< Locate NIC_DEVICE from Simple Network Protocol

//------------------------------------------------------------------------------
// Simple Network Protocol
//------------------------------------------------------------------------------

/**
  Reset the network adapter.

  Resets a network adapter and reinitializes it with the parameters that
  were provided in the previous call to Initialize ().  The transmit and
  receive queues are cleared.  Receive filters, the station address, the
  statistics, and the multicast-IP-to-HW MAC addresses are not reset by
  this call.

  This routine calls ::Ax88179Reset to perform the adapter specific
  reset operation.  This routine also starts the link negotiation
  by calling ::Ax88179NegotiateLinkStart.

  @param [in] SimpleNetwork    Protocol instance pointer
  @param [in] ExtendedVerification  Indicates that the driver may perform a more
                                exhaustive verification operation of the device
                                during reset.

  @retval EFI_SUCCESS           This operation was successful.
  @retval EFI_NOT_STARTED       The network interface was not started.
  @retval EFI_INVALID_PARAMETER SimpleNetwork parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       The increased buffer size feature is not supported.

**/
EFI_STATUS
EFIAPI
SN_Reset (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork,
  IN BOOLEAN                     ExtendedVerification
  );

/**
  Initialize the simple network protocol.

  This routine calls ::Ax88179MacAddressGet to obtain the
  MAC address.

  @param [in] NicDevice       NIC_DEVICE_INSTANCE pointer

  @retval EFI_SUCCESS     Setup was successful

**/
EFI_STATUS
SN_Setup (
  IN NIC_DEVICE *NicDevice
  );

/**
  This routine starts the network interface.

  @param [in] SimpleNetwork    Protocol instance pointer

  @retval EFI_SUCCESS           This operation was successful.
  @retval EFI_ALREADY_STARTED   The network interface was already started.
  @retval EFI_INVALID_PARAMETER SimpleNetwork parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       The increased buffer size feature is not supported.

**/
EFI_STATUS
EFIAPI
SN_Start (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork
  );

/**
  Set the MAC address.

  This function modifies or resets the current station address of a
  network interface.  If Reset is TRUE, then the current station address
  is set ot the network interface's permanent address.  If Reset if FALSE
  then the current station address is changed to the address specified by
  New.

  This routine calls ::Ax88179MacAddressSet to update the MAC address
  in the network adapter.

  @param [in] SimpleNetwork    Protocol instance pointer
  @param [in] Reset            Flag used to reset the station address to the
                                network interface's permanent address.
  @param [in] New              New station address to be used for the network
                                interface.

  @retval EFI_SUCCESS           This operation was successful.
  @retval EFI_NOT_STARTED       The network interface was not started.
  @retval EFI_INVALID_PARAMETER SimpleNetwork parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       The increased buffer size feature is not supported.

**/
EFI_STATUS
EFIAPI
SN_StationAddress (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork,
  IN BOOLEAN                     Reset,
  IN EFI_MAC_ADDRESS             *New
  );

/**
  This function resets or collects the statistics on a network interface.
  If the size of the statistics table specified by StatisticsSize is not
  big enough for all of the statistics that are collected by the network
  interface, then a partial buffer of statistics is returned in
  StatisticsTable.

  @param [in] SimpleNetwork    Protocol instance pointer
  @param [in] Reset            Set to TRUE to reset the statistics for the network interface.
  @param [in, out] StatisticsSize  On input the size, in bytes, of StatisticsTable.  On output
                                the size, in bytes, of the resulting table of statistics.
  @param [out] StatisticsTable A pointer to the EFI_NETWORK_STATISTICS structure that
                                conains the statistics.

  @retval EFI_SUCCESS           This operation was successful.
  @retval EFI_NOT_STARTED       The network interface was not started.
  @retval EFI_BUFFER_TOO_SMALL  The StatisticsTable is NULL or the buffer is too small.
  @retval EFI_INVALID_PARAMETER SimpleNetwork parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       The increased buffer size feature is not supported.

**/
EFI_STATUS
EFIAPI
SN_Statistics (
  IN     EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork,
  IN     BOOLEAN                     Reset,
  IN OUT UINTN                       *StatisticsSize,
  OUT    EFI_NETWORK_STATISTICS      *StatisticsTable
  );

/**
  This function stops a network interface.  This call is only valid
  if the network interface is in the started state.

  @param [in] SimpleNetwork    Protocol instance pointer

  @retval EFI_SUCCESS           This operation was successful.
  @retval EFI_NOT_STARTED       The network interface was not started.
  @retval EFI_INVALID_PARAMETER SimpleNetwork parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       The increased buffer size feature is not supported.

**/
EFI_STATUS
EFIAPI
SN_Stop (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork
  );

/**
  This function releases the memory buffers assigned in the Initialize() call.
  Ending transmits and receives are lost, and interrupts are cleared and disabled.
  After this call, only Initialize() and Stop() calls may be used.

  @param [in] SimpleNetwork    Protocol instance pointer

  @retval EFI_SUCCESS           This operation was successful.
  @retval EFI_NOT_STARTED       The network interface was not started.
  @retval EFI_INVALID_PARAMETER SimpleNetwork parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       The increased buffer size feature is not supported.

**/
EFI_STATUS
EFIAPI
SN_Shutdown (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork
  );

/**
  Send a packet over the network.

  This function places the packet specified by Header and Buffer on
  the transmit queue.  This function performs a non-blocking transmit
  operation.  When the transmit is complete, the buffer is returned
  via the GetStatus() call.

  This routine calls ::Ax88179Rx to empty the network adapter of
  receive packets.  The routine then passes the transmit packet
  to the network adapter.

  @param [in] SimpleNetwork    Protocol instance pointer
  @param [in] HeaderSize        The size, in bytes, of the media header to be filled in by
                                the Transmit() function.  If HeaderSize is non-zero, then
                                it must be equal to SimpleNetwork->Mode->MediaHeaderSize
                                and DestAddr and Protocol parameters must not be NULL.
  @param [in] BufferSize        The size, in bytes, of the entire packet (media header and
                                data) to be transmitted through the network interface.
  @param [in] Buffer           A pointer to the packet (media header followed by data) to
                                to be transmitted.  This parameter can not be NULL.  If
                                HeaderSize is zero, then the media header is Buffer must
                                already be filled in by the caller.  If HeaderSize is nonzero,
                                then the media header will be filled in by the Transmit()
                                function.
  @param [in] SrcAddr          The source HW MAC address.  If HeaderSize is zero, then
                                this parameter is ignored.  If HeaderSize is nonzero and
                                SrcAddr is NULL, then SimpleNetwork->Mode->CurrentAddress
                                is used for the source HW MAC address.
  @param [in] DestAddr         The destination HW MAC address.  If HeaderSize is zero, then
                                this parameter is ignored.
  @param [in] Protocol         The type of header to build.  If HeaderSize is zero, then
                                this parameter is ignored.

  @retval EFI_SUCCESS           This operation was successful.
  @retval EFI_NOT_STARTED       The network interface was not started.
  @retval EFI_NOT_READY         The network interface is too busy to accept this transmit request.
  @retval EFI_BUFFER_TOO_SMALL  The BufferSize parameter is too small.
  @retval EFI_INVALID_PARAMETER SimpleNetwork parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.

**/
EFI_STATUS
EFIAPI
SN_Transmit (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork,
  IN UINTN           HeaderSize,
  IN UINTN           BufferSize,
  IN VOID            *Buffer,
  IN EFI_MAC_ADDRESS *SrcAddr,
  IN EFI_MAC_ADDRESS *DestAddr,
  IN UINT16          *Protocol
  );

//------------------------------------------------------------------------------
// Support Routines
//------------------------------------------------------------------------------

/**
  Get the MAC address

  This routine calls ::Ax88179UsbCommand to request the MAC
  address from the network adapter.

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure
  @param [out] MacAddress      Address of a six byte buffer to receive the MAC address.

  @retval EFI_SUCCESS          The MAC address is available.
  @retval other                The MAC address is not valid.

**/
EFI_STATUS
Ax88179MacAddressGet (
  IN NIC_DEVICE *NicDevice,
  OUT UINT8     *MacAddress
  );


/**
  Clear the multicast hash table

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure

**/
VOID
Ax88179MulticastClear (
  IN NIC_DEVICE *NicDevice
  );

/**
  Enable a multicast address in the multicast hash table

  This routine calls ::Ax88179Crc to compute the hash bit for
  this MAC address.

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure
  @param [in] MacAddress      Address of a six byte buffer to containing the MAC address.

**/
VOID
Ax88179MulticastSet (
  IN NIC_DEVICE *NicDevice,
  IN UINT8      *MacAddress
  );

/**
  Start the link negotiation

  This routine calls ::Ax88179PhyWrite to start the PHY's link
  negotiation.

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure

  @retval EFI_SUCCESS          The link negotiation was started.
  @retval other                Failed to start the link negotiation.

**/
EFI_STATUS
Ax88179NegotiateLinkStart (
  IN NIC_DEVICE *NicDevice
  );

/**
  Complete the negotiation of the PHY link

  This routine calls ::Ax88179PhyRead to determine if the
  link negotiation is complete.

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure
  @param [in, out] PollCount  Address of number of times this routine was polled
  @param [out] Complete      Address of boolean to receive complate status.
  @param [out] LinkUp        Address of boolean to receive link status, TRUE=up.
  @param [out] HiSpeed       Address of boolean to receive link speed, TRUE=100Mbps.
  @param [out] FullDuplex    Address of boolean to receive link duplex, TRUE=full.

  @retval EFI_SUCCESS          The MAC address is available.
  @retval other                The MAC address is not valid.

**/
EFI_STATUS
Ax88179NegotiateLinkComplete (
  IN     NIC_DEVICE *NicDevice,
  IN OUT UINTN      *PollCount,
  OUT    BOOLEAN    *Complete,
  OUT    BOOLEAN    *LinkUp,
  OUT    BOOLEAN    *HiSpeed,
  OUT    BOOLEAN    *GigaSpeed,
  OUT    BOOLEAN    *FullDuplex
  );

/**
  Read a register from the PHY

  This routine calls ::Ax88179UsbCommand to read a PHY register.

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure
  @param [in] RegisterAddress  Number of the register to read.
  @param [in, out] PhyData    Address of a buffer to receive the PHY register value

  @retval EFI_SUCCESS          The PHY data is available.
  @retval other                The PHY data is not valid.

**/


/**
  Reset the AX88179

  This routine uses ::Ax88179UsbCommand to reset the network
  adapter.  This routine also uses ::Ax88179PhyWrite to reset
  the PHY.

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure

  @retval EFI_SUCCESS          The MAC address is available.
  @retval other                The MAC address is not valid.

**/
EFI_STATUS
Ax88179Reset (
  IN NIC_DEVICE *NicDevice
  );

/**
  Enable or disable the receiver

  This routine calls ::Ax88179UsbCommand to update the
  receiver state.  This routine also calls ::Ax88179MacAddressSet
  to establish the MAC address for the network adapter.

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure
  @param [in] RxFilter         Simple network RX filter mask value

  @retval EFI_SUCCESS          The MAC address was set.
  @retval other                The MAC address was not set.

**/
EFI_STATUS
Ax88179RxControl (
  IN NIC_DEVICE *NicDevice,
  IN UINT32     RxFilter
  );

  EFI_STATUS
Ax88179ReloadSrom  (
  IN NIC_DEVICE *NicDevice
  );

/**
  Read an SROM location

  This routine calls ::Ax88179UsbCommand to read data from the
  SROM.

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure
  @param [in] Address          SROM address
  @param [out] Data           Buffer to receive the data

  @retval EFI_SUCCESS          The read was successful
  @retval other                The read failed

**/
EFI_STATUS
Ax88179SromRead (
  IN  NIC_DEVICE *NicDevice,
  IN  UINT32     Address,
  OUT UINT16     *Data
  );


EFI_STATUS
Ax88179EnableSromWrite  (
  IN NIC_DEVICE *NicDevice
  );


EFI_STATUS
Ax88179DisableSromWrite  (
  IN NIC_DEVICE *NicDevice
  );

EFI_STATUS
Ax88179SromWrite (
  IN  NIC_DEVICE *NicDevice,
  IN  UINT32     Address,
  OUT UINT16     *Data
  );

/**
  Send a command to the USB device.

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure
  @param [in] Request         Pointer to the request structure
  @param [in, out] Buffer     Data buffer address

  @retval EFI_SUCCESS          The USB transfer was successful
  @retval other                The USB transfer failed

**/

EFI_STATUS
Ax88179UsbCommand (
  IN NIC_DEVICE         *NicDevice,
  IN USB_DEVICE_REQUEST *Request,
  IN OUT VOID           *Buffer
  );

//------------------------------------------------------------------------------
// EFI Component Name Protocol Support
//------------------------------------------------------------------------------
extern EFI_DRIVER_BINDING_PROTOCOL   gDriverBinding;
extern EFI_COMPONENT_NAME_PROTOCOL   gComponentName;  ///<  Component name protocol declaration
extern EFI_COMPONENT_NAME2_PROTOCOL  gComponentName2; ///<  Component name 2 protocol declaration

/**
  Retrieves a Unicode string that is the user readable name of the driver.

  This function retrieves the user readable name of a driver in the form of a
  Unicode string. If the driver specified by This has a user readable name in
  the language specified by Language, then a pointer to the driver name is
  returned in DriverName, and EFI_SUCCESS is returned. If the driver specified
  by This does not support the language specified by Language,
  then EFI_UNSUPPORTED is returned.

  @param [in] This             A pointer to the EFI_COMPONENT_NAME2_PROTOCOL or
                                EFI_COMPONENT_NAME_PROTOCOL instance.
  @param [in] Language         A pointer to a Null-terminated ASCII string
                                array indicating the language. This is the
                                language of the driver name that the caller is
                                requesting, and it must match one of the
                                languages specified in SupportedLanguages. The
                                number of languages supported by a driver is up
                                to the driver writer. Language is specified
                                in RFC 3066 or ISO 639-2 language code format.
  @param [out] DriverName     A pointer to the Unicode string to return.
                                This Unicode string is the name of the
                                driver specified by This in the language
                                specified by Language.

  @retval EFI_SUCCESS           The Unicode string for the Driver specified by
                                This and the language specified by Language was
                                returned in DriverName.
  @retval EFI_INVALID_PARAMETER Language is NULL.
  @retval EFI_INVALID_PARAMETER DriverName is NULL.
  @retval EFI_UNSUPPORTED       The driver specified by This does not support
                                the language specified by Language.

**/
EFI_STATUS
EFIAPI
GetDriverName (
  IN  EFI_COMPONENT_NAME_PROTOCOL *This,
  IN  CHAR8                       *Language,
  OUT CHAR16                      **DriverName
  );


/**
  Retrieves a Unicode string that is the user readable name of the controller
  that is being managed by a driver.

  This function retrieves the user readable name of the controller specified by
  ControllerHandle and ChildHandle in the form of a Unicode string. If the
  driver specified by This has a user readable name in the language specified by
  Language, then a pointer to the controller name is returned in ControllerName,
  and EFI_SUCCESS is returned.  If the driver specified by This is not currently
  managing the controller specified by ControllerHandle and ChildHandle,
  then EFI_UNSUPPORTED is returned.  If the driver specified by This does not
  support the language specified by Language, then EFI_UNSUPPORTED is returned.

  @param [in] This             A pointer to the EFI_COMPONENT_NAME2_PROTOCOL or
                                EFI_COMPONENT_NAME_PROTOCOL instance.
  @param [in] ControllerHandle  The handle of a controller that the driver
                                specified by This is managing.  This handle
                                specifies the controller whose name is to be
                                returned.
  @param [in] ChildHandle       The handle of the child controller to retrieve
                                the name of.  This is an optional parameter that
                                may be NULL.  It will be NULL for device
                                drivers.  It will also be NULL for a bus drivers
                                that wish to retrieve the name of the bus
                                controller.  It will not be NULL for a bus
                                driver that wishes to retrieve the name of a
                                child controller.
  @param [in] Language         A pointer to a Null-terminated ASCII string
                                array indicating the language.  This is the
                                language of the driver name that the caller is
                                requesting, and it must match one of the
                                languages specified in SupportedLanguages. The
                                number of languages supported by a driver is up
                                to the driver writer. Language is specified in
                                RFC 3066 or ISO 639-2 language code format.
  @param [out] ControllerName A pointer to the Unicode string to return.
                                This Unicode string is the name of the
                                controller specified by ControllerHandle and
                                ChildHandle in the language specified by
                                Language from the point of view of the driver
                                specified by This.

  @retval EFI_SUCCESS           The Unicode string for the user readable name in
                                the language specified by Language for the
                                driver specified by This was returned in
                                DriverName.
  @retval EFI_INVALID_PARAMETER ControllerHandle is not a valid EFI_HANDLE.
  @retval EFI_INVALID_PARAMETER ChildHandle is not NULL and it is not a valid
                                EFI_HANDLE.
  @retval EFI_INVALID_PARAMETER Language is NULL.
  @retval EFI_INVALID_PARAMETER ControllerName is NULL.
  @retval EFI_UNSUPPORTED       The driver specified by This is not currently
                                managing the controller specified by
                                ControllerHandle and ChildHandle.
  @retval EFI_UNSUPPORTED       The driver specified by This does not support
                                the language specified by Language.

**/
EFI_STATUS
EFIAPI
GetControllerName (
  IN  EFI_COMPONENT_NAME_PROTOCOL *This,
  IN  EFI_HANDLE                  ControllerHandle,
  IN OPTIONAL EFI_HANDLE          ChildHandle,
  IN  CHAR8                       *Language,
  OUT CHAR16                      **ControllerName
  );

EFI_STATUS
Ax88179SetMedium (
  IN NIC_DEVICE * NicDevice
  );

//-----------------------------------------------------------------------------


#define ACCESS_MAC  0x01
#define ACCESS_PHY  0x02

#define PLSR                0x02
  #define PLSR_USB_FS  0x01
  #define PLSR_USB_HS  0x02
  #define PLSR_USB_SS  0x04

#define NODE_ID               0x10

#define RXCTL                 0x0b
  #define RXCTL_PRO         0x0001  ///<  Receive all packets
  #define RXCTL_AMALL       0x0002  ///<  Receive all multicast packets
  #define RXCTL_SEP         0x0004  ///<  Save error packets
  #define RXCTL_AB          0x0008  ///<  Receive broadcast packets
  #define RXCTL_AM          0x0010  ///<  Use multicast destination address hash table
  #define RXCTL_AP          0x0020  ///<  Accept physical address from Multicast Filter
  #define RXCTL_ARP         0x0040  ///<  Accept runt packet
  #define RXCTL_SO          0x0080  ///<  Start operation
  #define RXCTL_DROPCRCERR  0x0100  ///<  RX header 1 formate selection
  #define RXCTL_IPE         0x0200  ///<  RX header 2 formate selection
  #define RXCTL_TXPADCRC    0x0400  ///<  RX header 3 formate selection
  #define RXCTL_LPBK        0x1000  ///<  MAC loop back for diagnostic

#define MULCATFLTARRY       0x16  ///<  Write the multicast hash table, Data: New 8 byte value

#define MEDIUMSTSMOD          0x22
  #define MEDIUMSTSMOD_GM   0x0001  ///<  Gigabit Mode
  #define MEDIUMSTSMOD_FD   0x0002  ///<  Full duplex
  #define MEDIUMSTSMOD_ONE  0x0004  ///<  Must be one
  #define MEDIUMSTSMOD_ENCK 0x0008  ///<  Add in 04h
  #define MEDIUMSTSMOD_RFC  0x0010  ///<  RX flow control enable
  #define MEDIUMSTSMOD_TFC  0x0020  ///<  TX flow control enable
  #define MEDIUMSTSMOD_JFE  0x0040  ///<  Pause frame enable
  #define MEDIUMSTSMOD_PF   0x0080  ///<  Pause frame enable
  #define MEDIUMSTSMOD_RE   0x0100  ///<  Receive enable
  #define MEDIUMSTSMOD_PS   0x0200  ///<  Port speed 1=100, 0=10 Mbps
  #define MEDIUMSTSMOD_SBP  0x0800  ///<  Stop back pressure
  #define MEDIUMSTSMOD_SM   0x1000  ///<  Super MAC support

#define MONITORSTAUS          0x24
  #define  MONITORSTAUS_PMETYPE   0x40
  #define  MONITORSTAUS_PMEPOL    0x20

#define PHYPWRRSTCTL          0x26
  #define PHYPWRRSTCTL_IPRL     0x20
  #define PHYPWRRSTCTL_OSCDSSEL 0x80
  #define PHYPWRRSTCTL_BZTYPE   0x4
  #define PHYPWRRSTCTL_BZ      0x10

#define SISSR                 0x28
  #define SISSR_PSEL        0x01
  #define SISSR_ASEL        0x02

#define RXBINQCTRL            0x2e
  #define RXBINQCTRL_TIMEN  0x01
  #define RXBINQCTRL_IFGEN  0x02
  #define RXBINQCTRL_SIZEN  0x04

#define RXBINQTIMERL          0x2f
#define RXBINQTIMERH          0x30
#define RXBINQSIZE            0x31
#define RXBINQIFG             0x32

#define CLKSELECT             0x33
  #define CLKSELECT_ACS       0x02
  #define CLKSELECT_BCS       0x01

#define PAUSE_WATERLVL_HIGH   0x54
#define PAUSE_WATERLVL_LOW    0x55

#define RXFREEBUF             0x57
  #define MAX_RXBUF_PAGE    0x067F

#define PHY_BMCR                        0       ///<  Control register
  #define BMCR_RESET                      0x8000  ///<  1 = Reset the PHY, bit clears after reset
  #define BMCR_LOOPBACK                   0x4000  ///<  1 = Loopback enabled
  #define BMCR_AUTONEGOTIATION_ENABLE     0x1000  ///<  1 = Enable autonegotiation
  #define BMCR_POWER_DOWN                 0x0800  ///<  1 = Power down
  #define BMCR_ISOLATE                    0x0400  ///<  0 = Isolate PHY
  #define BMCR_RESTART_AUTONEGOTIATION    0x0200  ///<  1 = Restart autonegotiation
  #define BMCR_FULL_DUPLEX                0x0100  ///<  Full duplex operation
  #define BMCR_COLLISION_TEST             0x0080  ///<  1 = Collision test enabled
  #define BMCR_1000MBPS                   0x40  ///<  Forced mode in 1000Mbps
  #define BMCR_100MBPS                    0x2000    ///<  Forced mode in 10Mbps

#define PHY_BMSR                        1       ///<  Status register
  #define BMSR_100BASET4                  0x8000  ///<  1 = 100BASE-T4 mode
  #define BMSR_100BASETX_FDX              0x4000  ///<  1 = 100BASE-TX full duplex
  #define BMSR_100BASETX_HDX              0x2000  ///<  1 = 100BASE-TX half duplex
  #define BMSR_10BASET_FDX                0x1000  ///<  1 = 10BASE-T full duplex
  #define BMSR_10BASET_HDX                0x0800  ///<  1 = 10BASE-T half duplex
  #define BMSR_MF                         0x0040  ///<  1 = PHY accepts frames with preamble suppressed
  #define BMSR_AUTONEG_CMPLT              0x0020  ///<  1 = Autonegotiation complete
  #define BMSR_RF                         0x0010  ///<  1 = Remote fault
  #define BMSR_AUTONEG                    0x0008  ///<  1 = Able to perform autonegotiation
  #define BMSR_LINKST                     0x0004  ///<  1 = Link up
  #define BMSR_JABBER_DETECT              0x0002  ///<  1 = jabber condition detected
  #define BMSR_EXTENDED_CAPABILITY        0x0001  ///<  1 = Extended register capable

#define PHY_ANAR                        4       ///<  Autonegotiation advertisement register
  #define AN_NP                           0x8000  ///<  1 = Next page available
  #define AN_RF                           0x2000  ///<  1 = Remote fault indicated by link partner
  #define AN_FCS                          0x0400  ///<  1 = Flow control ability
  #define AN_T4                           0x0200  ///<  1 = 100BASE-T4 support
  #define AN_TX_FDX                       0x0100  ///<  1 = 100BASE-TX Full duplex
  #define AN_TX_HDX                       0x0080  ///<  1 = 100BASE-TX support
  #define AN_10_FDX                       0x0040  ///<  1 = 10BASE-T Full duplex
  #define AN_10_HDX                       0x0020  ///<  1 = 10BASE-T support
  #define AN_CSMA_CD                      0x0001  ///<  1 = IEEE 802.3 CSMA/CD support

#define PHY_ANLPAR                      5       ///<  Autonegotiation link parter ability register

#define PHY_PHYSR                       0x11
  #define PHYSR_SPEED_MASK                0xC000
  #define PHYSR_1000                      0x8000
  #define PHYSR_100                       0x4000
  #define PHYSR_FULLDUP                   0x2000
  #define PHYSR_LINK                      0x400

EFI_STATUS
Ax88179PhyRead (
  IN     NIC_DEVICE *NicDevice,
  IN     UINT8      RegisterAddress,
  IN OUT UINT16     *PhyData
  );

EFI_STATUS
Ax88179PhyWrite (
  IN NIC_DEVICE *NicDevice,
  IN UINT8      RegisterAddress,
  IN UINT16     PhyData
  );

EFI_STATUS
Ax88179MacRead (
  IN            UINT8 Offset,
  IN            UINT8 Length,
  IN            NIC_DEVICE * NicDevice,
  IN  OUT  VOID *Data
  );

EFI_STATUS
Ax88179SetIInInterval (
  IN  NIC_DEVICE *NicDevice,
  IN  UINT8      Offset
  );

EFI_STATUS
Ax88179MacWrite (
  IN      UINT8      Offset,
  IN      UINT8      Length,
  IN      NIC_DEVICE *NicDevice,
  IN  OUT VOID       *Data
  );

EFI_STATUS
Ax88179MacAddressGet (
  IN  NIC_DEVICE *NicDevice,
  OUT UINT8      *MacAddress
  );

EFI_STATUS
Ax88179MacAddressSet (
  IN  NIC_DEVICE *NicDevice,
  OUT UINT8      *MacAddress
  );

BOOLEAN
Ax88179GetLinkStatus (
  IN NIC_DEVICE *NicDevice
);

EFI_STATUS
Ax88179BulkIn(
  IN NIC_DEVICE *NicDevice
);


#endif  //  AX88179_H_
