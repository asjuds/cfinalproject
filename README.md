# Stock Management System

A complete GUI-based stock management application built with C and GTK4.

## Features

- **Product Management**: Add, update, remove products with ID, name, category, price, and quantity
- **Stock Operations**: Update stock levels, sell products with quantity validation
- **Stock Monitoring**: View current stock with low stock warnings (quantity < 5 highlighted in red)
- **Value Calculation**: Calculate total inventory value
- **Discount System**: Apply discounts (10-20%) to sales
- **History Tracking**: Complete log of all operations (add, sell, update, remove)
- **Data Persistence**: Automatic CSV file storage for products and history
- **Sortable Tables**: Click column headers to sort products
- **Modern UI**: Beautiful GTK4 interface with custom CSS styling

## Quick Start

### For End Users

**Option 1: Installer (Recommended)**
1. Download `StockManager_Installer.exe`
2. Double-click to install
3. Run from Start Menu or Desktop shortcut

**Option 2: Portable Package**
1. Extract `StockManagerPackage` folder
2. Double-click `RunStockManager.bat`
3. Application runs immediately

### For Developers

#### Prerequisites
- MSYS2 with UCRT64 environment
- GTK4 development libraries
- GCC compiler

#### Installation
```bash
# Install GTK4 (in MSYS2 UCRT64)
pacman -S mingw-w64-ucrt-x86_64-gtk4
```

#### Building
```bash
make clean
make
```

#### Running
```bash
./stock_manager.exe
```

## Project Structure

```
cfinalproject/
├── src/                    # Source code
├── docs/                   # Documentation
├── scripts/                # Utility scripts
│   ├── package.sh         # Create distribution package
│   └── create_installer_direct.bat  # Create installer
├── data/                   # Application data (CSV files)
├── StockManagerPackage/    # Distribution package
├── Makefile                # Build configuration
├── installer.nsi           # NSIS installer script
└── StockManager_Installer.exe  # Final installer (30 MB)
```

## Creating Distribution

### Create Package Folder
```bash
./scripts/package.sh
```

### Create Installer
```bash
# Requires NSIS installed
.\scripts\create_installer_direct.bat
```

## Documentation

- [Project Structure](docs/PROJECT_STRUCTURE.md) - Detailed project organization
- [Distribution Guide](docs/DISTRIBUTION_README.md) - Distribution instructions

## License

Educational purposes.

## Version

1.0 - December 2025
