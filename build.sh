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
    echo "  --tfa-flags \"FLAGS\"         Flags appended to open TF-A build process."
    echo "  --edk2-flags \"FLAGS\"        Flags appended to the EDK2 build process."
    echo "  --skip-patchsets            Skip applying upstream submodule patchsets during development."
    echo "  -C, --clean                 Clean workspace and output."
    echo "  -D, --distclean             Clean up all files that are not in repo."
    echo "  -h, --help                  Show this help."
    echo
    exit "${1}"
}

function _error() { echo "${@}" >&2; exit 1; }

function apply_patchset() {
    ${SKIP_PATCHSETS} && return 0

    local patches_dir="$1"
    local target_dir="$2"

    [ ! -d "${patches_dir}" ] && return 0

    if [ ! -d "${target_dir}" ]; then
        echo "Patchset target directory does not exist: ${target_dir}"
        return 1
    fi

    echo "Checking patchset ${patches_dir} for ${target_dir}"

    local patchset_name=$(basename "${patches_dir}")
    local patchset_marker="${target_dir}/.patchset_${patchset_name}"

    if [ ! -f "${patchset_marker}" ] || [ "${patches_dir}" -nt "${patchset_marker}" ]; then
        echo "Patchset needs to be (re)applied"
        if ! git -C "${target_dir}" reset --hard || ! git -C "${target_dir}" clean -xfd; then
            echo "Failed to reset git repository - aborting"
            return 1
        fi
    else
        echo "Patchset already applied - skipping"
        return 0
    fi

    local patch_file
    local patch_count=0

    for patch_file in "${patches_dir}"/*.patch; do
        [ -f "${patch_file}" ] || continue

        local patch_name=$(basename "${patch_file}")
        echo "Patch ${patch_count}: ${patch_name}"

        if patch -p1 -d "${target_dir}" < "${patch_file}"; then
            echo "  Successfully applied"
            ((patch_count++))
        else
            echo "  Failed to apply - aborting"
            return 1
        fi
    done

    touch "${patchset_marker}"

    echo "Patchset summary: ${patch_count} applied"
    return 0
}

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

    if ${OPEN_TFA}; then
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
    if ${OPEN_TFA}; then
        apply_patchset "${ROOTDIR}/arm-trusted-firmware-patches" "${ROOTDIR}/arm-trusted-firmware" || exit 1

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
    apply_patchset "${ROOTDIR}/edk2-patches" "${ROOTDIR}/edk2" || exit 1
    apply_patchset "${ROOTDIR}/devicetree/mainline/patches" "${ROOTDIR}/devicetree/mainline/upstream" || exit 1

    [ -d "${WORKSPACE}/Conf" ] || mkdir -p "${WORKSPACE}/Conf"

    export GCC_AARCH64_PREFIX="${CROSS_COMPILE}"
    export CLANG38_AARCH64_PREFIX="${CROSS_COMPILE}"
    PACKAGES_PATH="${ROOTDIR}"
    PACKAGES_PATH+=":${ROOTDIR}/devicetree"
    PACKAGES_PATH+=":${ROOTDIR}/edk2"
    PACKAGES_PATH+=":${ROOTDIR}/edk2-non-osi"
    PACKAGES_PATH+=":${ROOTDIR}/edk2-platforms"
    PACKAGES_PATH+=":${ROOTDIR}/edk2-rockchip"
    PACKAGES_PATH+=":${ROOTDIR}/edk2-rockchip-non-osi"
    export PACKAGES_PATH

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
OPEN_TFA=true
TFA_FLAGS=""
EDK2_FLAGS=""
SKIP_PATCHSETS=false
CLEAN=false
DISTCLEAN=false
OUTDIR="${PWD}"

#
# Get options
#
OPTS=$(getopt -o "d:r:t:CDh" -l "device:,release:,toolchain:,open-tfa:,tfa-flags:,edk2-flags:,skip-patchsets,clean,distclean,help" -n build.sh -- "${@}") || _help $?
eval set -- "${OPTS}"
while true; do
    case "${1}" in
        -d|--device) DEVICE="${2}"; shift 2 ;;
        -r|--release) RELEASE_TYPE="${2}"; shift 2 ;;
        -t|--toolchain) TOOLCHAIN="${2}"; shift 2 ;;
        --open-tfa) OPEN_TFA="${2}"; shift 2 ;;
        --tfa-flags) TFA_FLAGS="${2}"; shift 2 ;;
        --edk2-flags) EDK2_FLAGS="${2}"; shift 2 ;;
        --skip-patchsets) SKIP_PATCHSETS=true; shift ;;
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
