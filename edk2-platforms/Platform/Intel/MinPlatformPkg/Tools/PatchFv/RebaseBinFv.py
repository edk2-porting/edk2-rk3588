## @ PatchBinFv.py
#
# Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

import os
import re
import sys
import time
import shutil
import struct
import binascii
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

class EFI_FIRMWARE_VOLUME_EXT_HEADER(Structure):
    _fields_ = [
        ('FvName',               GUID),
        ('ExtHeaderSize',        c_uint32),
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


class EFI_FV_FILETYPE:
    ALL                        = 0x00
    RAW                        = 0x01
    FREEFORM                   = 0x02
    SECURITY_CORE              = 0x03
    PEI_CORE                   = 0x04
    DXE_CORE                   = 0x05
    PEIM                       = 0x06
    DRIVER                     = 0x07
    COMBINED_PEIM_DRIVER       = 0x08
    APPLICATION                = 0x09
    SMM                        = 0x0a
    FIRMWARE_VOLUME_IMAGE      = 0x0b
    COMBINED_SMM_DXE           = 0x0c
    SMM_CORE                   = 0x0d
    OEM_MIN                    = 0xc0
    OEM_MAX                    = 0xdf
    DEBUG_MIN                  = 0xe0
    DEBUG_MAX                  = 0xef
    FFS_MIN                    = 0xf0
    FFS_MAX                    = 0xff
    FFS_PAD                    = 0xf0

class EFI_SECTION_TYPE:
    ALL                        = 0x00
    COMPRESSION                = 0x01
    GUID_DEFINED               = 0x02
    DISPOSABLE                 = 0x03
    PE32                       = 0x10
    PIC                        = 0x11
    TE                         = 0x12
    DXE_DEPEX                  = 0x13
    VERSION                    = 0x14
    USER_INTERFACE             = 0x15
    COMPATIBILITY16            = 0x16
    FIRMWARE_VOLUME_IMAGE      = 0x17
    FREEFORM_SUBTYPE_GUID      = 0x18
    RAW                        = 0x19
    PEI_DEPEX                  = 0x1b
    SMM_DEPEX                  = 0x1c

IMAGE_FILE_MACHINE_I386           = 0x014c
IMAGE_FILE_MACHINE_X64            = 0x8664

EFI_IMAGE_DIRECTORY_ENTRY_BASERELOC   = 5

class EFI_IMAGE_DOS_HEADER(Structure):
    _fields_ = [
        ('e_magic',              c_uint16),
        ('e_cblp',               c_uint16),
        ('e_cp',                 c_uint16),
        ('e_crlc',               c_uint16),
        ('e_cparhdr',            c_uint16),
        ('e_minalloc',           c_uint16),
        ('e_maxalloc',           c_uint16),
        ('e_ss',                 c_uint16),
        ('e_sp',                 c_uint16),
        ('e_csum',               c_uint16),
        ('e_ip',                 c_uint16),
        ('e_cs',                 c_uint16),
        ('e_lfarlc',             c_uint16),
        ('e_ovno',               c_uint16),
        ('e_res',                ARRAY(c_uint16, 4)),
        ('e_oemid',              c_uint16),
        ('e_oeminfo',            c_uint16),
        ('e_res2',               ARRAY(c_uint16, 10)),
        ('e_lfanew',             c_uint16)
        ]

class EFI_IMAGE_DATA_DIRECTORY(Structure):
    _fields_ = [
        ('VirtualAddress',       c_uint32),
        ('Size',                 c_uint32)
        ]

class EFI_IMAGE_FILE_HEADER(Structure):
    _fields_ = [
        ('Machine',               c_uint16),
        ('NumberOfSections',      c_uint16),
        ('TimeDateStamp',         c_uint32),
        ('PointerToSymbolTable',  c_uint32),
        ('NumberOfSymbols',       c_uint32),
        ('SizeOfOptionalHeader',  c_uint16),
        ('Characteristics',       c_uint16)
        ]

class EFI_IMAGE_OPTIONAL_HEADER32(Structure):
    _fields_ = [
        ('Magic',                         c_uint16),
        ('MajorLinkerVersion',            c_uint8),
        ('MinorLinkerVersion',            c_uint8),
        ('SizeOfCode',                    c_uint32),
        ('SizeOfInitializedData',         c_uint32),
        ('SizeOfUninitializedData',       c_uint32),
        ('AddressOfEntryPoint',           c_uint32),
        ('BaseOfCode',                    c_uint32),
        ('BaseOfData',                    c_uint32),
        ('ImageBase',                     c_uint32),
        ('SectionAlignment',              c_uint32),
        ('FileAlignment',                 c_uint32),
        ('MajorOperatingSystemVersion',   c_uint16),
        ('MinorOperatingSystemVersion',   c_uint16),
        ('MajorImageVersion',             c_uint16),
        ('MinorImageVersion',             c_uint16),
        ('MajorSubsystemVersion',         c_uint16),
        ('MinorSubsystemVersion',         c_uint16),
        ('Win32VersionValue',             c_uint32),
        ('SizeOfImage',                   c_uint32),
        ('SizeOfHeaders',                 c_uint32),
        ('CheckSum'     ,                 c_uint32),
        ('Subsystem',                     c_uint16),
        ('DllCharacteristics',            c_uint16),
        ('SizeOfStackReserve',            c_uint32),
        ('SizeOfStackCommit' ,            c_uint32),
        ('SizeOfHeapReserve',             c_uint32),
        ('SizeOfHeapCommit' ,             c_uint32),
        ('LoaderFlags'     ,              c_uint32),
        ('NumberOfRvaAndSizes',           c_uint32),
        ('DataDirectory',                 ARRAY(EFI_IMAGE_DATA_DIRECTORY, 16))
        ]

class EFI_IMAGE_OPTIONAL_HEADER64(Structure):
    _fields_ = [
        ('Magic',                         c_uint16),
        ('MajorLinkerVersion',            c_uint8),
        ('MinorLinkerVersion',            c_uint8),
        ('SizeOfCode',                    c_uint32),
        ('SizeOfInitializedData',         c_uint32),
        ('SizeOfUninitializedData',       c_uint32),
        ('AddressOfEntryPoint',           c_uint32),
        ('BaseOfCode',                    c_uint32),
        ('ImageBase',                     c_uint64),
        ('SectionAlignment',              c_uint32),
        ('FileAlignment',                 c_uint32),
        ('MajorOperatingSystemVersion',   c_uint16),
        ('MinorOperatingSystemVersion',   c_uint16),
        ('MajorImageVersion',             c_uint16),
        ('MinorImageVersion',             c_uint16),
        ('MajorSubsystemVersion',         c_uint16),
        ('MinorSubsystemVersion',         c_uint16),
        ('Win32VersionValue',             c_uint32),
        ('SizeOfImage',                   c_uint32),
        ('SizeOfHeaders',                 c_uint32),
        ('CheckSum'     ,                 c_uint32),
        ('Subsystem',                     c_uint16),
        ('DllCharacteristics',            c_uint16),
        ('SizeOfStackReserve',            c_uint64),
        ('SizeOfStackCommit' ,            c_uint64),
        ('SizeOfHeapReserve',             c_uint64),
        ('SizeOfHeapCommit' ,             c_uint64),
        ('LoaderFlags'     ,              c_uint32),
        ('NumberOfRvaAndSizes',           c_uint32),
        ('DataDirectory',                 ARRAY(EFI_IMAGE_DATA_DIRECTORY, 16))
        ]

class EFI_IMAGE_NT_HEADERS32(Structure):
    _fields_ = [
        ('Signature',            c_uint32),
        ('FileHeader',           EFI_IMAGE_FILE_HEADER),
        ('OptionalHeader',       EFI_IMAGE_OPTIONAL_HEADER32)
        ]

class EFI_IMAGE_NT_HEADERS64(Structure):
    _fields_ = [
        ('Signature',            c_uint32),
        ('FileHeader',           EFI_IMAGE_FILE_HEADER),
        ('OptionalHeader',       EFI_IMAGE_OPTIONAL_HEADER64)
        ]

class EFI_IMAGE_SECTION_HEADER(Structure):
    _fields_ = [
        ('Name',                  ARRAY(c_uint8, 8)),
        ('VirtualSize',           c_uint32),
        ('VirtualAddress',        c_uint32),
        ('SizeOfRawData',         c_uint32),
        ('PointerToRawData',      c_uint32),
        ('PointerToRelocations',  c_uint32),
        ('PointerToLinenumbers',  c_uint32),
        ('NumberOfRelocations',   c_uint16),
        ('NumberOfLinenumbers',   c_uint16),
        ('Characteristics',       c_uint32),
        ]

class EFI_TE_IMAGE_HEADER(Structure):
    _fields_ = [
        ('Signature',            ARRAY(c_char, 2)),
        ('Machine',              c_uint16),
        ('NumberOfSections',     c_uint8),
        ('Subsystem',            c_uint8),
        ('StrippedSize',         c_uint16),
        ('AddressOfEntryPoint',  c_uint32),
        ('BaseOfCode',           c_uint32),
        ('ImageBase',            c_uint64),
        ('DataDirectoryBaseReloc',  EFI_IMAGE_DATA_DIRECTORY),
        ('DataDirectoryDebug',      EFI_IMAGE_DATA_DIRECTORY)
        ]

class EFI_IMAGE_DIRECTORY_ENTRY:
    EXPORT                     = 0
    IMPORT                     = 1
    RESOURCE                   = 2
    EXCEPTION                  = 3
    SECURITY                   = 4
    BASERELOC                  = 5
    DEBUG                      = 6
    COPYRIGHT                  = 7
    GLOBALPTR                  = 8
    TLS                        = 9
    LOAD_CONFIG                = 10

class PE_RELOC_BLOCK_HEADER(Structure):
    _fields_ = [
        ('PageRVA',              c_uint32),
        ('BlockSize',            c_uint32)
        ]

def AlignPtr (offset, alignment = 8):
    return (offset + alignment - 1) & ~(alignment - 1)

def Bytes2Val (bytes):
    return reduce(lambda x,y: (x<<8)|y,  bytes[::-1] )

def Val2Bytes (value, blen):
    return [(value>>(i*8) & 0xff) for i in range(blen)]

class PeTeImage:
    def __init__(self, offset, data):
        self.Offset    = offset
        tehdr          = EFI_TE_IMAGE_HEADER.from_buffer (data, 0)
        if   tehdr.Signature == 'VZ': # TE image
            self.TeHdr   = tehdr
        elif tehdr.Signature == 'MZ': # PE32 image
            self.TeHdr   = None
            self.DosHdr  = EFI_IMAGE_DOS_HEADER.from_buffer (data, 0)
            self.PeHdr   = EFI_IMAGE_NT_HEADERS32.from_buffer (data, self.DosHdr.e_lfanew)
            if self.PeHdr.Signature != 0x4550:
                raise Exception("ERROR: Invalid PE32 header !")
            if self.PeHdr.FileHeader.SizeOfOptionalHeader < EFI_IMAGE_OPTIONAL_HEADER32.DataDirectory.offset:
                raise Exception("ERROR: Unsupported PE32 image !")
            if self.PeHdr.OptionalHeader.NumberOfRvaAndSizes <= EFI_IMAGE_DIRECTORY_ENTRY.BASERELOC:
                raise Exception("ERROR: No relocation information available !")
        self.Offset    = offset
        self.Data      = data
        self.RelocList = []

    def IsTeImage(self):
        return  self.TeHdr is not None

    def ParseReloc(self):
        if self.IsTeImage():
            rsize   = self.TeHdr.DataDirectoryBaseReloc.Size
            roffset = sizeof(self.TeHdr) - self.TeHdr.StrippedSize + self.TeHdr.DataDirectoryBaseReloc.VirtualAddress
        else:
            rsize   = self.PeHdr.OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY.BASERELOC].Size
            roffset = self.PeHdr.OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY.BASERELOC].VirtualAddress

        alignment = 4
        offset = roffset
        while offset < roffset + rsize:
            offset = AlignPtr(offset, 4)
            blkhdr = PE_RELOC_BLOCK_HEADER.from_buffer(self.Data, offset)
            offset += sizeof(blkhdr)
            # Read relocation type,offset pairs
            rlen  = blkhdr.BlockSize - sizeof(PE_RELOC_BLOCK_HEADER)
            rnum  = rlen/sizeof(c_uint16)
            rdata = (c_uint16 * rnum).from_buffer(self.Data, offset)
            for each in rdata:
                roff  = each & 0xfff
                rtype = each >> 12
                if rtype == 0: # IMAGE_REL_BASED.ABSOLUTE:
                    continue
                if rtype != 3: # IMAGE_REL_BASED_HIGHLOW
                    raise Exception("ERROR: Unsupported relocation type %d!" % rtype)
                # Calculate the offset of the relocation
                aoff  = blkhdr.PageRVA + roff
                if self.IsTeImage():
                    aoff += sizeof(self.TeHdr) - self.TeHdr.StrippedSize
                self.RelocList.append((rtype, aoff))
            offset += sizeof(rdata)

    def Rebase(self, delta, fdbin):
        count = 0
        if delta == 0:
            return count

        for (rtype, roff) in self.RelocList:
            if rtype == 0x03: # HIGHLOW
                offset = roff + self.Offset
                value  = Bytes2Val(fdbin[offset:offset+sizeof(c_uint32)])
                value += delta
                fdbin[offset:offset+sizeof(c_uint32)] = Val2Bytes(value, sizeof(c_uint32))
                count += 1
            else:
                raise Exception('ERROR: Unknown relocation type %d !' % rtype)

        if self.IsTeImage():
            offset  = self.Offset + EFI_TE_IMAGE_HEADER.ImageBase.offset
            size    = EFI_TE_IMAGE_HEADER.ImageBase.size
        else:
            offset  = self.Offset + self.DosHdr.e_lfanew
            offset += EFI_IMAGE_NT_HEADERS32.OptionalHeader.offset
            offset += EFI_IMAGE_OPTIONAL_HEADER32.ImageBase.offset
            size    = EFI_IMAGE_OPTIONAL_HEADER32.ImageBase.size

        value  = Bytes2Val(fdbin[offset:offset+size]) + delta
        fdbin[offset:offset+size] = Val2Bytes(value, size)

        return count


class Section:
    def __init__(self, offset, secdata):
        self.SecHdr   = EFI_COMMON_SECTION_HEADER.from_buffer (secdata, 0)
        self.SecData  = secdata[0:int(self.SecHdr.Size)]
        self.Offset   = offset

class FirmwareFile:
    def __init__(self, offset, filedata):
        self.FfsHdr   = EFI_FFS_FILE_HEADER.from_buffer (filedata, 0)
        self.FfsData  = filedata[0:int(self.FfsHdr.Size)]
        self.Offset   = offset
        self.SecList  = []

    def ParseFfs(self):
        ffssize = len(self.FfsData)
        offset  = sizeof(self.FfsHdr)
        if self.FfsHdr.Name != '\xff' * 16:
            while offset < ffssize:
                sechdr = EFI_COMMON_SECTION_HEADER.from_buffer (self.FfsData, offset)
                sec = Section (offset, self.FfsData[offset:offset + int(sechdr.Size)])
                self.SecList.append(sec)
                offset += int(sechdr.Size)
                offset  = AlignPtr(offset, 4)

class FirmwareVolume:
    def __init__(self, offset, fvdata):
        self.FvHdr    = EFI_FIRMWARE_VOLUME_HEADER.from_buffer (fvdata, 0)
        self.FvData   = fvdata[0 : self.FvHdr.FvLength]
        self.Offset   = offset
        if self.FvHdr.ExtHeaderOffset > 0:
            self.FvExtHdr = EFI_FIRMWARE_VOLUME_EXT_HEADER.from_buffer (self.FvData, self.FvHdr.ExtHeaderOffset)
        else:
            self.FvExtHdr = None
        self.FfsList  = []

    def ParseFv(self):
        fvsize = len(self.FvData)
        if self.FvExtHdr:
            offset = self.FvHdr.ExtHeaderOffset + self.FvExtHdr.ExtHeaderSize
        else:
            offset = self.FvHdr.HeaderLength
        offset = AlignPtr(offset)
        while offset < fvsize:
            ffshdr = EFI_FFS_FILE_HEADER.from_buffer (self.FvData, offset)
            if (ffshdr.Name == '\xff' * 16) and (int(ffshdr.Size) == 0xFFFFFF):
                offset = fvsize
            else:
                ffs = FirmwareFile (offset, self.FvData[offset:offset + int(ffshdr.Size)])
                ffs.ParseFfs()
                self.FfsList.append(ffs)
                offset += int(ffshdr.Size)
                offset = AlignPtr(offset)


class FileChecker:
    def __init__(self):
        # sourceRoot == WORKSPACE
        # sourceRoot != PACKAGES_PATH
        self.RebasePcd = ["", "", "", ""]
        self.FvName = ""
        self.target = ""
        self.sourceRoot = ""
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
        print "PCD: " + pcd[0] + "|" + pcd[3] + " <== " + pcd[1] + "(" + pcd[2] + ")"

    def RebaseFv(self, fvName, rebasePcd):
        sourceFileName = os.path.join(self.sourceRoot,fvName,self.target,fvName+".Fv")
        print "rebasing(FV) - " + sourceFileName

        try :
            file = open(sourceFileName, "rb")
        except Exception:
            print "fail to open " + sourceFileName
            return
        try:
            buffer = file.read()
            data = bytearray(buffer)
            file.close()

            FvHeader = EFI_FIRMWARE_VOLUME_HEADER.from_buffer (data, 0)
            print "HeaderLength    - " + hex(FvHeader.HeaderLength)
            print "ExtHeaderOffset - " + hex(FvHeader.ExtHeaderOffset)

            if (FvHeader.ExtHeaderOffset == 0):
                Offset = FvHeader.HeaderLength
            else:
                FvExHeader = EFI_FIRMWARE_VOLUME_EXT_HEADER.from_buffer(data, FvHeader.ExtHeaderOffset)
                print "  FvName  - %08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x" % (FvExHeader.FvName.Guid1, FvExHeader.FvName.Guid2, FvExHeader.FvName.Guid3, FvExHeader.FvName.Guid4[0], FvExHeader.FvName.Guid4[1], FvExHeader.FvName.Guid4[2], FvExHeader.FvName.Guid4[3], FvExHeader.FvName.Guid4[4], FvExHeader.FvName.Guid4[5], FvExHeader.FvName.Guid4[6], FvExHeader.FvName.Guid4[7])
                print "  ExtHeaderSize - " + hex(FvExHeader.ExtHeaderSize)
                Offset = FvHeader.ExtHeaderOffset + FvExHeader.ExtHeaderSize
            Offset = (Offset + 0x7) & ~0x7

            while (Offset < FvHeader.FvLength) :
                FfsHeader = EFI_FFS_FILE_HEADER.from_buffer (data, Offset)
                FfsOffset = Offset
                FfsSize = FfsHeader.Size[0] + (FfsHeader.Size[1] << 8) + (FfsHeader.Size[2] << 16)
                if (FfsSize == 0xFFFFFF) :
                    break
                #print "Ffs    - " + hex(FfsOffset)
                if (FfsHeader.Type == 0xFF) or (FfsHeader.Type == EFI_FV_FILETYPE_FFS_PAD) :
                    Offset = (FfsOffset + FfsSize + 7) & ~0x7
                    continue
                print "Ffs - %08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x" % (FfsHeader.Name.Guid1, FfsHeader.Name.Guid2, FfsHeader.Name.Guid3, FfsHeader.Name.Guid4[0], FfsHeader.Name.Guid4[1], FfsHeader.Name.Guid4[2], FfsHeader.Name.Guid4[3], FfsHeader.Name.Guid4[4], FfsHeader.Name.Guid4[5], FfsHeader.Name.Guid4[6], FfsHeader.Name.Guid4[7])
                Offset = Offset + sizeof(EFI_FFS_FILE_HEADER)
                while (Offset < FfsOffset + FfsSize) :
                    SectionHeader = EFI_COMMON_SECTION_HEADER.from_buffer (data, Offset)
                    #print "  Section - " + hex(Offset)
                    if (SectionHeader.Type == EFI_SECTION_PE32) or (SectionHeader.Type == EFI_SECTION_TE) :
                        PeOffset = Offset + sizeof(EFI_COMMON_SECTION_HEADER)
                        print "    PE - " + hex(PeOffset) + "(" + binascii.hexlify(data[PeOffset:PeOffset+2]) + ")"

                        newbase = int(rebasePcd[1],16)
                        oldbase = int(rebasePcd[3],16)

                        delta = newbase - oldbase
                        print "    delta - " + hex(delta) + "(" + hex(oldbase) + " <== " + hex(newbase) + ")"

                        PeLength = FfsSize-sizeof(EFI_FFS_FILE_HEADER);

                        img = PeTeImage(PeOffset, data[PeOffset:PeOffset + PeLength])
                        img.ParseReloc()
                        img.Rebase(delta, data)

                    SectionSize = SectionHeader.Size[0] + (SectionHeader.Size[1] << 8) + (SectionHeader.Size[2] << 16)
                    Offset = (Offset + SectionSize + 3) & ~0x3
                Offset = (FfsOffset + FfsSize + 7) & ~0x7

            file = open(sourceFileName, "wb")
            file.write(data)
        finally:
            file.close()

    def GetPcdFromReport(self, file, pcd):
        FoundPkg = False
        pcdSplit = pcd.split(".")
        TargetPkg = pcdSplit[0]
        TargetPcd = pcdSplit[1]
        while 1:
            line = file.readline()
            if not line:
                break

            newline = line[:-1].replace('\r','')

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

    def GetOldFvBase (self, fvName, PcdName):
        ParseBase = False
        Value = ""
        fileName = os.path.join(self.sourceRoot,fvName,self.target,fvName+".inf")
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

                newline = line[:-1].replace('\r','')

                if cmp (newline, "") == 0:
                    continue

                if cmp (newline, "#![Pcd]") == 0:
                    ParseBase = True
                    continue

                if ParseBase == True :
                    if (cmp (line[0:2], "#!") != 0) :
                        ParseBase = False
                        continue
                    newline = newline[2:].strip()

                    splitLine = newline.split("|")
                    if cmp (PcdName, splitLine[0]) == 0:
                        Value = splitLine[1]
        finally:
            file.close()

        return Value

    def SetNewFvBase (self, fvName, PcdName, OldFvBase, NewFvBase):
        fileName = os.path.join(self.sourceRoot,fvName,self.target,fvName+".inf")
        print "update - " + fileName
        try :
            file = open(fileName, "r")
        except Exception:
            print "fail to open " + fileName
            return
        try:
            lines = file.readlines()
            file.close()

            ParseBase = False
            for index in range(len(lines)):
                line = lines[index]
                if not line:
                    break

                newline = line[:-1].strip()

                if cmp (newline, "") == 0:
                    continue

                if cmp (newline, "#![Pcd]") == 0:
                    ParseBase = True
                    continue

                if ParseBase == True :
                    if (cmp (line[0:2], "#!") != 0) :
                        ParseBase = False
                        continue
                    newline = newline[2:].strip()

                    splitLine = newline.split("|")
                    if cmp (PcdName, splitLine[0]) == 0:
                        if cmp (OldFvBase, splitLine[1]) != 0:
                            print "ERROR: OldFvBase mismatch!"
                        else:
                            lines[index] = "#!  " + PcdName + "|" + NewFvBase + "\n"
                            break

            file = open(fileName, "w")
            file.writelines(lines)
        finally:
            file.close()


    def GetRebaseAddressFromReport(self):
        try :
            file = open(self.reportFile)
        except Exception:
            print "fail to open " + self.reportFile
            return
        try:
            file.seek(0)
            print "checking - " + self.RebasePcd[0]
            ValuePair = self.GetPcdFromReport (file, self.RebasePcd[0])
            self.RebasePcd[1] = ValuePair[0]
            self.RebasePcd[2] = ValuePair[1]
        finally:
            file.close()

def main():
    global FileChecker

    fileChecker = FileChecker()

    if (len(sys.argv) != 6) :
        print "usage: RebaseBinFv <Target> <SourceRoot> <ReportFile> <FvName> <RebasePcdName>"
        return 0

    fileChecker.target       = sys.argv[1]
    fileChecker.sourceRoot   = sys.argv[2]
    fileChecker.reportFile   = sys.argv[3]
    fileChecker.FvName       = sys.argv[4]
    fileChecker.RebasePcd[0] = sys.argv[5]

    fileChecker.GetRebaseAddressFromReport()

    fileChecker.RebasePcd[3] = fileChecker.GetOldFvBase (fileChecker.FvName, fileChecker.RebasePcd[0])

    fileChecker.PrintRebasePcd(fileChecker.RebasePcd)

    fileChecker.RebaseFv (fileChecker.FvName, fileChecker.RebasePcd)

    fileChecker.SetNewFvBase (fileChecker.FvName, fileChecker.RebasePcd[0], fileChecker.RebasePcd[3], fileChecker.RebasePcd[1])

if __name__ == '__main__':
    sys.exit(main())
