/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#include <Library/ArmLib.h>
#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>

#define MPP_PIN_VAL(pin,func)    (((func) & 0xf) << ((pin) * 4))
#define MPP_MAX_REGS             8
#define MPP_PINS_PER_REG         8
#define PCD_PINS_PER_GROUP       10

#define SD_MMC_PHY_AP_MPP_OFFSET   0x100
#define SD_MMC_PHY_CP0_MPP_OFFSET  0x424
#define MPP_ON_SDPHY_ENABLE        (1 << 0)

#define MAX_CHIPS                4

#define GET_PCD_PTR(id,num)      PcdGetPtr(PcdChip##id##MppSel##num)
#define GET_PIN_COUNT(id)        PcdGet32(PcdChip##id##MppPinCount)
#define GET_BASE(id)             PcdGet64(PcdChip##id##MppBaseAddress)
#define GET_REV_FLAG(id)         PcdGetBool(PcdChip##id##MppReverseFlag)

/* We get chip number */
#define GetMppPcd(id) {                 \
  PinCount[id] = GET_PIN_COUNT(id);     \
  MppRegPcd[id][7] = GET_PCD_PTR(id,7); \
  MppRegPcd[id][6] = GET_PCD_PTR(id,6); \
  MppRegPcd[id][5] = GET_PCD_PTR(id,5); \
  MppRegPcd[id][4] = GET_PCD_PTR(id,4); \
  MppRegPcd[id][3] = GET_PCD_PTR(id,3); \
  MppRegPcd[id][2] = GET_PCD_PTR(id,2); \
  MppRegPcd[id][1] = GET_PCD_PTR(id,1); \
  MppRegPcd[id][0] = GET_PCD_PTR(id,0); \
  BaseAddr[id] = GET_BASE(id);          \
  ReverseFlag[id] = GET_REV_FLAG(id);   \
}

STATIC
VOID
SetRegisterValue (
  UINT8 RegCount,
  UINT8 MppRegPcd[][MPP_PINS_PER_REG],
  UINTN BaseAddr,
  BOOLEAN ReverseFlag
  )
{
  UINT32 i, j, CtrlVal, CtrlMask, PinIndex;
  INTN Sign;

  Sign = ReverseFlag ? -1 : 1;

  for (i = 0; i < RegCount; i++) {
    CtrlVal = 0;
    CtrlMask = 0;
    for (j = 0; j < MPP_PINS_PER_REG; j++) {

      PinIndex = 7 * (UINTN)ReverseFlag + j * Sign;

      if (MppRegPcd[i][PinIndex] != 0xff) {
        CtrlVal |= MPP_PIN_VAL(PinIndex, MppRegPcd[i][PinIndex]);
        CtrlMask |= MPP_PIN_VAL(PinIndex, 0xf);
      }
    }
    MmioAndThenOr32 (BaseAddr + 4 * i * Sign, ~CtrlMask, CtrlVal);
  }
}

STATIC
/* Transform PCD MPP group format into hardware register format */
UINT8
PcdToMppRegs (
  UINTN PinCount,
  UINT8 **MppRegPcd,
  UINT8 MppRegPcdTmp[][MPP_PINS_PER_REG]
  )
{
  UINT8 PcdGroupCount, MppRegCount;
  UINTN i, j, k, l;

  if (PinCount == 0) {
    return 0;
  }

  PcdGroupCount = PinCount / PCD_PINS_PER_GROUP;
  if ((PinCount % PCD_PINS_PER_GROUP) != 0) {
    PcdGroupCount += 1;
  }

  MppRegCount = PinCount / MPP_PINS_PER_REG;
  if ((PinCount % MPP_PINS_PER_REG) != 0) {
    MppRegCount += 1;
  }

  /* Fill temporary table with data from PCD groups in HW format */
  for (i = 0; i < PcdGroupCount; i++) {
    for (j = 0; j < PCD_PINS_PER_GROUP; j++) {
      k = (PCD_PINS_PER_GROUP * i + j) / MPP_PINS_PER_REG;
      l = (PCD_PINS_PER_GROUP * i + j) % MPP_PINS_PER_REG;
      MppRegPcdTmp[k][l] = (UINT8)MppRegPcd[i][j];
    }
  }

  return MppRegCount;
}

STATIC
VOID
SetSdMmcPhyMpp (
  UINTN  BaseAddr,
  UINT32 Index
  )
{
  UINTN  Size, Offset;
  UINT8 *Ptr;
  UINT32 Reg;

  Size = PcdGetSize(PcdPciESdhci);
  Ptr = (UINT8 *) PcdGetPtr(PcdPciESdhci);

  if (Ptr == NULL || Index >= Size) {
    return;
  }

  /* Check if SDHCI controller is enabled on the HW block */
  if (Ptr[Index] != 1) {
    return;
  }

  /* Choose adequate Offset */
  switch (Index) {
  case 0:
    Offset = SD_MMC_PHY_AP_MPP_OFFSET;
    break;
  default:
    Offset = SD_MMC_PHY_CP0_MPP_OFFSET;
    break;
  }

  /*
   * If there is SDHCI controller on platform, connect SD/MMC PHY to
   * SD/MMC controller insted of using it as MPP multiplexer
   */
  Reg = MmioRead32 (BaseAddr + Offset);
  Reg &= ~MPP_ON_SDPHY_ENABLE;
  MmioWrite32 (BaseAddr + Offset, Reg);
}

EFI_STATUS
MppInitialize (
  )
{
  UINTN BaseAddr[MAX_CHIPS], PinCount[MAX_CHIPS], RegCount;
  BOOLEAN ReverseFlag[MAX_CHIPS];
  UINT8 *MppRegPcd[MAX_CHIPS][MPP_MAX_REGS];
  UINT32 i, ChipCount;
  UINT8 TmpMppValue[MPP_MAX_REGS][MPP_PINS_PER_REG];

  ChipCount = PcdGet32 (PcdMppChipCount);

  /* Read all needed PCD for MPP configuration */
  GetMppPcd(0);
  GetMppPcd(1);
  GetMppPcd(2);
  GetMppPcd(3);

  for (i = 0; i < MAX_CHIPS; i++) {
    if (i == ChipCount)
      break;

    RegCount = PcdToMppRegs (PinCount[i], MppRegPcd[i], TmpMppValue);
    SetRegisterValue (RegCount, TmpMppValue, BaseAddr[i], ReverseFlag[i]);

    /*
     * eMMC PHY IP has its own MPP configuration.
     */
    SetSdMmcPhyMpp (BaseAddr[i], i);
  }

  return EFI_SUCCESS;
}
