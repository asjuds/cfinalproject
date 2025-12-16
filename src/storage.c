#include "storage.h"
#include <stdio.h>
#include <string.h>

/* Defined in main.c, declared in model.h */
extern GPtrArray *products;
extern GPtrArray *history;

static void trim_newline(char *s) {
    if (!s) return;
    char *p = strchr(s, '\n');
    if (p) *p = '\0';
}

gboolean storage_load_products(const char *path, GError **error) {
    FILE *f = fopen(path, "r");
    if (!f) {
        return TRUE;
    }

    char line[512];
    while (fgets(line, sizeof(line), f)) {
        trim_newline(line);
        if (line[0] == '\0') continue;

        Product *p = g_new0(Product, 1);
        char price_str[64], qty_str[64], sold_str[64];

        if (sscanf(line, "%31[^,],%63[^,],%31[^,],%63[^,],%63[^,],%63s",
                   p->id, p->name, p->category,
                   price_str, qty_str, sold_str) != 6) {
            g_free(p);
            continue;
        }

        p->price = g_ascii_strtod(price_str, NULL);
        p->quantity = (int)g_ascii_strtoll(qty_str, NULL, 10);
        p->sold = (int)g_ascii_strtoll(sold_str, NULL, 10);

        g_ptr_array_add(products, p);
    }

    fclose(f);
    return TRUE;
}

gboolean storage_save_products(const char *path, GError **error) {
    FILE *f = fopen(path, "w");
    if (!f) {
        g_set_error(error, g_quark_from_static_string("storage"),
                    1, "Failed to open %s for writing", path);
        return FALSE;
    }

    for (guint i = 0; i < products->len; i++) {
        Product *p = g_ptr_array_index(products, i);
        fprintf(f, "%s,%s,%s,%.2f,%d,%d\n",
                p->id, p->name, p->category,
                p->price, p->quantity, p->sold);
    }

    fclose(f);
    return TRUE;
}

gboolean storage_load_history(const char *path, GError **error) {
    FILE *f = fopen(path, "r");
    if (!f) {
        return TRUE;
    }

    char line[512];
    while (fgets(line, sizeof(line), f)) {
        trim_newline(line);
        if (line[0] == '\0') continue;

        HistoryEntry *h = g_new0(HistoryEntry, 1);
        char ts_str[64], qty_str[64], val_str[64];

        if (sscanf(line, "%63[^,],%15[^,],%31[^,],%63[^,],%63[^,],%127[^\n]",
                   ts_str, h->operation, h->product_id,
                   qty_str, val_str, h->description) != 6) {
            g_free(h);
            continue;
        }

        h->timestamp = (time_t)g_ascii_strtoll(ts_str, NULL, 10);
        h->quantity_change = (int)g_ascii_strtoll(qty_str, NULL, 10);
        h->value_change = g_ascii_strtod(val_str, NULL);

        g_ptr_array_add(history, h);
    }

    fclose(f);
    return TRUE;
}

gboolean storage_save_history(const char *path, GError **error) {
    FILE *f = fopen(path, "w");
    if (!f) {
        g_set_error(error, g_quark_from_static_string("storage"),
                    2, "Failed to open %s for writing", path);
        return FALSE;
    }

    for (guint i = 0; i < history->len; i++) {
        HistoryEntry *h = g_ptr_array_index(history, i);
        fprintf(f, "%lld,%s,%s,%d,%.2f,%s\n",
                (long long)h->timestamp, h->operation, h->product_id,
                h->quantity_change, h->value_change, h->description);
    }

    fclose(f);
    return TRUE;
}


