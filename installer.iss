#include "version.iss"
#define MyAppName "TextTempl"
#define MyAppExeName "TextTempl.exe"

[Files]
Source: release\{#MyAppExeName}; DestDir: {app}; Components: Base
Source: {#MyAppName}_*.qm; DestDir: {app}; Components: Base
Source: lib\*.dll; DestDir: {app}; Components: Base
Source: help\{#MyAppName}_*.qhc; DestDir: {app}; Components: Base
Source: help\{#MyAppName}_*.qch; DestDir: {app}; Components: Base
Source: help\assistant.exe; DestDir: {app}; Components: Base
Source: templates\UserConfig.templ; DestDir: {app}\templates; Components: Examples
[Setup]
MinVersion=0,5.01.2600
AppCopyright=©2010, SST Signal & System Technik GmbH
AppName={#MyAppName}
AppVerName={#MyAppName}-{#MyAppVersion}
PrivilegesRequired=none
DefaultDirName={pf}\{#MyAppName}
AppID={{EE99EB5D-F8BB-47AB-817B-08A74556B122}
LanguageDetectionMethod=locale
SetupIconFile=C:\Programme\Inno Setup 5\Examples\Setup.ico
WizardImageFile=C:\Programme\Inno Setup 5\WizModernImage-IS.bmp
WizardSmallImageFile=C:\Programme\Inno Setup 5\WizModernSmallImage-IS.bmp
OutputBaseFilename={#MyAppName}-{#MyAppVersion}-Setup
DefaultGroupName={#MyAppName}
UsePreviousGroup=false
AppendDefaultGroupName=false
DisableStartupPrompt=false
UsePreviousSetupType=false
ShowLanguageDialog=auto
ChangesAssociations=true
[LangOptions]
LanguageName=German
LanguageID=$0407
[Icons]
Name: {group}\{#MyAppName}; Filename: {app}\{#MyAppExeName}; IconFilename: {app}\{#MyAppExeName}; WorkingDir: {app}
Name: {commondesktop}\{#MyAppName}; Filename: {app}\{#MyAppExeName}; Tasks: desktopicon; WorkingDir: {app}
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}; Filename: {app}\{#MyAppExeName}; Tasks: quicklaunchicon
[Languages]
Name: German; MessagesFile: compiler:Languages\German.isl
[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked
Name: fileextension; Description: {cm:AssocFileExtension,TextTempl, *.templ}
[Components]
Name: Base; Description: Basis Programm; Flags: fixed; Types: compact custom full
Name: Examples; Description: Beispiele; Types: full
[Registry]
Root: HKCR; Subkey: .templ; ValueType: string; ValueName: ; ValueData: TextTemplFile; Flags: deletekey uninsdeletekey; Tasks: fileextension
Root: HKCR; Subkey: TextTemplFile; ValueType: string; ValueName: ; ValueData: TextTempl-File; Flags: deletekey uninsdeletekey; Tasks: fileextension
Root: HKCR; Subkey: TextTemplFile\DefaultIcon; ValueType: string; ValueName: ; ValueData: {app}\{#MyAppExeName}; Tasks: fileextension
Root: HKCR; Subkey: TextTemplFile\shell\open\command; ValueType: string; ValueData: """{app}\{#MyAppExeName}"" ""%1"""; Tasks: fileextension
Root: HKCR; Subkey: TextTemplFile\shell\open; ValueType: string; ValueName: ; ValueData: Öffnen mit TextTempl
