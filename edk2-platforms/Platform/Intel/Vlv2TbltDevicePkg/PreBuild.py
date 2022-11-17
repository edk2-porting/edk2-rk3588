## @file
# PreBuild operations for Vlv2TbltDevicePkg
#
# Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

'''
PreBuild
'''

import os
import sys
import argparse
import subprocess
import glob
import shutil
import struct
import datetime

#
# Globals for help information
#
__prog__        = 'PreBuild'
__copyright__   = 'Copyright (c) 2019, Intel Corporation. All rights reserved.'
__description__ = 'Vlv2Tbl2DevicePkg pre-build operations.\n'

#
# Globals
#
gWorkspace = ''
gBaseToolsPath = ''
gArgs      = None

def LogAlways(Message):
    sys.stdout.write (__prog__ + ': ' + Message + '\n')
    sys.stdout.flush()

def Log(Message):
    global gArgs
    if not gArgs.Verbose:
        return
    sys.stdout.write (__prog__ + ': ' + Message + '\n')
    sys.stdout.flush()

def Error(Message, ExitValue=1):
    sys.stderr.write (__prog__ + ': ERROR: ' + Message + '\n')
    sys.exit (ExitValue)

def RelativePath(target):
    global gWorkspace
    Log('RelativePath' + target)
    return os.path.relpath (target, gWorkspace)

def NormalizePath(target):
    if isinstance(target, tuple):
        return os.path.normpath (os.path.join (*target))
    else:
        return os.path.normpath (target)

def RemoveFile(target):
    target = NormalizePath(target)
    if not target or target == os.pathsep:
        Error ('RemoveFile() invalid target')
    if os.path.exists(target):
        os.remove (target)
        Log ('remove %s' % (RelativePath (target)))

def RemoveDirectory(target):
    target = NormalizePath(target)
    if not target or target == os.pathsep:
        Error ('RemoveDirectory() invalid target')
    if os.path.exists(target):
        Log ('rmdir %s' % (RelativePath (target)))
        shutil.rmtree(target)

def CreateDirectory(target):
    target = NormalizePath(target)
    if not os.path.exists(target):
        Log ('mkdir %s' % (RelativePath (target)))
        os.makedirs (target)

def Copy(src, dst):
    src = NormalizePath(src)
    dst = NormalizePath(dst)
    for File in glob.glob(src):
        Log ('copy %s -> %s' % (RelativePath (File), RelativePath (dst)))
        shutil.copy (File, dst)

def GenCapsuleDevice (BaseName, PayloadFileName, Guid, Version, Lsv, CapsulesPath, CapsulesSubDir):
    global gBaseToolsPath
    LogAlways ('Generate Capsule: {0} {1:08x} {2:08x} {3}'.format (Guid, Version, Lsv, PayloadFileName))

    VersionString = '.'.join([str(ord(x)) for x in struct.pack('>I', Version).decode()])

    FmpCapsuleFile = NormalizePath ((CapsulesPath, CapsulesSubDir, BaseName + '.' + VersionString + '.cap'))
    Command = GenerateCapsuleCommand.format (
                FMP_CAPSULE_GUID    = Guid,
                FMP_CAPSULE_VERSION = Version,
                FMP_CAPSULE_LSV     = Lsv,
                BASE_TOOLS_PATH     = gBaseToolsPath,
                FMP_CAPSULE_FILE    = FmpCapsuleFile,
                FMP_CAPSULE_PAYLOAD = PayloadFileName
                )
    Command = ' '.join(Command.splitlines()).strip()
    if gArgs.Verbose:
        Command = Command + ' -v'

    Log (Command)

    Process = subprocess.Popen(Command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    ProcessOutput = Process.communicate()

    if Process.returncode == 0:
        Log (ProcessOutput[0].decode())
    else:
        LogAlways (Command)
        LogAlways (ProcessOutput[0].decode())
        Error ('GenerateCapsule returned an error')

    Copy (PayloadFileName, (CapsulesPath, 'firmware.bin'))
    MetaInfoXml = MetaInfoXmlTemplate
    MetaInfoXml = MetaInfoXml.replace ('FMP_CAPSULE_GUID', Guid)
    MetaInfoXml = MetaInfoXml.replace ('FMP_CAPSULE_BASE_NAME', BaseName)
    MetaInfoXml = MetaInfoXml.replace ('FMP_CAPSULE_VERSION_DECIMAL', str(Version))
    MetaInfoXml = MetaInfoXml.replace ('FMP_CAPSULE_STRING', VersionString)
    MetaInfoXml = MetaInfoXml.replace ('FMP_CAPSULE_DATE', str(datetime.date.today()))
    f = open (NormalizePath ((CapsulesPath, 'firmware.metainfo.xml')), 'w')
    f.write(MetaInfoXml)
    f.close()

    f = open (NormalizePath ((CapsulesPath, 'Lvfs.ddf')), 'w')
    f.write(LvfsDdfTemplate)
    f.close()

    if sys.platform == "win32":
        Command = 'makecab /f ' + NormalizePath ((CapsulesPath, 'Lvfs.ddf'))
    else:
        Command = 'gcab --create firmware.cab firmware.bin firmware.metainfo.xml'
    Log (Command)

    Process = subprocess.Popen(Command, cwd=CapsulesPath, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    ProcessOutput = Process.communicate()

    if Process.returncode == 0:
        Log (ProcessOutput[0].decode())
    else:
        LogAlways (Command)
        LogAlways (ProcessOutput[0].decode())
        Error ('GenerateCapsule returned an error')

    FmpCabinetFile = NormalizePath ((CapsulesPath, CapsulesSubDir, BaseName + '.' + VersionString + '.cab'))

    Copy ((CapsulesPath, 'firmware.cab'), FmpCabinetFile)

    RemoveFile ((CapsulesPath, 'firmware.cab'))
    RemoveFile ((CapsulesPath, 'setup.inf'))
    RemoveFile ((CapsulesPath, 'setup.rpt'))
    RemoveFile ((CapsulesPath, 'Lvfs.ddf'))
    RemoveFile ((CapsulesPath, 'firmware.metainfo.xml'))
    RemoveFile ((CapsulesPath, 'firmware.bin'))

BiosIdTemplate = '''
BOARD_ID       = MNW2MAX
BOARD_REV      = $BOARD_REV
BOARD_EXT      = $ARCH
VERSION_MAJOR  = 0090
BUILD_TYPE     = $BUILD_TYPE
VERSION_MINOR  = 01
'''


if __name__ == '__main__':
    #
    # Create command line argument parser object
    #
    parser = argparse.ArgumentParser (
                        prog = __prog__,
                        description = __description__ + __copyright__,
                        conflict_handler = 'resolve'
                        )
    parser.add_argument (
             '-a', '--arch', dest = 'Arch', nargs = '+', action = 'append',
             required = True,
             help = '''ARCHS is one of list: IA32, X64, IPF, ARM, AARCH64 or EBC,
                       which overrides target.txt's TARGET_ARCH definition. To
                       specify more archs, please repeat this option.'''
             )
    parser.add_argument (
             '-t', '--tagname', dest = 'ToolChain', required = True,
             help = '''Using the Tool Chain Tagname to build the platform,
                       overriding target.txt's TOOL_CHAIN_TAG definition.'''
             )
    parser.add_argument (
             '-p', '--platform', dest = 'PlatformFile', required = True,
             help = '''Build the platform specified by the DSC file name argument,
                       overriding target.txt's ACTIVE_PLATFORM definition.'''
             )
    parser.add_argument (
             '-b', '--buildtarget', dest = 'BuildTarget', required = True,
             help = '''Using the TARGET to build the platform, overriding
                       target.txt's TARGET definition.'''
             )
    parser.add_argument (
             '--conf=', dest = 'ConfDirectory', required = True,
             help = '''Specify the customized Conf directory.'''
             )
    parser.add_argument (
             '-D', '--define', dest = 'Define', nargs='*', action = 'append',
             help = '''Macro: "Name [= Value]".'''
             )
    parser.add_argument (
             '-v', '--verbose', dest = 'Verbose', action = 'store_true',
             help = '''Turn on verbose output with informational messages printed'''
             )
    parser.add_argument (
             '--package', dest = 'Package', nargs = '*', action = 'append',
             help = '''The directory name of a package of tests to copy'''
             )

    #
    # Parse command line arguments
    #
    gArgs, remaining = parser.parse_known_args()
    gArgs.BuildType = 'all'
    for BuildType in ['all', 'fds', 'genc', 'genmake', 'clean', 'cleanall', 'modules', 'libraries', 'run']:
        if BuildType in remaining:
            gArgs.BuildType = BuildType
            remaining.remove(BuildType)
            break
    gArgs.Remaining = ' '.join(remaining)

    #
    # Get WORKSPACE environment variable
    #
    try:
        gWorkspace = os.environ['WORKSPACE']
    except:
        Error ('WORKSPACE environment variable not set')

    #
    # Get PACKAGES_PATH and generate prioritized list of paths
    #
    PathList = [gWorkspace]
    try:
        PathList += os.environ['PACKAGES_PATH'].split(os.pathsep)
    except:
        pass

    #
    # Determine full path to BaseTools
    #
    Vlv2Tbl2DevicePkgPath = ''
    for Path in PathList:
        if gBaseToolsPath == '':
            if os.path.exists (os.path.join (Path, 'BaseTools')):
                gBaseToolsPath = os.path.join (Path, 'BaseTools')
        if Vlv2Tbl2DevicePkgPath == '':
            if os.path.exists (os.path.join (Path, 'Vlv2TbltDevicePkg')):
                Vlv2Tbl2DevicePkgPath = os.path.join (Path, 'Vlv2TbltDevicePkg')
    if gBaseToolsPath == '':
        Error ('Can not find BaseTools in WORKSPACE or PACKAGES_PATH')
    if Vlv2Tbl2DevicePkgPath == '':
        Error ('Can not find Vlv2Tbl2DevicePkg in WORKSPACE or PACKAGES_PATH')

    #
    # Parse OUTPUT_DIRECTORY from DSC file
    #
    for Path in PathList:
        if os.path.exists (os.path.join (Path, gArgs.PlatformFile)):
            Dsc = open (os.path.join (Path, gArgs.PlatformFile), 'r').readlines()
            break
    for Line in Dsc:
        if Line.strip().startswith('OUTPUT_DIRECTORY'):
            OutputDirectory = Line.strip().split('=')[1].strip()
            break

    #
    # Determine full paths to EDK II build directory, EDK II build output
    # directory and the CPU arch of the UEFI phase.
    #
    CommandDir = os.path.dirname(sys.argv[0])
    EdkiiBuildDir = os.path.join (gWorkspace, OutputDirectory)
    EdkiiBuildOutput = os.path.join (EdkiiBuildDir, gArgs.BuildTarget + '_' + gArgs.ToolChain)
    UefiArch = gArgs.Arch[0][0]
    if len (gArgs.Arch) > 1:
        if ['X64'] in gArgs.Arch:
            UefiArch = 'X64'

    if gArgs.BuildType == 'run':
        Error ("'run' target not supported")
    if gArgs.BuildType == 'clean':
        sys.exit (0)

    #
    # Create output directories to put BiosId files
    #
    try:
        CreateDirectory ((gWorkspace, 'Build'))
    except:
        pass
    try:
        CreateDirectory ((EdkiiBuildDir))
    except:
        pass
    try:
        CreateDirectory ((EdkiiBuildOutput))
    except:
        pass

    #
    # Generate BiosId files
    #
    BiosId = BiosIdTemplate
    if sys.platform == "win32":
        # Built from a Windows Host OS
        BiosId = BiosId.replace ('$BOARD_REV', 'W')
    else:
        # Built from a Linux/Unix/Mac Host OS
        BiosId = BiosId.replace ('$BOARD_REV', 'L')
    if UefiArch == 'X64':
      BiosId = BiosId.replace ('$ARCH', 'X64')
    else:
      BiosId = BiosId.replace ('$ARCH', 'I32')
    BiosId = BiosId.replace ('$BUILD_TYPE', gArgs.BuildTarget[0])

    BiosIdFileName = NormalizePath ((EdkiiBuildOutput, 'BiosId.env'))
    f = open (BiosIdFileName, 'w')
    f.write(BiosId)
    f.close()

    Command = 'python ' + NormalizePath ((Vlv2Tbl2DevicePkgPath, '../Tools/GenBiosId/GenBiosId.py'))
    Command = Command + ' -i '  + BiosIdFileName
    Command = Command + ' -o '  + NormalizePath ((EdkiiBuildOutput, 'BiosId.bin'))
    Command = Command + ' -ot ' + NormalizePath ((EdkiiBuildOutput, 'BiosId.txt'))

    LogAlways (Command)

    Process = subprocess.Popen(Command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    ProcessOutput = Process.communicate()

    if Process.returncode == 0:
        Log (ProcessOutput[0].decode())
    else:
        LogAlways (Command)
        LogAlways (ProcessOutput[0].decode())
        Error ('GenBiosId returned an error')
