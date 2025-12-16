#include "logic.h"
#include <string.h>

extern GPtrArray *products;
extern GPtrArray *history;

Product *find_product_by_id(const char *id) {
    for (guint i = 0; i < products->len; i++) {
        Product *p = g_ptr_array_index(products, i);
        if (g_strcmp0(p->id, id) == 0) {
            return p;
        }
    }
    return NULL;
}

void record_history(const char *operation,
                    const Product *p,
                    int qty_change,
                    double value_change,
                    const char *description) {
    HistoryEntry *h = g_new0(HistoryEntry, 1);
    h->timestamp = time(NULL);
    g_strlcpy(h->operation, operation, sizeof(h->operation));
    g_strlcpy(h->product_id, p ? p->id : "", sizeof(h->product_id));
    h->quantity_change = qty_change;
    h->value_change = value_change;
    g_strlcpy(h->description, description, sizeof(h->description));
    g_ptr_array_add(history, h);
}

gboolean add_product(const char *id,
                     const char *name,
                     const char *category,
                     double price,
                     int quantity,
                     GError **error) {
    if (price <= 0 || quantity <= 0) {
        g_set_error(error, g_quark_from_static_string("logic"), 1,
                    "Price and quantity must be > 0");
        return FALSE;
    }
    if (find_product_by_id(id)) {
        g_set_error(error, g_quark_from_static_string("logic"), 2,
                    "Product with this ID already exists");
        return FALSE;
    }

    Product *p = g_new0(Product, 1);
    g_strlcpy(p->id, id, sizeof(p->id));
    g_strlcpy(p->name, name, sizeof(p->name));
    g_strlcpy(p->category, category, sizeof(p->category));
    p->price = price;
    p->quantity = quantity;
    p->sold = 0;

    g_ptr_array_add(products, p);
    record_history("ADD", p, quantity, price * quantity, "Added product");
    return TRUE;
}

gboolean update_stock(const char *id, int add_qty, GError **error) {
    if (add_qty <= 5) {
        g_set_error(error, g_quark_from_static_string("logic"), 3,
                    "Quantity to add must be > 5");
        return FALSE;
    }

    Product *p = find_product_by_id(id);
    if (!p) {
        g_set_error(error, g_quark_from_static_string("logic"), 4,
                    "Product not found");
        return FALSE;
    }

    p->quantity += add_qty;
    record_history("UPDATE", p, add_qty, p->price * add_qty, "Updated stock");
    return TRUE;
}

gboolean sell_product(const char *id,
                      int qty,
                      double *total,
                      GError **error) {
    /* Allow selling any positive quantity */
    if (qty <= 0) {
        g_set_error(error, g_quark_from_static_string("logic"), 5,
                    "Quantity to sell must be > 0");
        return FALSE;
    }

    Product *p = find_product_by_id(id);
    if (!p) {
        g_set_error(error, g_quark_from_static_string("logic"), 6,
                    "Product not found");
        return FALSE;
    }

    if (p->quantity < qty) {
        g_set_error(error, g_quark_from_static_string("logic"), 7,
                    "Not enough stock");
        return FALSE;
    }

    p->quantity -= qty;
    p->sold += qty;
    double value = p->price * qty;
    if (total) *total = value;
    record_history("SELL", p, -qty, value, "Sold product");
    return TRUE;
}

gboolean remove_product(const char *id, GError **error) {
    for (guint i = 0; i < products->len; i++) {
        Product *p = g_ptr_array_index(products, i);
        if (g_strcmp0(p->id, id) == 0) {
            record_history("REMOVE", p, -p->quantity, 0.0, "Removed product");
            g_ptr_array_remove_index(products, i);
            g_free(p);
            return TRUE;
        }
    }
    g_set_error(error, g_quark_from_static_string("logic"), 8,
                "Product not found");
    return FALSE;
}

int get_stock_level(const char *id, int *out_qty, GError **error) {
    Product *p = find_product_by_id(id);
    if (!p) {
        g_set_error(error, g_quark_from_static_string("logic"), 9,
                    "Product not found");
        return -1;
    }
    if (out_qty) *out_qty = p->quantity;
    return p->quantity;
}

double compute_total_stock_value(void) {
    double sum = 0.0;
    for (guint i = 0; i < products->len; i++) {
        Product *p = g_ptr_array_index(products, i);
        sum += p->price * p->quantity;
    }
    return sum;
}

gboolean apply_discount(const char *id,
                        int qty,
                        double discount_percent,
                        double *discounted_total,
                        GError **error) {
    if (discount_percent < 10.0 || discount_percent > 20.0) {
        g_set_error(error, g_quark_from_static_string("logic"), 10,
                    "Discount must be between 10 and 20");
        return FALSE;
    }
    if (qty <= 0) {
        g_set_error(error, g_quark_from_static_string("logic"), 11,
                    "Quantity must be > 0");
        return FALSE;
    }

    Product *p = find_product_by_id(id);
    if (!p) {
        g_set_error(error, g_quark_from_static_string("logic"), 12,
                    "Product not found");
        return FALSE;
    }

    double total = p->price * qty;
    double final = total * (1.0 - discount_percent / 100.0);
    if (discounted_total) *discounted_total = final;

    record_history("DISCOUNT", p, 0, final, "Applied discount");
    return TRUE;
}


