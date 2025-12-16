#include <gtk/gtk.h>
#include "model.h"
#include "storage.h"
#include "ui_main_window.h"

GPtrArray *products = NULL;
GPtrArray *history = NULL;

static void on_activate(GtkApplication *app, gpointer user_data) {
    products = g_ptr_array_new_with_free_func(g_free);
    history = g_ptr_array_new_with_free_func(g_free);

    GError *err = NULL;
    storage_load_products("data/products.csv", &err);
    if (err) {
        g_warning("Error loading products: %s", err->message);
        g_clear_error(&err);
    }
    storage_load_history("data/history.csv", &err);
    if (err) {
        g_warning("Error loading history: %s", err->message);
        g_clear_error(&err);
    }

    GtkWidget *window = ui_create_main_window(app);
    gtk_window_present(GTK_WINDOW(window));
}

static void on_shutdown(GApplication *app, gpointer user_data) {
    GError *err = NULL;
    storage_save_products("data/products.csv", &err);
    if (err) {
        g_warning("Error saving products: %s", err->message);
        g_clear_error(&err);
    }

    storage_save_history("data/history.csv", &err);
    if (err) {
        g_warning("Error saving history: %s", err->message);
        g_clear_error(&err);
    }

    if (products) {
        g_ptr_array_free(products, TRUE);
        products = NULL;
    }
    if (history) {
        g_ptr_array_free(history, TRUE);
        history = NULL;
    }
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.example.stockmanager",
                                              G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    g_signal_connect(app, "shutdown", G_CALLBACK(on_shutdown), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}


