## @ SyncBinFvInf.py
#
# Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

import os
import re
import sys
import time
import shutil
from   ctypes import *

class GUID(Structure):
    _fields_ = [
        ('Guid1',            c_uint32),
        ('Guid2',            c_uint16),
        ('Guid3',            c_uint16),
        ('Guid4',            ARRAY(c_uint8, 8)),
        ]


class EFI_FIRMWARE_VOLUME_HEADER(Structure):
    _fields_ = [
        ('ZeroVector',           ARRAY(c_uint8, 16)),
        ('FileSystemGuid',       GUID),
        ('FvLength',             c_uint64),
        ('Signature',            c_uint32),
        ('Attributes',           c_uint32),
        ('HeaderLength',         c_uint16),
        ('Checksum',             c_uint16),
        ('ExtHeaderOffset',      c_uint16),
        ('Reserved',             c_uint8),
        ('Revision',             c_uint8),
        ]

#
# File Types Definitions
#
EFI_FV_FILETYPE_ALL                   = 0x00
EFI_FV_FILETYPE_RAW                   = 0x01
EFI_FV_FILETYPE_FREEFORM              = 0x02
EFI_FV_FILETYPE_SECURITY_CORE         = 0x03
EFI_FV_FILETYPE_PEI_CORE              = 0x04
EFI_FV_FILETYPE_DXE_CORE              = 0x05
EFI_FV_FILETYPE_PEIM                  = 0x06
EFI_FV_FILETYPE_DRIVER                = 0x07
EFI_FV_FILETYPE_COMBINED_PEIM_DRIVER  = 0x08
EFI_FV_FILETYPE_APPLICATION           = 0x09
EFI_FV_FILETYPE_SMM                   = 0x0A
EFI_FV_FILETYPE_FIRMWARE_VOLUME_IMAGE = 0x0B
EFI_FV_FILETYPE_COMBINED_SMM_DXE      = 0x0C
EFI_FV_FILETYPE_SMM_CORE              = 0x0D
EFI_FV_FILETYPE_OEM_MIN               = 0xc0
EFI_FV_FILETYPE_OEM_MAX               = 0xdf
EFI_FV_FILETYPE_DEBUG_MIN             = 0xe0
EFI_FV_FILETYPE_DEBUG_MAX             = 0xef
EFI_FV_FILETYPE_FFS_MIN               = 0xf0
EFI_FV_FILETYPE_FFS_MAX               = 0xff
EFI_FV_FILETYPE_FFS_PAD               = 0xf0
#
# FFS File Attributes.
#
FFS_ATTRIB_LARGE_FILE         = 0x01
FFS_ATTRIB_DATA_ALIGNMENT_2   = 0x02
FFS_ATTRIB_FIXED              = 0x04
FFS_ATTRIB_DATA_ALIGNMENT     = 0x38
FFS_ATTRIB_CHECKSUM           = 0x40

#
# FFS File State Bits.
#
EFI_FILE_HEADER_CONSTRUCTION  = 0x01
EFI_FILE_HEADER_VALID         = 0x02
EFI_FILE_DATA_VALID           = 0x04
EFI_FILE_MARKED_FOR_UPDATE    = 0x08
EFI_FILE_DELETED              = 0x10
EFI_FILE_HEADER_INVALID       = 0x20

class EFI_FFS_FILE_HEADER(Structure):
    _fields_ = [
        ('Name',                 GUID),
        ('IntegrityCheck',       c_uint16),
        ('Type',                 c_uint8),
        ('Attributes',           c_uint8),
        ('Size',                 ARRAY(c_uint8, 3)),
        ('State',                c_uint8),
        ]

class EFI_FFS_FILE_HEADER2(Structure):
    _fields_ = [
        ('Name',                 GUID),
        ('IntegrityCheck',       c_uint16),
        ('Type',                 c_uint8),
        ('Attributes',           c_uint8),
        ('Size',                 ARRAY(c_uint8, 3)),
        ('State',                c_uint8),
        ('ExtendedSize',         c_uint64),
        ]

#
# Pseudo type. It is used as a wild card when retrieving sections.
#  The section type EFI_SECTION_ALL matches all section types.
#
EFI_SECTION_ALL                   = 0x00

#
# Encapsulation section Type values.
#
EFI_SECTION_COMPRESSION           = 0x01

EFI_SECTION_GUID_DEFINED          = 0x02

EFI_SECTION_DISPOSABLE            = 0x03

#
# Leaf section Type values.
#
EFI_SECTION_PE32                  = 0x10
EFI_SECTION_PIC                   = 0x11
EFI_SECTION_TE                    = 0x12
EFI_SECTION_DXE_DEPEX             = 0x13
EFI_SECTION_VERSION               = 0x14
EFI_SECTION_USER_INTERFACE        = 0x15
EFI_SECTION_COMPATIBILITY16       = 0x16
EFI_SECTION_FIRMWARE_VOLUME_IMAGE = 0x17
EFI_SECTION_FREEFORM_SUBTYPE_GUID = 0x18
EFI_SECTION_RAW                   = 0x19
EFI_SECTION_PEI_DEPEX             = 0x1B
EFI_SECTION_SMM_DEPEX             = 0x1C

class EFI_COMMON_SECTION_HEADER(Structure):
    _fields_ = [
        ('Size',                 ARRAY(c_uint8, 3)),
        ('Type',                 c_uint8),
        ]

class EFI_COMMON_SECTION_HEADER2(Structure):
    _fields_ = [
        ('Size',                 ARRAY(c_uint8, 3)),
        ('Type',                 c_uint8),
        ('ExtendedSize',         c_uint32),
        ]

class FileChecker:
    def __init__(self):
        self.SyncSectionList = ["Packages", "PatchPcd", "PcdEx"]
        self.RebasePcd = ["", "", ""]
        self.FvName = ""
        self.FfsGuidList = []
        self.FfsInfList = []
        self.FfsOffsetList = []
        self.PeOffsetList = []
        self.target = ""
        self.sourceRoot = ""
        self.sourceInfList = []
        self.destRoot = ""
        self.reportFile = ""

    def GetSectionName(self, line):
        splitLine = line[1:-1].split(".")
        return splitLine[0]

    def IsSyncSection(self, line):
        name = self.GetSectionName(line)
        for sectionName in self.SyncSectionList:
            if (cmp (sectionName, name) == 0) :
                return True
        return False

    def PrintRebasePcd(self, pcd):
        if cmp (pcd[0], "") != 0:
            print "PCD: " + pcd[0] + "|" + pcd[1] + "(" + pcd[2] + ")"

    def PrintList(self, fileList):
        for file in fileList:
            print file

    def GetFileList(self, dir, fileList, checkFunc):
        if os.path.isdir(dir):  
            for sub in os.listdir(dir):
                if sub[0] == "." :
                    continue
                newDir = os.path.join(dir,sub)
                if (os.path.isdir(newDir) == True):
                    self.GetFileList(newDir, fileList, checkFunc)
                else :
                    AppendName = checkFunc (newDir)
                    if cmp (AppendName, "") != 0:
                        #print "AppendName = " + AppendName
                        if AppendName not in fileList:
                            fileList.append(AppendName)

    def GetInfFileGuid(self, fileName):
        guid = ""
        try :
            file = open(fileName)
        except Exception:
            print "fail to open " + fileName
            return
        try:
            while 1:
                line = file.readline()
                if not line:
                    break

                newline = line[:-1]

                if cmp (line[:11], "  FILE_GUID") == 0:
                    splitLine = line.split("=")
                    templine = splitLine[1]
                    guid = templine[1:1+36]
        finally:
            file.close()
        return guid

    def GetInfNameFromGuid(self, fileList, guid):
        for file in fileList:
            fileGuid = self.GetInfFileGuid (file)
            if (cmp (fileGuid.upper(), guid.upper()) == 0) :
                return file
        return ""

    def CheckSourceInf(self, file):
        if (cmp (file[-4:], ".inf") == 0) and (file.find("BinPkg") != -1) and (file.find(self.target) != -1) and (file.find("FVFSP") == -1):
            return file
        return ""

    def ParseInfFile(self, fileName, destFile, Offset):
        SyncToDest = False
        PatchOffset = False
        try :
            file = open(fileName)
        except Exception:
            print "fail to open " + fileName
            return
        try:
            while 1:
                line = file.readline()
                if not line:
                    break

                newline = line[:-1]

                if cmp (line[0], "[") == 0:
                    SyncToDest = self.IsSyncSection(line)
                    PatchOffset = False

                if SyncToDest == True :
                    if (cmp (self.GetSectionName(line), "PatchPcd") == 0) :
                        PatchOffset = True

                    if (PatchOffset == True) and ('|' in line) :
                        splitLine = line.split("|")

                        destFile.writelines(splitLine[0] + "|" + splitLine[1] + "|")

                        DataOffset = int(splitLine[2], 16)
                        DataOffset = DataOffset + int(Offset, 16)
                        destFile.writelines(hex(DataOffset))

                        destFile.writelines(" # " + splitLine[2][:-1] + "+" + Offset + "\n")
                    else :
                        destFile.writelines(line)
                else :
                    destFile.write("# ")
                    destFile.writelines(line)

        finally:
            file.close()
        return

    def ParseInfFiles(self, fileList, FfsOffsetList, destFileName, RebasePcd):
        try :
            destFile = open(destFileName, "a")
        except Exception:
            print "fail to open " + destFileName
            return
        try:
            if cmp (RebasePcd[0], "") != 0 :
                destFile.write("\n#![Pcd]\n")
                destFile.write("#!  ")
                destFile.write(RebasePcd[0])
                destFile.write("|")
                destFile.write(RebasePcd[1])
                destFile.write("\n\n")

            index = 0
            for file in fileList:
                if (cmp (file, "") == 0) :
                    index = index + 1
                    continue
                print "handling - (" + str(index) + ") :" + file
                self.ParseInfFile (file, destFile, FfsOffsetList[index])
                index = index + 1
        finally:
            destFile.close()
        return

    def GetGuildFfsGuidList(self, fileName):
        self.FfsGuidList = []
        self.FfsOffsetList = []
        try :
            file = open(fileName)
        except Exception:
            print "fail to open " + fileName
            return
        try:
            while 1:
                line = file.readline()
                if not line:
                    break

                #0x000D4868 A8499E65-A6F6-48B0-96DB-45C266030D83
                MatchString = "(0x[0-9a-fA-F]{8}) ([0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12})"
                match = re.match(MatchString, line)
                if match is not None:
                    offset = match.group(1)
                    self.FfsOffsetList.append(offset)
                    ffsGuid = match.group(2)
                    self.FfsGuidList.append(ffsGuid)
        finally:
            file.close()
        return

    def GetPeOffsetList(self, fileName):
        self.PeOffsetList = []
        try :
            file = open(fileName, "rb")
        except Exception:
            print "fail to open " + fileName
            return
        try:
            FvBuffer = file.read()
            FvData = bytearray(FvBuffer)

            for FfsOffset in self.FfsOffsetList:
                PeOffsetFound = False
                PeOffset = int(FfsOffset, 16)
                FfsHeader = EFI_FFS_FILE_HEADER.from_buffer (FvData, PeOffset)
                FfsSize = FfsHeader.Size[0] + (FfsHeader.Size[1] << 8) + (FfsHeader.Size[2] << 16)
                PeOffset = PeOffset + sizeof(EFI_FFS_FILE_HEADER)
                while (PeOffset < int(FfsOffset, 16) + FfsSize) :
                    SectionHeader = EFI_COMMON_SECTION_HEADER.from_buffer (FvData, PeOffset)
                    if (SectionHeader.Type == EFI_SECTION_PE32) or (SectionHeader.Type == EFI_SECTION_TE) :
                        PeOffset = PeOffset + sizeof(EFI_COMMON_SECTION_HEADER)
                        self.PeOffsetList.append(hex(PeOffset))
                        PeOffsetFound = True
                        break
                    else:
                        SectionSize = SectionHeader.Size[0] + (SectionHeader.Size[1] << 8) + (SectionHeader.Size[2] << 16)
                        PeOffset = (PeOffset + SectionSize + 3) & ~0x3
                if (PeOffsetFound == False):
                        self.PeOffsetList.append(0)

        finally:
            file.close()
        return

    def ProcessFvInf(self, fvName, RebasePcd):
        destFile = os.path.join(self.destRoot,fvName+"\\"+self.target+"\\"+fvName+".Fv.txt")
        print "\nprocessing - " + destFile
        self.GetGuildFfsGuidList (destFile)

        #print "FfsGuidList"
        #self.PrintList(self.FfsGuidList)
        #print "FfsOffsetList"
        #self.PrintList(self.FfsOffsetList)

        destFile = os.path.join(self.destRoot,fvName+"\\"+self.target+"\\"+fvName+".Fv")
        self.GetPeOffsetList (destFile)

        #print "PeOffsetList"
        #self.PrintList(self.PeOffsetList)

        self.FfsInfList = []
        for guid in self.FfsGuidList:
            fileName = self.GetInfNameFromGuid(self.sourceInfList, guid)
            print "    adding - " + guid + " : " + fileName
            self.FfsInfList.append(fileName)

        #print "FfsInfList"
        #self.PrintList(self.FfsInfList)

        shutil.copy(os.path.join(self.destRoot,fvName+"\\"+self.target+"\\"+fvName+".Base.inf"),os.path.join(self.destRoot,fvName+"\\"+self.target+"\\"+fvName+".inf"))

        self.ParseInfFiles (self.FfsInfList, self.PeOffsetList, os.path.join(self.destRoot,fvName+"\\"+self.target+"\\"+fvName+".inf"), RebasePcd)

    def GetPcdFromReport(self, file, pcd):
        FoundPkg = False
        pcdSplit = pcd.split(".")
        TargetPkg = pcdSplit[0]
        TargetPcd = pcdSplit[1]
        while 1:
            line = file.readline()
            if not line:
                break

            newline = line[:-1]

            if (cmp (newline, TargetPkg) == 0):
                FoundPkg = True
                continue

            if (cmp (newline, "") == 0) or ((cmp (newline[0], " ") != 0) and (cmp (newline[0], "0") != 0)):
                FoundPkg = False

            if (FoundPkg == True) :
                newline = newline.strip()
                splitLine = newline.split(" ", 2)
                if (cmp (splitLine[0], "*F") == 0) or (cmp (splitLine[0], "*P") == 0):
                    if (cmp (splitLine[1], TargetPcd) == 0):
                        print "found - " + TargetPkg + "." + TargetPcd

                        splitLine = splitLine[2].strip()[1:].strip().split(" ", 1)
                        if (cmp (splitLine[0], "FIXED") == 0) or (cmp (splitLine[0], "PATCH") == 0):
                            SplitLine = splitLine[1].strip()[1:].split(")", 1)
                            Type = SplitLine[0]
                            Value = SplitLine[1].strip()[1:].strip().split()[0]
                            print "  Type - (" + Type + "), Value - (" + Value + ")"
                            return [Value, Type]
        return ["", ""]

    def GetRebaseAddressFromReport(self):
        try :
            file = open(self.reportFile)
        except Exception:
            print "fail to open " + self.reportFile
            return
        try:
            file.seek(0)
            if (cmp(self.RebasePcd[0], "") != 0):
                print "checking - " + self.RebasePcd[0]
                ValuePair = self.GetPcdFromReport (file, self.RebasePcd[0])
                self.RebasePcd[1] = ValuePair[0]
                self.RebasePcd[2] = ValuePair[1]
        finally:
            file.close()

    def DumpFileList(self, dir):
        #print "DumpFileList - " + dir
        if os.path.exists(dir) == False:
            return
        if os.path.isdir(dir):
            for sub in os.listdir(dir):
                #print "Get sub - " + sub
                if sub[0] == "." :
                    continue
                newDir = os.path.join(dir,sub)
                if (os.path.isdir(newDir) == True):  
                    self.DumpFileList(newDir)
                else :
                    print "file - " + newDir
            
def main():
    global FileChecker

    fileChecker = FileChecker()

    if (len(sys.argv) != 6) and (len(sys.argv) != 7):
        print "usage: SyncBinFvInf <Target> <SourceRoot> <DestRoot> <ReportFile> <FspFvName> [<RebasePcdName>]"
        return 0

    fileChecker.target     = sys.argv[1]
    fileChecker.sourceRoot = sys.argv[2]
    fileChecker.destRoot   = sys.argv[3]
    fileChecker.reportFile = sys.argv[4]
    fileChecker.FvName     = sys.argv[5]
    if (len(sys.argv) == 7):
        fileChecker.RebasePcd[0]  = sys.argv[6]

    fileChecker.GetRebaseAddressFromReport()
    fileChecker.PrintRebasePcd (fileChecker.RebasePcd)

    fileChecker.GetFileList (fileChecker.sourceRoot, fileChecker.sourceInfList, fileChecker.CheckSourceInf)

    fileChecker.ProcessFvInf (fileChecker.FvName, fileChecker.RebasePcd)

if __name__ == '__main__':
    sys.exit(main())
