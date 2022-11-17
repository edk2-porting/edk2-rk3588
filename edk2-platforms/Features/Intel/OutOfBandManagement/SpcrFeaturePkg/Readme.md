# Overview
* **Feature Name:** Serial Port Console Redirection Table (SPCR)
* **PI Phase(s) Supported:** DXE
* **SMM Required?** No

More Information:
* [Serial Port Console Redirection Table (SPCR)](https://docs.microsoft.com/en-us/windows-hardware/drivers/serports/serial-port-console-redirection-table)

## Purpose
The SPCR feature provides the functionality that select a serial port device and fill its info to ACPI table.
Windows can get this info and use this serial port as input/output device in Microsoft? Windows? Emergency Management Services (EMS).

# High-Level Theory of Operation
A dxe driver will call a library function to get the serial port device, and produces the ACPI table.
Platform can implement its own library to select the serial port device.

## Firmware Volumes
* PostMemory

## Modules
* SpcrAcpiDxe
* SpcrDeviceLibNull

## SpcrAcpiDxe
The main driver of convert the serial port device info to ACPI table.
It will call the function GetSpcrDevice() in the library SpcrDeviceLib to get the serial port device path.
If the serial port device path is not NULL, then use gEfiPciIoProtocolGuid to get the PCI info, and use the gEfiSerialIoProtocolGuid to get the serial port info, such as the baud rate etc.
If the serial port device is PCI device 00:16:03 (AMT serial over lan PCI device), then will set the BaseAddress.
At last fill the ACPI table as Windows required.

## SpcrDeviceLibNull
A NULL implemention of library SpcrDeviceLib, it return NULL for function GetSpcrDevice().
Platform maybe implemente its own SpcrDeviceLib.

## Key Functions
Fill the serial port device info used for SPCR to ACPI table.

## Configuration
There is not special configuration.
But the platform maybe implemente its own SpcrDeviceLib.

## Data Flows
GetSpcrDevice() -> serial port device path -> get PCI info and serial port info -> ACPI table.

## Control Flows
GetSpcrDevice() in SpcrDeviceLib -> SpcrAcpiDxe.

## Build Flows
There is not special build flow.

## Test Point Results
* Check the output ACPI table.

## Functional Exit Criteria
* Check the output ACPI table.

## Feature Enabling Checklist
There is not special PCD to enable this feature.

## Performance Impact
The SpcrAcpiDxe.efi with SpcrDeviceLibNull debug version size: 1.4KB, release version size: 1.12KB.

## Common Optimizations
N/A now.
