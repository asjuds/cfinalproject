#include <gtk/gtk.h>
#include "model.h"
#include "storage.h"
#include "ui_main_window.h"

/* This is the main file - it starts everything */
/* It loads data, shows the window, and saves data when closing */

/* These are the global arrays that hold all our data */
/* I put them here so every file can use them */
GPtrArray *products = NULL;  /* List of all products */
GPtrArray *history = NULL;   /* List of all history */

/* This function sets up the colors and styling */
/* CSS is like HTML styling - makes things look nice */
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

/* This function runs when the app starts */
/* It loads data from files and shows the window */
static void on_activate(GtkApplication *app, gpointer user_data) {
    /* Create empty lists for products and history */
    products = g_ptr_array_new();
    history = g_ptr_array_new();

    /* Set up the colors and styling */
    setup_css();

    /* Try to load data from CSV files */
    /* If files don't exist, that's OK - first time running */
    /* Create data folder if it doesn't exist */
    g_mkdir_with_parents("data", 0755);
    
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

    /* Create the main window and show it */
    GtkWidget *window = ui_create_main_window(app);
    gtk_window_present(GTK_WINDOW(window));
}

/* This function runs when the app closes */
/* It saves everything to files and frees memory */
static void on_shutdown(GApplication *app, gpointer user_data) {
    /* Save all data to CSV files so we don't lose it */
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

    /* Free all the product memory */
    if (products) {
        for (guint i = 0; i < products->len; i++) {
            g_free(g_ptr_array_index(products, i));  /* Free each product */
        }
        g_ptr_array_free(products, TRUE);  /* Free the array itself */
        products = NULL;
    }
    /* Free all the history memory */
    if (history) {
        for (guint i = 0; i < history->len; i++) {
            g_free(g_ptr_array_index(history, i));  /* Free each history entry */
        }
        g_ptr_array_free(history, TRUE);  /* Free the array itself */
        history = NULL;
    }
}

/* This is where the program starts - the main function */
int main(int argc, char **argv) {
    /* Create the GTK application */
    GtkApplication *app = gtk_application_new("com.example.stockmanager",
                                              G_APPLICATION_FLAGS_NONE);
    /* Connect functions to events */
    /* When app starts, call on_activate */
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    /* When app closes, call on_shutdown */
    g_signal_connect(app, "shutdown", G_CALLBACK(on_shutdown), NULL);

    /* Run the app - this starts the GUI and waits for user input */
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    /* Clean up */
    g_object_unref(app);
    return status;  /* Return 0 if OK, non-zero if error */
}


