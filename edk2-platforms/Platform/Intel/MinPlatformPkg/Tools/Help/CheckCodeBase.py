## @ CheckCodeBase.py
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

class FileChecker:
    def __init__(self):
        # sourceRoot == WORKSPACE
        # sourceRoot != PACKAGES_PATH
        self.sourceRoot = ""
        self.includeCoreList = ["\\CryptoPkg\\", "\\FatBinPkg\\", "\\FatPkg\\", "\\IntelFrameworkModulePkg\\", "\\IntelFrameworkPkg\\", "\\IntelFsp2Pkg\\", "\\IntelFsp2WrapperPkg\\", "\\IntelFspPkg\\", "\\IntelFspWrapperPkg\\", "\\IntelSiliconPkg\\", "\\MdeModulePkg\\", "\\MdePkg\\", "\\NetworkPkg\\", "\\PcAtChipsetPkg\\", "\\PerformancePkg\\", "\\SecurityPkg\\", "\\ShellBinPkg\\", "\\ShellPkg\\", "\\SignedCapsulePkg\\", "\\SourceLevelDebugPkg\\", "\\UefiCpuPkg\\"]
        self.includeList = self.includeCoreList
        self.excluseCoreList = ["\\BaseTools\\", "\\Conf\\", "\\Tools\\", "\\Build\\", "\\tool\\", "\\.svn", "\\.git", "\\Override\\", "\\SampleCode\\", "\\openssl"]
        self.excluseList = self.excluseCoreList
        self.usedModuleList = []
        self.usedLibraryList = []
        self.makefileList = []
        self.usedIncludeFileList = []
        self.usedModuleFileList = []
        self.usedLibraryFileList = []
        self.usedFileList = []
        self.usedPackageList = []
        self.allModuleList = []
        self.allLibraryList = []
        self.allDirList = []
        self.allIncludeFileList = []
        self.allModuleFileList = []
        self.allLibraryFileList = []
        self.allFileList = []
        self.allPackageList = []
        self.unusedModuleList = []
        self.unusedLibraryList = []
        self.unusedIncludeFileList = []
        self.unusedModuleFileList = []
        self.unusedLibraryFileList = []
        self.unusedFileList = []
        self.unusedPackageList = []
        self.unusedPackageFileList = []

    def CheckFile(self, file):
        if os.path.isfile(file):
            return file.decode('gbk')
        else:
            return ""

    def CheckDir(self, file):
        if os.path.isdir(file):
            return file.decode('gbk')
        else:
            return ""

    def IsIncludeFile(self, file):
        if (cmp (file[-2:], ".h") == 0) and (file.find ("Pkg\\Include") != -1):
            return True
        else:
            return False
            
    def IsLibraryFile(self, file):
        if (file.find ("Pkg\\Library") != -1):
            return True
        else:
            return False

    def CheckIncludeFile(self, file):
        if self.IsIncludeFile(file) and (self.IsInExclusiveList(file) == False):
            return file.decode('gbk')
        else:
            return ""

    def GetModulePathFromMakefile (self, file):
        makefile = open (file, "r")
        data = makefile.read ()
        for line in data.split("\n"):
            # MODULE_DIR = c:\home\edkiigit\edk2\MdeModulePkg\Library\BaseSerialPortLib16550
            if "MODULE_DIR =" in line:
                moduleDir = line.split(" = ")[1]
            # BUILD_DIR = c:\home\edkiigit\Build\KabylakeOpenBoardPkg\KabylakeRvp3\DEBUG_VS2015x86
            if "BUILD_DIR =" in line:
                buildDir = line.split(" = ")[1]
        makefile.close()

        # record sourceRoot = c:\home\Edk-II
        self.sourceRoot = buildDir.split("\\Build\\")[0]

        if (self.IsInExclusiveList(moduleDir) == False):
            self.AddPackageList(moduleDir, self.usedPackageList)

        return moduleDir

    def AddPackageList(self, file, packageList):
        packageName = file.split("Pkg")
        packagePath = packageName[0] + "Pkg"
        if packagePath not in packageList:
            packageList.append(packagePath)

    def IsInPackageList(self, file):
        for package in self.usedPackageList:
            if file.find(package) != -1:
                return True
        return False

    def CheckUsedModule(self, file):
        if (cmp (file[-8:], "Makefile") == 0) and (self.IsLibraryFile(file) == False):
            finalDir = self.GetModulePathFromMakefile(file)
            if self.IsInExclusiveList(finalDir):
                return ""
            return finalDir.decode('gbk')
        else:
            return ""
            
    def CheckUsedLibrary(self, file):
        if (cmp (file[-8:], "Makefile") == 0) and self.IsLibraryFile(file):
            finalDir = self.GetModulePathFromMakefile(file)
            if self.IsInExclusiveList(finalDir):
                return ""
            return finalDir.decode('gbk')
        else:
            return ""

    def CheckMakefile(self, file):
        if cmp (file[-8:], "Makefile") == 0:
            finalDir = self.GetModulePathFromMakefile(file)
            if self.IsInExclusiveList(finalDir):
                return ""
            return file.decode('gbk')
        else:
            return ""

    def ParseMakefile(self, fileName):
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
                if cmp (line[0], "#") == 0:
                    continue
                newline = line[:-1]

                #print "check - (" + newline + ")"
                headFile = ""
                #COMMON_DEPS = $(WORKSPACE)\MdePkg\Include\Protocol\DebugSupport.h \
                #              $(WORKSPACE)\MdePkg\Include\Ppi\PciCfg2.h \
                #$(OUTPUT_DIR)\X64\Semaphore.obj : $(WORKSPACE)\UefiCpuPkg\Include\Library\MtrrLib.h
                #MatchString = "[\w\\\s\=\:\.\(\)\$]*\$\(WORKSPACE\)([\\\w]*\.h)[\\w\s]*"
                MatchString1 = "(COMMON_DEPS = |              |\$\(OUTPUT_DIR\)[\\\\\w]*.obj : )\$\(WORKSPACE\)\\\\([\\\\\w]*\.h)"
                match1 = re.match(MatchString1, newline)
                if match1 is not None:
                    #print "match1 - " + newline
                    #print "0 - " + match1.group(0)
                    #print "1 - " + match1.group(1)
                    #print "2 - " + match1.group(2)
                    headFile = match1.group(2)

                if (cmp (headFile, "") != 0) and self.IsIncludeFile(headFile):
                    finalPath = os.path.join (self.sourceRoot, headFile)
                    if (self.IsInExclusiveList(finalPath) == False) and (finalPath not in self.usedIncludeFileList):
                        self.usedIncludeFileList.append(finalPath)
                        self.AddPackageList (finalPath, self.usedPackageList)

        finally:
            file.close()

        return

    def IsInIncludeList(self, file):
        for list in self.includeList:
            result = file.find(list);
            if (result != -1):
                return True
        return False

    def IsInExclusiveList(self, file):
        if self.IsInIncludeList(file) == False:
            return True
        for list in self.excluseList:
            result = file.find(list);
            if (result != -1):
                return True
        for list in self.excluseList:
            full_file = file + "\\"
            result = full_file.find(list);
            if (result != -1):
                return True
        return False

    def IsFinalDir(self, file):
        for s in os.listdir(file):
            newFile=os.path.join(file,s)
            if cmp (newFile[-4:], ".inf") == 0:
                return True
        return False

    def IsInUsedModuleList(self, file):
        if file in self.usedModuleList:
            return True
        else:
            return False

    def IsInUsedLibraryList(self, file):
        if file in self.usedLibraryList:
            return True
        else:
            return False

    def IsUsedModule(self, file):
        for used in self.usedModuleList:
            final = used + "\\"
            result = file.find(final);
            if (result != -1):
                return True
        return False
        
    def IsUsedLibrary(self, file):
        for used in self.usedLibraryList:
            final = used + "\\"
            result = file.find(final);
            if (result != -1):
                return True
        return False

    def IsUnusedModule(self, file):
        for unused in self.unusedModuleList:
            final = unused + "\\"
            result = file.find(final);
            if (result != -1):
                return True
        return False
        
    def IsUnusedLibrary(self, file):
        for unused in self.unusedLibraryList:
            final = unused + "\\"
            result = file.find(final);
            if (result != -1):
                return True
        return False
        
    def IsUnusedPackage(self, file):
        for unused in self.unusedPackageList:
            final = unused + "\\"
            result = file.find(final);
            if (result != -1):
                return True
        return False

    def CheckUnusedModule(self, file):
        if os.path.isdir(file):
            if (self.IsInExclusiveList(file) == False) and self.IsFinalDir(file) and (self.IsInUsedModuleList(file) == False) and (self.IsLibraryFile(file) == False):
                return file.decode('gbk')
            else:
                return ""
        else:
            return ""
            
    def CheckUnusedLibrary(self, file):
        if os.path.isdir(file):
            if (self.IsInExclusiveList(file) == False) and self.IsFinalDir(file) and (self.IsInUsedLibraryList(file) == False) and self.IsLibraryFile(file):
                return file.decode('gbk')
            else:
                return ""
        else:
            return ""
            
    def CheckAllModule(self, file):
        if os.path.isdir(file):
            if (self.IsInExclusiveList(file) == False) and self.IsFinalDir(file) and (self.IsLibraryFile(file) == False):
                return file.decode('gbk')
            else:
                return ""
        else:
            return ""
            
    def CheckAllLibrary(self, file):
        if os.path.isdir(file):
            if (self.IsInExclusiveList(file) == False) and self.IsFinalDir(file) and self.IsLibraryFile(file):
                return file.decode('gbk')
            else:
                return ""
        else:
            return ""
            
    def CheckAllDir(self, file):
        if os.path.isdir(file):
            if (self.IsInExclusiveList(file) == False):
                return file.decode('gbk')
            else:
                return ""
        else:
            return ""

    def CheckAllModuleFile(self, file):
        if os.path.isfile(file):
            if (self.IsInExclusiveList(file) == False) and (self.IsUsedModule(file) or self.IsUnusedModule(file)) and (self.IsLibraryFile(file) == False):
                return file.decode('gbk')
            else:
                return ""
        else:
            return ""
            
    def CheckAllLibraryFile(self, file):
        if os.path.isfile(file):
            if (self.IsInExclusiveList(file) == False) and (self.IsUsedLibrary(file) or self.IsUnusedLibrary(file)) and self.IsLibraryFile(file):
                return file.decode('gbk')
            else:
                return ""
        else:
            return ""
            
    def CheckAllFile(self, file):
        if os.path.isfile(file):
            if (self.IsInExclusiveList(file) == False):
                return file.decode('gbk')
            else:
                return ""
        else:
            return ""
            
    def CheckUsedModuleFile(self, file):
        if os.path.isfile(file):
            if (self.IsInExclusiveList(file) == False) and self.IsUsedModule(file):
                return file.decode('gbk')
            else:
                return ""
        else:
            return ""
            
    def CheckUsedLibraryFile(self, file):
        if os.path.isfile(file):
            if (self.IsInExclusiveList(file) == False) and self.IsUsedLibrary(file):
                return file.decode('gbk')
            else:
                return ""
        else:
            return ""
            
    def CheckUnusedModuleFile(self, file):
        if os.path.isfile(file):
            if (self.IsInExclusiveList(file) == False) and self.IsUnusedModule(file):
                return file.decode('gbk')
            else:
                return ""
        else:
            return ""
            
    def CheckUnusedLibraryFile(self, file):
        if os.path.isfile(file):
            if (self.IsInExclusiveList(file) == False) and self.IsUnusedLibrary(file):
                return file.decode('gbk')
            else:
                return ""
        else:
            return ""
            
    def CheckUnusedPackageFile(self, file):
        if os.path.isfile(file):
            if (self.IsInExclusiveList(file) == False) and self.IsUnusedPackage(file):
                return file.decode('gbk')
            else:
                return ""
        else:
            return ""
            
    def GetFileList(self, dir, fileList, checkFunc):
        newDir = dir
        AppendName = checkFunc (dir)
        if cmp (AppendName, "") != 0:
            #print "AppendName = " + AppendName
            if AppendName not in fileList:
                fileList.append(AppendName)
        if os.path.isdir(dir):  
            for sub in os.listdir(dir):
                newDir = os.path.join(dir,sub)
                self.GetFileList(newDir, fileList, checkFunc)  
        return fileList

    def DeleteEmptyDir(self, dir):
        if os.path.exists(dir) == False:
            return
        if os.path.isdir(dir):
            for sub in os.listdir(dir):
                newDir = os.path.join(dir,sub)
                if (os.path.isdir(newDir) == True):  
                    self.DeleteEmptyDir(newDir)
        if not os.listdir(dir):
            print "deleting empty " + dir
            os.rmdir(dir)
            
    def DeleteEmptyDirList(self, fileList):
        for file in fileList:
            self.DeleteEmptyDir(file)

    def DeleteUnusedFile(self, fileList):
        for file in fileList:
            if os.path.exists(file) == False:
                continue
            print "deleting " + file
            try :
                if os.path.isdir(file):
                    shutil.rmtree(file)
                if os.path.isfile(file):
                    os.remove(file)
            except :
                print "deleting error (" + file + ")"

    def PrintFileTime(self, file):
        statinfo = os.stat(file)
        strucatime = time.localtime(statinfo.st_atime)
        print "atime: " + str(statinfo.st_atime) + " (" + str(strucatime.tm_year) + "." + str(strucatime.tm_mon) + "." + str(strucatime.tm_mday) + " " + str(strucatime.tm_hour) + ":" + str(strucatime.tm_min) + ":" + str(strucatime.tm_sec) + ")"
        strucctime = time.localtime(statinfo.st_ctime)
        print "ctime: " + str(statinfo.st_ctime) + " (" + str(strucctime.tm_year) + "." + str(strucctime.tm_mon) + "." + str(strucctime.tm_mday) + " " + str(strucctime.tm_hour) + ":" + str(strucctime.tm_min) + ":" + str(strucctime.tm_sec) + ")"
        strucmtime = time.localtime(statinfo.st_mtime)
        print "mtime: " + str(statinfo.st_mtime) + " (" + str(strucmtime.tm_year) + "." + str(strucmtime.tm_mon) + "." + str(strucmtime.tm_mday) + " " + str(strucmtime.tm_hour) + ":" + str(strucmtime.tm_min) + ":" + str(strucmtime.tm_sec) + ")"
        
    def TouchFileTime(self, file):
        #currentTime = time.time()
        #os.utime(file, (currentTime, currentTime))
        os.utime(file, None)
        
    def TouchFileListTime(self, fileList):
        for file in fileList:
            self.TouchFileTime(file)

    def PrintFileList(self, fileList):
        for file in fileList:
            print file
        print "  Count - " + str(len(fileList))

    def SortFileList(self, fileList):
        fileList.sort()
            
    def PrintFileListTime(self, fileList):
        for file in fileList:
            print file
            self.PrintFileTime(file)

    def GetUsedModuleList(self):
        if (len(self.usedModuleList) == 0):
            self.usedModuleList = self.GetFileList(sys.argv[1], [], self.CheckUsedModule)
            self.SortFileList(self.usedModuleList)
            
    def GetUsedLibraryList(self):
        if (len(self.usedLibraryList) == 0):
            self.usedLibraryList = self.GetFileList(sys.argv[1], [], self.CheckUsedLibrary)
            self.SortFileList(self.usedLibraryList)

    def GetMakefileList(self):
        if (len(self.makefileList) == 0):
            self.GetUsedModuleList()
            self.makefileList = self.GetFileList(sys.argv[1], [], self.CheckMakefile)

    def GetUsedIncludeFileList(self):
        if (len(self.usedIncludeFileList) == 0):
            self.GetMakefileList()

            #print "\nMakefile:"
            #fileChecker.PrintFileList (fileChecker.makefileList)

            for file in self.makefileList:
                self.ParseMakefile(file)
            self.SortFileList(self.usedIncludeFileList)

    def GetUsedModuleFileList(self):
        if (len(self.usedModuleFileList) == 0):
            self.GetUsedModuleList()

            self.usedModuleFileList = self.GetFileList(self.sourceRoot, [], self.CheckUsedModuleFile)
            self.SortFileList(self.usedModuleFileList)
            
    def GetUsedLibraryFileList(self):
        if (len(self.usedLibraryFileList) == 0):
            self.GetUsedLibraryList()

            self.usedLibraryFileList = self.GetFileList(self.sourceRoot, [], self.CheckUsedLibraryFile)
            self.SortFileList(self.usedLibraryFileList)

    def GetUsedFileList(self):
        if (len(self.usedFileList) == 0):
            self.GetAllFileList()
            self.GetUnusedFileList()

            self.usedFileList = []
            for file in self.allFileList:
                if (file not in self.unusedFileList) and self.IsInPackageList(file):
                    self.usedFileList.append(file)
            
            self.SortFileList(self.usedFileList)

    def GetUsedPackageList(self):
        self.GetUsedModuleList()
        self.GetUsedLibraryList()
        self.GetUsedIncludeFileList()
        self.SortFileList(self.usedPackageList)

    def GetAllModuleList(self):
        if (len(self.allModuleList) == 0):
            self.GetUsedModuleList()

            self.allModuleList = self.GetFileList(self.sourceRoot, [], self.CheckAllModule)
            self.SortFileList(self.allModuleList)
            
    def GetAllLibraryList(self):
        if (len(self.allLibraryList) == 0):
            self.GetUsedLibraryList()

            self.allLibraryList = self.GetFileList(self.sourceRoot, [], self.CheckAllLibrary)
            self.SortFileList(self.allLibraryList)

    def GetAllIncludeFileList(self):
        if (len(self.allIncludeFileList) == 0):
            self.GetUsedModuleList()

            self.allIncludeFileList = self.GetFileList(self.sourceRoot, [], self.CheckIncludeFile)
            self.SortFileList(self.allIncludeFileList)

    def GetAllModuleFileList(self):
        if (len(self.allFileList) == 0):
            self.GetUsedModuleList()
            self.GetUnusedModuleList()

            self.allModuleFileList = self.GetFileList(self.sourceRoot, [], self.CheckAllModuleFile)
            self.SortFileList(self.allModuleFileList)
            
    def GetAllLibraryFileList(self):
        if (len(self.allFileList) == 0):
            self.GetUsedLibraryList()
            self.GetUnusedLibraryList()

            self.allLibraryFileList = self.GetFileList(self.sourceRoot, [], self.CheckAllLibraryFile)
            self.SortFileList(self.allLibraryFileList)

    def GetAllFileList(self):
        if (len(self.allFileList) == 0):
            self.GetUsedModuleList()

            self.allFileList = self.GetFileList(self.sourceRoot, [], self.CheckAllFile)
            self.SortFileList(self.allFileList)

    def GetAllPackageList(self):
        if (len(self.allPackageList) == 0):
            self.GetAllFileList()
            prefixLength = len(self.sourceRoot)
            for file in self.allFileList:
                finalPath = os.path.join (self.sourceRoot, file[prefixLength + 1:])
                self.AddPackageList(finalPath, self.allPackageList)
            self.SortFileList(self.allPackageList)

    def GetUnusedModuleList(self):
        if (len(self.unusedModuleList) == 0):
            self.GetUsedModuleList()

            self.unusedModuleList = self.GetFileList(self.sourceRoot, [], self.CheckUnusedModule)
            
    def GetUnusedLibraryList(self):
        if (len(self.unusedLibraryList) == 0):
            self.GetUsedLibraryList()

            self.unusedLibraryList = self.GetFileList(self.sourceRoot, [], self.CheckUnusedLibrary)

    def GetUnusedIncludeFileList(self):
        if (len(self.unusedIncludeFileList) == 0):
            self.GetUsedIncludeFileList()
            self.GetAllIncludeFileList()

            self.unusedIncludeFileList = []
            for file in self.allIncludeFileList:
                if file not in self.usedIncludeFileList:
                    self.unusedIncludeFileList.append(file)

            self.SortFileList(self.unusedIncludeFileList)

    def GetUnusedModuleFileList(self):
        if (len(self.unusedModuleFileList) == 0):
            self.GetUnusedModuleList ()

            self.unusedModuleFileList = self.GetFileList(self.sourceRoot, [], self.CheckUnusedModuleFile)
            self.SortFileList(self.unusedModuleFileList)

    def GetUnusedLibraryFileList(self):
        if (len(self.unusedLibraryFileList) == 0):
            self.GetUnusedLibraryList ()

            self.unusedLibraryFileList = self.GetFileList(self.sourceRoot, [], self.CheckUnusedLibraryFile)
            self.SortFileList(self.unusedLibraryFileList)

    def GetUnusedFileList(self):
        if (len(self.unusedFileList) == 0):
            self.GetUnusedIncludeFileList()
            self.GetUnusedModuleFileList()
            self.GetUnusedLibraryFileList()

            self.GetUnusedPackageList()
            self.unusedPackageFileList = self.GetFileList(self.sourceRoot, [], self.CheckUnusedPackageFile)

            self.unusedFileList = []
            self.unusedFileList.extend(self.unusedIncludeFileList)
            self.unusedFileList.extend(self.unusedModuleFileList)
            self.unusedFileList.extend(self.unusedLibraryFileList)
            
            for file in self.unusedPackageFileList:
                if file not in self.unusedFileList:
                    self.unusedFileList.append(file)

            self.SortFileList(self.unusedFileList)

    def GetUnusedPackageList(self):
        if (len(self.unusedPackageList) == 0):
            self.GetUsedPackageList()
            self.GetAllPackageList()

            for package in self.allPackageList:
                if package not in self.usedPackageList:
                    self.unusedPackageList.append(package)
            self.SortFileList(self.unusedPackageList)

#
#  Print out the usage
#
def usage():
    print "Usage: \n\tCheckCodeBase <Build Dir> used|unused|all"
    print "       used - used library, modules, include file, library files, module files, all used files"
    print "       unused - unused library, modules, include file, library files, module files, all unused files"
    print "       all - all library, modules, include file, library files, module files, all files"
    print "  library      : the directory of a library"
    print "  module       : the directory of a driver"
    print "  include file : the header files in include directory"
    print "  library file : all files in a library directory"
    print "  module file  : all files in a driver directory"
    print "  all file     : all files in project, including any other metadata files or batch files"
    #print "Usage: \n\tCheckCodeBase <Build Dir> time|touch"
    print "For Example: \n\tCheckCodeBase Build\KabylakeOpenBoardPkg\KabylakeRvp3\DEBUG_VS2015x86 used"

def main():
    global FileChecker

    fileChecker = FileChecker()

    if len(sys.argv) < 3:
        usage()
        return 1

    if cmp (sys.argv[2], "used") == 0:
        fileChecker.GetUsedModuleList ()
        print "\n  Used Module List:"
        fileChecker.PrintFileList (fileChecker.usedModuleList)

        fileChecker.GetUsedLibraryList ()
        print "\n  Used Library List:"
        fileChecker.PrintFileList (fileChecker.usedLibraryList)

        fileChecker.GetUsedIncludeFileList()
        print "\n Used Include File List:"
        fileChecker.PrintFileList (fileChecker.usedIncludeFileList)

        fileChecker.GetUsedModuleFileList()
        print "\n Used Module File List:"
        fileChecker.PrintFileList (fileChecker.usedModuleFileList)
        
        fileChecker.GetUsedLibraryFileList()
        print "\n Used Library File List:"
        fileChecker.PrintFileList (fileChecker.usedLibraryFileList)

        fileChecker.GetUsedFileList()
        print "\n All Used File List:"
        fileChecker.PrintFileList (fileChecker.usedFileList)
        
        fileChecker.GetUsedPackageList()
        print "\n Used Package List:"
        fileChecker.PrintFileList (fileChecker.usedPackageList)

        print "\n  ==== Used Module Summary ===="
        print "  Module Count       - " + str(len(fileChecker.usedModuleList))
        print "  Library Count      - " + str(len(fileChecker.usedLibraryList))
        print "  Include File Count - " + str(len(fileChecker.usedIncludeFileList))
        print "  Module File Count  - " + str(len(fileChecker.usedModuleFileList))
        print "  Library File Count - " + str(len(fileChecker.usedLibraryFileList))
        print "  All File Count     - " + str(len(fileChecker.usedFileList))

    elif cmp (sys.argv[2], "all") == 0:
        fileChecker.GetAllModuleList()
        print "\n All Module List:"
        fileChecker.PrintFileList (fileChecker.allModuleList)

        fileChecker.GetAllLibraryList()
        print "\n All Library List:"
        fileChecker.PrintFileList (fileChecker.allLibraryList)

        fileChecker.GetAllIncludeFileList()
        print "\n All Include File List:"
        fileChecker.PrintFileList (fileChecker.allIncludeFileList)
        
        fileChecker.GetAllModuleFileList()
        print "\n All Module File List:"
        fileChecker.PrintFileList (fileChecker.allModuleFileList)
        
        fileChecker.GetAllLibraryFileList()
        print "\n All Library File List:"
        fileChecker.PrintFileList (fileChecker.allLibraryFileList)
        
        fileChecker.GetAllFileList()
        print "\n All File List:"
        fileChecker.PrintFileList (fileChecker.allFileList)
        
        fileChecker.GetAllPackageList()
        print "\n All Package List:"
        fileChecker.PrintFileList (fileChecker.allPackageList)

        print "\n  ==== All Module Summary ===="
        print "  Module Count       - " + str(len(fileChecker.allModuleList))
        print "  Library Count      - " + str(len(fileChecker.allLibraryList))
        print "  Include File Count - " + str(len(fileChecker.allIncludeFileList))
        print "  Module File Count  - " + str(len(fileChecker.allModuleFileList))
        print "  Library File Count - " + str(len(fileChecker.allLibraryFileList))
        print "  All File Count     - " + str(len(fileChecker.allFileList))
        
    elif cmp (sys.argv[2], "unused") == 0:
        fileChecker.GetUnusedModuleList()
        print "\n Unused Module List:"
        fileChecker.PrintFileList (fileChecker.unusedModuleList)
        
        fileChecker.GetUnusedLibraryList()
        print "\n Unused Library List:"
        fileChecker.PrintFileList (fileChecker.unusedLibraryList)

        fileChecker.GetUnusedIncludeFileList()
        print "\n Unused Include List:"
        fileChecker.PrintFileList (fileChecker.unusedIncludeFileList)

        fileChecker.GetUnusedModuleFileList()
        print "\n Unused Module File List:"
        fileChecker.PrintFileList (fileChecker.unusedModuleFileList)
        
        fileChecker.GetUnusedLibraryFileList()
        print "\n Unused Library File List:"
        fileChecker.PrintFileList (fileChecker.unusedLibraryFileList)

        fileChecker.GetUnusedFileList()
        print "\n Unused File List:"
        fileChecker.PrintFileList (fileChecker.unusedFileList)
        
        fileChecker.GetUnusedPackageList()
        print "\n Unused Package List:"
        fileChecker.PrintFileList (fileChecker.unusedPackageList)

        print "\n  ==== Unused Module Summary ===="
        print "  Module Count       - " + str(len(fileChecker.unusedModuleList))
        print "  Library Count      - " + str(len(fileChecker.unusedLibraryList))
        print "  Include File Count - " + str(len(fileChecker.unusedIncludeFileList))
        print "  Module File Count  - " + str(len(fileChecker.unusedModuleFileList))
        print "  Library File Count - " + str(len(fileChecker.unusedLibraryFileList))
        print "  All File Count     - " + str(len(fileChecker.unusedFileList))

    elif cmp (sys.argv[2], "delete_unused") == 0:
        fileChecker.GetUnusedModuleList()
        fileChecker.GetUnusedLibraryList()
        fileChecker.GetUnusedIncludeFileList()
        fileChecker.GetUnusedPackageList()

        fileChecker.DeleteUnusedFile(fileChecker.unusedPackageList)
        fileChecker.DeleteUnusedFile(fileChecker.unusedModuleList)
        fileChecker.DeleteUnusedFile(fileChecker.unusedLibraryList)
        fileChecker.DeleteUnusedFile(fileChecker.unusedIncludeFileList)

        fileChecker.allDirList = fileChecker.GetFileList(fileChecker.sourceRoot, [], fileChecker.CheckAllDir)
        fileChecker.DeleteEmptyDirList(fileChecker.allDirList)

    elif cmp (sys.argv[2], "time") == 0:
        fileChecker.allDirList = fileChecker.GetFileList(sys.argv[1], [], fileChecker.CheckFile)
        fileChecker.PrintFileListTime(fileChecker.allDirList)
        
    elif cmp (sys.argv[2], "touch") == 0:
        fileChecker.allDirList = fileChecker.GetFileList(sys.argv[1], [], fileChecker.CheckFile)
        fileChecker.TouchFileListTime(fileChecker.allDirList)
    else:
        print "Unknown - " + sys.argv[2]

if __name__ == '__main__':
    sys.exit(main())
