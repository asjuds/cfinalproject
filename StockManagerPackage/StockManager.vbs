' Stock Management System Launcher (No Terminal Window)
' This VBScript runs the application without showing a command prompt

Set WshShell = CreateObject("WScript.Shell")
Set fso = CreateObject("Scripting.FileSystemObject")

' Get the directory where this script is located
ScriptPath = fso.GetParentFolderName(WScript.ScriptFullName)

' Change to the script directory
WshShell.CurrentDirectory = ScriptPath

' Add current directory to PATH
Set env = WshShell.Environment("PROCESS")
env("PATH") = ScriptPath & ";" & env("PATH")

' Set GTK environment variables
env("GTK_DATA_PREFIX") = ScriptPath
env("GTK_EXE_PREFIX") = ScriptPath
env("GTK_PATH") = ScriptPath
env("GDK_PIXBUF_MODULEDIR") = ScriptPath & "\loaders"
env("GDK_PIXBUF_MODULE_FILE") = ScriptPath & "\loaders\loaders.cache"

If fso.FolderExists(ScriptPath & "\schemas") Then
    env("GSETTINGS_SCHEMA_DIR") = ScriptPath & "\schemas"
End If

' Create data folder if it doesn't exist
If Not fso.FolderExists(ScriptPath & "\data") Then
    fso.CreateFolder(ScriptPath & "\data")
End If

' Run the application (hidden window)
WshShell.Run """" & ScriptPath & "\stock_manager.exe""", 0, False

