#ifndef BASE64_H
#define BASE64_H

#include <stddef.h>

typedef struct {
    unsigned char *data;
    size_t         len;
    size_t         capacity;
} decimal_ctx_t;

typedef decimal_ctx_t hex_ctx_t;

char *raw_bytes2base64(const unsigned char *bytes, size_t len);

int base64_index(char c);
void ensure_hex_capacity(hex_ctx_t *ctx, size_t extra);
void ensure_byte_capacity(long value, void *ctx_void);

#endif // BASE64_H
