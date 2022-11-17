## @file
#
# Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

'''
AmlGenOffset
'''

import os
import sys
import argparse
import subprocess
import uuid
import struct
import collections
import binascii
import re
from   ctypes import *

#
# Globals for help information
#
__prog__      = 'AmlGenOffset'
__version__   = '%s Version %s' % (__prog__, '0.1 ')
__copyright__ = 'Copyright (c) 2017, Intel Corporation. All rights reserved.'
__usage__     = '%s -e|-d [options] <input_file>' % (__prog__)


if __name__ == '__main__':
  #
  # Create command line argument parser object
  #
  parser = argparse.ArgumentParser(prog=__prog__, version=__version__, usage=__usage__, description=__copyright__, conflict_handler='resolve')
  group = parser.add_mutually_exclusive_group(required=True)
  group.add_argument("-e", action="store_true", dest='Encode', help='encode file')
  group.add_argument("-d", action="store_true", dest='Decode', help='decode file')
  parser.add_argument("-o", "--output", dest='OutputFile', type=str, metavar='filename', help="specify the output filename", required=True)
  parser.add_argument("-v", "--verbose", dest='Verbose', action="store_true", help="increase output messages")
  parser.add_argument("-q", "--quiet", dest='Quiet', action="store_true", help="reduce output messages")
  parser.add_argument("--debug", dest='Debug', type=int, metavar='[0-9]', choices=range(0,10), default=0, help="set debug level")
  parser.add_argument("--aml_filter", dest='AmlFilterStr', type=str, help="specify the AML filter.")
  parser.add_argument(metavar="input_file", dest='InputFile', type=argparse.FileType('rb'), help="specify the input filename")

  #
  # Parse command line arguments
  #
  args = parser.parse_args()

  if args.Encode:
    print 'Unsupported'

  if args.Decode:
    args.OutputFileName = args.OutputFile
    args.OutputFile = open(args.OutputFileName, 'wb')

    AmlFilter = args.AmlFilterStr
    filter_pattern = '|'.join(AmlFilter.split(' '))

    lines = args.InputFile.readlines()
    args.InputFile.close()
    for line in lines:
        if line.strip().startswith('{\"') == False:
            if line.strip().startswith('* Compilation') == False and line.strip().startswith('* ASL+') == False and line.strip().startswith('* Copyright') == False:
                args.OutputFile.write(line)
        else:
            match_obj = re.search(filter_pattern, line, re.M | re.I)
            if match_obj is not None:
                args.OutputFile.write(line)
    args.OutputFile.close()

