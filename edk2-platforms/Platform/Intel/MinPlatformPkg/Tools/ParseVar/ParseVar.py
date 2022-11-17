## @file
#
# Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

'''
ParseVar
'''

import os
import sys
import argparse
import subprocess
import uuid
import struct
import collections
import binascii
from   ctypes import *

#
# Globals for help information
#
__prog__      = 'ParseVar'
__version__   = '%s Version %s' % (__prog__, '0.1 ')
__copyright__ = 'Copyright (c) 2017, Intel Corporation. All rights reserved.'
__usage__     = '%s -e|-d [options] <input_file>' % (__prog__)

class GUID(Structure):
    _fields_ = [
        ('Guid1',            c_uint32),
        ('Guid2',            c_uint16),
        ('Guid3',            c_uint16),
        ('Guid4',            ARRAY(c_uint8, 8)),
        ]

class TIME(Structure):
    _fields_ = [
        ('Year',             c_uint16),
        ('Month',            c_uint8),
        ('Day',              c_uint8),
        ('Hour',             c_uint8),
        ('Minute',           c_uint8),
        ('Second',           c_uint8),
        ('Pad1',             c_uint8),
        ('Nanosecond',       c_uint32),
        ('TimeZone',         c_uint16),
        ('Daylight',         c_uint8),
        ('Pad2',             c_uint8),
        ]

EFI_VARIABLE_GUID = [0xddcf3616, 0x3275, 0x4164, 0x98, 0xb6, 0xfe, 0x85, 0x70, 0x7f, 0xfe, 0x7d]
EFI_AUTHENTICATED_VARIABLE_GUID = [0xaaf32c78, 0x947b, 0x439a, 0xa1, 0x80, 0x2e, 0x14, 0x4e, 0xc3, 0x77, 0x92]

# Variable Store Header Format.
VARIABLE_STORE_FORMATTED  =        0x5a
# Variable Store Header State.
VARIABLE_STORE_HEALTHY    =        0xfe

class VARIABLE_STORE_HEADER(Structure):
    _fields_ = [
        ('Signature',                GUID),
        ('Size',                     c_uint32),
        ('Format',                   c_uint8),
        ('State',                    c_uint8),
        ('Reserved',                 c_uint16),
        ('Reserved1',                c_uint32),
        ]

# Variable data start flag.
VARIABLE_DATA             =        0x55AA

# Variable State flags.
VAR_IN_DELETED_TRANSITION =    0xfe
VAR_DELETED               =    0xfd
VAR_HEADER_VALID_ONLY     =    0x7f
VAR_ADDED                 =    0x3f

class VARIABLE_HEADER(Structure):
    _fields_ = [
        ('StartId',                  c_uint16),
        ('State',                    c_uint8),
        ('Reserved',                 c_uint8),
        ('Attributes',               c_uint32),
        ('NameSize',                 c_uint32),
        ('DataSize',                 c_uint32),
        ('VendorGuid',               GUID),
        ]

class AUTHENTICATED_VARIABLE_HEADER(Structure):
    _fields_ = [
        ('StartId',                  c_uint16),
        ('State',                    c_uint8),
        ('Reserved',                 c_uint8),
        ('Attributes',               c_uint32),
        ('MonotonicCount',           c_uint64),
        ('TimeStamp',                TIME),
        ('PubKeyIndex',              c_uint32),
        ('NameSize',                 c_uint32),
        ('DataSize',                 c_uint32),
        ('VendorGuid',               GUID),
        ]

# Alignment of Variable Data Header in Variable Store region.
HEADER_ALIGNMENT = 4

class DEFAULT_INFO(Structure):
    _fields_ = [
        ('DefaultId',                c_uint16),
        ('BoardId',                  c_uint8),
        ('Reserved',                 c_uint8),
        ]

class DEFAULT_DATA(Structure):
    _fields_ = [
        ('HeaderSize',               c_uint16),
        ('DefaultInfo',              DEFAULT_INFO),
        ]


def DumpHexData(Data):
  CharPerLine = 8
  DataLen = len(Data)
  Count = DataLen / CharPerLine
  Rem   = DataLen % CharPerLine
  CountIndex = 0
  for CountIndex in range (0, Count) :
    TempData = Data[CountIndex * CharPerLine:CountIndex * CharPerLine + CharPerLine]
    print "#//    %04x: "%(CountIndex*CharPerLine) + binascii.hexlify(TempData)
  if Rem != 0 :
    TempData = Data[Count * CharPerLine:Count * CharPerLine + Rem]
    print "#//    %04x: "%(Count*CharPerLine) + binascii.hexlify(TempData)

def DumpBin(Data):
  DataLen = len(Data)
  for Index in range (0, DataLen - 1) :
    print "0x%02x,"%(Data[Index]),
  print "0x%02x"%(Data[DataLen - 1]),

def DumpName(Data):
  DataLen = len(Data)
  for Index in range (0, DataLen/2 - 1) :
    print "%c"%(Data[Index * 2]),


if __name__ == '__main__':
  #
  # Create command line argument parser object
  #
  parser = argparse.ArgumentParser(prog=__prog__, version=__version__, usage=__usage__, description=__copyright__, conflict_handler='resolve')
  group = parser.add_mutually_exclusive_group(required=True)
  group.add_argument("-e", action="store_true", dest='Encode', help='encode file')
  group.add_argument("-d", action="store_true", dest='Decode', help='decode file')
  parser.add_argument("-v", "--verbose", dest='Verbose', action="store_true", help="increase output messages")
  parser.add_argument("-q", "--quiet", dest='Quiet', action="store_true", help="reduce output messages")
  parser.add_argument("--debug", dest='Debug', type=int, metavar='[0-9]', choices=range(0,10), default=0, help="set debug level")
  parser.add_argument(metavar="input_file", dest='InputFile', type=argparse.FileType('rb'), help="specify the input filename")

  #
  # Parse command line arguments
  #
  args = parser.parse_args()

  #
  # Read input file into a buffer and save input filename
  #
  args.InputFileName   = args.InputFile.name
  args.InputFileBuffer = args.InputFile.read()
  args.InputFile.close()

  if args.Encode:
    print 'Unsupported'

  if args.Decode:
    print '#//FCE binary'

    FullInputFileBuffer = bytearray(args.InputFileBuffer)
    FullSize = len(FullInputFileBuffer)

    DefaultData = DEFAULT_DATA.from_buffer (bytearray(args.InputFileBuffer), 0)
    print "#//  DEFAULT_DATA:"
    print "#//    HeaderSize - 0x%04x" % DefaultData.HeaderSize
    print "#//    DefaultId  - 0x%04x" % DefaultData.DefaultInfo.DefaultId
    print "#//    BoardId    - 0x%02x" % DefaultData.DefaultInfo.BoardId
    print ""

    Offset = DefaultData.HeaderSize
    VariableStoreHeader = VARIABLE_STORE_HEADER.from_buffer (bytearray(args.InputFileBuffer), Offset)
    print "#//  VARIABLE_STORE_HEADER:"
    print "#//    Signature  - %08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x" % (VariableStoreHeader.Signature.Guid1, VariableStoreHeader.Signature.Guid2, VariableStoreHeader.Signature.Guid3, VariableStoreHeader.Signature.Guid4[0], VariableStoreHeader.Signature.Guid4[1], VariableStoreHeader.Signature.Guid4[2], VariableStoreHeader.Signature.Guid4[3], VariableStoreHeader.Signature.Guid4[4], VariableStoreHeader.Signature.Guid4[5], VariableStoreHeader.Signature.Guid4[6], VariableStoreHeader.Signature.Guid4[7])
    print "#//    Size       - 0x%08x" % VariableStoreHeader.Size
    print "#//    Format     - 0x%02x" % VariableStoreHeader.Format
    print "#//    State      - 0x%02x" % VariableStoreHeader.State
    print ""
    Offset += sizeof(VARIABLE_STORE_HEADER)
    Offset = (Offset + HEADER_ALIGNMENT - 1) & (~(HEADER_ALIGNMENT - 1))

    if VariableStoreHeader.Format != VARIABLE_STORE_FORMATTED :
      sys.exit(0)

    if VariableStoreHeader.State != VARIABLE_STORE_HEALTHY :
      sys.exit(0)

    VarIndex = 1
    while Offset < FullSize :
      VariableHeader = VARIABLE_HEADER.from_buffer (bytearray(args.InputFileBuffer), Offset)
      print "#//  VARIABLE_HEADER:"
      print "#//    StartId    - 0x%04x" % VariableHeader.StartId
      print "#//    State      - 0x%02x" % VariableHeader.State
      print "#//    Attributes - 0x%08x" % VariableHeader.Attributes
      print "#//    NameSize   - 0x%08x" % VariableHeader.NameSize
      print "#//    DataSize   - 0x%08x" % VariableHeader.DataSize
      print "#//    VendorGuid - %08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x" % (VariableHeader.VendorGuid.Guid1, VariableHeader.VendorGuid.Guid2, VariableHeader.VendorGuid.Guid3, VariableHeader.VendorGuid.Guid4[0], VariableHeader.VendorGuid.Guid4[1], VariableHeader.VendorGuid.Guid4[2], VariableHeader.VendorGuid.Guid4[3], VariableHeader.VendorGuid.Guid4[4], VariableHeader.VendorGuid.Guid4[5], VariableHeader.VendorGuid.Guid4[6], VariableHeader.VendorGuid.Guid4[7])
      Offset += sizeof(VARIABLE_HEADER)

      if VariableHeader.StartId != VARIABLE_DATA :
        sys.exit(0)

      Name = FullInputFileBuffer[Offset:Offset + VariableHeader.NameSize]
      AsciiName = []
      for Int in Name:
        if Int == 0:
          continue
        AsciiName.append(chr(Int))
      AsciiName = ''.join(AsciiName)
      print "#//    Name       - L\"" + AsciiName + "\""
      #print "#//    Name       - L\"",
      #DumpName(Name)
      #print "\""
      Offset += VariableHeader.NameSize

      print "#//    Data       - "
      Data=FullInputFileBuffer[Offset:Offset + VariableHeader.DataSize]
      DumpHexData (Data)
      Offset += VariableHeader.DataSize

      print "  gOemSkuTokenSpaceGuid.Pcd" + AsciiName + "Data|{",
      DumpBin(Data)
      #print "}|VOID*|0x000F%04x" % VarIndex
      print "}"
      print ""

      #OutputFile = open(AsciiName, 'wb')
      #OutputFile.write(Data)
      #OutputFile.close()

      VarIndex = VarIndex + 1
      Offset = (Offset + HEADER_ALIGNMENT - 1) & (~(HEADER_ALIGNMENT - 1))

