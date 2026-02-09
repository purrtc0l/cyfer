#define _POSIX_C_SOURCE 200809L

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bytes.h"
#include "config.h"
#include "cyfer.h"
#include "cyfer_utils.h"
#include "logging.h"

typedef struct {
    char       *buf;
    size_t      capacity;
    size_t      used;
    size_t      per_len;
    formatter_t fmt;
} format_ctx_t;

// === composed converters BEGINS ===

// ASCII -> BYTES -> DECIMAL
char *ascii2decimal(const char *ascii, Interactive *i)
{
    (void)i;
    size_t data_len = 0;
    // ASCII -> BYTES
    unsigned char *bytes = ascii2bytes(ascii, &data_len);
    // BYTES -> DECIMAL
    char *decimal = bytes2decimal((const char *)bytes, data_len, i);

    free(bytes);
    return decimal;
}

// ASCII -> BYTES -> BITS
char *ascii2bits(const char *ascii, Interactive *i)
{
    (void)i;
    size_t data_len = 0;
    // ASCII -> BYTES
    unsigned char *bytes = ascii2bytes(ascii, &data_len);
    // BYTES -> BITS
    char *bits = bytes2bits((const char *)bytes, data_len, i);

    free(bytes);
    return bits;
}

// ASCII -> BYTES -> HEX
char *ascii2hex(const char *ascii, Interactive *i)
{
    (void)i;
    size_t data_len = 0;
    // ASCII -> BYTES
    unsigned char *bytes = ascii2bytes(ascii, &data_len);
    // BYTES -> HEX
    char *hex = bytes2hex((const char *)bytes, data_len, i);

    free(bytes);
    return hex;
}

// ASCII -> BYTES -> BASE64
char *ascii2base64(const char *ascii, Interactive *i)
{
    (void)i;
    size_t data_len = 0;
    // ASCII -> BYTES
    unsigned char *bytes = ascii2bytes(ascii, &data_len);
    // BYTES -> BASE64
    char *base64 = bytes2base64((const char *)bytes, data_len, i);

    free(bytes);
    return base64;
}

// DECIMAL -> BYTES -> ASCII
char *decimal2ascii(const char *decimal, Interactive *i)
{
    size_t data_len = 0;
    // DECIMAL -> BYTES
    unsigned char *bytes = decimal2bytes(decimal, &data_len);
    // BYTES -> ASCII
	char *ascii = bytes2ascii((const char *)bytes, data_len, i);

    free(bytes);
    return ascii;
}

// DECIMAL -> BYTES -> BITS
char *decimal2bits(const char *decimal, Interactive *i)
{
    (void)i;
    size_t data_len = 0;
    // DECIMAL -> BYTES
    unsigned char *bytes = decimal2bytes(decimal, &data_len);
    // BYTES -> BITS
	char *bits = bytes2bits((const char *)bytes, data_len, i);

    free(bytes);
    return bits;
}

// DECIMAL -> BYTES -> HEX
char *decimal2hex(const char *decimal, Interactive *i)
{
    (void)i;
    size_t data_len = 0;
    // DECIMAL -> BYTES
    unsigned char *bytes = decimal2bytes(decimal, &data_len);
    // BYTES -> HEX
	char *hex = bytes2hex((const char *)bytes, data_len, i);

    free(bytes);
    return hex;
}

// DECIMAL -> BYTES -> BASE65
char *decimal2base64(const char *decimal, Interactive *i)
{
    (void)i;
    size_t data_len = 0;
    // DECIMAL -> BYTES
    unsigned char *bytes = decimal2bytes(decimal, &data_len);
    // BYTES -> BASE64
	char *base64 = bytes2base64((const char *)bytes, data_len, i);

    free(bytes);
    return base64;
}

// BITS -> BYTES -> ASCII
char *bits2ascii(const char *bits, Interactive *i)
{
    size_t data_len = 0;
    // BITS -> BYTES
    unsigned char *bytes = bits2bytes(bits, &data_len);
    // BYTES -> ASCII
    char *ascii = bytes2ascii((const char *)bytes, data_len, i);

    free(bytes);
    return ascii;
}

// BITS -> BYTES -> DECIMAL
char *bits2decimal(const char *bits, Interactive *i)
{
    (void)i;
    size_t data_len = 0;
    // BITS -> BYTES
    unsigned char *bytes = bits2bytes(bits, &data_len);
    // DECIMAL -> HEX
    char *decimal = bytes2decimal((const char *)bytes, data_len, i);

    free(bytes);
    return decimal;
}

// BITS -> BYTES -> HEX
char *bits2hex(const char *bits, Interactive *i)
{
    (void)i;
    size_t data_len = 0;
    // BITS -> BYTES
    unsigned char *bytes = bits2bytes(bits, &data_len);
    // BYTES -> HEX
    char *hex = bytes2hex((const char *)bytes, data_len, i);

    free(bytes);
    return hex;
}

// BITS -> BYTES -> BASE64
char *bits2base64(const char *bits, Interactive *i)
{
    (void)i;
    size_t data_len = 0;
    // BITS -> BYTES
    unsigned char *bytes = bits2bytes(bits, &data_len);
    // BYTES -> BASE64
    char *base64 = bytes2base64((const char *)bytes, data_len, i);

    free(bytes);
    return base64;
}

// HEX -> BYTES
// import from base64.h

// HEX -> BYTES -> ASCII
char *hex2ascii(const char *hex, Interactive *i)
{
    size_t data_len = 0;
    // HEX -> BYTES
    unsigned char *bytes = hex2bytes(hex, &data_len);
    // BYTES -> ASCII
    char *ascii = bytes2ascii((const char *)bytes, data_len, i);

    free(bytes);
    return ascii;
}

// HEX -> BYTES -> DECIMAL
char *hex2decimal(const char *hex, Interactive *i)
{
    (void)i;
    size_t data_len = 0;
    // HEX -> BYTES
    unsigned char *bytes = hex2bytes(hex, &data_len);
    // BYTES -> DECIMAl
    char *decimal = bytes2decimal((const char *)bytes, data_len, i);

    free(bytes);
    return decimal;
}

// HEX -> BYTES -> BITS
char *hex2bits(const char *hex, Interactive *i)
{
    (void)i;
    size_t data_len = 0;
    // HEX -> BYTES
    unsigned char *bytes = hex2bytes(hex, &data_len);
    // BYTES -> BITS
    char *bits = bytes2bits((const char *)bytes, data_len, i);

    free(bytes);
    return bits;
}

// HEX -> BYTES -> BASE64
char *hex2base64(const char *hex, Interactive *i)
{
    (void)i;
    size_t data_len = 0;
    // HEX -> BYTES
    unsigned char *bytes = hex2bytes(hex, &data_len);
    // BYTES -> BASE64
    char *base64 = bytes2base64((const char *)bytes, data_len, i);

    free(bytes);
    return base64;
}

// BASE64 -> BYTES
// import from base64.h

// BASE64 -> BYTES -> ASCII
char *base642ascii(const char *base64, Interactive *i)
{
    size_t data_len = 0;
    // BASE64 -> BYTES
	unsigned char *bytes = base642bytes(base64, &data_len);
    // BYTES -> ASCII
    char *ascii = bytes2ascii((const char *)bytes, data_len, i);

    free(bytes);
    return ascii;
}

// BASE64 -> BYTES -> DECIMAL
char *base642decimal(const char *base64, Interactive *i)
{
    (void)i;
    size_t data_len = 0;
    // BASE64 -> BYTES
	unsigned char *bytes = base642bytes(base64, &data_len);
    // BYTES -> DECIMAL
    char *decimal = bytes2decimal((const char *)bytes, data_len, i);

    free(bytes);
    return decimal;
}

// BASE64 -> BYTES -> BITS
char *base642bits(const char *base64, Interactive *i)
{
    (void)i;
    size_t data_len = 0;
    // BASE64 -> BYTES
	unsigned char *bytes = base642bytes(base64, &data_len);
    // BYTES -> BITS
    char *bits = bytes2bits((const char *)bytes, data_len, i);

    free(bytes);
    return bits;
}


// BASE64 -> BYTES -> HEX
char *base642hex(const char *base64, Interactive *i)
{
    (void)i;
    size_t data_len = 0;
    // BASE64 -> BYTES
	unsigned char *bytes = base642bytes(base64, &data_len);
    // BYTES -> HEX
    char *hex = bytes2hex((const char *)bytes, data_len, i);

    free(bytes);
    return hex;
}

// === composed converters END ===

char *read_all_stdin(size_t *out_len, size_t buflen, ReadMode mode)
{
    char *buf = malloc(buflen);
    if (buf == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    int c;
    size_t len = 0; // total chars read

    while ((c = fgetc(stdin)) != EOF) {
        // stopping rule for interactive mode
        if (mode == STOP_ON_NEWLINE && c == '\n') {
            break;
        }
        // grow buffer if needed
        if (len + 1 >= buflen) {
            buflen *= 2;
            char *tmp = realloc(buf, buflen);
            if (!tmp) {
                free(buf);
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            buf = tmp;
        }

        // append to buffer
        buf[len++] = (char)c;
    }

    // strip all trailing whitespace
    while (len > 0 && (buf[len-1] == '\n' || 
                       buf[len-1] == '\r' || 
                       buf[len-1] == ' '  || 
                       buf[len-1] == '\t')) {
        len--;
    }
    
    buf[len] = '\0';
    *out_len = len;
    return buf;
}

// Display intro prompt, then get input content
// input_grp and output_grp are hard coded here, so this func would never fail ;-)
// intro_prompt only scans converter entries, and all converters have valid from/to/prpt fields, so matching is guaranteed not to dereference NULL.
char *intro_prompt(const Canonical *c, const char *input_grp, const char *output_grp, Chattiness chat)
{
    char banner[256];

    // loop each *converters* struct in `Canonical cmds[]`
    for (size_t i = 0; i < converters_count; i++) {
        if (strcmp(c[i].from, input_grp) == 0 && 
                strcmp(c[i].to,  output_grp) == 0) 
            if (chat == VERBOSE) {
                // show banner
                snprintf(banner, sizeof(banner),
                         "╰─ %s → %s\n",
                         c[i].from, c[i].to);
                printf("%s", banner);

                // show tip and e.g.
                c[i].prpt();
            }
    }

    // prompt user to input
    printf(C256(87, "❯ "));

    size_t len;
    char *input_content = read_all_stdin(&len, 128, STOP_ON_NEWLINE);

    return input_content;
}

// === prompts printing helpers BEGINS ===

void prpt_ascii(void)
{
    const char *prpt_ascii = 
        "  Paste ASCII (e.g., Hello)\n";

    printf("%s\n", prpt_ascii);
}

void prpt_decimal(void)
{
    const char *prpt_decimal = 
        "  Paste Decimal (e.g., 72 101 108 108 111)\n";

    printf("%s\n", prpt_decimal);
}

void prpt_bit(void)
{
    const char *prpt_bit = 
        "  Paste Bits (e.g., 01001000 01100101 01101100 01101100 01101111)\n";

    printf("%s\n", prpt_bit);
}

void prpt_hex(void)
{
    const char *prpt_hex = 
        "  Paste Hex (e.g., 48 65 6C 6C 6F)`\n";

    printf("%s\n", prpt_hex);
}

void prpt_base64(void)
{
    const char *prpt_base64 = 
        "  Paste Base64 (e.g., SGVsbG8=)\n";

    printf("%s\n", prpt_base64);
}

// === prompts printing helpers END ===

// === other helpers BEGINS ===

// char *remove_delimiter(char *raw_str)
// {
//     // remove space in space
//     size_t j = 0;
//     for (size_t i = 0; raw_str[i] != '\0'; i++) {
//         if (!strchr(DELIMS, raw_str[i])) {
//             raw_str[j++] = raw_str[i];
//         }
//     }
//     raw_str[j] = '\0'; // add '\0' at the very end
//     return raw_str;
// }

char *dup_str(const char *raw_str)
{
    char *str_cp = strdup(raw_str);
    if (!str_cp) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
    return str_cp;
}

// === other helpers END ===

// Apply formatter to tokenized content
char *format_num_token_with(const char *str, size_t len, size_t per_len, int base, formatter_t format_func)
{
    format_ctx_t ctx = {0};

    ctx.capacity = len * per_len + 1;
    ctx.buf      = malloc(ctx.capacity);
    if (!ctx.buf) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    ctx.used    = 0;
    ctx.per_len = per_len;
    ctx.fmt     = format_func;

    // drive it
    for_each_num_token(str, base, ensure_num_capacity, &ctx);

    ctx.buf[ctx.used] = '\0';
    return ctx.buf;
}

// Apply formatter to each byte in a buffer (no tokenizing, raw bytes)
char *format_decimal_with(const unsigned char *buf, size_t len, size_t per_len, formatter_t format_func)
{
    size_t capacity = len * per_len + 1;  // rough guess, can grow later
    char *p = malloc(capacity);
    if (!p) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    size_t track_len = 0;
    for (size_t i = 0; i < len; i++) {
        if (track_len + per_len > capacity) {
            capacity *= 2;
            char *new_p = realloc(p, capacity);
            if (!new_p) {
                perror("realloc");
                free(p);
                exit(EXIT_FAILURE);
            }
            p = new_p;
        }

        long num = buf[i];  // numeric code of this byte
        track_len += format_func(p + track_len,
                                 capacity - track_len,
                                 num);
    }

    return p;
}

// === format helpers for memory allocate & function pointer BEGINS ===

void for_each_num_token(const char *str, int base, num_consumer_t cb, void *ctx)
{
    if (DELIMS == NULL) DELIMS = DEFAULT_DELIMS; 

    char *copy = dup_str(str);
    char *token = strtok(copy, DELIMS);

    while (token != NULL) {
        long value = strtol(token, NULL, base);
        cb(value, ctx);
        token = strtok(NULL, DELIMS);
    }

    free(copy);
}

void ensure_num_capacity(long value, void *ctx_void)
{
    format_ctx_t *ctx = ctx_void;

    // Ensure there is room for one more formatted chunk
    if (ctx->used + ctx->per_len >= ctx->capacity) {
        ctx->capacity *= 2;
        char *new_buf = realloc(ctx->buf, ctx->capacity);
        if (!new_buf) {
            perror("realloc");
            free(ctx->buf);
            exit(EXIT_FAILURE);
        }
        ctx->buf = new_buf;
    }

    ctx->used += ctx->fmt(ctx->buf + ctx->used,
                          ctx->capacity - ctx->used,
                          value);
}

int format_num2_hex(char *dst, size_t size, long num)
{
    return snprintf(dst, size, "%02X ", (unsigned char)num);
}

int format_num2_dec(char *dst, size_t size, long num)
{
    return snprintf(dst, size, "%3d ", (unsigned char)num);
}

int format_num2_bits(char *dst, size_t size, long num)
{
    char buf[9];
    for (int i = 7; i >= 0; i--) {
        buf[7 - i] = ((num >> i) & 1) + '0';
    }
    buf[8] = '\0';

    return snprintf(dst, size, "%s ", buf);
}

// shows [?]
int format_num2_ascii_verbose(char *dst, size_t size, long num)
{
    if (num < 32 || num > 126) {
        return snprintf(dst, size, "[?]");  // just write literal
    }
    return snprintf(dst, size, "%c", (char)num);  // write the character
}

// shows ""
int format_num2_ascii_quiet(char *dst, size_t size, long num)
{
    if (num < 32 || num > 126) {
        return 0;  // write nothing
    }
    return snprintf(dst, size, "%c", (char)num);
}
// === format helpers for memory allocate & function pointer END ===
