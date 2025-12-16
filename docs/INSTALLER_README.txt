========================================
  Creating a Standalone Installer .exe
========================================

You have two options to create a standalone installer:

OPTION 1: Professional NSIS Installer (Recommended)
----------------------------------------------------
1. Download and install NSIS from: https://nsis.sourceforge.io/Download
2. Run: create_installer.bat
3. This creates: StockManagerSystem_Installer.exe
   - Professional installer with Start Menu shortcuts
   - Appears in Add/Remove Programs
   - Includes uninstaller
   - Installs to Program Files

OPTION 2: Simple Self-Extracting Archive
-----------------------------------------
1. Run: create_sfx_installer.bat
2. This creates: StockManagerSystem_Standalone.exe
   - Simple self-extracting archive
   - Extracts to AppData\Local\StockManager
   - Creates Desktop shortcut
   - No installation wizard

Both methods create a single .exe file that contains everything
and can be distributed to any Windows computer.

========================================

