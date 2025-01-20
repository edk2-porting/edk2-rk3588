/** @file
  Rockchip HDMI Driver.

  Copyright (c) 2022 Rockchip Electronics Co. Ltd.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DwHdmiQpLib.h>
#include <Library/PWMLib.h>
#include <Library/DrmModes.h>
#include <Library/RockchipPlatformLib.h>
#include <Library/MediaBusFormat.h>
#include <Library/DrmModes.h>
#include <Library/GpioLib.h>

#include <Protocol/RockchipConnectorProtocol.h>

#include <Uefi/UefiBaseType.h>

#include <Library/uboot-env.h>

#define HIWORD_UPDATE(val, mask)  (val | (mask) << 16)

#define RK3588_GRF_SOC_CON2             0x0308
#define RK3588_HDMI1_HPD_INT_MSK        BIT(15)
#define RK3588_HDMI1_HPD_INT_CLR        BIT(14)
#define RK3588_HDMI0_HPD_INT_MSK        BIT(13)
#define RK3588_HDMI0_HPD_INT_CLR        BIT(12)
#define RK3588_GRF_SOC_CON7             0x031c
#define RK3588_SET_HPD_PATH_MASK        (0x3 << 12)
#define RK3588_GRF_SOC_STATUS1          0x0384
#define RK3588_HDMI0_LOW_MORETHAN100MS  BIT(20)
#define RK3588_HDMI0_HPD_PORT_LEVEL     BIT(19)
#define RK3588_HDMI0_IHPD_PORT          BIT(18)
#define RK3588_HDMI0_OHPD_INT           BIT(17)
#define RK3588_HDMI0_LEVEL_INT          BIT(16)
#define RK3588_HDMI0_INTR_CHANGE_CNT    (0x7 << 13)
#define RK3588_HDMI1_LOW_MORETHAN100MS  BIT(28)
#define RK3588_HDMI1_HPD_PORT_LEVEL     BIT(27)
#define RK3588_HDMI1_IHPD_PORT          BIT(26)
#define RK3588_HDMI1_OHPD_INT           BIT(25)
#define RK3588_HDMI1_LEVEL_INT          BIT(24)
#define RK3588_HDMI1_INTR_CHANGE_CNT    (0x7 << 21)

#define RK3588_GRF_VO1_CON3        0x000c
#define RK3588_COLOR_FORMAT_MASK   0xf
#define RK3588_YUV444              0x2
#define RK3588_YUV420              0x3
#define RK3588_COMPRESSED_DATA     0xb
#define RK3588_COLOR_DEPTH_MASK    (0xf << 4)
#define RK3588_8BPC                0
#define RK3588_10BPC               (0x6 << 4)
#define RK3588_CECIN_MASK          BIT(8)
#define RK3588_SCLIN_MASK          BIT(9)
#define RK3588_SDAIN_MASK          BIT(10)
#define RK3588_MODE_MASK           BIT(11)
#define RK3588_COMPRESS_MODE_MASK  BIT(12)
#define RK3588_I2S_SEL_MASK        BIT(13)
#define RK3588_SPDIF_SEL_MASK      BIT(14)
#define RK3588_GRF_VO1_CON4        0x0010
#define RK3588_HDMI21_MASK         BIT(0)
#define RK3588_GRF_VO1_CON9        0x0024
#define RK3588_HDMI0_GRANT_SEL     BIT(10)
#define RK3588_HDMI0_GRANT_SW      BIT(11)
#define RK3588_HDMI1_GRANT_SEL     BIT(12)
#define RK3588_HDMI1_GRANT_SW      BIT(13)
#define RK3588_GRF_VO1_CON6        0x0018
#define RK3588_GRF_VO1_CON7        0x001c

#define PPS_TABLE_LEN  8

#define COLOR_DEPTH_10BIT  BIT(31)
#define HDMI_FRL_MODE      BIT(30)
#define HDMI_EARC_MODE     BIT(29)
#define DATA_RATE_MASK     0xFFFFFFF

#define HDMI20_MAX_RATE  600000
#define HDMI_8K60_RATE   2376000

#define DDC_CI_ADDR       0x37
#define DDC_SEGMENT_ADDR  0x30

#define HDMI_EDID_LEN        512
#define HDMI_EDID_BLOCK_LEN  128

#define DDC_ADDR  0x50

VOID
DwHdmiQpRegWrite (
  OUT struct  DwHdmiQpDevice  *Hdmi,
  IN  UINT32                  Value,
  IN  UINT32                  Offset
  )
{
  UINT32  BASE;

  if (!Hdmi->Id) {
    BASE = HDMI0_BASE;
  } else {
    BASE = HDMI1_BASE;
  }

  MmioWrite32 (BASE + Offset, Value);
}

UINT32
DwHdmiQpRegRead (
  OUT struct  DwHdmiQpDevice  *Hdmi,
  IN  UINT32                  Offset
  )
{
  UINT32  Value;
  UINT32  BASE;

  if (!Hdmi->Id) {
    BASE = HDMI0_BASE;
  } else {
    BASE = HDMI1_BASE;
  }

  Value = MmioRead32 (BASE + Offset);
  return Value;
}

VOID
DwHdmiQpRegMod (
  OUT struct  DwHdmiQpDevice  *Hdmi,
  IN  UINT32                  Value,
  IN  UINT32                  Mask,
  IN  UINT32                  Offset
  )
{
  UINT32  Val;
  UINT32  BASE;

  if (!Hdmi->Id) {
    BASE = HDMI0_BASE;
  } else {
    BASE = HDMI1_BASE;
  }

  Val  = MmioRead32 (BASE + Offset);
  Val &= ~Mask;
  Val |= Value;

  MmioWrite32 (BASE + Offset, Val);
}

VOID
DwHdmiQpSetIomux (
  OUT struct  DwHdmiQpDevice  *Hdmi
  )
{
  UINT32  Val;

  if (!Hdmi->Id) {
    Val = HIWORD_UPDATE (RK3588_SCLIN_MASK, RK3588_SCLIN_MASK) |
          HIWORD_UPDATE (RK3588_SDAIN_MASK, RK3588_SDAIN_MASK) |
          HIWORD_UPDATE (RK3588_MODE_MASK, RK3588_MODE_MASK) |
          HIWORD_UPDATE (RK3588_I2S_SEL_MASK, RK3588_I2S_SEL_MASK);
    MmioWrite32 (0xFD5A8000 + RK3588_GRF_VO1_CON3, Val);

    Val = HIWORD_UPDATE (
            RK3588_SET_HPD_PATH_MASK,
            RK3588_SET_HPD_PATH_MASK
            );
    MmioWrite32 (0xFD58C000 + RK3588_GRF_SOC_CON7, Val);

    Val = HIWORD_UPDATE (
            RK3588_HDMI0_GRANT_SEL,
            RK3588_HDMI0_GRANT_SEL
            );
    MmioWrite32 (0xFD5A8000 + RK3588_GRF_VO1_CON9, Val);
  } else {
    Val = HIWORD_UPDATE (RK3588_SCLIN_MASK, RK3588_SCLIN_MASK) |
          HIWORD_UPDATE (RK3588_SDAIN_MASK, RK3588_SDAIN_MASK) |
          HIWORD_UPDATE (RK3588_MODE_MASK, RK3588_MODE_MASK) |
          HIWORD_UPDATE (RK3588_I2S_SEL_MASK, RK3588_I2S_SEL_MASK);
    MmioWrite32 (0xFD5A8000 + RK3588_GRF_VO1_CON6, Val);

    Val = HIWORD_UPDATE (
            RK3588_SET_HPD_PATH_MASK,
            RK3588_SET_HPD_PATH_MASK
            );
    MmioWrite32 (0xFD58C000 + RK3588_GRF_SOC_CON7, Val);

    Val = HIWORD_UPDATE (
            RK3588_HDMI1_GRANT_SEL,
            RK3588_HDMI1_GRANT_SEL
            );
    MmioWrite32 (0xFD5A8000 + RK3588_GRF_VO1_CON9, Val);
  }
}

STATIC
BOOLEAN
DwHdmiI2cPollForIrq (
  OUT struct DwHdmiQpDevice  *Hdmi
  )
{
  struct DwHdmiQpI2c  *I2c = &Hdmi->I2c;
  UINT32              Stat;

  Stat      = DwHdmiQpRegRead (Hdmi, MAINUNIT_1_INT_STATUS);
  I2c->Stat = Stat & (I2CM_OP_DONE_IRQ | I2CM_READ_REQUEST_IRQ |
                      I2CM_NACK_RCVD_IRQ);
  Hdmi->ScdcIntr = Stat& (SCDC_UPD_FLAGS_RD_IRQ |
                          SCDC_UPD_FLAGS_CHG_IRQ |
                          SCDC_UPD_FLAGS_CLR_IRQ |
                          SCDC_RR_REPLY_STOP_IRQ |
                          SCDC_NACK_RCVD_IRQ);

  Hdmi->FltIntr = Stat & (FLT_EXIT_TO_LTSP_IRQ |
                          FLT_EXIT_TO_LTS4_IRQ |
                          FLT_EXIT_TO_LTSL_IRQ);

  DEBUG ((DEBUG_VERBOSE, "i2c main unit irq:%02x\n", Stat));
  if (I2c->Stat) {
    DwHdmiQpRegWrite (Hdmi, I2c->Stat, MAINUNIT_1_INT_CLEAR);
    I2c->Cmp = TRUE;
  }

  if (Hdmi->FltIntr) {
    DEBUG ((DEBUG_VERBOSE, "i2c flt irq:%02x\n", Hdmi->FltIntr));
    DwHdmiQpRegWrite (Hdmi, Hdmi->FltIntr, MAINUNIT_1_INT_CLEAR);
    Hdmi->FltCmp = TRUE;
  }

  if (Hdmi->ScdcIntr) {
    UINT8  val;

    DEBUG ((DEBUG_VERBOSE, "i2c scdc irq:%02x\n", Hdmi->ScdcIntr));
    DwHdmiQpRegWrite (Hdmi, Hdmi->ScdcIntr, MAINUNIT_1_INT_CLEAR);
    val = DwHdmiQpRegRead (Hdmi, SCDC_STATUS0);

    /* frl start */
    if (val & BIT (4)) {
      DwHdmiQpRegMod (
        Hdmi,
        0,
        SCDC_UPD_FLAGS_POLL_EN |
        SCDC_UPD_FLAGS_AUTO_CLR,
        SCDC_CONFIG0
        );
      DwHdmiQpRegMod (
        Hdmi,
        0,
        SCDC_UPD_FLAGS_RD_IRQ,
        MAINUNIT_1_INT_MASK_N
        );
      DEBUG ((DEBUG_VERBOSE, "frl start\n"));
    }
  }

  if (Stat) {
    return TRUE;
  }

  return FALSE;
}

STATIC
EFI_STATUS
DwHdmiI2cRead (
  IN struct DwHdmiQpDevice  *Hdmi,
  UINT8                     *Buf,
  UINTN                     Length
  )
{
  EFI_STATUS          Status = EFI_SUCCESS;
  struct DwHdmiQpI2c  *I2c   = &Hdmi->I2c;

  if (!I2c->IsRegAddr) {
    DEBUG ((DEBUG_INFO, "Set read register address to 0\n"));
    I2c->SlaveReg  = 0x0;
    I2c->IsRegAddr = TRUE;
  }

  while (Length--) {
    I2c->Cmp = FALSE;
    DwHdmiQpRegMod (Hdmi, I2c->SlaveReg++ << 12, I2CM_ADDR, I2CM_INTERFACE_CONTROL0);
    if (I2c->IsSegment) {
      DwHdmiQpRegMod (
        Hdmi,
        I2CM_EXT_READ,
        I2CM_WR_MASK,
        I2CM_INTERFACE_CONTROL0
        );
    } else {
      DwHdmiQpRegMod (
        Hdmi,
        I2CM_FM_READ,
        I2CM_WR_MASK,
        I2CM_INTERFACE_CONTROL0
        );
    }

    // Wait for transfer done here
    int      timeout  = 10 * 1000;
    int      interval = 100;
    BOOLEAN  ret;
    while (timeout) {
      ret = DwHdmiI2cPollForIrq (Hdmi);
      if (I2c->Cmp) {
        break;
      }

      timeout -= interval;
      MicroSecondDelay (interval);
    }

    if (!timeout && !ret) {
      DEBUG ((DEBUG_ERROR, "HDMI I2C read time out!\n"));
      DwHdmiQpRegWrite (Hdmi, 0x01, I2CM_CONTROL0);
      Status = EFI_TIMEOUT;
      goto exit;
    }

    if (I2c->Stat & I2CM_NACK_RCVD_IRQ) {
      DEBUG ((DEBUG_ERROR, "HDMI I2C read error\n"));
      DwHdmiQpRegWrite (Hdmi, 0x01, I2CM_CONTROL0);
      Status = EFI_DEVICE_ERROR;
      goto exit;
    }

    MicroSecondDelay (500);

    *Buf = DwHdmiQpRegRead (Hdmi, I2CM_INTERFACE_RDDATA_0_3) & 0xff;
    Buf++;
    DEBUG ((
      DEBUG_VERBOSE,
      "i2c read succeed I2c->Stat = %02x 0x%02x RegAddr=%02x\n",
      I2c->Stat,
      DwHdmiQpRegRead (Hdmi, I2CM_INTERFACE_RDDATA_0_3) & 0xff,
      I2c->SlaveReg-1
      ));

    DwHdmiQpRegMod (Hdmi, 0, I2CM_WR_MASK, I2CM_INTERFACE_CONTROL0);
  }

  I2c->IsSegment = FALSE;

exit:
  return Status;
}

STATIC
EFI_STATUS
DwHdmiI2cWrite (
  IN struct DwHdmiQpDevice  *Hdmi,
  UINT8                     *Buf,
  UINTN                     Length
  )
{
  EFI_STATUS          Status = EFI_SUCCESS;
  struct DwHdmiQpI2c  *I2c   = &Hdmi->I2c;

  if (!I2c->IsRegAddr) {
    I2c->SlaveReg = Buf[0];
    Length--;
    Buf++;
    I2c->IsRegAddr = TRUE;
  }

  while (Length--) {
    I2c->Cmp = FALSE;
    DwHdmiQpRegWrite (Hdmi, *Buf, I2CM_INTERFACE_WRDATA_0_3);
    Buf++;
    DwHdmiQpRegMod (Hdmi, I2c->SlaveReg++ << 12, I2CM_ADDR, I2CM_INTERFACE_CONTROL0);
    DwHdmiQpRegMod (
      Hdmi,
      I2CM_FM_WRITE,
      I2CM_WR_MASK,
      I2CM_INTERFACE_CONTROL0
      );

    // Wait for transfer done here
    int      timeout  = 10 * 1000;
    int      interval = 100;
    BOOLEAN  ret;
    while (timeout) {
      ret = DwHdmiI2cPollForIrq (Hdmi);
      if (I2c->Cmp) {
        break;
      }

      timeout -= interval;
      MicroSecondDelay (interval);
    }

    if (!timeout && !ret) {
      DEBUG ((DEBUG_ERROR, "HDMI I2C write time out!\n"));
      DwHdmiQpRegWrite (Hdmi, 0x01, I2CM_CONTROL0);
      Status = EFI_TIMEOUT;
      goto exit;
    }

    /* Check for error condition on the bus */
    if (I2c->Stat & I2CM_NACK_RCVD_IRQ) {
      DEBUG ((DEBUG_ERROR, "HDMI I2C write nack!\n"));
      DwHdmiQpRegWrite (Hdmi, 0x01, I2CM_CONTROL0);
      Status = EFI_DEVICE_ERROR;
      goto exit;
    }

    DwHdmiQpRegMod (Hdmi, 0, I2CM_WR_MASK, I2CM_INTERFACE_CONTROL0);
  }

  DEBUG ((DEBUG_VERBOSE, "HDMI I2C write done! I2c->Stat = %02x\n", I2c->Stat));

exit:
  return Status;
}

STATIC
EFI_STATUS
DwHdmiQpI2cXfer (
  IN struct DwHdmiQpDevice  *Hdmi,
  IN struct i2c_msg         *Msgs,
  IN INTN                   Num
  )
{
  EFI_STATUS          Status = EFI_SUCCESS;
  struct DwHdmiQpI2c  *I2c   = &Hdmi->I2c;
  UINT8               Addr   = Msgs[0].addr;

  if (Addr == DDC_CI_ADDR) {
    /*
                 * The internal I2C controller does not support the multi-byte
                 * read and write operations needed for DDC/CI.
                 * TOFIX: Blacklist the DDC/CI address until we filter out
                 * unsupported I2C operations.
                 */
    return EFI_UNSUPPORTED;
  }

  DEBUG ((
    DEBUG_VERBOSE,
    "HDMI I2C xfer: Num: %d, Addr: %02x\n",
    Num,
    Addr
    ));

  for (int i = 0; i < Num; i++) {
    if (Msgs[i].len == 0) {
      DEBUG ((
        DEBUG_ERROR,
        "Unsupported transfer %d/%d, no data\n",
        i + 1,
        Num
        ));
      return EFI_UNSUPPORTED;
    }
  }

  /* Unmute DONE and ERROR interrupts */
  DwHdmiQpRegMod (
    Hdmi,
    I2CM_NACK_RCVD_MASK_N | I2CM_OP_DONE_MASK_N,
    I2CM_NACK_RCVD_MASK_N | I2CM_OP_DONE_MASK_N,
    MAINUNIT_1_INT_MASK_N
    );

  if ((Addr == DDC_SEGMENT_ADDR) && (Msgs[0].len == 1)) {
    Addr = DDC_ADDR;
  }

  DwHdmiQpRegMod (Hdmi, Addr << 5, I2CM_SLVADDR, I2CM_INTERFACE_CONTROL0);

  /* Set slave device register address on transfer */
  I2c->IsRegAddr = FALSE;

  /* Set segment pointer for I2C extended read mode operation */
  I2c->IsSegment = FALSE;

  for (int i = 0; i < Num; i++) {
    DEBUG ((
      DEBUG_VERBOSE,
      "xfer: num: %d/%d, len: %d, flags: %x\n",
      i + 1,
      Num,
      Msgs[i].len,
      Msgs[i].flags
      ));

    if ((Msgs[i].addr == DDC_SEGMENT_ADDR) && (Msgs[i].len == 1)) {
      I2c->IsSegment = TRUE;
      DwHdmiQpRegMod (
        Hdmi,
        DDC_SEGMENT_ADDR,
        I2CM_SEG_ADDR,
        I2CM_INTERFACE_CONTROL1
        );
      DwHdmiQpRegMod (
        Hdmi,
        *Msgs[i].buf << 7,
        I2CM_SEG_PTR,
        I2CM_INTERFACE_CONTROL1
        );
    } else {
      if (Msgs[i].flags & I2C_M_RD) {
        Status = DwHdmiI2cRead (Hdmi, Msgs[i].buf, Msgs[i].len);
      } else {
        Status = DwHdmiI2cWrite (Hdmi, Msgs[i].buf, Msgs[i].len);
      }
    }

    if (Status) {
      break;
    }
  }

  if (Status) {
    return Status;
  }

  /* Mute DONE and ERROR interrupts */
  DwHdmiQpRegMod (
    Hdmi,
    0,
    I2CM_OP_DONE_MASK_N | I2CM_NACK_RCVD_MASK_N,
    MAINUNIT_1_INT_MASK_N
    );

  return Status;
}

STATIC
VOID
DwHdmiI2cInit (
  OUT struct DwHdmiQpDevice  *Hdmi
  )
{
  UINT32  BaseAddr;

  if (Hdmi->Id) {
    BaseAddr = HDMI1_BASE;
  } else {
    BaseAddr = HDMI0_BASE;
  }

  /* Software reset */
  DwHdmiQpRegWrite (Hdmi, 0x01, I2CM_CONTROL0);

  DwHdmiQpRegMod (Hdmi, 0, I2CM_FM_EN, I2CM_INTERFACE_CONTROL0);

  /* Clear DONE and ERROR interrupts */
  DwHdmiQpRegWrite (
    Hdmi,
    I2CM_OP_DONE_CLEAR | I2CM_NACK_RCVD_CLEAR,
    MAINUNIT_1_INT_CLEAR
    );
}

STATIC
VOID
DumpEdid (
  IN struct DwHdmiQpDevice  *Hdmi
  )
{
  DEBUG ((DEBUG_INIT, "DwHdmiQpLib.c: Dumping EDID: \n"));
  UINT8           EDID[EDID_SIZE];
  UINT8           BaseAddr = 0x0;
  struct i2c_msg  msgs[]   = {
    {
      .addr  = DDC_ADDR,
      .flags = 0,
      .len   = 1,
      .buf   = &BaseAddr,
    },{
      .addr  = DDC_ADDR,
      .flags = I2C_M_RD,
      .len   = EDID_SIZE,
      .buf   = EDID,
    }
  };

  if (DwHdmiQpI2cXfer (Hdmi, msgs, 2)) {
    return;
  }

  for (int i = 0; i < EDID_SIZE; i++) {
    DEBUG ((DEBUG_INIT, "%02x ", EDID[i]));
    if (!((i + 1) % 8)) {
      DEBUG ((DEBUG_INIT, "\n"));
    }
  }
}

VOID
DwHdmiQpI2cSetIomux (
  OUT struct  DwHdmiQpDevice  *Hdmi
  )
{
  if (!Hdmi->Id) {
    switch (Hdmi->I2c.PinMux) {
      case 0:
        GpioPinSetFunction (4, GPIO_PIN_PB7, 0x5);
        GpioPinSetFunction (4, GPIO_PIN_PC0, 0x5);
        break;
      case 1:
        GpioPinSetFunction (0, GPIO_PIN_PD5, 0xb);
        GpioPinSetFunction (0, GPIO_PIN_PD4, 0xb);
        break;
      case 2:
        GpioPinSetFunction (3, GPIO_PIN_PC7, 0x5);
        GpioPinSetFunction (3, GPIO_PIN_PD0, 0x5);
        break;
      default:
        break;
    }
  } else {
    switch (Hdmi->I2c.PinMux) {
      case 0:
        GpioPinSetFunction (2, GPIO_PIN_PB4, 0x4);
        GpioPinSetFunction (2, GPIO_PIN_PB5, 0x4);
        break;
      case 1:
        GpioPinSetFunction (3, GPIO_PIN_PC5, 0x5);
        GpioPinSetFunction (3, GPIO_PIN_PC6, 0x5);
        break;
      case 2:
        GpioPinSetFunction (1, GPIO_PIN_PA3, 0x5);
        GpioPinSetFunction (1, GPIO_PIN_PA4, 0x5);
        break;
      default:
        break;
    }
  }
}

EFI_STATUS
DwHdmiQpConnectorPreInit (
  OUT ROCKCHIP_CONNECTOR_PROTOCOL  *This,
  OUT DISPLAY_STATE                *DisplayState
  )
{
  CONNECTOR_STATE              *ConnectorState = &DisplayState->ConnectorState;
  struct RockchipHdptxPhyHdmi  Hdptx;
  struct DwHdmiQpDevice        *Hdmi;

  Hdmi = AllocateZeroPool (sizeof (*Hdmi));

  DEBUG ((DEBUG_INIT, "DwHdmiQpConnectorPreInit"));
  ConnectorState->Type = DRM_MODE_CONNECTOR_HDMIA;
  Hdmi->Id             = Hdptx.Id = PcdGet32 (PcdHdmiId);
  Hdmi->I2c.PinMux     = PcdGet32 (PcdHdmiDDCI2CPinMux);

  if (Hdmi->Id) {
    ConnectorState->OutputInterface = VOP_OUTPUT_IF_HDMI1;
  } else {
    ConnectorState->OutputInterface = VOP_OUTPUT_IF_HDMI0;
  }

  DwHdmiQpSetIomux (Hdmi);
  DwHdmiQpI2cSetIomux (Hdmi);
  DwHdmiI2cInit (Hdmi);

  HdptxRopllCmnConfig (&Hdptx);
  DEBUG ((DEBUG_INFO, "%a hdmi pre init success\n", __func__));
  return 0;
}

EFI_STATUS
DwHdmiQpConnectorInit (
  OUT ROCKCHIP_CONNECTOR_PROTOCOL  *This,
  OUT DISPLAY_STATE                *DisplayState
  )
{
  CONNECTOR_STATE  *ConnectorState = &DisplayState->ConnectorState;

  DEBUG ((DEBUG_INIT, "DwHdmiQpConnectorInit"));
  ConnectorState->OutputMode = ROCKCHIP_OUT_MODE_AAAA;
  ConnectorState->ColorSpace = V4L2_COLORSPACE_DEFAULT;

  return 0;
}

EFI_STATUS
DwHdmiQpConnectorGetEdid (
  OUT ROCKCHIP_CONNECTOR_PROTOCOL  *This,
  OUT DISPLAY_STATE                *DisplayState
  )
{
  // Todo
  return 0;
}

VOID
Rk3588SetColorFormat (
  OUT struct DwHdmiQpDevice  *Hdmi,
  IN UINT64                  BusFormat,
  IN UINT32                  Depth
  )
{
  UINT32  Val = 0;

  switch (BusFormat) {
    case MEDIA_BUS_FMT_RGB888_1X24:
    case MEDIA_BUS_FMT_RGB101010_1X30:
      Val = HIWORD_UPDATE (0, RK3588_COLOR_FORMAT_MASK);
      break;
    case MEDIA_BUS_FMT_UYYVYY8_0_5X24:
    case MEDIA_BUS_FMT_UYYVYY10_0_5X30:
      Val = HIWORD_UPDATE (RK3588_YUV420, RK3588_COLOR_FORMAT_MASK);
      break;
    case MEDIA_BUS_FMT_YUV8_1X24:
    case MEDIA_BUS_FMT_YUV10_1X30:
      Val = HIWORD_UPDATE (RK3588_YUV444, RK3588_COLOR_FORMAT_MASK);
      break;
    default:
      DEBUG ((DEBUG_INFO, "%a can't set correct color format\n", __func__));
      return;
  }

  if (Depth == 8) {
    Val |= HIWORD_UPDATE (RK3588_8BPC, RK3588_COLOR_DEPTH_MASK);
  } else {
    Val |= HIWORD_UPDATE (RK3588_10BPC, RK3588_COLOR_DEPTH_MASK);
  }

  if (!Hdmi->Id) {
    MmioWrite32 (0xFD5A8000 + RK3588_GRF_VO1_CON3, Val);
  } else {
    MmioWrite32 (0xFD5A8000 + RK3588_GRF_VO1_CON6, Val);
  }
}

VOID
HdmiConfigAvi (
  OUT struct DwHdmiQpDevice  *Hdmi
  )
{
  /* 1080p60 */
  DwHdmiQpRegWrite (Hdmi, 0x000d0200, PKT_AVI_CONTENTS0);
  DwHdmiQpRegWrite (Hdmi, 0x00281027, PKT_AVI_CONTENTS1);
  DwHdmiQpRegWrite (Hdmi, 0x00000010, PKT_AVI_CONTENTS2);
  DwHdmiQpRegWrite (Hdmi, 0x00000000, PKT_AVI_CONTENTS3);
  DwHdmiQpRegWrite (Hdmi, 0x00000000, PKT_AVI_CONTENTS4);
  DwHdmiQpRegWrite (Hdmi, 0x00000000, PKT_AVI_CONTENTS5);
  DwHdmiQpRegWrite (Hdmi, 0x00000000, PKT_AVI_CONTENTS6);
  DwHdmiQpRegWrite (Hdmi, 0x00000000, PKT_AVI_CONTENTS7);
}

VOID
DwHdmiQpSetup (
  OUT struct DwHdmiQpDevice  *Hdmi,
  OUT DISPLAY_STATE          *DisplayState
  )
{
  struct RockchipHdptxPhyHdmi  Hdptx;
  CONNECTOR_STATE              *ConnectorState = &DisplayState->ConnectorState;
  UINT32                       Val             = 0;

  ConnectorState->Type = DRM_MODE_CONNECTOR_HDMIA;
  Hdptx.Id             = PcdGet32 (PcdHdmiId);

  Val = DwHdmiQpRegRead (Hdmi, 0xb0);
  DEBUG ((DEBUG_INIT, "%a Hdptx.Id :%d\n", __func__, Hdptx.Id));
  DEBUG ((DEBUG_INIT, "%a 0xb0:%d\n", __func__, Val));
  Rk3588SetColorFormat (Hdmi, MEDIA_BUS_FMT_RGB888_1X24, 8);
  HdmiConfigAvi (Hdmi);

  DwHdmiQpRegMod (Hdmi, 0, OPMODE_DVI, LINK_CONFIG0);
  DwHdmiQpRegMod (Hdmi, HDCP2_BYPASS, HDCP2_BYPASS, HDCP2LOGIC_CONFIG0);
  DwHdmiQpRegMod (Hdmi, KEEPOUT_REKEY_ALWAYS, KEEPOUT_REKEY_CFG, FRAME_COMPOSER_CONFIG9);
  DwHdmiQpRegWrite (Hdmi, 0, FLT_CONFIG0);

  DumpEdid (Hdmi);

  // enable phy output
  HdptxRopllTmdsModeConfig (&Hdptx);
  MicroSecondDelay (50);
  DwHdmiQpRegWrite (Hdmi, 2, PKTSCHED_PKT_CONTROL0);
  DwHdmiQpRegMod (Hdmi, PKTSCHED_GCP_TX_EN, PKTSCHED_GCP_TX_EN, PKTSCHED_PKT_EN);
}

EFI_STATUS
DwHdmiQpConnectorEnable (
  OUT ROCKCHIP_CONNECTOR_PROTOCOL  *This,
  OUT DISPLAY_STATE                *DisplayState
  )
{
  struct DwHdmiQpDevice  *Hdmi;

  Hdmi = AllocatePool (sizeof (*Hdmi));
  DEBUG ((DEBUG_INIT, "DwHdmiQpConnectorEnable\n"));
  Hdmi->Id = PcdGet32 (PcdHdmiId);

  DwHdmiQpSetup (Hdmi, DisplayState);

  return 0;
}

EFI_STATUS
DwHdmiQpConnectorDisable (
  OUT ROCKCHIP_CONNECTOR_PROTOCOL  *This,
  OUT DISPLAY_STATE                *DisplayState
  )
{
  // Todo
  return 0;
}

EFI_STATUS
DwHdmiQpConnectorDetect (
  OUT ROCKCHIP_CONNECTOR_PROTOCOL  *This,
  OUT DISPLAY_STATE                *DisplayState
  )
{
  // Todo
  return 0;
}

ROCKCHIP_CONNECTOR_PROTOCOL  mHdmi = {
  NULL,
  DwHdmiQpConnectorPreInit,
  DwHdmiQpConnectorInit,
  NULL,
  DwHdmiQpConnectorDetect,
  NULL,
  DwHdmiQpConnectorGetEdid,
  NULL,
  DwHdmiQpConnectorEnable,
  DwHdmiQpConnectorDisable,
  NULL
};

EFI_STATUS
EFIAPI
DwHdmiQpInitHdmi (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  Handle;

  DEBUG ((DEBUG_INIT, "hdmi init start\n"));
  Handle = NULL;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gRockchipConnectorProtocolGuid,
                  &mHdmi,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INIT, "hdmi init success\n"));
  return EFI_SUCCESS;
}
