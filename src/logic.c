#include "logic.h"
#include <string.h>

extern GPtrArray *products;
extern GPtrArray *history;

/* This function finds a product by looking for its ID */
/* It loops through all products and checks each one */
Product *find_product_by_id(const char *id) {
    /* Go through every product in our list */
    for (guint i = 0; i < products->len; i++) {
        Product *p = g_ptr_array_index(products, i);
        /* Compare IDs - if they match, we found it! */
        if (g_strcmp0(p->id, id) == 0) {
            return p;  /* Return the product we found */
        }
    }
    return NULL;  /* Didn't find it */
}

/* This function saves what we did to the history log */
/* Every time we add, sell, or update something, we call this */
void record_history(const char *operation,
                    const Product *p,
                    int qty_change,
                    double value_change,
                    const char *description) {
    /* Create a new history entry */
    HistoryEntry *h = g_new0(HistoryEntry, 1);
    h->timestamp = time(NULL);  /* Save current time */
    g_strlcpy(h->operation, operation, sizeof(h->operation));  /* Like "ADD" or "SELL" */
    g_strlcpy(h->product_id, p ? p->id : "", sizeof(h->product_id));  /* Which product */
    h->quantity_change = qty_change;  /* How much quantity changed */
    h->value_change = value_change;  /* How much money changed */
    g_strlcpy(h->description, description, sizeof(h->description));  /* A note about it */
    /* Add it to our history list */
    g_ptr_array_add(history, h);
}

/* This function adds a new product to our inventory */
/* It checks if the ID already exists and if price/quantity are valid */
gboolean add_product(const char *id,
                     const char *name,
                     const char *category,
                     double price,
                     int quantity,
                     GError **error) {
    /* First check: price and quantity must be positive numbers */
    if (price <= 0 || quantity <= 0) {
        g_set_error(error, g_quark_from_static_string("logic"), 1,
                    "Price and quantity must be > 0");
        return FALSE;  /* Can't add if invalid */
    }
    /* Second check: make sure this ID doesn't already exist */
    if (find_product_by_id(id)) {
        g_set_error(error, g_quark_from_static_string("logic"), 2,
                    "Product with this ID already exists");
        return FALSE;  /* Can't have two products with same ID */
    }

    /* Create a new Product struct and fill it with data */
    Product *p = g_new0(Product, 1);
    g_strlcpy(p->id, id, sizeof(p->id));  /* Copy the ID string */
    g_strlcpy(p->name, name, sizeof(p->name));  /* Copy the name */
    g_strlcpy(p->category, category, sizeof(p->category));  /* Copy category */
    p->price = price;
    p->quantity = quantity;
    p->sold = 0;  /* Start with 0 sold */

    /* Add it to our products list */
    g_ptr_array_add(products, p);
    /* Remember to log this in history */
    record_history("ADD", p, quantity, price * quantity, "Added product");
    return TRUE;  /* Success! */
}

/* This function adds more stock to an existing product */
/* Requirement: you can only add more than 5 at a time */
gboolean update_stock(const char *id, int add_qty, GError **error) {
    /* Check: must add more than 5 */
    if (add_qty <= 5) {
        g_set_error(error, g_quark_from_static_string("logic"), 3,
                    "Quantity to add must be > 5");
        return FALSE;
    }

    /* Find the product first */
    Product *p = find_product_by_id(id);
    if (!p) {
        g_set_error(error, g_quark_from_static_string("logic"), 4,
                    "Product not found");
        return FALSE;  /* Can't update if product doesn't exist */
    }

    /* Add the quantity to what we already have */
    p->quantity += add_qty;
    /* Save this to history */
    record_history("UPDATE", p, add_qty, p->price * add_qty, "Updated stock");
    return TRUE;
}

/* This function sells some products */
/* It checks if we have enough stock, then reduces quantity and increases sold count */
gboolean sell_product(const char *id,
                      int qty,
                      double *total,
                      GError **error) {
    /* Check: must sell at least 1 */
    if (qty <= 0) {
        g_set_error(error, g_quark_from_static_string("logic"), 5,
                    "Quantity to sell must be > 0");
        return FALSE;
    }

    /* Find the product */
    Product *p = find_product_by_id(id);
    if (!p) {
        g_set_error(error, g_quark_from_static_string("logic"), 6,
                    "Product not found");
        return FALSE;
    }

    /* Check if we have enough in stock */
    if (p->quantity < qty) {
        g_set_error(error, g_quark_from_static_string("logic"), 7,
                    "Not enough stock");
        return FALSE;  /* Can't sell what we don't have */
    }

    /* Do the sale: reduce quantity, increase sold count */
    p->quantity -= qty;  /* Take away from stock */
    p->sold += qty;  /* Add to sold counter */
    /* Calculate how much money we made */
    double value = p->price * qty;
    if (total) *total = value;  /* Return the total if they want it */
    /* Save to history */
    record_history("SELL", p, -qty, value, "Sold product");
    return TRUE;
}

/* This function deletes a product completely */
/* It finds it, saves to history, then removes it from the list */
gboolean remove_product(const char *id, GError **error) {
    /* Look for the product */
    for (guint i = 0; i < products->len; i++) {
        Product *p = g_ptr_array_index(products, i);
        if (g_strcmp0(p->id, id) == 0) {
            /* Save to history before we delete it */
            record_history("REMOVE", p, -p->quantity, 0.0, "Removed product");
            /* Remove from list and free the memory */
            g_ptr_array_remove_index(products, i);
            g_free(p);  /* Free the memory so we don't leak */
            return TRUE;  /* Success! */
        }
    }
    /* Couldn't find it */
    g_set_error(error, g_quark_from_static_string("logic"), 8,
                "Product not found");
    return FALSE;
}

/* This function checks how many of a product we have in stock */
int get_stock_level(const char *id, int *out_qty, GError **error) {
    Product *p = find_product_by_id(id);
    if (!p) {
        g_set_error(error, g_quark_from_static_string("logic"), 9,
                    "Product not found");
        return -1;  /* Return -1 if not found */
    }
    /* Return the quantity */
    if (out_qty) *out_qty = p->quantity;
    return p->quantity;
}

/* This function calculates total value of ALL our stock */
/* It adds up (price * quantity) for every product */
double compute_total_stock_value(void) {
    double sum = 0.0;
    /* Go through every product */
    for (guint i = 0; i < products->len; i++) {
        Product *p = g_ptr_array_index(products, i);
        /* Add this product's value: price times how many we have */
        sum += p->price * p->quantity;
    }
    return sum;  /* Return the total */
}

/* This function applies a discount to a sale */
/* Discount must be between 10% and 20% */
/* It doesn't change the product's price, just calculates the discounted total */
gboolean apply_discount(const char *id,
                        int qty,
                        double discount_percent,
                        double *discounted_total,
                        GError **error) {
    /* Check: discount must be 10-20% */
    if (discount_percent < 10.0 || discount_percent > 20.0) {
        g_set_error(error, g_quark_from_static_string("logic"), 10,
                    "Discount must be between 10 and 20");
        return FALSE;
    }
    /* Check: quantity must be positive */
    if (qty <= 0) {
        g_set_error(error, g_quark_from_static_string("logic"), 11,
                    "Quantity must be > 0");
        return FALSE;
    }

    /* Find the product */
    Product *p = find_product_by_id(id);
    if (!p) {
        g_set_error(error, g_quark_from_static_string("logic"), 12,
                    "Product not found");
        return FALSE;
    }

    /* Calculate: first get normal total, then apply discount */
    double total = p->price * qty;  /* Normal price */
    double final = total * (1.0 - discount_percent / 100.0);  /* With discount */
    if (discounted_total) *discounted_total = final;  /* Return the discounted price */

    /* Save to history */
    record_history("DISCOUNT", p, 0, final, "Applied discount");
    return TRUE;
}


