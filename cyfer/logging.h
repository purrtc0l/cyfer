#ifndef LOGGING_H
#define LOGGING_H

// === Color definitions ===
// == ANSI ==
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"

// bold variants
#define COLOR_BOLD_RED     "\033[1;31m"
#define COLOR_BOLD_GREEN   "\033[1;32m"
#define COLOR_BOLD_BLUE    "\033[1;34m"
#define COLOR_BOLD_YELLOW  "\033[1;33m"
#define COLOR_BOLD_MAGENTA "\033[1;35m"
#define COLOR_BOLD_CYAN    "\033[1;36m"

// == 256 colors ==
#define FG256(n) "\x1b[38;5;" #n "m"
#define BG256(n) "\x1b[48;5;" #n "m"
#define C256_RESET    "\x1b[0m"

// == wrappers ==
#define RED(s)          COLOR_RED          s COLOR_RESET
#define GREEN(s)        COLOR_GREEN        s COLOR_RESET
#define BLUE(s)         COLOR_BLUE         s COLOR_RESET
#define YELLOW(s)       COLOR_YELLOW       s COLOR_RESET
#define MAGENTA(s)      COLOR_MAGENTA      s COLOR_RESET
#define CYAN(s)         COLOR_CYAN         s COLOR_RESET

#define BOLD_RED(s)     COLOR_BOLD_RED     s COLOR_RESET
#define BOLD_GREEN(s)   COLOR_BOLD_GREEN   s COLOR_RESET
#define BOLD_BLUE(s)    COLOR_BOLD_BLUE    s COLOR_RESET
#define BOLD_YELLOW(s)  COLOR_BOLD_YELLOW  s COLOR_RESET
#define BOLD_MAGENTA(s) COLOR_BOLD_MAGENTA s COLOR_RESET
#define BOLD_CYAN(s)    COLOR_BOLD_CYAN    s COLOR_RESET

#define C256(code, s)   FG256(code)        s C256_RESET

// === Logging ===

typedef enum {
    USAGE,
    ERROR,
    WRITE,
} LogType;

typedef struct { 
    const char *tag;
    const char *color;
} LogConf;

extern const LogConf logs[];

void log_err(LogType type, const char *fmt, ...);

#endif // LOGGING_H
