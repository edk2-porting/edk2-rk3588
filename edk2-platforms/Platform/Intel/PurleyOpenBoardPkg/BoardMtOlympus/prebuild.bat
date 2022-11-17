@REM @file
@REM
@REM Copyright (c) 2018 - 2021, Intel Corporation. All rights reserved.<BR>
@REM SPDX-License-Identifier: BSD-2-Clause-Patent
@REM

@set SCRIPT_ERROR=0

set /a prebuildstep=0

call :check_BuildTools
if %SCRIPT_ERROR% NEQ 0 GOTO :done

call :setBuildEnv
if %SCRIPT_ERROR% NEQ 0 GOTO :done

call :createTargetTxt
if %SCRIPT_ERROR% NEQ 0 GOTO :done

call :genPlatformOffsetHeaderFile
if %SCRIPT_ERROR% NEQ 0 GOTO :done

:prebuildFinish
echo.
echo   ACTIVE_PLATFORM              = %WORKSPACE%\edk2-platforms\Platform\Intel\%BOARD_PKG%\%BOARD_NAME%\OpenBoardPkg.dsc
echo   EDK_TOOLS_PATH               = %EDK_TOOLS_PATH%
echo   TARGET                       = %TARGET%
echo   TARGET_ARCH                  = IA32 X64
echo   TOOL_CHAIN_TAG               = %TOOL_CHAIN_TAG%
echo   WORKSPACE                    = %WORKSPACE%
echo   PACKAGES_PATH                = %PACKAGES_PATH%
echo   MAX_CONCURRENT_THREAD_NUMBER = %BUILD_MAX_CON_THREAD_NUM%
echo.
echo   Build Path                   = %OUTPUT_DIR%
echo.

REM Remove environment variable because it's no longer needed.
set BUILD_MAX_CON_THREAD_NUM=

:done
REM Use done label to exit batch file and run any final steps; GOTO :EOF immediately exits.
EXIT /B %SCRIPT_ERROR%

::--------------------------------------------------------
::-- Function section starts below here
::--------------------------------------------------------

:cleanup_check_VSTools
set COMPILER_VERSION_STRING=
del cloutput.txt > nul
REM cleanup_check_VSTools is called below. When a label is called, 'GOTO :EOF' is used to return to caller.
GOTO :EOF

:check_BuildTools
echo PreBuild.%prebuildstep% check_BuildTools
echo ..VSTools
set /a prebuildstep=%prebuildstep%+1
set TOOL_CHAIN_TAG=
@if not defined TOOL_CHAIN_TAG (
  echo.
  echo Prebuild:  TOOL_CHAIN_TAG is not set before
  echo.

  @if defined VS140COMNTOOLS (
    echo.
    echo Set the VS2015 environment.
    echo.
    set CL_SEL=VS2015
    if /I "%VS140COMNTOOLS%" == "C:\Program Files\Microsoft Visual Studio 14.0\Common7\Tools\" (
      set TOOL_CHAIN_TAG=VS2015
    ) else (
      set TOOL_CHAIN_TAG=VS2015x86
    )
    if /I "%PROCESSOR_ARCHITECTURE%" == "AMD64" (
      set CL_CMDLINE="%VS140COMNTOOLS:~0,-14%VC\bin\amd64\cl.exe"
    ) else (
      set CL_CMDLINE="%VS140COMNTOOLS:~0,-14%VC\bin\cl.exe"
    )
  ) else if defined VS120COMNTOOLS (
    echo.
    echo Set the VS2013 environment.
    echo.
    set CL_SEL=VS2013
    if /I "%VS120COMNTOOLS%" == "C:\Program Files\Microsoft Visual Studio 12.0\Common7\Tools\" (
      set TOOL_CHAIN_TAG=VS2013
    ) else (
      set TOOL_CHAIN_TAG=VS2013x86
    )
    if /I "%PROCESSOR_ARCHITECTURE%" == "AMD64" (
      set CL_CMDLINE="%VS120COMNTOOLS:~0,-14%VC\bin\amd64\cl.exe"
    ) else (
      set CL_CMDLINE="%VS120COMNTOOLS:~0,-14%VC\bin\cl.exe"
    )
  ) else (
    echo.
    echo !!! ERROR !!! VS2015 or VS2013 not installed correctly. !!!
    echo.
    goto :ErrorExit
  )
)

echo ..iASL
set CHECK_PATH_IASL=%IASL_PREFIX%
if not exist %CHECK_PATH_IASL%\iasl.exe (
  echo.
  echo !!! ERROR !!! Could not find iASL compiler at %CHECK_PATH_IASL%\iasl.exe. !!!
  echo.
  set SCRIPT_ERROR=1
)
set CHECK_PATH_IASL=

echo ..NASM
set CHECK_PATH_NASM=c:\NASM
if not exist %CHECK_PATH_NASM%\nasm.exe (
  echo.
  echo !!! ERROR !!! Could not find NASM compiler at %CHECK_PATH_NASM%\nasm.exe. !!!
  echo.
  set SCRIPT_ERROR=1
)
set CHECK_PATH_NASM=

echo ..Python
set CHECK_PATH_PYTHON=c:\Python27
if not exist %CHECK_PATH_PYTHON%\python.exe (
  echo.
  echo !!! ERROR !!! Could not find Python at %CHECK_PATH_PYTHON%\python.exe. !!!
  echo.
  set SCRIPT_ERROR=1
)
set CHECK_PATH_PYTHON=
set PYTHON_HOME=C:\Python27

GOTO :EOF

:setBuildEnv
echo PreBuild.%prebuildstep% SetBuildEnv
set /a prebuildstep=%prebuildstep%+1

@set BOARD_PKG=PurleyOpenBoardPkg
@set BOARD_NAME=BoardMtOlympus
@set MAX_SOCKET=2

echo.
echo BOARD_NAME=%BOARD_NAME%
echo BOARD_PKG=%BOARD_PKG%
echo MAX_SOCKET=%MAX_SOCKET%
echo TARGET=%TARGET%

@set OUTPUT_DIR=%WORKSPACE%\Build\%BOARD_PKG%\%BOARD_NAME%\%TARGET%_%TOOL_CHAIN_TAG%

if not exist %OUTPUT_DIR% mkdir %OUTPUT_DIR%
GOTO :EOF

:createTargetTxt
echo PreBuild.%prebuildstep% CreateTargetTxt
set /a prebuildstep=%prebuildstep%+1
set /a BUILD_MAX_CON_THREAD_NUM = %NUMBER_OF_PROCESSORS%-1
@REM set /a BUILD_MAX_CON_THREAD_NUM = 1
findstr /V "ACTIVE_PLATFORM TARGET TARGET_ARCH TOOL_CHAIN_TAG BUILD_RULE_CONF MAX_CONCURRENT_THREAD_NUMBER" %WORKSPACE%\Conf\target.txt > %OUTPUT_DIR%\target.txt 2>NUL
echo ACTIVE_PLATFORM             = %WORKSPACE%/edk2-platforms/Platform/Intel/%BOARD_PKG%/%BOARD_NAME%/OpenBoardPkg.dsc >> %OUTPUT_DIR%\target.txt
echo TARGET                      = %TARGET%                       >> %OUTPUT_DIR%\target.txt
echo TARGET_ARCH                 = IA32 X64                       >> %OUTPUT_DIR%\target.txt
echo TOOL_CHAIN_TAG              = %TOOL_CHAIN_TAG%               >> %OUTPUT_DIR%\target.txt
echo BUILD_RULE_CONF             = Conf/build_rule.txt            >> %OUTPUT_DIR%\target.txt
echo MAX_CONCURRENT_THREAD_NUMBER = %BUILD_MAX_CON_THREAD_NUM%    >> %OUTPUT_DIR%\target.txt
if exist %WORKSPACE%\Conf\target.txt (
  del /f %WORKSPACE%\Conf\target.txt
)
move /Y %OUTPUT_DIR%\target.txt %WORKSPACE%\Conf\ > nul
if not exist %OUTPUT_DIR%\X64 mkdir %OUTPUT_DIR%\X64
GOTO :EOF


:genPlatformOffsetHeaderFile
echo.
echo PreBuild.%prebuildstep% GenPlatformOffsetHeaderFile
set /a prebuildstep=%prebuildstep%+1

echo Info: re-generating PlatformOffset header files

set PRE_BUILD_CMD_LINE=%BUILD_CMD_LINE% -D MAX_SOCKET=%MAX_SOCKET%
set PRE_BUILD_LOG=%WORKSPACE%\Build\prebuild.log
set PRE_BUILD_REPORT=%WORKSPACE%\Build\preBuildReport.txt

echo build %PRE_BUILD_CMD_LINE% -m %BOARD_PKG%\Acpi\BoardAcpiDxe\Dsdt.inf -y %PRE_BUILD_REPORT% --log=%PRE_BUILD_LOG%
call build %PRE_BUILD_CMD_LINE% -m %BOARD_PKG%\Acpi\BoardAcpiDxe\Dsdt.inf -y %PRE_BUILD_REPORT% --log=%PRE_BUILD_LOG%
if %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%

@REM PSYS == FIX0
@REM MCTL == FIX8
set AML_FILTER="\"PSYS\" .MCTL\" .FIX[0-9,A-Z]\""
echo AML_FILTER=%AML_FILTER%
call %PYTHON_HOME%\python.exe %WORKSPACE%\edk2-platforms\Platform\Intel\MinPlatformPkg\Tools\AmlGenOffset\AmlGenOffset.py -d --aml_filter %AML_FILTER% -o %WORKSPACE%\edk2-platforms\Platform\Intel\%BOARD_PKG%\Acpi\BoardAcpiDxe\AmlOffsetTable.c %OUTPUT_DIR%\X64\PurleyOpenBoardPkg\Acpi\BoardAcpiDxe\DSDT\OUTPUT\Dsdt\WFPPlatform.offset.h
echo.
echo GenOffset done

GOTO :EOF