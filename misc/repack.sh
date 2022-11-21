#!/bin/bash

# Used to pack any executable into a complete flash image

function _fit_repack(){
	mkdir -p ${WORKSPACE}/unpack

	cp ${REPACK_FILE} ${WORKSPACE}/unpack/uboot
	cp ${ROOTDIR}/misc/prebuilts/uboot_uefi.img ${WORKSPACE}/

	pushd ${ROOTDIR}/../git/u-boot
	./scripts/fit-repack.sh -f ${WORKSPACE}/uboot_uefi.img -d ${WORKSPACE}/unpack/
	popd
}

function _pack(){
	_fit_repack

	echo "****Build 8MB NOR FLASH IMAGE****"

	# FIT Image at 0x100000
	dd if=${WORKSPACE}/uboot_uefi.img of=${WORKSPACE}/RK3588_NOR_FLASH.img bs=1K seek=1024

	cp ${WORKSPACE}/RK3588_NOR_FLASH.img ${ROOTDIR}/
	echo "Build done: RK3588_NOR_FLASH.img"
}

ROOTDIR="$(realpath "$(dirname "$0")")/.."
cd "${ROOTDIR}"||exit 1
REPACK_FILE=${1}
export ROOTDIR REPACK_FILE
WORKSPACE="${ROOTDIR}/workspace"

_pack
