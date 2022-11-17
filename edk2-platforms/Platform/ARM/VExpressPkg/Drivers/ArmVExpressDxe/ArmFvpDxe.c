/** @file

  Copyright (c) 2013-2021, Arm Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>

#include <Library/ArmShellCmdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/VirtioMmioDeviceLib.h>

#include <VExpressMotherBoard.h>

#define ARM_FVP_BASE_VIRTIO_BLOCK_BASE    0x1c130000

// SMMUv3 Global Bypass Attribute (GBPA) register offset.
#define SMMU_GBPA                         0x0044

// SMMU_GBPA register fields.
#define SMMU_GBPA_UPDATE                  BIT31
#define SMMU_GBPA_ABORT                   BIT20

#pragma pack(1)
typedef struct {
  VENDOR_DEVICE_PATH                  Vendor;
  EFI_DEVICE_PATH_PROTOCOL            End;
} VIRTIO_BLK_DEVICE_PATH;
#pragma pack()

VIRTIO_BLK_DEVICE_PATH mVirtioBlockDevicePath =
{
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)( sizeof(VENDOR_DEVICE_PATH) ),
        (UINT8)((sizeof(VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    EFI_CALLER_ID_GUID,
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      sizeof (EFI_DEVICE_PATH_PROTOCOL),
      0
    }
  }
};

/**
  Poll the SMMU register and test the value based on the mask.

  @param [in]  SmmuReg    Base address of the SMMU register.
  @param [in]  Mask       Mask of register bits to monitor.
  @param [in]  Value      Expected value.

  @retval EFI_SUCCESS     Success.
  @retval EFI_TIMEOUT     Timeout.
**/
STATIC
EFI_STATUS
EFIAPI
SmmuV3Poll (
  IN  UINT64 SmmuReg,
  IN  UINT32 Mask,
  IN  UINT32 Value
  )
{
  UINT32 RegVal;
  UINTN  Count;

  // Set 1ms timeout value.
  Count = 10;
  do {
    RegVal = MmioRead32 (SmmuReg);
    if ((RegVal & Mask) == Value) {
      return EFI_SUCCESS;
    }
    MicroSecondDelay (100);
  } while ((--Count) > 0);

  DEBUG ((DEBUG_ERROR, "Timeout polling SMMUv3 register @%p\n", SmmuReg));
  DEBUG ((
    DEBUG_ERROR,
    "Read value 0x%x, expected 0x%x\n",
    RegVal,
    ((Value == 0) ? (RegVal & ~Mask) : (RegVal | Mask))
    ));
  return EFI_TIMEOUT;
}

/**
  Initialise the SMMUv3 to set Non-secure streams to bypass the SMMU.

  @param [in]  SmmuReg    Base address of the SMMUv3.

  @retval EFI_SUCCESS     Success.
  @retval EFI_TIMEOUT     Timeout.
**/
STATIC
EFI_STATUS
EFIAPI
SmmuV3Init (
  IN  UINT64 SmmuBase
  )
{
  EFI_STATUS  Status;
  UINT32      RegVal;

  // Attribute update has completed when SMMU_(S)_GBPA.Update bit is 0.
  Status = SmmuV3Poll (SmmuBase + SMMU_GBPA, SMMU_GBPA_UPDATE, 0);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // SMMU_(S)_CR0 resets to zero with all streams bypassing the SMMU,
  // so just abort all incoming transactions.
  RegVal = MmioRead32 (SmmuBase + SMMU_GBPA);

  // TF-A configures the SMMUv3 to abort all incoming transactions.
  // Clear the SMMU_GBPA.ABORT to allow Non-secure streams to bypass
  // the SMMU.
  RegVal &= ~SMMU_GBPA_ABORT;
  RegVal |= SMMU_GBPA_UPDATE;

  MmioWrite32 (SmmuBase + SMMU_GBPA, RegVal);

  Status = SmmuV3Poll (SmmuBase + SMMU_GBPA, SMMU_GBPA_UPDATE, 0);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
 * Generic UEFI Entrypoint for 'ArmFvpDxe' driver
 * See UEFI specification for the details of the parameters
 */
EFI_STATUS
EFIAPI
ArmFvpInitialise (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                   Status;
  UINT32                       SysId;

  Status = gBS->InstallProtocolInterface (&ImageHandle,
                 &gEfiDevicePathProtocolGuid, EFI_NATIVE_INTERFACE,
                 &mVirtioBlockDevicePath);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Declare the Virtio BlockIo device
  Status = VirtioMmioInstallDevice (ARM_FVP_BASE_VIRTIO_BLOCK_BASE, ImageHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "ArmFvpDxe: Failed to install Virtio block device\n"));
  }

  // Install dynamic Shell command to run baremetal binaries.
  Status = ShellDynCmdRunAxfInstall (ImageHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "ArmFvpDxe: Failed to install ShellDynCmdRunAxf\n"));
  }

  // If FVP RevC - Configure SMMUv3 to set NS transactions in bypass mode.
  SysId = MmioRead32 (ARM_VE_SYS_ID_REG);
  if ((SysId & ARM_FVP_SYS_ID_REV_MASK) == ARM_FVP_BASE_REVC_REV) {
    Status = SmmuV3Init (FVP_REVC_SMMUV3_BASE);
    if (EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_ERROR,
        "ArmFvpDxe: Failed to initialise SMMUv3 in bypass mode.\n"
        ));
    }
  }

  return Status;
}
