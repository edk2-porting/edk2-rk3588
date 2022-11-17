/** @file
  USB2 PHY configuration policy

@copyright
  Copyright 2018 - 2021 Intel Corporation.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _USB2_PHY_CONFIG_H_
#define _USB2_PHY_CONFIG_H_

#define USB2_PHY_CONFIG_REVISION 1
extern EFI_GUID gUsb2PhyConfigGuid;

#pragma pack (push,1)

/**
  This structure configures per USB2 AFE settings.
  It allows to setup the port electrical parameters.
**/
typedef struct {
/** Per Port HS Preemphasis Bias (PERPORTPETXISET)
  000b - 0mV
  001b - 11.25mV
  010b - 16.9mV
  011b - 28.15mV
  100b - 28.15mV
  101b - 39.35mV
  110b - 45mV
  111b - 56.3mV
**/
  UINT8   Petxiset;
/** Per Port HS Transmitter Bias (PERPORTTXISET)
  000b - 0mV
  001b - 11.25mV
  010b - 16.9mV
  011b - 28.15mV
  100b - 28.15mV
  101b - 39.35mV
  110b - 45mV
  111b - 56.3mV
**/
  UINT8   Txiset;
/**
  Per Port HS Transmitter Emphasis (IUSBTXEMPHASISEN)
  00b - Emphasis OFF
  01b - De-emphasis ON
  10b - Pre-emphasis ON
  11b - Pre-emphasis & De-emphasis ON
**/
  UINT8   Predeemp;
/**
  Per Port Half Bit Pre-emphasis (PERPORTTXPEHALF)
  1b - half-bit pre-emphasis
  0b - full-bit pre-emphasis
**/
  UINT8   Pehalfbit;
} USB2_PHY_PARAMETERS;

#pragma pack (pop)

#endif // _USB2_PHY_CONFIG_H_

