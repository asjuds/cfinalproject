#ifndef STORAGE_H
#define STORAGE_H

#include "model.h"

/* This file handles saving and loading data from CSV files */
/* CSV = Comma Separated Values, like Excel but simpler */

/* Functions to work with products */
gboolean storage_load_products(const char *path, GError **error);  /* Read products from file */
gboolean storage_save_products(const char *path, GError **error); /* Write products to file */

/* Functions to work with history */
gboolean storage_load_history(const char *path, GError **error);  /* Read history from file */
gboolean storage_save_history(const char *path, GError **error);  /* Write history to file */

#endif /* STORAGE_H */


