#ifndef MODEL_H
#define MODEL_H

#include <glib.h>
#include <time.h>

/* This is the Product struct - basically holds all info about one product */
/* I made it a struct so I can store multiple products easily */
typedef struct {
    char id[32];        /* Product ID like "P001" - max 31 characters */
    char name[64];      /* Product name like "Laptop" - max 63 characters */
    char category[32];  /* Category like "Electronics" - max 31 characters */
    double price;       /* How much one unit costs (like 99.99) */
    int quantity;       /* How many we have in stock right now */
    int sold;           /* How many we've sold total (keeps counting up) */
} Product;

/* This struct stores history of what we did - like a log file */
/* Every time we add, sell, or update something, we save it here */
typedef struct {
    time_t timestamp;        /* When this happened (like a date/time) */
    char operation[16];      /* What we did: "ADD", "SELL", "UPDATE", etc */
    char product_id[32];     /* Which product was affected */
    int quantity_change;     /* How much quantity changed (+10 or -5) */
    double value_change;     /* How much money changed */
    char description[128];   /* A note about what happened */
} HistoryEntry;

/* These are global arrays - they hold ALL our products and history */
/* I put them here so every file can access them */
extern GPtrArray *products;  /* List of all products */
extern GPtrArray *history;   /* List of all history entries */

#endif /* MODEL_H */


