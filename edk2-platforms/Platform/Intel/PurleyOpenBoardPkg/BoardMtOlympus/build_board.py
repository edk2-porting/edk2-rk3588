# @ build_board.py
# This adds additional functions to the build_bios.py
#
# Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

"""
This module serves as an additional build steps for the Mt Olympus board
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
    print("Info: re-generating PlatformOffset header files")

    execute_script = functions.get("execute_script")

    command = ["build", "-D", "MAX_SOCKET=" + config.get("MAX_SOCKET", "1"),
               "-m",
               os.path.join(config["PLATFORM_BOARD_PACKAGE"],
                            "Acpi", "BoardAcpiDxe", "Dsdt.inf"),
               "-y",
               config.get("PRE_BUILD_REPORT",
                          os.path.join(config["WORKSPACE"],
                                       "preBuildReport.txt")),
               "--log=" + config.get("PRE_BUILD_LOG",
                                     os.path.join(config["WORKSPACE"],
                                                  "prebuild.log"))]

    _, _, _, code = execute_script(command, config)
    if code != 0:
        print(" ".join(command))
        print("Error re-generating PlatformOffset header files")
        sys.exit(1)

    config["AML_FILTER"] = "\"PSYS\" .MCTL\" .FIX[0-9,A-Z]\""
    print("AML_FILTER= ", config.get("AML_FILTER"))

    # build the command with arguments
    command = ["python",
               os.path.join(config["MIN_PACKAGE_TOOLS"],
                            "AmlGenOffset",
                            "AmlGenOffset.py"),
               "-d", "--aml_filter", config["AML_FILTER"],
               "-o", os.path.join(config["WORKSPACE_PLATFORM"],
                                  config["PLATFORM_BOARD_PACKAGE"],
                                  "Acpi", "BoardAcpiDxe",
                                  "AmlOffsetTable.c"),
               os.path.join(config["BUILD_X64"],
                            "PurleyOpenBoardPkg",
                            "Acpi",
                            "BoardAcpiDxe",
                            "DSDT",
                            "OUTPUT",
                            "Dsdt", "WFPPlatform.offset.h")]

    # execute the command
    _, _, _, code = execute_script(command, config)
    if code != 0:
        print(" ".join(command))
        print("Error re-generating PlatformOffset header files")
        sys.exit(1)

    print("GenOffset done")
    return config


def build_ex(config, functions):
    """Additional BIOS build function

    :param config: The environment variables to be used in
    the build process
    :type config: Dictionary
    :param functions: A dictionary of function pointers
    :type functions: Dictionary
    :returns: config dictionary
    :rtype: Dictionary
    """
    print("build_ex")
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

    execute_script = functions.get("execute_script")

    if not execute_script:
        print("post_build_ex Error")
        sys.exit(1)

    common_patch_command = [os.path.join(config["PYTHON_HOME"], "python"),
                            os.path.join(config["MIN_PACKAGE_TOOLS"],
                                         "PatchFv", "PatchBinFv.py"),
                            config["TARGET"],
                            os.path.join(config["WORKSPACE_SILICON_BIN"],
                                         "PurleySiliconBinPkg", "FV"),
                            os.path.join(config["WORKSPACE"],
                                         "BuildReport.log")]

    fvs_to_patch = ["FvTempMemorySilicon",
                    "FvPreMemorySilicon",
                    "FvPostMemorySilicon",
                    "FvLateSilicon"]
    for fv in fvs_to_patch:
        patch_command = common_patch_command + [fv]
        _, _, _, code = execute_script(patch_command, config)
        if code != 0:
            print(" ".join(patch_command))
            print("Patch Error!")
            sys.exit(1)

    common_rebase_command = [os.path.join(config["PYTHON_HOME"], "python"),
                             os.path.join(config["MIN_PACKAGE_TOOLS"],
                                          "PatchFv", "RebaseBinFv.py"),
                             config["TARGET"],
                             os.path.join(config["WORKSPACE_SILICON_BIN"],
                                          "PurleySiliconBinPkg", "FV"),
                             os.path.join(config["WORKSPACE"],
                                          "BuildReport.log")]

    rebase_command = common_rebase_command +\
        ["FvPreMemorySilicon",
         "gMinPlatformPkgTokenSpaceGuid.PcdFlashFvFspMBase"]

    _, _, _, code = execute_script(rebase_command, config)
    if code != 0:
        print(" ".join(rebase_command))
        print("Patch Error!")
        sys.exit(1)

    rebase_command = common_rebase_command +\
        ["FvPostMemorySilicon",
         "gMinPlatformPkgTokenSpaceGuid.PcdFlashFvFspSBase"]

    _, _, _, code = execute_script(rebase_command, config)
    if code != 0:
        print(" ".join(rebase_command))
        print("Patch Error!")
        sys.exit(1)

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
