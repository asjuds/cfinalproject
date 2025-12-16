@echo off
REM Create installer - tries to find NSIS automatically

echo Creating Professional Windows Installer...
echo.

cd /d "%~dp0"

REM Try to find NSIS
set "NSIS_PATH="
if exist "C:\Program Files (x86)\NSIS\makensis.exe" (
    set "NSIS_PATH=C:\Program Files (x86)\NSIS\makensis.exe"
) else if exist "C:\Program Files\NSIS\makensis.exe" (
    set "NSIS_PATH=C:\Program Files\NSIS\makensis.exe"
) else (
    where makensis >nul 2>&1
    if not errorlevel 1 (
        set "NSIS_PATH=makensis"
    )
)

if "%NSIS_PATH%"=="" (
    echo ERROR: Could not find NSIS!
    echo.
    echo Please either:
    echo   1. Add NSIS to your PATH, OR
    echo   2. Edit this script and set NSIS_PATH to your NSIS location
    echo.
    pause
    exit /b 1
)

REM Check if package exists
if not exist "StockManagerPackage\stock_manager.exe" (
    echo ERROR: StockManagerPackage\stock_manager.exe not found!
    pause
    exit /b 1
)

echo Found NSIS at: %NSIS_PATH%
echo.
echo Compiling installer...
echo.

"%NSIS_PATH%" installer.nsi

if exist "StockManager_Installer.exe" (
    echo.
    echo SUCCESS! Installer created: StockManager_Installer.exe
    echo.
    dir StockManager_Installer.exe
    echo.
    echo Ready for distribution!
) else (
    echo.
    echo ERROR: Failed to create installer!
    echo Check the NSIS output above for errors.
)

pause

