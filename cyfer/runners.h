#ifndef RUNNERS_H
#define RUNNERS_H

#include "commands.h"
#include "cyfer.h"

typedef          char *(*bytes_to_cvt_t) (const char *bytes, size_t len,     Interactive *i);
typedef unsigned char *(*to_bytes_cvt_t) (const char *repr,  size_t *out_len);
typedef          char *(*repr_cvt_t)     (const char *data,                  Interactive *i);

void run_auto_detect_converter(Interactive *i, const Canonical *c, const char     *guess);
void run_bytes_to_converter   (Interactive *i, const Canonical *c, bytes_to_cvt_t bytes_to_converter);
void run_to_bytes_converter   (Interactive *i, const Canonical *c, to_bytes_cvt_t to_bytes_converter);
void run_repr_converter       (Interactive *i, const Canonical *c, repr_cvt_t     repr_converter);

void run_detect        (Interactive *i, const Canonical *c);
void run_bytes2ascii   (Interactive *i, const Canonical *c);
void run_bytes2bits    (Interactive *i, const Canonical *c);
void run_bytes2decimal (Interactive *i, const Canonical *c);
void run_bytes2hex     (Interactive *i, const Canonical *c);
void run_bytes2base64  (Interactive *i, const Canonical *c);

void run_ascii2bytes   (Interactive *i, const Canonical *c);
void run_ascii2decimal (Interactive *i, const Canonical *c);
void run_ascii2bits    (Interactive *i, const Canonical *c);
void run_ascii2hex     (Interactive *i, const Canonical *c);
void run_ascii2base64  (Interactive *i, const Canonical *c);

void run_decimal2bytes (Interactive *i, const Canonical *c);
void run_decimal2ascii (Interactive *i, const Canonical *c);
void run_decimal2bits  (Interactive *i, const Canonical *c);
void run_decimal2hex   (Interactive *i, const Canonical *c);
void run_decimal2base64(Interactive *i, const Canonical *c);

void run_bits2bytes    (Interactive *i, const Canonical *c);
void run_bits2decimal  (Interactive *i, const Canonical *c);
void run_bits2ascii    (Interactive *i, const Canonical *c);
void run_bits2hex      (Interactive *i, const Canonical *c);
void run_bits2base64   (Interactive *i, const Canonical *c);

void run_hex2bytes     (Interactive *i, const Canonical *c);
void run_hex2ascii     (Interactive *i, const Canonical *c);
void run_hex2decimal   (Interactive *i, const Canonical *c);
void run_hex2bits      (Interactive *i, const Canonical *c);
void run_hex2base64    (Interactive *i, const Canonical *c);

void run_base642bytes  (Interactive *i, const Canonical *c);
void run_base642decimal(Interactive *i, const Canonical *c);
void run_base642bits   (Interactive *i, const Canonical *c);
void run_base642hex    (Interactive *i, const Canonical *c);
void run_base642ascii  (Interactive *i, const Canonical *c);

#endif // RUNNERS_H
