/** @file
  Header file for GpioPrivateLib.
  All function in this library is available for PEI, DXE, and SMM,

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _GPIO_PRIVATE_LIB_H_
#define _GPIO_PRIVATE_LIB_H_

#include <GpioConfig.h>
#include <Library/PchPcrLib.h>

//
// Structure for native pin data
//
typedef struct {
  GPIO_PAD       Pad;
  GPIO_PAD_MODE  Mode;
} GPIO_PAD_NATIVE_FUNCTION;

//
// Below defines are based on GPIO_CONFIG structure fields
//
#define B_GPIO_PAD_MODE_MASK                            0xF
#define N_GPIO_PAD_MODE_BIT_POS                         0
#define B_GPIO_HOSTSW_OWN_MASK                          0x3
#define N_GPIO_HOSTSW_OWN_BIT_POS                       0
#define B_GPIO_DIRECTION_MASK                           0x1F
#define B_GPIO_DIRECTION_DIR_MASK                       0x7
#define N_GPIO_DIRECTION_DIR_BIT_POS                    0
#define B_GPIO_DIRECTION_INV_MASK                       0x18
#define N_GPIO_DIRECTION_INV_BIT_POS                    3
#define B_GPIO_OUTPUT_MASK                              0x3
#define N_GPIO_OUTPUT_BIT_POS                           0
#define N_GPIO_INT_CONFIG_INT_SOURCE_BIT_POS            0
#define N_GPIO_INT_CONFIG_INT_TYPE_BIT_POS              5
#define B_GPIO_RESET_CONFIG_RESET_MASK                  0x3F
#define N_GPIO_RESET_CONFIG_OLD_RESET_TYPE              BIT1
#define B_GPIO_RESET_CONFIG_OLD_RESET_MASK              0xF
#define N_GPIO_RESET_CONFIG_RESET_BIT_POS               0
#define B_GPIO_RESET_CONFIG_GPD_RESET_MASK              (BIT5 | BIT4)
#define B_GPIO_RESET_CONFIG_GPP_RESET_MASK              (BIT3 | BIT2)
#define N_GPIO_ELECTRICAL_CONFIG_TERMINATION_BIT_POS    0
#define N_GPIO_OTHER_CONFIG_RXRAW_BIT_POS               0

//
// Structure for storing information about registers offset, community,
// maximal pad number for available groups
//
typedef struct {
  PCH_SBI_PID  Community;
  UINT16       PadOwnOffset;
  UINT16       HostOwnOffset;
  UINT16       GpiIsOffset;
  UINT16       GpiIeOffset;
  UINT16       GpiGpeStsOffset;
  UINT16       GpiGpeEnOffset;
  UINT16       SmiStsOffset;
  UINT16       SmiEnOffset;
  UINT16       NmiStsOffset;
  UINT16       NmiEnOffset;
  UINT16       PadCfgLockOffset;
  UINT16       PadCfgLockTxOffset;
  UINT16       PadCfgOffset;
  UINT16       PadPerGroup;
} GPIO_GROUP_INFO;

//
// If in GPIO_GROUP_INFO structure certain register doesn't exist
// it will have value equal to NO_REGISTER_FOR_PROPERTY
//
#define NO_REGISTER_FOR_PROPERTY 0xFFFF

/**
  This procedure will retrieve address and length of GPIO info table

  @param[out]  GpioGroupInfoTableLength   Length of GPIO group table

  @retval Pointer to GPIO group table
**/
CONST GPIO_GROUP_INFO*
GpioGetGroupInfoTable (
  OUT UINT32              *GpioGroupInfoTableLength
  );

typedef struct {
  CONST CHAR8*    GpioGroupPrefix;
  CONST GPIO_PAD  FirstUniqueGpio;
  CONST CHAR8**   GroupUniqueNames;
  CONST UINT32    UniqueNamesTableSize;
} GPIO_GROUP_NAME_INFO;

//
// Helper macros for initializing GPIO_GROUP_NAME_INFO structures
//
#define GPIO_GROUP_NAME(GroupName,FirstUniqueGpio,GroupUniqueNamesTable) \
  {GroupName, FirstUniqueGpio, GroupUniqueNamesTable, ARRAY_SIZE (GroupUniqueNamesTable)}

#define GPIO_GROUP_NAME_BASIC(GroupName) \
  {GroupName, 0, NULL, 0}

/**
  Returns GPIO_GROUP_NAME_INFO corresponding to the give GpioPad

  @param[in]  GroupIndex  Group index

  @retval  GPIO_GROUP_NAME_INFO*  Pointer to the GPIO_GROUP_NAME_INFO
  @retval  NULL                   If no group descriptor was found
**/
CONST
GPIO_GROUP_NAME_INFO*
GpioGetGroupNameInfo (
  IN UINT32  GroupIndex
  );

/**
  Get GPIO Chipset ID specific to PCH generation and series
**/
UINT32
GpioGetThisChipsetId (
  VOID
  );

/**
  This procedure is used to check if GpioPad is valid for certain chipset

  @param[in]  GpioPad             GPIO pad

  @retval TRUE                    This pin is valid on this chipset
          FALSE                   Incorrect pin
**/
BOOLEAN
GpioIsCorrectPadForThisChipset (
  IN  GPIO_PAD        GpioPad
  );

/**
  Generates GPIO name from GpioPad
  This function returns pointer to the static buffer.

  @param[in] GpioPad  GpioPad

  @retval CHAR8*  Pointer to the GPIO name
**/
CHAR8*
GpioName (
  IN GPIO_PAD  GpioPad
  );

/**
  This procedure will get value of selected gpio register

  @param[in]  Group               GPIO group number
  @param[in]  Offset              GPIO register offset
  @param[out] RegVal              Value of gpio register

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetReg (
  IN  GPIO_GROUP              Group,
  IN  UINT32                  Offset,
  OUT UINT32                  *RegVal
  );

/**
  This procedure will set value of selected gpio register

  @param[in] Group               GPIO group number
  @param[in] Offset              GPIO register offset
  @param[in] RegVal              Value of gpio register

  @retval EFI_SUCCESS            The function completed successfully
  @retval EFI_INVALID_PARAMETER  Invalid group or pad number
**/
EFI_STATUS
GpioSetReg (
  IN GPIO_GROUP              Group,
  IN UINT32                  Offset,
  IN UINT32                  RegVal
  );

/**
  This procedure is used by PchSmiDispatcher and will return information
  needed to register GPI SMI.

  @param[in]  Index                   GPI SMI number
  @param[out] GpioPin                 GPIO pin
  @param[out] GpiSmiBitOffset         GPI SMI bit position within GpiSmi Registers
  @param[out] GpiHostSwOwnRegAddress  Address of HOSTSW_OWN register
  @param[out] GpiSmiStsRegAddress     Address of GPI SMI status register

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetPadAndSmiRegs (
  IN UINT32            Index,
  OUT GPIO_PAD         *GpioPin,
  OUT UINT8            *GpiSmiBitOffset,
  OUT UINT32           *GpiHostSwOwnRegAddress,
  OUT UINT32           *GpiSmiStsRegAddress
  );

/**
  This procedure will set GPIO Driver IRQ number

  @param[in]  Irq                 Irq number

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid IRQ number
**/
EFI_STATUS
GpioSetIrq (
  IN  UINT8          Irq
  );

/**
  This function provides GPIO Community PortIDs

  @param[out] NativePinsTable                Table with GPIO COMMx SBI PortIDs

  @retval      Number of communities
**/
UINT32
GpioGetComSbiPortIds (
  OUT PCH_SBI_PID    **GpioComSbiIds
  );

/**
  This procedure will perform special handling of GPP_A_12.

  @param[in]  None

  @retval None
**/
VOID
GpioA12SpecialHandling (
  VOID
  );

//
// Structure which stores information needed to map GPIO Group
// to 1-Tier GPE. Configuration is needed both in PMC and GPIO IP.
// Because GPE_DWx can handle only 32 pins only single double word can
// be mapped at a time. Each DW for a group has different configuration in PMC and GPIO
//
typedef struct {
  GPIO_GROUP  Group;
  UINT8       GroupDw;
  UINT8       PmcGpeDwxVal;
  UINT8       GpioGpeDwxVal;
} GPIO_GROUP_TO_GPE_MAPPING;

/**
  Get information for GPIO Group required to program GPIO and PMC for desired 1-Tier GPE mapping

  @param[out] GpioGroupToGpeMapping        Table with GPIO Group to GPE mapping
  @param[out] GpioGroupToGpeMappingLength  GPIO Group to GPE mapping table length
**/
VOID
GpioGetGroupToGpeMapping (
  OUT GPIO_GROUP_TO_GPE_MAPPING  **GpioGroupToGpeMapping,
  OUT UINT32                     *GpioGroupToGpeMappingLength
  );

/**
  This procedure will return Port ID of GPIO Community from GpioPad

  @param[in] GpioPad            GpioPad

  @retval GpioCommunityPortId   Port ID of GPIO Community
**/
UINT8
GpioGetGpioCommunityPortIdFromGpioPad (
  IN GPIO_PAD        GpioPad
  );

/**
  This procedure will return PadCfg address from GpioPad

  @param[in] GpioPad            GpioPad

  @retval GpioPadCfgAddress     PadCfg Address of GpioPad
**/
UINT32
GpioGetGpioPadCfgAddressFromGpioPad (
  IN GPIO_PAD        GpioPad
  );

/**
  This procedure is used to unlock all GPIO pads.
  This function can only be called when platform is still in HOSTIA_BOOT_SAI.
**/
VOID
GpioUnlockAllPads (
  VOID
  );

/**
  This procedure will check if GpioPad is owned by host.

  @param[in] GpioPad       GPIO pad

  @retval TRUE             GPIO pad is owned by host
  @retval FALSE            GPIO pad is not owned by host and should not be used with GPIO lib API
**/
BOOLEAN
GpioIsPadHostOwned (
  IN GPIO_PAD             GpioPad
  );


/**
  This procedure will check if GpioPad argument is valid.
  Function will check below conditions:
   - GpioPad represents a pad for current PCH
   - GpioPad belongs to valid GpioGroup
   - GPIO PadNumber is not greater than number of pads for this group

  @param[in] GpioPad       GPIO pad

  @retval TRUE             GPIO pad is valid and can be used with GPIO lib API
  @retval FALSE            GPIO pad is invalid and cannot be used with GPIO lib API
**/
BOOLEAN
GpioIsPadValid (
  IN GPIO_PAD             GpioPad
  );

/**
  This procedure will read GPIO Pad Configuration register

  @param[in] GpioPad          GPIO pad
  @param[in] DwReg            Choose PADCFG register: 0:DW0, 1:DW1

  @retval PadCfgRegValue      PADCFG_DWx value
**/
UINT32
GpioReadPadCfgReg (
  IN GPIO_PAD             GpioPad,
  IN UINT8                DwReg
  );

/**
  This procedure will write or read GPIO Pad Configuration register

  @param[in] GpioPad              GPIO pad
  @param[in] DwReg                Choose PADCFG register: 0:DW0, 1:DW1
  @param[in] PadCfgAndMask        Mask to be AND'ed with PADCFG reg value
  @param[in] PadCfgOrMask         Mask to be OR'ed with PADCFG reg value

  @retval none
**/
VOID
GpioWritePadCfgReg (
  IN GPIO_PAD             GpioPad,
  IN UINT8                DwReg,
  IN UINT32               PadCfgAndMask,
  IN UINT32               PadCfgOrMask
  );

/**
  This procedure will set GPIO mode

  @param[in]  GpioPad             GPIO pad
  @param[out] PadModeValue        GPIO pad mode value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioSetPadMode (
  IN GPIO_PAD                GpioPad,
  IN GPIO_PAD_MODE           PadModeValue
  );

/**
  This procedure will get GPIO mode

  @param[in]  GpioPad             GPIO pad
  @param[out] PadModeValue        GPIO pad mode value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetPadMode (
  IN  GPIO_PAD                 GpioPad,
  OUT GPIO_PAD_MODE            *PadModeValue
  );

/**
  This procedure will check if group is within DeepSleepWell.

  @param[in]  Group               GPIO Group

  @retval GroupWell               TRUE:  This is DSW Group
                                  FALSE: This is not DSW Group
**/
BOOLEAN
GpioIsDswGroup (
  IN  GPIO_GROUP         Group
  );

/**
  The function performs GPIO Power Management programming.
**/
VOID
GpioConfigurePm (
  VOID
  );

/**
  This function sets SerialIo I2C controller pins into native mode

  @param[in]  SerialIoI2cControllerNumber   I2C controller
  @param[in]  GpioTermination               GPIO termination type

  @retval Status
**/
EFI_STATUS
GpioEnableSerialIoI2c (
  IN  UINT32                  SerialIoI2cControllerNumber,
  IN  GPIO_ELECTRICAL_CONFIG  GpioTermination
  );

/**
  This function sets SerialIo UART controller pins into native mode

  @param[in]  SerialIoUartControllerNumber   UART controller
  @param[in]  HardwareFlowControl            Hardware Flow control
  @param[in]  PinMuxing                      UART controller pin muxing

  @retval Status
**/
EFI_STATUS
GpioEnableSerialIoUart (
  IN  UINT32            SerialIoUartControllerNumber,
  IN  BOOLEAN           HardwareFlowControl,
  IN  UINT32            PinMuxing
  );

/**
  This function sets SerialIo SPI controller pins into native mode

  @param[in]  SerialIoSpiControllerNumber   SPI controller

  @retval Status
**/
EFI_STATUS
GpioEnableSerialIoSpi (
  IN  UINT32            SerialIoSpiControllerNumber
  );

/**
  This function sets ISH I2C controller pins into native mode

  @param[in]  IshI2cControllerNumber   I2C controller

  @retval Status
**/
EFI_STATUS
GpioEnableIshI2c (
  IN  UINT32            IshI2cControllerNumber
  );

/**
  This function sets ISH UART controller pins into native mode

  @param[in]  IshUartControllerNumber   UART controller

  @retval Status
**/
EFI_STATUS
GpioEnableIshUart (
  IN  UINT32            IshUartControllerNumber
  );

/**
  This function sets ISH SPI controller pins into native mode

  @param[in]  IshSpiControllerNumber   SPI controller

  @retval Status
**/
EFI_STATUS
GpioEnableIshSpi (
  IN  UINT32            IshSpiControllerNumber
  );

/**
  This function sets ISH GP pin into native mode

  @param[in]  IshGpPinNumber   ISH GP pin number

  @retval Status
**/
EFI_STATUS
GpioEnableIshGpPin (
  IN  UINT32            IshGpPinNumber
  );

/**
  This function sets SCS SD card controller pins into native mode

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnableScsSdCard (
  VOID
  );

/**
  This function enables SCS SD Card controller card detect pin

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnableScsSdCardDetect (
  VOID
  );

/**
  This function sets SCS eMMC controller pins into native mode

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnableScsEmmc (
  VOID
  );

/**
  This function sets HDA Link pins into native mode

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnableHdaLink (
  VOID
  );

/**
  This function sets HDA DMIC pins into native mode

  @param[in]  DmicNumber   DMIC number

  @retval Status
**/
EFI_STATUS
GpioEnableHdaDmic (
  IN  UINT32            DmicNumber
  );

/**
  This function sets HDA SSP interface pins into native mode

  @param[in]  SspInterfaceNumber   SSPx interface number

  @retval Status
**/
EFI_STATUS
GpioEnableHdaSsp (
  IN  UINT32            SspInterfaceNumber
  );

/**
  This function sets HDA SSP Master Clock into native mode

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnableHdaSspMasterClock (
  VOID
  );

/**
  This function sets HDA SoundWire interface pins into native mode

  @param[in]  SndwInterfaceNumber   SNDWx interface number

  @retval Status
**/
EFI_STATUS
GpioEnableHdaSndw (
  IN  UINT32            SndwInterfaceNumber
  );

/**
  This function sets SMBUS controller pins into native mode

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnableSmbus (
  VOID
  );

/**
  This function sets SMBUS ALERT pins into native mode

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnableSmbusAlert (
  VOID
  );

/**
  This function enables USB OverCurrent pins by setting
  USB2 OCB pins into native mode

  @param[in]  OcPinNumber            USB OC pin number

  @retval Status
**/
EFI_STATUS
GpioEnableUsbOverCurrent (
  IN  UINTN   OcPinNumber
  );

/**
  This function sets SATA DevSlp pins into native mode

  @param[in]  SataCtrlIndex       SATA controller index
  @param[in]  SataPort            SATA port number

  @retval Status
**/
EFI_STATUS
GpioEnableSataDevSlpPin (
  IN  UINT32  SataCtrlIndex,
  IN  UINTN   SataPort
  );

/**
  This function checks if SataDevSlp pin is in native mode

  @param[in]  SataCtrlIndex       SATA controller index
  @param[in]  SataPort            SATA port
  @param[out] DevSlpPad           DevSlpPad
                                  This is an optional parameter and may be NULL.

  @retval TRUE                    DevSlp is in native mode
          FALSE                   DevSlp is not in native mode
**/
BOOLEAN
GpioIsSataDevSlpPinEnabled (
  IN  UINT32          SataCtrlIndex,
  IN  UINTN           SataPort,
  OUT GPIO_PAD        *DevSlpPad  OPTIONAL
  );

/**
  This function sets SATAGPx pin into native mode

  @param[in]  SataCtrlIndex       SATA controller index
  @param[in]  SataPort            SATA port number

  @retval Status
**/
EFI_STATUS
GpioEnableSataGpPin (
  IN  UINT32  SataCtrlIndex,
  IN  UINTN   SataPort
  );

/**
  This function provides SATA GP pin data

  @param[in]  SataCtrlIndex       SATA controller index
  @param[in]  SataPort            SATA port number
  @param[out] NativePin           SATA GP pin
**/
VOID
GpioGetSataGpPin (
  IN  UINT32                    SataCtrlIndex,
  IN  UINTN                     SataPort,
  OUT GPIO_PAD_NATIVE_FUNCTION  *NativePin
  );

/**
  This function sets SATA LED pin into native mode. SATA LED indicates
  SATA controller activity

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnableSataLed (
  VOID
  );

/**
  Returns pad for given CLKREQ# index.

  @param[in]  ClkreqIndex       CLKREQ# number

  @return CLKREQ# pad.
**/
GPIO_PAD
GpioGetClkreqPad (
  IN     UINT32   ClkreqIndex
  );

/**
  Enables CLKREQ# pad in native mode.

  @param[in]  ClkreqIndex       CLKREQ# number

  @return none
**/
VOID
GpioEnableClkreq (
  IN     UINT32   ClkreqIndex
  );

/**
  This function sets PCHHOT pin into native mode

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnablePchHot (
  VOID
  );

/**
  This function sets VRALERTB pin into native mode

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnableVrAlert (
  VOID
  );

/**
  This function sets CPU GP pins into native mode

  @param[in]  CpuGpPinNum               CPU GP pin number

  @retval Status
**/
EFI_STATUS
GpioEnableCpuGpPin (
  IN  UINT32                            CpuGpPinNum
  );

/**
This function sets CPU C10 Gate pins into native mode

@retval Status
**/
EFI_STATUS
GpioEnableCpuC10GatePin (
  VOID
  );

//
// DDSP_HPD pins
//
typedef enum {
  GpioDdspHpd0 = 0x00,
  GpioDdspHpd1 = 0x01,
  GpioDdspHpd2 = 0x02,
  GpioDdspHpd3 = 0x03,
  GpioDdspHpd4 = 0x04,
  GpioDdspHpdA = 0x10,
  GpioDdspHpdB = 0x11,
  GpioDdspHpdC = 0x12
} GPIO_DDSP_HPD;

/**
  This function sets DDSP_HPDx pin into native mode

  @param[in]  DdspHpdPin     DDSP_HPDx pin

  @retval Status
**/
EFI_STATUS
GpioEnableDpHotPlugDetect (
  IN GPIO_DDSP_HPD  DdspHpdPin
  );

/**
  This function sets HPD, VDDEN, BKLTEN and BKLTCTL pins into native mode for eDP Panel

  @retval Status
**/
EFI_STATUS
GpioEnableEdpPins (
  VOID
  );

//
// DDPx pins
//
typedef enum {
  GpioDdp1 = 0x01,
  GpioDdp2 = 0x02,
  GpioDdp3 = 0x03,
  GpioDdp4 = 0x04,
  GpioDdpA = 0x10,
  GpioDdpB = 0x11,
  GpioDdpC = 0x12,
  GpioDdpD = 0x13,
  GpioDdpF = 0x15,
} GPIO_DDP;

/**
  This function sets DDP pins into native mode

  @param[in]  DdpInterface   DDPx interface

  @retval Status
**/
EFI_STATUS
GpioEnableDpInterface (
  IN  GPIO_DDP            DdpInterface
  );

/**
  This function configures GPIO connection between CNVi and CRF

  @retval Status
**/
EFI_STATUS
GpioConfigureCnviCrfConnection (
  VOID
  );

/**
  This function enables CNVi RF Reset pin
**/
VOID
GpioEnableCnviRfResetPin (
  VOID
  );

/**
  This function enables CNVi MODEM CLKREQ pin
**/
VOID
GpioEnableCnviModemClkReqPin (
  VOID
  );

/**
  CNVi Bluetooth UART connection options
**/
typedef enum {
  GpioCnviBtUartNotConnected,
  GpioCnviBtUartToSerialIoUart0,
  GpioCnviBtUartToIshUart0,
  GpioCnviBtUartToExternalPads
} VGPIO_CNVI_BT_UART_CONNECTION_TYPE;

/**
  This function configures virtual GPIO connection for CNVi Bluetooth UART

  @param[in]  ConnectionType

  @retval Status
**/
EFI_STATUS
GpioConfigureCnviBtUartConnection (
  IN  VGPIO_CNVI_BT_UART_CONNECTION_TYPE  ConnectionType
  );

/**
  CNVi Bluetooth I2S connection options
**/
typedef enum {
  GpioCnviBtI2sNotConnected,
  GpioCnviBtI2sToSsp0,
  GpioCnviBtI2sToSsp1,
  GpioCnviBtI2sToSsp2,
  GpioCnviBtI2sToExternalPads
} VGPIO_CNVI_BT_I2S_CONNECTION_TYPE;

/**
  This function configures virtual GPIO connection for CNVi Bluetooth I2S

  @param[in]  ConnectionType

  @retval Status
**/
EFI_STATUS
GpioConfigureCnviBtI2sConnection (
  IN  VGPIO_CNVI_BT_I2S_CONNECTION_TYPE  ConnectionType
  );

/**
  CNVi MultiFunction UART connection options
**/
typedef enum {
  GpioCnviMfUart1NotConnected,
  GpioCnviMfUart1ToSerialIoUart2,
  GpioCnviMfUart1ToIshUart0,
  GpioCnviMfUart1ToExternalPads
} VGPIO_CNVI_MF_UART1_CONNECTION_TYPE;

/**
  This function configures virtual GPIO connection for CNVi MFUART1

  @param[in]  ConnectionType

  @retval Status
**/
EFI_STATUS
GpioConfigureCnviMfUart1Connection (
  IN  VGPIO_CNVI_MF_UART1_CONNECTION_TYPE  ConnectionType
  );


/**
  This function sets CNVi Bluetooth Enable value

  @param[in] Value                CNVi BT enable value
                                  0: Disable, 1: Enable
  @retval Status
**/
EFI_STATUS
GpioSetCnviBtEnState (
   IN  UINT32  Value
  );

/**
  CNVi Bluetooth UART connection options
**/
typedef enum {
  GpioCnviBtIfUart = 0,
  GpioCnviBtIfUsb,
} VGPIO_CNVI_BT_INTERFACE;

/**
  This function sets CNVi Bluetooth main host interface

  @param[in] BtInterface          CNVi BT Interface Select value
                                  GpioCnviBtIfUart: UART, GpioCnviBtIfUsb: USB
  @retval Status
**/
EFI_STATUS
GpioSetCnviBtInterface (
  IN  VGPIO_CNVI_BT_INTERFACE  BtInterface
  );

/**
  This function sets CNVi Bluetooth Wireless Charging support

  @param[in] BtWirelessCharging   CNVi BT Wireless Charging support
                                  0: Normal BT operation (no Wireless Charging support)
                                  1: Enable BT Wireless Charging
  @retval Status
**/
EFI_STATUS
GpioSetCnviBtWirelessCharging (
  IN  UINT32  BtWirelessCharging
  );

/**
  This function enables and configures CNVi Bluetooth Host wake-up interrupt

  @param[in] None

  @retval Status
**/
EFI_STATUS
GpioConfigureCnviBtHostWakeInt (
  VOID
  );

/**
  CNVi WiFi mode
**/
typedef enum {
  GpioCnviWiFiEnabled,
  GpioCnviWiFiAuto
} VGPIO_CNVI_WIFI_MODE;

/**
  This function sets CNVi WiFi mode

  @param[in] Value                CNVi WiFi Mode value
                                  GpioCnviWiFiAuto: WiFi is automatically enabled/disabled by WiFi core
                                  GpioCnviWiFiEnabled: WiFi is enabled regardless of WiFi core decision
  @retval Status
**/
EFI_STATUS
GpioSetCnviWifiMode (
  IN  VGPIO_CNVI_WIFI_MODE  WiFiMode
  );

/**
  This function enables IMGU CLKOUT native pin

  @param[in] None

  @retval Status
**/
EFI_STATUS
GpioEnableImguClkOut (
  VOID
  );

/**
  Power button debounce configuration
  Debounce time can be specified in microseconds. Only certain values according
  to below formula are supported:
   DebounceTime = (2 ^ PADCFG_DW2.DEBOUNCE)*(glitch filter clock period).
  RTC clock with f = 32 KHz is used for glitch filter.
   DebounceTime = (2 ^ PADCFG_DW2.DEBOUNCE)*(31.25 us).
  Supported DebounceTime values are following:
   DebounceTime = 0 -> Debounce feature disabled
   DebounceTime > 0 && < 250us -> Not supported
   DebounceTime = 250us - 1024000us -> Supported range (DebounceTime = 250us * 2^n)
  For values not supported by HW, they will be rounded down to closest supported one

  @param[in] DebounceTime    Debounce Time in microseconds
                             If Debounce Time = 0, Debouncer feature will be disabled
                             Function will set DebounceTime argument to rounded supported value
**/
VOID
GpioSetPwrBtnDebounceTimer (
  IN UINT32                DebounceTime
  );

/**
  Configure LPC GPIO
**/
VOID
LpcConfigureGpio (
  VOID
  );

#endif // _GPIO_PRIVATE_LIB_H_
