/** @file
*
*  ON Semiconductor FUSB302 USB Type-C port controller.
*
*  Register definitions follow the FUSB302B datasheet and the Linux driver
*  at drivers/usb/typec/tcpm/fusb302_reg.h.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __FUSB302_DXE_H__
#define __FUSB302_DXE_H__

#include <Uefi.h>
#include <Protocol/UsbTypeCPort.h>

#define I2C_GUID \
  { \
  0xadc1901b, 0xb83c, 0x4831, { 0x8f, 0x59, 0x70, 0x89, 0x8f, 0x26, 0x57, 0x1e } \
  }

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
#define FUSB302_REG_INTERRUPT               0x42

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
  USB_TYPE_C_ORIENTATION      Orientation;
} FUSB302_CONTEXT;

#define FUSB302_SC_FROM_TYPEC_PORT(a) \
  CR (a, FUSB302_CONTEXT, TypeCPort, FUSB302_SIGNATURE)

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
