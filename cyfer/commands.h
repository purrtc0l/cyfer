#ifndef COMMANDS_H
#define COMMANDS_H

#include "cyfer.h"

typedef struct Canonical Canonical; // ;)

struct Canonical {
    char *name;
    char *from;
    char *to;
    char *aliases[3];
    void (*func)(Interactive *, const Canonical *);
    char *group;
    void (*prpt)(void);
};

extern const Canonical cmds[];
extern const size_t cmds_count;
extern const size_t converters_count;

#endif // COMMANDS_H
