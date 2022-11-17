## @file
# Trim files preprocessed by compiler
#
# Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

##
# Import Modules
#
import os
import sys
import time
import logging
import struct
import datetime
import argparse
import platform
from collections import OrderedDict
try:
    from configparser import ConfigParser
except:
    from ConfigParser import ConfigParser

# Config message
_BIOS_Signature = "$IBIOSI$"

_ConfigItem = {
    "BOARD_ID": {'Value': '', 'Length': 7},
    "BOARD_REV": {'Value': '', 'Length': 1},
    "BOARD_EXT": {'Value': '', 'Length': 3},
    "BUILD_TYPE": {'Value': '', 'Length': 1},
    "VERSION_MAJOR": {'Value': '0000', 'Length': 4},
    "VERSION_MINOR": {'Value': '00', 'Length': 2},
}

# Version message
__prog__ = 'GenBiosld'
__description__ = 'Trim files preprocessed by compiler'
__copyright__ = 'Copyright (c) 2019, Intel Corporation. All rights reserved.<BR> '
__version__ = '%s Version %s' % (__prog__, '0.1 ')

# ExtraData message
_Usage = "Usage: GenBiosId -i Configfile -o OutputFile [-ot OutputTextFile]"
_ConfigSectionNotDefine = "Not support the config file format, need config section"
_ErrorMessageTemplate = '\n\n%(tool)s...\n : error: %(msg)s\n\t%(extra)s'
_ErrorLogger = logging.getLogger("tool_error")
_ErrorFormatter = logging.Formatter("%(message)s")
_ConfigLenInvalid = "Config item %s length is invalid"
_ConfigItemInvalid = "Item %s is invalid"

# Error message
INFO = 20
ERRORCODE = 50
OPTION_MISSING = 'Missing option'
FORMAT_INVALID = 'Invalid syntax/format'
FILE_NOT_FOUND = 'File/directory not found in workspace'
FORMAT_UNKNOWN_ERROR = 'Unknown error in syntax/format'
FORMAT_NOT_SUPPORTED = 'Not supported syntax/format'


def SetEdkLogger():
    _ErrorLogger.setLevel(INFO)
    _ErrorCh = logging.StreamHandler(sys.stderr)
    _ErrorCh.setFormatter(_ErrorFormatter)
    _ErrorLogger.addHandler(_ErrorCh)
    return _ErrorLogger


# Output the error message and exit the tool
def EdkLogger(ToolName, Message, ExtraData):
    _ErrorLogger = SetEdkLogger()
    TemplateDict = {"tool": ToolName, "msg": Message, "extra": ExtraData}
    LogText = _ErrorMessageTemplate % TemplateDict
    _ErrorLogger.log(ERRORCODE, LogText)
    sys.exit(1)


# Open the file in the correct way
def FileOpen(FileName, Mode, Buffer=-1):
    def LongFilePath(FileName):
        FileName = os.path.normpath(FileName)
        if platform.system() == 'Windows':
            if FileName.startswith('\\\\?\\'):
                return FileName
            if FileName.startswith('\\\\'):
                return '\\\\?\\UNC\\' + FileName[2:]
            if os.path.isabs(FileName):
                return '\\\\?\\' + FileName
        return FileName

    return open(LongFilePath(FileName), Mode, Buffer)


# Parse command line options
def MyOptionParser():
    parser = argparse.ArgumentParser(prog=__prog__,
                                     description=__description__ + __copyright__ + _Usage,
                                     conflict_handler='resolve')
    parser.add_argument('-v', '--version', action='version', version=__version__,
                        help="show program's version number and exit")
    parser.add_argument('-i', '--int', metavar='FILENAME', dest='InputFile', help="Input Config file")
    parser.add_argument('-o', '--out', metavar='FILENAME', dest='OutputFile', help="Output file")
    parser.add_argument('-ot', '--text', metavar='FILENAME', dest='OutputTextFile', help="Output Text file")
    parser.add_argument('-nt', '--notimestamp', dest='NoTimestamp', action='store_true', default=False, help="Set timestamp to zero")
    Options = parser.parse_args()
    return Options


# Check the Tool for missing variables
def CheckOptions(Options):
    if len(sys.argv) not in [5,6] and not (len(sys.argv) in [7,8] and Options.OutputTextFile):
        EdkLogger("GenBiosId", OPTION_MISSING, ExtraData=_Usage)
    elif not Options.InputFile or not Options.OutputFile:
        EdkLogger("GenBiosId", OPTION_MISSING, ExtraData=_Usage)
    InputFile = Options.InputFile
    OutputFile = Options.OutputFile
    OutputTextFile = Options.OutputTextFile
    NoTimestamp = Options.NoTimestamp
    if not os.path.exists(InputFile):
        EdkLogger("GenBiosId", FILE_NOT_FOUND, ExtraData="Input file not found")
    return InputFile, OutputFile, OutputTextFile, NoTimestamp

# Read input file and get config
def ReadInputFile(InputFile):
    InputDict = OrderedDict()
    with open(InputFile) as File:
        FileLines = File.readlines()
    for Line in FileLines:
        if Line.strip().startswith('#'):
            continue
        if '=' in Line:
            Key, Value = Line.split('=')
            InputDict[Key.strip()] = Value.strip()
    return InputDict


# Parse the input file and extract the information
def ParserInputFile(InputDict, NoTimestamp):
    for Item in InputDict:
        if Item not in _ConfigItem:
            EdkLogger("GenBiosId", FORMAT_INVALID, ExtraData=_ConfigItemInvalid % Item)
        _ConfigItem[Item]['Value'] = InputDict[Item]
        if len(_ConfigItem[Item]['Value']) != _ConfigItem[Item]['Length']:
            EdkLogger("GenBiosId", FORMAT_INVALID, ExtraData=_ConfigLenInvalid % Item)
    for Item in _ConfigItem:
        if not _ConfigItem[Item]['Value']:
            EdkLogger("GenBiosId", FORMAT_UNKNOWN_ERROR, ExtraData="Item %s is missing" % Item)
    utcnow = datetime.datetime.utcnow()
    if NoTimestamp:
        TimeStamp = "\0\0\0\0\0\0\0\0\0\0"
    else:
        TimeStamp = time.strftime("%y%m%d%H%M", utcnow.timetuple())
    Id_Str = _ConfigItem['BOARD_ID']['Value'] + _ConfigItem['BOARD_REV']['Value'] + '.' + _ConfigItem['BOARD_EXT'][
        'Value'] + '.' + _ConfigItem['VERSION_MAJOR']['Value'] + \
             '.' + _ConfigItem["BUILD_TYPE"]['Value'] + _ConfigItem['VERSION_MINOR']['Value'] + '.' + TimeStamp
    return Id_Str


# Output information to a file
def PrintOutputFile(OutputFile, OutputTextFile, Id_Str):
    with FileOpen(OutputFile, 'wb') as FdOut:
        for i in _BIOS_Signature:
            FdOut.write(struct.pack('B', ord(i)))

        for i in Id_Str:
            FdOut.write(struct.pack('H', ord(i)))

        FdOut.write(struct.pack('H', 0x00))
    if OutputTextFile:
        with FileOpen(OutputTextFile, 'w') as FdOut:
            FdOut.write(Id_Str)


# Tool entrance method
def Main():
    Options = MyOptionParser()
    InputFile, OutputFile, OutputTextFile, NoTimestamp = CheckOptions(Options)
    InputDict = ReadInputFile(InputFile)
    Id_Str = ParserInputFile(InputDict, NoTimestamp)
    PrintOutputFile(OutputFile, OutputTextFile, Id_Str)
    return 0


if __name__ == '__main__':
    r = Main()
    ## 0-127 is a safe return range, and 1 is a standard default error
    if r < 0 or r > 127: r = 1
    sys.exit(r)
