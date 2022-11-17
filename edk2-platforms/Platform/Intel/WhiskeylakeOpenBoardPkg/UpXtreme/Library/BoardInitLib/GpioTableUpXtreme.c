/** @file
  GPIO definition table for the UP Xtreme

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <GpioPinsCnlLp.h>
#include <Library/GpioLib.h>
#include <GpioConfig.h>

GPIO_INIT_CONFIG mGpioTableUpXtreme[] =
{
  {GPIO_CNL_LP_GPP_A0,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_A1,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNative }},
  {GPIO_CNL_LP_GPP_A2,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNative }},
  {GPIO_CNL_LP_GPP_A3,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNative }},
  {GPIO_CNL_LP_GPP_A4,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNative }},
  {GPIO_CNL_LP_GPP_A5,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_A6,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_A7,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_A8,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_A9,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermWpd20K }},
  {GPIO_CNL_LP_GPP_A10, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermWpd20K }},
  {GPIO_CNL_LP_GPP_A11, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_A12, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_A13, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_A14, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_A15, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_A16, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirOut,      GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioPlatformReset,  GpioTermWpd20K }},
  {GPIO_CNL_LP_GPP_A17, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_A18, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_A19, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_A20, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirOut,      GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioResumeReset,    GpioTermWpu20K }},
  {GPIO_CNL_LP_GPP_A21, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirOut,      GpioOutHigh, GpioIntDefault | GpioIntEdge,  GpioResumeReset,    GpioTermWpu20K }},
  {GPIO_CNL_LP_GPP_A22, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirOut,      GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioResumeReset,    GpioTermWpu20K }},
  {GPIO_CNL_LP_GPP_A23, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirOut,      GpioOutHigh, GpioIntDefault | GpioIntEdge,  GpioResumeReset,    GpioTermWpu20K }},

  {GPIO_CNL_LP_GPP_B0,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_B1,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_B2,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_B3,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_B4,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_B5,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_B6,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_B7,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_B8,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_B9,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_B10, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_B11, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_B12, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_B13, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_B14, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_B15, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_B16, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_B17, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_B18, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_B19, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_B20, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_B21, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_B22, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_B23, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},

  //(RC control) {GPIO_CNL_LP_GPP_C0,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_C1,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_C2,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirOut,      GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(CSME control) {GPIO_CNL_LP_GPP_C3,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(CSME control) {GPIO_CNL_LP_GPP_C4,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_C5,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_C6,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_C7,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_C8,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_C9,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_C10, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_C11, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_C12, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_C13, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_C14, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_C15, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_C16, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_C17, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_C18, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_C19, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_C20, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_C21, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_C22, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_C23, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},

  //(CSME control) {GPIO_CNL_LP_GPP_D0,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(CSME control) {GPIO_CNL_LP_GPP_D1,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirNone,     GpioOutHigh, GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(CSME control) {GPIO_CNL_LP_GPP_D2,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(CSME control) {GPIO_CNL_LP_GPP_D3,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_D4,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirOut,      GpioOutHigh, GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermWpu20K }},
  //(RC control) {GPIO_CNL_LP_GPP_D5,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_D6,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_D7,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_D8,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_D9,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_D10, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_D11, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_D12, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_D13, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirOut,      GpioOutHigh, GpioIntDefault | GpioIntEdge,  GpioResumeReset,    GpioTermNone   }},
  {GPIO_CNL_LP_GPP_D14, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_D15, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  {GPIO_CNL_LP_GPP_D16, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirOut,      GpioOutHigh, GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermWpu20K }},
  //(RC control) {GPIO_CNL_LP_GPP_D17, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_D18, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_D19, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_D20, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(CSME control) {GPIO_CNL_LP_GPP_D21, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(CSME control) {GPIO_CNL_LP_GPP_D22, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_D23, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},

  {GPIO_CNL_LP_GPP_E0,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},
  //(RC control) {GPIO_CNL_LP_GPP_E1,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //M.2_SSD_DET
  {GPIO_CNL_LP_GPP_E2,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //Reserved for SATA HP val
  {GPIO_CNL_LP_GPP_E3,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //EC_SMI_N
  {GPIO_CNL_LP_GPP_E4,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //DGPU_PWROK
  //(RC control) {GPIO_CNL_LP_GPP_E5,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //SSD_DEVSLP
  //(RC control) {GPIO_CNL_LP_GPP_E6,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //HDD_DEVSLP
  {GPIO_CNL_LP_GPP_E7,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //TCH_PNL_INT_N
  //(RC control) {GPIO_CNL_LP_GPP_E8,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //SATA_LED_N
  //(RC control) {GPIO_CNL_LP_GPP_E9,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //BSSB_CLK
  //(RC control) {GPIO_CNL_LP_GPP_E10, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //BSSB_DI
  //(RC control) {GPIO_CNL_LP_GPP_E11, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //USB_OC_2
  //(RC control) {GPIO_CNL_LP_GPP_E12, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //USB_OC_3
  //(RC control) {GPIO_CNL_LP_GPP_E13, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //DDI1_HPD
  //(RC control) {GPIO_CNL_LP_GPP_E14, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //DDI2_HPD_EC
  //(RC control) {GPIO_CNL_LP_GPP_E15, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirOut,      GpioOutHigh, GpioIntDefault | GpioIntEdge,  GpioPlatformReset,  GpioTermNone   }},  //DDI3_HPD
  //(RC control) {GPIO_CNL_LP_GPP_E16, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //DDI4_HPD
  //(RC control) {GPIO_CNL_LP_GPP_E17, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //EDP_HPD
  //(RC control) {GPIO_CNL_LP_GPP_E18, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //DDI1_CTRL_CLK
  //(RC control) {GPIO_CNL_LP_GPP_E19, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirOut,      GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //DDI1_CTRL_DATA
  //(RC control) {GPIO_CNL_LP_GPP_E20, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //DDI2_CTRL_CLK
  //(RC control) {GPIO_CNL_LP_GPP_E21, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirOut,      GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //DDI2_CTRL_DATA
  //(RC control) {GPIO_CNL_LP_GPP_E22, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //DDI3_CTRL_CLK
  //(RC control) {GPIO_CNL_LP_GPP_E23, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirOut,      GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //DDI3_CTRL_DATA

  {GPIO_CNL_LP_GPP_F0,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //GPP_F0_COEX3
  {GPIO_CNL_LP_GPP_F1,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //WWAN_RST_N
  {GPIO_CNL_LP_GPP_F2,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //SATA_HDD_PWREN
  {GPIO_CNL_LP_GPP_F3,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //WF_CLK_EN
  {GPIO_CNL_LP_GPP_F4,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //CNV_BRI_DT_UART0_RTSB
  {GPIO_CNL_LP_GPP_F5,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //CNV_BRI_RSP_UART0_RXD
  {GPIO_CNL_LP_GPP_F6,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //CNV_RGI_DT_UART0_TXD
  {GPIO_CNL_LP_GPP_F7,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //CNV_RGI_RSP_UART0_CTSB
  {GPIO_CNL_LP_GPP_F8,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //CNV_MFUART2_RXD
  {GPIO_CNL_LP_GPP_F9,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //CNV_MFUART2_TXD
  {GPIO_CNL_LP_GPP_F10, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //BIOS_REC

  {GPIO_CNL_LP_GPP_F11, {GpioPadModeNative1,   GpioHostOwnGpio,     GpioDirNone,     GpioOutDefault, GpioIntDis, GpioHostDeepReset,  GpioTermNone}},//EMMC_CMD
  {GPIO_CNL_LP_GPP_F12, {GpioPadModeNative1,   GpioHostOwnGpio,     GpioDirNone,     GpioOutDefault, GpioIntDis, GpioHostDeepReset,  GpioTermNone}},//EMMC_DATA0
  {GPIO_CNL_LP_GPP_F13, {GpioPadModeNative1,   GpioHostOwnGpio,     GpioDirNone,     GpioOutDefault, GpioIntDis, GpioHostDeepReset,  GpioTermNone}},//EMMC_DATA1
  {GPIO_CNL_LP_GPP_F14, {GpioPadModeNative1,   GpioHostOwnGpio,     GpioDirNone,     GpioOutDefault, GpioIntDis, GpioHostDeepReset,  GpioTermNone}},//EMMC_DATA2
  {GPIO_CNL_LP_GPP_F15, {GpioPadModeNative1,   GpioHostOwnGpio,     GpioDirNone,     GpioOutDefault, GpioIntDis, GpioHostDeepReset,  GpioTermNone}},//EMMC_DATA3
  {GPIO_CNL_LP_GPP_F16, {GpioPadModeNative1,   GpioHostOwnGpio,     GpioDirNone,     GpioOutDefault, GpioIntDis, GpioHostDeepReset,  GpioTermNone}},//EMMC_DATA4
  {GPIO_CNL_LP_GPP_F17, {GpioPadModeNative1,   GpioHostOwnGpio,     GpioDirNone,     GpioOutDefault, GpioIntDis, GpioHostDeepReset,  GpioTermNone}},//EMMC_DATA5
  {GPIO_CNL_LP_GPP_F18, {GpioPadModeNative1,   GpioHostOwnGpio,     GpioDirNone,     GpioOutDefault, GpioIntDis, GpioHostDeepReset,  GpioTermNone}},//EMMC_DATA6
  {GPIO_CNL_LP_GPP_F19, {GpioPadModeNative1,   GpioHostOwnGpio,     GpioDirNone,     GpioOutDefault, GpioIntDis, GpioHostDeepReset,  GpioTermNone}},//EMMC_DATA7
  {GPIO_CNL_LP_GPP_F20, {GpioPadModeNative1,   GpioHostOwnGpio,     GpioDirNone,     GpioOutDefault, GpioIntDis, GpioHostDeepReset,  GpioTermNone}},//EMMC_RCLK
  {GPIO_CNL_LP_GPP_F21, {GpioPadModeNative1,   GpioHostOwnGpio,     GpioDirNone,     GpioOutDefault, GpioIntDis, GpioHostDeepReset,  GpioTermNone}},//EMMC_CLK
  {GPIO_CNL_LP_GPP_F22, {GpioPadModeNative1,   GpioHostOwnGpio,     GpioDirNone,     GpioOutDefault, GpioIntDis, GpioHostDeepReset,  GpioTermNone}},//EMMC_RESETB

  //(RC control) {GPIO_CNL_LP_GPP_F23, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //GPP_F_23

  //(RC control) {GPIO_CNL_LP_GPP_G0,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //GPP_G_0_SD3_CMD
  //(RC control) {GPIO_CNL_LP_GPP_G1,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //GPP_G_1_SD3_D0_SD4_RCLK_P
  //(RC control) {GPIO_CNL_LP_GPP_G2,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //GPP_G_2_SD3_D1_SD4_RCLK_N
  //(RC control) {GPIO_CNL_LP_GPP_G3,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //GPP_G_3_SD3_D2
  //(RC control) {GPIO_CNL_LP_GPP_G4,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //GPP_G_4_SD3_D3
  {GPIO_CNL_LP_GPP_G5,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //GPP_G_5_SD3_CDB
  {GPIO_CNL_LP_GPP_G6,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //GPP_G_6_SD3_CLK
  {GPIO_CNL_LP_GPP_G7,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //GPP_G_7_SD3_WP

  //H0-H3 -> Under GPIO table for GPIO Termination -20K WPU
  {GPIO_CNL_LP_GPP_H0,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //GPP_H_0_SSP2_SCLK
  {GPIO_CNL_LP_GPP_H1,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //GPP_H_1_SSP2_SFRM
  {GPIO_CNL_LP_GPP_H2,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //GPP_H_2_SSP2_TXD
  {GPIO_CNL_LP_GPP_H3,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //GPP_H_3_SSP2_RXD
  //(RC control) {GPIO_CNL_LP_GPP_H4,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirin,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //GPP_H_4_I2C2_SDA
  //(RC control) {GPIO_CNL_LP_GPP_H5,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirin,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //GPP_H_5_I2C2_SCL
  //(RC control) {GPIO_CNL_LP_GPP_H6,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //GPP_H_6_I2C3_SDA
  //(RC control) {GPIO_CNL_LP_GPP_H7,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //GPP_H_7_I2C3_SCL
  //(RC control) {GPIO_CNL_LP_GPP_H8,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //GPP_H_8_I2C4_SDA
  //(RC control) {GPIO_CNL_LP_GPP_H9,  { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //GPP_H_9_I2C4_SCL
  {GPIO_CNL_LP_GPP_H10, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirOut,      GpioOutHigh, GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //IVCAM_PWREN
  {GPIO_CNL_LP_GPP_H11, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirOut,      GpioOutHigh, GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //IVCAM_RECOVERY
  {GPIO_CNL_LP_GPP_H12, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //IRIS_STROBE
  {GPIO_CNL_LP_GPP_H13, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //IVCAM_MUX_SEL0
  {GPIO_CNL_LP_GPP_H14, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //UF_CAM_PRIVACY_LED
  {GPIO_CNL_LP_GPP_H15, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //IVCAM_KEY
  {GPIO_CNL_LP_GPP_H16, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirOut,      GpioOutHigh, GpioIntDefault | GpioIntEdge,  GpioResumeReset,    GpioTermNone   }},  //DDI4_CTRL_CLK
  {GPIO_CNL_LP_GPP_H17, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirOut,      GpioOutHigh, GpioIntDefault | GpioIntEdge,  GpioResumeReset,    GpioTermNone   }},  //DDI4_CTRL_DATA
  {GPIO_CNL_LP_GPP_H18, { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirNone,     GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //VCCIO_LPM
  {GPIO_CNL_LP_GPP_H19, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //IVCAM_MUX_SEL1
  //(RC control) {GPIO_CNL_LP_GPP_H20, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }}, //IMGCLKOUT_WF_CAM
  {GPIO_CNL_LP_GPP_H21, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }}, //GPP_H21
  {GPIO_CNL_LP_GPP_H22, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }},  //WF_CAM_RST
  {GPIO_CNL_LP_GPP_H23, { GpioPadModeGpio,     GpioHostOwnDefault,  GpioDirIn,       GpioOutLow,  GpioIntDefault | GpioIntEdge,  GpioHostDeepReset,  GpioTermNone   }}, //GPP_H23

  //(Default HW)  {GPIO_CNL_LP_GPD0,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //PM_BATLOW_N
  //(Default HW)  {GPIO_CNL_LP_GPD1,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //BC_ACOK
  //(Default HW)  {GPIO_CNL_LP_GPD2,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //LAN_WAKE
  //(Default HW)  {GPIO_CNL_LP_GPD3,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //PM_PWRBTN_N
  //(Default HW)  {GPIO_CNL_LP_GPD4,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //PM_SLP_S3_N
  //(Default HW)  {GPIO_CNL_LP_GPD5,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //PM_SLP_S4_N
  //(Default HW)  {GPIO_CNL_LP_GPD6,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //SLP_A_N
  //{GPIO_CNL_LP_GPD7,  { GpioPadModeNotUsed,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //GPD_7
  //(Default HW)  {GPIO_CNL_LP_GPD8,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //SUS_CLK
  //(Default HW)  {GPIO_CNL_LP_GPD9,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //PM_SLP_WLAN_N
  //(Default HW)  {GPIO_CNL_LP_GPD10,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //PM_SLP_S5_N
  //(Default HW)  {GPIO_CNL_LP_GPD11,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //LANPHY_EN

  {GPIO_CNL_LP_PECI,  { GpioHardwareDefault,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermDefault }}, // PECI
};
UINT16 mGpioTableUpXtremeSize = sizeof (mGpioTableUpXtreme) / sizeof (GPIO_INIT_CONFIG);
