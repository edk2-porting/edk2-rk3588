/** @file
  Platform Hooks file

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Include/SioRegs.h>

UINT8
IsSimPlatform (VOID)
{
  return 0;
}

/**

    Read Aspeed AHB register.

    @param RegIndex: register index of Aspeed.

    @retval value of register.

**/
UINT32
ReadAHBDword (
  UINT32 RegIndex
  )
{
  UINT8    bValue;
  UINT32   rdValue = 0;

  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, REG_LOGICAL_DEVICE);
  IoWrite8 (0xED, 0);//short delay.
  IoWrite8 (ASPEED2500_SIO_DATA_PORT, ASPEED2500_SIO_SMI);
  IoWrite8 (0xED, 0);//short delay.

  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0x30);
  IoWrite8 (0xED, 0);//short delay.
  IoWrite8 (ASPEED2500_SIO_DATA_PORT, 1);
  IoWrite8 (0xED, 0);//short delay.

  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0xf8);
  bValue = IoRead8(ASPEED2500_SIO_DATA_PORT);
  bValue &= 0xfc;
  bValue |= 2;  // 4 byte window.
  IoWrite8 (ASPEED2500_SIO_DATA_PORT, bValue);
  IoWrite8 (0xED, 0);//short delay.

  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0xf0);
  IoWrite8 (0xED, 0);//short delay.
  IoWrite8 (ASPEED2500_SIO_DATA_PORT,  (UINT8)((RegIndex >> 24)& 0xff));

  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0xf1);
  IoWrite8 (0xED, 0);//short delay.
  IoWrite8 (ASPEED2500_SIO_DATA_PORT,  (UINT8)((RegIndex >> 16)& 0xff));

  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0xf2);
  IoWrite8 (0xED, 0);//short delay.
  IoWrite8 (ASPEED2500_SIO_DATA_PORT,  (UINT8)((RegIndex >> 8) & 0xff));

  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0xf3);
  IoWrite8 (0xED, 0);//short delay.
  IoWrite8 (ASPEED2500_SIO_DATA_PORT,  (UINT8)((RegIndex )& 0xff));

  // trigger read
  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0xfe);
  IoRead8 (ASPEED2500_SIO_DATA_PORT);


  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0xf4);
  rdValue += IoRead8 (ASPEED2500_SIO_DATA_PORT);
  rdValue <<= 8;

  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0xf5);
  rdValue += IoRead8 (ASPEED2500_SIO_DATA_PORT);
  rdValue <<= 8;

  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0xf6);
  rdValue += IoRead8 (ASPEED2500_SIO_DATA_PORT);
  rdValue <<= 8;

  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0xf7);
  rdValue += IoRead8 (ASPEED2500_SIO_DATA_PORT);


  return rdValue;

}

/**
 * Checks for the presence of ASPEED SIO
 * @return TRUE if its present. FALSE if not.
 */
BOOLEAN
IsAspeedPresent (
  VOID
 )
{
 BOOLEAN PresenceStatus = FALSE;
 UINT32 DeviceID;

 //
 //ASPEED AST2500/AST2600
 //
 IoWrite8 (ASPEED2500_SIO_INDEX_PORT, ASPEED2500_SIO_UNLOCK);
 IoWrite8 (ASPEED2500_SIO_INDEX_PORT, ASPEED2500_SIO_UNLOCK);
 IoWrite8 (ASPEED2500_SIO_INDEX_PORT, REG_LOGICAL_DEVICE);
 IoWrite8 (ASPEED2500_SIO_DATA_PORT, ASPEED2500_SIO_UART1);
 if (IoRead8 (ASPEED2500_SIO_DATA_PORT) == ASPEED2500_SIO_UART1) {
  //
  //right now, maybe it is ASPEED. to detect the  device ID.
  //
  DeviceID = ReadAHBDword (SCU7C);
  //
  //There is a Aspeed card need to support as well. it's type is AST2500 A1 EVB.
  //
  //AST2300-A0 0x01000003
  //AST2300-A1 0x01010303
  //AST1300-A1 0x01010003
  //AST1050-A1 0x01010203
  //AST2400-A0 0x02000303
  //AST2400-A1 0x02010303
  //AST1400-A1 0x02010103
  //AST1250-A1 0x02010303
  //AST2500-A0 0x04000303
  //AST2510-A0 0x04000103
  //AST2520-A0 0x04000203
  //AST2530-A0 0x04000403
  //AST2500-A1 0x04010303
  //AST2510-A1 0x04010103
  //AST2520-A1 0x04010203
  //AST2530-A1 0x04010403
  //
  if ((DeviceID & 0xff0000ff) == 0x04000003) {
   PresenceStatus = TRUE;
  }
 }
 IoWrite8 (ASPEED2500_SIO_INDEX_PORT, ASPEED2500_SIO_LOCK);
 return PresenceStatus;
}

/**
 * Checks for the presence of Nuvoton SIO
 * @return TRUE if its present. FALSE if not.
 */
BOOLEAN
IsNuvotonPresent (
  VOID
  )
{
  BOOLEAN PresenceStatus = FALSE;

  //
  // Nuvoton NCT5104D
  //
  IoWrite8 (NCT5104D_SIO_INDEX_PORT, NCT5104D_ENTER_THE_EXTENDED_FUNCTION_MODE);
  IoWrite8 (NCT5104D_SIO_INDEX_PORT, NCT5104D_ENTER_THE_EXTENDED_FUNCTION_MODE);
  IoWrite8 (NCT5104D_SIO_INDEX_PORT, NCT5104D_CHIP_ID_REG);
  if (IoRead8 (NCT5104D_SIO_DATA_PORT) == NCT5104D_CHIP_ID) {
    PresenceStatus = TRUE;
  }
  return PresenceStatus;
}

/**
 * Checks for the presence of the following SIO:
 *  -ASPEED AST2500
 *  -Nuvoton NCT5104D
 *
 * @return An UINT32 with the corresponding bit set for each SIO.
 *         -ASPEED_EXIST     BIT4
 *         -NCT5104D_EXIST   BIT3
 *         -PC8374_EXIST     BIT1
 *         -PILOTIV_EXIST    BIT0
 */
UINT32
IsSioExist (
  VOID
  )
{
  UINT32   SioExit = 0;

  if (IsAspeedPresent ()) {
    SioExit |= ASPEED_EXIST;
  }

  if (IsNuvotonPresent ()) {
    SioExit |= NCT5104D_EXIST;
  }

  DEBUG((DEBUG_INFO, "[SIO] Current system SIO exist bit:%x \n", SioExit));

  return SioExit;
}
