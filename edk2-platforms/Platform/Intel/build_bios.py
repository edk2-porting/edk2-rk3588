#!/usr/bin/env python3
#
# @ build_bios.py
# Builds BIOS using configuration files and dynamically
# imported functions from board directory
#
# Copyright (c) 2019 - 2020, Intel Corporation. All rights reserved.<BR>
# Copyright (c) 2021, American Megatrends International LLC.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

"""
This module builds BIOS using configuration files and dynamically
imported functions from board directory
"""


import os
import re
import sys
import glob
import signal
import shutil
import argparse
import traceback
import subprocess
from importlib import import_module

try:
    # python 2.7
    import ConfigParser as configparser
except ImportError:
    # python 3
    import configparser


def pre_build(build_config, build_type="DEBUG", silent=False, toolchain=None):
    """Sets the environment variables that shall be used for the build

        :param build_config: The build configuration as defined in the JSON
            configuration files
        :type build_config: Dictionary
        :param build_type: The build target, DEBUG, RELEASE, RELEASE_PDB,
            TEST_RELEASE
        :type build_type: String
        :param silent: Enables build in silent mode
        :type silent: Boolean
        :param toolchain: Specifies the tool chain tag to use for the build
        :type toolchain: String
        :returns: The updated environment variables
        :rtype: Dictionary
    """

    # get current environment variables
    config = os.environ.copy()

    # patch the build config
    build_config = patch_config(build_config)

    # update the current config with the build config
    config.update(build_config)

    # make the config and build python 2.7 compatible
    config = py_27_fix(config)

    # Set WORKSPACE environment.
    config["WORKSPACE"] = os.path.abspath(os.path.join("..", "..", "..", ""))
    print("Set WORKSPACE as: {}".format(config["WORKSPACE"]))

    # Check whether Git has been installed and been added to system path.
    try:
        subprocess.Popen(["git", "--help"], stdout=subprocess.PIPE)
    except OSError:
        print("The 'git' command is not recognized.")
        print("Please make sure that Git is installed\
                and has been added to system path.")
        sys.exit(1)

    # Create the Conf directory under WORKSPACE
    if not os.path.isdir(os.path.join(config["WORKSPACE"], "Conf")):
        try:
            # create directory
            os.makedirs(os.path.join(config["WORKSPACE"], "Conf"))
            # copy files to it
            config_template_path = os.path.join(config["WORKSPACE"],
                                                config["BASE_TOOLS_PATH"],
                                                "Conf")
            config_path = os.path.join(config["WORKSPACE"], "Conf")
            shutil.copyfile(config_template_path +
                            os.sep + "target.template",
                            config_path + os.sep + "target.txt")
            shutil.copyfile(config_template_path +
                            os.sep + "tools_def.template",
                            config_path + os.sep + "tools_def.txt")
            shutil.copyfile(config_template_path +
                            os.sep + "build_rule.template",
                            config_path + os.sep + "build_rule.txt")
        except OSError:
            print("Error while creating Conf")
            sys.exit(1)

    # Set other environments.
    # Basic Rule:
    # Platform override Silicon override Core
    # Source override Binary
    config["WORKSPACE_PLATFORM"] = os.path.join(config["WORKSPACE"],
                                                config["WORKSPACE_PLATFORM"])
    config["WORKSPACE_SILICON"] = os.path.join(config["WORKSPACE"],
                                               config["WORKSPACE_SILICON"])
    config["WORKSPACE_FEATURES"] = os.path.join(config["WORKSPACE"],
                                               config["WORKSPACE_FEATURES"])
    config["WORKSPACE_DRIVERS"] = os.path.join(config["WORKSPACE"],
                                               config["WORKSPACE_DRIVERS"])
    config["WORKSPACE_PLATFORM_BIN"] = \
        os.path.join(config["WORKSPACE"], config["WORKSPACE_PLATFORM_BIN"])
    config["WORKSPACE_SILICON_BIN"] = \
        os.path.join(config["WORKSPACE"], config["WORKSPACE_SILICON_BIN"])
    config["WORKSPACE_FSP_BIN"] = os.path.join(config["WORKSPACE"],
                                               config["WORKSPACE_FSP_BIN"])

    # add to package path
    config["PACKAGES_PATH"] = config["WORKSPACE_PLATFORM"]
    config["PACKAGES_PATH"] += os.pathsep + config["WORKSPACE_SILICON"]
    config["PACKAGES_PATH"] += os.pathsep + config["WORKSPACE_SILICON_BIN"]
    config["PACKAGES_PATH"] += os.pathsep + config["WORKSPACE_FEATURES"]
    # add all feature domains in WORKSPACE_FEATURES to package path
    for filename in os.listdir(config["WORKSPACE_FEATURES"]):
        filepath = os.path.join(config["WORKSPACE_FEATURES"], filename)
        # feature domains folder does not contain dec file
        if os.path.isdir(filepath) and \
          not glob.glob(os.path.join(filepath, "*.dec")):
            config["PACKAGES_PATH"] += os.pathsep + filepath
    config["PACKAGES_PATH"] += os.pathsep + config["WORKSPACE_DRIVERS"]
    config["PACKAGES_PATH"] += os.pathsep + \
        os.path.join(config["WORKSPACE"], config["WORKSPACE_FSP_BIN"])
    config["PACKAGES_PATH"] += os.pathsep + \
        os.path.join(config["WORKSPACE"], config["WORKSPACE_CORE"])
    config["PACKAGES_PATH"] += os.pathsep + os.path.join(config["WORKSPACE"])
    config["PACKAGES_PATH"] += os.pathsep + config["WORKSPACE_PLATFORM_BIN"]
    config["EDK_TOOLS_PATH"] = os.path.join(config["WORKSPACE"],
                                            config["EDK_TOOLS_PATH"])
    config["BASE_TOOLS_PATH"] = config["EDK_TOOLS_PATH"]
    config["EDK_TOOLS_BIN"] = os.path.join(config["WORKSPACE"],
                                           config["EDK_TOOLS_BIN"])

    #
    # Board may have different FSP binary between API and Dispatch modes.
    # In API mode if FSP_BIN_PKG_FOR_API_MODE is assigned, it should
    # override FSP_BIN_PKG.
    #
    if config.get("API_MODE_FSP_WRAPPER_BUILD", "FALSE") == "TRUE":
        if config.get("FSP_BIN_PKG_FOR_API_MODE") is not None:
            config['FSP_BIN_PKG'] = config['FSP_BIN_PKG_FOR_API_MODE']

    config["PLATFORM_FSP_BIN_PACKAGE"] = \
        os.path.join(config['WORKSPACE_FSP_BIN'], config['FSP_BIN_PKG'])
    config['PROJECT_DSC'] = os.path.join(config["WORKSPACE_PLATFORM"],
                                         config['PROJECT_DSC'])
    config['BOARD_PKG_PCD_DSC'] = os.path.join(config["WORKSPACE_PLATFORM"],
                                               config['BOARD_PKG_PCD_DSC'])
    config["CONF_PATH"] = os.path.join(config["WORKSPACE"], "Conf")

    # get the python path
    if os.environ.get("PYTHON_HOME") is None:
        if os.environ.get("PYTHONPATH") is not None:
            config["PYTHON_HOME"] = os.environ.get("PYTHONPATH")
        else:
            config["PYTHON_HOME"] = os.path.dirname(sys.executable)
            config["PYTHONPATH"] = config["PYTHON_HOME"]

    if config.get("PYTHON_HOME") is None or \
       not os.path.exists(config.get("PYTHON_HOME")):
        print("PYTHON_HOME environment variable is not found")
        sys.exit(1)

    # if python is installed, disable the binary base tools.
    # python is installed if this code is running :)
    if config.get("PYTHON_HOME") is not None:
        if config.get("EDK_TOOLS_BIN") is not None:
            del config["EDK_TOOLS_BIN"]

    # Run edk setup and  update config
    if os.name == 'nt':
        edk2_setup_cmd = [os.path.join(config["EFI_SOURCE"], "edksetup"),
                          "Rebuild"]

        if config.get("EDK_SETUP_OPTION") and \
           config["EDK_SETUP_OPTION"] != " ":
            edk2_setup_cmd.append(config["EDK_SETUP_OPTION"])

        _, _, result, return_code = execute_script(edk2_setup_cmd,
                                                   config,
                                                   collect_env=True,
                                                   shell=True)
        if return_code == 0 and result is not None and isinstance(result,
                                                                  dict):
            config.update(result)

    # nmake BaseTools source
    # and enable BaseTools source build
    shell = True
    command = ["nmake", "-f", os.path.join(config["BASE_TOOLS_PATH"],
                                           "Makefile")]
    if os.name == "posix":  # linux
        shell = False
        command = ["make", "-C", os.path.join(config["BASE_TOOLS_PATH"])]

    _, _, result, return_code = execute_script(command, config, shell=shell)
    if return_code != 0:
        build_failed(config)

    #
    # build platform silicon tools
    #
    # save the current workspace
    saved_work_directory = config["WORKSPACE"]
    # change the workspace to silicon tools directory
    config["WORKSPACE"] = os.path.join(config["WORKSPACE_SILICON"], "Tools")

    command = ["nmake"]
    if os.name == "posix":  # linux
        command = ["make"]
        # add path to generated FitGen binary to
        # environment path variable
        config["PATH"] += os.pathsep + \
                          os.path.join(config["BASE_TOOLS_PATH"],
                                       "Source", "C", "bin")

    # build the silicon tools
    _, _, result, return_code = execute_script(command, config, shell=shell)
    if return_code != 0:
        build_failed(config)

    # restore WORKSPACE environment variable
    config["WORKSPACE"] = saved_work_directory

    config["SILENT_MODE"] = 'TRUE' if silent else 'FALSE'

    print("==============================================")

    if os.path.isfile(os.path.join(config['WORKSPACE'], "Prep.log")):
        os.remove(os.path.join(config['WORKSPACE'], "Prep.log"))

    config["PROJECT"] = os.path.join(config["PLATFORM_BOARD_PACKAGE"],
                                     config["BOARD"])

    # Setup Build
    # @todo: Need better TOOL_CHAIN_TAG detection
    if toolchain is not None:
        config["TOOL_CHAIN_TAG"] = toolchain
    elif config.get("TOOL_CHAIN_TAG") is None:
        if os.name == 'nt':
            config["TOOL_CHAIN_TAG"] = "VS2015"
        else:
            config["TOOL_CHAIN_TAG"] = "GCC5"

    # echo Show CL revision
    config["PrepRELEASE"] = build_type

    if build_type == "DEBUG":
        config["TARGET"] = 'DEBUG'
        config["TARGET_SHORT"] = 'D'
    else:
        config["TARGET"] = 'RELEASE'
        config["TARGET_SHORT"] = 'R'

    # set BUILD_DIR_PATH path
    config["BUILD_DIR_PATH"] = os.path.join(config["WORKSPACE"],
                                            'Build',
                                            config["PROJECT"],
                                            "{}_{}".format(
                                                config["TARGET"],
                                                config["TOOL_CHAIN_TAG"]))
    # set BUILD_DIR path
    config["BUILD_DIR"] = os.path.join('Build',
                                       config["PROJECT"],
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

    # Set FSP_WRAPPER_BUILD
    if config['FSP_WRAPPER_BUILD'] == "TRUE":
        # Create dummy Fsp_Rebased_S_padded.fd to build the BiosInfo.inf
        # if it is wrapper build, due to the SECTION inclusion
        open(os.path.join(config["WORKSPACE_FSP_BIN"],
                          config["FSP_BIN_PKG"],
                          "Fsp_Rebased_S_padded.fd"), 'w').close()

    if not os.path.isdir(config["BUILD_X64"]):
        try:
            os.mkdir(config["BUILD_X64"])
        except OSError:
            print("Error while creating {}".format(config["BUILD_X64"]))
            sys.exit(1)

    # update config file with changes
    update_target_file(config)

    # Additional pre build scripts for this platform
    result = pre_build_ex(config)
    if result is not None and isinstance(result, dict):
        config.update(result)

    # print user settings
    print("BIOS_SIZE_OPTION     = {}".format(config["BIOS_SIZE_OPTION"]))
    print("EFI_SOURCE           = {}".format(config["EFI_SOURCE"]))
    print("TARGET               = {}".format(config["TARGET"]))
    print("TARGET_ARCH          = {}".format("IA32 X64"))
    print("TOOL_CHAIN_TAG       = {}".format(config["TOOL_CHAIN_TAG"]))
    print("WORKSPACE            = {}".format(config["WORKSPACE"]))
    print("WORKSPACE_CORE       = {}".format(config["WORKSPACE_CORE"]))
    print("EXT_BUILD_FLAGS      = {}".format(config["EXT_BUILD_FLAGS"]))

    return config


def build(config):
    """Builds the BIOS image

        :param config: The environment variables to be used
            in the build process
        :type config: Dictionary
        :returns: nothing
    """

    if config["FSP_WRAPPER_BUILD"] == "TRUE":
        pattern = "Fsp_Rebased.*\\.fd$"
        file_dir = os.path.join(config['WORKSPACE_FSP_BIN'],
                                config['FSP_BIN_PKG'])
        for item in os.listdir(file_dir):
            if re.search(pattern, item):
                os.remove(os.path.join(file_dir, item))

        command = [os.path.join(config['PYTHON_HOME'], "python"),
                   os.path.join(config['WORKSPACE_PLATFORM'],
                                config['PLATFORM_PACKAGE'],
                                'Tools', 'Fsp',
                                'RebaseFspBinBaseAddress.py'),
                   os.path.join(config['WORKSPACE_PLATFORM'],
                                config['FLASH_MAP_FDF']),
                   os.path.join(config['WORKSPACE_FSP_BIN'],
                                config['FSP_BIN_PKG']),
                   "Fsp.fd",
                   "0x0"]

        _, _, _, return_code = execute_script(command, config, shell=False)

        if return_code != 0:
            print("ERROR:RebaseFspBinBaseAddress failed")
            sys.exit(return_code)

        # create Fsp_Rebased.fd which is Fsp_Rebased_S.fd +
        # Fsp_Rebased_M + Fsp_Rebased_T
        with open(os.path.join(file_dir, "Fsp_Rebased_S.fd"), 'rb') as fsp_s, \
                open(os.path.join(file_dir,
                                  "Fsp_Rebased_M.fd"), 'rb') as fsp_m, \
                open(os.path.join(file_dir,
                                  "Fsp_Rebased_T.fd"), 'rb') as fsp_t:

            fsp_rebased = fsp_s.read() + fsp_m.read() + fsp_t.read()
            with open(os.path.join(file_dir,
                                   "Fsp_Rebased.fd"), 'wb') as new_fsp:
                new_fsp.write(fsp_rebased)

        if not os.path.isfile(os.path.join(file_dir, "Fsp_Rebased.fd")):
            print("!!! ERROR:failed to create fsp!!!")
            sys.exit(1)

    # Output the build variables the user has selected.
    print("==========================================")
    print(" User Selected build options:")
    print(" SILENT_MODE    = ", config.get("SILENT_MODE"))
    print(" REBUILD_MODE   = ", config.get("REBUILD_MODE"))
    print(" BUILD_ROM_ONLY = ", config.get("BUILD_ROM_ONLY"))
    print(" BINARY_CACHE_CMD_LINE = ", config.get("HASH"), config.get("BINARY_CACHE_CMD_LINE"))
    print("==========================================")

    command = ["build", "-n", config["NUMBER_OF_PROCESSORS"]]

    if config["REBUILD_MODE"] and config["REBUILD_MODE"] != "":
        command.append(config["REBUILD_MODE"])

    if config["EXT_BUILD_FLAGS"] and config["EXT_BUILD_FLAGS"] != "":
        ext_build_flags = config["EXT_BUILD_FLAGS"].split(" ")
        ext_build_flags = [x.strip() for x in ext_build_flags]
        ext_build_flags = [x for x in ext_build_flags if x != ""]
        command.extend(ext_build_flags)

    if config.get('BINARY_CACHE_CMD_LINE'):
        command.append(config['HASH'])
        command.append(config['BINARY_CACHE_CMD_LINE'])

    if config.get("SILENT_MODE", "FALSE") == "TRUE":
        command.append("--silent")
        command.append("--quiet")

    else:
        command.append("--log=" + config.get("BUILD_LOG", "Build.log"))
        command.append("--report-file=" +
                       config.get("BUILD_REPORT", "BuildReport.log"))

    if config.get("VERBOSE", "FALSE") == "TRUE":
        command.append("--verbose")

    if config.get("MAX_SOCKET") is not None:
        command.append("-D")
        command.append("MAX_SOCKET=" + config["MAX_SOCKET"])

    if config.get("API_MODE_FSP_WRAPPER_BUILD", "FALSE") == "TRUE":
        #Override PCD to enable API mode FSP wrapper.
        command.append("--pcd")
        command.append("gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection=1")

    shell = True
    if os.name == "posix":
        shell = False

    _, _, _, exit_code = execute_script(command, config, shell=shell)
    if exit_code != 0:
        build_failed(config)

    # Additional build scripts for this platform
    result = build_ex(config)
    if result is not None and isinstance(result, dict):
        config.update(result)

    return config


def post_build(config):
    """Post build process of BIOS image

    :param config: The environment variables to be used in the build process
    :type config: Dictionary
    :returns: nothing
    """
    print("Running post_build to complete the build process.")
    board_fd = config["BOARD"].upper()
    final_fd = os.path.join(config["BUILD_DIR_PATH"], "FV",
                            "{}.fd".format(board_fd))

    if config["BIOS_INFO_GUID"]:
        # Generate the fit table
        print("Generating FIT ...")
        if os.path.isfile(final_fd):
            temp_fd = os.path.join(config["BUILD_DIR_PATH"], "FV",
                                   "{}_.fd".format(board_fd))
            shell = True
            command = ["FitGen", "-D",
                       final_fd, temp_fd, "-NA",
                       "-I", config["BIOS_INFO_GUID"]]   #@todo: Need mechanism to add additional options to the FitGen command line

            if os.name == "posix": # linux
                shell = False

            _, _, result, return_code = execute_script(command, config, shell=shell)
            if return_code != 0:
                print("Error while generating fit")
            else:
                # copy output to final binary
                shutil.copyfile(temp_fd, final_fd)
                # remove temp file
                os.remove(temp_fd)
        else:
            print("{} does not exist".format(final_fd))
            # remove temp file

    # Additional build scripts for this platform
    result = post_build_ex(config)
    if result is not None and isinstance(result, dict):
        config.update(result)

    # cleanup
    pattern = "Fsp_Rebased.*\\.fd$"
    file_dir = os.path.join(config['WORKSPACE_FSP_BIN'],
                            config['FSP_BIN_PKG'])
    for item in os.listdir(file_dir):
        if re.search(pattern, item):
            os.remove(os.path.join(file_dir, item))

    if config.get("DYNAMIC_BUILD_INIT_FILES") is not None:
        for item in config["DYNAMIC_BUILD_INIT_FILES"].split(","):
            try:
                os.remove(item)  # remove __init__.py
                os.remove(item + "c")  # remove __init__.pyc as well
            except OSError:
                pass

    print("Done")
    if os.path.isfile(final_fd):
        print("Fd file can be found at {}".format(final_fd))

def build_failed(config):
    """Displays results when build fails

        :param config: The environment variables used in the build process
        :type config: Dictionary
        :returns: nothing
    """
    print(" The EDKII BIOS Build has failed!")
    # clean up
    if config.get("DYNAMIC_BUILD_INIT_FILES") is not None:
        for item in config["DYNAMIC_BUILD_INIT_FILES"].split(","):
            if os.path.isfile(item):
                try:
                    os.remove(item)  # remove __init__.py
                    os.remove(item + "c")  # remove __init__.pyc as well
                except OSError:
                    pass
    sys.exit(1)


def import_platform_lib(path, function):
    """Imports custom functions for the platforms being built

        :param path: the location of the custom build script to be executed
        :type path: String
        :param path: the function to be executed
        :type path: String
        :returns: nothing
    """
    if path.endswith(".py"):
        path = path[:-3]
    path = path.replace(os.sep, ".")
    module = import_module(path)
    lib = getattr(module, function)
    return lib


def pre_build_ex(config):
    """ An extension of the pre_build process as defined platform
        specific pre_build setup script

        :param config: The environment variables used in the pre build process
        :type config: Dictionary
        :returns: config dictionary
        :rtype: Dictionary
    """
    if config.get("ADDITIONAL_SCRIPTS"):
        try:
            platform_function =\
                import_platform_lib(config["ADDITIONAL_SCRIPTS"],
                                    "pre_build_ex")
            functions = {"execute_script": execute_script}
            return platform_function(config, functions)
        except ImportError as error:
            print(config["ADDITIONAL_SCRIPTS"], str(error))
            build_failed(config)
    return None


def build_ex(config):
    """ An extension of the build process as defined platform
        specific build setup script

        :param config: The environment variables used in the build process
        :type config: Dictionary
        :returns: config dictionary
        :rtype: Dictionary
    """
    if config.get("ADDITIONAL_SCRIPTS"):
        try:
            platform_function =\
                import_platform_lib(config["ADDITIONAL_SCRIPTS"],
                                    "build_ex")
            functions = {"execute_script": execute_script}
            return platform_function(config, functions)
        except ImportError as error:
            print("error", config["ADDITIONAL_SCRIPTS"], str(error))
            build_failed(config)
    return None


def post_build_ex(config):
    """ An extension of the post build process as defined platform
        specific build setup script

        :param config: The environment variables used in the post build
            process
        :type config: Dictionary
        :returns: config dictionary
        :rtype: Dictionary
    """
    if config.get("ADDITIONAL_SCRIPTS"):
        try:
            platform_function =\
                import_platform_lib(config["ADDITIONAL_SCRIPTS"],
                                    "post_build_ex")
            functions = {"execute_script": execute_script}
            return platform_function(config, functions)
        except ImportError as error:
            print(config["ADDITIONAL_SCRIPTS"], str(error))
            build_failed(config)
    return None


def clean_ex(config):
    """ An extension of the platform cleaning

        :param config: The environment variables used in the clean process
        :type config: Dictionary
        :returns: config dictionary
        :rtype: Dictionary
    """
    if config.get("ADDITIONAL_SCRIPTS"):
        try:
            platform_function =\
                import_platform_lib(config["ADDITIONAL_SCRIPTS"],
                                    "clean_ex")
            functions = {"execute_script": execute_script}
            return platform_function(config, functions)
        except ImportError as error:
            print(config["ADDITIONAL_SCRIPTS"], str(error))
            build_failed(config)
    return None


def get_environment_variables(std_out_str, marker):
    """Gets the environment variables from a process

        :param std_out_str: The std_out pipe
        :type std_out_str: String
        :param marker: A begining and end mark of environment
            variables printed to std_out
        :type marker: String
        :returns: The environment variables read from the process' std_out pipe
        :rtype: Tuple
    """
    start_env_update = False
    environment_vars = {}
    out_put = ""
    for line in std_out_str.split("\n"):
        if start_env_update and len(line.split("=")) == 2:
            key, value = line.split("=")
            environment_vars[key] = value
        else:
            out_put += "\n" + line.replace(marker, "")

        if marker in line:
            if start_env_update:
                start_env_update = False
            else:
                start_env_update = True
    return (out_put, environment_vars)


def execute_script(command, env_variables, collect_env=False,
                   enable_std_pipe=False, shell=True):
    """launches a process that executes a script/shell command passed to it

        :param command: The command/script with its commandline
            arguments to be executed
        :type command:  List:String
        :param env_variables: Environment variables passed to the process
        :type env_variables: String
        :param collect_env: Enables the collection of environment variables
            when process execution is done
        :type collect_env: Boolean
        :param enable_std_pipe: Enables process out to be piped to
        :type enable_std_pipe: String
        :returns: a tuple of std_out, stderr , environment variables,
            return code
        :rtype: Tuple: (std_out, stderr , enVar, return_code)
    """

    print("Calling " + " ".join(command))

    env_marker = '-----env-----'
    env = {}
    kwarg = {"env": env_variables,
             "universal_newlines": True,
             "shell": shell,
             "cwd": env_variables["WORKSPACE"]}

    if enable_std_pipe or collect_env:
        kwarg["stdout"] = subprocess.PIPE
        kwarg["stderr"] = subprocess.PIPE

    # collect environment variables
    if collect_env:
        # get the binary that prints environment variables based on os
        if os.name == 'nt':
            get_var_command = "set"
        else:
            get_var_command = "env"
        # modify the command to print the environment variables
        if isinstance(command, list):
            command += ["&&", "echo", env_marker, "&&",
                        get_var_command, "&&", "echo", env_marker]
        else:
            command += " " + " ".join(["&&", "echo", env_marker,
                                       "&&", get_var_command,
                                       "&&", "echo", env_marker])

    # execute the command
    execute = subprocess.Popen(command, **kwarg)
    std_out, stderr = execute.communicate()
    code = execute.returncode

    # wait for process to be done
    execute.wait()

    # if collect environment variables
    if collect_env:
        # get the new environment variables
        std_out, env = get_environment_variables(std_out, env_marker)
    return (std_out, stderr, env, code)


def patch_config(config):
    """ An extension of the platform cleaning

        :param config: The environment variables used in the build process
        :type config: Dictionary
        :returns: config dictionary
        :rtype: Dictionary
    """
    new_config = {}
    for key in config:
        new_config[str(key)] = str(config[key].replace("/", os.sep))
    return new_config


def py_27_fix(config):
    """  Prepares build for python 2.7 => build
        :param config: The environment variables used in the build process
        :type config: Dictionary
        :returns: config dictionary
        :rtype: Dictionary
    """
    if not sys.version_info > (3, 0):
        path_list = []
        # create __init__.py in directories in this path
        if config.get("ADDITIONAL_SCRIPTS"):
            # get the directory
            path_to_directory =\
                os.path.dirname(config.get("ADDITIONAL_SCRIPTS"))
            path = ""
            for directories in path_to_directory.split(os.sep):
                path += directories + os.sep
                init_file = path + os.sep + "__init__.py"
                if not os.path.isfile(init_file):
                    open(init_file, 'w').close()
                    path_list.append(init_file)
            config["DYNAMIC_BUILD_INIT_FILES"] = ",".join(path_list)

    return config


def clean(build_config, board=False):
    """Cleans the build workspace

        :param config: The environment variables used in the build process
        :type config: Dictionary
        :param board: This flag specifies specific board clean
        :type board: Bool
        :returns: nothing
    """

    # patch the config
    build_config = patch_config(build_config)

    # get current environment variables
    config = os.environ.copy()

    # update it with the build variables
    config.update(build_config)

    if config.get('WORKSPACE') is None or not config.get('WORKSPACE'):
        config["WORKSPACE"] =\
                os.path.abspath(os.path.join("..", "..", "..", ""))

    # build cleanall
    print("Cleaning directories...")

    if board:
        platform_pkg = config.get("PLATFORM_BOARD_PACKAGE", None)
        if platform_pkg is None or\
            not os.path.isdir(os.path.join(config['WORKSPACE'],
                                           "Build", platform_pkg)):
            print("Platform package not found")
            sys.exit(1)
        else:
            print("Removing " + os.path.join(config['WORKSPACE'],
                                             "Build", platform_pkg))
            shutil.rmtree(os.path.join(config['WORKSPACE'],
                                       "Build", platform_pkg))

    else:
        if os.path.isdir(os.path.join(config['WORKSPACE'], "Build")):
            print("Removing " + os.path.join(config['WORKSPACE'], "Build"))
            shutil.rmtree(os.path.join(config['WORKSPACE'], "Build"))

    if os.path.isdir(os.path.join(config['WORKSPACE'], "Conf")):
        print("Removing " + os.path.join(config['WORKSPACE'], "Conf"))
        shutil.rmtree(os.path.join(config['WORKSPACE'], "Conf"))

    print("Cleaning files...")

    if os.path.isfile(os.path.join(config['WORKSPACE'],
                                   config.get("BUILD_REPORT",
                                              "BuildReport.log"))):
        print("Removing ", os.path.join(config['WORKSPACE'],
                                        config.get("BUILD_REPORT",
                                                   "BuildReport.log")))
        os.remove(os.path.join(config['WORKSPACE'],
                               config.get("BUILD_REPORT", "BuildReport.log")))

    print("  All done...")

    sys.exit(0)


def update_target_file(config):
    """Updates Conf's target file that will be used in the build

        :param config: The environment variables used in the build process
        :type config: Dictionary
        :returns: True if update was successful and False if update fails
        :rtype: Boolean
    """
    contents = None
    result = False
    with open(os.path.join(config["CONF_PATH"], "target.txt"), 'r') as target:
        contents = target.readlines()
        options_list = ['ACTIVE_PLATFORM', 'TARGET',
                        'TARGET_ARCH', 'TOOL_CHAIN_TAG', 'BUILD_RULE_CONF']
        modified = []

        # remove these options from the config file
        for line in contents:
            if line.replace(" ", "")[0] != '#' and\
             any(opt in line for opt in options_list):
                continue
            modified.append(line)

        # replace with config options provided
        string = "{} = {}\n".format("ACTIVE_PLATFORM",
                                    os.path.join(
                                        config['WORKSPACE_PLATFORM'],
                                        config['PLATFORM_BOARD_PACKAGE'],
                                        config['BOARD'],
                                        config['PROJECT_DSC']))
        modified.append(string)

        string = "{} = {}\n".format("TARGET", config['TARGET'])
        modified.append(string)

        string = "TARGET_ARCH = IA32 X64\n"
        modified.append(string)

        string = "{} = {}\n".format("TOOL_CHAIN_TAG", config['TOOL_CHAIN_TAG'])
        modified.append(string)

        string = "{} = {}\n".format("BUILD_RULE_CONF",
                                    os.path.join("Conf", "build_rule.txt"))
        modified.append(string)

    if modified is not None:
        with open(os.path.join(config["WORKSPACE"],
                               "Conf", "target.txt"), 'w') as target:
            for line in modified:
                target.write(line)
            result = True

    return result


def get_config():
    """Reads the default projects config file

        :returns: The config defined in the the Build.cfg file
        :rtype: Dictionary
    """
    config_file = configparser.RawConfigParser()
    config_file.optionxform = str
    config_file.read('build.cfg')
    config_dictionary = {}
    for section in config_file.sections():
        dictionary = dict(config_file.items(section))
        config_dictionary[section] = dictionary
    return config_dictionary


def get_platform_config(platform_name, config_data):
    """ Reads the platform specific config file

        param platform_name: The name of the platform to be built
        :type platform_name: String
        param configData: The environment variables to be
            used in the build process
        :type configData: Dictionary
        :returns: The config defined in the the Build.cfg file
        :rtype: Dictionary
    """
    config = {}

    platform_data = config_data.get("PLATFORMS")
    path = platform_data.get(platform_name)
    config_file = configparser.RawConfigParser()
    config_file.optionxform = str
    config_file.read(path)
    for section in config_file.sections():
        config[section] = dict(config_file.items(section))

    return config


def get_cmd_config_arguments(arguments):
    """Get commandline config arguments

    param arguments: The environment variables to be used in the build process
    :type arguments: argparse
    :returns: The config dictionary built from the commandline arguments
    :rtype: Dictionary
    """
    result = {}
    if arguments.capsule is True:
        result["CAPSULE_BUILD"] = "1"

    if arguments.performance is True:
        result["PERFORMANCE_BUILD"] = "TRUE"

    if arguments.fsp is True:
        result["FSP_WRAPPER_BUILD"] = "TRUE"

    if arguments.fspapi is True:
        result["API_MODE_FSP_WRAPPER_BUILD"] = "TRUE"

    if not arguments.UseHashCache:
        result['BINARY_CACHE_CMD_LINE'] = ''
    elif arguments.BinCacheDest:
        result['HASH'] = '--hash'
        result['BINARY_CACHE_CMD_LINE'] = '--binary-destination=%s' % arguments.BinCacheDest
    elif arguments.BinCacheSource:
        result['HASH'] = '--hash'
        result['BINARY_CACHE_CMD_LINE'] = '--binary-source=%s' % arguments.BinCacheSource
    else:
        result['BINARY_CACHE_CMD_LINE'] = ''

    return result


def get_cmd_arguments(build_config):
    """ Get commandline inputs from user

        param config_data: The environment variables to be
            used in the build process
        :type config_data: Dictionary
        :returns: The commandline arguments input by the user
        :rtype: argparse object
    """

    class PrintPlatforms(argparse.Action):
        """ this is an argparse action that lists the available platforms
        """
        def __call__(self, parser, namespace, values, option_string=None):
            print("Platforms:")
            for key in build_config.get("PLATFORMS"):
                print("    " + key)
            setattr(namespace, self.dest, values)
            sys.exit(0)

    # get the build commands
    parser = argparse.ArgumentParser(description="Build Help")
    parser.add_argument('--platform', '-p', dest="platform",
                        help='the platform to build',
                        choices=build_config.get("PLATFORMS"),
                        required=('-l' not in sys.argv and
                                  '--cleanall' not in sys.argv))

    parser.add_argument('--toolchain', '-t', dest="toolchain",
                        help="using the Tool Chain Tagname to build \
                            the platform,overriding \
                            target.txt's TOOL_CHAIN_TAG definition")

    parser.add_argument("--DEBUG", '-d', help="debug flag",
                        action='store_const', dest="target",
                        const="DEBUG", default="DEBUG")

    parser.add_argument("--RELEASE", '-r', help="release flag",
                        action='store_const',
                        dest="target", const="RELEASE")

    parser.add_argument("--TEST_RELEASE", '-tr', help="test Release flag",
                        action='store_const',
                        dest="target", const="TEST_RELEASE")

    parser.add_argument("--RELEASE_PDB", '-rp', help="release flag",
                        action='store_const', dest="target",
                        const="RELEASE_PDB")

    parser.add_argument('--list', '-l', action=PrintPlatforms,
                        help='lists available platforms', nargs=0)

    parser.add_argument('--cleanall', dest='clean_all',
                        help='cleans all', action='store_true')

    parser.add_argument('--clean', dest='clean',
                        help='cleans specific platform', action='store_true')

    parser.add_argument("--capsule", help="capsule build enabled",
                        action='store_true', dest="capsule")

    parser.add_argument("--silent", help="silent build enabled",
                        action='store_true', dest="silent")

    parser.add_argument("--performance", help="performance build enabled",
                        action='store_true', dest="performance")

    parser.add_argument("--fsp", help="fsp wrapper build enabled",
                        action='store_true', dest="fsp")

    parser.add_argument("--fspapi", help="API mode fsp wrapper build enabled",
                        action='store_true', dest="fspapi")

    parser.add_argument("--hash", action="store_true", dest="UseHashCache", default=False,
                        help="Enable hash-based caching during build process.")

    parser.add_argument("--binary-destination", help="Generate a cache of binary \
                            files in the specified directory.",
                        action='store', dest="BinCacheDest")

    parser.add_argument("--binary-source", help="Consume a cache of binary files \
                                from the specified directory.",
                        action='store', dest="BinCacheSource")

    return parser.parse_args()


def keyboard_interruption(int_signal, int_frame):
    """ Catches a keyboard interruption handler

        param int_signal: The signal this handler is called with
        :type int_signal: Signal
        param int_frame: The signal this handler is called with
        :type int_frame: frame
        :rtype: nothing
    """
    print("Signal #: {} Frame: {}".format(int_signal, int_frame))
    print("Quiting...")
    sys.exit(0)


def main():
    """ The main function of this module
        :rtype: nothing
    """
    # to quit the build
    signal.signal(signal.SIGINT, keyboard_interruption)

    # get general build configurations
    build_config = get_config()

    # get commandline parameters
    arguments = get_cmd_arguments(build_config)

    if arguments.clean_all:
        clean(build_config.get("DEFAULT_CONFIG"))

    # get platform specific config
    platform_config = get_platform_config(arguments.platform, build_config)

    # update general build config with platform specific config
    config = build_config.get("DEFAULT_CONFIG")
    config.update(platform_config.get("CONFIG"))

    # if user selected clean
    if arguments.clean:
        clean(config, board=True)

    # Override config with cmd arguments
    cmd_config_args = get_cmd_config_arguments(arguments)
    config.update(cmd_config_args)

    # get pre_build configurations
    config = pre_build(config,
                       build_type=arguments.target,
                       toolchain=arguments.toolchain,
                       silent=arguments.silent)

    # build selected platform
    config = build(config)

    # post build
    post_build(config)


if __name__ == "__main__":
    try:
        EXIT_CODE = 0
        main()
    except Exception as error:
        EXIT_CODE = 1
        traceback.print_exc()
    sys.exit(EXIT_CODE)
