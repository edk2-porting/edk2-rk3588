ARM Trusted Firmware for Raspberry Pi 4
=======================================

The `bl31.bin` TF-A binary found in this directory was built from the
[official TF-A 2.5 release](https://git.trustedfirmware.org/TF-A/trusted-firmware-a.git/tag/?h=v2.5)
through a [GitHub build script](https://github.com/pftf/pitf/blob/master/.github/workflows/build.yml)
that is designed to provide evidence that these binaries match the vanilla TF-A source.

Per the [GitHub Actions log](https://github.com/pftf/pitf/runs/2822874196),
the SHA-256 sum for the bin can be validated to be as follows:
- `bl31.bin`: `59c4486a0a257c8d630d8ea39d6a13d038cd50be4fc9a81985bf2c32a7c3fca6`

For Raspberry Pi 4 usage, TF-A was built using the command:
```
make PLAT=rpi4 RPI3_PRELOADED_DTB_BASE=0x1F0000 PRELOADED_BL33_BASE=0x20000 SUPPORT_VFP=1 DEBUG=0 all
```
