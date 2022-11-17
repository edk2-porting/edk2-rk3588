/**
*
*  Copyright (C) 2018, Marvell International Ltd. and its affiliates
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/
#ifndef __ARMADA_ICU_LIB_H__
#define __ARMADA_ICU_LIB_H__

typedef enum {
  IcuIrqTypeLevel = 0,
  IcuIrqTypeEdge  = 1
} ICU_IRQ_TYPE;

typedef struct {
  UINTN IcuId;
  UINTN SpiId;
  ICU_IRQ_TYPE IrqType;
} ICU_IRQ;

typedef struct {
  CONST ICU_IRQ   *Map;
  UINTN           Size;
} ICU_CONFIG_ENTRY;

typedef struct {
  ICU_CONFIG_ENTRY NonSecure;
  ICU_CONFIG_ENTRY Sei;
  ICU_CONFIG_ENTRY Rei;
} ICU_CONFIG;

EFI_STATUS
EFIAPI
ArmadaIcuInitialize (
  VOID
  );

#endif /* __ARMADA_ICU_LIB_H__ */
