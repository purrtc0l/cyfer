#ifndef HELP_H
#define HELP_H

#include "commands.h"
#include "cyfer.h"

void help_cmd             (Interactive *i, const Canonical *c);
void list_converters_cmd  (Interactive *i, const Canonical *c);
void list_aliases_cmd     (Interactive *i, const Canonical *c);

void version   (void);

#endif // HELP_H

