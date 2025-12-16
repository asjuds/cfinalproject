#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

#include <gtk/gtk.h>

GtkWidget *ui_create_main_window(GtkApplication *app);

void ui_refresh_products_table(void);
void ui_refresh_history_view(void);

char *ui_get_selected_product_id(void);

#endif /* UI_MAIN_WINDOW_H */


