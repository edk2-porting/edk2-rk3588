# Overview
* **Feature Name:** S3
* **PI Phase(s) Supported:** PEI
* **SMM Required?** Yes

More Information:
* [A Tour Beyond BIOS - Implementing S3 Resume with EDK II](https://github.com/tianocore-docs/Docs/raw/master/White_Papers/A_Tour_Beyond_BIOS_Implementing_S3_resume_with_EDKII_V2.pdf)
* [UEFI ACPI & Platform Initialization (PI Specification)](https://uefi.org/specifications)

## Purpose
The S3 feature provides the set of firmware resources and board enabling instructions to enable S3 firmware feature
support in a board package. S3 resume is a power saving feature defined in the Advanced Configuration and Power
Interface (ACPI) specification. The ACPI specification defines a set of power state transitions. In the context of this
feature, S3 refers to the G1 sleeping - S3 sleeping state.

**Note: The S3 feature is currently not complete. Additional functionality will be enabled in the future.**

# High-Level Theory of Operation
*_TODO_*
A description of how the device works at a high-level.

The description should not be constrained to implementation details but provide a simple mental model of how the
feature is supposed to work.

## Firmware Volumes
*_TODO_*
A bulleted list of the firmware volumes that feature module(s) are placed in.

## Modules
*_TODO_*
A bulleted list of the modules that make up the feature.

## <Module Name>
*_TODO_*
Each module in the feature should have a section that describes the module in a level of detail that is useful
to better understand the module source code.

## <Library Name>
*_TODO_*
Each library in the feature should have a section that describes the library in a level of detail that is useful
to better understand the library source code.

## Key Functions
*_TODO_*
A bulleted list of key functions for interacting with the feature.

Not all features need to be listed. Only functions exposed through external interfaces that are important for feature
users to be aware of.

## Configuration
*_TODO_*
Information that is useful for configuring the feature.

Not all configuration options need to be listed. This section is used to provide more background on configuration
options than possible elsewhere.

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
