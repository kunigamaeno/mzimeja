; Installer Settings for Inno Setup
; (Release Version)

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{2FCD745D-F3C2-4115-B537-D6AE6E066B82}
AppName=MZ-IME日本語入力
AppVerName=MZ-IME日本語入力 0.4
AppPublisher=片山博文MZ
AppPublisherURL=http://katahiromz.web.fc2.com/
AppSupportURL=http://katahiromz.web.fc2.com/
AppUpdatesURL=http://katahiromz.web.fc2.com/
DefaultDirName={pf}\mzimeja
DefaultGroupName=MZ-IME日本語入力
OutputDir=.
OutputBaseFilename=mzimeja-0.4-setup
Compression=lzma
SolidCompression=yes
VersionInfoVersion=0.4
VersionInfoTextVersion=0.4

[Languages]
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"

[Files]
Source: "READMEJP.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "LICENSE.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "res\mzimeja.dic"; DestDir: "{app}\res"; Flags: ignoreversion
Source: "res\name.dic"; DestDir: "{app}\res"; Flags: ignoreversion
Source: "res\kanji.dat"; DestDir: "{app}\res"; Flags: ignoreversion
Source: "res\radical.dat"; DestDir: "{app}\res"; Flags: ignoreversion
Source: "projects\Release\mzimeja.ime"; DestDir: "{app}"; Flags: ignoreversion
Source: "projects\Release\setup.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "projects\Release\imepad.exe"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\READMEJP.txt"; Filename: "{app}\READMEJP.txt"
Name: "{group}\LICENSE.txt"; Filename: "{app}\LICENSE.txt"
Name: "{group}\MZ-IMEパッド"; Filename: "{app}\imepad.exe"
Name: "{group}\アンインストール"; Filename: "{uninstallexe}"

[Run]
Filename: "{app}\setup.exe"; Parameters: "/i"

[UninstallRun]
Filename: "{app}\setup.exe"; Parameters: "/u"
