#include "storage.h"
#include <stdio.h>
#include <string.h>

/* These are the global arrays from main.c */
extern GPtrArray *products;
extern GPtrArray *history;

/* Helper function to remove the \n (newline) at the end of a string */
/* When we read from file, each line ends with \n, we need to remove it */
static void trim_newline(char *s) {
    if (!s) return;  /* Safety check - make sure string exists */
    char *p = strchr(s, '\n');  /* Find where the newline is */
    if (p) *p = '\0';  /* Replace it with null terminator (end of string) */
}

/* This function reads products from a CSV file and puts them in memory */
/* CSV format is: id,name,category,price,quantity,sold */
/* Example line: P001,Laptop,Electronics,999.99,10,5 */
gboolean storage_load_products(const char *path, GError **error) {
    FILE *f = fopen(path, "r");  /* Open file for reading */
    if (!f) {
        /* If file doesn't exist, that's OK - maybe first time running */
        return TRUE;
    }

    char line[512];  /* Buffer to hold one line from file */
    /* Read file line by line */
    while (fgets(line, sizeof(line), f)) {
        trim_newline(line);  /* Remove the \n at the end */
        if (line[0] == '\0') continue;  /* Skip empty lines */

        /* Create a new Product struct in memory */
        Product *p = g_new0(Product, 1);
        char price_str[64], qty_str[64], sold_str[64];  /* Temporary strings for numbers */

        /* Parse the CSV line - split by commas */
        /* sscanf reads: id, name, category, price, quantity, sold */
        if (sscanf(line, "%31[^,],%63[^,],%31[^,],%63[^,],%63[^,],%63s",
                   p->id, p->name, p->category,
                   price_str, qty_str, sold_str) != 6) {
            /* If we couldn't read 6 things, the line is broken - skip it */
            g_free(p);
            continue;
        }

        /* Convert strings to numbers (price is double, quantity and sold are int) */
        p->price = g_ascii_strtod(price_str, NULL);  /* "99.99" -> 99.99 */
        p->quantity = (int)g_ascii_strtoll(qty_str, NULL, 10);  /* "10" -> 10 */
        p->sold = (int)g_ascii_strtoll(sold_str, NULL, 10);  /* "5" -> 5 */

        /* Add this product to our global list */
        g_ptr_array_add(products, p);
    }

    fclose(f);  /* Always close the file when done */
    return TRUE;
}

/* This function saves all products from memory to a CSV file */
/* It writes each product as one line in the file */
gboolean storage_save_products(const char *path, GError **error) {
    FILE *f = fopen(path, "w");  /* Open file for writing (creates new file) */
    if (!f) {
        /* Couldn't open file - maybe no permission? */
        g_set_error(error, g_quark_from_static_string("storage"),
                    1, "Failed to open %s for writing", path);
        return FALSE;
    }

    /* Loop through all products and write each one */
    for (guint i = 0; i < products->len; i++) {
        Product *p = g_ptr_array_index(products, i);
        /* Write one line: id,name,category,price,quantity,sold */
        fprintf(f, "%s,%s,%s,%.2f,%d,%d\n",
                p->id, p->name, p->category,
                p->price, p->quantity, p->sold);
    }

    fclose(f);  /* Close file when done */
    return TRUE;
}

/* This function reads history from a CSV file */
/* History is like a log of everything we did */
gboolean storage_load_history(const char *path, GError **error) {
    FILE *f = fopen(path, "r");
    if (!f) {
        /* File doesn't exist - that's OK, maybe first time */
        return TRUE;
    }

    char line[512];
    /* Read each line */
    while (fgets(line, sizeof(line), f)) {
        trim_newline(line);
        if (line[0] == '\0') continue;  /* Skip empty lines */

        /* Create a new HistoryEntry */
        HistoryEntry *h = g_new0(HistoryEntry, 1);
        char ts_str[64], qty_str[64], val_str[64];  /* Temp strings for numbers */

        /* Parse the line: timestamp,operation,product_id,quantity_change,value_change,description */
        if (sscanf(line, "%63[^,],%15[^,],%31[^,],%63[^,],%63[^,],%127[^\n]",
                   ts_str, h->operation, h->product_id,
                   qty_str, val_str, h->description) != 6) {
            /* Bad line - skip it */
            g_free(h);
            continue;
        }

        /* Convert strings to numbers */
        h->timestamp = (time_t)g_ascii_strtoll(ts_str, NULL, 10);
        h->quantity_change = (int)g_ascii_strtoll(qty_str, NULL, 10);
        h->value_change = g_ascii_strtod(val_str, NULL);

        /* Add to history list */
        g_ptr_array_add(history, h);
    }

    fclose(f);
    return TRUE;
}

/* This function saves all history to a CSV file */
/* Same idea as saving products, but for history entries */
gboolean storage_save_history(const char *path, GError **error) {
    FILE *f = fopen(path, "w");
    if (!f) {
        g_set_error(error, g_quark_from_static_string("storage"),
                    2, "Failed to open %s for writing", path);
        return FALSE;
    }

    /* Write each history entry as one line */
    for (guint i = 0; i < history->len; i++) {
        HistoryEntry *h = g_ptr_array_index(history, i);
        fprintf(f, "%lld,%s,%s,%d,%.2f,%s\n",
                (long long)h->timestamp, h->operation, h->product_id,
                h->quantity_change, h->value_change, h->description);
    }

    fclose(f);
    return TRUE;
}


