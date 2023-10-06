#!/bin/bash

function _help(){
	echo "Usage: build.sh --device DEV"
	echo
	echo "Build edk2 for Rockchip RK35xx platforms."
	echo
	echo "Options: "
	echo "	--device DEV, -d DEV:    build for DEV."
	echo "	--all, -a:               build all devices."
	echo "	--gui:                   Enable simple-init GUI."
	echo "	--release MODE, -r MODE: Release mode for building, default is 'DEBUG', 'RELEASE' alternatively."
	echo "	--toolchain TOOLCHAIN:   Set toolchain, default is 'GCC5'."
	echo " 	--skip-rootfs-gen:       skip generating SimpleInit rootfs to speed up building."
	echo "	--clean, -C:             clean workspace and output."
	echo "	--distclean, -D:         clean up all files that are not in repo."
	echo "	--outputdir, -O:         output folder."
	echo "	--help, -h:              show this help."
	echo
	exit "${1}"
}

function _error(){ echo "${@}" >&2;exit 1; }

MACHINE_TYPE=`uname -m`

# Fix-up possible differences in reported arch
if [ ${MACHINE_TYPE} == 'arm64' ]; then
	MACHINE_TYPE='aarch64'
elif [ ${MACHINE_TYPE} == 'amd64' ]; then
	MACHINE_TYPE='x86_64'
fi

function _build_idblock(){
	echo " => Building idblock.bin"
	pushd ${WORKSPACE}
	FLASHFILES="FlashHead.bin FlashData.bin FlashBoot.bin"
	rm -f rk35*_spl_loader_*.bin idblock.bin rk35*_ddr_*.bin rk35*_usbplug*.bin UsbHead.bin ${FLASHFILES}

	DDRBIN=$(grep '^FlashData' ${ROOTDIR}/misc/rkbin/RKBOOT/${MINIALL_INI} | cut -d = -f 2-)
	SPL=$(grep '^FlashBoot' ${ROOTDIR}/misc/rkbin/RKBOOT/${MINIALL_INI} | cut -d = -f 2-)

	# Create idblock.bin
	${ROOTDIR}/misc/tools/${MACHINE_TYPE}/mkimage -n rk3588 -T rksd -d ${ROOTDIR}/misc/rkbin/${DDRBIN}:${ROOTDIR}/misc/rkbin/${SPL} idblock.bin
	popd
	echo " => idblock.bin build done"
}

function _build_fit(){
	echo " => Building FIT"
	pushd ${WORKSPACE}
	BL31=$(grep '^PATH=.*_bl31_' ${ROOTDIR}/misc/rkbin/RKTRUST/${TRUST_INI} | cut -d = -f 2-)
	BL32=$(grep '^PATH=.*_bl32_' ${ROOTDIR}/misc/rkbin/RKTRUST/${TRUST_INI} | cut -d = -f 2-)
	rm -f bl31_0x*.bin ${WORKSPACE}/BL33_AP_UEFI.Fv ${SOC_L}_${DEVICE}_EFI.its

	${ROOTDIR}/misc/extractbl31.py ${ROOTDIR}/misc/rkbin/${BL31}
	cp ${ROOTDIR}/misc/rkbin/${BL32} ${WORKSPACE}/bl32.bin
	cp ${ROOTDIR}/misc/${SOC_L}_spl.dtb ${WORKSPACE}/${DEVICE}.dtb
	cp ${WORKSPACE}/Build/${PLATFORM_NAME}/${_MODE}_${TOOLCHAIN}/FV/BL33_AP_UEFI.Fv ${WORKSPACE}/
	cat ${ROOTDIR}/misc/uefi_${SOC_L}.its | sed "s,@DEVICE@,${DEVICE},g" > ${SOC_L}_${DEVICE}_EFI.its
	${ROOTDIR}/misc/tools/${MACHINE_TYPE}/mkimage -f ${SOC_L}_${DEVICE}_EFI.its -E ${DEVICE}_EFI.itb

	popd
	echo " => FIT build done"
}

function _pack(){
	_build_idblock
	_build_fit

	echo " => Building 8MB NOR FLASH IMAGE"
	cp ${WORKSPACE}/Build/${PLATFORM_NAME}/${_MODE}_${TOOLCHAIN}/FV/NOR_FLASH_IMAGE.fd ${WORKSPACE}/RK3588_NOR_FLASH.img

	# might be GPT table? size:0x4400
	dd if=${ROOTDIR}/misc/rk3588_spi_nor_gpt.img of=${WORKSPACE}/RK3588_NOR_FLASH.img
	# idblock at 0x8000
	dd if=${WORKSPACE}/idblock.bin of=${WORKSPACE}/RK3588_NOR_FLASH.img bs=1K seek=32
	# FIT Image at 0x100000
	dd if=${WORKSPACE}/${DEVICE}_EFI.itb of=${WORKSPACE}/RK3588_NOR_FLASH.img bs=1K seek=1024
	cp ${WORKSPACE}/RK3588_NOR_FLASH.img ${ROOTDIR}/
}

function _build(){
	local DEVICE="${1}"
	shift
	[ -d "${WORKSPACE}/Conf" ]||mkdir -p "${WORKSPACE}/Conf"
	source "${ROOTDIR}/edk2/edksetup.sh"
	[ -d "${WORKSPACE}" ]||mkdir "${WORKSPACE}"
	set -x
	make -C "${ROOTDIR}/edk2/BaseTools"||exit "$?"

	EXT=""

	if [ -f "configs/${DEVICE}.conf" ]
	then source "configs/${DEVICE}.conf"
	else _error "Device configuration not found"
	fi
	if [ -f "configs/${SOC}.conf" ]
	then source "configs/${SOC}.conf"
	else _error "SoC configuration not found"
	fi
	typeset -l SOC_L="$SOC"

	# based on the instructions from edk2-platform
	rm -f "${OUTDIR}/RK35*_NOR_FLASH.img"

	case "${MODE}" in
		RELEASE) _MODE=RELEASE;;
		*) _MODE=DEBUG;;
	esac

	build \
		-s \
		-n 0 \
		-a AARCH64 \
		-t "${TOOLCHAIN}" \
		-p "${ROOTDIR}/${DSC_FILE}" \
		-b "${_MODE}" \
		-D FIRMWARE_VER="${GITCOMMIT}" \
		-D ENABLE_SIMPLE_INIT="${BUILD_GUI}" \
		-D CONFIG_SOC="${SOC}" \
		||return "$?"

	_pack
	set +x

	echo "Build done: RK3588_NOR_FLASH.img"
}

function _clean(){ rm --one-file-system --recursive --force "${OUTDIR}"/workspace "${OUTDIR}"/RK3588_*.img "${OUTDIR}"/uefi-*.img*; }

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
export BUILD_GUI=false
OPTS="$(getopt -o t:d:haCDO:r -l toolchain:,device:,help,all,skip-rootfs-gen,gui,clean,distclean,outputdir:,release: -n 'build.sh' -- "$@")"||exit 1
eval set -- "${OPTS}"
while true
do	case "${1}" in
		-d|--device) DEVICE="${2}";shift 2;;
		-a|--all) DEVICE=all;shift;;
		-C|--clean) CLEAN=true;shift;;
		-D|--distclean) DISTCLEAN=true;shift;;
		-O|--outputdir) OUTDIR="${2}";shift 2;;
		--skip-rootfs-gen) GEN_ROOTFS=false;shift;;
		--gui) BUILD_GUI=true;shift;;
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

if ! [ -f edk2/edksetup.sh ] && ! [ -f ../edk2/edksetup.sh ]
then
	set -e
	echo "SKIP Updating submodules"
	set +e
fi

for i in "${SIMPLE_INIT}" ./simple-init ../simple-init
do
	if [ -n "${i}" ]&&[ -f "${i}/SimpleInit.inc" ]
	then
		_SIMPLE_INIT="$(realpath "${i}")"
		break
	fi
done

[ -n "${_SIMPLE_INIT}" ]||[ ${BUILD_GUI} == false ]||error "SimpleInit not found, please see README.md"
[ -f "configs/${DEVICE}.conf" ]||[ "${DEVICE}" == "all" ]||_error "Device configuration not found"

export CROSS_COMPILE="${CROSS_COMPILE:-aarch64-linux-gnu-}"
export GCC5_AARCH64_PREFIX="${CROSS_COMPILE}"
export CLANG38_AARCH64_PREFIX="${CROSS_COMPILE}"
# export PACKAGES_PATH="$_EDK2:$_EDK2_PLATFORMS:$_SIMPLE_INIT:$PWD"
export PACKAGES_PATH="${ROOTDIR}/edk2:${ROOTDIR}/edk2-platforms:${ROOTDIR}/edk2-rockchip:${ROOTDIR}/edk2-rockchip-non-osi:${ROOTDIR}/edk2-non-osi:${ROOTDIR}:${_SIMPLE_INIT}"
export WORKSPACE="${OUTDIR}/workspace"
GITCOMMIT="$(git describe --tags --always)"||GITCOMMIT="unknown"
export GITCOMMIT
set -e

if "${BUILD_GUI}"
then
	mkdir -p "${_SIMPLE_INIT}/build" "${_SIMPLE_INIT}/root/usr/share/locale"
	for i in "${_SIMPLE_INIT}/po/"*.po
	do
		[ -f "${i}" ]||continue
		_name="$(basename "$i" .po)"
		_path="${_SIMPLE_INIT}/root/usr/share/locale/${_name}/LC_MESSAGES"
		mkdir -p "${_path}"
		msgfmt -o "${_path}/simple-init.mo" "${i}"
	done

	if "${GEN_ROOTFS}"
	then
		bash "${_SIMPLE_INIT}/scripts/gen-rootfs-source.sh" \
			"${_SIMPLE_INIT}" \
			"${_SIMPLE_INIT}/build"
	fi
fi

if [ "${DEVICE}" == "all" ]
then
	E=0
	for i in configs/*.conf
	do
		DEV="$(basename "$i" .conf)"
		if [ "${DEV}" != "RK3588" ]&&[ "${DEV}" != "RK3568" ]
		then
			echo "Building ${DEV}"
			_build "${DEV}"||E="$?"
		fi
	done
	exit "${E}"
else
	_build "${DEVICE}"
fi
