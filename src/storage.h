#ifndef STORAGE_H
#define STORAGE_H

#include "model.h"

gboolean storage_load_products(const char *path, GError **error);
gboolean storage_save_products(const char *path, GError **error);

gboolean storage_load_history(const char *path, GError **error);
gboolean storage_save_history(const char *path, GError **error);

#endif /* STORAGE_H */


