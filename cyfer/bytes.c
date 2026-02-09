#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "base64.h"
#include "bytes.h"
#include "cyfer_utils.h"

// === BYTES => ===
char *bytes2ascii(const char *bytes, size_t len, Interactive *i)
{
    // worst case: "[?] " = 4 chars
    return format_decimal_with(
        (const unsigned char *)bytes, len, 5, 
        i->interactive ? format_num2_ascii_verbose : format_num2_ascii_quiet
    );
}

char *bytes2bits(const char *bytes, size_t len, Interactive *i)
{
    (void)i;
    // each byte becomes 8 bits + space = 9 characters (`"01010101 "`)
    // set len as 10 for safety
    return format_decimal_with(
        (const unsigned char *)bytes, len, 10, 
        format_num2_bits
    );
}

char *bytes2decimal(const char *bytes, size_t len, Interactive *i)
{
    (void)i;
    // "255 " = 4 chars
    return format_decimal_with(
        (const unsigned char *)bytes, len, 5, 
        format_num2_dec
    );
}

char *bytes2hex(const char *bytes, size_t len, Interactive *i)
{
    (void)i;
    // "FF " = 3 chars
    return format_decimal_with(
        (const unsigned char *)bytes, len, 5, 
        format_num2_hex
    );
}

char *bytes2base64(const char *bytes, size_t len, Interactive *i)
{
    (void)i;
    return raw_bytes2base64(
        (const unsigned char *)bytes, len
    );
}

// === => BYTES ===
// ascii -> raw bytes
unsigned char *ascii2bytes(const char *ascii, size_t *out_len)
{
    size_t len = strlen(ascii);

    // 1) allocate bytes memory
    unsigned char *bytes = malloc(len);
    if (!bytes) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // 2) copy the data directly to bytes
    memcpy(bytes, ascii, len);

    *out_len = len;

    return bytes;
}

// decimal -> raw bytes
unsigned char *decimal2bytes(const char *decimal, size_t *out_len)
{
    // 1) Collect bytes from decimal tokens
    decimal_ctx_t ctx = {0};
    ctx.capacity = 128;
    ctx.data = malloc(ctx.capacity);
    if (!ctx.data) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // 2) Parse the decimal string into the data buffer
    for_each_num_token(decimal, 10, ensure_byte_capacity, &ctx);

    *out_len = ctx.len;
    return ctx.data; // caller must free
}

// bits -> raw bytes
unsigned char *bits2bytes(const char *bits, size_t *out_len)
{
    // 1) Collect bytes from bits tokens
    decimal_ctx_t ctx = {0};
    ctx.capacity = 128;
    ctx.data = malloc(ctx.capacity);
    if (!ctx.data) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // 2) Parse the bits string into the data buffer
    for_each_num_token(bits, 2, ensure_byte_capacity, &ctx);

    *out_len = ctx.len;
    return ctx.data; // caller must free
}

// hex -> raw bytes
unsigned char *hex2bytes(const char *hex, size_t *out_len)
{
    // 1) Collect bytes from hex tokens
    decimal_ctx_t ctx = {0};
    ctx.capacity = 128;
    ctx.data = malloc(ctx.capacity);
    if (!ctx.data) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // 2) Parse the hex string into the data buffer
    for_each_num_token(hex, 16, ensure_byte_capacity, &ctx);

    *out_len = ctx.len;
    return ctx.data; // caller must free
}

// * **Base64 → Bytes**
//   1. Take 4 Base64 chars → convert each back to a 6-bit number.
//   2. Glue into 24 bits.
//   3. Slice into 3 bytes.
// > HEX: 48 65 6c 6c 6f
// SGVsbG8=
// base64 -> raw butes
unsigned char *base642bytes(const char *base64, size_t *out_len)
{
    // 1) Initialize byte context
    hex_ctx_t ctx = {0};

    ctx.capacity = 128;
    ctx.data = malloc(ctx.capacity);
    if (!ctx.data) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    ctx.len = 0;

    // 2) Make a copy and trim trailing whitespace
    char *clean = dup_str(base64);
    size_t len = strlen(clean);

    while (len > 0 && 
            (clean[len-1] == '\n' || 
             clean[len-1] == ' '  || 
             clean[len-1] == '\r')) 
    {
        clean[--len] = '\0';
    }
    
    const char *s = clean;

    // 3) The actual decoding loop
    while (*s && strlen(s) >= 4) {
        // Step 1: read 4 chars
        char c0 = s[0];
        char c1 = s[1];
        char c2 = s[2];
        char c3 = s[3];

        // Step 2: convert to indexes (6-bit values)
        int v0 = base64_index(c0);
        int v1 = base64_index(c1);
        int v2 = base64_index(c2);
        int v3 = base64_index(c3);

        // Step 3: combine into a 24-bit integer
        unsigned int triple = 0;
        if (v0 >= 0) triple |= (v0 << 18);
        if (v1 >= 0) triple |= (v1 << 12);
        if (v2 >= 0) triple |= (v2 << 6);
        if (v3 >= 0) triple |= v3;

        // Ensure capacity
        // up to 3 bytes per 4 chars
        ensure_hex_capacity(&ctx, 3);

        // Step 4: extract bytes & handle padding ('=')
        // byte1 ALWAYS exists
        ctx.data[ctx.len++] = (triple >> 16) & 0xFF;
        // byte2 exists if c2 != '='
        if (v2 >= 0)
            ctx.data[ctx.len++] = (triple >> 8) & 0xFF;
        // byte3 exists if c3 != '='
        if (v3 >= 0)
            ctx.data[ctx.len++] = triple & 0xFF;

        // move to next block of 4 chars
        s += 4;
    }

    free(clean);
    
    // Provide the length so the caller knows how big the blob is
    *out_len = ctx.len;
    
    return ctx.data; // caller must free
}
