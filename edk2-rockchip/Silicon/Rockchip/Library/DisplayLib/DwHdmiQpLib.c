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

#include <string.h>

#include <Protocol/RockchipConnectorProtocol.h>

#include <Uefi/UefiBaseType.h>

#define HIWORD_UPDATE(val, mask)	(val | (mask) << 16)

#define RK3588_GRF_SOC_CON2		0x0308
#define RK3588_HDMI1_HPD_INT_MSK	BIT(15)
#define RK3588_HDMI1_HPD_INT_CLR	BIT(14)
#define RK3588_HDMI0_HPD_INT_MSK	BIT(13)
#define RK3588_HDMI0_HPD_INT_CLR	BIT(12)
#define RK3588_GRF_SOC_CON7		0x031c
#define RK3588_SET_HPD_PATH_MASK	(0x3 << 12)
#define RK3588_GRF_SOC_STATUS1		0x0384
#define RK3588_HDMI0_LOW_MORETHAN100MS	BIT(20)
#define RK3588_HDMI0_HPD_PORT_LEVEL	BIT(19)
#define RK3588_HDMI0_IHPD_PORT		BIT(18)
#define RK3588_HDMI0_OHPD_INT		BIT(17)
#define RK3588_HDMI0_LEVEL_INT		BIT(16)
#define RK3588_HDMI0_INTR_CHANGE_CNT	(0x7 << 13)
#define RK3588_HDMI1_LOW_MORETHAN100MS	BIT(28)
#define RK3588_HDMI1_HPD_PORT_LEVEL	BIT(27)
#define RK3588_HDMI1_IHPD_PORT		BIT(26)
#define RK3588_HDMI1_OHPD_INT		BIT(25)
#define RK3588_HDMI1_LEVEL_INT		BIT(24)
#define RK3588_HDMI1_INTR_CHANGE_CNT	(0x7 << 21)

#define RK3588_GRF_VO1_CON3		0x000c
#define RK3588_COLOR_FORMAT_MASK	0xf
#define RK3588_YUV444			0x2
#define RK3588_YUV420			0x3
#define RK3588_COMPRESSED_DATA		0xb
#define RK3588_COLOR_DEPTH_MASK		(0xf << 4)
#define RK3588_8BPC			0
#define RK3588_10BPC			(0x6 << 4)
#define RK3588_CECIN_MASK		BIT(8)
#define RK3588_SCLIN_MASK		BIT(9)
#define RK3588_SDAIN_MASK		BIT(10)
#define RK3588_MODE_MASK		BIT(11)
#define RK3588_COMPRESS_MODE_MASK	BIT(12)
#define RK3588_I2S_SEL_MASK		BIT(13)
#define RK3588_SPDIF_SEL_MASK		BIT(14)
#define RK3588_GRF_VO1_CON4		0x0010
#define RK3588_HDMI21_MASK		BIT(0)
#define RK3588_GRF_VO1_CON9		0x0024
#define RK3588_HDMI0_GRANT_SEL		BIT(10)
#define RK3588_HDMI0_GRANT_SW		BIT(11)
#define RK3588_HDMI1_GRANT_SEL		BIT(12)
#define RK3588_HDMI1_GRANT_SW		BIT(13)
#define RK3588_GRF_VO1_CON6		0x0018
#define RK3588_GRF_VO1_CON7		0x001c

#define PPS_TABLE_LEN 8

#define COLOR_DEPTH_10BIT		BIT(31)
#define HDMI_FRL_MODE			BIT(30)
#define HDMI_EARC_MODE			BIT(29)
#define DATA_RATE_MASK			0xFFFFFFF

#define HDMI20_MAX_RATE			600000
#define HDMI_8K60_RATE			2376000

VOID
DwHdmiQpRegWrite (
  OUT struct  DwHdmiQpDevice *Hdmi,
  IN  UINT32  Value,
  IN  UINT32  Offset
  )
{
  UINT32 BASE;

  if (!Hdmi->Id)
    BASE = HDMI0_BASE;
  else
    BASE = HDMI1_BASE;

  MmioWrite32(BASE + Offset, Value);
};

UINT32
DwHdmiQpRegRead (
  OUT struct  DwHdmiQpDevice *Hdmi,
  IN  UINT32  Offset
  )
{
  UINT32 Value;
  UINT32 BASE;

  if (!Hdmi->Id)
    BASE = HDMI0_BASE;
  else
    BASE = HDMI1_BASE;

  Value = MmioRead32(BASE + Offset);
  return Value;
};

VOID
DwHdmiQpRegMod (
  OUT struct  DwHdmiQpDevice *Hdmi,
  IN  UINT32  Value,
  IN  UINT32  Mask,
  IN  UINT32  Offset
  )
{
  UINT32 Val;
  UINT32 BASE;

  if (!Hdmi->Id)
    BASE = HDMI0_BASE;
  else
    BASE = HDMI1_BASE;

  Val = MmioRead32(BASE + Offset);
  Val |= Value & Mask;

  MmioWrite32(BASE + Offset, Val);
};

VOID
DwHdmiQpSetIomux(
  OUT struct  DwHdmiQpDevice *Hdmi
  )
{
  UINT32 Val;

  if (!Hdmi->Id) {
    Val = HIWORD_UPDATE(RK3588_SCLIN_MASK, RK3588_SCLIN_MASK) |
          HIWORD_UPDATE(RK3588_SDAIN_MASK, RK3588_SDAIN_MASK) |
          HIWORD_UPDATE(RK3588_MODE_MASK, RK3588_MODE_MASK) |
          HIWORD_UPDATE(RK3588_I2S_SEL_MASK, RK3588_I2S_SEL_MASK);
    MmioWrite32(0xFD5A8000 + RK3588_GRF_VO1_CON3, Val);
  
    Val = HIWORD_UPDATE(RK3588_SET_HPD_PATH_MASK,
          RK3588_SET_HPD_PATH_MASK);
    MmioWrite32(0xFD58C000 + RK3588_GRF_SOC_CON7, Val);
  
    Val = HIWORD_UPDATE(RK3588_HDMI0_GRANT_SEL,
          RK3588_HDMI0_GRANT_SEL);
    MmioWrite32(0xFD5A8000 + RK3588_GRF_VO1_CON9, Val);
  } else {
    Val = HIWORD_UPDATE(RK3588_SCLIN_MASK, RK3588_SCLIN_MASK) |
          HIWORD_UPDATE(RK3588_SDAIN_MASK, RK3588_SDAIN_MASK) |
          HIWORD_UPDATE(RK3588_MODE_MASK, RK3588_MODE_MASK) |
          HIWORD_UPDATE(RK3588_I2S_SEL_MASK, RK3588_I2S_SEL_MASK);
    MmioWrite32(0xFD5A8000 + RK3588_GRF_VO1_CON6, Val);
  
    Val = HIWORD_UPDATE(RK3588_SET_HPD_PATH_MASK,
          RK3588_SET_HPD_PATH_MASK);
    MmioWrite32(0xFD58C000 + RK3588_GRF_SOC_CON7, Val);
  
    Val = HIWORD_UPDATE(RK3588_HDMI1_GRANT_SEL,
          RK3588_HDMI1_GRANT_SEL);
    MmioWrite32(0xFD5A8000 + RK3588_GRF_VO1_CON9, Val);
  }
};

EFI_STATUS
DwHdmiQpConnectorPreInit (
  OUT ROCKCHIP_CONNECTOR_PROTOCOL          *This,
  OUT DISPLAY_STATE                        *DisplayState
  )
{
  CONNECTOR_STATE *ConnectorState = &DisplayState->ConnectorState;
  struct RockchipHdptxPhyHdmi Hdptx;
  struct DwHdmiQpDevice *Hdmi;
  Hdmi = AllocateZeroPool(sizeof (*Hdmi));

  DEBUG ((DEBUG_INIT, "DwHdmiQpConnectorPreInit"));
  ConnectorState->Type = DRM_MODE_CONNECTOR_HDMIA;
  Hdmi->Id = Hdptx.Id = PcdGet32(PcdHdmiId);

  if (Hdmi->Id)
    ConnectorState->OutputInterface = VOP_OUTPUT_IF_HDMI1;
  else
    ConnectorState->OutputInterface = VOP_OUTPUT_IF_HDMI0;

  DwHdmiQpSetIomux(Hdmi);

  HdptxRopllCmnConfig(&Hdptx);
  DEBUG ((DEBUG_INFO, "%a hdmi pre init success\n", __func__));
  return 0;
};

EFI_STATUS
DwHdmiQpConnectorInit (
  OUT ROCKCHIP_CONNECTOR_PROTOCOL          *This,
  OUT DISPLAY_STATE                        *DisplayState
  )
{
  CONNECTOR_STATE *ConnectorState = &DisplayState->ConnectorState;

  DEBUG ((DEBUG_INIT, "DwHdmiQpConnectorInit"));
  ConnectorState->OutputMode = ROCKCHIP_OUT_MODE_AAAA;
  ConnectorState->ColorSpace = V4L2_COLORSPACE_DEFAULT;

  return 0;
};

EFI_STATUS
DwHdmiQpConnectorGetEdid (
  OUT ROCKCHIP_CONNECTOR_PROTOCOL          *This,
  OUT DISPLAY_STATE                        *DisplayState
  )
{
  //Todo
  return 0;
};

VOID
Rk3588SetColorFormat(
  OUT struct DwHdmiQpDevice *Hdmi,
  IN UINT64 BusFormat,
  IN UINT32 Depth
  )
{
  UINT32 Val = 0;

  switch (BusFormat) {
    case MEDIA_BUS_FMT_RGB888_1X24:
    case MEDIA_BUS_FMT_RGB101010_1X30:
      Val = HIWORD_UPDATE(0, RK3588_COLOR_FORMAT_MASK);
      break;
    case MEDIA_BUS_FMT_UYYVYY8_0_5X24:
    case MEDIA_BUS_FMT_UYYVYY10_0_5X30:
      Val = HIWORD_UPDATE(RK3588_YUV420, RK3588_COLOR_FORMAT_MASK);
      break;
    case MEDIA_BUS_FMT_YUV8_1X24:
    case MEDIA_BUS_FMT_YUV10_1X30:
      Val = HIWORD_UPDATE(RK3588_YUV444, RK3588_COLOR_FORMAT_MASK);
      break;
    default:
      DEBUG ((DEBUG_INFO, "%a can't set correct color format\n", __func__));
      return;
  }

  if (Depth == 8)
    Val |= HIWORD_UPDATE(RK3588_8BPC, RK3588_COLOR_DEPTH_MASK);
  else
    Val |= HIWORD_UPDATE(RK3588_10BPC, RK3588_COLOR_DEPTH_MASK);

  if (!Hdmi->Id)
    MmioWrite32(0xFD5A8000 + RK3588_GRF_VO1_CON3, Val);
  else
    MmioWrite32(0xFD5A8000 + RK3588_GRF_VO1_CON6, Val);
}

VOID
HdmiConfigAvi(
  OUT struct DwHdmiQpDevice *Hdmi
  )
{
  /* 1080p60 */
  DwHdmiQpRegWrite(Hdmi, 0x000d0200, PKT_AVI_CONTENTS0);
  DwHdmiQpRegWrite(Hdmi, 0x00281027, PKT_AVI_CONTENTS1);
  DwHdmiQpRegWrite(Hdmi, 0x00000010, PKT_AVI_CONTENTS2);
  DwHdmiQpRegWrite(Hdmi, 0x00000000, PKT_AVI_CONTENTS3);
  DwHdmiQpRegWrite(Hdmi, 0x00000000, PKT_AVI_CONTENTS4);
  DwHdmiQpRegWrite(Hdmi, 0x00000000, PKT_AVI_CONTENTS5);
  DwHdmiQpRegWrite(Hdmi, 0x00000000, PKT_AVI_CONTENTS6);
  DwHdmiQpRegWrite(Hdmi, 0x00000000, PKT_AVI_CONTENTS7);
};

VOID
DwHdmiQpSetup(
  OUT struct DwHdmiQpDevice *Hdmi,
  OUT DISPLAY_STATE *DisplayState
  )
{
  struct RockchipHdptxPhyHdmi Hdptx;
  CONNECTOR_STATE *ConnectorState = &DisplayState->ConnectorState;
  UINT32 Val = 0;

  ConnectorState->Type = DRM_MODE_CONNECTOR_HDMIA;
  Hdptx.Id = PcdGet32(PcdHdmiId);

  Val = DwHdmiQpRegRead(Hdmi, 0xb0);
  DEBUG ((DEBUG_INIT, "%a Hdptx.Id :%d\n", __func__, Hdptx.Id));
  DEBUG ((DEBUG_INIT, "%a 0xb0:%d\n", __func__, Val));
  Rk3588SetColorFormat(Hdmi, MEDIA_BUS_FMT_RGB888_1X24, 8);
  HdmiConfigAvi(Hdmi);

  DwHdmiQpRegMod(Hdmi, 0, OPMODE_DVI, LINK_CONFIG0);
  DwHdmiQpRegMod(Hdmi, HDCP2_BYPASS, HDCP2_BYPASS, HDCP2LOGIC_CONFIG0);
  DwHdmiQpRegMod(Hdmi, KEEPOUT_REKEY_ALWAYS, KEEPOUT_REKEY_CFG, FRAME_COMPOSER_CONFIG9);
  DwHdmiQpRegWrite(Hdmi, 0, FLT_CONFIG0);
  
  //enable phy output
  HdptxRopllTmdsModeConfig(&Hdptx);
  MicroSecondDelay(50);
  DwHdmiQpRegWrite(Hdmi, 2, PKTSCHED_PKT_CONTROL0);
  DwHdmiQpRegMod(Hdmi, PKTSCHED_GCP_TX_EN, PKTSCHED_GCP_TX_EN, PKTSCHED_PKT_EN);
};

EFI_STATUS
DwHdmiQpConnectorEnable (
  OUT ROCKCHIP_CONNECTOR_PROTOCOL          *This,
  OUT DISPLAY_STATE                       *DisplayState
  )
{
  struct DwHdmiQpDevice *Hdmi;
  Hdmi = AllocatePool(sizeof (*Hdmi));
  DEBUG ((DEBUG_INIT, "DwHdmiQpConnectorEnable\n"));
  Hdmi->Id = PcdGet32(PcdHdmiId);
  
  DwHdmiQpSetup(Hdmi, DisplayState);

  return 0;
};

EFI_STATUS
DwHdmiQpConnectorDisable (
  OUT ROCKCHIP_CONNECTOR_PROTOCOL          *This,
  OUT DISPLAY_STATE                        *DisplayState
  )
{
  //Todo
  return 0;
};

EFI_STATUS
DwHdmiQpConnectorDetect (
  OUT ROCKCHIP_CONNECTOR_PROTOCOL          *This,
  OUT DISPLAY_STATE                        *DisplayState
  )
{
  //Todo
  return 0;
};

ROCKCHIP_CONNECTOR_PROTOCOL mHdmi = {
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
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS    Status;
  EFI_HANDLE    Handle;
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
