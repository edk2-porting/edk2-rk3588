# Overview
* **Feature Name:** System Management BIOS (SMBIOS)
* **PI Phase(s) Supported:** DXE
* **SMM Required?** No

More Information:
* [SMBIOS Reference Specification](https://www.dmtf.org/standards/smbios)

## Purpose
System Management BIOS (SMBIOS) is an industry standard for delivering management information via system firmware.
SMBIOS provides a standard format to present management information across various environments including OS-present,
OS-absent, and pre-OS environments. SMBIOS was originally designed for Intel&reg; processor architecture systems, SMBIOS
now includes support for IA-32 (x86), x64 (x86-64, Intel64, AMD64, EM64T), Intel&reg; Itanium&reg; architecture, 32-bit
ARM (Aarch32) and 64-bit ARM (Aarch64).

The SMBIOS feature includes generic firmware functionality to implement the SMBIOS reference specification. Since the
information required by these modules is very platform/board-specific by nature this feature requires a high-level of
customization (mostly in the form of PCD values) by the feature consumer.

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
