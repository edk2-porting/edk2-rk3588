# @ build_board.py
# This is a sample code provides Optional dynamic imports
# of build functions to the BuildBios.py script
#
# Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

"""
This module serves as a sample implementation of the build extension
scripts
"""


def pre_build_ex(config, functions):
    """Additional Pre BIOS build function

    :param config: The environment variables to be used in the build process
    :type config: Dictionary
    :param functions: A dictionary of function pointers
    :type functions: Dictionary
    :returns: nothing
    """
    print("pre_build_ex")
    return None


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
