# Tested Operating Systems

## Debian

[Debian 10](https://cdimage.debian.org/debian-cd/current/arm64/iso-cd/) netinst has been
tested and confirmed to work, both on the Model B and Model B+, including installation in
either wired or wireless mode.

Below are steps you can follow to install Debian 10 onto an SD card:
* Partition the media as MBR and create a ~300 MB partition on it with MBR type `0x0e`.
  __Note:__ Make sure that the partition scheme is MBR (not GPT) and the type `0x0e` (not
  `0xef` for instance), as the on-CPU Broadcom bootloader supports neither the GPT scheme
  nor the ESP MBR type.
* Set the partition as active/bootable. This is needed as the Debian partition manager can
  not detect it as ESP otherwise, which we need for GRUB installation. If using `fdisk` on
  Linux, you can use the `a` command to set a partition as active. On Windows, you can use
  `diskpart` and then type `active` after selecting the relevant disk and partition.
* Format the partition as FAT. Here you should make sure that you use FAT16 over FAT32 else
  the Debian partition manager may not automatically detect the partition as ESP. If you
  are using Windows `diskpart` then `format fs=fat quick` will format a drive to FAT16. On
  Linux, the equivalent command would be `mkfs.vfat -F 16 /dev/<your_device>`. As long as
  the partition is smaller than 2 GB, the use of FAT16 over FAT32 should not be a problem.
  Note that it is also possible to use FAT32, but you will probably have to invoke `fdisk`
  before rebooting, as shown in the _Additional Notes_ below, to reset the partition type.
* Copy the UEFI firmware files (`RPI_EFI.fd`, `bootcode.bin`, `fixup.dat` and `start.elf`)
  as well as an appropriate `config.txt` onto the FAT partition. If needed you can download
  the non UEFI binary files from https://github.com/raspberrypi/firmware/tree/master/boot.
* (Optional) If you plan to install through WiFi, you will need to download relevant
  non-free  WLAN firmware binaries from your WLAN interface (`brcmfmac43430-sdio.txt` and
  `brcmfmac43430-sdio.bin` for a Raspberry Pi 3 Model B, `brcmfmac43455-sdio.txt` and
  `brcmfmac43455-sdio.bin` for a Raspberry Pi 3 Model B+). You may also want to obtain the
  relevant `.clm_blob` (`brcmfmac43430-sdio.clm_blob` or `brcmfmac43455-sdio.clm_blob`),
  though wireless networking should work even if you do not provide these files. Copy these
  files either at the root of your FAT partition or into a `firmware/` directory there.
* Download the latest `debian-##.#.#-arm64-netinst.iso` from
  https://cdimage.debian.org/debian-cd/current/arm64/iso-cd/
* Extract the full content of the ISO onto the FAT partition you created.
* Insert the media and power up the Raspberry Pi device.
* On the GRUB menu select `Install` and let the Debian Installer process start.
  __Note:__ In case anything goes wrong during the install process, you can use
  <kbd>Alt</kbd>-<kbd>F4</kbd> to check the installation log.
* Select your Language, Country and Keyboard and let the installer proceed until it reports
  that `No Common CD-ROM drive was detected.`
* On `Load CD-ROM drivers from removable media` select `No`.
* On `Manually select a CD-ROM module and device` select `Yes`.
* On `Module needed for accessing the CD-ROM` select `none`.
* On `Device file for accessing the CD-ROM` type the following exactly:
  ```
  -t vfat -o rw /dev/mmcblk0p1
  ```
* (Optional) If you have copied the non-free WLAN firmware binaries, and plan to install
  through wireless, you can let the installer select the firmware files. Please be mindful
  that you may be asked multiple times as there are multiple files to provide.
* If requested by the installer, set up your network by choosing the network interface you
  want to use for installation and (optionally) your access point and credentials.
* Go through the hostname, user/password set up and customize those as you see fit.
* Let the installer continue until you get to the `Partition disks` screen. There, for
  `Partitioning method` select `Manual`. You __should__ see something like this:
  ```
  MMC/SD card #1 (mmcblk0) - 16.0 GB SD 2WCGO
       #1  primary  314.6 MB  B  K  ESP
           pri/log                  FREE SPACE
  ```
  In other words, the partition manager should already detect your existing partition as
  `ESP`, with the `B` (bootable) and `K` (keep data) flags. If that is not the case, (e.g.
  if it says `fat16` or `fat32` instead of `ESP`) then it probably means you either didn't
  format the partition to FAT16 or you forgot to set the bootable flag. In that case,
  please refer to the _Additional Notes_ below.
* Select `FREE SPACE` &rarr; `Create a new partition` and create a `1 GB` primary `swap`
  partition.
* Select `FREE SPACE` &rarr; `Create a new partition` and allocate the rest to a primary
 `ext4` root partition (mountpoint = `/`)
* After doing the above, your partition report should look like this:
  ```
  MMC/SD card #1 (mmcblk0) - 16.0 GB SD 2WCGO
       #1  primary  314.6 MB  B  K  ESP
       #2  primary    1.0 GB     f  swap    swap
       #3  primary   14.7 GB     f  ext4    /
  ```
* Select `Finish partitioning and write changes to disk` and then `Yes` and let the
  installer continue with the base system installation.
* After a while, the installer should produce a message that states:
  ```
  [!!] Configure the package manager

  apt-configuration problem
  An attempt to configure apt to install additional packages from the CD failed.
  ```
  This is a __benign__ message that you can safely ignore by selecting `Continue` (The
  reason it is benign is we are running a net install and won't need to access the "CD-ROM"
  files post install).
* Once you have dimissed the message above, pick the mirror closest to your geographical
  location and let the installer proceed with some more software installation.
* Finally, at the `Software selection` screen, choose any additional software package you
  wish to install. `Debian desktop environment` should work out of the box if you choose to
  install it.
* Let the process finalize the software and GRUB bootloader installation and, provided you
  didn't run into the partition manager issue described above (installation partition not
  seen as `ESP`) you can reboot your machine when prompted, which, once completed, should
  bring you to your newly installed Debian environment.

### Additional Notes for Debian

The reason we use `-t vfat -o rw /dev/mmcblk0p1` for the source media (i.e. "CD-ROM" device)
is because, whereas the first partition on the SD card is indeed `/dev/mmcblk0p1`, we also
need to provide additional parameters for the `mount` command that the installer invokes
behind the scenes. For instance, if we don't use `-t vfat`, then ISO-9660 is forced as the
file system, and if we don't use `-o rw` then the partition will be mounted as read-only
which then prevents the same partition from being remounted when locating the non-free
firmware files or when setting up `/efi/boot`.

With regards to fixing the partitioning if you don't see `B  K  ESP` when entering the
partition manager, what you need to do is:
* Before you create the additional partitions, select the first partition and change its
  type to `ESP`. Note however that doing this changes the type of the partition to `0xef`
  which is precisely what we're trying to avoid by having the partition manager already
  detect it as ESP, as type `0xef` is __unbootable__ by the Broadcom CPU.
* To fix this then, before you choose `Continue` on the `Installation complete` prompt you
  should open a new console with <kbd>Alt</kbd>-<kbd>F2</kbd> and type:
  ```
  chroot /target fdisk /dev/mmcblk0
  ```
  Then press <kbd>t</kbd>, <kbd>1</kbd>, <kbd>e</kbd> <kbd>w</kbd>, to reset the partition
  to type `0x0e` (FAT16 LBA).

## Ubuntu

[Ubuntu 18.04 LTS](http://releases.ubuntu.com/18.04/) has been tested and confirmed to work,
on a Raspberry 3 Model B, including the installation process. Note however that network
installation and networking may not work on the Model B+, due to the `lan78xx` Linux driver
still requiring some support.

Below are the steps you can follow to install Ubuntu LTS onto SD/USB:
* Download the latest Ubuntu LTS ARM64 [`mini.iso`](http://ports.ubuntu.com/ubuntu-ports/dists/bionic/main/installer-arm64/current/images/netboot/mini.iso).
* Partition the media as MBR and create a ~200 MB FAT32 partition on it with MBR type `0x0c`.
  Note: Do not be tempted to use GPT partition scheme or `0xef` (EFI System Partition) for the
  type, as none of these are supported by the Raspberry Pi's internal boot rom.
* Extract the full content of the ISO onto the partition you created.
* Also extract the GRUB EFI bootloader `bootaa64.efi` from `/boot/grub/efi.img` to `/boot/grub/`.
  Note: Do not be tempted to copy this file to another directory (such as `/efi/boot/`) as GRUB looks for its
  modules and configuration data in the same directory as the EFI loader and also, the installation
  process will create a `bootaa64.efi` into `/efi/boot/`.
* If needed, copy the UEFI firmware files (`RPI_EFI.fd`, `bootcode.bin`, `fixup.dat` and `start.elf`)
  onto the FAT partition.
* Boot the pi and let it go into the UEFI shell.
* Navigate to `fs0:` then `/boot/grub/` and launch the GRUB efi loader.
* Follow the Ubuntu installation process.

Note: Because Ubuntu operates in quiet mode by default (no boot messages), you may think the system is frozen
on first reboot after installation. However, if you wait long enough you **will** get to a login prompt.

Once Linux is running, if desired, you can disable quiet boot, as well as force the display
of the GRUB selector, by editing `/etc/default/grub` and changing:
* `GRUB_TIMEOUT_STYLE=hidden` &rarr; `GRUB_TIMEOUT_STYLE=menu`
* `GRUB_CMDLINE_LINUX_DEFAULT="splash quiet"` &rarr; `GRUB_CMDLINE_LINUX_DEFAULT=""`

Then, to have your changes applied run `update-grub` and reboot.

## Other Linux distributions

* OpenSUSE Leap 42.3 has been reported to work on Raspberry 3 Model B.

* Other ARM64 Linux releases, that support UEFI boot and have the required hardware support
  for Pi hardware are expected to run, though their installation process might require some
  cajoling.

## Windows

Windows 10 1809 for ARM64 (build 17763) has been tested and confirmed to work (after replacing
`C:\Windows\System32\Drivers\WppRecorder.sys` with an older version, since the one from 1809
appears to be buggy across all archs, and results in a similar BSOD when trying to run Windows
To Go on x64 with native drivers for instance).

Windows 10 1803 for ARM64 and earlier do not work due to the presence of a hardware ASSERT check
in the Windows kernel, that was removed in later versions.

You probably want to look at https://www.worproject.ml/ as well as the
[Windows thread in the original RaspberryPiPkg](https://github.com/andreiw/RaspberryPiPkg/issues/12)
for installation details.

## Other platforms

Details you may need to run other platforms, including FreeBSD, is provided in the
[Readme from the original RaspberryPiPkg](https://github.com/andreiw/RaspberryPiPkg).
