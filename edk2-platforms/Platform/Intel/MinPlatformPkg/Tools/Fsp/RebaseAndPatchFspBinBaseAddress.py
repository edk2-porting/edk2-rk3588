## @ RebaseAndPatchFspBinBaseAddress.py
#
# Copyright (c) 2017 - 2021, Intel Corporation. All rights reserved.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

import os
import sys
import re
import subprocess

if len(sys.argv) not in [6,7]:
  print ("RebaseAndPatchFspBinBaseAddress.py - Error in number of arguments received")
  print ("Usage - RebaseAndPatchFspBinBaseAddress.py <FlashMap file path> <FspBinPkg Folder> <Fsp.fd file name>\
  <Dsc file path to be patched> <pad_offset for Fsp-S Base Address> <OPTIONAL SplitFspBin.py tool path>")
  exit(1)

flashMapName      = sys.argv[1]
fspBinPath        = sys.argv[2]
fspBinFile        = sys.argv[3]
targetDscFile     = sys.argv[4]
fvOffset          = int(sys.argv[5], 16)
fspBinFileRebased = "Fsp_Rebased.fd"
splitFspBinPath   = os.path.join("edk2","IntelFsp2Pkg","Tools","SplitFspBin.py")

if len(sys.argv) == 7:
  splitFspBinPath   = sys.argv[6]

#
# Make sure argument passed or valid
#
if not os.path.exists(flashMapName):
  print ("WARNING!  " + str(flashMapName) + " is not found.")
  exit(1)
fspBinFilePath = fspBinPath + os.sep + fspBinFile
if not os.path.exists(fspBinFilePath):
  print ("WARNING!  " + str(fspBinFilePath) + " is not found.")
  exit(1)
if not os.path.exists(targetDscFile):
  print ("WARNING!  " + str(targetDscFile) + " is not found.")
  exit(1)
ext_file = str(os.path.splitext(targetDscFile)[-1]).lower()
if ext_file != ".dsc":
  print ("WARNING!  " + str(targetDscFile) + " is not a dsc file")
  exit(1)
if not os.path.exists(splitFspBinPath):
  print ("WARNING!  " + str(splitFspBinPath) + " is not found.")
  exit(1)

#
# Get the FSP-S / FSP-M-T FV Base Address from Flash Map
#
file = open (flashMapName, "r")
data = file.read ()

# Get the Flash Base Address
flashBase = int(data.split("FLASH_BASE")[1].split("=")[1].split()[0], 16)

# Based on Build Target, select the section in the FlashMap file
flashmap = data

# Get FSP-S & FSP-M & FSP-T offset & calculate the base
for line in flashmap.split("\n"):
  if "PcdFlashFvFspSOffset" in line:
    fspSBaseOffset = int(line.split("=")[1].split()[0], 16)
  if "PcdFlashFvFspMOffset" in line:
    fspMBaseOffset = int(line.split("=")[1].split()[0], 16)
  if "PcdFlashFvFspTOffset" in line:
    fspTBaseOffset = int(line.split("=")[1].split()[0], 16)
file.close()

#
# Get FSP-M Size, in order to calculate the FSP-T Base. Used SplitFspBin.py script 
# to dump the header, and get the ImageSize in FSP-M section
#
pythontool = sys.executable
Process = subprocess.Popen([pythontool, splitFspBinPath, "info","-f",fspBinFilePath], stdout=subprocess.PIPE)
Output = Process.communicate()[0]
FsptInfo = Output.rsplit(b"FSP_M", 1);
for line in FsptInfo[1].split(b"\n"):
  if b"ImageSize" in line:
    fspMSize = int(line.split(b"=")[1], 16)
    break

# Calculate FSP-S/M/T base address, to which re-base has to be done
fspSBaseAddress = flashBase + fspSBaseOffset + fvOffset
fspMBaseAddress = flashBase + fspMBaseOffset
fspTBaseAddress = flashBase + fspTBaseOffset

#
# Re-base FSP bin file to new address and save it as fspBinFileRebased using SplitFspBin.py
#
rebaseArguments = fspBinFilePath + " -c s m t -b " + str(hex(fspSBaseAddress).rstrip("L")) + " " + str(hex(fspMBaseAddress).rstrip("L")) + " " + str(hex(fspTBaseAddress).rstrip("L")) + " -o" + fspBinPath + " -n " + fspBinFileRebased
os.system('"' + pythontool + '"' + " " + splitFspBinPath + " rebase -f" + rebaseArguments)

#
# Split FSP bin to FSP-S/M/T segments
#
splitArguments = fspBinPath + os.sep + fspBinFileRebased + " -o " + fspBinPath + " -n Fsp_Rebased.fd"
os.system('"' + pythontool + '"' + " " + splitFspBinPath + " split -f" + splitArguments)

#
# Patch dsc file with the re-based FSP-S/M/T address, so internally build will use the same.
#
DscFile  = open(targetDscFile, "r")
DscLines = DscFile.readlines()
DscFile.close()
DscContent = []
for line in DscLines:
  DscContent.append(line)
DscFile = open(targetDscFile,"w")
for index in range(len(DscContent)):
  DscLine = DscContent[index]
  Match = re.match("([\s_a-zA-Z0-9]+).Pcd(Fspt|Fspm|Fsps)BaseAddress",DscLine)
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
exit(0)
