#include "ui_dialogs.h"
#include "logic.h"
#include "ui_main_window.h"

/* GTK4 removed gtk_dialog_run; implement a simple blocking runner. */
static void on_dialog_response(GtkDialog *dialog,
                               int response_id,
                               gpointer user_data) {
    int *resp = (int *)user_data;
    *resp = response_id;
}

static int run_dialog_blocking(GtkDialog *dialog) {
    int response = GTK_RESPONSE_NONE;
    g_signal_connect(dialog, "response",
                     G_CALLBACK(on_dialog_response), &response);
    gtk_widget_show(GTK_WIDGET(dialog));
    while (response == GTK_RESPONSE_NONE) {
        while (g_main_context_pending(NULL)) {
            g_main_context_iteration(NULL, TRUE);
        }
        g_main_context_iteration(NULL, TRUE);
    }
    return response;
}

static void show_error(GtkWindow *parent, const char *msg) {
    GtkWidget *d = gtk_message_dialog_new(parent,
                                          GTK_DIALOG_MODAL,
                                          GTK_MESSAGE_ERROR,
                                          GTK_BUTTONS_OK,
                                          "%s", msg);
    run_dialog_blocking(GTK_DIALOG(d));
    gtk_window_destroy(GTK_WINDOW(d));
}

static void show_info(GtkWindow *parent, const char *msg) {
    GtkWidget *d = gtk_message_dialog_new(parent,
                                          GTK_DIALOG_MODAL,
                                          GTK_MESSAGE_INFO,
                                          GTK_BUTTONS_OK,
                                          "%s", msg);
    run_dialog_blocking(GTK_DIALOG(d));
    gtk_window_destroy(GTK_WINDOW(d));
}

static GtkWidget *add_labeled_entry(GtkWidget *box, const char *label, GtkWidget **entry_out) {
    GtkWidget *h = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
    GtkWidget *lbl = gtk_label_new(label);
    GtkWidget *entry = gtk_entry_new();
    gtk_box_append(GTK_BOX(h), lbl);
    gtk_box_append(GTK_BOX(h), entry);
    gtk_box_append(GTK_BOX(box), h);
    if (entry_out) *entry_out = entry;
    return entry;
}

void ui_show_add_product_dialog(GtkWindow *parent) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Add Product",
                                                    parent,
                                                    GTK_DIALOG_MODAL,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    "_Add", GTK_RESPONSE_OK,
                                                    NULL);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_margin_top(vbox, 8);
    gtk_widget_set_margin_bottom(vbox, 8);
    gtk_widget_set_margin_start(vbox, 8);
    gtk_widget_set_margin_end(vbox, 8);
    gtk_box_append(GTK_BOX(content), vbox);

    GtkWidget *entry_id, *entry_name, *entry_cat, *entry_price, *entry_qty;
    add_labeled_entry(vbox, "ID:", &entry_id);
    add_labeled_entry(vbox, "Name:", &entry_name);
    add_labeled_entry(vbox, "Category:", &entry_cat);
    add_labeled_entry(vbox, "Price:", &entry_price);
    add_labeled_entry(vbox, "Quantity:", &entry_qty);

    int resp = run_dialog_blocking(GTK_DIALOG(dialog));
    if (resp == GTK_RESPONSE_OK) {
        const char *id = gtk_editable_get_text(GTK_EDITABLE(entry_id));
        const char *name = gtk_editable_get_text(GTK_EDITABLE(entry_name));
        const char *cat = gtk_editable_get_text(GTK_EDITABLE(entry_cat));
        const char *price_str = gtk_editable_get_text(GTK_EDITABLE(entry_price));
        const char *qty_str = gtk_editable_get_text(GTK_EDITABLE(entry_qty));

        GError *err = NULL;
        double price = g_ascii_strtod(price_str, NULL);
        int qty = (int)g_ascii_strtoll(qty_str, NULL, 10);

        if (!add_product(id, name, cat, price, qty, &err)) {
            show_error(parent, err->message);
            g_clear_error(&err);
        } else {
            show_info(parent, "Product added successfully.");
            ui_refresh_products_table();
            ui_refresh_history_view();
        }
    }

    gtk_window_destroy(GTK_WINDOW(dialog));
}

void ui_show_update_stock_dialog(GtkWindow *parent) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Update Stock",
                                                    parent,
                                                    GTK_DIALOG_MODAL,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    "_Update", GTK_RESPONSE_OK,
                                                    NULL);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_margin_top(vbox, 8);
    gtk_widget_set_margin_bottom(vbox, 8);
    gtk_widget_set_margin_start(vbox, 8);
    gtk_widget_set_margin_end(vbox, 8);
    gtk_box_append(GTK_BOX(content), vbox);

    GtkWidget *entry_id, *entry_qty;
    add_labeled_entry(vbox, "Product ID:", &entry_id);
    add_labeled_entry(vbox, "Quantity to add:", &entry_qty);

    int resp = run_dialog_blocking(GTK_DIALOG(dialog));
    if (resp == GTK_RESPONSE_OK) {
        const char *id = gtk_editable_get_text(GTK_EDITABLE(entry_id));
        const char *qty_str = gtk_editable_get_text(GTK_EDITABLE(entry_qty));
        int qty = (int)g_ascii_strtoll(qty_str, NULL, 10);

        GError *err = NULL;
        if (!update_stock(id, qty, &err)) {
            show_error(parent, err->message);
            g_clear_error(&err);
        } else {
            show_info(parent, "Stock updated successfully.");
            ui_refresh_products_table();
            ui_refresh_history_view();
        }
    }

    gtk_window_destroy(GTK_WINDOW(dialog));
}

void ui_show_sell_product_dialog(GtkWindow *parent) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Sell Product",
                                                    parent,
                                                    GTK_DIALOG_MODAL,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    "_Sell", GTK_RESPONSE_OK,
                                                    NULL);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_margin_top(vbox, 8);
    gtk_widget_set_margin_bottom(vbox, 8);
    gtk_widget_set_margin_start(vbox, 8);
    gtk_widget_set_margin_end(vbox, 8);
    gtk_box_append(GTK_BOX(content), vbox);

    GtkWidget *entry_id, *entry_qty;
    add_labeled_entry(vbox, "Product ID:", &entry_id);
    add_labeled_entry(vbox, "Quantity to sell:", &entry_qty);

    int resp = run_dialog_blocking(GTK_DIALOG(dialog));
    if (resp == GTK_RESPONSE_OK) {
        const char *id = gtk_editable_get_text(GTK_EDITABLE(entry_id));
        const char *qty_str = gtk_editable_get_text(GTK_EDITABLE(entry_qty));
        int qty = (int)g_ascii_strtoll(qty_str, NULL, 10);

        GError *err = NULL;
        double total = 0.0;
        if (!sell_product(id, qty, &total, &err)) {
            show_error(parent, err->message);
            g_clear_error(&err);
        } else {
            char msg[128];
            g_snprintf(msg, sizeof(msg), "Sale completed. Total = %.2f", total);
            show_info(parent, msg);
            ui_refresh_products_table();
            ui_refresh_history_view();
        }
    }

    gtk_window_destroy(GTK_WINDOW(dialog));
}

void ui_show_check_stock_dialog(GtkWindow *parent) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Check Stock Level",
                                                    parent,
                                                    GTK_DIALOG_MODAL,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    "_Check", GTK_RESPONSE_OK,
                                                    NULL);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_margin_top(vbox, 8);
    gtk_widget_set_margin_bottom(vbox, 8);
    gtk_widget_set_margin_start(vbox, 8);
    gtk_widget_set_margin_end(vbox, 8);
    gtk_box_append(GTK_BOX(content), vbox);

    GtkWidget *entry_id;
    add_labeled_entry(vbox, "Product ID:", &entry_id);

    int resp = run_dialog_blocking(GTK_DIALOG(dialog));
    if (resp == GTK_RESPONSE_OK) {
        const char *id = gtk_editable_get_text(GTK_EDITABLE(entry_id));
        int qty = 0;
        GError *err = NULL;
        int res = get_stock_level(id, &qty, &err);
        if (res < 0) {
            show_error(parent, err->message);
            g_clear_error(&err);
        } else {
            char msg[128];
            g_snprintf(msg, sizeof(msg), "Current stock for %s = %d", id, qty);
            if (qty < 5) {
                GtkWidget *d = gtk_message_dialog_new(parent,
                                                      GTK_DIALOG_MODAL,
                                                      GTK_MESSAGE_WARNING,
                                                      GTK_BUTTONS_OK,
                                                      "%s\nWarning: stock is below 5!", msg);
                run_dialog_blocking(GTK_DIALOG(d));
                gtk_window_destroy(GTK_WINDOW(d));
            } else {
                show_info(parent, msg);
            }
        }
    }

    gtk_window_destroy(GTK_WINDOW(dialog));
}

void ui_show_calculate_value_dialog(GtkWindow *parent) {
    double total = compute_total_stock_value();
    char msg[128];
    g_snprintf(msg, sizeof(msg), "Total stock value = %.2f", total);
    show_info(parent, msg);
}

void ui_show_apply_discount_dialog(GtkWindow *parent) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Apply Discount",
                                                    parent,
                                                    GTK_DIALOG_MODAL,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    "_Apply", GTK_RESPONSE_OK,
                                                    NULL);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_margin_top(vbox, 8);
    gtk_widget_set_margin_bottom(vbox, 8);
    gtk_widget_set_margin_start(vbox, 8);
    gtk_widget_set_margin_end(vbox, 8);
    gtk_box_append(GTK_BOX(content), vbox);

    GtkWidget *entry_id, *entry_qty, *entry_disc;
    add_labeled_entry(vbox, "Product ID:", &entry_id);
    add_labeled_entry(vbox, "Quantity:", &entry_qty);
    add_labeled_entry(vbox, "Discount % (10-20):", &entry_disc);

    int resp = run_dialog_blocking(GTK_DIALOG(dialog));
    if (resp == GTK_RESPONSE_OK) {
        const char *id = gtk_editable_get_text(GTK_EDITABLE(entry_id));
        const char *qty_str = gtk_editable_get_text(GTK_EDITABLE(entry_qty));
        const char *disc_str = gtk_editable_get_text(GTK_EDITABLE(entry_disc));
        int qty = (int)g_ascii_strtoll(qty_str, NULL, 10);
        double disc = g_ascii_strtod(disc_str, NULL);

        GError *err = NULL;
        double discounted = 0.0;
        if (!apply_discount(id, qty, disc, &discounted, &err)) {
            show_error(parent, err->message);
            g_clear_error(&err);
        } else {
            char msg[128];
            g_snprintf(msg, sizeof(msg),
                       "Discounted total for %d units = %.2f", qty, discounted);
            show_info(parent, msg);
            ui_refresh_history_view();
        }
    }

    gtk_window_destroy(GTK_WINDOW(dialog));
}

void ui_show_remove_product_dialog(GtkWindow *parent) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Remove Product",
                                                    parent,
                                                    GTK_DIALOG_MODAL,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    "_Next", GTK_RESPONSE_OK,
                                                    NULL);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_margin_top(vbox, 8);
    gtk_widget_set_margin_bottom(vbox, 8);
    gtk_widget_set_margin_start(vbox, 8);
    gtk_widget_set_margin_end(vbox, 8);
    gtk_box_append(GTK_BOX(content), vbox);

    GtkWidget *entry_id;
    add_labeled_entry(vbox, "Product ID:", &entry_id);

    int resp = run_dialog_blocking(GTK_DIALOG(dialog));
    if (resp == GTK_RESPONSE_OK) {
        const char *id = gtk_editable_get_text(GTK_EDITABLE(entry_id));
        GtkWidget *confirm = gtk_message_dialog_new(parent,
                                                    GTK_DIALOG_MODAL,
                                                    GTK_MESSAGE_QUESTION,
                                                    GTK_BUTTONS_YES_NO,
                                                    "Are you sure you want to remove product %s?",
                                                    id);
        int c = run_dialog_blocking(GTK_DIALOG(confirm));
        gtk_window_destroy(GTK_WINDOW(confirm));
        if (c == GTK_RESPONSE_YES) {
            GError *err = NULL;
            if (!remove_product(id, &err)) {
                show_error(parent, err->message);
                g_clear_error(&err);
            } else {
                show_info(parent, "Product removed.");
                ui_refresh_products_table();
                ui_refresh_history_view();
            }
        }
    }

    gtk_window_destroy(GTK_WINDOW(dialog));
}

void ui_show_report_window(GtkWindow *parent) {
    GtkWidget *win = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(win), "Stock Report");
    gtk_window_set_transient_for(GTK_WINDOW(win), parent);
    gtk_window_set_modal(GTK_WINDOW(win), TRUE);
    gtk_window_set_default_size(GTK_WINDOW(win), 400, 200);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_top(vbox, 12);
    gtk_widget_set_margin_bottom(vbox, 12);
    gtk_widget_set_margin_start(vbox, 12);
    gtk_widget_set_margin_end(vbox, 12);
    gtk_window_set_child(GTK_WINDOW(win), vbox);

    int total_products = (int)products->len;
    double stock_value = compute_total_stock_value();

    int total_sold = 0;
    Product *most_active = NULL;
    for (guint i = 0; i < products->len; i++) {
        Product *p = g_ptr_array_index(products, i);
        total_sold += p->sold;
        if (!most_active || p->sold > most_active->sold) {
            most_active = p;
        }
    }

    char buf[256];

    g_snprintf(buf, sizeof(buf), "Total products: %d", total_products);
    GtkWidget *lbl1 = gtk_label_new(buf);
    gtk_box_append(GTK_BOX(vbox), lbl1);

    g_snprintf(buf, sizeof(buf), "Total stock value: %.2f", stock_value);
    GtkWidget *lbl2 = gtk_label_new(buf);
    gtk_box_append(GTK_BOX(vbox), lbl2);

    g_snprintf(buf, sizeof(buf), "Total stock sold: %d", total_sold);
    GtkWidget *lbl3 = gtk_label_new(buf);
    gtk_box_append(GTK_BOX(vbox), lbl3);

    if (most_active) {
        g_snprintf(buf, sizeof(buf), "Most active product: %s (sold %d)",
                   most_active->name, most_active->sold);
    } else {
        g_snprintf(buf, sizeof(buf), "Most active product: N/A");
    }
    GtkWidget *lbl4 = gtk_label_new(buf);
    gtk_box_append(GTK_BOX(vbox), lbl4);

    GtkWidget *close_btn = gtk_button_new_with_label("Close");
    gtk_box_append(GTK_BOX(vbox), close_btn);
    g_signal_connect_swapped(close_btn, "clicked",
                             G_CALLBACK(gtk_window_destroy), win);

    gtk_widget_show(win);
}


