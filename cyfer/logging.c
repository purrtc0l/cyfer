#include <stdio.h>
#include <stdarg.h>

#include "logging.h"

const LogConf logs[] = {
    {"Usage: ", COLOR_BLUE},
    {"Error: ", COLOR_RED},
    {"[+] "   , COLOR_BLUE},
};

// === logging write to stderr ===
void log_err(LogType type, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "%s%s%s", 
            logs[type].color, logs[type].tag , COLOR_RESET);
    vfprintf(stderr, fmt, args);
    va_end(args);
}


