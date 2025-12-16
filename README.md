## Stock Management System (GTK4, C)

This is a GUI-based stock management system written in C using GTK4 and GLib.
It supports product management, stock operations, reporting, and CSV-based
persistent storage.

### Features

- **Add Product**: Register products with ID, name, category, unit price, and initial quantity.
- **View Products**: Products shown in a sortable table; quantity \< 5 is highlighted in red.
- **Update Stock**: Increase quantity for an existing product (must be a positive integer).
- **Sell Product**: Sell any positive quantity, with stock checks and total sale calculation.
- **Check Stock Level**: Show current stock; warns when quantity \< 5.
- **Calculate Stock Value**: Computes sum of `price * quantity` for all products.
- **Apply Discount**: Apply 10–20% discount to a given product/quantity and show discounted total.
- **Remove Product**: Delete a product after confirmation; table and history update.
- **Stock History**: All operations (add, update, sell, discount, remove) logged chronologically.
- **Generate Report**: Shows total products, overall stock value, total sold quantity, and most active product.

### Data model and storage

- `Product`:
  - `id`, `name`, `category`, `price`, `quantity`, `sold`.
- `HistoryEntry`:
  - `timestamp`, `operation`, `product_id`, `quantity_change`, `value_change`, `description`.
- Stored in memory as `GPtrArray *products` and `GPtrArray *history`.
- Persisted in CSV files:
  - `data/products.csv`: `id,name,category,price,quantity,sold`
  - `data/history.csv`: `timestamp,operation,product_id,quantity_change,value_change,description`

### Source files

- `src/main.c` – `GtkApplication` entry point, CSS styling, startup/shutdown, load/save and cleanup.
- `src/model.h` – Structs for `Product` and `HistoryEntry`, and global `GPtrArray` declarations.
- `src/storage.c/.h` – CSV load/save for products and history.
- `src/logic.c/.h` – Business logic: add/update/sell/remove/discount, stock value, history recording.
- `src/ui_main_window.c/.h` – Main window, toolbar buttons, products table, history table, styling hooks.
- `src/ui_dialogs.c/.h` – Dialogs for all operations and the report window.

### Building and running (MSYS2 UCRT64 on Windows)

1. Install MSYS2 from `https://www.msys2.org` and open **MSYS2 UCRT64**.
2. Install toolchain and GTK4 (one-time):

   ```bash
   pacman -Syu
   pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-gtk4
   ```

3. Build and run:

   ```bash
   cd /c/Users/ALL\ Atoz/OneDrive/Desktop/cfinalproject
   make
   ./stock_manager.exe
   ```

### How to use (GUI flow)

1. **Add products** first using the **Add Product** button.
2. Use **Update Stock**, **Sell**, and **Apply Discount** as needed.
3. Check low inventory with **Check Stock** and the red-highlighted quantities.
4. Use **Generate Report** to view a summary of current stock and sales.
5. Close the app; data is automatically saved to CSV files for the next run.


