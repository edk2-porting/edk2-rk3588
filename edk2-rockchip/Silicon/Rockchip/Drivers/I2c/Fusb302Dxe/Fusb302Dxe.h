/** @file
*
*  ON Semiconductor FUSB302 USB Type-C port controller.
*
*  Register definitions follow the FUSB302B datasheet and the Linux driver
*  at drivers/usb/typec/tcpm/fusb302_reg.h.
*
*  Copyright (c) 2026 Corey Moyer <cronmod.dev@gmail.com>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __FUSB302_DXE_H__
#define __FUSB302_DXE_H__

#include <Uefi.h>
#include <Protocol/UsbTypeCPort.h>

#include <Protocol/Fusb302PlatformDevice.h>

#define FUSB302_REG_DEVICE_ID               0x01
#define FUSB302_REG_SWITCHES0               0x02
#define   FUSB302_SWITCHES0_CC2_PU_EN       BIT7
#define   FUSB302_SWITCHES0_CC1_PU_EN       BIT6
#define   FUSB302_SWITCHES0_VCONN_CC2       BIT5
#define   FUSB302_SWITCHES0_VCONN_CC1       BIT4
#define   FUSB302_SWITCHES0_MEAS_CC2        BIT3
#define   FUSB302_SWITCHES0_MEAS_CC1        BIT2
#define   FUSB302_SWITCHES0_CC2_PD_EN       BIT1
#define   FUSB302_SWITCHES0_CC1_PD_EN       BIT0
#define FUSB302_REG_SWITCHES1               0x03
#define   FUSB302_SWITCHES1_POWERROLE       BIT7
#define   FUSB302_SWITCHES1_SPECREV_SHIFT   5
#define   FUSB302_SWITCHES1_DATAROLE        BIT4
#define   FUSB302_SWITCHES1_AUTO_GCRC       BIT2
#define   FUSB302_SWITCHES1_TXCC2_EN        BIT1
#define   FUSB302_SWITCHES1_TXCC1_EN        BIT0
#define FUSB302_REG_MEASURE                 0x04
#define   FUSB302_MEASURE_MDAC_MASK         0x3F
#define   FUSB302_MEASURE_VBUS              BIT6
#define FUSB302_REG_SLICE                   0x05
#define FUSB302_REG_CONTROL0                0x06
#define   FUSB302_CONTROL0_TX_FLUSH         BIT6
#define   FUSB302_CONTROL0_INT_MASK         BIT5
#define   FUSB302_CONTROL0_HOST_CUR_MASK    (BIT3 | BIT2)
#define   FUSB302_CONTROL0_HOST_CUR_HIGH    (BIT3 | BIT2)
#define   FUSB302_CONTROL0_HOST_CUR_MED     BIT3
#define   FUSB302_CONTROL0_HOST_CUR_DEF     BIT2
#define   FUSB302_CONTROL0_TX_START         BIT0
#define FUSB302_REG_CONTROL1                0x07
#define   FUSB302_CONTROL1_RX_FLUSH         BIT2
#define FUSB302_REG_CONTROL2                0x08
#define   FUSB302_CONTROL2_TOGGLE           BIT0
#define FUSB302_REG_CONTROL3                0x09
#define   FUSB302_CONTROL3_SEND_HARD_RESET  BIT6
#define   FUSB302_CONTROL3_AUTO_HARDRESET   BIT4
#define   FUSB302_CONTROL3_AUTO_SOFTRESET   BIT3
#define   FUSB302_CONTROL3_N_RETRIES_MASK   (BIT2 | BIT1)
#define   FUSB302_CONTROL3_N_RETRIES_SHIFT  1
#define   FUSB302_CONTROL3_AUTO_RETRY       BIT0
#define FUSB302_REG_MASK                    0x0A
#define FUSB302_REG_POWER                   0x0B
#define   FUSB302_POWER_PWR_BANDGAP         BIT0
#define   FUSB302_POWER_PWR_RECEIVER        BIT1
#define   FUSB302_POWER_PWR_MEASURE         BIT2
#define   FUSB302_POWER_PWR_INT_OSC         BIT3
#define   FUSB302_POWER_PWR_ALL             (BIT0 | BIT1 | BIT2 | BIT3)
#define FUSB302_REG_RESET                   0x0C
#define   FUSB302_RESET_PD_RESET            BIT1
#define   FUSB302_RESET_SW_RESET            BIT0
#define FUSB302_REG_MASKA                   0x0E
#define FUSB302_REG_MASKB                   0x0F
#define FUSB302_REG_STATUS0                 0x40
#define   FUSB302_STATUS0_VBUSOK            BIT7
#define   FUSB302_STATUS0_ACTIVITY          BIT6
#define   FUSB302_STATUS0_COMP              BIT5
#define   FUSB302_STATUS0_BC_LVL_MASK       (BIT1 | BIT0)
#define FUSB302_REG_STATUS1                 0x41
#define   FUSB302_STATUS1_RX_EMPTY          BIT5
#define   FUSB302_STATUS1_RX_FULL           BIT4
#define   FUSB302_STATUS1_TX_EMPTY          BIT3
#define   FUSB302_STATUS1_TX_FULL           BIT2
#define FUSB302_REG_INTERRUPT               0x42
#define   FUSB302_INTERRUPT_I_CRC_CHK       BIT4
#define FUSB302_REG_FIFOS                   0x43

//
// FIFO tokens. A transmitted message is framed as the SOP ordered set, a
// PACKSYM carrying the byte count, the payload, then CRC/EOP/TXOFF and finally
// TXON to key the transmitter.
//
#define FUSB302_TKN_TXON                    0xA1
#define FUSB302_TKN_SYNC1                   0x12
#define FUSB302_TKN_SYNC2                   0x13
#define FUSB302_TKN_SYNC3                   0x1B
#define FUSB302_TKN_RST1                    0x15
#define FUSB302_TKN_RST2                    0x16
#define FUSB302_TKN_PACKSYM                 0x80
#define FUSB302_TKN_JAMCRC                  0xFF
#define FUSB302_TKN_EOP                     0x14
#define FUSB302_TKN_TXOFF                   0xFE

//
// A received frame starts with a token whose top three bits identify the
// ordered set; only SOP is of interest here.
//
#define FUSB302_RX_TOKEN_MASK               0xE0
#define FUSB302_RX_TOKEN_SOP                0xE0

//
// USB Power Delivery message header, as defined by the specification.
//
#define PD_HEADER_TYPE(Header)              ((Header) & 0x1F)
#define PD_HEADER_ID(Header)                (((Header) >> 9) & 0x7)
#define PD_HEADER_COUNT(Header)             (((Header) >> 12) & 0x7)
#define PD_HEADER_EXTENDED(Header)          (((Header) >> 15) & 0x1)
#define PD_HEADER_REV(Header)               (((Header) >> 6) & 0x3)
#define PD_HEADER_DATA_ROLE(Header)         (((Header) >> 5) & 0x1)
#define PD_HEADER_POWER_ROLE(Header)        (((Header) >> 8) & 0x1)

#define PD_HEADER_BUILD(Type, Id, Count, DataRole, PowerRole, Revision)  \
  ((UINT16)(((Type) & 0x1F)             |                                \
            (((DataRole) & 0x1) << 5)   |                                \
            (((Revision) & 0x3) << 6)   |                                \
            (((PowerRole) & 0x1) << 8)  |                                \
            (((Id) & 0x7) << 9)         |                                \
            (((Count) & 0x7) << 12)))

#define PD_REV_2_0                          1

//
// Control message types.
//
#define PD_CTRL_GOOD_CRC                    1
#define PD_CTRL_GOTO_MIN                    2
#define PD_CTRL_ACCEPT                      3
#define PD_CTRL_REJECT                      4
#define PD_CTRL_PING                        5
#define PD_CTRL_PS_RDY                      6
#define PD_CTRL_GET_SOURCE_CAP              7
#define PD_CTRL_GET_SINK_CAP                8
#define PD_CTRL_DR_SWAP                     9
#define PD_CTRL_PR_SWAP                     10
#define PD_CTRL_VCONN_SWAP                  11
#define PD_CTRL_WAIT                        12
#define PD_CTRL_SOFT_RESET                  13

//
// Data message types.
//
#define PD_DATA_SOURCE_CAP                  1
#define PD_DATA_REQUEST                     2
#define PD_DATA_SINK_CAP                    4

//
// Power data objects. Only fixed supplies are ever requested from a source;
// a variable supply is understood on the sink side alone, where it is how a
// board says it will take anything within a range.
//
#define PD_PDO_TYPE(Pdo)                    (((Pdo) >> 30) & 0x3)
#define PD_PDO_TYPE_FIXED                   0
#define PD_PDO_TYPE_VARIABLE                2
#define PD_PDO_FIXED_VOLTAGE_MV(Pdo)        ((((Pdo) >> 10) & 0x3FF) * 50)
#define PD_PDO_FIXED_CURRENT_MA(Pdo)        (((Pdo) & 0x3FF) * 10)
#define PD_PDO_VAR_MAX_VOLTAGE_MV(Pdo)      ((((Pdo) >> 20) & 0x3FF) * 50)
#define PD_PDO_VAR_MIN_VOLTAGE_MV(Pdo)      ((((Pdo) >> 10) & 0x3FF) * 50)
#define PD_PDO_VAR_MAX_CURRENT_MA(Pdo)      (((Pdo) & 0x3FF) * 10)

//
// Fixed request data object.
//
#define PD_RDO_FIXED(ObjectPosition, OperatingMa, MaxMa)  \
  ((UINT32)((((ObjectPosition) & 0x7) << 28)            | \
            (((((OperatingMa) / 10)) & 0x3FF) << 10)    | \
            (((MaxMa) / 10) & 0x3FF)                    | \
            BIT25 /* USB communications capable */ ))

#define PD_MAX_DATA_OBJECTS                 7

//
// A source sends Source_Capabilities every ~150 ms until a contract exists, so
// this catches several attempts while staying short enough not to stall the
// boot by much when the partner does not speak PD at all.
//
#define PD_SOURCE_CAP_TIMEOUT_US            (600 * 1000)
//
// Once a Soft_Reset has been accepted the partner is known to speak PD, so it
// is worth waiting longer for the advertisement that follows.
//
#define PD_SOURCE_CAP_RETRY_TIMEOUT_US      (1200 * 1000)
//
// tSenderResponse is 30 ms; allow a little more before giving up.
//
#define PD_SENDER_RESPONSE_TIMEOUT_US       (60 * 1000)
//
// tPSTransition is 550 ms at worst.
//
#define PD_PS_TRANSITION_TIMEOUT_US         (600 * 1000)

#define PD_POLL_INTERVAL_US                 500

//
// Vendor defined messages. Only structured VDMs are used here.
//
#define PD_DATA_VENDOR_DEFINED              15

#define PD_VDM_SVID_PD                      0xFF00
#define PD_VDM_SVID_DISPLAYPORT             0xFF01

#define PD_VDM_HEADER(Svid, ObjectPosition, CommandType, Command)  \
  ((UINT32)(((UINT32)(Svid) << 16)        |                        \
            BIT15 /* structured */        |                        \
            (((ObjectPosition) & 0x7) << 8) |                      \
            (((CommandType) & 0x3) << 6)  |                        \
            ((Command) & 0x1F)))

#define PD_VDM_CMD_TYPE(Vdm)                (((Vdm) >> 6) & 0x3)
#define PD_VDM_CMD(Vdm)                     ((Vdm) & 0x1F)
#define PD_VDM_SVID(Vdm)                    (((Vdm) >> 16) & 0xFFFF)
#define PD_VDM_IS_STRUCTURED(Vdm)           (((Vdm) >> 15) & 0x1)

#define PD_VDM_TYPE_REQ                     0
#define PD_VDM_TYPE_ACK                     1
#define PD_VDM_TYPE_NAK                     2
#define PD_VDM_TYPE_BUSY                    3

#define PD_VDM_DISCOVER_IDENTITY            1
#define PD_VDM_DISCOVER_SVIDS               2
#define PD_VDM_DISCOVER_MODES               3
#define PD_VDM_ENTER_MODE                   4
#define PD_VDM_EXIT_MODE                    5
#define PD_VDM_ATTENTION                    6
#define PD_VDM_DP_STATUS                    16
#define PD_VDM_DP_CONFIGURE                 17

//
// The identity header says whether the partner supports any alternate mode at
// all, which saves asking further questions of one that does not.
//
#define PD_ID_HEADER_MODAL_OPERATION(Vdo)   (((Vdo) >> 26) & 0x1)

//
// DisplayPort capability VDO, returned by Discover Modes for the DisplayPort
// SVID. Note the pin assignment fields are named for the role the *partner*
// plays: a display sink fills in the UFP_D field.
//
#define DP_CAP_PORT_CAPABILITY(Vdo)         ((Vdo) & 0x3)
#define   DP_CAP_PORT_UFP_D                 1
#define   DP_CAP_PORT_DFP_D                 2
#define   DP_CAP_PORT_BOTH                  3
#define DP_CAP_PIN_ASSIGN_UFP_D(Vdo)        (((Vdo) >> 8) & 0xFF)
#define DP_CAP_PIN_ASSIGN_DFP_D(Vdo)        (((Vdo) >> 16) & 0xFF)

//
// DisplayPort status VDO, returned by the DisplayPort Status command.
//
#define DP_STATUS_HPD_STATE(Vdo)            (((Vdo) >> 7) & 0x1)
#define DP_STATUS_HPD_IRQ(Vdo)              (((Vdo) >> 8) & 0x1)

//
// DisplayPort configure VDO.
//
//
// Select Configuration says which role the partner is to take, so a display
// has to be told to be the UFP_D. Telling it to be the DFP_D instead gets the
// configuration refused, since a display cannot source DisplayPort.
//
#define DP_CONFIG_SELECT_UFP_U_AS_DFP_D     1
#define DP_CONFIG_SELECT_UFP_U_AS_UFP_D     2
#define DP_CONFIG_SIGNALLING_DP             1
#define DP_CONFIG_BUILD(PinAssignment)          \
  ((UINT32)(DP_CONFIG_SELECT_UFP_U_AS_UFP_D |     \
            (DP_CONFIG_SIGNALLING_DP << 2)  |     \
            ((UINT32)(PinAssignment) << 8)))

//
// tVDMSenderResponse is 30 ms at most; discovery is otherwise not time
// critical.
//
#define PD_VDM_RESPONSE_TIMEOUT_US          (60 * 1000)

//
// A partner may answer BUSY while it gets itself ready. The specification
// allows retrying, so give it a few goes before concluding it will not
// answer at all.
//
#define PD_VDM_BUSY_RETRIES                 3
#define PD_VDM_BUSY_DELAY_US                (5 * 1000)

//
// Source role. A sink must answer an advertisement within tSenderResponse, and
// the specification has the source repeat Source_Capabilities every 100-200 ms
// until it does.
//
#define PD_SOURCE_CAP_INTERVAL_US           (150 * 1000)
#define PD_SOURCE_ATTACH_TIMEOUT_US         (1500 * 1000)
//
// Time allowed for VBUS to come up once the supply is switched on.
//
#define PD_VBUS_ON_TIMEOUT_US               (200 * 1000)

//
// After a Hard Reset the source takes VBUS to zero and brings it back. The
// specification allows tSafe0V plus tSrcRecover for that, which together come
// to around a second; allow half again on top.
//
#define PD_HARD_RESET_RECOVER_US            (1500 * 1000)

//
// A source may advertise again straight after a contract. Answer a few of
// those before deciding it will never settle, so a partner stuck repeating
// itself cannot hold up the rest of the boot.
//
#define PD_SOURCE_CAP_REPEAT_US             (250 * 1000)
#define PD_SOURCE_CAP_REPEAT_LIMIT          4

//
// A display behind an adapter is not necessarily awake when alternate mode is
// entered, and hot-plug detect asserts once it is. Give it a moment rather
// than reporting the port empty on the strength of a single early read.
//
#define PD_DP_HPD_TIMEOUT_US                (2500 * 1000)
#define PD_DP_HPD_POLL_US                   (250 * 1000)

#define PD_RDO_OBJECT_POSITION(Rdo)         (((Rdo) >> 28) & 0x7)


//
// Expected value of the version field in DEVICE_ID for the parts this driver
// has been written against (FUSB302B revisions report 0x8 or 0x9).
//
#define FUSB302_DEVICE_ID_VERSION_SHIFT     4

//
// BC_LVL thresholds, in the encoding STATUS0 reports them.
//
#define FUSB302_BC_LVL_RA                   0
#define FUSB302_BC_LVL_USB_DEFAULT          1
#define FUSB302_BC_LVL_1P5A                 2
#define FUSB302_BC_LVL_3A                   3

//
// Settling time after switching the measure block between CC pins. The
// datasheet asks for 250 us; allow a wide margin since we poll rather than
// take the interrupt.
//
#define FUSB302_MEASURE_SETTLE_US           350

typedef struct {
  UINT16    Header;
  UINT32    Objects[PD_MAX_DATA_OBJECTS];
} PD_MESSAGE;

typedef enum {
  Fusb302RoleSink,
  Fusb302RoleSource
} FUSB302_PROBE_ROLE;

#define FUSB302_SIGNATURE  SIGNATURE_32 ('F', 'U', 'S', 'B')

typedef struct {
  UINT32                      Signature;
  EFI_HANDLE                  Handle;
  EFI_I2C_IO_PROTOCOL         *I2cIo;
  USB_TYPE_C_PORT_PROTOCOL    TypeCPort;
  //
  // What the board says about this port: where it sits, what it may
  // source, what it will sink, and how to switch its supply. This is the
  // whole of what the driver knows about the platform it runs on.
  //
  FUSB302_PLATFORM_DEVICE_PROTOCOL    *Platform;
  USB_TYPE_C_ORIENTATION      Orientation;
  USB_TYPE_C_POWER_CONTRACT   Contract;
  //
  // TRUE when the partner supplies power to us, which is the only case
  // where there is a contract to negotiate.
  //
  BOOLEAN                     PartnerIsSource;
  USB_TYPE_C_DP_ALT_MODE      DpAltMode;
  //
  // TRUE once we are the downstream facing port for data. A sink starts
  // out upstream facing and has to swap to drive alternate modes. Every
  // message header must agree with this and with SWITCHES1, or the
  // partner ignores it.
  //
  BOOLEAN                     DataRoleDfp;
  //
  // Rolling message ID for messages this driver sends, per the specification.
  //
  UINT8                       MessageId;
} FUSB302_CONTEXT;

#define FUSB302_SC_FROM_TYPEC_PORT(a) \
  CR (a, FUSB302_CONTEXT, TypeCPort, FUSB302_SIGNATURE)

//
// The power role bit every header this driver sends has to carry. A partner
// that supplies us makes this board the sink; anything else and we are the
// one supplying. A header disagreeing with the role the partner has us in
// gets the message ignored.
//
#define FUSB302_POWER_ROLE(Context)  ((Context)->PartnerIsSource ? 0 : 1)

/**
  How many of a FUSB302_MAX_PDOS list the board actually filled in. A zero
  object is the absence of one, so the first zero ends the list.

**/
UINTN
Fusb302PdoCount (
  IN CONST UINT32  *Pdos
  );

EFI_STATUS
Fusb302RegRead (
  IN  FUSB302_CONTEXT  *Context,
  IN  UINT8            Register,
  OUT UINT8            *Value
  );

EFI_STATUS
Fusb302RegWrite (
  IN FUSB302_CONTEXT  *Context,
  IN UINT8            Register,
  IN UINT8            Value
  );

EFI_STATUS
Fusb302RegUpdate (
  IN FUSB302_CONTEXT  *Context,
  IN UINT8            Register,
  IN UINT8            Mask,
  IN UINT8            Value
  );

/**
  Negotiate a USB Power Delivery contract as a sink.

  @param[in,out] Context      Driver context; Contract is updated on success.
  @param[in]     Orientation  Which CC pin the partner is on.

**/
EFI_STATUS
Fusb302PdSend (
  IN FUSB302_CONTEXT  *Context,
  IN UINT16           Header,
  IN CONST UINT32     *Objects
  );

EFI_STATUS
Fusb302PdWaitFor (
  IN OUT FUSB302_CONTEXT  *Context,
  IN     BOOLEAN          WantData,
  IN     UINT8            Type,
  IN     UINTN            TimeoutUs,
  OUT    PD_MESSAGE       *Message
  );

EFI_STATUS
Fusb302PdNegotiateSink (
  IN OUT FUSB302_CONTEXT         *Context,
  IN     USB_TYPE_C_ORIENTATION  Orientation
  );

/**
  Discover and enter DisplayPort Alternate Mode, and read back whether a
  display is present.

  Requires a PD link to already be up, which Fusb302PdNegotiateSink() leaves
  running on success.

**/
/**
  Act as a power source for an attached sink.

  Only does anything where the board has opted in and can actually supply
  power. Refuses to switch VBUS on if something else is already driving it.

**/
EFI_STATUS
Fusb302PdSourceRun (
  IN OUT FUSB302_CONTEXT         *Context,
  IN     USB_TYPE_C_ORIENTATION  Orientation
  );

/**
  Present a source termination and switch VBUS on with nothing attached.

  A board that can source power is expected to have a live port whether or not
  anything was plugged in when firmware started, which is what an always on
  supply gave before this driver took the rail over. Rp alone advertises
  Type-C default current, so a device attached later is still powered
  correctly even though no negotiation ever runs for it.

  @retval EFI_SUCCESS       The port was left live.
  @retval EFI_UNSUPPORTED   The board does not source power.

**/
/**
  The fixed supply this port offers a sink, or 0 where the board lists none.

  Only fixed supplies can be advertised: a sink asks for an object by position
  and expects the voltage it named, which a variable object does not pin down.

**/
UINT32
Fusb302SourcePdo (
  IN CONST FUSB302_CONTEXT  *Context
  );

/**
  Switch this port's USB-C supply on or off.

  Boards whose supply defaults to on at reset have to be able to take it down
  again: driving VBUS while a source is attached puts two supplies onto one
  rail, and a source will not negotiate into that.

  @retval EFI_SUCCESS       The supply was switched.
  @retval EFI_UNSUPPORTED   The board gave this port no way to switch it.

**/
EFI_STATUS
Fusb302SourceSetVbus (
  IN FUSB302_CONTEXT  *Context,
  IN BOOLEAN          Enable
  );

EFI_STATUS
Fusb302SourceIdle (
  IN OUT FUSB302_CONTEXT  *Context
  );

/**
  Send a USB Power Delivery Hard Reset and wait for the port to come back.

  The last resort when a source will not talk to us. Both ends drop their
  protocol state and the source removes VBUS and restores it, after which it
  advertises from scratch. That is the only way back when a source is still
  holding a contract agreed before this firmware started and does not answer
  Soft_Reset.

  Because VBUS really does go away for a moment, a board drawing all its power
  from this port will brown out. That is inherent to Hard Reset rather than
  anything this driver can avoid, and it is what the specification requires a
  sink to do when capabilities never arrive.

  @retval EFI_SUCCESS       The reset was sent and VBUS returned.
  @retval EFI_TIMEOUT       VBUS did not come back.

**/
EFI_STATUS
Fusb302PdHardReset (
  IN OUT FUSB302_CONTEXT  *Context
  );

EFI_STATUS
Fusb302PdDataRoleSwapToDfp (
  IN OUT FUSB302_CONTEXT  *Context
  );

EFI_STATUS
Fusb302DpAltModeEnter (
  IN OUT FUSB302_CONTEXT  *Context
  );

EFI_STATUS
EFIAPI
Fusb302Supported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  );

EFI_STATUS
EFIAPI
Fusb302Start (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  );

EFI_STATUS
EFIAPI
Fusb302Stop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN UINTN                        NumberOfChildren,
  IN EFI_HANDLE                   *ChildHandleBuffer
  );

#endif // __FUSB302_DXE_H__
