/** @file
  Header file for GpioPrivateLib.
  All function in this library is available for PEI, DXE, and SMM,

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _GPIO_PRIVATE_LIB_H_
#define _GPIO_PRIVATE_LIB_H_

#include <Uefi/UefiBaseType.h>
#include <Library/GpioConfig.h>
#include <Library/PchPcrLib.h>
#include <TcssPeiConfig.h>

/**
  GPIO Standby State configuration
  Standby State options for GPIO Pads
**/
typedef enum {
  GpioIosStateDefault         = 0x0,
  GpioIosStateLatchLastValue  = (0x0 << 1) | 0x01,  ///< Latch last value driven on TX, TX Enable and RX Enable
  GpioIosStateTx0Rx0RxDis     = (0x1 << 1) | 0x01,  ///< TX: 0, RX: 0 (internally), RX disabled
  GpioIosStateTx0Rx1RxDis     = (0x2 << 1) | 0x01,  ///< TX: 0, RX: 1 (internally), RX disabled
  GpioIosStateTx1Rx0RxDis     = (0x3 << 1) | 0x01,  ///< TX: 1, RX: 0 (internally), RX disabled
  GpioIosStateTx1Rx1RxDis     = (0x4 << 1) | 0x01,  ///< TX: 1, RX: 1 (internally), RX disabled
  GpioIosStateTx0RxEn         = (0x5 << 1) | 0x01,  ///< TX: 0, RX enabled
  GpioIosStateTx1RxEn         = (0x6 << 1) | 0x01,  ///< TX: 1, RX enabled
  GpioIosStateHizRx0          = (0x7 << 1) | 0x01,  ///< Hi-Z, RX: 0 (internally)
  GpioIosStateHizRx1          = (0x8 << 1) | 0x01,  ///< Hi-Z, RX: 1 (internally)
  GpioIosStateTxDisRxEn       = (0x9 << 1) | 0x01,  ///< TX Disabled and RX Enabled (i.e. wake or interrupt)
  GpioIosStateMasked          = (0xF << 1) | 0x01   ///< IO Standby signal is masked for this pad. In this mode, a pad operates as if IOStandby has not been asserted.
} GPIO_IOSTANDBY_STATE;

/**
  GPIO Standby Term configuration
  Standby Termination options for GPIO Pads
**/
typedef enum {
  GpioIosTermDefault         = 0x00,
  GpioIosTermSame            = (0x00 << 1) | 0x01, ///< Same as state specified in Term
  GpioIosTermPuDisPdDis      = (0x01 << 1) | 0x01, ///< Disable Pullup and Pulldown
  GpioIosTermPuDisPdEn       = (0x02 << 1) | 0x01, ///< Enable Pulldown
  GpioIosTermPuEnPdDis       = (0x03 << 1) | 0x01  ///< Enable Pullup
} GPIO_IOSTANDBY_TERM;

//
// Structure for native pin data
//
typedef struct {
  GPIO_PAD              Pad;
  GPIO_PAD_MODE         Mode;
  GPIO_IOSTANDBY_STATE  IosState;
  GPIO_IOSTANDBY_TERM   IosTerm;
} GPIO_PAD_NATIVE_FUNCTION;

//
// Structure for Serial GPIO pin definition
//
typedef struct {
  GPIO_PAD_NATIVE_FUNCTION  Sclock;
  GPIO_PAD_NATIVE_FUNCTION  Sload;
  GPIO_PAD_NATIVE_FUNCTION  Sdataout;
} SGPIO_PINS;

//
// Structure for USB Virtual Wire OverCurrent Pad Mode group
//
typedef struct {
  GPIO_PAD       OcRxPad;
  GPIO_PAD       OcTxPad;
} GPIO_VWOC_FUNCTION;

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
#define B_GPIO_GPIO_IOSTANDBY_STATE_MASK                ((0xF << 1) | 0x01)
#define B_GPIO_GPIO_IOSTANDBY_STATE_POS                 0
#define B_GPIO_GPIO_IOSTANDBY_TERM_MASK                 ((0x2 << 1) | 0x01)
#define B_GPIO_GPIO_IOSTANDBY_TERM_POS                  0

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

#define GPIO_PAD_DEF(Group,Pad)                (UINT32)(((Group) << 16) + (Pad))
#define GPIO_GROUP_DEF(GroupIndex,ChipsetId)   ((GroupIndex) | ((ChipsetId) << 8))
#define GPIO_GET_GROUP_INDEX(Group)            ((Group) & 0x1F)
#define GPIO_GET_GROUP_FROM_PAD(GpioPad)       (((GpioPad) & 0x0F1F0000) >> 16)
#define GPIO_GET_GROUP_INDEX_FROM_PAD(GpioPad) GPIO_GET_GROUP_INDEX (GPIO_GET_GROUP_FROM_PAD(GpioPad))
#define GPIO_GET_PAD_NUMBER(GpioPad)           ((GpioPad) & 0x1FF)
#define GPIO_GET_CHIPSET_ID(GpioPad)           (((GpioPad) >> 24) & 0xF)

#define GPIO_GET_PAD_POSITION(PadNumber)       ((PadNumber) % 32)
#define GPIO_GET_DW_NUM(PadNumber)             ((PadNumber) / 32u)

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
  Generates GPIO name from GpioNativePad
  This function returns pointer to the static buffer.

  @param[in] GpioNativePad  GpioNativePad

  @retval CHAR8*  Pointer to the GPIO name
**/
CHAR8*
GpioPinMuxName (
  IN GPIO_NATIVE_PAD  GpioNativePad
  );

/**
  Generates GPIO Pad Termination string
  This function returns pointer to the static buffer.

  @param[in] GpioPadTermination  GPIO Pad Termination

  @retval CHAR8*  Painter to the pad termianation string
**/
CHAR8*
GpioGetPadTerminationString (
  IN GPIO_ELECTRICAL_CONFIG PadTermination
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
  This function provides list of GPIO for which IO Standby State configuration
  has to be set as 'Masked'

  @param[out] GpioPadsList                Table with pads

  @retval      Number of pads
**/
UINT32
GpioGetIoStandbyStateConfigurationPadsList (
  OUT GPIO_PAD_NATIVE_FUNCTION    **GpioPadsList
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
  This procedure will Enable USB Virtual Wire Overcurrent pin

  @param[in] GpioPad             GPIO Pad

  @retval EFI_SUCCESS
**/
EFI_STATUS
GpioSetVwOverCurrentPin (
  IN GPIO_VWOC_FUNCTION GpioPad
  );

/**
  This procedure will set Native Function IOSF-SB Virtual Wire Message Generation bit
  in DW0 of requested GPIO Pad

  @param[in] GPIO_PAD   GpioPad
**/
VOID
GpioSetNafVweBit (
  IN CONST GPIO_PAD PadCfg
  );

/**
  This procedure will set GPIO mode

  @param[in] GpioPad             GPIO pad
  @param[in] PadModeValue        GPIO pad mode value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioSetPadMode (
  IN GPIO_PAD                GpioPad,
  IN GPIO_PAD_MODE           PadModeValue
  );

/**
  This procedure will set GPIO pad to native mode.
  To be used if no other settings are to be configured when enabling native mode.

  @param[in]  GpioNativePad       GPIO Pad with native mode information

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioSetNativePad (
  IN GPIO_NATIVE_PAD  GpioNativePad
  );

/**
  This procedure will set GPIO pad to native function based on provided native function
  and platform muxing selection (if needed).

  @param[in]  PadFunction         PadMode for a specific native signal. Please refer to GpioNativePads.h
  @param[in]  PinMux              GPIO Native pin mux platform config.
                                  This argument is optional and needs to be
                                  provided only if feature can be enabled
                                  on multiple pads

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioSetNativePadByFunction (
  IN UINT32  PadFunction,
  IN UINT32  PinMux
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
  This function performs initial IO Standby State related configurations
**/
VOID
GpioConfigureIoStandbyState (
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

  @param[in]  MclkIndex       MCLK index

  @retval Status
**/
EFI_STATUS
GpioEnableHdaSspMasterClock (
  IN UINT32  MclkIndex
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
  This function provides SPI IO pin for Touch Host Controller

  @param[in]  SpiIndex                  SPI1 or SPI2 - 0 or 1
  @param[in]  IoIndex                   IoIndex Valid from 0 (SPI_IO_0) to 3 (SPI_IO_3)

  @retval     NativePin                 Native Pin Configuration, 0 if SpiIndex or IoIndex is invalid
**/
GPIO_PAD_NATIVE_FUNCTION
GpioGetThcSpiIo (
  IN  UINT32                      SpiIndex,
  IN  UINT32                      IoIndex
  );

/**
  This function provides SPI ChipSelect pin for Touch Host Controller

  @param[in]  SpiIndex                  SPI1 or SPI2 - 0 or 1

  @retval     NativePin                 Native Pin Configuration, 0 if SpiIndex is invalid
**/
GPIO_PAD_NATIVE_FUNCTION
GpioGetThcSpiCs (
  IN  UINT32                      SpiIndex
  );

/**
  This function provides SPI Clock pin for Touch Host Controller

  @param[in]  SpiIndex                  SPI1 or SPI2 - 0 or 1

  @retval     NativePin                 Native Pin Configuration, 0 if SpiIndex is invalid
**/
GPIO_PAD_NATIVE_FUNCTION
GpioGetThcSpiClk (
  IN  UINT32                      SpiIndex
  );

/**
  This function provides SPI Reset pin for Touch Host Controller

  @param[in]  SpiIndex                  SPI1 or SPI2 - 0 or 1

  @retval     NativePin                 Native Pin Configuration, 0 if SpiIndex is invalid
**/
GPIO_PAD_NATIVE_FUNCTION
GpioGetThcSpiReset (
  IN  UINT32                      SpiIndex
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
  This function provides Serial GPIO pins

  @param[in]  SataCtrlIndex       SATA controller index
  @param[out] SgpioPins           SATA Serial GPIO pins
**/
VOID
GpioGetSataSgpioPins (
  IN  UINT32        SataCtrlIndex,
  OUT SGPIO_PINS    *SgpioPins
  );

/**
  This function sets Serial GPIO pins into native mode

  @param[in]  SataCtrlIndex       SATA controller index
  @param[in]  SataPort            SATA port number

  @retval Status
**/
EFI_STATUS
GpioEnableSataSgpio (
  IN  UINT32  SataCtrlIndex
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
  This function enables USB Virtual Wire OverCurrent pins by OcPinNumber.

  @param[in]  OcPinNumber            USB OC pin number

  @retval Status
**/
EFI_STATUS
GpioEnableUsbVwOverCurrent (
  IN  UINTN   OcPinNumber
  );

/**
  This function sets SATA DevSlp pins into native mode

  @param[in]  SataCtrlIndex       SATA controller index
  @param[in]  SataPort            SATA port number
  @param[in]  ResetType           GPIO reset type (see GPIO_RESET_CONFIG in GpioConfig.h)

  @retval Status
**/
EFI_STATUS
GpioEnableSataDevSlpPin (
  IN  UINT32  SataCtrlIndex,
  IN  UINTN   SataPort,
  IN  UINT32  ResetType
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

  @param[in]  SataCtrlIndex       SATA controller index
  @retval     Status
**/
EFI_STATUS
GpioEnableSataLed (
  IN  UINT32                    SataCtrlIndex
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

//
// DDI Port TBT_LSX interface
//
typedef enum {
  GpioTbtLsxDdi1,
  GpioTbtLsxDdi2,
  GpioTbtLsxDdi3,
  GpioTbtLsxDdi4,
  GpioTbtLsxDdi5,
  GpioTbtLsxDdi6
} GPIO_TBT_LSX;

/**
  This function sets TBT_LSx pin into native mode

  @param[in]  TbtLsxDdiPort     TBT_LSx DDI Port Number

  @retval     Status
**/
EFI_STATUS
GpioEnableTbtLsxInterface (
  IN GPIO_TBT_LSX  TbtLsxDdiPort
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
  This function enables IMGU CLKOUT native pin

  @param[in] ImguClkOutPinIndex    The index of IMGU CLKOUT natine pin

  @retval Status
**/
EFI_STATUS
GpioEnableImguClkOut (
  IN UINT8 ImguClkOutPinIndex
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
  VCCIO level selection
**/
typedef enum {
  GpioVcc3v3,
  GpioVcc1v8,
  MaxVccioSel
} GPIO_VCCIO_SEL;
/**
  The function sets VCCIOSEL

  @param[in]  GpioPad             GPIO pad
  @param[in]  VccioSel            Pad voltage

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_UNSUPPORTED         The Pin is owned by others
  @retval EFI_INVALID_PARAMETER   Invalid group or parameter
**/
EFI_STATUS
GpioSetVccLevel (
  IN  GPIO_PAD        GpioPad,
  IN  GPIO_VCCIO_SEL  VccioSel
  );

/**
  SBU (Sideband use) pins are used as auxiliary signals for Type C connector,
  which are hard-wired to BSSB_LS natively for debug function.
  when USB-C is enablde and debug not needed, disable pins (BSSB) used for debug through TypeC connector,
  program SBU pins to high-Z/open circuit per USB-C spec.

  @param[in]  UsbTcPortEnBitmap   USB Type C port enabled bitmap

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_UNSUPPORTED         SBU pads are not supported
  @retval EFI_INVALID_PARAMETER   Invalid input parameter
**/
EFI_STATUS
GpioDisableTypeCSbuDebug (
  IN UINT32           UsbTcPortEnBitmap
  );

/**
  When 2-wire DCI OOB is connected via SBU from Type C port, need set IO Standby state to masked (to operate as if no standby signal asserted)
  to remain connection in low power state.

  @param[in] DciPortId            DCI connection port ID

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_UNSUPPORTED         SBU pads are not supported
  @retval EFI_INVALID_PARAMETER   Invalid input parameter
**/
EFI_STATUS
Gpio2WireDciOobSetting (
  IN UINT8                        DciPortId
  );

/**
  This function enables the virtual wire msg bus from GPIO controller
  to FIA. The virtual wire is used to transfer CLKREQ assert/de-assert
  msg for CPU PCIe ports. Each of the PCIe ports has its dedicated VW
  msg.

  @param[in] PortIndex            Index of the CPU PCIe port for which VW
                                  should be enabled.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_UNSUPPORTED         Failed to set native mode.
**/
EFI_STATUS
GpioEnableCpuPcieVwClkReqMsgBus (
  IN UINT32  PortIndex
  );

/**
  This function sets Time Sync Gpio into native mode

  @param[in]  Index       index

  @retval Status
**/
EFI_STATUS
GpioEnableTimeSync (
  IN UINT32  Index
  );

/**
  This function sets Tsn into native mode

  @retval Status
**/
EFI_STATUS
GpioEnableTsn (
  VOID
  );

/**
  This function is to be used In GpioLockPads() to override a lock request by SOC code.

  @param[in]  Group          GPIO group
  @param[in]  DwNum          Register number for current group (parameter applicable in accessing whole register).
                             For group which has less then 32 pads per group DwNum must be 0.
  @param[out] *UnlockCfgPad  DWORD bitmask for pads which are going to be left unlocked
                             Bit position - PadNumber
                             Bit value - 0: to be locked, 1: Leave unlocked
  @param[out] *UnlockTxPad   DWORD bitmask for pads which are going to be left unlocked
                             Bit position - PadNumber
                             Bit value - 0: to be locked, 1: Leave unlocked

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid input parameter
**/
EFI_STATUS
GpioUnlockOverride (
  IN  GPIO_GROUP  Group,
  IN  UINT32      DwNum,
  OUT UINT32      *UnlockCfgPad,
  OUT UINT32      *UnlockTxPad
  );

/**
  Check if 0x13 opcode supported for writing to GPIO lock unlock register

  @retval TRUE                It's supported
  @retval FALSE               It's not supported
**/
BOOLEAN
IsGpioLockOpcodeSupported (
  VOID
  );

/**
  Configures IO standby related settings for the GPIO pad.

  @param[in]  GpioPad             GPIO pad
  @param[in]  IoStandbyState      GPIO pad IO Standby state
  @param[in]  IoStandbyTerm       GPIO pad IO Standby termination

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioConfigurePadIoStandby (
  IN GPIO_PAD              GpioPad,
  IN GPIO_IOSTANDBY_STATE  IoStandbyState,
  IN GPIO_IOSTANDBY_TERM   IoStandbyTerm
  );

/**
  Checks if GPIO PinMux corresponds to I2C4 B

  @param[in] SdaPinMux    GPIO pad pinmux for SDA
  @param[in] SclPinMux    GPIO pad pinmux for SCL

  @retval TRUE         PinMux corresponds to I2C4 B
          FALSE        PinMux equals to I2C4 A
**/
EFI_STATUS
GpioIsSerialIoI2c4bMuxed (
  IN UINT32  SdaPinMux,
  IN UINT32  SclPinMux
  );

#endif // _GPIO_PRIVATE_LIB_H_
