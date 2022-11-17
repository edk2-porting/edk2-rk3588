## @ PatchFspBinBaseAddress.py
#
# Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

import os
import re
import sys
import struct
from   datetime import date

fspSBaseAddress = 0
fspMBaseAddress = 0
fspTBaseAddress = 0

def GetFspBaseAddress (binfile):
      offset = 0;
      for index in range(1,4):
        attribute = readDataFromFile(binfile, offset+0xB7, 1) >> 4
        if attribute == 0x3:
          global fspSBaseAddress
          fspSBaseAddress = readDataFromFile(binfile, offset+0xB0, 4)
        if attribute == 0x2:
          global fspMBaseAddress
          fspMBaseAddress = readDataFromFile(binfile, offset+0xB0, 4)
        if attribute == 0x1:
          global fspTBaseAddress
          fspTBaseAddress = readDataFromFile(binfile, offset+0xB0, 4)
        offset += readDataFromFile(binfile, offset+0xAC, 4)
      return 0

#
#  Read data from file
#
#  param [in]  binfile     Binary file
#  param [in]  offset      Offset
#  param [in]  len         Length
#
#  retval      value       Value
#
def readDataFromFile (binfile, offset, len=1):
      fd     = open(binfile, "rb")
      fsize  = os.path.getsize(binfile)
      offval = offset & 0xFFFFFFFF
      if (offval & 0x80000000):
          offval = fsize - (0xFFFFFFFF - offval + 1)
      fd.seek(offval)
      bytearray = [ord(b) for b in fd.read(len)]
      value = 0
      idx   = len - 1
      while  idx >= 0:
          value = value << 8 | bytearray[idx]
          idx = idx - 1
      fd.close()
      return value

def updateFspFvsBase (binfile, TargetFile):
      ext_file = str(os.path.splitext(TargetFile)[-1]).lower()
      if not os.path.exists(binfile):
        print "WARNING!  " + str(binfile) + " is not found."
        return 1
      if not os.path.exists(TargetFile):
        print "WARNING!  " + str(TargetFile) + " is not found."
        return 1

      GetFspBaseAddress(binfile)

      if ext_file == ".dsc":
        DscFile        = open(TargetFile, "r")
        DscLines     = DscFile.readlines()
        DscFile.close()
        DscContent = []

        for line in DscLines:
            DscContent.append(line)
        DscFile = open(TargetFile,"w")

        for index in range(len(DscContent)):
              DscLine = DscContent[index]
              Match = re.match("([_a-zA-Z0-9]+).Pcd(Fspt|Fspm|Fsps)BaseAddress",DscLine)
              if Match:
                  DscLine = Match.group(1) + ".Pcd" + Match.group(2) + "BaseAddress|0x"
                  if Match.group(2) == 'Fspt':
                      BaseAddrStr = str(hex(fspTBaseAddress)[2:]).zfill(8).upper().rstrip('L')
                  elif Match.group(2) == 'Fspm':
                      BaseAddrStr = str(hex(fspMBaseAddress)[2:]).zfill(8).upper().rstrip('L')
                  elif Match.group(2) == 'Fsps':
                      BaseAddrStr = str(hex(fspSBaseAddress)[2:]).zfill(8).upper().rstrip('L')
                  DscLine = DscLine + BaseAddrStr + "\n"
              DscFile.writelines(DscLine)
        DscFile.close()
        return 0

def Main():
    #
    # Parse the options and args
    #
    if len(sys.argv) != 3:
        print "error"
        return 1
    ret = updateFspFvsBase (sys.argv[1], sys.argv[2])
    
    if ret != 0:
      return 1
    return 0
    
if __name__ == '__main__':
    sys.exit(Main())
