

#define MyAppName                    "TEMPLATE"
#define MyAppVersion                 "0.0 "
#define MyAppPublisher               "ON Semiconductor"
#define MyAppURL                     "http://www.onsemi.com/"
#define MyAppExeName                 "TEMPLATE.exe"
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

Source: "bin\TEMPLATE.exe"; DestDir: "{app}"; Flags: ignoreversion

      

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{pf32}\IDC Tool\idc.exe" ; Parameters: {#MyAppExeName} /regserver;  StatusMsg: Registering ActiveX components...
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
