/** @file
  PCIe Initialization Library header file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PCIE_INIT_LIB_H_
#define _PCIE_INIT_LIB_H_

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PostCodeLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/PeiServicesLib.h>
#include <IndustryStandard/Pci30.h>
#include <Library/MmPciLib.h>
#include <Library/GpioLib.h>


#ifdef PCIE_X32_SUPPORT
#define PCIE_MAX_LANE                   32
#else
#define PCIE_MAX_LANE                   16
#endif
#define PCIE_ROOT_PORT_BITMAP_LENGTH    8
#define PCIE_SWEQ_MAX_PRESETS           11

#ifndef STALL_ONE_MICRO_SECOND
#define STALL_ONE_MICRO_SECOND  1
#endif
#ifndef STALL_ONE_MILLI_SECOND
#define STALL_ONE_MILLI_SECOND  1000
#endif

///
/// PCIe Root Port description data structure, used as the interface between low
/// level and high level
///
typedef struct {
  UINT8                   MaxCapableSpeed;
  UINT8                   MaxCapableWidth;
  BOOLEAN                 EnableMargin;
  BOOLEAN                 SkipMargin;
  BOOLEAN                 FoundUsableTxEq;
  UINT8                   ActiveLaneListLength;
  UINT8                   ActiveLaneList[PCIE_MAX_LANE];
} PCIE_PORT_SWEQ_DATA;

typedef struct {
  UINT8                   Bus;
  UINT8                   Device;
  UINT8                   Function;
  UINTN                   ConfigSpaceBase;
  UINT8                   PcieCapOffset;
  UINT8                   MaxPortWidth;
  UINT8                   MaxPortSpeed;
  UINT8                   MaxPortLaneListLength;
  UINT8                   MaxPortLaneList[PCIE_MAX_LANE];
  ///
  /// Variables below this line will only contain valid data after the PCIe link
  /// is enabled and completes training
  ///
  BOOLEAN                 EndpointPresent;
  UINT32                  EndpointVendorIdDeviceId;
  UINT8                   EndpointMaxLinkSpeed;
  UINT8                   EndpointMaxLinkWidth;
  PCIE_PORT_SWEQ_DATA     SwEqData;
} PCIE_PORT_INFO;

///
/// Data structure for passing static equalization data for programming
///
typedef struct {
  PCIE_PORT_INFO          *PciePort;
  UINT8                   RootPortPresets[PCIE_MAX_LANE];
  UINT8                   EndpointPresets[PCIE_MAX_LANE];
  UINT8                   EndpointHints[PCIE_MAX_LANE];
} PCIE_PORT_EQS;

///
/// Input Configuration Parameters for Software Equalization Support
///
typedef struct {
  BOOLEAN                 EnableGpioPerstSupport;
  GPIO_PAD                GpioPad;
  BOOLEAN                 GpioActiveHigh;
} PCIE_SWEQ_GPIO_CONFIG;

typedef struct {
  PCIE_PORT_INFO          *PciePort;
  UINT8                   StaticEndpointPresets[PCIE_MAX_LANE];
  UINT8                   LastBootBestPresets[PCIE_MAX_LANE];
  UINT32                  LastBootEndpointVendorIdDeviceId;
  PCIE_SWEQ_GPIO_CONFIG   GpioConfig;
  BOOLEAN                 EnableSwEq;
} PCIE_SWEQ_PORT_INPUT;

typedef struct {
  PCIE_SWEQ_PORT_INPUT    *PerPortInputParameters;
  UINT8                   PerPortInputParametersLength;
  BOOLEAN                 HaveDataFromLastBoot;
  UINT8                   PresetsToTest[PCIE_SWEQ_MAX_PRESETS];
  UINT8                   PresetsToTestLength;
  BOOLEAN                 AlwaysAttemptSwEq;
  UINTN                   JitterDwellTime;
  UINT16                  JitterErrorTarget;
  UINTN                   VocDwellTime;
  UINT16                  VocErrorTarget;
  BOOLEAN                 EnableVocTest;
  BOOLEAN                 EnableBdatScoreSchema;
} PCIE_SWEQ_INPUT_PARAMETERS;

///
/// Data Output from Software Equalization
///
typedef struct {
  INT32                   MarginScore[PCIE_MAX_LANE];
  UINT8                   Preset;
} PCIE_SWEQ_PRESET_SCORE;

typedef struct {
  PCIE_PORT_INFO          *PciePort;
  INT32                   TempMarginData[PCIE_MAX_LANE];
  UINT8                   BestPresets[PCIE_MAX_LANE];
  INT32                   BestScores[PCIE_MAX_LANE];
  PCIE_SWEQ_PRESET_SCORE  PresetScores[PCIE_SWEQ_MAX_PRESETS];
} PCIE_SWEQ_PORT_OUTPUT;

typedef struct {
  PCIE_SWEQ_PORT_OUTPUT   *PortOutputList;
  UINT8                   PortOutputListLength;
  BOOLEAN                 DeferredPlatformResetRequired;
} PCIE_SWEQ_OUTPUT;

typedef enum {
  LaneLevelRxJitter,
  PortLevelRxJitter,
  VocCorners,
  MarginTestTypeMax
} MARGIN_TEST_TYPE;

typedef struct _PCIE_SI_LOW_LEVEL_FUNCTION_CALLS PCIE_SI_LOW_LEVEL_FUNCTION_CALLS;

///
/// PCIe Initialization Library Generic High Level Function Calls
///

/**
  The PCIe Software Equalization algorithm.  Provides an adaptive EQ Phase 3
  implementation in software.

  @param[in]  PcieAccess                  - Low level function table
  @param[in]  PciePorts                   - PCIe Root Ports to wait for
  @param[in]  PciePortsLength             - Length of the PciePorts array
  @param[in]  InputParameters             - Configuration options for SW EQ
  @param[out] OutputData                  - The data that the algorithm generated
**/
VOID
PcieGen3SoftwareEqualization (
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *PcieAccess,
  IN  PCIE_PORT_INFO                    *PciePorts,
  IN  UINT8                             PciePortsLength,
  IN  PCIE_SWEQ_INPUT_PARAMETERS        *InputParameters,
  OUT PCIE_SWEQ_OUTPUT                  *OutputData
  );

/**
  Waits for the Data Link Layer on all given root ports to reach the DL_Active state.
  The user passes a fail mask that indicates which root ports to check.  The function
  will update the fail mask to indicate which root ports successfully trained.

  The fail mask is a bitmap based on PciePorts array indices.  The array must be
  of length 8 or greater since the PciePorts array can have at most 256 entries.

  @param[in]      PcieAccess              - Low level function table
  @param[in]      PciePorts               - PCIe Root Ports to wait for
  @param[in]      PciePortsLength         - Length of the PciePorts array
  @param[in]      ForceCheck              - TRUE to ignore current FailMask and check all root ports
  @param[in, out] FailMask                - Bitmap of root ports to check.  Returns bitmap indicating which root ports
                                            failed to reach DL_Active.  Array must be of length 8 or greater!
**/
VOID
EFIAPI
WaitForDataLinkLayerLinkActiveOnAllPorts (
  IN      PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *PcieAccess,
  IN      PCIE_PORT_INFO                    *PciePorts,
  IN      UINT8                             PciePortsLength,
  IN      BOOLEAN                           ForceCheck,
  IN OUT  UINT32                            *FailMask ///< [PCIE_ROOT_PORT_BITMAP_LENGTH]
  );

/**
  This function prints the time required for DL_Active to be set. Quits after 100 msec.

  @param[in]  This                        - Low level function table
  @param[in]  PciePort                    - PCIe Root Port
**/
VOID
WaitForDataLinkLayerLinkActive (
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *PcieAccess,
  IN  PCIE_PORT_INFO                    *PciePort
  );

/**
  Computes the Pre-Cursor, Cursor, and Post-Cursor from a preset

  @param[in]  Preset                      - Preset to compute coefficients for
  @param[in]  FullSwing                   - The full swing of the transmitter
  @param[out] PreCursor                   - Computed Pre-Cursor
  @param[out] Cursor                      - Computed Cursor
  @param[out] PostCursor                  - Computed Post-Cursor
**/
VOID
GetCoefficientsFromPreset (
  IN  UINT8                             Preset,
  IN  UINT8                             FullSwing,
  OUT UINT8                             *PreCursor,
  OUT UINT8                             *Cursor,
  OUT UINT8                             *PostCursor
  );

/**
  Checks for link speed and width downgrades

  @param[in]  PcieAccess                  - Low level function table
  @param[in]  PciePort                    - PCIe Root Port
  @param[in]  OriginalLinkSpeed           - Original Speed of the Link
  @param[in]  OriginalLinkWidth           - Original Width of the Link
**/
BOOLEAN
LinkIsDowngraded (
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *PcieAccess,
  IN  PCIE_PORT_INFO                    *PciePort,
  IN  UINT8                             OriginalLinkSpeed,
  IN  UINT8                             OriginalLinkWidth
  );


///
/// PCIe Initialization Library Generic Low Level Function Calls
/// All of these functions can be implemented using only PCIe specification
/// level details.  However, it is possible to override the default
/// implementation provided by this library with a Silicon Specific one if needed
///

/**
  This function detects if an endpoint is attached to each given root port and
  if so, reads data from the endpoint and fills in the remaining fields of the
  PCIE_PORT_INFO structure that could not be filled before initial link training

  @param[in]  This                        - Low level function table
  @param[out] PciePorts                   - Array of PCIe Root Ports
  @param[out] PciePortsLength             - Length of the PciePorts array
**/
typedef
VOID
(EFIAPI *PCIE_DETECT_ENDPOINT_PRESENCE)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_PORT_INFO                    *PciePorts,
  IN  UINT8                             PciePortsLength
  );

/**
  Gets the PCIe Capability Structure Pointer

  @param[in]  This                        - Low level function table
  @param[in]  PciePort                    - PCIe Root Port

  @retval Offset to the PCIe Capability Structure
**/
typedef
UINT8
(EFIAPI *PCIE_GET_PCIE_CAP_OFFSET)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_PORT_INFO                    *PciePort
  );

/**
  Checks if the Data Link Layer is in DL_Active state on the given root port

  @param[in]  This                        - Low level function table
  @param[in]  PciePort                    - Root Port to check for DL_Active

  @retval TRUE  - Data Link Layer is in DL_Active state
  @retval FALSE - Data Link Layer is NOT in DL_Active state
**/
typedef
BOOLEAN
(EFIAPI *PCIE_DATA_LINK_LAYER_LINK_ACTIVE)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_PORT_INFO                    *PciePort
  );

/**
  Returns the current value of the PCIe Slot Status Presence Detect bit

  @param[in]  This                        - Low level function table
  @param[in]  PciePort                    - PCIe Root Port

  @retval Slot Presence Detect bit state
**/
typedef
BOOLEAN
(EFIAPI *PCIE_GET_SLOT_PRESENCE_DETECT)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_PORT_INFO                    *PciePort
  );

/**
  Set the Link Disable bit in the PCIe Link Control Register

  @param[in]  This                        - Low level function table
  @param[in]  PciePort                    - PCIe Root Port
  @param[in]  LinkDisable                 - New value for link disable bit
**/
typedef
VOID
(EFIAPI *PCIE_SET_LINK_DISABLE)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_PORT_INFO                    *PciePort,
  IN  BOOLEAN                           LinkDisable
  );

/**
  Retrain the PCIe link

  @param[in]  This                        - Low level function table
  @param[in]  PciePort                    - PCIe Root Port
**/
typedef
VOID
(EFIAPI *PCIE_RETRAIN_LINK)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_PORT_INFO                    *PciePort
  );

/**
  Get Negotiated Link Width

  @param[in]  This                        - Low level function table
  @param[in]  PciePort                    - PCIe Root Port
**/
typedef
UINT8
(EFIAPI *PCIE_GET_NEGOTIATED_WIDTH)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_PORT_INFO                    *PciePort
  );

/**
  Get Current Link Speed

  @param[in]  This                        - Low level function table
  @param[in]  PciePort                    - PCIe Root Port
**/
typedef
UINT8
(EFIAPI *PCIE_GET_CURRENT_LINK_SPEED)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_PORT_INFO                    *PciePort
  );

///
///  PCIe Initialization Library Silicon Specific Low Level Function Calls
///  Enables Abstraction of Silicon details keeping this library generic
///

/**
  This function determines if the silicon implements the PCIe bus interface
  that this instance of PCIE_SI_LOW_LEVEL_FUNCTION_CALLS is intended for.

  @retval TRUE - Silicon supports the bus interface
  @retval FALSE - otherwise
**/
typedef
BOOLEAN
(EFIAPI *PCIE_EXISTS)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This
  );

/**
  This function determines the topology of the PCIe bus interface that is being
  initialized using silicon defined mechanisms.  The PciePorts pointer must
  point to a pre-allocated array which is capable of containing the maximum
  number of root ports that this function will return.  Generally this is done
  by a component specific entrypoint that can allocate the array on the stack
  using a fixed size appropriate for the HW.  If this needs to be called from
  generic code, the generic code must allocate a buffer that can contain 256
  entries (which should be avoided.)

  @param[in]  This                        - Low level function table
  @param[out] PciePorts                   - Array of Detected PCIe Root Ports
  @param[out] PciePortsLength             - Length of the PciePorts array
**/
typedef
VOID
(EFIAPI *PCIE_GET_ROOT_PORTS)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  OUT PCIE_PORT_INFO                    *PciePorts,
  OUT UINT8                             *PciePortsLength
  );

/**
  Programs static equalization settings for the given list of PCIe root ports.
  The PCIE_PORT_EQs structure is laid out such that the Root Port preset for
  PHYSICAL lane number PciePortEqs->PciePort->MaxPortLaneList[0] is
  PciePortEqs->RootPortPresets[0].  Note that physical lane numbers may not
  start at or include zero.  Package pin 0 may not be mapped to a given Root Port

  @param[in]  This                        - Low level function table
  @param[in]  PciePortEqs                 - Array of Root Ports + Eqs to program
  @param[in]  PciePortEqsLength           - Number of Root Ports to program
**/
typedef
VOID
(EFIAPI *PCIE_PROGRAM_STATIC_GEN3_EQ)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_PORT_EQS                     *PciePortEqs,
  IN  UINT8                             PciePortEqsLength
  );

/**
  Sets Gen3 Equalization Phase 2 Bypass for all given Root Ports

  @param[in]  This                        - Low level function table
  @param[in]  PciePorts                   - PCIe Root Ports to program Phase2 for
  @param[in]  PciePortsLength             - Length of the PciePorts array
  @param[in]  BypassPhase2                - TRUE to enable Phase2 bypass, FALSE otherwise

  @retval EFI_SUCCESS     - Phase 2 bypass was successful
  @retval EFI_UNSUPPORTED - Hardware does not support the given Phase2 bypass request
**/
typedef
EFI_STATUS
(EFIAPI *PCIE_SET_GEN3_PHASE2_BYPASS)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_PORT_INFO                    *PciePorts,
  IN  UINT8                             PciePortsLength,
  IN  BOOLEAN                           BypassPhase2
  );

/**
 This function reports a PCIe controller's link status

  @param[in]  This                        - Low level function table
  @param[in]  PciePort                    - PCIe Root Port
**/
typedef
VOID
(EFIAPI *PCIE_REPORT_LINK_STATUS)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_PORT_INFO                    *PciePort
  );

///
/// PCIe Link Recovery Functions
///

/**
  Wait until link is up.

  @param[in]  This                        - Low level function table
  @param[in]  PciePort                    - PCIe Root Port

  @retval EFI_SUCCESS     - Completed successfully before timeout
  @retval EFI_TIMEOUT     - Timed out
**/
typedef
EFI_STATUS
(EFIAPI *PCIE_WAIT_FOR_L0)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_PORT_INFO                    *PciePort
  );

/**
  Get Target Link Speed

  @param[in]  This                        - Low level function table
  @param[in]  PciePort                    - PCIe Root Port
**/
typedef
UINT8
(EFIAPI *PCIE_GET_TARGET_LINK_SPEED)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_PORT_INFO                    *PciePort
  );

/**
  Set Target Link Speed

  @param[in]  This                        - Low level function table
  @param[in]  PciePort                    - PCIe Root Port
  @param[in]  TargetLinkSpeed             - Target Link Speed
**/
typedef
VOID
(EFIAPI *PCIE_SET_TARGET_LINK_SPEED)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_PORT_INFO                    *PciePort,
  IN  UINT8                             TargetLinkSpeed
  );

/**
  Resets the endpoint connected to the given root port by directly pulsing the
  PERST# signal.  The minimum assertion time, T_PERST (100 usec), is defined in
  the PCIe CEM Specification.

  @param[in]  This                        - Low level function table
  @param[in]  PciePort                    - PCIe Root Port
  @param[in]  InputParameters             - SW EQ Input Parameters

  @retval EFI_SUCCESS            - GPIO set successfully
  @retval EFI_UNSUPPORTED        - GPIO is not supported
  @retval EFI_INVALID_PARAMETER  - GPIO pin number is invalid
  @retval EFI_TIMEOUT            - Link did not train after pulsing PERST#
**/
typedef
EFI_STATUS
(EFIAPI *PCIE_RESET_ENDPOINT_PERST)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_PORT_INFO                    *PciePort,
  IN  PCIE_SWEQ_INPUT_PARAMETERS        *InputParameters
  );

/**
  This function asserts/deasserts a GPIO that controls PERST#.  The minimum
  assertion time, T_PERST (100 usec), is defined in the PCIe CEM Specification.

  @param[in]  This                        - Low level function table
  @param[in]  PciePort                    - PCIe Root Port
  @param[in]  InputParameters             - SW EQ Input Parameters
  @param[in]  AssertPerst                 - TRUE to assert PERST#, FALSE to deassert

  @retval EFI_SUCCESS            - GPIO set successfully
  @retval EFI_UNSUPPORTED        - GPIO is not supported
  @retval EFI_INVALID_PARAMETER  - GPIO pin number is invalid
**/
typedef
EFI_STATUS
(EFIAPI *PCIE_SET_PERST)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_PORT_INFO                    *PciePort,
  IN  PCIE_SWEQ_INPUT_PARAMETERS        *InputParameters,
  IN  BOOLEAN                           AssertPerst
  );

/**
  Recovers a link width downgrade back to the original width.  Generally this
  doesn't need to be called directly since EnsureLinkIsHealthy() checks link
  width in addition to other link health checks.

  @param[in]  This               - Low level function table
  @param[in]  PciePort           - PCIe Root Port
  @param[in]  OriginalLinkWidth  - Original Link Width

  @retval EFI_SUCCESS            - Link is running at the correct width
  @retval EFI_DEVICE_ERROR       - Unable to correct link width downgrade
  @retval EFI_TIMEOUT            - Link did not successfully retrain
**/
typedef
EFI_STATUS
(EFIAPI *PCIE_RECOVER_LINK_WIDTH)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_PORT_INFO                    *PciePort,
  IN  UINT8                             OriginalLinkWidth
  );

/**
  This function sets a GPIO to a particular level.

  @param[in] This                - Low level function table
  @param[in] GpioPad             - PCH GPIO Pad
  @param[in] Level               - 0 = Low, 1 = High

  @retval EFI_SUCCESS            - GPIO set successfully
  @retval EFI_UNSUPPORTED        - GPIO is not supported
  @retval EFI_INVALID_PARAMETER  - GPIO pin number is invalid
**/
typedef
EFI_STATUS
(EFIAPI *PCIE_SET_PCH_GPIO)(
  IN PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN GPIO_PAD                          GpioPad,
  IN UINT8                             Level
  );

/**
  Check the status of the given PCIe link, detect and correct and downgrades

  @param[in]  This                          - Low level function table
  @param[in]  InputParameters               - SW EQ Input Parameters
  @param[in]  PciePort                      - PCIe Root Port
  @param[in]  OriginalLinkSpeed             - Expected speed of the PCIe link
  @param[in]  OriginalLinkWidth             - Expected width of the PCIe link
  @param[out] DeferredPlatformResetRequired - A platform reset is needed after saving Eq data to NVRAM

  @retval EFI_SUCCESS            - Link is running at the correct speed/width
  @retval EFI_UNSUPPORTED        - Unable to correct failure due to lack of GPIO PERST# support
  @retval EFI_INVALID_PARAMETER  - Unable to correct failure because the GPIO pin number is invalid
  @retval EFI_DEVICE_ERROR       - Unable to correct link downgrade
  @retval EFI_TIMEOUT            - Link did not successfully retrain
**/
typedef
EFI_STATUS
(EFIAPI *PCIE_ENSURE_LINK_IS_HEALTHY)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_SWEQ_INPUT_PARAMETERS        *InputParameters,
  IN  PCIE_PORT_INFO                    *PciePort,
  IN  UINT8                             OriginalLinkSpeed,
  IN  UINT8                             OriginalLinkWidth,
  OUT BOOLEAN                           *DeferredPlatformResetRequired
  );

///
/// PCIe Error Counting Functions
///

/**
  Open port for monitor

  @param[in]  This                          - Low level function table

  @retval Monitor Port
**/
typedef
UINT32
(EFIAPI *PCIE_OPEN_MONITOR)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This
  );

/**
  Close port for monitor

  @param[in]  This                          - Low level function table
  @param[in]  MonitorPort                   - Monitor Port
**/
typedef
VOID
(EFIAPI *PCIE_CLOSE_MONITOR)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN UINT32                             MonitorPort
  );

/**
  Get Current Error Count

  @param[in]  This                          - Low level function table
  @param[in]  MonitorPort                   - Monitor Port
  @param[in]  PciePort                      - PCIe Root Port
**/
typedef
UINT32
(EFIAPI *PCIE_GET_ERROR_COUNT)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  UINT32                            MonitorPort,
  IN  PCIE_PORT_INFO                    *PciePort
  );

/**
  Clear Current Error Count for all Root Ports

  @param[in]  This                          - Low level function table
  @param[in]  MonitorPort                   - Monitor Port
**/
typedef
VOID
(EFIAPI *PCIE_CLEAR_ERROR_COUNT)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  UINT32                            MonitorPort
  );

/**
 Enable or Disable Polling Compliance Mode

  @param[in]  This                        - Low level function table
  @param[in]  PciePorts                   - PCIe Root Ports
  @param[in]  PciePortsLength             - Length of PciePorts array
  @param[in]  Enable                      - TRUE to enable, FALSE to disable
**/
typedef
EFI_STATUS
(EFIAPI *PCIE_POLLING_COMPLIANCE_MODE)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_PORT_INFO                    *PciePorts,
  IN  UINT8                             PciePortsLength,
  IN  BOOLEAN                           Enable
  );

/**
  Program TxEQs on the endpoint attached to the given root port.

  @param[in]  This                        - Low level function table
  @param[in]  PciePort                    - PCIe Root Port
  @param[in]  Presets                     - Array of presets to program per lane
                                            must be of sufficient length to program all lanes
**/
typedef
VOID
(EFIAPI *PCIE_PROGRAM_PORT_PHASE3_TXEQ)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_PORT_INFO                    *PciePort,
  IN  UINT8                             *Presets///[PCIE_MAX_LANE]
  );

/**
  Program the same TxEQ to all lanes on the endpoint attached to the given root port.

  @param[in]  This                        - Low level function table
  @param[in]  PciePort                    - PCIe Root Port
  @param[in]  Preset                      - Preset to program
**/
typedef
VOID
(EFIAPI *PCIE_PROGRAM_UNIFORM_PORT_PHASE3_TXEQ)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_PORT_INFO                    *PciePort,
  IN  UINT8                             Preset
  );

/**
  Runs a Margin Test on the specified root ports.  The MarginData parameter must
  be an array with capacity of PciePortsLength elements or more.

  @param[in]  This                          - Low level function table
  @param[in]  PciePorts                     - PCIe Root Ports to margin
  @param[in]  PciePortsLength               - Length of the PciePorts array
  @param[in]  InputParameters               - SW EQ Input Parameters
  @param[in]  MonitorPort                   - Monitor Port
  @param[in]  MarginTest                    - Type of Margin Test to Run
  @param[out] MarginData                    - Margin Data, must be array of size >= PciePortsLength
  @param[out] DeferredPlatformResetRequired - A platform reset is needed after saving Eq data to NVRAM

  @retval EFI_SUCCESS            - Margin Data Generated Successfully
**/
typedef
EFI_STATUS
(EFIAPI *PCIE_RUN_MARGIN_TEST)(
  IN  PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *This,
  IN  PCIE_PORT_INFO                    *PciePorts,
  IN  UINT8                             PciePortsLength,
  IN  PCIE_SWEQ_INPUT_PARAMETERS        *InputParameters,
  IN  UINT32                            MonitorPort,
  IN  MARGIN_TEST_TYPE                  MarginTest,
  OUT PCIE_SWEQ_PORT_OUTPUT             *MarginData,
  OUT BOOLEAN                           *DeferredPlatformResetRequired
  );

struct _PCIE_SI_LOW_LEVEL_FUNCTION_CALLS {
  VOID                                  *PrivateData;

  ///
  /// Generic Low Level
  ///
  PCIE_DETECT_ENDPOINT_PRESENCE         DetectEndpointPresence;
  PCIE_GET_PCIE_CAP_OFFSET              GetPcieCapOffset;
  PCIE_DATA_LINK_LAYER_LINK_ACTIVE      DataLinkLayerLinkActive;
  PCIE_GET_SLOT_PRESENCE_DETECT         GetSlotPresenceDetect;
  PCIE_SET_LINK_DISABLE                 SetLinkDisable;
  PCIE_RETRAIN_LINK                     RetrainLink;
  PCIE_GET_NEGOTIATED_WIDTH             GetNegotiatedWidth;
  PCIE_GET_CURRENT_LINK_SPEED           GetCurrentLinkSpeed;
  PCIE_GET_TARGET_LINK_SPEED            GetTargetLinkSpeed;
  PCIE_SET_TARGET_LINK_SPEED            SetTargetLinkSpeed;
  PCIE_SET_PERST                        SetPerst;
  PCIE_ENSURE_LINK_IS_HEALTHY           EnsureLinkIsHealthy;

  ///
  /// Silicon Specific Low Level
  ///
  PCIE_EXISTS                           PcieExists;
  PCIE_GET_ROOT_PORTS                   GetPcieRootPorts;
  PCIE_PROGRAM_STATIC_GEN3_EQ           ProgramStaticGen3Eq;
  PCIE_SET_GEN3_PHASE2_BYPASS           SetPhase2Bypass;
  PCIE_REPORT_LINK_STATUS               ReportPcieLinkStatus;
  PCIE_WAIT_FOR_L0                      WaitForL0;
  PCIE_RESET_ENDPOINT_PERST             ResetEndpointPerst;
  PCIE_RECOVER_LINK_WIDTH               RecoverLinkWidth;
  PCIE_SET_PCH_GPIO                     SetPchGpio;
  PCIE_OPEN_MONITOR                     OpenMonitor;
  PCIE_CLOSE_MONITOR                    CloseMonitor;
  PCIE_GET_ERROR_COUNT                  GetErrorCount;
  PCIE_CLEAR_ERROR_COUNT                ClearErrorCount;
  PCIE_POLLING_COMPLIANCE_MODE          PollingComplianceMode;
  PCIE_PROGRAM_PORT_PHASE3_TXEQ         ProgramPortPhase3TxEq;
  PCIE_PROGRAM_UNIFORM_PORT_PHASE3_TXEQ ProgramUniformPortPhase3TxEq;
  PCIE_RUN_MARGIN_TEST                  RunMarginTest;
};

/**
  Find the Offset to a given Capabilities ID
  CAPID list:
    - 0x01 = PCI Power Management Interface
    - 0x04 = Slot Identification
    - 0x05 = MSI Capability
    - 0x10 = PCI Express Capability

  @param[in] Bus       -   PCI Bus Number
  @param[in] Device    -   PCI Device Number
  @param[in] Function  -   PCI Function Number
  @param[in] CapId     -   CAPID to search for

  @retval 0       - CAPID not found
  @retval Other   - CAPID found, Offset of desired CAPID
**/
UINT32
PcieLibFindCapId (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function,
  IN UINT8   CapId
  );

/**
  This function gets the table of generic low level function calls for the PCIe
  interface.  These function calls use PCIe spec defined mechanisms and can be
  overrided by a silicon specific implementation if needed.

  @param[out] PcieLowLevel - Table of function calls for PCIe

  @retval EFI_SUCCESS - Table of function calls returned successfully
**/
EFI_STATUS
GetGenericPcieLowLevelFunctionCalls (
  OUT PCIE_SI_LOW_LEVEL_FUNCTION_CALLS  *PcieLowLevelFunctionCalls
  );

#endif
