@REM @file
@REM
@REM Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
@REM Copyright (c) 2021, American Megatrends International LLC.<BR>
@REM SPDX-License-Identifier: BSD-2-Clause-Patent
@REM

@echo off

pushd ..\..\..\..\..\

@REM Set WORKSPACE environment.
set WORKSPACE=%cd%
echo.
echo Set WORKSPACE as: %WORKSPACE%
echo.

@REM Check whether Git has been installed and been added to system path.
git --help >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
  echo.
  echo The 'git' command is not recognized.
  echo Please make sure that Git is installed and has been added to system path.
  echo.
  goto :EOF
)

@REM Create the Conf directory under WORKSPACE
if not exist %WORKSPACE%\Conf (
  mkdir Conf
)

@REM Set other environments.
@REM Basic Rule:
@REM   Platform override Silicon override Core
@REM   Source override Binary

set PACKAGES_PATH=%WORKSPACE%\edk2-platforms\Platform\Intel;%WORKSPACE%\edk2-platforms\Silicon\Intel;%WORKSPACE%\edk2-platforms\Features\Intel;%WORKSPACE%\edk2-platforms\Features\Intel\Debugging;%WORKSPACE%\edk2-platforms\Features\Intel\Network;%WORKSPACE%\edk2-platforms\Features\Intel\OutOfBandManagement;%WORKSPACE%\edk2-platforms\Features\Intel\PowerManagement;%WORKSPACE%\edk2-platforms\Features\Intel\SystemInformation;%WORKSPACE%\edk2-platforms\Features\Intel\UserInterface;%WORKSPACE%\edk2-non-osi\Silicon\Intel;%WORKSPACE%\edk2;%WORKSPACE%

set EDK_TOOLS_BIN=%WORKSPACE%\edk2-BaseTools-win32

@if not defined PYTHON_HOME (
  @if exist C:\Python27 (
    set PYTHON_HOME=C:\Python27
  )
)

set EDK_SETUP_OPTION=
@rem if python is installed, disable the binary base tools.
if defined PYTHON_HOME (
  set EDK_TOOLS_BIN=
  set EDK_SETUP_OPTION=Rebuild
)
pushd %WORKSPACE%\edk2
call edksetup.bat %EDK_SETUP_OPTION%
popd

  if exist "ProgramFiles(x86)%\Windows Kits\8.1\bin" (
    echo "ProgramFiles(x86)\Windows Kits\8.1\bin present"
    echo "No need to override WINSDK81_PREFIX/WINSDK81x86_PREFIX"
  ) else (
   echo "ProgramFiles(x86)\Windows Kits\8.1\bin not present"
   echo "Need to override WINSDK81_PREFIX/WINSDK81x86_PREFIX with WINSDK10_PREFIX "
   echo WINSDK81_PREFIX defined "%WINSDK81_PREFIX%"
   echo WINSDK81x86_PREFIX "%WINSDK81x86_PREFIX%"
   if defined WINSDK10_PREFIX (
    echo WINSDK10_PREFIX defined 1 "%WINSDK10_PREFIX%"
    set "WINSDK81_PREFIX=%WINSDK10_PREFIX%"
    set "WINSDK81x86_PREFIX=%WINSDK10_PREFIX%"
   ) else (
   echo Build may fail when trying to find RC.exe
   goto :EOF
   )
  )

set openssl_path=%WORKSPACE%

popd

goto :EOF

:Help
echo.
echo Usage:
echo GitEdk2.bat [-w Workspace_Directory] (optional) [-b Branch_Name] (optional)
echo.
echo -w    A absolute/relative path to be the workspace.
echo       Default value is the current directory.
echo.
echo -b    The branch name of the repository. Currently, only master, udk2015,
echo       trunk (same as master) and bp13 (same as udk2015) are supported.
echo       Default value is master.
echo.
