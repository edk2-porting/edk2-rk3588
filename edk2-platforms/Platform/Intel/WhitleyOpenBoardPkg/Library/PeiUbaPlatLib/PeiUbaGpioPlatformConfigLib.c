/** @file

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <Uefi/UefiSpec.h>
#include <Ppi/UbaCfgDb.h>
#include <Ppi/DynamicSiLibraryPpi.h>
#include <Base.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>

//
// UBA and GPIO headers
//
#include <Library/UbaGpioPlatformConfig.h>
#include <Library/GpioLib.h>


/**

Function gets pointers to the UBA data installed by the right RP package

@param[out] GpioParams          The pointer to the platform GPIO parameters


@retval EFI_SUCCESS             The function completed successfully

**/
EFI_STATUS
PlatformGetGpioPlatformMappings (
  IN OUT   PLATFORM_GPIO_CONFIG_TABLE             *GpioParams
  )
{
  EFI_STATUS                            Status;
  UBA_CONFIG_DATABASE_PPI               *UbaConfigPpi = NULL;
  UINTN                                 TableSize;

  Status = PeiServicesLocatePpi (&gUbaConfigDatabasePpiGuid, 0, NULL, &UbaConfigPpi);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  TableSize = sizeof(PLATFORM_GPIO_CONFIG_TABLE);
  Status = UbaConfigPpi->GetData (
                                UbaConfigPpi,
                                &gPlatformGpioPlatformConfigDataGuid,
                                GpioParams,
                                &TableSize
                                );

  return Status;
}

/**

    Reads GPIO pin to get Flash Security Override jumper status

    @param[out] Jumper - The pointer to the jumper output

    @retval Status - Success if GPIO's are read properly
            Jumper - 0x0 if an error happened, otherwise the jumperl value

**/
EFI_STATUS
GpioGetFlashSecOvrdVal (
  OUT UINT32 *Jumper
  )
{
  EFI_STATUS                  Status;
  PLATFORM_GPIO_CONFIG_TABLE  GpioPlatformConfig;
  DYNAMIC_SI_LIBARY_PPI       *DynamicSiLibraryPpi = NULL;

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  *Jumper = 0x0;

  Status = PlatformGetGpioPlatformMappings (&GpioPlatformConfig);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (GpioPlatformConfig.FlashSecOverride == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }

  Status = DynamicSiLibraryPpi->GpioGetInputValue (GpioPlatformConfig.FlashSecOverride, Jumper);
  return Status;
}

/**

    Reads GPIO pin to get recovery jumper status

    @param[out] RcvJumper - The pointer to the Recovery jumper input

    @retval Status - Success if GPIO's are read properly

**/
EFI_STATUS
GpioGetRcvPadVal (
  OUT UINT32 *RcvJumper
)
{
  EFI_STATUS                  Status;
  PLATFORM_GPIO_CONFIG_TABLE  GpioPlatformConfig;
  DYNAMIC_SI_LIBARY_PPI       *DynamicSiLibraryPpi = NULL;

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = PlatformGetGpioPlatformMappings (&GpioPlatformConfig);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (GpioPlatformConfig.RcvJumper == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }

  Status = DynamicSiLibraryPpi->GpioGetInputValue (GpioPlatformConfig.RcvJumper, RcvJumper);
  return Status;
}


/**

    Reads GPIO pin to get FM ADR trigger pin

    @param[out] FmAdrTrigger - The pointer to the ADR trigger input

    @retval Status - Success if GPIO's are read properly

**/
EFI_STATUS
GpioGetFmAdrTriggerPadVal (
  OUT UINT32 *FmAdrTrigger
)
{
  EFI_STATUS                  Status;
  PLATFORM_GPIO_CONFIG_TABLE  GpioPlatformConfig;
  DYNAMIC_SI_LIBARY_PPI       *DynamicSiLibraryPpi = NULL;

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = PlatformGetGpioPlatformMappings (&GpioPlatformConfig);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (GpioPlatformConfig.FmAdrTrigger == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }

  Status = DynamicSiLibraryPpi->GpioGetInputValue (GpioPlatformConfig.FmAdrTrigger, FmAdrTrigger);
  return Status;
}


/**

    Sets GPIO pin to enable ADR on the board

    @param Set[in] - If TRUE means the pas should go 'high', otherwise 'low'

    @retval Status - Success if GPIO set properly

**/
EFI_STATUS
GpioSetAdrEnablePadOutVal (
  IN BOOLEAN Set
)
{
  EFI_STATUS                  Status;
  PLATFORM_GPIO_CONFIG_TABLE  GpioPlatformConfig;
  DYNAMIC_SI_LIBARY_PPI       *DynamicSiLibraryPpi = NULL;

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = PlatformGetGpioPlatformMappings (&GpioPlatformConfig);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  if (GpioPlatformConfig.AdrEnable == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }

  if (Set) {
    Status = DynamicSiLibraryPpi->GpioSetOutputValue (GpioPlatformConfig.AdrEnable, GpioOutHigh);
  }
  else {
    Status = DynamicSiLibraryPpi->GpioSetOutputValue (GpioPlatformConfig.AdrEnable, GpioOutLow);
  }
  return Status;
}

/**

    Reads GPIO pin to Force to S1 config mode pad

    @param[out] ForceS1ConfigPad - Input value of the Froce S1 Config pad

    @retval Status - Success if GPIO's are read properly

**/
EFI_STATUS
GpioGetForcetoS1ConfigModePadVal (
  OUT UINT32 *ForceS1ConfigPad
)
{
  EFI_STATUS                  Status;
  PLATFORM_GPIO_CONFIG_TABLE  GpioPlatformConfig;
  DYNAMIC_SI_LIBARY_PPI       *DynamicSiLibraryPpi = NULL;

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = PlatformGetGpioPlatformMappings (&GpioPlatformConfig);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (GpioPlatformConfig.ForceTo1SConfigModePad == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }

  Status = DynamicSiLibraryPpi->GpioGetInputValue (GpioPlatformConfig.ForceTo1SConfigModePad, ForceS1ConfigPad);
  return Status;
}

/**

    Reads GPIO pin related to QAT

    @param[out] QATPad - Input value of the QAT pad

    @retval Status - Success if GPIO's are read properly

**/
EFI_STATUS
GpioGetQATPadVal (
  OUT UINT32 *QATPad
)
{
  EFI_STATUS                  Status;
  PLATFORM_GPIO_CONFIG_TABLE  GpioPlatformConfig;
  DYNAMIC_SI_LIBARY_PPI       *DynamicSiLibraryPpi = NULL;

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = PlatformGetGpioPlatformMappings (&GpioPlatformConfig);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (GpioPlatformConfig.QATGpio == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }

  Status = DynamicSiLibraryPpi->GpioGetInputValue (GpioPlatformConfig.QATGpio, QATPad);
  return Status;

}


/**

    Get GPIO pin for FPGA error detection RAS functionality

    @param[out] FpgaErrorPad -The input value of the FPGA error 1 pad

    @retval Status - Success if GPIO's pad read properly

**/
EFI_STATUS
GpioGetFpgaErrorPad1 (
  OUT UINT32 *FpgaErrorPad
)
{
  EFI_STATUS           Status;
  PLATFORM_GPIO_CONFIG_TABLE GpioPlatformConfig;

  Status = PlatformGetGpioPlatformMappings (&GpioPlatformConfig);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (GpioPlatformConfig.FpgaErrorSingnalPad1 == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }

  *FpgaErrorPad = (UINT32) GpioPlatformConfig.FpgaErrorSingnalPad1;
  return EFI_SUCCESS;

}


/**

    Get GPIO pin for FPGA error detection RAS functionality

    @param[out] FpgaErrorPad -The input value of the FPGA error 2 pad

    @retval Status - Success if GPIO's pad read properly

**/
EFI_STATUS
GpioGetFpgaErrorPad2 (
  OUT UINT32 *FpgaErrorPad
)
{
  EFI_STATUS           Status;
  PLATFORM_GPIO_CONFIG_TABLE GpioPlatformConfig;

  Status = PlatformGetGpioPlatformMappings (&GpioPlatformConfig);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (GpioPlatformConfig.FpgaErrorSingnalPad2 == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }

  *FpgaErrorPad = (UINT32) GpioPlatformConfig.FpgaErrorSingnalPad2;
  return Status;

}


/**

    Get GPIO pin for CPU HP SMI detection for RAS functionality

    @retval Status - Success if GPIO's pad read properly

**/
EFI_STATUS
GpioGetCpuHpSmiPad (
  OUT UINT32 *CpuHpSmiPad
)
{
  EFI_STATUS           Status;
  PLATFORM_GPIO_CONFIG_TABLE GpioPlatformConfig;

  Status = PlatformGetGpioPlatformMappings (&GpioPlatformConfig);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (GpioPlatformConfig.CpuHpSmiPad == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }

  *CpuHpSmiPad = (UINT32) GpioPlatformConfig.CpuHpSmiPad;
  return Status;

}


/**

    Reads GPIO pin that is first bit of the Board ID indication word

    @retval Status - Success if GPIO's are read properly

**/
EFI_STATUS
GpioGetBoardId0PadVal (
  OUT UINT32 *BoardID0Gpio
)
{
  EFI_STATUS                  Status;
  PLATFORM_GPIO_CONFIG_TABLE  GpioPlatformConfig;
  DYNAMIC_SI_LIBARY_PPI       *DynamicSiLibraryPpi = NULL;

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = PlatformGetGpioPlatformMappings (&GpioPlatformConfig);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (GpioPlatformConfig.BoardID0Gpio == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }

  Status = DynamicSiLibraryPpi->GpioGetInputValue (GpioPlatformConfig.BoardID0Gpio, BoardID0Gpio);
  return Status;

}

/**

    Sets GPIO's used for Boot Mode

    @param None

    @retval Status - Success if GPIO's are configured

**/
EFI_STATUS
GpioConfigForMFGMode (
  VOID
  )
{
  EFI_STATUS                  Status;
  PLATFORM_GPIO_CONFIG_TABLE  GpioPlatformConfig;
  DYNAMIC_SI_LIBARY_PPI       *DynamicSiLibraryPpi = NULL;

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = PlatformGetGpioPlatformMappings (&GpioPlatformConfig);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (GpioPlatformConfig.GpioMfgPad.GpioPad == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }

  DEBUG ((EFI_D_INFO, "Start ConfigureGpio() for BootMode Detection.\n"));

  Status = DynamicSiLibraryPpi->GpioSetPadConfig (GpioPlatformConfig.GpioMfgPad.GpioPad, &GpioPlatformConfig.GpioMfgPad.GpioConfig);

  ASSERT_EFI_ERROR (Status);
  DEBUG ((EFI_D_INFO, "End ConfigureGpio() for BootMode Detection.\n"));
  return Status;
}

/**

    Checks whether the MDF jumper has been set

    @param None

    @retval ManufacturingMode - TRUE when MFG jumper is on, FALSE otherwise

**/
BOOLEAN
IsManufacturingMode (
  VOID
)
{

  BOOLEAN                     ManufacturingMode = TRUE;

  EFI_STATUS                  Status;
  UINT32                      GpiValue;
  PLATFORM_GPIO_CONFIG_TABLE  GpioPlatformConfig;
  DYNAMIC_SI_LIBARY_PPI       *DynamicSiLibraryPpi = NULL;

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return FALSE;
  }

  Status = PlatformGetGpioPlatformMappings (&GpioPlatformConfig);
  ASSERT_EFI_ERROR (Status);

  if (GpioPlatformConfig.GpioMfgPad.GpioPad == UNUSED_GPIO) {
    return FALSE;
  }

  Status = GpioConfigForMFGMode ();
  ASSERT_EFI_ERROR (Status);


  Status = DynamicSiLibraryPpi->GpioGetInputValue (GpioPlatformConfig.GpioMfgPad.GpioPad, &GpiValue);

  ASSERT_EFI_ERROR (Status);

  if (!GpiValue) {
    ManufacturingMode = FALSE;
  }
  return ManufacturingMode;

}


