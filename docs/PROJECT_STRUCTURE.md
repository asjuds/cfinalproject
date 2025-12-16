# Stock Management System - Project Structure

## Overview
This is a GTK4-based Stock Management System written in C. The application provides a complete GUI for managing product inventory, sales, and stock history.

## Directory Structure

```
cfinalproject/
├── src/                    # Source code files
│   ├── main.c             # Application entry point
│   ├── model.h            # Data structures (Product, HistoryEntry)
│   ├── storage.c/h        # CSV file I/O operations
│   ├── logic.c/h          # Business logic (add, sell, update, etc.)
│   ├── ui_main_window.c/h # Main window UI
│   └── ui_dialogs.c/h     # Dialog windows
│
├── docs/                   # Documentation
│   ├── PROJECT_STRUCTURE.md    # This file
│   ├── DISTRIBUTION_README.md   # Distribution instructions
│   ├── INSTALLER_README.txt     # Installer creation guide
│   └── COPY_DLLS_INSTRUCTIONS.txt # DLL copying guide
│
├── scripts/                # Utility scripts
│   ├── package.sh         # Create distribution package
│   └── create_final_installer.bat # Create standalone installer
│
├── data/                   # Application data (CSV files)
│   ├── products.csv       # Product database
│   └── history.csv        # Operation history
│
├── StockManagerPackage/    # Final distribution package
│   ├── stock_manager.exe   # Main executable
│   ├── RunStockManager.bat # Launcher script
│   ├── *.dll              # GTK4 dependencies (78 DLLs)
│   ├── loaders/           # GTK4 image loaders
│   ├── schemas/           # GTK4 configuration schemas
│   ├── icons/             # GTK4 icon themes
│   ├── data/              # Data folder
│   └── README.txt         # User instructions
│
├── Makefile               # Build configuration
├── README.md              # Main project documentation
├── stock_manager.exe      # Compiled executable (development)
└── StockManagerSystem_Installer.exe # Standalone installer (final distribution)
```

## Building the Project

### Prerequisites
- MSYS2 with UCRT64 environment
- GTK4 development libraries
- GCC compiler
- Make

### Build Steps
```bash
# In MSYS2 UCRT64 terminal
make clean
make
```

This creates `stock_manager.exe` in the project root.

## Creating Distribution Package

### Option 1: Create Package Folder
```bash
./scripts/package.sh
```
Creates `StockManagerPackage/` with all required files.

### Option 2: Create Standalone Installer
```bash
# In Windows PowerShell or Command Prompt
.\scripts\create_final_installer.bat
```
Creates `StockManagerSystem_Installer.exe` - a single-file installer.

## Distribution

### For End Users
Distribute `StockManagerSystem_Installer.exe`:
- Single file (30 MB)
- Extracts to `%LOCALAPPDATA%\StockManager`
- Creates Desktop shortcut
- No installation of dependencies required

### For Developers
- Source code in `src/`
- Build with `make`
- Package with `scripts/package.sh`

## File Descriptions

### Source Files
- **main.c**: Application initialization, GTK setup, data loading/saving
- **model.h**: Data structures for Product and HistoryEntry
- **storage.c/h**: CSV file operations for persistence
- **logic.c/h**: Business logic functions (validation, calculations)
- **ui_main_window.c/h**: Main window with product/history tables
- **ui_dialogs.c/h**: Dialog windows for user input

### Key Features
- Product registration and management
- Stock updates and sales tracking
- Low stock warnings (quantity < 5 highlighted in red)
- Stock value calculation
- Discount application (10-20%)
- Complete operation history
- CSV-based data persistence
- Sortable product table
- Beautiful GTK4 UI with CSS styling

## Data Storage
- Products: `data/products.csv`
- History: `data/history.csv`
- Format: CSV (Comma Separated Values)
- Auto-saved on application close

## Notes
- Object files (`.o`) are generated during build and can be cleaned with `make clean`
- The `StockManagerPackage/` folder contains the complete portable distribution
- The installer (`StockManagerSystem_Installer.exe`) is the recommended distribution method

