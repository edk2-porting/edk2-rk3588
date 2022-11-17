/** @file
*
*  Copyright (c) 2017, Rockchip Inc. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#ifndef __PWM_H__
#define __PWM_H__

RETURN_STATUS
EFIAPI
PWMApply (
  IN UINTN   PWMBase,
  IN UINT32  PeriodNs,
  IN UINT32  DutyNs,
  IN BOOLEAN Polarity
  );

RETURN_STATUS
EFIAPI
PWMEnable (
  IN UINTN   PWMBase
  );

RETURN_STATUS
EFIAPI
PWMDisable (
  IN UINTN   PWMBase
  );

#endif
