#ifndef UI_DIALOGS_H
#define UI_DIALOGS_H

#include <gtk/gtk.h>

void ui_show_add_product_dialog(GtkWindow *parent);
void ui_show_update_stock_dialog(GtkWindow *parent);
void ui_show_sell_product_dialog(GtkWindow *parent);
void ui_show_check_stock_dialog(GtkWindow *parent);
void ui_show_calculate_value_dialog(GtkWindow *parent);
void ui_show_apply_discount_dialog(GtkWindow *parent);
void ui_show_remove_product_dialog(GtkWindow *parent);
void ui_show_report_window(GtkWindow *parent);

#endif /* UI_DIALOGS_H */


