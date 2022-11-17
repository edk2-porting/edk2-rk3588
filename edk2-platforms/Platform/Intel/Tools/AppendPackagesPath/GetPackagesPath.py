## @file
# Get all recursive package paths from special directories.
#
# Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

import os
import glob
import argparse

#
# Globals for help information
#
__prog__ = 'GetPackagesPath.py'
__copyright__ = 'Copyright (c) 2020, Intel Corporation. All rights reserved.'
__description__ = 'Gets all recursive package paths in specified directory.\n'

def __get_packages_path(root):
    """ Gets all recursive package paths in specified directory.
        A directory is a package path if it satisfies conditions below:
        1. it is a directory
        2. it is not an EDK II Package. An EDK II Package (directory) is
           a directory that contains an EDK II package declaration (DEC) file.
        3. it contains at least one first level EDK II Package.
        Note: A directory is not package path but its subdirectory could be.
        Example: edk2-platforms/Features is not package path
        but edk2-platforms/Features/Intel is.

        :param root: The specified directory to find package paths in it,
            the caller should ensure it is an valid directory
        :type root: String
        :returns: Return all recursive package paths
        :rtype: String list
    """

    paths = []
    contain_package = False
    for filename in os.listdir(root):
        # skip files whose name starts with ".", such as ".git"
        if filename.startswith('.'):
            continue
        filepath = os.path.join(root, filename)
        if os.path.isdir(filepath):
            if glob.glob(os.path.join(filepath, '*.dec')):
                # it is an EDK II Package
                contain_package = True
            else:
                # get package paths for subdirectory if it is not package
                paths = paths + __get_packages_path(filepath)

    if contain_package:
        # root is a package path because it contains EDK II Package
        # in first level folder, inset it to head of list
        paths.insert(0, root)

    # return package paths
    return paths

def get_packages_path(directories):
    """ For each direcory in directories, gets all recursive package paths
        in this directory and joins them into one string.

        :param directories: the list of directory
        :type directories: String list
        :returns: Return string of package paths
        :rtype: String
    """

    packages_path = ''
    for directory in directories:
        directory = os.path.abspath(directory)
        if (not os.path.exists(directory)) or (not os.path.isdir(directory)):
            continue

        if glob.glob(os.path.join(directory, '*.dec')):
            # it is an EDK II Package
            continue

        paths = __get_packages_path(directory)
        for path in paths:
            if packages_path == '':
                packages_path = path
            else:
                packages_path += os.pathsep + path
    return packages_path

if __name__ == '__main__':
    # Create command line argument parser object
    parser = argparse.ArgumentParser(
            prog=__prog__,
            description=__description__ + __copyright__,
            conflict_handler='resolve'
    )
    parser.add_argument('directory', nargs='+',
            help='Specified directory where package packages are got from')
    args = parser.parse_args()
    print(get_packages_path(args.directory))
