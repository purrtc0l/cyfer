#ifndef CYFER_H
#define CYFER_H

#include <stddef.h>
#include <stdbool.h>
#include <stddef.h>

typedef enum {
    STOP_ON_NEWLINE,
    STOP_ON_EOF
} ReadMode;

typedef struct {
    bool interactive;
    bool verbose;
    const char *arg;
    size_t len; // length of arg in bytes (excluding '\0')
} Interactive;

#endif // CYFER_H
