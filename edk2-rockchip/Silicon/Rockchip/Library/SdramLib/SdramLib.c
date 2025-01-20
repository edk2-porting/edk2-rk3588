/** @file
 *
 *  SDRAM size detection for Rockchip SoCs
 *
 *  Copyright (c) 2022, Jared McNeill <jmcneill@invisible.ca>
 *  Copyright (c) 2023, Gábor Stefanik <netrolller.3d@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/SdramLib.h>

// TODO convert these to PCDs
#define SDRAM_OS_REG_BASE  0xFD58A208
#define SDRAM_BANK_COUNT   2

typedef enum {
  SDRAM_DDR4    = 0,
  SDRAM_DDR2    = 2,
  SDRAM_DDR3    = 3,
  SDRAM_LPDDR2  = 5,
  SDRAM_LPDDR3  = 6,
  SDRAM_LPDDR4  = 7,
  SDRAM_LPDDR4X = 8,
  SDRAM_LPDDR5  = 9,
  SDRAM_DDR5    = 10
} SDRAM_DDRTYPE;

#define SYS_REG_DDRTYPE(x)           (((x) >> 13) & 0x7)
#define SYS_REG_CHANNELNUM(x)        (((x) >> 12) & 0x1)
#define SYS_REG_RANK_CH(x, c)        (((x) >> ((c) ? 27 : 11)) & 0x1)
#define SYS_REG_COL_CH(x, c)         (((x) >> ((c) ? 25 : 9)) & 0x3)
#define SYS_REG_BK_CH(x, c)          (((x) >> ((c) ? 24 : 8)) & 0x1)
#define SYS_REG_CS0_ROW_CH_LO(x, c)  (((x) >> ((c) ? 22 : 6)) & 0x3)
#define SYS_REG_CS1_ROW_CH_LO(x, c)  (((x) >> ((c) ? 20 : 4)) & 0x3)
#define SYS_REG_BW_CH(x, c)          (((x) >> ((c) ? 18 : 2)) & 0x3)
#define SYS_REG_ROW34_CH(x, c)       (((x) >> ((c) ? 31 : 30)) & 0x1)
#define SYS_REG_DBW_CH(x, c)         (((x) >> ((c) ? 16 : 0)) & 0x3)

#define SYS_REG1_VERSION(x)           (((x) >> 28) & 0xF)
#define SYS_REG1_CS0_ROW_CH_HI(x, c)  (((x) >> ((c) ? 7 : 5)) & 0x1)
#define SYS_REG1_CS1_ROW_CH_HI(x, c)  (((x) >> ((c) ? 6 : 4)) & 0x1)
#define SYS_REG1_CS1_COL_CH(x, c)     (((x) >> ((c) ? 2 : 0)) & 0x3)
#define SYS_REG1_EXTENDED_DDRTYPE(x)  (((x) >> 12) & 0x3)

UINT64
SdramGetMemorySize (
  VOID
  )
{
  UINT32  OsReg;
  UINT32  OsReg1;
  INT32   Bank;
  INT32   ChNum;
  INT32   Ch;
  INT32   Rank;
  INT32   Cs0Col, Cs1Col;
  INT32   Cs0Row, Cs1Row;
  INT32   Bk;
  INT32   Bw;
  INT32   Row34;
  INT32   Bg;
  INT32   ChSizeMb;
  INT32   SizeMb = 0;
  UINT32  Version;
  UINT32  DdrType;

  for (Bank = 0; Bank < SDRAM_BANK_COUNT; Bank++) {
    OsReg  = MmioRead32 (SDRAM_OS_REG_BASE + 8 * Bank);
    OsReg1 = MmioRead32 (SDRAM_OS_REG_BASE + 8 * Bank + 4);

    Version = SYS_REG1_VERSION (OsReg1);

    DdrType = SYS_REG_DDRTYPE (OsReg);
    if (Version >= 3) {
      DdrType |= SYS_REG1_EXTENDED_DDRTYPE (OsReg1) << 3;
    }

    ChNum = 1 + SYS_REG_CHANNELNUM (OsReg);

    DEBUG ((
      DEBUG_INFO,
      "%a(): Bank #%d: %d channel(s), type 0x%X, version 0x%X\n",
      __func__,
      Bank,
      ChNum,
      SYS_REG_DDRTYPE (OsReg),
      Version
      ));

    for (Ch = 0; Ch < ChNum; Ch++) {
      Rank   = 1 + SYS_REG_RANK_CH (OsReg, Ch);
      Cs0Col = 9 + SYS_REG_COL_CH (OsReg, Ch);
      Cs1Col = Cs0Col;

      if (DdrType == SDRAM_LPDDR5) {
        Bk = 3 + SYS_REG_BK_CH (OsReg, Ch);
      } else {
        Bk = 3 - SYS_REG_BK_CH (OsReg, Ch);
      }

      if (Version >= 0x2) {
        Cs1Col = 9 + SYS_REG1_CS1_COL_CH (OsReg1, Ch);
        if (((SYS_REG1_CS0_ROW_CH_HI (OsReg1, Ch) << 2) +
             SYS_REG_CS0_ROW_CH_LO (OsReg, Ch)) == 7)
        {
          Cs0Row = 12;
        } else {
          Cs0Row = 13 + (SYS_REG1_CS0_ROW_CH_HI (OsReg1, Ch) << 2) +
                   SYS_REG_CS0_ROW_CH_LO (OsReg, Ch);
        }

        if (((SYS_REG1_CS1_ROW_CH_HI (OsReg1, Ch) << 2) +
             SYS_REG_CS1_ROW_CH_LO (OsReg, Ch)) == 7)
        {
          Cs1Row = 12;
        } else {
          Cs1Row = 13 + (SYS_REG1_CS1_ROW_CH_HI (OsReg1, Ch) << 2) +
                   SYS_REG_CS1_ROW_CH_LO (OsReg, Ch);
        }
      } else {
        Cs0Row = 13 + SYS_REG_CS0_ROW_CH_LO (OsReg, Ch);
        Cs1Row = 13 + SYS_REG_CS1_ROW_CH_LO (OsReg, Ch);
      }

      Bw    = 2 >> SYS_REG_BW_CH (OsReg, Ch);
      Row34 = SYS_REG_ROW34_CH (OsReg, Ch);
      if ((DdrType == SDRAM_DDR4) && (Version != 0x3)) {
        Bg = SYS_REG_DBW_CH (OsReg, Ch) == 2 ? 2 : 1;
      } else {
        Bg = 0;
      }

      ChSizeMb = 1 << (Cs0Row + Cs0Col + Bk + Bg + Bw - 20);
      if (Rank > 1) {
        ChSizeMb += ChSizeMb >> ((Cs0Row - Cs1Row) + (Cs0Col - Cs1Col));
      }

      if (Row34) {
        ChSizeMb = ChSizeMb * 3 / 4;
      }

      DEBUG ((DEBUG_INFO, "%a(): Ch #%d: %u MB\n", __func__, Ch + Bank * 2, ChSizeMb));
      SizeMb += ChSizeMb;
    }
  }

  DEBUG ((DEBUG_INFO, "%a(): Detected %u MB RAM\n", __func__, SizeMb));

  ASSERT (SizeMb != 0);

  return (UINT64)SizeMb * 1024 * 1024;
}
