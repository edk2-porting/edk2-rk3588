/** @file
  This file contains internal PSF routines for PCH PSF lib usage

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SataLib.h>
#include <Library/BaseMemoryLib.h>
#include <Register/PchRegsPsf.h>
#include <Register/PchRegsPsfCnl.h>
#include <Register/PchRegsPcie.h>
#include "PchPsfPrivateLibInternal.h"

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mPchLpSerialIoI2cPsfRegs[] =
{
  R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_I2C0_REG_BASE,
  R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_I2C1_REG_BASE,
  R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_I2C2_REG_BASE,
  R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_I2C3_REG_BASE,
  R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_I2C4_REG_BASE,
  R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_I2C5_REG_BASE
};

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mPchHSerialIoI2cPsfRegs[] =
{
  R_CNL_PCH_H_PSF3_PCR_T0_SHDW_I2C0_REG_BASE,
  R_CNL_PCH_H_PSF3_PCR_T0_SHDW_I2C1_REG_BASE,
  R_CNL_PCH_H_PSF3_PCR_T0_SHDW_I2C2_REG_BASE,
  R_CNL_PCH_H_PSF3_PCR_T0_SHDW_I2C3_REG_BASE
};

/**
  Return PSF_PORT for SerialIO I2C device

  @param[in] I2cNum  Serial IO I2C device (I2C0, I2C1, ....)

  @retval  PsfPort   PSF PORT structure for SerialIO I2C device
**/
PSF_PORT
PsfSerialIoI2cPort (
  IN UINT32  I2cNum
  )
{
  PSF_PORT PsfPort;

  PsfPort.PsfPid = PID_PSF3;

  if (IsPchLp ()) {
    if (I2cNum < ARRAY_SIZE(mPchLpSerialIoI2cPsfRegs)) {
      PsfPort.RegBase = mPchLpSerialIoI2cPsfRegs[I2cNum];
      return PsfPort;
    }
  } else {
    if (I2cNum < ARRAY_SIZE(mPchHSerialIoI2cPsfRegs)) {
      PsfPort.RegBase = mPchHSerialIoI2cPsfRegs[I2cNum];
      return PsfPort;
    }
  }

  ASSERT(FALSE);
  return PSF_PORT_NULL;
}

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mPchLpSerialIoSpiPsfRegs[] =
{
  R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_SPI0_REG_BASE,
  R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_SPI1_REG_BASE,
  R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_SPI2_REG_BASE
};

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mPchHSerialIoSpiPsfRegs[] =
{
  R_CNL_PCH_H_PSF3_PCR_T0_SHDW_SPI0_REG_BASE,
  R_CNL_PCH_H_PSF3_PCR_T0_SHDW_SPI1_REG_BASE,
  R_CNL_PCH_H_PSF3_PCR_T0_SHDW_SPI2_REG_BASE
};

/**
  Return PSF_PORT for SerialIO SPI device

  @param[in] SpiNum  Serial IO SPI device (SPI0, SPI1, ....)

  @retval  PsfPort   PSF PORT structure for SerialIO SPI device
**/
PSF_PORT
PsfSerialIoSpiPort (
  IN UINT32  SpiNum
  )
{
  PSF_PORT PsfPort;

  PsfPort.PsfPid = PID_PSF3;

  if (IsPchLp ()) {
    if (SpiNum < ARRAY_SIZE(mPchLpSerialIoSpiPsfRegs)) {
      PsfPort.RegBase = mPchLpSerialIoSpiPsfRegs[SpiNum];
      return PsfPort;
    }
  } else {
    if (SpiNum < ARRAY_SIZE(mPchHSerialIoSpiPsfRegs)) {
      PsfPort.RegBase = mPchHSerialIoSpiPsfRegs[SpiNum];
      return PsfPort;
    }
  }

  ASSERT(FALSE);
  return PSF_PORT_NULL;
}

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mPchLpSerialIoUartPsfRegs[] =
{
  R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_UART0_REG_BASE,
  R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_UART1_REG_BASE,
  R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_UART2_REG_BASE
};

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mPchHSerialIoUartPsfRegs[] =
{
  R_CNL_PCH_H_PSF3_PCR_T0_SHDW_UART0_REG_BASE,
  R_CNL_PCH_H_PSF3_PCR_T0_SHDW_UART1_REG_BASE,
  R_CNL_PCH_H_PSF3_PCR_T0_SHDW_UART2_REG_BASE
};

/**
  Return PSF_PORT for SerialIO UART device

  @param[in] UartNum  Serial IO UART device (UART0, UART1, ....)

  @retval  PsfPort    PSF PORT structure for SerialIO UART device
**/
PSF_PORT
PsfSerialIoUartPort (
  IN UINT32  UartNum
  )
{
  PSF_PORT PsfPort;

  PsfPort.PsfPid = PID_PSF3;

  if (IsPchLp ()) {
    if (UartNum < ARRAY_SIZE(mPchLpSerialIoUartPsfRegs)) {
      PsfPort.RegBase = mPchLpSerialIoUartPsfRegs[UartNum];
      return PsfPort;
    }
  } else {
    if (UartNum < ARRAY_SIZE(mPchHSerialIoUartPsfRegs)) {
      PsfPort.RegBase = mPchHSerialIoUartPsfRegs[UartNum];
      return PsfPort;
    }
  }

  ASSERT(FALSE);
  return PSF_PORT_NULL;
}

/**
  Get EOI register data for given PSF ID

  @param[in]  PsfId           PSF ID (1 - PSF1, 2 - PSF2, ...)
  @param[out] EoiTargetBase   EOI Target register
  @param[out] EoiControlBase  EOI Control register

  @retval MaxTargets          Number of supported targets

**/
UINT8
PsfEoiRegData (
  UINT32        PsfId,
  UINT16        *EoiTargetBase,
  UINT16        *EoiControlBase
  )
{
  UINT8  MaxTargets;

  MaxTargets = 0;
  *EoiTargetBase = 0;
  *EoiControlBase = 0;

  switch (PsfId) {
    case 1:
      if (IsPchLp ()) {
        *EoiTargetBase = R_CNL_PCH_LP_PSF1_PCR_PSF_MC_AGENT_MCAST0_TGT0_EOI;
        *EoiControlBase = R_CNL_PCH_LP_PSF1_PCR_PSF_MC_CONTROL_MCAST0_EOI;
        MaxTargets = 17;
      } else {
        *EoiTargetBase = R_CNL_PCH_H_PSF1_PCR_PSF_MC_AGENT_MCAST0_TGT0_EOI;
        *EoiControlBase = R_CNL_PCH_H_PSF1_PCR_PSF_MC_CONTROL_MCAST0_EOI;
        MaxTargets = 7;
      }
      break;

    case 3:
      *EoiTargetBase = R_CNL_PCH_PSF3_PCR_PSF_MC_AGENT_MCAST0_TGT0_EOI;
      *EoiControlBase = R_CNL_PCH_PSF3_PCR_PSF_MC_CONTROL_MCAST0_EOI;
      MaxTargets = 1;
      break;

    case 6:
      if (IsPchH ()) {
        *EoiTargetBase = R_CNL_PCH_H_PSF6_PCR_PSF_MC_AGENT_MCAST0_TGT0_EOI;
        *EoiControlBase = R_CNL_PCH_H_PSF6_PCR_PSF_MC_CONTROL_MCAST0_EOI;
        MaxTargets = 8;
      }
      break;

    case 7:
      if (IsPchH ()) {
        *EoiTargetBase = R_CNL_PCH_H_PSF7_PCR_PSF_MC_AGENT_MCAST0_TGT0_EOI;
        *EoiControlBase = R_CNL_PCH_H_PSF7_PCR_PSF_MC_CONTROL_MCAST0_EOI;
        MaxTargets = 8;
      }
      break;

    case 8:
      if (IsPchH ()) {
        *EoiTargetBase = R_CNL_PCH_H_PSF8_PCR_PSF_MC_AGENT_MCAST0_TGT0_EOI;
        *EoiControlBase = R_CNL_PCH_H_PSF8_PCR_PSF_MC_CONTROL_MCAST0_EOI;
        MaxTargets = 8;
      }
      break;
  }
  return MaxTargets;
}

GLOBAL_REMOVE_IF_UNREFERENCED PSF_PORT_DEST_ID PchLpRpDestId[] =
{
  {0x18000}, {0x18001}, {0x18002}, {0x18003}, // SPA: PSF1, PortID = 0
  {0x18200}, {0x18201}, {0x18202}, {0x18203}, // SPB: PSF1, PortID = 2
  {0x18400}, {0x18401}, {0x18402}, {0x18403}, // SPC: PSF1, PortID = 4
  {0x18600}, {0x18601}, {0x18602}, {0x18603}  // SPD: PSF1, PortID = 6
};

GLOBAL_REMOVE_IF_UNREFERENCED PSF_PORT_DEST_ID PchHRpDestId[] =
{
  {0x68000}, {0x68001}, {0x68002}, {0x68003}, // SPA: PSF6, PortID = 0
  {0x88000}, {0x88001}, {0x88002}, {0x88003}, // SPB: PSF8, PortID = 0
  {0x68100}, {0x68101}, {0x68102}, {0x68103}, // SPC: PSF6, PortID = 1
  {0x78000}, {0x78001}, {0x78002}, {0x78003}, // SPD: PSF7, PortID = 0
  {0x78100}, {0x78101}, {0x78102}, {0x78103}, // SPE: PSF7, PortID = 1
  {0x88100}, {0x88101}, {0x88102}, {0x88103}  // SPF: PSF8, PortID = 1
};

/**
  PCIe PSF port destination ID (psf_id:port_group_id:port_id:channel_id)

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval Destination ID
**/
PSF_PORT_DEST_ID
PsfPcieDestinationId (
  IN UINT32  RpIndex
  )
{
  if (IsPchLp ()) {
    if (RpIndex < ARRAY_SIZE(PchLpRpDestId)) {
      return PchLpRpDestId[RpIndex];
    }
  } else {
    if (RpIndex < ARRAY_SIZE(PchHRpDestId)) {
      return PchHRpDestId[RpIndex];
    }
  }
  ASSERT (FALSE);
  return (PSF_PORT_DEST_ID){0};
}


/**
  Return PSF_PORT for PMC device

  @retval    PsfPort         PSF PORT structure for PMC device
**/
PSF_PORT
PsfPmcPort (
  VOID
  )
{
  PSF_PORT PsfPort;

  PsfPort.PsfPid = PID_PSF3;

  if (IsPchLp ()) {
    PsfPort.RegBase = R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_PMC_REG_BASE;
  } else {
    PsfPort.RegBase = R_CNL_PCH_H_PSF3_PCR_T0_SHDW_PMC_REG_BASE;
  }
  return PsfPort;
}

GLOBAL_REMOVE_IF_UNREFERENCED PSF_SEGMENT mPchLpPsfTable[] =
{
  {1, PID_PSF1},
  {2, PID_PSF2},
  {3, PID_PSF3},
  {4, PID_PSF4},
  {5, PID_CSME_PSF}
};

GLOBAL_REMOVE_IF_UNREFERENCED PSF_SEGMENT mPchHPsfTable[]  =
{
  {1, PID_PSF1},
  {2, PID_PSF2},
  {3, PID_PSF3},
  {4, PID_PSF4},
  {5, PID_CSME_PSF},
  {6, PID_PSF6},
  {7, PID_PSF7},
  {8, PID_PSF8}
};

/**
  Get list of supported PSF segments.

  @param[out] PsfTable        Array of supported PSF segments
  @param[out] PsfTableLength  Length of PsfTable
**/
VOID
PsfSegments (
  OUT PSF_SEGMENT  **PsfTable,
  OUT UINT32       *PsfTableLength
  )
{
  if (IsPchLp ()) {
    *PsfTable = mPchLpPsfTable;
    *PsfTableLength = ARRAY_SIZE(mPchLpPsfTable);
  } else {
    *PsfTable = mPchHPsfTable;
    *PsfTableLength = ARRAY_SIZE(mPchHPsfTable);
  }
}
