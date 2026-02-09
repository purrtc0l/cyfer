#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bytes.h"
#include "commands.h"
#include "cyfer.h"
#include "cyfer_utils.h"
#include "detect.h"
#include "logging.h"
#include "runners.h"

typedef enum { 
    REPR_BASE64,
    REPR_DECIMAL,
    REPR_HEX,
    REPR_BITS,
    REPR_ASCII,
    REPR_BYTES,
    REPR_COUNT // auto count to 6
} ReprType;

typedef struct {
    ReprType type;
    const char *name;
    bool sure;
} Confidence;

void run_auto_detect_converter(Interactive *i, const Canonical *c, const char *guess)
{
    (void)c;

    for (size_t k = 0; k < converters_count; k++) {
        if (strcmp(cmds[k].from, guess) == 0 &&
            strcmp(cmds[k].to, "Bytes") != 0) {
            printf(C256(69, "→ ") GREEN("%s: "), cmds[k].to);
            cmds[k].func(i, cmds);
        }
    }
}

void run_bytes_to_converter(Interactive *i, const Canonical *c, bytes_to_cvt_t bytes_to_converter)
{
    // == non-interactive only ==
    // error handling: user provided NO argument
    if (i->interactive) {
        log_err(ERROR, "'%s' does not support interactive mode.\n", c->name);
        log_err(USAGE, "\n  cyfer %s < <FILE>\n  cat <FILE> | cyfer %s\n", 
                c->name, c->name);
        exit(EXIT_FAILURE);
    }

    if (!i->arg) {
        log_err(ERROR, "no input provided\n");
        exit(EXIT_FAILURE);
    }

    // do NOT copy: dup_str would stop at the first null byte
    char *result = bytes_to_converter(i->arg, i->len, i);
    printf("%s\n", result);

    // free memory
    free(result);
}

void run_to_bytes_converter(Interactive *i, const Canonical *c, to_bytes_cvt_t to_bytes_converter)
{
    // == non-interactive only ==
    // error handling: user provided NO argument
    if (i->interactive) {
        log_err(ERROR, "'%s' does not support interactive mode.\n", c->name);
        log_err(USAGE, "\n  cyfer %s <ARG> > <FILE>\n  echo <ARG> | cyfer %s > <FILE>\n", 
                c->name, c->name);
        exit(EXIT_FAILURE);
    }

    if (!i->arg) {
        log_err(ERROR, "no input provided\n");
        exit(EXIT_FAILURE);
    }

    char *value = dup_str(i->arg);
    size_t data_len = 0;
    // convert from -> to
    unsigned char *result = to_bytes_converter(value, &data_len);

    // write to stdout & error handling
    size_t written = fwrite(result, 1, data_len, stdout);
    if (written  != data_len) {
        log_err(ERROR, "partial write (%zu/%zu bytes)\n", 
                written, data_len);
        perror("fwrite");
        exit(EXIT_FAILURE);
    }
    log_err(WRITE, "Wrote %zu bytes\n", data_len);

    // free memory
    free(result);
}

void run_repr_converter(Interactive *i, const Canonical *c, repr_cvt_t repr_converter)
{
    char *value = i->interactive // interactive
                ? intro_prompt(cmds, c->from, c->to, 
                                i->verbose? VERBOSE : QUIET) // Get input
                : dup_str(i->arg); // non-interactive

    // convert from -> to
    char *result = repr_converter(value, i);
    printf("%s\n", result);

    // free memory
    free(value);
    free(result);
}

// === Auto Detect: Any -> reprs ===
void run_detect(Interactive *i, const Canonical *c)
{
    (void)c;

    const char *input = i->arg;

    Confidence candidates[REPR_COUNT] = {
        {REPR_BASE64,  "Base64",  false},
        {REPR_DECIMAL, "Decimal", false},
        {REPR_HEX,     "Hex",     false},
        {REPR_BITS,    "Bits",    false},
        {REPR_ASCII,   "ASCII",   false},
        {REPR_BYTES,   "Bytes",   false},
    };

    if (is_base64         (input)) candidates[REPR_BASE64] .sure = true;
    if (is_decimal        (input)) candidates[REPR_DECIMAL].sure = true; 
    if (is_hex            (input)) candidates[REPR_HEX]    .sure = true;
    if (is_bits           (input)) candidates[REPR_BITS]   .sure = true;
    if (is_printable_ascii(input)) candidates[REPR_ASCII]  .sure = true;

    const char *confident_name = candidates[REPR_BYTES].name; // fallback as default
    const char *unconfident_names[REPR_COUNT] = {NULL};
    int   confident_type    = REPR_BYTES; // fallback as default
    int   unconfident_count = 0;

    // == find confident_name ==
    for (int k = 0; k < REPR_COUNT; k++) {
        if (candidates[k].sure) {
            // assign the first true sure to final type
            confident_name = candidates[k].name;
            // its index
            confident_type = k;
            break;
        }
    };

    // == find unconfident_names ===
    // start right after the index of confident name
    for (int n = confident_type + 1; n < REPR_COUNT; n++) {
        if (candidates[n].sure) {
            unconfident_names[unconfident_count] = candidates[n].name;  // use counter as index
            unconfident_count++;
        }
    }

    // == print confidece and list possibilities ==
    // unconfident_count = 0 -> 100% 
    // unconfident_count = 1 -> 50%
    // unconfident_count = 2 -> 33.33%
    float confidence_percent = 100.0f / (unconfident_count + 1);

    printf(BLUE("%s (%.2f%%)"), confident_name, confidence_percent);

    if (unconfident_count > 0) {
        printf(" · might be");

        if (unconfident_count > 0) {
            for (int t = 0; t < unconfident_count; t++) {
                printf(YELLOW(" %s") , unconfident_names[t]);
            }
            printf("\n");
        }
    }

    // == run ==
    run_auto_detect_converter(i, c, confident_name);
}

// === => BYTES ===
// ASCII -> BYTES
void run_ascii2bytes(Interactive *i, const Canonical *c)
{
    return run_to_bytes_converter(i, c, ascii2bytes);
}

// DECIMAL -> BYTES
void run_decimal2bytes(Interactive *i, const Canonical *c)
{
    return run_to_bytes_converter(i, c, decimal2bytes);
}

// BITS -> BYTES
void run_bits2bytes(Interactive *i, const Canonical *c)
{
    return run_to_bytes_converter(i, c, bits2bytes);
}

// HEX -> BYTES
void run_hex2bytes(Interactive *i, const Canonical *c)
{
    return run_to_bytes_converter(i, c, hex2bytes);
}

// BASE64 -> BYTES
void run_base642bytes(Interactive *i, const Canonical *c)
{
    return run_to_bytes_converter(i, c, base642bytes);
}

// === BYTES => ===

// BYTES -> ASCII
void run_bytes2ascii(Interactive *i, const Canonical *c)
{
    run_bytes_to_converter(i, c, bytes2ascii);
}

// BYTES -> BITS
void run_bytes2bits(Interactive *i, const Canonical *c)
{
    run_bytes_to_converter(i, c, bytes2bits);
}

// BYTES -> DECIMAL
void run_bytes2decimal(Interactive *i, const Canonical *c)
{
    run_bytes_to_converter(i, c, bytes2decimal);
}

// BYTES -> HEX
void run_bytes2hex(Interactive *i, const Canonical *c)
{
    run_bytes_to_converter(i, c, bytes2hex);
}

// BYTES -> BASE64
void run_bytes2base64(Interactive *i, const Canonical *c)
{
    run_bytes_to_converter(i, c, bytes2base64);
}

// representation converters BEGINS
// === ASCII => ===
// ASCII -> DECIMAL
void run_ascii2decimal(Interactive *i, const Canonical *c)
{
    return run_repr_converter(i, c, ascii2decimal);
}

// ASCII -> BITS
void run_ascii2bits(Interactive *i, const Canonical *c)
{
    return run_repr_converter(i, c, ascii2bits);
}

// ASCII -> HEX
void run_ascii2hex(Interactive *i, const Canonical *c)
{
    return run_repr_converter(i, c, ascii2hex);
}

// ASCII -> BASE64
void run_ascii2base64(Interactive *i, const Canonical *c)
{
    return run_repr_converter(i, c, ascii2base64);
}

// === DECIMAL => ===

// DECIMAL -> ASCII
void run_decimal2ascii(Interactive *i, const Canonical *c)
{
    return run_repr_converter(i, c, decimal2ascii);
}

// === DECIMAL => ===

// DECIMAL -> BITS
void run_decimal2bits(Interactive *i, const Canonical *c)
{
    return run_repr_converter(i, c, decimal2bits);
}

// DECIMAL -> HEX
void run_decimal2hex(Interactive *i, const Canonical *c)
{
    return run_repr_converter(i, c, decimal2hex);
}

// DECIMAL -> BASE64
void run_decimal2base64(Interactive *i, const Canonical *c)
{
    return run_repr_converter(i, c, decimal2base64);
}

// === BITS => ===

// BITS -> ASCII
void run_bits2ascii(Interactive *i, const Canonical *c)
{
    return run_repr_converter(i, c, bits2ascii);
}

// BITS -> DECIMAL
void run_bits2decimal(Interactive *i, const Canonical *c)
{
    return run_repr_converter(i, c, bits2decimal);
}

// BITS -> HEX
void run_bits2hex(Interactive *i, const Canonical *c)
{
    return run_repr_converter(i, c, bits2hex);
}

// BITS -> BASE64
void run_bits2base64(Interactive *i, const Canonical *c)
{
    return run_repr_converter(i, c, bits2base64);
}

// === HEX => ===

// HEX -> ASCII
void run_hex2ascii(Interactive *i, const Canonical *c)
{
    return run_repr_converter(i, c, hex2ascii);
}

// HEX -> DECIMAL
void run_hex2decimal(Interactive *i, const Canonical *c)
{
    return run_repr_converter(i, c, hex2decimal);
}

// HEX -> BITS
void run_hex2bits(Interactive *i, const Canonical *c)
{
    return run_repr_converter(i, c, hex2bits);
}

// HEX -> BASE64
void run_hex2base64(Interactive *i, const Canonical *c)
{
    return run_repr_converter(i, c, hex2base64);
}

// === BASE64 => ===

// BASE64 -> ASCII
void run_base642ascii(Interactive *i, const Canonical *c)
{
    return run_repr_converter(i, c, base642ascii);
}

// BASE64 -> DECIMAL
void run_base642decimal(Interactive *i, const Canonical *c)
{
    return run_repr_converter(i, c, base642decimal);
}

// BASE64 -> BITS
void run_base642bits(Interactive *i, const Canonical *c)
{
    return run_repr_converter(i, c, base642bits);
}

// BASE64 -> HEX
void run_base642hex(Interactive *i, const Canonical *c)
{
    return run_repr_converter(i, c, base642hex);
}
// representation converters END
