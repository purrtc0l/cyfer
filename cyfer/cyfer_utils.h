#ifndef CYFER_UTILS_H
#define CYFER_UTILS_H

#include <stddef.h>

#include "commands.h"
#include "cyfer.h"

typedef enum { 
    QUIET, 
    VERBOSE 
} Chattiness;

void prpt_ascii  (void);
void prpt_decimal(void);
void prpt_bit    (void);
void prpt_decimal(void);
void prpt_hex    (void);
void prpt_base64 (void);

// === composed converters BEGINS ===
char *ascii2decimal (const char* ascii,   Interactive *i);
char *ascii2bits    (const char* ascii,   Interactive *i);
char *ascii2hex     (const char *ascii,   Interactive *i);
char *ascii2base64  (const char *ascii,   Interactive *i);

char *decimal2ascii (const char *decimal, Interactive *i);
char *decimal2bits  (const char *decimal, Interactive *i);
char *decimal2hex   (const char *decimal, Interactive *i);
char *decimal2base64(const char *decimal, Interactive *i);

char *bits2ascii    (const char *bits,    Interactive *i);
char *bits2decimal  (const char *bits,    Interactive *i);
char *bits2hex      (const char *bits,    Interactive *i);
char *bits2base64   (const char *bits,    Interactive *i);

char *hex2ascii     (const char *hex,     Interactive *i);
char *hex2decimal   (const char *hex,     Interactive *i);
char *hex2bits      (const char *hex,     Interactive *i);
char *hex2base64    (const char *hex,     Interactive *i);

char *base642ascii  (const char *base64,  Interactive *i);
char *base642decimal(const char *base64,  Interactive *i);
char *base642bits   (const char *base64,  Interactive *i);
char *base642hex    (const char *base64,  Interactive *i);
// === composed converters END ===

char *read_all_stdin(size_t *out_len, size_t buflen, ReadMode mode);

char *intro_prompt(const Canonical *c, const char *input_grp, const char *output_grp, Chattiness chat);

char *dup_str(const char *raw_str);

typedef int (*formatter_t)(char *dst, size_t dst_size, long num);
char *format_num_token_with(const char *str, 
                            size_t len, 
                            size_t per_len, 
                            int base, 
                            formatter_t format_func
                            );
char *format_decimal_with  (const unsigned char *buf, 
                            size_t len, 
                            size_t per_len, 
                            formatter_t format_func
                            );

typedef void (*num_consumer_t)(long value, void *ctx);
void for_each_num_token(const char *str, 
                        int base, 
                        num_consumer_t cb, 
                        void *ctx
                        );
void ensure_num_capacity(long value, void *ctx_void);

int format_num2_hex          (char *dst, size_t size, long num);
int format_num2_dec          (char *dst, size_t size, long num);
int format_num2_bits         (char *dst, size_t size, long num);
int format_num2_ascii_verbose(char *dst, size_t size, long num);
int format_num2_ascii_quiet  (char *dst, size_t size, long num);

#endif // CYFER_UTILS_H
