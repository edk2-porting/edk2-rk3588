#!/bin/bash

#
# Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
# This script calls GetPackagesPath.py to collect all package paths under
# specified directories and appends them to PACKAGES_PATH environment
# variable. A sub directory is a qualified package path when an EDKII
# Package can be found under it.
#
# Note: This script must be \'sourced\' so the environment can be changed:
# source SetPackagesPath.sh
# . AppendPackagesPath.sh

function Usage()
{
    echo "Usage: source AppendPackagesPath.sh directory [directory ...]"
    echo "Copyright(c) 2020, Intel Corporation. All rights reserved."
    echo "Options:"
    echo "  --help, -h     Print this help screen and exit"
    echo "Please note: This script must be \'sourced\' so the environment can be changed."
    echo ". AppendPackagesPath.sh"
    echo "source AppendPackagesPath.sh"
}

function SetEnv()
{
    local tool_path=$(dirname "$BASH_SOURCE")
    local paths=$(python $tool_path/GetPackagesPath.py $@)
    if [ "$PACKAGES_PATH" ]; then
        PACKAGES_PATH=$PACKAGES_PATH:$paths
    else
        PACKAGES_PATH=$paths
    fi
}

if [ $# -eq 0 -o "$1" == "-h" -o "$1" == "--help" -o "$1" == "/?" ]; then
    Usage
else
    SetEnv $@
fi
