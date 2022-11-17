# Introduction

These instructions explain how to get an edk2/edk2-platforms build running
on the ARM Base FVP, which is a software model provided by ARM (for free)
, which models a Cortex A core with various peripherals. More information
can be found here:
[https://developer.arm.com/products/system-design/fixed-virtual-platforms]

##Requirements
- A 32-bit or 64-bit Linux host machine.

# Build EDK2 TianoCore

`build -a AARCH64 -p Platform/ARM/VExpressPkg/ArmVExpress-FVP-AArch64.dsc -t GCC5`

Once built, the edk2 image is the following file Build/ArmVExpress-FVP-AArch64/DEBUG_GCC5/FV/FVP_AARCH64_EFI.fd

# Run edk2/edk2-platforms on the ARM Base Platform FVP

In order to run the binary we have just built there are a few steps we need to
go through, we need to get a model, a set of prebuilts (where we will swap out
the edk2 image with our own) and the tool with which we will swap out the
prebuilt edk2 image.

We will also rely on the "run_model" script that comes with the prebuilts, it
is entirely possible to run the model without this but would require quite a bit
of knowledge regarding the arguments of the ARM fastmodel (documentation can be found here:
[https://developer.arm.com/docs/100966/1101/programming-reference-for-base-fvps/base-platform-revc-features])
however the manual set of the FVP is outside the scope of this document. If you are interested
please consult the documentation.

It's recommended you create a folder where you download the prebuilts and
required tool and copy your edk2 image in to it, as the run script expects
the binaries in the same directory.

1. Download the Base FVP from here https://developer.arm.com/products/system-design/fixed-virtual-platforms

	- Select Armv8-A Base Platform FVP based on Fast Models 11.4
	- It has a click through license but is free.

2. Download the 20.01 Linaro ARM Landing Team release for FVP booting UEFI
https://releases.linaro.org/members/arm/platforms/20.01/fvp-uefi.zip

3. Clone the trusted firmware repo from https://git.trustedfirmware.org/TF-A/trusted-firmware-a.git

4. Build fiptool: `make -C trusted-firmware-a/tools/fiptool`

5. Update the fip.bin image from fvp-uefi.zip by running the following command:

    `./trusted-firmware-a/tools/fiptool/fiptool update --nt-fw=[path to binary built above] fip.bin`

6. Execute the FVP run_model.sh script from fvp-uefi.zip and provide a path to the FVP binaries
downloaded in step 1):

	`MODEL=[path to FVP binary] ./run_model.sh`

This expects the contents of fvp-uefi.zip, the bl1.bin and fip.bin (which is
the file we modify), to be in the same directory as the run_model.sh script.

This should be sufficient to provide a build/run/debug environment for aarch64.
