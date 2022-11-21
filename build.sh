#!/bin/bash

function _help(){
	echo "Usage: build.sh --device DEV"
	echo
	echo "Build edk2 for Rockchip RK35xx platforms."
	echo
	echo "Options: "
	echo "	--device DEV, -d DEV:    build for DEV."
	echo "	--all, -a:               build all devices."
	echo "	--release MODE, -r MODE: Release mode for building, default is 'DEBUG', 'RELEASE' alternatively."
	echo "	--toolchain TOOLCHAIN:   Set toolchain, default is 'GCC5'."
	# echo " 	--skip-rootfs-gen:       skip generating SimpleInit rootfs to speed up building."
	echo "	--clean, -C:             clean workspace and output."
	echo "	--distclean, -D:         clean up all files that are not in repo."
	echo "	--outputdir, -O:         output folder."
	echo "	--help, -h:              show this help."
	echo
	exit "${1}"
}

function _error(){ echo "${@}" >&2;exit 1; }

function _call_hook(){
	local NAME="${1}"
	shift
	if declare -F "${NAME}" &>/dev/null
	then eval "${NAME}" "${@}"||return 1
	fi
	return 0
}

function _load_platform_hooks(){
	if [ -f "${ROOTDIR}/${1}" ]
	then source "${ROOTDIR}/${1}"
	fi
}

function _fit_repack(){
	mkdir -p ${WORKSPACE}/unpack
	# BL33_AP_UEFI is PrePi executable
	cp ${WORKSPACE}/Build/${PLATFORM_NAME}/${_MODE}_${TOOLCHAIN}/FV/BL33_AP_UEFI.Fv ${WORKSPACE}/unpack/uboot
	cp ${ROOTDIR}/misc/prebuilts/uboot_uefi.img ${WORKSPACE}/

	pushd ${ROOTDIR}/../git/u-boot
	./scripts/fit-repack.sh -f ${WORKSPACE}/uboot_uefi.img -d ${WORKSPACE}/unpack/
	popd
}

function _pack(){
	_fit_repack

	echo "****Build 8MB NOR FLASH IMAGE****"
	cp ${WORKSPACE}/Build/${PLATFORM_NAME}/${_MODE}_${TOOLCHAIN}/FV/NOR_FLASH_IMAGE.fd ${WORKSPACE}/RK3588_NOR_FLASH.img

	# backup NV_DATA at 0x007C0000
	dd if=${WORKSPACE}/RK3588_NOR_FLASH.img of=${WORKSPACE}/NV_DATA.img bs=1K skip=7936
	# might be GPT table? size:0x4400
	dd if=${ROOTDIR}/misc/rk3588_spi_nor_gpt.img of=${WORKSPACE}/RK3588_NOR_FLASH.img
	# idblock at 0x8000 and 0x88000
	dd if=${ROOTDIR}/misc/prebuilts/idblock.bin of=${WORKSPACE}/RK3588_NOR_FLASH.img bs=1K seek=32
	dd if=${ROOTDIR}/misc/prebuilts/idblock.bin of=${WORKSPACE}/RK3588_NOR_FLASH.img bs=1K seek=544
	# FIT Image at 0x100000
	dd if=${WORKSPACE}/uboot_uefi.img of=${WORKSPACE}/RK3588_NOR_FLASH.img bs=1K seek=1024
	# restore NV_DATA at 0x007C0000
	dd if=${WORKSPACE}/NV_DATA.img of=${WORKSPACE}/RK3588_NOR_FLASH.img bs=1K seek=7936
	cp ${WORKSPACE}/RK3588_NOR_FLASH.img ${ROOTDIR}/
	echo "Build done: RK3588_NOR_FLASH.img"
}

function _build(){
	local DEVICE="${1}"
	shift
	source "${ROOTDIR}/edk2/edksetup.sh"
	[ -d "${WORKSPACE}" ]||mkdir "${WORKSPACE}"
	set -x
	make -C "${ROOTDIR}/edk2/BaseTools"||exit "$?"

	EXT=""

	if [ -f "configs/${DEVICE}.conf" ]
	then source "configs/${DEVICE}.conf"
	else _error "Device configuration not found"
	fi

	# _load_platform_hooks Platform/platform.sh.inc
	# _load_platform_hooks Silicon/platform.sh.inc

	# based on the instructions from edk2-platform
	rm -f "${OUTDIR}/boot-${DEVICE}${EXT}.img" "${OUTDIR}/uefi-${DEVICE}"*

	case "${MODE}" in
		RELEASE) _MODE=RELEASE;;
		*) _MODE=DEBUG;;
	esac

	# _call_hook platform_pre_build||return "$?"

	build \
		-s \
		-n 0 \
		-a AARCH64 \
		-t "${TOOLCHAIN}" \
		-p "${ROOTDIR}/${DSC_FILE}" \
		-b "${_MODE}" \
		-D FIRMWARE_VER="${GITCOMMIT}" \
		||return "$?"

	_pack

	# _call_hook platform_build_kernel||return "$?"
	# echo "Build done: ${OUTDIR}/boot-${DEVICE}${EXT}.img"
	set +x
}

function _clean(){ rm --one-file-system --recursive --force "${WORKSPACE}" "${OUTDIR}"/boot-*.img "${OUTDIR}"/uefi-*.img*; }

function _distclean(){ if [ -d .git ];then git clean -xdf;else _clean;fi; }

OUTDIR="${PWD}"
ROOTDIR="$(realpath "$(dirname "$0")")"
cd "${ROOTDIR}"||exit 1
typeset -l DEVICE
typeset -u MODE
DEVICE=""
MODE=DEBUG
CLEAN=false
DISTCLEAN=false
TOOLCHAIN=GCC5
export ROOTDIR OUTDIR
export GEN_ROOTFS=true
OPTS="$(getopt -o t:d:haCDO:r -l toolchain:,device:,help,all,skip-rootfs-gen,clean,distclean,outputdir:,release: -n 'build.sh' -- "$@")"||exit 1
eval set -- "${OPTS}"
while true
do	case "${1}" in
		-d|--device) DEVICE="${2}";shift 2;;
		-a|--all) DEVICE=all;shift;;
		-C|--clean) CLEAN=true;shift;;
		-D|--distclean) DISTCLEAN=true;shift;;
		-O|--outputdir) OUTDIR="${2}";shift 2;;
		--skip-rootfs-gen) GEN_ROOTFS=false;shift;;
		-r|--release) MODE="${2}";shift 2;;
		-t|--toolchain) TOOLCHAIN="${2}";shift 2;;
		-h|--help) _help 0;shift;;
		--) shift;break;;
		*) _help 1;;
	esac
done
if "${DISTCLEAN}";then _distclean;exit "$?";fi
if "${CLEAN}";then _clean;exit "$?";fi
[ -z "${DEVICE}" ]&&_help 1

if ! [ -f Common/edk2/edksetup.sh ] && ! [ -f ../edk2/edksetup.sh ]
then
	set -e
	echo "SKIP Updating submodules"
	set +e
fi

# for i in "${SIMPLE_INIT}" Platform/RenegadePkg/Library/SimpleInit ./simple-init ../simple-init
# do
# 	if [ -n "${i}" ]&&[ -f "${i}/SimpleInit.inc" ]
# 	then
# 		_SIMPLE_INIT="$(realpath "${i}")"
# 		break
# 	fi
# done

# [ -n "${_SIMPLE_INIT}" ]||_error "SimpleInit not found, please see README.md"
[ -f "configs/${DEVICE}.conf" ]||_error "Device configuration not found"

export CROSS_COMPILE="${CROSS_COMPILE:-aarch64-linux-gnu-}"
export GCC5_AARCH64_PREFIX="${CROSS_COMPILE}"
export CLANG38_AARCH64_PREFIX="${CROSS_COMPILE}"
# export PACKAGES_PATH="$_EDK2:$_EDK2_PLATFORMS:$_SIMPLE_INIT:$PWD"
export PACKAGES_PATH="${ROOTDIR}/edk2:${ROOTDIR}/edk2-platforms:${ROOTDIR}/edk2-non-osi:${ROOTDIR}"
export WORKSPACE="${OUTDIR}/workspace"
GITCOMMIT="$(git describe --tags --always)"||GITCOMMIT="unknown"
export GITCOMMIT
set -e

# mkdir -p "${_SIMPLE_INIT}/build" "${_SIMPLE_INIT}/root/usr/share/locale"
# for i in "${_SIMPLE_INIT}/po/"*.po
# do
# 	[ -f "${i}" ]||continue
# 	_name="$(basename "$i" .po)"
# 	_path="${_SIMPLE_INIT}/root/usr/share/locale/${_name}/LC_MESSAGES"
# 	mkdir -p "${_path}"
# 	msgfmt -o "${_path}/simple-init.mo" "${i}"
# done

# if "${GEN_ROOTFS}"
# then
# 	 bash "${_SIMPLE_INIT}/scripts/gen-rootfs-source.sh" \
# 		"${_SIMPLE_INIT}" \
# 		"${_SIMPLE_INIT}/build"
# fi

if [ "${DEVICE}" == "all" ]
then
	E=0
	for i in configs/*.conf
	do
		DEV="$(basename "$i" .conf)"
		echo "Building ${DEV}"
		_build "${DEV}"||E="$?"
	done
	exit "${E}"
else
	_build "${DEVICE}"
fi
