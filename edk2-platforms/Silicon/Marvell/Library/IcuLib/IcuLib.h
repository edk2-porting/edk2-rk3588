/**
*
*  Copyright (C) 2018, Marvell International Ltd. and its affiliates.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
*  Glossary - abbreviations used in Marvell SampleAtReset library implementation:
*  ICU - Interrupt Consolidation Unit
*  AP - Application Processor hardware block (Armada 7k8k incorporates AP806)
*  CP - South Bridge hardware blocks (Armada 7k8k incorporates CP110)
*
**/

#include <Uefi.h>

#include <Library/ArmadaIcuLib.h>
#include <Library/ArmadaSoCDescLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>

#define ICU_REG_BASE(CpIndex)   (ArmadaSoCDescCpBaseGet (CpIndex) + 0x1E0000)

#define ICU_GROUP_REGISTER_BASE_OFFSET    0x10
#define ICU_SET_SPI_AL(x)       (0x10 + (ICU_GROUP_REGISTER_BASE_OFFSET * x))
#define ICU_SET_SPI_AH(x)       (0x14 + (ICU_GROUP_REGISTER_BASE_OFFSET * x))
#define ICU_CLR_SPI_AL(x)       (0x18 + (ICU_GROUP_REGISTER_BASE_OFFSET * x))
#define ICU_CLR_SPI_AH(x)       (0x1c + (ICU_GROUP_REGISTER_BASE_OFFSET * x))
#define ICU_INT_CFG(x)          (0x100 + (sizeof (UINT32) * x))

#define ICU_INT_ENABLE_OFFSET    24
#define ICU_IS_EDGE_OFFSET       28
#define ICU_GROUP_OFFSET         29

#define ICU_MAX_SUPPORTED_UNITS  3
#define ICU_SPI_OFFSET(CpIndex)  ((CpIndex) * 32)

#define MAX_ICU_IRQS             207
