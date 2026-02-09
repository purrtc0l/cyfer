#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "base64.h"

// base64 alphabet
const char *ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// * **Bytes → Base64**
//   1. grab 3 bytes (24 bits).
//   2. Slice into 4 groups of 6 bits.
//   3. Map each group to a char in the Base64 alphabet (`A-Z`, `a-z`, `0-9`, `+`, `/`).
//   4. If there aren't enough bytes (e.g. 1 or 2 at the end), pad with `=`.

// Pure base64 encoder (takes raw bytes)
char *raw_bytes2base64(const unsigned char *bytes, size_t len)
{
    // Base64-encode into a string using snprintf
    size_t out_cap = 4 * ((len + 2) / 3) + 1; // safe upper bound
    char *out = malloc(out_cap);
    if (!out) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    size_t out_len = 0;

    for (size_t n = 0; n < len; n += 3) {
        // group 3 bytes into 24 bits
        unsigned int triple = 0;
        triple |= bytes[n] << 16;
        if (n + 1 < len) triple |= bytes[n + 1] << 8;
        if (n + 2 < len) triple |= bytes[n + 2];

        int grp_1 = (triple >> 18) & 0x3F;
        int grp_2 = (triple >> 12) & 0x3F;
        int grp_3 = (triple >>  6) & 0x3F;
        int grp_4 =  triple        & 0x3F;

        out_len += snprintf(out + out_len, out_cap - out_len, "%c%c%c%c",
                            ALPHABET[grp_1], ALPHABET[grp_2],
                            (n + 1 < len) ? ALPHABET[grp_3] : '=',
                            (n + 2 < len) ? ALPHABET[grp_4] : '=');
    }

    out[out_len] = '\0';
    return out;
}

// hex2base64 helpers BEGINS

void ensure_byte_capacity(long value, void *ctx_void)
{
    decimal_ctx_t *ctx = ctx_void;

    if (ctx->len >= ctx->capacity) {
        ctx->capacity *= 2;
        unsigned char *new_data = realloc(ctx->data, ctx->capacity);
        if (!new_data) {
            perror("realloc");
            free(ctx->data);
            exit(EXIT_FAILURE);
        }
        ctx->data = new_data;
    }

    ctx->data[ctx->len++] = (unsigned char)value;
}

// hex2base64 helpers END

// base642hex helpers BEGINS

int base64_index(char c)
{
    const char *p = strchr(ALPHABET, c);
    if (!p) return -1; // padding or invalid char
    return p - ALPHABET;
}

void ensure_hex_capacity(hex_ctx_t *ctx, size_t extra)
{
    if (ctx->len + extra > ctx->capacity) {
        size_t new_cap = ctx->capacity * 2;
        if (new_cap < ctx->len + extra) {
            new_cap = ctx->len + extra;
        }

        unsigned char *new_data = realloc(ctx->data, new_cap);
        if (!new_data) {
            perror("realloc");
            free(ctx->data);
            exit(EXIT_FAILURE);
        }

        ctx->data = new_data;
        ctx->capacity = new_cap;
    }
}

// base642hex helpers END
