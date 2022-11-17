@REM @file
@REM
@REM Copyright (c) 2018 - 2021, Intel Corporation. All rights reserved.<BR>
@REM SPDX-License-Identifier: BSD-2-Clause-Patent
@REM

@echo off

REM Run setlocal to take a snapshot of the environment variables.  endlocal is called to restore the environment.
setlocal
set SCRIPT_ERROR=0

REM ---- Do NOT use :: for comments Inside of code blocks() ----

::**********************************************************************
:: Initial Setup
::**********************************************************************

:parseCmdLine
if "%1"=="" goto :argumentCheck

if /I "%1"=="debug"          set TARGET=DEBUG
if /I "%1"=="release"        set TARGET=RELEASE

if /I "%1"=="clean" (
  set BUILD_TYPE=cleantree
  call :cleantree
  goto :EOF
)

shift
GOTO :parseCmdLine

:argumentCheck:

if /I "%TARGET%" == "" (
  echo Info: debug/release argument is empty, use DEBUG as default
  set TARGET=DEBUG
)

REM Art to notify which board you're working on
echo.
type logo.txt
echo.

::
:: Build configuration
::
set BUILD_REPORT_FLAGS=
set BUILD_CMD_LINE=
set BUILD_LOG=%WORKSPACE%\Build\build.log
set BUILD_REPORT=%WORKSPACE%\Build\BuildReport.txt

del %BUILD_LOG% *.efi *.log 2>NUL

echo --------------------------------------------------------------------------------------------
echo.
echo                                Purley Build Start
echo.
echo --------------------------------------------------------------------------------------------


:doPreBuild
echo.
echo --------------------------------------------------------------------
echo.
echo                          Prebuild Start
echo.
echo --------------------------------------------------------------------
call prebuild.bat
if %SCRIPT_ERROR% NEQ 0 EXIT /b %ERRORLEVEL%

echo --------------------------------------------------------------------
echo.
echo                          Prebuild End
echo.
echo --------------------------------------------------------------------
if %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
timeout 1

:buildBios
set BUILD_CMD_LINE=%BUILD_CMD_LINE% -D MAX_SOCKET=%MAX_SOCKET% -y %BUILD_REPORT%
echo --------------------------------------------------------------------
echo.
echo                          Build Start
echo.
echo --------------------------------------------------------------------
echo.
echo build %BUILD_CMD_LINE% --log=%BUILD_LOG% %BUILD_REPORT_FLAGS%
call build %BUILD_CMD_LINE% --log=%BUILD_LOG% %BUILD_REPORT_FLAGS%
echo --------------------------------------------------------------------
echo.
echo                          Build End
echo.
echo --------------------------------------------------------------------
if %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
timeout 1

:postBuild

echo --------------------------------------------------------------------
echo.
echo                          PostBuild Start
echo.
echo --------------------------------------------------------------------
echo.
call postbuild.bat
if %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
timeout 1
echo --------------------------------------------------------------------
echo.
echo                          PostBuild End
echo.
echo --------------------------------------------------------------------

echo %date%  %time%
echo.

echo --------------------------------------------------------------------------------------------
echo.
echo                                Purley Build End
echo.
echo --------------------------------------------------------------------------------------------

:done
endlocal & EXIT /b %SCRIPT_ERROR%

::--------------------------------------------------------
::-- Function section starts below here
::--------------------------------------------------------
:cleantree
choice /t 3 /d y /m "Confirm: clean tree of intermediate files created in tree during build"
if %ERRORLEVEL% EQU 2 goto :EOF
goto :EOF


:ErrorHandler:
echo Error handler