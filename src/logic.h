#ifndef LOGIC_H
#define LOGIC_H

#include "model.h"
#include <glib.h>

/* This file has all the business logic - the actual work functions */
/* Like adding products, selling, updating stock, etc. */

/* Find a product by its ID */
Product *find_product_by_id(const char *id);

/* Functions to manage products */
gboolean add_product(const char *id, const char *name, const char *category,
                     double price, int quantity, GError **error);  /* Add a new product */
gboolean update_stock(const char *id, int add_qty, GError **error);  /* Add more stock (must be > 5) */
gboolean sell_product(const char *id, int qty, double *total, GError **error);  /* Sell some products */
gboolean remove_product(const char *id, GError **error);  /* Delete a product */

/* Functions to check things */
int get_stock_level(const char *id, int *out_qty, GError **error);  /* Check how many we have */
double compute_total_stock_value(void);  /* Calculate total money value of all stock */

/* Discount function */
gboolean apply_discount(const char *id, int qty, double discount_percent,
                       double *discounted_total, GError **error);  /* Apply discount (10-20%) */

/* History function */
void record_history(const char *operation, const Product *p, int qty_change,
                    double value_change, const char *description);  /* Save what we did to history */

#endif /* LOGIC_H */


