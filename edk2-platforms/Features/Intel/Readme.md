# **EDK II Minimum Platform Firmware Advanced Features for Intel&reg; Platforms**

In the EDK II Minimum Platform, advanced features are non-essential features. Essential features are those required
to achieve an earlier boot stage (Stage I through Stage V). The Minimum Platform boot stages are defined in the
[EDK II Minimum Platform Draft Specification](https://edk2-docs.gitbooks.io/edk-ii-minimum-platform-specification/). A
brief overview is also provided in the [Platform/Intel/Readme.md](https://github.com/tianocore/edk2-platforms/blob/master/Platform/Intel/Readme.md).

An advanced feature must be implemented as highly cohesive and stand-alone software to only support a specific
feature. Advanced features are the primary method to extend platform firmware capabilities in a modular fashion.

If you would like to quickly get started creating a new advanced feature, you can jump ahead to the
[Add New Advanced Feature Checklist](#Add-New-Advanced-Feature-Checklist) and reference other sections of this
document as needed.

## Overview

### Advanced Feature Attributes
Advanced features should be:
* _Cohesive_, the feature should not contain any functionality unrelated to the feature.
* _Complete_, the feature must have a complete design that minimizes dependencies. A feature package can only depend
  on following packages:
  * edk2 repo: `MdePkg`, `MdeModulePkg`, `UefiCpuPkg`, `FmpDevicePkg`, `SecurityPkg`, `NetworkPkg`, `ShellPkg`.
  * edk2-platforms repo: `MinPlatformPkg`, `IntelSiliconPkg`.
* _Easy to Integrate_, the feature should expose well-defined software interfaces to use and configure the feature.
  * It should also present a set of simple and well-documented standard EDK II configuration options such as PCDs to
  configure the feature.
  * In general, features should be self-contained and started by the dispatcher. The board firmware should
    be required to perform as few steps as possible to enable the feature.
  * All features are required to have a feature enable PCD (`PcdFeatureEnable`). Any effort to enable the feature
    besides this PCD should be carefully considered. Default configuration values should apply to the common case.
* _Portable_, the feature is not allowed to depend on other advanced feature or board source code packages. For example,
  if Feature A depends on output Feature B, a board integration module should use a generic interface in Feature A
  to get the output and pass it to a generic interface in Feature B. Structures should not be shared between feature
  packages. Most structures should be defined in a common package such as MdePkg if the structure is industry standard,
  IntelSiliconPkg if the structure is specific to Intel silicon initialization, etc. Feature-specific structures are
  of course allowed to be defined within a feature package and used by libraries and modules in that package.
* _Self Documenting_, the feature should follow software best practices to allow others to debug the code and
  contribute changes. In addition to source code, advanced features must have a Readme.md with sufficient
  information for a newcomer to understand the feature.
* _Single Instance_, the feature should not have more than one instance of a source solution. If an existing feature
  package does not solve a specific instance of a problem for the feature, the feature package should be re-worked
  to consider new requirements instead of duplicating feature code.

  Features should be written for a specific feature technology. Outside of technology restrictions, the feature
  should not make arbitrary assumptions about the type of board or system that may integrate the feature. Any
  board or hardware-specific details that may vary in design should be given to the feature through a defined
  and documented software interface.

### Advanced Feature Packages
Feature portability is a key aspect of board scalability. To maintain high cohesion within a feature package and
reduce potential coupling between features, each feature is maintained in a dedicated package. Such encapsulation enables
increased flexibility in implementation, rapid integration with board firmware, improved software maintenance by reducing
coupling between features, and a better scope for feature-focused maintenance and testing over time.

Two package types exist for advanced features:
 * AdvancedFeaturePkg - A special package described further in [AdvancedFeaturePkg](#AdvancedFeaturePkg).
 * XxxFeaturePkg - A specific feature package described further in [XxxFeaturePkg](#XxxFeaturePkg).

#### AdvancedFeaturePkg
A single package called `AdvancedFeaturePkg` exists with the following responsibilities:
1. Present a consolidated and simplified view of all available advanced features to board packages.
2. Provide a single package build for all available advanced features.
  2.a. This can save time. Each feature package will send build output to a package-specific build output directory.
       By building a features from a single package build object files can be reused across features.

##### Template Resources
A template for a new advanced feature package is provided in `TemplateFeaturePkg`. It is recommended to start
a new advanced feature package using this template. The minimally required set of files for an advanced feature to
be acceptable is defined in [Advanced Feature Collateral](#Advanced-Feature-Collateral). Apart from required files,
the content organization within the files should follow the layout provided in the template files.

##### Consolidated View of All Advanced Features to Board Packages
A board package may consume any number of advanced features. In order to reduce the overall effort required to add
advanced features, all available advanced features are consolidated into single includable files in `AdvancedFeaturePkg`.

_DSC File_

All advanced features are importable to a board package DSC file by simply including `AdvancedFeatures.dsc`.
`AdvancedFeatures.dsc` already includes the conditional logic to only include features that are enabled by the board package.

_FDF File_

The EDK II Minimum Platform has two firmware volumes to contain advanced features.

1. `FvAdvancedPreMemory` - Contains advanced feature modules that should be dispatched before permanent memory is enabled.
   Typically, these are PEI modules (PEIMs).
2. `FvAdvanced` - Contains advanced feature modules that do not need to be executed prior to permanent memory being
   initialized. Typically, these are post-memory PEIMs or DXE modules.

##### Single Build of All Advanced Features
With many advanced feature packages, it is convenient to have a single build target that can build all advanced features.
The `AdvancedFeaturePkg.dsc` enables all the advanced features for its package build.

#### XxxFeaturePkg
Each feature is maintained in its own feature package called `XxxFeaturePkg` where *Xxx* is replaced by the feature
name.

### Source Code Organization of Advanced Features
All advanced feature code is maintained in `edk2-platforms/Features`. Features that are only tested on Intel systems
are maintained in `edk2-platforms/Features/Intel`. These features are not intended to be constrained to Intel systems.

A package resides at the root of `edk2-platforms/Features/Intel` called [AdvancedFeaturePkg](#AdvancedFeaturePkg).
All feature packages are organized into directories by feature domain. Each feature domain directory is required to
have a `Readme.md` that explains the scope of features for that domain. Each feature package is required to have a
`Readme.md` that explain the feature. All feature packages are required to base their `Readme.md` on the
[template](TemplateFeaturePkg/Readme.md) provided in `TemplateFeaturePkg`.

A generic tree layout of the advanced features is illustrated below. The contents are shown at the time of writing but
of course, are subject to change over time.

  <pre>
    WORKSPACE
          |------edk2
          |------edk2-non-osi
          |------edk2-platforms
          |       |---Features
          |       |    |--Intel
          |       |        |------AdvancedFeaturePkg
          |       |        |
          |       |        |------TemplateFeaturePkg
          |       |        |
          |       |        |------Debugging: Debug related advanced features
          |       |        |       |------AcpiDebugFeaturePkg
          |       |        |       |       |---AcpiDebugDxeSmm (module directories)
          |       |        |       |       |---.  .  .
          |       |        |       |       |---Include
          |       |        |       |       |    |---AcpiDebugFeature.dsc (feature build DSC file)
          |       |        |       |       |    |---PostMemory.fdf (post-memory feature modules)
          |       |        |       |       |    |---PreMemory.fdf (pre-memory feature modules)
          |       |        |       |       |    |---.  .  .
          |       |        |       |       |---AcpiDebugFeaturePkg.dec (feature package DEC file)
          |       |        |       |       |---AcpiDebugFeature.dsc (feature package build DSC file)
          |       |        |       |
          |       |        |       |------Usb3DebugFeaturePkg
          |       |        |       |       |---.  .  .
          |       |        |       |
          |       |        |       |------.  .  .
          |       |        |------Network: Network related advanced features
          |       |        |       |------.  .  .
          |       |        |
          |       |        |------OutOfBandManagement: Out-of-Band Management related advanced features
          |       |        |       |------.  .  .
          |       |        |
          |       |        |------PowerManagement: Power Management related advanced features
          |       |        |       |------.  .  .
          |       |        |
          |       |        |------SystemInformation: System Information related advanced features
          |       |        |       |------.  .  .
          |       |        |
          |       |        |------UserInterface: User Interface related advanced features
          |       |        |       |------.  .  .
          |       |        |
          |------FSP
  </pre>

## Adding a New Advanced Feature

### Advanced Feature Collateral
At a minimum, an advanced feature must consist of the following elements:
1. A feature package directory (`XxxFeaturePkg`)
2. A `Readme.md` file in the feature package directory root to describe the feature
3. Some advanced feature source code (e.g. a PEI or DXE module)
4. A feature DSC file (`XxxFeaturePkg/Include/XxxFeature.dsc`)
5. Feature pre-memory / post-memory FDF files (`XxxFeaturePkg/Include/PreMemory.fdf` & `XxxFeaturePkg/Include/PostMemory.fdf`)
6. A feature package DSC file (`XxxFeaturePkg/XxxFeaturePkg.dsc`)
7. A feature package DEC file (`XxxFeaturePkg/XxxFeaturePkg.dec`)
8. A reference in the `AdvancedFeaturePkg` as described in the [checklist](#Add-New-Advanced-Feature-Checklist)

### Add New Advanced Feature Checklist
1. Check if a feature package already exists for your new feature.
    * If it does, use the existing feature package. If changes are needed to meet your requirements, work with the
      package maintainer to make any required updates to the existing package.

2. Determine the functional domain of your feature. For example, "Debugging", "Power Management", etc. This should not be
   very specific and abstracted so the feature is available alongside similar feature packages for the given domain. It
   is preferred to keep the number of feature domain directories minimal but new directories can be created if needed.

3. Decide on a succinct name for your feature package.
    * The name should be descriptive enough to uniquely identify the feature from similar features in the same feature
    domain.

    * At this point, the feature package location is: `FeatureDomain/<Xxx>FeaturePkg`.

    * For example, the ACPI Debug feature is located in `Debugging/AcpiDebugFeaturePkg` based on this naming pattern.

4. Use the package template provided by `TemplateFeaturePkg` to create a new advanced feature package in the
   feature location.

5. Review the [advanced feature attributes](#Advanced-Feature-Attributes) to draft a feature design proposal.
    * It is recommended to use the template in `TemplateFeaturePkg/Readme.md` to document the feature design for review.
    Once the design review is approved, a subsequent review can be sent for the feature implementation. An advantage to
    this approach is that the requirement to complete a Readme.md file for the feature and gain approval in a feature
    design review are combined into a single step.

6. Add the source code for the advanced feature.
    * If the feature is large it is recommended to add libraries in one patch and then modules in a following patch.

7. Update the feature DSC file. This file is in `XxxFeaturePkg/Include/XxxFeature.dsc`.
    * In most cases, `XxxFeaturePkg/XxxFeaturePkg.dsc` should just `!include XxxFeaturePkg/Include/XxxFeature.dsc`.

8. Update the feature FDF files. These files are `XxxFeaturePkg/Include/PreMemory.fdf` and
   `XxxFeaturePkg/Include/PostMemory.fdf`.
    * Each file should contain the feature pre-memory modules and post-memory modules respectively.

9. Build the advanced feature package to ensure the build is successful:

    From the workspace root:
    1. cd edk2-platforms/edk2
    2. Execute edksetup.bat (Windows) or edksetup.sh (Linux).
    3. Verify the "WORKSPACE" environment variable is set to the edk2 directory in your workspace.
    4. Set the "PACKAGES_PATH" environment variable to include the edk2-platforms/Platform/Intel, edk2-platforms/Silicon/Intel,
       and edk2-platforms/Features/Intel directories.
       * Windows example:
         * set PACKAGES_PATH=c:\Edk2Workspace\edk2-platforms\Platform\Intel;
                             c:\Edk2Workspace\edk2-platforms\Silicon\Intel;
                             c:\Edk2Workspace\edk2-platforms\Features\Intel
       * Linux example:
         * export PACKAGES_PATH=~/Edk2Workspace/edk2-platforms/Platform/Intel:
                                ~/Edk2Workspace/edk2-platforms/Silicon/Intel:
                                ~/Edk2Workspace/edk2-platforms/Features/Intel
    5. cd edk2-platforms/Features/Intel
    6. <pre>build -p FeatureDomainDirectory/XxxFeaturePkg/XxxFeaturePkg.dsc -a IA32 -a X64</pre>

       *Note:* -a specifies the architecture. Typically IA32 and X64 modules are built for 32-bit PEI and 64-bit
       DXE though build for your specific requirements.

10. Add the advanced feature to `AdvancedFeaturePkg` so it is available to board packages.
    1. Add `XxxFeaturePkg/Include/XxxFeature.dsc` to `AdvancedFeatures.dsc`
    2. Add `XxxFeaturePkg/Include/PreMemory.fdf` to `AdvancedFeaturePkg/Include/PreMemory.fdf`
    3. Add `XxxFeaturePkg/Include/PostMemory.fdf` to `AdvancedFeaturePkg/Include/PostMemory.fdf`

11. Build `AdvancedFeaturePkg` to ensure the build is successful.
    1. Follow the steps in step #9 but change the build command to:

    <pre>build -p AdvancedFeaturePkg/AdvancedFeaturePkg.dsc -a IA32 -a X64</pre>

12. Before sending your patch series, ensure the `Readme.md` file in `XxxFeaturePkg` is completed so others can use it
    during the feature review.
