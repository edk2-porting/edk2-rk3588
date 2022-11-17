/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2017, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Uefi.h>
#include <Library/CpldIoLib.h>
#include <Library/DebugLib.h>
#include <Library/I2CLib.h>
#include <Library/IoLib.h>
#include <Library/OemNicLib.h>

#define CPU2_SFP2_100G_CARD_OFFSET   0x25

#define SOCKET1_NET_PORT_100G         1
#define SOCKET0_NET_PORT_NUM          4
#define SOCKET1_NET_PORT_NUM          2

#define CARD_PRESENT_100G               (BIT7)
#define EEPROM_I2C_PORT          4
#define EEPROM_PAGE_SIZE         0x40
#define MAC_ADDR_LEN             6
#define I2C_OFFSET_EEPROM_ETH0   (0xc00)
#define I2C_SLAVEADDR_EEPROM     (0x52)

#define SRAM_NIC_NCL1_OFFSET_ADDRESS   0xA0E87FE0
#define SRAM_NIC_NCL2_OFFSET_ADDRESS   0xA0E87FE4

#pragma pack(1)
typedef struct {
  UINT16 Crc16;
  UINT16 MacLen;
  UINT8  Mac[MAC_ADDR_LEN];
} NIC_MAC_ADDRESS;
#pragma pack()

ETH_PRODUCT_DESC gEthPdtDesc[ETH_MAX_PORT] =
{
    {TRUE,   ETH_SPEED_10KM,  ETH_FULL_DUPLEX, ETH_INVALID, ETH_INVALID},
    {TRUE,   ETH_SPEED_10KM,  ETH_FULL_DUPLEX, ETH_INVALID, ETH_INVALID},
    {FALSE,  ETH_INVALID,     ETH_INVALID,     ETH_INVALID, ETH_INVALID},
    {FALSE,  ETH_INVALID,     ETH_INVALID,     ETH_INVALID, ETH_INVALID},
    {TRUE,   ETH_SPEED_1000M, ETH_FULL_DUPLEX, ETH_PHY_MVL88E1512_ID, 0},
    {TRUE,   ETH_SPEED_1000M, ETH_FULL_DUPLEX, ETH_PHY_MVL88E1512_ID, 1},
    {FALSE,  ETH_INVALID,     ETH_INVALID,     ETH_INVALID, ETH_INVALID},
    {FALSE,  ETH_INVALID,     ETH_INVALID,     ETH_INVALID, ETH_INVALID}
};

UINT16 CrcTable16[256] = {
  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
  0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
  0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
  0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
  0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
  0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
  0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
  0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
  0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
  0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
  0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
  0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
  0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
  0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
  0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
  0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
  0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
  0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
  0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
  0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
  0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
  0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
  0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
  0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
  0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
  0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
  0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
  0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
  0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
  0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
  0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
  0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0,
};

UINT16 MakeCrcCheckSum (
  UINT8 *Buffer,
  UINT32 Length
  )
{
  UINT16 StartCRC = 0;

  if (Length > SIZE_512KB) {
    return 0;
  }

  if (Buffer == NULL) {
    return 0;
  }

  while (Length) {
    StartCRC = CrcTable16 [((UINT8) ((StartCRC >> 8) & 0xff)) ^ *(Buffer++)] ^
               ((UINT16) (StartCRC << 8));
    Length--;
  }

  return StartCRC;
}


EFI_STATUS
OemGetMacE2prom(
  IN  UINT32 Port,
  OUT UINT8  *Addr
  )
{
  I2C_DEVICE       I2cDev = {0};
  EFI_STATUS       Status;
  UINT16           I2cOffset;
  UINT16           Crc16;
  NIC_MAC_ADDRESS  MacDesc = {0};
  UINT16           RemainderMacOffset;
  UINT16           LessSizeOfPage;
  UINT32           I = 0;

  Status = I2CInit (0, EEPROM_I2C_PORT, Normal);
  if (EFI_ERROR (Status))
  {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Call I2CInit failed! p1=0x%x.\n",
            __FUNCTION__, __LINE__, Status));
    return Status;
  }

  I2cOffset = I2C_OFFSET_EEPROM_ETH0 + (Port * sizeof (NIC_MAC_ADDRESS));

  I2cDev.DeviceType = DEVICE_TYPE_E2PROM;
  I2cDev.Port = EEPROM_I2C_PORT;
  I2cDev.SlaveDeviceAddress = I2C_SLAVEADDR_EEPROM;
  I2cDev.Socket = 0;
  RemainderMacOffset = I2cOffset % EEPROM_PAGE_SIZE;
  LessSizeOfPage = EEPROM_PAGE_SIZE - RemainderMacOffset;
  //The length of NIC_MAC_ADDRESS is 10 bytes long,
  //It surly less than EEPROM page size, so we could
  //code as below, check the address whether across the page boundary,
  //and split the data when across page boundary.
  if (sizeof (NIC_MAC_ADDRESS) <= LessSizeOfPage) {
    Status = I2CRead (&I2cDev, I2cOffset, sizeof (NIC_MAC_ADDRESS), (UINT8 *) &MacDesc);
  } else {
    Status = I2CRead (&I2cDev, I2cOffset, LessSizeOfPage, (UINT8 *) &MacDesc);
    if (!EFI_ERROR (Status)) {
      Status |= I2CRead (
                  &I2cDev,
                  I2cOffset + LessSizeOfPage,
                  sizeof (NIC_MAC_ADDRESS) - LessSizeOfPage,
                  (UINT8 *) &MacDesc + LessSizeOfPage
                  );
    }
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Call I2cRead failed! p1=0x%x.\n",
            __FUNCTION__, __LINE__, Status));
    return Status;
  }

  Crc16 = MakeCrcCheckSum (
            (UINT8 *)&(MacDesc.MacLen),
            sizeof (MacDesc.MacLen) + sizeof (MacDesc.Mac)
            );
  if ((Crc16 != MacDesc.Crc16) || (Crc16 == 0)) {
    return EFI_NOT_FOUND;
  }

  for (I = 0; I < MAC_ADDR_LEN; I++) {
    Addr[I] = MacDesc.Mac[I];
  }

  return EFI_SUCCESS;
}


EFI_STATUS
OemSetMacE2prom (
  IN UINT32 Port,
  IN UINT8 *Addr
  )
{
  I2C_DEVICE       I2cDev = {0};
  EFI_STATUS       Status;
  UINT16           I2cOffset;
  NIC_MAC_ADDRESS  MacDesc = {0};
  UINT32           I;
  UINT16           RemainderMacOffset;
  UINT16           LessSizeOfPage;

  I = 0;
  MacDesc.MacLen = MAC_ADDR_LEN;

  for (I = 0; I < MAC_ADDR_LEN; I++) {
    MacDesc.Mac[I] = Addr[I];
  }

  MacDesc.Crc16 = MakeCrcCheckSum (
                    (UINT8 *)&(MacDesc.MacLen),
                    sizeof (MacDesc.MacLen) + MAC_ADDR_LEN
                    );

  Status = I2CInit (0, EEPROM_I2C_PORT, Normal);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Call I2CInit failed! p1=0x%x.\n",
           __FUNCTION__, __LINE__, Status));
    return Status;
  }

  I2cOffset = I2C_OFFSET_EEPROM_ETH0 + (Port * sizeof (NIC_MAC_ADDRESS));

  I2cDev.DeviceType = DEVICE_TYPE_E2PROM;
  I2cDev.Port = EEPROM_I2C_PORT;
  I2cDev.SlaveDeviceAddress = I2C_SLAVEADDR_EEPROM;
  I2cDev.Socket = 0;
  RemainderMacOffset = I2cOffset % EEPROM_PAGE_SIZE;
  LessSizeOfPage = EEPROM_PAGE_SIZE - RemainderMacOffset;
  //The length of NIC_MAC_ADDRESS is 10 bytes long,
  //It surly less than EEPROM page size, so we could
  //code as below, check the address whether across the page boundary,
  //and split the data when across page boundary.
  if (sizeof (NIC_MAC_ADDRESS) <= LessSizeOfPage) {
    Status = I2CWrite (
               &I2cDev,
               I2cOffset,
               sizeof (NIC_MAC_ADDRESS),
               (UINT8 *) &MacDesc
               );
  } else {
    Status = I2CWrite (&I2cDev, I2cOffset, LessSizeOfPage, (UINT8 *) &MacDesc);
    if (!EFI_ERROR (Status)) {
      Status |= I2CWrite (
                  &I2cDev,
                  I2cOffset + LessSizeOfPage,
                  sizeof (NIC_MAC_ADDRESS) - LessSizeOfPage,
                  (UINT8 *) &MacDesc + LessSizeOfPage
                  );
    }
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Call I2cWrite failed! p1=0x%x.\n",
            __FUNCTION__, __LINE__, Status));
    return Status;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
OemGetMac (
  IN OUT EFI_MAC_ADDRESS *Mac,
  IN     UINTN           Port
  )
{
  EFI_STATUS Status;

  if (Mac == NULL) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Mac buffer is null!\n",
            __FUNCTION__, __LINE__));
    return EFI_INVALID_PARAMETER;
  }

  Status = OemGetMacE2prom (Port, Mac->Addr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "[%a]:[%dL] Cannot get MAC from EEPROM, Status: %r; using default MAC.\n",
      __FUNCTION__, __LINE__, Status));

    Mac->Addr[0] = 0xFF;
    Mac->Addr[1] = 0xFF;
    Mac->Addr[2] = 0xFF;
    Mac->Addr[3] = 0xFF;
    Mac->Addr[4] = 0xFF;
    Mac->Addr[5] = 0xFF;
    return EFI_SUCCESS;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
OemSetMac (
  IN EFI_MAC_ADDRESS *Mac,
  IN UINTN           Port
  )
{
  EFI_STATUS Status;

  if (Mac == NULL) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Mac buffer is null!\n",
            __FUNCTION__, __LINE__));
    return EFI_INVALID_PARAMETER;
  }

  Status = OemSetMacE2prom (Port, Mac->Addr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Set mac failed!\n", __FUNCTION__, __LINE__));
    return Status;
  }

  return EFI_SUCCESS;
}

UINT32
OemEthFindFirstSP (
  VOID
  )
{
  UINT32 I;

  for (I = 0; I < ETH_MAX_PORT; I++) {
    if (gEthPdtDesc[I].Valid == TRUE) {
      return I;
    }
  }

  return ETH_INVALID;
}

ETH_PRODUCT_DESC *
OemEthInit (
  UINT32 port
  )
{
  return (ETH_PRODUCT_DESC *)(&(gEthPdtDesc[port]));
}


BOOLEAN
OemIsInitEth (
  UINT32 Port
  )
{
  return TRUE;
}

EFI_STATUS
ConfigCDR (
  UINT32 Socket
  )
{
  return EFI_SUCCESS;
}

UINT32
OemGetNclConfOffset (
  UINT32 Socket
  )
{
  UINT32           ConfigurationOffset;

  if (Socket == 0) {
    // For 1st socket, the NCL configuration offset is 0
    ConfigurationOffset = 0;
    MmioWrite32 (SRAM_NIC_NCL1_OFFSET_ADDRESS, ConfigurationOffset);
    return ConfigurationOffset;
  }

  // For 2nd Socket
  if ((ReadCpldReg (CPU2_SFP2_100G_CARD_OFFSET) & CARD_PRESENT_100G) != 0) {
    ConfigurationOffset = SIZE_128KB;
  } else {
    ConfigurationOffset = SIZE_64KB;
  }
  MmioWrite32 (SRAM_NIC_NCL2_OFFSET_ADDRESS, ConfigurationOffset);
  return ConfigurationOffset;
}

UINT32
OemGetNetPortNum (
  UINT32 Socket
  )
{
  if (Socket == 0){
    return SOCKET0_NET_PORT_NUM;
  }

  if ((ReadCpldReg (CPU2_SFP2_100G_CARD_OFFSET) & CARD_PRESENT_100G) != 0) {
    return SOCKET1_NET_PORT_100G;
  } else {
    return SOCKET1_NET_PORT_NUM;
  }
}
