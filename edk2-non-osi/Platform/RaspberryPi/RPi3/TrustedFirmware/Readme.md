ARM Trusted Firmware for Raspberry Pi 3
=======================================

The `bl1.bin` and `fip.bin` TF-A binaries found in this directory were built from the
[official TF-A 2.5 release](https://git.trustedfirmware.org/TF-A/trusted-firmware-a.git/tag/?h=v2.5)
through a [GitHub build script](https://github.com/pftf/pitf/blob/master/.github/workflows/build.yml)
that is designed to provide evidence that these binaries match the vanilla TF-A source.

Per the [GitHub Actions log](https://github.com/pftf/pitf/runs/2822874196),
the SHA-256 sums for the blobs can be validated to be as follows:
- `bl1.bin`: `5ba701a7e977d308a19928e19937107387677d52a1a4d628a5c2bb4e795aae8b`
- `fip.bin`: `0c3f8a3e8192e5dcb3bdc5867976e4277e9d948159a92ee71a54e92cb8dce9a3`

For Raspberry Pi 3 usage, TF-A was built using the command:
```
make PLAT=rpi3 RPI3_PRELOADED_DTB_BASE=0x10000 PRELOADED_BL33_BASE=0x30000 SUPPORT_VFP=1 RPI3_USE_UEFI_MAP=1 DEBUG=0 fip all
```
which results in the following memory mapping:
```
    0x00000000 +-----------------+
               |       ROM       | BL1
    0x00010000 +-----------------+
               |     Nothing     |
    0x00020000 +-----------------+
               |       FIP       |
    0x00030000 +-----------------+
               |                 |
               |  UEFI PAYLOAD   |
               |                 |
    0x001f0000 +-----------------+
               |       DTB       | (Loaded by the VideoCore)
    0x00200000 +-----------------+
               |   Secure SRAM   | BL2, BL31
    0x00300000 +-----------------+
               |   Secure DRAM   | BL32 (Secure payload)
    0x00400000 +-----------------+
               |                 |
               |                 |
               | Non-secure DRAM | BL33
               |                 |
               |                 |
    0x01000000 +-----------------+
               |                 |
               |       ...       |
               |                 |
    0x3F000000 +-----------------+
               |       I/O       |
```
