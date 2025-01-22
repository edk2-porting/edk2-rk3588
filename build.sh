#!/bin/bash

function _help(){
    echo
    echo "Build EDK2 for Rockchip RK3588 platforms."
    echo
    echo "Usage: build.sh [options]"
    echo
    echo "Options:"
    echo "  -d, --device DEV            Build for DEV, or 'all'."
    echo "  -r, --release MODE          Release mode for building, default is 'DEBUG', 'RELEASE' alternatively."
    echo "  -t, --toolchain TOOLCHAIN   Set toolchain, default is 'GCC'."
    echo "  --open-tfa ENABLE           Use open-source TF-A submodule. Default: ${OPEN_TFA}"
    echo "  -C, --clean                 Clean workspace and output."
    echo "  -D, --distclean             Clean up all files that are not in repo."
    echo "  --tfa-flags \"FLAGS\"         Flags appended to open TF-A build process."
    echo "  --edk2-flags \"FLAGS\"        Flags appended to the EDK2 build process."
    echo "  -h, --help                  Show this help."
    echo
    exit "${1}"
}

function _error() { echo "${@}" >&2; exit 1; }

function _build_idblock() {
    echo " => Building idblock.bin"
    pushd ${WORKSPACE}

    FLASHFILES="FlashHead.bin FlashData.bin FlashBoot.bin"
    rm -f rk35*_spl_loader_*.bin idblock.bin rk35*_ddr_*.bin rk35*_usbplug*.bin UsbHead.bin ${FLASHFILES}

    DDRBIN_RKBIN=$(grep '^FlashData' ${ROOTDIR}/misc/rkbin/RKBOOT/${MINIALL_INI} | cut -d = -f 2-)
    SPL_RKBIN=$(grep '^FlashBoot' ${ROOTDIR}/misc/rkbin/RKBOOT/${MINIALL_INI} | cut -d = -f 2-)

    DDRBIN="${ROOTDIR}/misc/rkbin/${DDRBIN_RKBIN}"

    #
    # SPL v1.13 has broken SD card support!
    # Use v1.12 instead.
    #
    # SPL="${ROOTDIR}/misc/rkbin/${SPL_RKBIN}"
    SPL="${ROOTDIR}/misc/rk3588_spl_v1.12.bin"

    # Create idblock.bin
    ${ROOTDIR}/misc/tools/${MACHINE_TYPE}/mkimage -n rk3588 -T rksd -d ${DDRBIN}:${SPL} idblock.bin

    popd
    echo " => idblock.bin build done"
}

function _build_fit() {
    echo " => Building FIT"
    pushd ${WORKSPACE}

    BL31_RKBIN=$(grep '^PATH=.*_bl31_' ${ROOTDIR}/misc/rkbin/RKTRUST/${TRUST_INI} | cut -d = -f 2-)
    BL32_RKBIN=$(grep '^PATH=.*_bl32_' ${ROOTDIR}/misc/rkbin/RKTRUST/${TRUST_INI} | cut -d = -f 2-)

    BL31="${ROOTDIR}/misc/rkbin/${BL31_RKBIN}"
    BL32="${ROOTDIR}/misc/rkbin/${BL32_RKBIN}"

    if [ ${OPEN_TFA} == 1 ]; then
        BL31="${ROOTDIR}/arm-trusted-firmware/build/${TFA_PLAT}/${RELEASE_TYPE,,}/bl31/bl31.elf"
    fi

    rm -f bl31_0x*.bin ${WORKSPACE}/BL33_AP_UEFI.Fv ${SOC_L}_${DEVICE}_EFI.its

    ${ROOTDIR}/misc/extractbl31.py ${BL31}
    if [ ! -f bl31_0x000f0000.bin ]; then
        # Not used but FIT expects it.
        touch bl31_0x000f0000.bin
    fi

    cp ${BL32} ${WORKSPACE}/bl32.bin
    cp ${ROOTDIR}/misc/${SOC_L}_spl.dtb ${WORKSPACE}/${DEVICE}.dtb
    cp ${WORKSPACE}/Build/${PLATFORM_NAME}/${RELEASE_TYPE}_${TOOLCHAIN}/FV/BL33_AP_UEFI.Fv ${WORKSPACE}/
    cat ${ROOTDIR}/misc/uefi_${SOC_L}.its | sed "s,@DEVICE@,${DEVICE},g" > ${SOC_L}_${DEVICE}_EFI.its
    ${ROOTDIR}/misc/tools/${MACHINE_TYPE}/mkimage -f ${SOC_L}_${DEVICE}_EFI.its -E ${DEVICE}_EFI.itb

    popd
    echo " => FIT build done"
}

function _pack_image() {
    _build_idblock
    _build_fit

    echo " => Building 8MB NOR FLASH IMAGE"
    cp ${WORKSPACE}/Build/${PLATFORM_NAME}/${RELEASE_TYPE}_${TOOLCHAIN}/FV/NOR_FLASH_IMAGE.fd ${WORKSPACE}/RK3588_NOR_FLASH.img

    # GPT at 0x0, size:0x4400
    dd if=${ROOTDIR}/misc/rk3588_spi_nor_gpt.img of=${WORKSPACE}/RK3588_NOR_FLASH.img
    # idblock at 0x8000
    dd if=${WORKSPACE}/idblock.bin of=${WORKSPACE}/RK3588_NOR_FLASH.img bs=1K seek=32
    # FIT Image at 0x100000
    dd if=${WORKSPACE}/${DEVICE}_EFI.itb of=${WORKSPACE}/RK3588_NOR_FLASH.img bs=1K seek=1024
    cp ${WORKSPACE}/RK3588_NOR_FLASH.img ${ROOTDIR}/
}

function _build(){
    local DEVICE="${1}"; shift

    #
    # Grab platform parameters
    #
    if [ -f "configs/${DEVICE}.conf" ]
    then source "configs/${DEVICE}.conf"
    else _error "Device configuration not found"
    fi
    if [ -f "configs/${SOC}.conf" ]
    then source "configs/${SOC}.conf"
    else _error "SoC configuration not found"
    fi
    typeset -l SOC_L="$SOC"

    rm -f "${OUTDIR}/RK35*_NOR_FLASH.img"

    #
    # Build TF-A
    #
    if [ ${OPEN_TFA} == 1 ]; then
        pushd arm-trusted-firmware

        if [ ${RELEASE_TYPE} == "DEBUG" ]; then
            DEBUG=1
        else
            DEBUG=0
        fi

        make PLAT=${TFA_PLAT} DEBUG=${DEBUG} all ${TFA_FLAGS}

        popd
    fi

    #
    # Build EDK2
    #
    [ -d "${WORKSPACE}/Conf" ] || mkdir -p "${WORKSPACE}/Conf"

    export GCC_AARCH64_PREFIX="${CROSS_COMPILE}"
    export CLANG38_AARCH64_PREFIX="${CROSS_COMPILE}"
    export PACKAGES_PATH="${ROOTDIR}/edk2:${ROOTDIR}/edk2-platforms:${ROOTDIR}/edk2-rockchip:${ROOTDIR}/devicetree:${ROOTDIR}/edk2-non-osi:${ROOTDIR}"

    make -C "${ROOTDIR}/edk2/BaseTools"
    source "${ROOTDIR}/edk2/edksetup.sh"

    build \
        -s \
        -n 0 \
        -a AARCH64 \
        -t "${TOOLCHAIN}" \
        -p "${ROOTDIR}/${DSC_FILE}" \
        -b "${RELEASE_TYPE}" \
        -D FIRMWARE_VER="${GIT_COMMIT}" \
        -D NETWORK_ALLOW_HTTP_CONNECTIONS=TRUE \
        -D NETWORK_ISCSI_ENABLE=TRUE \
        -D INCLUDE_TFTP_COMMAND=TRUE \
        --pcd gRockchipTokenSpaceGuid.PcdFitImageFlashAddress=0x100000 \
        ${EDK2_FLAGS}

    #
    # Compile final image
    #
    _pack_image

    echo "Build done: RK3588_NOR_FLASH.img"
}

function _clean() { rm --one-file-system --recursive --force "${OUTDIR}"/workspace "${OUTDIR}"/RK3588_*.img; }
function _distclean() { if [ -d .git ]; then git clean -xdf; else _clean; fi; }

#
# Default variables
#
typeset -l DEVICE
typeset -u RELEASE_TYPE
DEVICE=""
RELEASE_TYPE=DEBUG
TOOLCHAIN=GCC
OPEN_TFA=1
TFA_FLAGS=""
EDK2_FLAGS=""
CLEAN=false
DISTCLEAN=false
OUTDIR="${PWD}"

#
# Get options
#
OPTS=$(getopt -o "d:r:t:CDh" -l "device:,release:,toolchain:,open-tfa:,tfa-flags:,edk2-flags:,clean,distclean,help" -n build.sh -- "${@}") || _help $?
eval set -- "${OPTS}"
while true; do
    case "${1}" in
        -d|--device) DEVICE="${2}"; shift 2 ;;
        -r|--release) RELEASE_TYPE="${2}"; shift 2 ;;
        -t|--toolchain) TOOLCHAIN="${2}"; shift 2 ;;
        --open-tfa) OPEN_TFA="${2}"; shift 2 ;;
        --tfa-flags) TFA_FLAGS="${2}"; shift 2 ;;
        --edk2-flags) EDK2_FLAGS="${2}"; shift 2 ;;
        -C|--clean) CLEAN=true; shift ;;
        -D|--distclean) DISTCLEAN=true; shift ;;
        -h|--help) _help 0; shift ;;
        --) shift; break ;;
        *) break ;;
    esac
done
if [[ -n "${@}" ]]; then
    echo "Invalid additional arguments '${@}'"
    _help 1
fi

if "${DISTCLEAN}"; then _distclean; exit "$?"; fi
if "${CLEAN}"; then _clean; exit "$?"; fi

[ -z "${DEVICE}" ] && _help 1
[ -f "configs/${DEVICE}.conf" ] || [ "${DEVICE}" == "all" ] || _error "Device configuration not found"

#
# Get machine architecture
#
MACHINE_TYPE=$(uname -m)

# Fix-up possible differences in reported arch
if [ ${MACHINE_TYPE} == 'arm64' ]; then
    MACHINE_TYPE='aarch64'
elif [ ${MACHINE_TYPE} == 'amd64' ]; then
    MACHINE_TYPE='x86_64'
fi

if [ ${MACHINE_TYPE} != 'aarch64' ]; then
    export CROSS_COMPILE="${CROSS_COMPILE:-aarch64-linux-gnu-}"
fi

GIT_COMMIT="$(git describe --tags --always)" || GIT_COMMIT="unknown"

export WORKSPACE="${OUTDIR}/workspace"
[ -d "${WORKSPACE}" ] || mkdir "${WORKSPACE}"

ROOTDIR="$(realpath "$(dirname "$0")")"
cd "${ROOTDIR}" || exit 1

# Exit on first error
set -e

if [ "${DEVICE}" == "all" ]
then
    for i in configs/*.conf; do
        DEV="$(basename "$i" .conf)"
        if [ "${DEV}" != "RK3588" ]
        then
            echo "Building ${DEV}"
            _build "${DEV}"
        fi
    done
else
    _build "${DEVICE}"
fi
