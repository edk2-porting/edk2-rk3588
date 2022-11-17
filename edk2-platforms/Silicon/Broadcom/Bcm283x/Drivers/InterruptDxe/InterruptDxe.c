/** @file
 *
 *  Copyright (c) 2016, Linaro, Ltd. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <PiDxe.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <IndustryStandard/Bcm2836.h>

#include <Protocol/Cpu.h>
#include <Protocol/HardwareInterrupt.h>

//
// This currently only implements support for the architected timer interrupts
// on the per-CPU interrupt controllers.
//
#define NUM_IRQS                    (4)

#ifdef MDE_CPU_AARCH64
#define ARM_ARCH_EXCEPTION_IRQ      EXCEPT_AARCH64_IRQ
#else
#define ARM_ARCH_EXCEPTION_IRQ      EXCEPT_ARM_IRQ
#endif

STATIC CONST
EFI_PHYSICAL_ADDRESS RegBase = FixedPcdGet32 (PcdInterruptBaseAddress);

//
// Notifications
//
STATIC EFI_EVENT                    mExitBootServicesEvent;
STATIC HARDWARE_INTERRUPT_HANDLER   mRegisteredInterruptHandlers[NUM_IRQS];

/**
  Shutdown our hardware

  DXE Core will disable interrupts and turn off the timer and disable interrupts
  after all the event handlers have run.

  @param[in]  Event   The Event that is being processed
  @param[in]  Context Event Context
**/
STATIC
VOID
EFIAPI
ExitBootServicesEvent (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  // Disable all interrupts
  MmioWrite32 (RegBase + BCM2836_INTC_TIMER_CONTROL_OFFSET, 0);
}

/**
  Enable interrupt source Source.

  @param This     Instance pointer for this protocol
  @param Source   Hardware source of the interrupt

  @retval EFI_SUCCESS       Source interrupt enabled.
  @retval EFI_DEVICE_ERROR  Hardware could not be programmed.

**/
STATIC
EFI_STATUS
EFIAPI
EnableInterruptSource (
  IN EFI_HARDWARE_INTERRUPT_PROTOCOL    *This,
  IN HARDWARE_INTERRUPT_SOURCE          Source
  )
{
  if (Source >= NUM_IRQS) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  MmioOr32 (RegBase + BCM2836_INTC_TIMER_CONTROL_OFFSET, 1 << Source);

  return EFI_SUCCESS;
}


/**
  Disable interrupt source Source.

  @param This     Instance pointer for this protocol
  @param Source   Hardware source of the interrupt

  @retval EFI_SUCCESS       Source interrupt disabled.
  @retval EFI_DEVICE_ERROR  Hardware could not be programmed.

**/
STATIC
EFI_STATUS
EFIAPI
DisableInterruptSource (
  IN EFI_HARDWARE_INTERRUPT_PROTOCOL    *This,
  IN HARDWARE_INTERRUPT_SOURCE          Source
  )
{
  if (Source >= NUM_IRQS) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  MmioAnd32 (RegBase + BCM2836_INTC_TIMER_CONTROL_OFFSET, ~(1 << Source));

  return EFI_SUCCESS;
}

/**
  Register Handler for the specified interrupt source.

  @param This     Instance pointer for this protocol
  @param Source   Hardware source of the interrupt
  @param Handler  Callback for interrupt. NULL to unregister

  @retval EFI_SUCCESS Source was updated to support Handler.
  @retval EFI_DEVICE_ERROR  Hardware could not be programmed.

**/
STATIC
EFI_STATUS
EFIAPI
RegisterInterruptSource (
  IN EFI_HARDWARE_INTERRUPT_PROTOCOL    *This,
  IN HARDWARE_INTERRUPT_SOURCE          Source,
  IN HARDWARE_INTERRUPT_HANDLER         Handler
  )
{
  if (Source >= NUM_IRQS) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  if (Handler == NULL && mRegisteredInterruptHandlers[Source] == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Handler != NULL && mRegisteredInterruptHandlers[Source] != NULL) {
    return EFI_ALREADY_STARTED;
  }

  mRegisteredInterruptHandlers[Source] = Handler;
  return EnableInterruptSource (This, Source);
}


/**
  Return current state of interrupt source Source.

  @param This     Instance pointer for this protocol
  @param Source   Hardware source of the interrupt
  @param InterruptState  TRUE: source enabled, FALSE: source disabled.

  @retval EFI_SUCCESS       InterruptState is valid
  @retval EFI_DEVICE_ERROR  InterruptState is not valid

**/
STATIC
EFI_STATUS
EFIAPI
GetInterruptSourceState (
  IN EFI_HARDWARE_INTERRUPT_PROTOCOL    *This,
  IN HARDWARE_INTERRUPT_SOURCE          Source,
  IN BOOLEAN                            *InterruptState
  )
{
  if (InterruptState == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Source >= NUM_IRQS) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  *InterruptState = (MmioRead32 (RegBase + BCM2836_INTC_TIMER_CONTROL_OFFSET) &
                     (1 << Source)) != 0;

  return EFI_SUCCESS;
}

/**
  Signal to the hardware that the End Of Intrrupt state
  has been reached.

  @param This     Instance pointer for this protocol
  @param Source   Hardware source of the interrupt

  @retval EFI_SUCCESS       Source interrupt EOI'ed.
  @retval EFI_DEVICE_ERROR  Hardware could not be programmed.

**/
STATIC
EFI_STATUS
EFIAPI
EndOfInterrupt (
  IN EFI_HARDWARE_INTERRUPT_PROTOCOL    *This,
  IN HARDWARE_INTERRUPT_SOURCE          Source
  )
{
  return EFI_SUCCESS;
}


/**
  EFI_CPU_INTERRUPT_HANDLER that is called when a processor interrupt occurs.

  @param  InterruptType    Defines the type of interrupt or exception that
                           occurred on the processor.This parameter is processor
                           architecture specific.
  @param  SystemContext    A pointer to the processor context when
                           the interrupt occurred on the processor.

  @return None

**/
STATIC
VOID
EFIAPI
IrqInterruptHandler (
  IN EFI_EXCEPTION_TYPE           InterruptType,
  IN EFI_SYSTEM_CONTEXT           SystemContext
  )
{
  HARDWARE_INTERRUPT_HANDLER  InterruptHandler;
  HARDWARE_INTERRUPT_SOURCE   Source;
  UINT32                      RegVal;

  RegVal = MmioRead32 (RegBase + BCM2836_INTC_TIMER_PENDING_OFFSET) &
    ((1 << NUM_IRQS) - 1);
  Source = HighBitSet32 (RegVal);
  if (Source < 0) {
    return;
  }

  InterruptHandler = mRegisteredInterruptHandlers[Source];
  if (InterruptHandler != NULL) {
    // Call the registered interrupt handler.
    InterruptHandler (Source, SystemContext);
  }
}

//
// The protocol instance produced by this driver
//
STATIC EFI_HARDWARE_INTERRUPT_PROTOCOL gHardwareInterruptProtocol = {
  RegisterInterruptSource,
  EnableInterruptSource,
  DisableInterruptSource,
  GetInterruptSourceState,
  EndOfInterrupt
};

STATIC VOID *mCpuArchProtocolNotifyEventRegistration;

STATIC
VOID
EFIAPI
CpuArchEventProtocolNotify (
  IN  EFI_EVENT       Event,
  IN  VOID            *Context
  )
{
  EFI_CPU_ARCH_PROTOCOL   *Cpu;
  EFI_STATUS              Status;

  //
  // Get the CPU protocol that this driver requires.
  //
  Status = gBS->LocateProtocol (&gEfiCpuArchProtocolGuid, NULL, (VOID**)&Cpu);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Unregister the default exception handler.
  //
  Status = Cpu->RegisterInterruptHandler (Cpu, ARM_ARCH_EXCEPTION_IRQ, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Cpu->RegisterInterruptHandler() - %r\n",
      __FUNCTION__, Status));
    ASSERT (FALSE);
    return;
  }

  //
  // Register to receive interrupts
  //
  Status = Cpu->RegisterInterruptHandler (Cpu, ARM_ARCH_EXCEPTION_IRQ, IrqInterruptHandler);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Cpu->RegisterInterruptHandler() - %r\n",
      __FUNCTION__, Status));
    ASSERT (FALSE);
    return;
  }
}


/**
  Initialize the state information for the CPU Architectural Protocol

  @param  ImageHandle   of the loaded driver
  @param  SystemTable   Pointer to the System Table

  @retval EFI_SUCCESS           Protocol registered
  @retval EFI_OUT_OF_RESOURCES  Cannot allocate protocol data structure
  @retval EFI_DEVICE_ERROR      Hardware problems

**/
EFI_STATUS
InterruptDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS Status;
  EFI_EVENT  CpuArchEvent;

  // Make sure the Interrupt Controller Protocol is not already installed in the system.
  ASSERT_PROTOCOL_ALREADY_INSTALLED (NULL, &gHardwareInterruptProtocolGuid);

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gHardwareInterruptProtocolGuid,
                  &gHardwareInterruptProtocol,
                  NULL
                );
  ASSERT_EFI_ERROR (Status);

  //
  // Install the interrupt handler as soon as the CPU arch protocol appears.
  //
  CpuArchEvent = EfiCreateProtocolNotifyEvent (
                   &gEfiCpuArchProtocolGuid,
                   TPL_CALLBACK,
                   CpuArchEventProtocolNotify,
                   NULL,
                   &mCpuArchProtocolNotifyEventRegistration
                 );
  ASSERT (CpuArchEvent != NULL);

  // Register for an ExitBootServicesEvent
  Status = gBS->CreateEvent (EVT_SIGNAL_EXIT_BOOT_SERVICES, TPL_NOTIFY,
                  ExitBootServicesEvent, NULL, &mExitBootServicesEvent);

  ASSERT_EFI_ERROR (Status);

  return Status;
}
