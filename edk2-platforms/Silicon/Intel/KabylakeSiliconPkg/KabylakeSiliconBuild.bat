@REM @file
@REM
@REM Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
@REM SPDX-License-Identifier: BSD-2-Clause-Patent
@REM

@call edksetup.bat

@if not defined TOOL_CHAIN_TAG (
  if defined VS140COMNTOOLS (
    echo.
    echo Prebuild:  Set the VS2015 environment.
    echo.
    if not defined VSINSTALLDIR call "%VS140COMNTOOLS%\vsvars32.bat"
    if /I "%VS140COMNTOOLS%" == "C:\Program Files\Microsoft Visual Studio 14.0\Common7\Tools\" (
      set TOOL_CHAIN_TAG=VS2015
    ) else (
      set TOOL_CHAIN_TAG=VS2015x86
    )
  )
)

@if not defined TOOL_CHAIN_TAG (
  if defined VS120COMNTOOLS (
    echo.
    echo Prebuild:  Set the VS2013 environment.
    echo.
    if not defined VSINSTALLDIR call "%VS120COMNTOOLS%\vsvars32.bat"
    if /I "%VS120COMNTOOLS%" == "C:\Program Files\Microsoft Visual Studio 12.0\Common7\Tools\" (
      set TOOL_CHAIN_TAG=VS2013
    ) else (
      set TOOL_CHAIN_TAG=VS2013x86
    )
  )
)

set TARGET=RELEASE

@echo.
@echo Set build environment.
@echo.
@if not exist Build\KabylakeSiliconPkg\%TARGET%_%TOOL_CHAIN_TAG% (
  mkdir Build\KabylakeSiliconPkg\%TARGET%_%TOOL_CHAIN_TAG%
)


@findstr /V "ACTIVE_PLATFORM TARGET TARGET_ARCH TOOL_CHAIN_TAG BUILD_RULE_CONF" Conf\target.txt > Build\%PLATFORM_PACKAGE%\%TARGET%_%TOOL_CHAIN_TAG%\target.txt
@echo ACTIVE_PLATFORM = KabylakeSiliconPkg/KabylakeSiliconPkg.dsc        >> Build\KabylakeSiliconPkg\%TARGET%_%TOOL_CHAIN_TAG%\target.txt
@echo TARGET          = %TARGET%                                 >> Build\KabylakeSiliconPkg\%TARGET%_%TOOL_CHAIN_TAG%\target.txt
@echo TARGET_ARCH     = IA32 X64                                  >> Build\KabylakeSiliconPkg\%TARGET%_%TOOL_CHAIN_TAG%\target.txt
@echo TOOL_CHAIN_TAG  = %TOOL_CHAIN_TAG%                          >> Build\KabylakeSiliconPkg\%TARGET%_%TOOL_CHAIN_TAG%\target.txt
@echo BUILD_RULE_CONF = Conf/build_rule.txt                       >> Build\KabylakeSiliconPkg\%TARGET%_%TOOL_CHAIN_TAG%\target.txt
@move /Y Build\KabylakeSiliconPkg\%TARGET%_%TOOL_CHAIN_TAG%\target.txt Conf

build

