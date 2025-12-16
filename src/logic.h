#ifndef LOGIC_H
#define LOGIC_H

#include "model.h"
#include <glib.h>

Product *find_product_by_id(const char *id);

gboolean add_product(const char *id,
                     const char *name,
                     const char *category,
                     double price,
                     int quantity,
                     GError **error);

gboolean update_stock(const char *id, int add_qty, GError **error);

gboolean sell_product(const char *id,
                      int qty,
                      double *total,
                      GError **error);

gboolean remove_product(const char *id, GError **error);

int get_stock_level(const char *id, int *out_qty, GError **error);

double compute_total_stock_value(void);

gboolean apply_discount(const char *id,
                        int qty,
                        double discount_percent,
                        double *discounted_total,
                        GError **error);

void record_history(const char *operation,
                    const Product *p,
                    int qty_change,
                    double value_change,
                    const char *description);

#endif /* LOGIC_H */


