#ifndef MODEL_H
#define MODEL_H

#include <glib.h>
#include <time.h>

typedef struct {
    char id[32];
    char name[64];
    char category[32];
    double price;
    int quantity;
    int sold;
} Product;

typedef struct {
    time_t timestamp;
    char operation[16];
    char product_id[32];
    int quantity_change;
    double value_change;
    char description[128];
} HistoryEntry;

extern GPtrArray *products;
extern GPtrArray *history;

#endif /* MODEL_H */


