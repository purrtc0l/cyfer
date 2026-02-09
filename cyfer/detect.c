#include "detect.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base64.h"
#include "config.h"
#include "cyfer_utils.h"

static bool is_binary(char c);

// === ASCII: check if it's printable ===
bool is_printable_ascii(const char *str)
{
    for (; *str; str++) {
        // if it's not printable
        if (*str < 32 || *str > 126) return false; 
    }
    return true;
}

// === Decimal: tokenizing, digit between 0 to 255 ===
bool is_decimal(const char *str)
{
    if (DELIMS == NULL) DELIMS = DEFAULT_DELIMS; 

    char *copy = dup_str(str);
    char *token = strtok(copy, DELIMS);
    bool valid = true;

    while (token != NULL && valid) {
        // check every char in token is a digit
        for (int i = 0; token[i]; i++) {
            if (!isdigit(token[i])) {
                valid = false;
                break;
            }
        }
        
        if (valid) {
            long value = strtol(token, NULL, 10);
            if (value < 0 || value > 255) {
                valid = false;
            }
        }
        
        token = strtok(NULL, DELIMS);
    }

    free(copy);
    return valid;
}

// === Bits: tokenizing, 0 < digits <= 8 (all 0 or 1) per token ===
static bool is_binary(char c) { return c == '0' || c == '1'; }

bool is_bits(const char *str)
{
    if (DELIMS == NULL) DELIMS = DEFAULT_DELIMS; 

    char *copy = dup_str(str);
    char *token = strtok(copy, DELIMS);
    bool valid = true;
    
    while (token != NULL) {
        size_t token_len = strlen(token);
        // 0 < digits <= 8
        if (token_len == 0 || token_len > 8) {
            valid = false;
            break;
        }

        for (size_t k = 0; k < token_len; k++) {
            // all 0 or 1
            if (!is_binary(token[k])) {
                valid = false;
                break;
            }
        }

        token = strtok(NULL, DELIMS);
    }

    free(copy);
    return valid;
}

// === Hex: tokenizing, check ^(0x)?[0-9a-fA-F]+$ ===
bool is_hex(const char *str)
{
    if (DELIMS == NULL) DELIMS = DEFAULT_DELIMS; 

    char *copy = dup_str(str);
    char *token = strtok(copy, DELIMS);
    bool valid = true;

    while (token != NULL && valid) {
        size_t track_len = strlen(token);
        int start = 0;

        // if first 2 char of every token is `0x`
        if (track_len >= 3 && token[0] == '0' && token[1] == 'x') {
            start = 2;
            track_len -= 2; // remaining hex digits
        }

        // only allow 1 or 2 hex digit(s) per token
        if (track_len < 1 || track_len > 2) {
            valid = false;
            break;
        }

        for (int i = start; token[i]; i++) {
            if (!isxdigit(token[i])) {
                valid = false;
                break;
            }
        }

        token = strtok(NULL, DELIMS);
    }

    free(copy);
    return valid;
}

// === Base64: check [A-Za-z0-9+/=] with padding rules ===
bool is_base64(const char *str)
{
    size_t len = strlen(str);

    // must be multiple of 4
    if (len == 0 || len % 4 != 0) return false;

    int padding = 0;
    for (size_t i = 0; i < len; i++) {
        char c = str[i];

        if (c == '=') {
            padding++;
            // maximum 2 padding at last
            if (i < len - 2) return false;
        } else if (base64_index(c) == -1) return false; // not in the alphabet
    }

    return (padding <= 2);
}

