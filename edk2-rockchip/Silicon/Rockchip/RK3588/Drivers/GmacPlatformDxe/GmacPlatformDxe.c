/** @file
 *
 *  RK3588 GMAC initializer
 *
 *  Copyright (c) 2021-2022, Jared McNeill <jmcneill@invisible.ca>
 *  Copyright (c) 2023-2025, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/BaseCryptLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/IoLib.h>
#include <Library/NetLib.h>
#include <Library/OtpLib.h>
#include <Library/RockchipPlatformLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/DwcEqosPlatformDevice.h>

#include "EthernetPhy.h"

#define GRF_BIT(nr)      (1 << (nr) | 1 << (nr + 16))
#define GRF_CLR_BIT(nr)  (1 << (nr + 16))
#define HIWORD_UPDATE(val, mask, shift) \
        ((val) << (shift) | (mask) << ((shift) + 16))

/* PHP_GRF registers */
#define RK3588_PHP_GRF_BASE  0xFD5B0000

#define RK3588_PHP_GRF_GMAC_CON0  (RK3588_PHP_GRF_BASE + 0x0008)

#define RK3588_GMAC_PHY_INTF_SEL_RGMII(id)      \
        (GRF_BIT(3 + (id) * 6) | GRF_CLR_BIT(4 + (id) * 6) | GRF_CLR_BIT(5 + (id) * 6))
#define RK3588_GMAC_PHY_INTF_SEL_RMII(id)       \
        (GRF_CLR_BIT(3 + (id) * 6) | GRF_CLR_BIT(4 + (id) * 6) | GRF_BIT(5 + (id) * 6))

#define RK3588_PHP_GRF_CLK_CON1  (RK3588_PHP_GRF_BASE + 0x0070)

#define RK3588_GMAC_CLK_RMII_MODE(id)   GRF_BIT(5 * (id))
#define RK3588_GMAC_CLK_RGMII_MODE(id)  GRF_CLR_BIT(5 * (id))

#define RK3588_GMAC_CLK_SELECT_CRU(id)  GRF_BIT(5 * (id) + 4)
#define RK3588_GMAC_CLK_SELECT_IO(id)   GRF_CLR_BIT(5 * (id) + 4)

#define RK3588_GMAC_CLK_SEL_RMII_2_5(id)  GRF_BIT(5 * (id) + 2)
#define RK3588_GMAC_CLK_SEL_RMII_25(id)   GRF_CLR_BIT(5 * (id) + 2)

#define RK3588_GMAC_CLK_SEL_RGMII_2_5(id)         \
                        (GRF_CLR_BIT(5 * (id) + 2) | GRF_BIT(5 * (id) + 3))
#define RK3588_GMAC_CLK_SEL_RGMII_25(id)          \
                        (GRF_BIT(5 * (id) + 2) | GRF_BIT(5 * (id) + 3))
#define RK3588_GMAC_CLK_SEL_RGMII_125(id)          \
                        (GRF_CLR_BIT(5 * (id) + 2) | GRF_CLR_BIT(5 * (id) + 3))

/* SYS_GRF registers */
#define RK3588_SYS_GRF_BASE  0xFD58C000

#define RK3588_SYS_GRF_SOC_CON7  (RK3588_SYS_GRF_BASE + 0x031C)

#define RK3588_GMAC_RXCLK_DLY_ENABLE(id)   GRF_BIT(2 * (id) + 3)
#define RK3588_GMAC_RXCLK_DLY_DISABLE(id)  GRF_CLR_BIT(2 * (id) + 3)
#define RK3588_GMAC_TXCLK_DLY_ENABLE(id)   GRF_BIT(2 * (id) + 2)
#define RK3588_GMAC_TXCLK_DLY_DISABLE(id)  GRF_CLR_BIT(2 * (id) + 2)

#define RK3588_SYS_GRF_SOC_CON8  (RK3588_SYS_GRF_BASE + 0x0320)
#define RK3588_SYS_GRF_SOC_CON9  (RK3588_SYS_GRF_BASE + 0x0324)

#define RK3588_GMAC_CLK_RX_DL_CFG(val)  HIWORD_UPDATE(val, 0xFF, 8)
#define RK3588_GMAC_CLK_TX_DL_CFG(val)  HIWORD_UPDATE(val, 0xFF, 0)

/* GMAC registers */
#define  GMAC_MAC_MDIO_ADDRESS              0x0200
#define   GMAC_MAC_MDIO_ADDRESS_PA_SHIFT    21
#define   GMAC_MAC_MDIO_ADDRESS_RDA_SHIFT   16
#define   GMAC_MAC_MDIO_ADDRESS_CR_SHIFT    8
#define   GMAC_MAC_MDIO_ADDRESS_CR_100_150  (1U << GMAC_MAC_MDIO_ADDRESS_CR_SHIFT)
#define   GMAC_MAC_MDIO_ADDRESS_GOC_SHIFT   2
#define   GMAC_MAC_MDIO_ADDRESS_GOC_READ    (3U << GMAC_MAC_MDIO_ADDRESS_GOC_SHIFT)
#define   GMAC_MAC_MDIO_ADDRESS_GOC_WRITE   (1U << GMAC_MAC_MDIO_ADDRESS_GOC_SHIFT)
#define   GMAC_MAC_MDIO_ADDRESS_GB          BIT0
#define  GMAC_MAC_MDIO_DATA                 0x0204

/* MII registers */
#define MII_PHYIDR1  0x02
#define MII_PHYIDR2  0x03

#pragma pack (1)
typedef struct {
  VENDOR_DEVICE_PATH          VendorDP;
  UINT8                       ControllerId;
  MAC_ADDR_DEVICE_PATH        MacAddrDP;
  EFI_DEVICE_PATH_PROTOCOL    End;
} GMAC_DEVICE_PATH;
#pragma pack ()

typedef struct {
  UINT32                               Signature;
  UINT8                                Id;
  EFI_PHYSICAL_ADDRESS                 BaseAddress;

  BOOLEAN                              Supported;
  UINT8                                TxDelay;
  UINT8                                RxDelay;

  DWC_EQOS_PLATFORM_DEVICE_PROTOCOL    EqosPlatform;
  GMAC_DEVICE_PATH                     DevicePath;
} GMAC_DEVICE;

#define GMAC_DEVICE_SIGNATURE  SIGNATURE_32 ('G', 'M', 'a', 'C')

#define GMAC_DEVICE_FROM_EQOS_PLATFORM(a) \
  CR (a, GMAC_DEVICE, EqosPlatform, GMAC_DEVICE_SIGNATURE)

#define GMAC_DEVICE_INIT(_Id, _BaseAddress)                     \
  {                                                             \
    .Signature    = GMAC_DEVICE_SIGNATURE,                      \
    .Id           = _Id,                                        \
    .BaseAddress  = _BaseAddress,                               \
    .Supported    = FixedPcdGetBool(PcdGmac##_Id##Supported),   \
    .TxDelay      = FixedPcdGet8(PcdGmac##_Id##TxDelay),        \
    .RxDelay      = FixedPcdGet8(PcdGmac##_Id##RxDelay)         \
  }

STATIC GMAC_DEVICE_PATH  mGmacDevicePathTemplate = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(OFFSET_OF (GMAC_DEVICE_PATH, MacAddrDP)),
        (UINT8)((OFFSET_OF (GMAC_DEVICE_PATH, MacAddrDP)) >> 8)
      }
    },
    EFI_CALLER_ID_GUID
  },
  0,
  {
    {
      MESSAGING_DEVICE_PATH,
      MSG_MAC_ADDR_DP,
      {
        (UINT8)(sizeof (MAC_ADDR_DEVICE_PATH)),
        (UINT8)((sizeof (MAC_ADDR_DEVICE_PATH)) >> 8)
      }
    },
    {
      { 0 }
    },
    NET_IFTYPE_ETHERNET
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      sizeof (EFI_DEVICE_PATH_PROTOCOL),
      0
    }
  }
};

STATIC GMAC_DEVICE  mGmacDevices[] = {
  GMAC_DEVICE_INIT (0, 0xfe1b0000),
  GMAC_DEVICE_INIT (1, 0xfe1c0000),
};

//
// XXX: It may be tempting to move all this PHY code to the MAC driver,
// however it is too platform specific and would make the driver less
// generic, as it does not currently need to touch any MII registers in
// order to work -- assuming the PHY has been previously initialized or
// is already usable in its default state.
//
// The proper way to fix this would be to have an MDIO bus/device protocol
// and separate PHY device drivers, including support for additional platform
// data.
//
// It is also important to leave the PHY configured for ACPI OSes that don't
// support initializing it (ESXi, Windows), regardless of whether UEFI uses
// the network interface (see how the MAC address is programmed in this case).
//
STATIC ETHERNET_PHY_INIT  mPhyInitList[] = {
  RealtekPhyInit,
  MotorcommPhyInit
};

VOID
PhyRead (
  IN  EFI_PHYSICAL_ADDRESS  GmacBase,
  IN  UINT8                 Phy,
  IN  UINT16                Reg,
  OUT UINT16                *Value
  )
{
  UINT32  Addr;
  UINTN   Retry;

  Addr = GMAC_MAC_MDIO_ADDRESS_CR_100_150 |
         (Phy << GMAC_MAC_MDIO_ADDRESS_PA_SHIFT) |
         (Reg << GMAC_MAC_MDIO_ADDRESS_RDA_SHIFT) |
         GMAC_MAC_MDIO_ADDRESS_GOC_READ |
         GMAC_MAC_MDIO_ADDRESS_GB;
  MmioWrite32 (GmacBase + GMAC_MAC_MDIO_ADDRESS, Addr);

  MicroSecondDelay (10000);

  for (Retry = 1000; Retry > 0; Retry--) {
    Addr = MmioRead32 (GmacBase + GMAC_MAC_MDIO_ADDRESS);
    if ((Addr & GMAC_MAC_MDIO_ADDRESS_GB) == 0) {
      *Value = MmioRead32 (GmacBase + GMAC_MAC_MDIO_DATA) & 0xFFFFu;
      break;
    }

    MicroSecondDelay (10);
  }

  if (Retry == 0) {
    DEBUG ((DEBUG_WARN, "MDIO: PHY read timeout!\n"));
    *Value = 0xFFFFU;
    ASSERT (FALSE);
  }
}

VOID
PhyWrite (
  IN EFI_PHYSICAL_ADDRESS  GmacBase,
  IN UINT8                 Phy,
  IN UINT16                Reg,
  IN UINT16                Value
  )
{
  UINT32  Addr;
  UINTN   Retry;

  MmioWrite32 (GmacBase + GMAC_MAC_MDIO_DATA, Value);

  Addr = GMAC_MAC_MDIO_ADDRESS_CR_100_150 |
         (Phy << GMAC_MAC_MDIO_ADDRESS_PA_SHIFT) |
         (Reg << GMAC_MAC_MDIO_ADDRESS_RDA_SHIFT) |
         GMAC_MAC_MDIO_ADDRESS_GOC_WRITE |
         GMAC_MAC_MDIO_ADDRESS_GB;
  MmioWrite32 (GmacBase + GMAC_MAC_MDIO_ADDRESS, Addr);

  MicroSecondDelay (10000);

  for (Retry = 1000; Retry > 0; Retry--) {
    Addr = MmioRead32 (GmacBase + GMAC_MAC_MDIO_ADDRESS);
    if ((Addr & GMAC_MAC_MDIO_ADDRESS_GB) == 0) {
      break;
    }

    MicroSecondDelay (10);
  }

  if (Retry == 0) {
    DEBUG ((DEBUG_WARN, "MDIO: PHY write timeout!\n"));
    ASSERT (FALSE);
  }
}

STATIC
VOID
EFIAPI
PhyInit (
  IN EFI_PHYSICAL_ADDRESS  GmacBase
  )
{
  EFI_STATUS  Status;
  UINT16      PhyIdReg;
  UINT32      PhyId;
  UINT32      Index;

  PhyRead (GmacBase, 0, MII_PHYIDR1, &PhyIdReg);
  PhyId = PhyIdReg << 16;
  PhyRead (GmacBase, 0, MII_PHYIDR2, &PhyIdReg);
  PhyId |= PhyIdReg;

  for (Index = 0; Index < ARRAY_SIZE (mPhyInitList); Index++) {
    Status = mPhyInitList[Index](GmacBase, PhyId);
    if (Status == EFI_UNSUPPORTED) {
      continue;
    } else if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: mPhyInitList[%d]() failed: %r\n", __func__, Index, Status));
    }

    return;
  }

  DEBUG ((DEBUG_ERROR, "%a: Unknown PHY ID %08X\n", __func__, PhyId));
}

STATIC
VOID
GmacSetToRgmii (
  IN UINT32  Id,
  IN UINT8   TxDelay,
  IN UINT8   RxDelay
  )
{
  UINT32  ConDlyReg;

  ConDlyReg = (Id == 1) ? RK3588_SYS_GRF_SOC_CON9
                        : RK3588_SYS_GRF_SOC_CON8;

  MmioWrite32 (RK3588_PHP_GRF_GMAC_CON0, RK3588_GMAC_PHY_INTF_SEL_RGMII (Id));

  MmioWrite32 (RK3588_PHP_GRF_CLK_CON1, RK3588_GMAC_CLK_RGMII_MODE (Id));

  if (TxDelay) {
    MmioWrite32 (RK3588_SYS_GRF_SOC_CON7, RK3588_GMAC_TXCLK_DLY_ENABLE (Id));
    MmioWrite32 (ConDlyReg, RK3588_GMAC_CLK_TX_DL_CFG (TxDelay));
  } else {
    MmioWrite32 (RK3588_SYS_GRF_SOC_CON7, RK3588_GMAC_TXCLK_DLY_DISABLE (Id));
  }

  if (RxDelay) {
    MmioWrite32 (RK3588_SYS_GRF_SOC_CON7, RK3588_GMAC_RXCLK_DLY_ENABLE (Id));
    MmioWrite32 (ConDlyReg, RK3588_GMAC_CLK_RX_DL_CFG (RxDelay));
  } else {
    MmioWrite32 (RK3588_SYS_GRF_SOC_CON7, RK3588_GMAC_RXCLK_DLY_DISABLE (Id));
  }
}

STATIC
VOID
GmacSetToRmii (
  IN UINT32  Id
  )
{
  MmioWrite32 (RK3588_PHP_GRF_GMAC_CON0, RK3588_GMAC_PHY_INTF_SEL_RMII (Id));

  MmioWrite32 (RK3588_PHP_GRF_CLK_CON1, RK3588_GMAC_CLK_RMII_MODE (Id));
}

STATIC
VOID
GmacSetClockSelectFromIo (
  IN UINT32   Id,
  IN BOOLEAN  Enable
  )
{
  UINT32  Value;

  Value = Enable ? RK3588_GMAC_CLK_SELECT_IO (Id)
                 : RK3588_GMAC_CLK_SELECT_CRU (Id);

  MmioWrite32 (RK3588_PHP_GRF_CLK_CON1, Value);
}

STATIC
EFI_STATUS
EFIAPI
GmacSetTxClockSpeed (
  IN UINT32   Id,
  IN BOOLEAN  RgmiiMode,
  IN UINT32   Speed
  )
{
  UINT32  Value;

  switch (Speed) {
    case 10:
      if (RgmiiMode) {
        Value = RK3588_GMAC_CLK_SEL_RGMII_2_5 (Id);
      } else {
        Value = RK3588_GMAC_CLK_SEL_RMII_2_5 (Id);
      }

      break;

    case 100:
      if (RgmiiMode) {
        Value = RK3588_GMAC_CLK_SEL_RGMII_25 (Id);
      } else {
        Value = RK3588_GMAC_CLK_SEL_RMII_25 (Id);
      }

      break;

    case 1000:
      if (RgmiiMode) {
        Value = RK3588_GMAC_CLK_SEL_RGMII_125 (Id);
      } else {
        ASSERT (FALSE);
        return EFI_UNSUPPORTED;
      }

      break;

    default:
      ASSERT (FALSE);
      return EFI_UNSUPPORTED;
  }

  MmioWrite32 (RK3588_PHP_GRF_CLK_CON1, Value);

  return EFI_SUCCESS;
}

STATIC
VOID
EFIAPI
GmacPlatformGetConfig (
  IN DWC_EQOS_PLATFORM_DEVICE_PROTOCOL  *This,
  IN DWC_EQOS_CONFIG                    *Config
  )
{
  Config->CsrClockRate  = 125000000;
  Config->AxiBusWidth   = EqosAxiBusWidth64;
  Config->AxiFixedBurst = FALSE;
  Config->AxiMixedBurst = TRUE;
  Config->AxiWrOsrLmt   = 4;
  Config->AxiRdOsrLmt   = 8;
  Config->AxiBlen       = EqosAxiBlen16 | EqosAxiBlen8 | EqosAxiBlen4;
}

STATIC
EFI_STATUS
EFIAPI
GmacPlatformSetInterfaceSpeed (
  IN DWC_EQOS_PLATFORM_DEVICE_PROTOCOL  *This,
  IN UINT32                             Speed
  )
{
  GMAC_DEVICE  *Gmac = GMAC_DEVICE_FROM_EQOS_PLATFORM (This);

  return GmacSetTxClockSpeed (Gmac->Id, TRUE, Speed);
}

STATIC
VOID
GmacGetOtpMacAddress (
  OUT EFI_MAC_ADDRESS  *MacAddress
  )
{
  UINT8  OtpData[32];
  UINT8  Hash[SHA256_DIGEST_SIZE];

  /* Generate MAC addresses from the first 32 bytes in the OTP */
  OtpRead (0x00, sizeof (OtpData), OtpData);
  Sha256HashAll (OtpData, sizeof (OtpData), Hash);

  /* Clear multicast bit, set locally administered bit. */
  Hash[0] &= 0xFE;
  Hash[0] |= 0x02;

  /* ... and for compatibility with old drivers (see https://github.com/jaredmcneill/quartz64_uefi/pull/68) */
  Hash[3] &= 0xFE;
  Hash[3] |= 0x02;

  ZeroMem (MacAddress, sizeof (EFI_MAC_ADDRESS));
  CopyMem (MacAddress, Hash, NET_ETHER_ADDR_LEN);
}

EFI_STATUS
EFIAPI
GmacPlatformDxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS       Status;
  EFI_MAC_ADDRESS  MacAddress;
  UINT32           Index;
  GMAC_DEVICE      *Gmac;
  EFI_HANDLE       Handle;

  GmacGetOtpMacAddress (&MacAddress);

  for (Index = 0; Index < ARRAY_SIZE (mGmacDevices); Index++) {
    Gmac = &mGmacDevices[Index];
    if (!Gmac->Supported) {
      continue;
    }

    /* Configure pins */
    GmacIomux (Gmac->Id);

    /* Setup clocks and delays */
    GmacSetClockSelectFromIo (Gmac->Id, FALSE);
    GmacSetToRgmii (Gmac->Id, Gmac->TxDelay, Gmac->RxDelay);

    /* Reset PHY */
    GmacIoPhyReset (Gmac->Id, TRUE);
    MicroSecondDelay (20000);
    GmacIoPhyReset (Gmac->Id, FALSE);
    MicroSecondDelay (200000);

    PhyInit (Gmac->BaseAddress);

    Gmac->EqosPlatform.BaseAddress       = Gmac->BaseAddress;
    Gmac->EqosPlatform.GetConfig         = GmacPlatformGetConfig;
    Gmac->EqosPlatform.SetInterfaceSpeed = GmacPlatformSetInterfaceSpeed;

    CopyMem (&Gmac->EqosPlatform.MacAddress, &MacAddress, NET_ETHER_ADDR_LEN);
    Gmac->EqosPlatform.MacAddress.Addr[5] += Gmac->Id;

    DEBUG ((
      DEBUG_INFO,
      "%a: GMAC%u MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n",
      __func__,
      Gmac->Id,
      Gmac->EqosPlatform.MacAddress.Addr[0],
      Gmac->EqosPlatform.MacAddress.Addr[1],
      Gmac->EqosPlatform.MacAddress.Addr[2],
      Gmac->EqosPlatform.MacAddress.Addr[3],
      Gmac->EqosPlatform.MacAddress.Addr[4],
      Gmac->EqosPlatform.MacAddress.Addr[5]
      ));

    CopyMem (&Gmac->DevicePath, &mGmacDevicePathTemplate, sizeof (GMAC_DEVICE_PATH));
    CopyMem (&Gmac->DevicePath.MacAddrDP.MacAddress, &Gmac->EqosPlatform.MacAddress, NET_ETHER_ADDR_LEN);
    Gmac->DevicePath.ControllerId = Gmac->Id;

    Handle = NULL;
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &Handle,
                    &gDwcEqosPlatformDeviceProtocolGuid,
                    &Gmac->EqosPlatform,
                    &gEfiDevicePathProtocolGuid,
                    &Gmac->DevicePath,
                    NULL
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_ERROR,
        "%a: Failed to install GMAC%u EQOS device. Status=%r",
        __func__,
        Gmac->Id,
        Status
        ));
    }
  }

  return EFI_SUCCESS;
}
