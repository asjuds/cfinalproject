#include <gtk/gtk.h>
#include "model.h"
#include "storage.h"
#include "ui_main_window.h"

GPtrArray *products = NULL;
GPtrArray *history = NULL;

static void setup_css(void) {
    const char *css =
        "window {"
        "  background-color: #ffffff;"
        "}"
        "box {"
        "  background-color: #f8f9fa;"
        "}"
        "button {"
        "  padding: 6px 14px;"
        "}"
        "button.suggested-action {"
        "  background: #2d8cff;"
        "  color: white;"
        "}"
        "button.destructive-action {"
        "  background: #ff4b4b;"
        "  color: white;"
        "}"
        "label.section-title {"
        "  font-weight: bold;"
        "  padding: 4px 0;"
        "}";

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1);
    GdkDisplay *display = gdk_display_get_default();
    if (display) {
        gtk_style_context_add_provider_for_display(
            display,
            GTK_STYLE_PROVIDER(provider),
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
    g_object_unref(provider);
}

static void on_activate(GtkApplication *app, gpointer user_data) {
    /* We manage element freeing manually on shutdown to avoid double-free */
    products = g_ptr_array_new();
    history = g_ptr_array_new();

    setup_css();

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
        for (guint i = 0; i < products->len; i++) {
            g_free(g_ptr_array_index(products, i));
        }
        g_ptr_array_free(products, TRUE);
        products = NULL;
    }
    if (history) {
        for (guint i = 0; i < history->len; i++) {
            g_free(g_ptr_array_index(history, i));
        }
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


