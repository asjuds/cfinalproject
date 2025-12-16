; Stock Management System - Professional Windows Installer
; NSIS Installer Script
; This creates a proper Windows installer .exe

;--------------------------------
; Include Modern UI

!include "MUI2.nsh"

;--------------------------------
; General

; Name and file
Name "Stock Management System"
OutFile "StockManager_Installer.exe"
Unicode True

; Default installation folder
InstallDir "$PROGRAMFILES\StockManager"

; Get installation folder from registry if available
InstallDirRegKey HKCU "Software\StockManager" ""

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------
; Variables

Var StartMenuFolder

;--------------------------------
; Interface Settings

!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

;--------------------------------
; Pages

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "StockManagerPackage\README.txt"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY

;Start Menu Folder Page Configuration
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\StockManager"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"

!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder

!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

;--------------------------------
; Languages

!insertmacro MUI_LANGUAGE "English"

;--------------------------------
; Installer Sections

Section "Stock Management System" SecMain

    SectionIn RO

    ; Set output path to the installation directory
    SetOutPath "$INSTDIR"

    ; Copy all files from StockManagerPackage
    File /r "StockManagerPackage\*.*"

    ; Store installation folder
    WriteRegStr HKCU "Software\StockManager" "" $INSTDIR

    ; Create uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"

    ; Create shortcuts (using VBS launcher to avoid terminal window)
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
        CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
        CreateShortcut "$SMPROGRAMS\$StartMenuFolder\Stock Manager.lnk" "$INSTDIR\StockManager.vbs" "" "$INSTDIR\stock_manager.exe" 0
        CreateShortcut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
        CreateShortcut "$DESKTOP\Stock Manager.lnk" "$INSTDIR\StockManager.vbs" "" "$INSTDIR\stock_manager.exe" 0
    !insertmacro MUI_STARTMENU_WRITE_END

    ; Add to Add/Remove Programs
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\StockManager" \
                     "DisplayName" "Stock Management System"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\StockManager" \
                     "UninstallString" "$\"$INSTDIR\Uninstall.exe$\""
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\StockManager" \
                     "InstallLocation" "$INSTDIR"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\StockManager" \
                     "Publisher" "Stock Manager"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\StockManager" \
                     "DisplayVersion" "1.0"
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\StockManager" \
                       "NoModify" 1
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\StockManager" \
                       "NoRepair" 1

SectionEnd

;--------------------------------
; Descriptions

; Language strings
LangString DESC_SecMain ${LANG_ENGLISH} "Stock Management System - Complete installation with all required files."

; Assign language strings to sections
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecMain} $(DESC_SecMain)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
; Uninstaller Section

Section "Uninstall"

    ; Remove files and directories
    RMDir /r "$INSTDIR"

    ; Remove shortcuts
    !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    Delete "$SMPROGRAMS\$StartMenuFolder\Stock Manager.lnk"
    Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
    RMDir "$SMPROGRAMS\$StartMenuFolder"
    Delete "$DESKTOP\Stock Manager.lnk"

    ; Remove registry keys
    DeleteRegKey /ifempty HKCU "Software\StockManager"
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\StockManager"

SectionEnd

