#ifndef DETECT_H
#define DETECT_H

#include <stdbool.h>

void detect            (const char *input);
bool is_printable_ascii(const char *str);
bool is_decimal        (const char *str);
bool is_bits           (const char *str);
bool is_hex            (const char *str);
bool is_base64         (const char *str);

#endif // DETECT_H

