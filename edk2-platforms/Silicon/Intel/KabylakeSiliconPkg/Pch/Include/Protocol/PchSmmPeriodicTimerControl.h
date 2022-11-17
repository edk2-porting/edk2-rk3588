/** @file
  PCH SMM Periodic Timer Control Protocol

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PCH_SMM_PERIODIC_TIMER_CONTROL_H_
#define _PCH_SMM_PERIODIC_TIMER_CONTROL_H_


//
// Extern the GUID for protocol users.
//
extern EFI_GUID                                             gPchSmmPeriodicTimerControlGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _PCH_SMM_PERIODIC_TIMER_CONTROL_PROTOCOL     PCH_SMM_PERIODIC_TIMER_CONTROL_PROTOCOL;

//
// Related Definitions
//

//
// Member functions
//

/**
  The Prototype of Pause and Resume SMM PERIODIC TIMER function.

  @param[in] This                       Pointer to the PCH_SMM_PERIODIC_TIMER_CONTROL_PROTOCOL instance.
  @param[in] DispatchHandle             Handle of the child service to change state.

  @retval EFI_SUCCESS                   This operation is complete.
  @retval EFI_INVALID_PARAMETER         The DispatchHandle is invalid.
  @retval EFI_ACCESS_DENIED             The SMI status is alrady PAUSED/RESUMED.
**/
typedef
EFI_STATUS
(EFIAPI *PCH_SMM_PERIODIC_TIMER_CONTROL_FUNCTION) (
  IN PCH_SMM_PERIODIC_TIMER_CONTROL_PROTOCOL      *This,
  IN EFI_HANDLE                                   DispatchHandle
  );

/**
  Interface structure for the SMM PERIODIC TIMER pause and resume protocol
  This protocol provides the functions to runtime control the SM periodic timer enabled/disable.
  This applies the capability to the DispatchHandle which returned by SMM periodic timer callback
  registration.
  Besides, when S3 resume, it only restores the state of callback registration.
  The Paused/Resume state won't be restored after S3 resume.
**/
struct _PCH_SMM_PERIODIC_TIMER_CONTROL_PROTOCOL {
  /**
    This runtime pauses the registered periodic timer handler.
  **/
  PCH_SMM_PERIODIC_TIMER_CONTROL_FUNCTION         Pause;
  /**
    This runtime resumes the registered periodic timer handler.
  **/
  PCH_SMM_PERIODIC_TIMER_CONTROL_FUNCTION         Resume;
};

#endif // _PCH_SMM_PERIODIC_TIMER_CONTROL_H_
