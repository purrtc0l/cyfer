#ifndef BYTES_H
#define BYTES_H

#include <stddef.h>

#include "cyfer.h"

char *bytes2ascii  (const char *bytes, size_t len, Interactive *i);
char *bytes2decimal(const char *bytes, size_t len, Interactive *i);
char *bytes2bits   (const char *bytes, size_t len, Interactive *i);
char *bytes2hex    (const char *bytes, size_t len, Interactive *i);
char *bytes2base64 (const char *bytes, size_t len, Interactive *i);

unsigned char *ascii2bytes  (const char *ascii,   size_t *out_len);
unsigned char *decimal2bytes(const char *decimal, size_t *out_len);
unsigned char *bits2bytes   (const char *bits,    size_t *out_len);
unsigned char *hex2bytes    (const char *hex,     size_t *out_len);
unsigned char *base642bytes (const char *base64,  size_t *out_len);

#endif // BYTES_H
