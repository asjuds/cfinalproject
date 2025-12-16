@echo off
REM Stock Management System Launcher
REM This script sets up the environment and runs the application

REM Change to the directory where this batch file is located
cd /d "%~dp0"

REM Verify critical DLL exists
if not exist "libgtk-4-1.dll" (
    echo ERROR: libgtk-4-1.dll not found!
    echo Please make sure all files were extracted correctly.
    pause
    exit /b 1
)

REM Verify executable exists
if not exist "stock_manager.exe" (
    echo ERROR: stock_manager.exe not found!
    pause
    exit /b 1
)

REM Add current directory to PATH so Windows can find DLLs
REM This ensures DLLs in the same folder as the exe are found
set "PATH=%~dp0;%PATH%"

REM Set GTK environment variables
set "GTK_DATA_PREFIX=%~dp0"
set "GTK_EXE_PREFIX=%~dp0"
set "GTK_PATH=%~dp0"
set "GDK_PIXBUF_MODULEDIR=%~dp0loaders"
set "GDK_PIXBUF_MODULE_FILE=%~dp0loaders\loaders.cache"

REM Set GLib schemas path
if exist "%~dp0schemas" (
    set "GSETTINGS_SCHEMA_DIR=%~dp0schemas"
)

REM Create data folder if it doesn't exist
if not exist "%~dp0data" mkdir "%~dp0data"

REM Run the application from the current directory
REM Using start with /D ensures the working directory is set correctly
cd /d "%~dp0"
"%~dp0stock_manager.exe"

REM If we get here, the application exited
if errorlevel 1 (
    echo.
    echo Application exited with an error.
    echo Check that all DLL files are present in this folder.
    pause
)
