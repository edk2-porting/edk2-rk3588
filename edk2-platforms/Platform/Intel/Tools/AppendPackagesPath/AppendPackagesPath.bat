@REM @file
@REM Windows batch file to set PACKAGES_PATH environment
@REM
@REM Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
@REM SPDX-License-Identifier: BSD-2-Clause-Patent
@REM
@REM This script calls GetPackagesPath.py to collect all package paths under
@REM specified directories and appends them to PACKAGES_PATH environment
@REM variable. A sub directory is a qualified package path when an EDKII
@REM Package can be found under it.

@echo off
@if /I "%1"=="" @goto Usage
@if /I "%1"=="-h" @goto Usage
@if /I "%1"=="--help" @goto Usage
@if /I "%1"=="/?" @goto Usage

for /f %%i in ('python %~dp0\GetPackagesPath.py %*') do (
    if defined PACKAGES_PATH (
        set "PACKAGES_PATH=%PACKAGES_PATH%;%%i"
    ) else (
        set "PACKAGES_PATH=%%i"
    )
)
@goto End

:Usage
@echo Usage: AppendPackagesPath.bat directory [directory ...]
@echo Copyright(c) 2020, Intel Corporation. All rights reserved.
@echo Options:
@echo   --help, -h     Print this help screen and exit

:End
