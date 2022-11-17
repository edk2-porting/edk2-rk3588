# Overview
* **Feature Name:** ACPI Debug
* **PI Phase(s) Supported:** DXE and SMM
* **SMM Required?** No

## Purpose
This feature is an alternative ACPI debug mechanism to port 0x80 or a kernel debugger such as WinDBG.

**_DO NOT_** use this feature in a production system. It may not handle SMM memory buffers or data input/output
from SMM in a safe manner.

# High-Level Theory of Operation
There are two driver modes:
  1. DXE - ACPI debug messages should be manually read from a memory buffer on the target machine using a utility
     that has the ability to read main memory.
  2. SMM - ACPI debug messages should be read from the firmware debug message output port.

The DXE driver is required and the SMM driver is optional. The SMM driver eases retrieval of the ACPI debug messages
from a message ring buffer in memory by sending the messages over the SMM debug mechanism. ASL code writes messages up
to 32 characters in length (shorter strings will be padded with zeroes and longer strings will be truncated) to an
ASL debug method.

## Firmware Volumes
* FvAdvancedPostMemory

## Modules
* AcpiDebugDxe
* AcpiDebugSmm

## AcpiDebugDxe
The entry point registers an end of DXE notification. Further action is deferred until end of DXE to allow the
feature PCDs to be customized at boot time in DXE if desired. The notification handler allocates a `EfiReservedMemoryType`
buffer (memory not available to the operating system) of the size specified in `PcdAcpiDebugBufferSize`. The actual
buffer size is allocated on a page boundary of size `EFI_PAGE_SIZE`. The allocated buffer address is written out
as a debug message `AcpiDebugAddress - 0xXXXXXXXX`. In addition, the address is written to `PcdAcpiDebugAddress`
and the actual allocation size is written to `PcdAcpiDebugBufferSize`.

To expose the ACPI debug buffer to ASL code, an ACPI debug SSDT (defined in `AcpiDebug.asl`) is installed. The pointer
fields in the SSDT are patched by `AcpiDebugDxe` to the actual buffer address. The SSDT is installed using the
`EFI_ACPI_TABLE_PROTOCOL`.

## AcpiDebugSmm
The entry point registers an end of DXE notification. Further action is deferred until end of DXE to allow the
feature PCDs to be customized at boot time if desired. The notification handler registers a SW SMI that can be
triggered in ACPI debug SSDT to invoke the SMI handler `AcpiDebugSmmCallback ()`. The SMI handler retrieves the debug
message from the buffer at `PcdAcpiDebugAddress` and sends it to the `DEBUG` function for the given SMM `DebugLib`
instance assigned to `AcpiDebugSmm`.

## Key Functions
* `MDBG` _(ASL method)_

  This method is given a single argument with a number or string to write
  to the ACPI memory debug buffer. If AcpiDebugSmm is used, an SMI will
  be used to send the message as a DEBUG message.

  It is recommended to instrument the ASL code with a method called `ADBG`. An ASL caller sends a debug
  message as follows:

  ```
  ADBG("This is a test.")
  ADBG(Arg0)
  ```

* `ADBG` _(ASL method)_ is intended to be a wrapper of `MDBG` that allows the `ADBG` references to remain in the ASL code even if
  the ACPI debug advanced feature is disabled. Below is a code snippet with a sample implementation for `ADBG`.

  ```
  External (MDBG, MethodObj)
  Method (ADBG, 1, Serialized)
  {
    If (CondRefOf (MDBG)) // Check if ACPI Debug SSDT is loaded
    {
      Return (MDBG (Arg0))
    }
    Return (0)
  }
  ```

## Configuration
* PcdAcpiDebugEnable - Enables this feature.
* PcdAcpiDebugFeatureActive - Activates this feature.
* PcdAcpiDebugAddress - The address of the ACPI debug message buffer.
* PcdAcpiDebugBufferSize - The size of the ACPI debug message buffer.

## Data Flows
*_TODO_*
Architecturally defined data structures and flows for the feature.

## Control Flows
*_TODO_*
Key control flows for the feature.

## Build Flows
*_TODO_*
Any special build flows should be described in this section.

This is particularly useful for features that use custom build tools or require non-standard tool configuration. If the
standard flow in the feature package template is used, this section may be empty.

## Test Point Results
*_TODO_*
The test(s) that can verify porting is complete for the feature.

Each feature must describe at least one test point to verify the feature is successful. If the test point is not
implemented, this should be stated.

## Functional Exit Criteria
*_TODO_*
The testable functionality for the feature.

This section should provide an ordered list of criteria that a board integrator can reference to ensure the feature is
functional on their board.

## Feature Enabling Checklist
*_TODO_*
An ordered list of required activities to achieve desired functionality for the feature.

## Performance Impact
A general expectation for the impact on overall boot performance due to using this feature.

This section is expected to provide guidance on:
* How to estimate performance impact due to the feature
* How to measure performance impact of the feature
* How to manage performance impact of the feature

## Common Optimizations
*_TODO_*
Common size or performance tuning options for this feature.

This section is recommended but not required. If not used, the contents should be left empty.
