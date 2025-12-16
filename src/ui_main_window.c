#include "ui_main_window.h"
#include "ui_dialogs.h"
#include "model.h"

GPtrArray *products;
GPtrArray *history;

static GtkListStore *products_store = NULL;
static GtkListStore *history_store = NULL;
static GtkTreeView *products_view = NULL;
static GtkTreeView *history_view = NULL;

enum {
    COL_ID,
    COL_NAME,
    COL_CATEGORY,
    COL_QUANTITY,
    COL_PRICE,
    COL_SOLD,
    COL_COLOR,
    N_COLS
};

enum {
    H_COL_TIME,
    H_COL_OP,
    H_COL_PID,
    H_COL_QTY,
    H_COL_VAL,
    H_COL_DESC,
    H_N_COLS
};

static void quantity_cell_data_func(GtkTreeViewColumn *column,
                                    GtkCellRenderer *renderer,
                                    GtkTreeModel *model,
                                    GtkTreeIter *iter,
                                    gpointer data) {
    gint quantity;
    gtk_tree_model_get(model, iter, COL_QUANTITY, &quantity, -1);
    if (quantity < 5) {
        g_object_set(renderer, "foreground", "red", NULL);
    } else {
        g_object_set(renderer, "foreground", NULL, NULL);
    }
}

void ui_refresh_products_table(void) {
    gtk_list_store_clear(products_store);
    for (guint i = 0; i < products->len; i++) {
        Product *p = g_ptr_array_index(products, i);
        GtkTreeIter iter;
        gtk_list_store_append(products_store, &iter);
        gtk_list_store_set(products_store, &iter,
                           COL_ID, p->id,
                           COL_NAME, p->name,
                           COL_CATEGORY, p->category,
                           COL_QUANTITY, p->quantity,
                           COL_PRICE, p->price,
                           COL_SOLD, p->sold,
                           COL_COLOR, "",
                           -1);
    }
}

void ui_refresh_history_view(void) {
    gtk_list_store_clear(history_store);
    for (guint i = 0; i < history->len; i++) {
        HistoryEntry *h = g_ptr_array_index(history, i);
        GtkTreeIter iter;
        char time_buf[64];
        struct tm *tm_info = localtime(&h->timestamp);
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
        gtk_list_store_append(history_store, &iter);
        gtk_list_store_set(history_store, &iter,
                           H_COL_TIME, time_buf,
                           H_COL_OP, h->operation,
                           H_COL_PID, h->product_id,
                           H_COL_QTY, h->quantity_change,
                           H_COL_VAL, h->value_change,
                           H_COL_DESC, h->description,
                           -1);
    }
}

char *ui_get_selected_product_id(void) {
    GtkTreeSelection *sel = gtk_tree_view_get_selection(products_view);
    GtkTreeModel *model;
    GtkTreeIter iter;
    if (gtk_tree_selection_get_selected(sel, &model, &iter)) {
        char *id = NULL;
        gtk_tree_model_get(model, &iter, COL_ID, &id, -1);
        return id;
    }
    return NULL;
}

static void on_add_product_clicked(GtkButton *btn, gpointer user_data) {
    GtkWindow *win = GTK_WINDOW(user_data);
    ui_show_add_product_dialog(win);
}

static void on_update_stock_clicked(GtkButton *btn, gpointer user_data) {
    GtkWindow *win = GTK_WINDOW(user_data);
    ui_show_update_stock_dialog(win);
}

static void on_sell_product_clicked(GtkButton *btn, gpointer user_data) {
    GtkWindow *win = GTK_WINDOW(user_data);
    ui_show_sell_product_dialog(win);
}

static void on_check_stock_clicked(GtkButton *btn, gpointer user_data) {
    GtkWindow *win = GTK_WINDOW(user_data);
    ui_show_check_stock_dialog(win);
}

static void on_calc_value_clicked(GtkButton *btn, gpointer user_data) {
    GtkWindow *win = GTK_WINDOW(user_data);
    ui_show_calculate_value_dialog(win);
}

static void on_apply_discount_clicked(GtkButton *btn, gpointer user_data) {
    GtkWindow *win = GTK_WINDOW(user_data);
    ui_show_apply_discount_dialog(win);
}

static void on_remove_product_clicked(GtkButton *btn, gpointer user_data) {
    GtkWindow *win = GTK_WINDOW(user_data);
    ui_show_remove_product_dialog(win);
}

static void on_generate_report_clicked(GtkButton *btn, gpointer user_data) {
    GtkWindow *win = GTK_WINDOW(user_data);
    ui_show_report_window(win);
}

GtkWidget *ui_create_main_window(GtkApplication *app) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Stock Management System");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 600);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    GtkWidget *toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
    gtk_box_append(GTK_BOX(vbox), toolbar);

    struct {
        const char *label;
        GCallback cb;
    } buttons[] = {
        { "Add Product",        G_CALLBACK(on_add_product_clicked) },
        { "Update Stock",       G_CALLBACK(on_update_stock_clicked) },
        { "Sell",               G_CALLBACK(on_sell_product_clicked) },
        { "Check Stock",        G_CALLBACK(on_check_stock_clicked) },
        { "Calculate Value",    G_CALLBACK(on_calc_value_clicked) },
        { "Apply Discount",     G_CALLBACK(on_apply_discount_clicked) },
        { "Remove Product",     G_CALLBACK(on_remove_product_clicked) },
        { "Generate Report",    G_CALLBACK(on_generate_report_clicked) }
    };

    for (guint i = 0; i < G_N_ELEMENTS(buttons); i++) {
        GtkWidget *btn = gtk_button_new_with_label(buttons[i].label);
        gtk_box_append(GTK_BOX(toolbar), btn);
        g_signal_connect(btn, "clicked", buttons[i].cb, window);
    }

    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_VERTICAL);
    gtk_box_append(GTK_BOX(vbox), paned);

    products_store = gtk_list_store_new(N_COLS,
                                        G_TYPE_STRING,
                                        G_TYPE_STRING,
                                        G_TYPE_STRING,
                                        G_TYPE_INT,
                                        G_TYPE_DOUBLE,
                                        G_TYPE_INT,
                                        G_TYPE_STRING);

    products_view = GTK_TREE_VIEW(gtk_tree_view_new_with_model(
                                      GTK_TREE_MODEL(products_store)));
    gtk_tree_view_set_headers_clickable(products_view, TRUE);

    GtkCellRenderer *renderer;
    GtkTreeViewColumn *col;

    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", COL_ID, NULL);
    gtk_tree_view_column_set_sort_column_id(col, COL_ID);
    gtk_tree_view_append_column(products_view, col);

    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("Name", renderer, "text", COL_NAME, NULL);
    gtk_tree_view_column_set_sort_column_id(col, COL_NAME);
    gtk_tree_view_append_column(products_view, col);

    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("Category", renderer, "text", COL_CATEGORY, NULL);
    gtk_tree_view_column_set_sort_column_id(col, COL_CATEGORY);
    gtk_tree_view_append_column(products_view, col);

    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("Quantity", renderer, "text", COL_QUANTITY, NULL);
    gtk_tree_view_column_set_cell_data_func(col, renderer, quantity_cell_data_func, NULL, NULL);
    gtk_tree_view_column_set_sort_column_id(col, COL_QUANTITY);
    gtk_tree_view_append_column(products_view, col);

    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("Price", renderer, "text", COL_PRICE, NULL);
    gtk_tree_view_column_set_sort_column_id(col, COL_PRICE);
    gtk_tree_view_append_column(products_view, col);

    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("Sold", renderer, "text", COL_SOLD, NULL);
    gtk_tree_view_column_set_sort_column_id(col, COL_SOLD);
    gtk_tree_view_append_column(products_view, col);

    GtkWidget *scroll_products = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll_products),
                                  GTK_WIDGET(products_view));
    gtk_paned_set_start_child(GTK_PANED(paned), scroll_products);

    history_store = gtk_list_store_new(H_N_COLS,
                                       G_TYPE_STRING,
                                       G_TYPE_STRING,
                                       G_TYPE_STRING,
                                       G_TYPE_INT,
                                       G_TYPE_DOUBLE,
                                       G_TYPE_STRING);

    history_view = GTK_TREE_VIEW(gtk_tree_view_new_with_model(
                                     GTK_TREE_MODEL(history_store)));

    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("Time", renderer, "text", H_COL_TIME, NULL);
    gtk_tree_view_append_column(history_view, col);

    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("Op", renderer, "text", H_COL_OP, NULL);
    gtk_tree_view_append_column(history_view, col);

    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("Product ID", renderer, "text", H_COL_PID, NULL);
    gtk_tree_view_append_column(history_view, col);

    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("Qty", renderer, "text", H_COL_QTY, NULL);
    gtk_tree_view_append_column(history_view, col);

    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("Value", renderer, "text", H_COL_VAL, NULL);
    gtk_tree_view_append_column(history_view, col);

    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("Description", renderer, "text", H_COL_DESC, NULL);
    gtk_tree_view_append_column(history_view, col);

    GtkWidget *scroll_history = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll_history),
                                  GTK_WIDGET(history_view));
    gtk_paned_set_end_child(GTK_PANED(paned), scroll_history);

    ui_refresh_products_table();
    ui_refresh_history_view();

    return window;
}


