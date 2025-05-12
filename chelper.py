#Inno setup path 
INNOSETUP_PATH = "\"C:\\Program Files (x86)\\Inno Setup 5\\compil32.exe\""

#firmware path to file
FIRMWARE_FILE_PATH = "..\\Firmware\\project\\RELEASE\\mpu_board_release.hex"

#file copy to labo disk except *.exe
update_files = []


















































import ctypes  # An included library with Python install.
import sys
import glob
import os
from shutil import copyfile
import re
from datetime import datetime




YES = 6
NO = 7




target = ""#automatically generated
version = "0.0"#automatically generated
version_date = ""#automatically generated

SETUP_ISS ="""

#define MyAppName                    "@PRJ"
#define MyAppVersion                 "@VER"
#define MyAppPublisher               "ON Semiconductor"
#define MyAppURL                     "http://www.onsemi.com/"
#define MyAppExeName                 "@PRJ.exe"
#define CompanyName                  "ON Semiconductor"
#define CompanyFolder                "ON Semiconductor"
#define CompanyCommonFolder          "Common"
#define CompanyDocumentationFolder   "Documentation"

[Setup]
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName=c:\Users\Public\{#MyAppName}
DefaultGroupName={#CompanyFolder}\{#MyAppName}
OutputBaseFilename=setup
OutputDir=..\Install
SetupIconFile=bin\onlogo.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "bin\QtCore4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\QtGui4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\QtNetwork4.dll"; DestDir: "{app}"; Flags: ignoreversion

;@MORE FILES ADD HERE
      

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{pf32}\IDC Tool\idc.exe" ; Parameters: {#MyAppExeName} /regserver;  StatusMsg: Registering ActiveX components...
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
"""



BENCH_RC ="""
1 TYPELIB "bench.rc"
IDI_ICON1               ICON    DISCARDABLE     "onlogo.ico"

    #include <windows.h>

    VS_VERSION_INFO     VERSIONINFO
    PRODUCTVERSION      0, 1, 0, 0
    FILEFLAGSMASK       0x3fL
    FILEFLAGS           0
    FILEOS              VOS_NT_WINDOWS32
    FILETYPE            VFT_APP
    FILESUBTYPE         VFT2_UNKNOWN
    BEGIN
        BLOCK   "VarFileInfo"
        BEGIN
            VALUE   "Translation",  0x409,  1200
        END
        BLOCK   "StringFileInfo"
        BEGIN
            BLOCK   "040904b0"
            BEGIN
                VALUE   "CompanyName",      "ON Semiconductor\\0"
                VALUE   "FileDescription",  "%3 , %2\\0"
                VALUE   "FileVersion",      "%1\\0"
                VALUE   "InternalName",     "\\0"
                VALUE   "LegalCopyright",   "\\0"
                VALUE   "OriginalFilename", "\\0"
                VALUE   "ProductName",      "%3 Bench Board Software\\0"
                VALUE   "ProductVersion",   "\\0"
            END
        END
    END
"""
VERSION_H ="""
/* Generated version file PROJECT:%3 */
#ifndef VERSION_H 
#define VERSION_H
#define VERSION "%1"
#define VERSION_DATE "%2"
#endif
"""


def readVersion():
  global version
  global version_date
  if os.path.isfile('v.h') :
    f = open("v.h","rb")   
    text =""
    m = None
    if f != None :
      text = f.read()  
      m = re.search( "#define VERSION \"([\\w\\s\\d\\.]+)\"", text, re.I | re.M)
      if m : version = m.group(1) 
      m = re.search( "#define VERSION_DATE \"([\\w\\s\\d:\\.]+)\"", text, re.I | re.M)
      if m : version_date = m.group(1)
  

def incrementVersion():
  global version
  global version_date
  minor=0
  major=0  
  if '.' in version : 
    major = int(version.split('.')[0])
    minor = int(version.split('.')[1])
    minor +=1
    version = "%02d.%04d"%(major,minor)
  else :
    pass
  version_date =  datetime.now().strftime('%a %b %d %H:%M:%S %Y')
  print version,version_date



def saveVersionFiles():
  f = open("v.h","wb+")
  text = VERSION_H
  text = text.replace('%2',version_date)
  text = text.replace('%1',version)
  text = text.replace('%3',target)
  if f: 
    f.write(text);
    f.close()
  f = open("bench.rc","wb+")
  text = BENCH_RC
  text = text.replace('%1',version)
  text = text.replace('%2',version_date)
  text = text.replace('%3',target)
  if f: 
    f.write(text);
    f.close()  





def makeSetupIss():
  text = SETUP_ISS 
  text = text.replace("@PRJ",target)
  text = text.replace("@VER","%s %s"%(version,version_date))

  tmp=""
  for f in update_files:
    if os.path.isfile(f) :
      tmp += "Source: \"%s\"; DestDir: \"{app}\"; Flags: ignoreversion\n" %f
  text = text.replace(";@MORE FILES ADD HERE",tmp)  

  f = open("%s_setup.iss"%target,"w")
  if f:
    f.write(text)
    f.close()  





def askNewVersion():
  if(ctypes.windll.user32.MessageBoxW(0, u"new version", u"Is it new version?",4) == YES):
    readVersion()  
    incrementVersion()
    saveVersionFiles()



def askCopyFiles():
  s= '\n'.join(update_files)
  if(ctypes.windll.user32.MessageBoxW(0, u"Copy new version file(s) to labo disk? \n%s"%s, u"new version",4) == YES):
    print "copying file to labo disk "
    for f in update_files:
      copyfile(f, "\\\\czbrsmb1\\labo\\sw\\#updates#\\%s"%os.path.basename(f))



def  findOutProjectName():
  global target
  global update_files
  project_file =""
  if len(glob.glob("*.pro"))>0:
    project_file = glob.glob("*.pro")[0]
  f = open(project_file, "rb")  
  if f:
    text = f.read()
    m = re.search("TARGET\\s+=\\s+(\\w+)", text, re.I | re.M)
    if m:
      target = m.group(1)
      update_files.append("bin\\%s.exe"%target)
      return   
  exit(-1)              



def askTakeFirmware():
  if len(FIRMWARE_FILE_PATH) >0:
    if(ctypes.windll.user32.MessageBoxW(0, u"Take firmware? \n %s"%FIRMWARE_FILE_PATH, u"new version",4) == YES):
      print "Ask take a firmware "
      copyfile(FIRMWARE_FILE_PATH, "firmware.hex")



def askMakeSetup():
  try:
    print "Making setup..."
    run_s = '%s /cc %s_setup.iss'%(INNOSETUP_PATH,target)
    print run_s
    return os.system(run_s)
  except:
    pass



if __name__ == "__main__":
  findOutProjectName()
  for par in sys.argv[1:]:
    if par == "version":
      askTakeFirmware()  
      askNewVersion()
      makeSetupIss()

    elif par == "copy":
      askCopyFiles()    
      askMakeSetup() 
 