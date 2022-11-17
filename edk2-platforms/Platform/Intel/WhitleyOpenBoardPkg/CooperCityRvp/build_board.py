# @ build_board.py
# Extensions for building CooperCityRvp using build_bios.py
#
# Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

"""
This module serves as a sample implementation of the build extension
scripts
"""

import os
import sys

def pre_build_ex(config, functions):
    """Additional Pre BIOS build function

    :param config: The environment variables to be used in the build process
    :type config: Dictionary
    :param functions: A dictionary of function pointers
    :type functions: Dictionary
    :returns: nothing
    """
    print("pre_build_ex")
    config["BUILD_DIR_PATH"] = os.path.join(config["WORKSPACE"],
                                            'Build',
                                            config["PLATFORM_BOARD_PACKAGE"],
                                            "{}_{}".format(
                                                config["TARGET"],
                                                config["TOOL_CHAIN_TAG"]))
    # set BUILD_DIR path
    config["BUILD_DIR"] = os.path.join('Build',
                                       config["PLATFORM_BOARD_PACKAGE"],
                                       "{}_{}".format(
                                           config["TARGET"],
                                           config["TOOL_CHAIN_TAG"]))
    config["BUILD_X64"] = os.path.join(config["BUILD_DIR_PATH"], 'X64')
    config["BUILD_IA32"] = os.path.join(config["BUILD_DIR_PATH"], 'IA32')

    if not os.path.isdir(config["BUILD_DIR_PATH"]):
        try:
            os.makedirs(config["BUILD_DIR_PATH"])
        except OSError:
            print("Error while creating Build folder")
            sys.exit(1)

    #@todo: Replace this with PcdFspModeSelection
    if config.get("API_MODE_FSP_WRAPPER_BUILD", "FALSE") == "TRUE":
        config["EXT_BUILD_FLAGS"] += " -D FSP_MODE=0"
    else:
        config["EXT_BUILD_FLAGS"] += " -D FSP_MODE=1"
    return None

def _merge_files(files, ofile):
    with open(ofile, 'wb') as of:
        for x in files:
            if not os.path.exists(x):
                return

            with open(x, 'rb') as f:
                of.write(f.read())

def build_ex(config, functions):
    """Additional BIOS build function

    :param config: The environment variables to be used in the build process
    :type config: Dictionary
    :param functions: A dictionary of function pointers
    :type functions: Dictionary
    :returns: config dictionary
    :rtype: Dictionary
    """
    print("build_ex")
    fv_path = os.path.join(config["BUILD_DIR_PATH"], "FV")
    binary_fd = os.path.join(fv_path, "BINARY.fd")
    main_fd = os.path.join(fv_path, "MAIN.fd")
    secpei_fd = os.path.join(fv_path, "SECPEI.fd")
    board_fd = config["BOARD"].upper()
    final_fd = os.path.join(fv_path, "{}.fd".format(board_fd))
    _merge_files((binary_fd, main_fd, secpei_fd), final_fd)
    return None


def post_build_ex(config, functions):
    """Additional Post BIOS build function

    :param config: The environment variables to be used in the post
        build process
    :type config: Dictionary
    :param functions: A dictionary of function pointers
    :type functions: Dictionary
    :returns: config dictionary
    :rtype: Dictionary
    """
    print("post_build_ex")
    fv_path = os.path.join(config["BUILD_DIR_PATH"], "FV")
    board_fd = config["BOARD"].upper()
    final_fd = os.path.join(fv_path, "{}.fd".format(board_fd))
    final_ifwi = os.path.join(fv_path, "{}.bin".format(board_fd))

    ifwi_ingredients_path = os.path.join(config["WORKSPACE_PLATFORM_BIN"], "Ifwi", config["BOARD"])
    flash_descriptor = os.path.join(ifwi_ingredients_path, "FlashDescriptor.bin")
    intel_me = os.path.join(ifwi_ingredients_path, "Me.bin")
    _merge_files((flash_descriptor, intel_me, final_fd), final_ifwi)
    if os.path.isfile(final_fd):
        print("IFWI image can be found at {}".format(final_ifwi))
    return None


def clean_ex(config, functions):
    """Additional clean function

    :param config: The environment variables to be used in the build process
    :type config: Dictionary
    :param functions: A dictionary of function pointers
    :type functions: Dictionary
    :returns: config dictionary
    :rtype: Dictionary
    """
    print("clean_ex")
    return None
