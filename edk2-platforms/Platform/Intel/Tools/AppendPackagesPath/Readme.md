
# How to use AppendPackagesPath

## Overview

This script calls GetPackagesPath.py to collect all package paths under specified directories and appends them to PACKAGES_PATH environment variable. A sub directory is a qualified package path when an EDKII Package can be found under it.

**Notice**:
The old PACKAGES_PATH will be replaced by new one.

## The usage of the tool

### Windows

Usage: AppendPackagesPath.bat directory [directory ...]
Copyright(c) 2020, Intel Corporation. All rights reserved.
Options:
  --help, -h     Print this help screen and exit

### Ubuntu

Usage: source AppendPackagesPath.sh directory [directory ...]
Copyright(c) 2020, Intel Corporation. All rights reserved.
Options:
  --help, -h     Print this help screen and exit
Please note: This script must be \'sourced\' so the environment can be changed.
. AppendPackagesPath.sh
source AppendPackagesPath.sh
