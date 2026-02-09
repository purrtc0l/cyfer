#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "commands.h"
#include "config.h"
#include "cyfer.h"
#include "cyfer_utils.h"
#include "help.h"
#include "logging.h"

int canonicalize(const Canonical *c, const char *arg);

// ========== MAIN ==========
int main(int argc, const char *argv[])
{
    // == define delimiters & shift 2 argc back ==
    // = delimiter flag is only for Decimal/Hex/Bits =
    // cyfer d2a "72|101|108|108|111" -d "|"
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--delimiters") == 0 || 
            strcmp(argv[i], "-D") == 0 ) {

            if (i + 1 >= argc) {
                log_err(ERROR, "%s requires an argument\n", argv[i]);
                return EXIT_FAILURE;
            }
            DELIMS = argv[i + 1];

            // remove `--delimiters` and its value
            for (int j = i; j < argc - 2; j++) {
                argv[j] = argv[i + 2];
            }
            argc -= 2;
            break;
        }
    }

    if (argc < 2) {
        printf("Usage: cyfer <mode>\n");
        return 1;
    }

    const char *mode = argv[1];
    Interactive ctx;
    // quiet is only for interactive mode
    bool is_quiet_flag = (argc >= 3 && 
                          (strcmp(argv[2], "--quiet") == 0 || 
                           strcmp(argv[2], "-q") == 0));

    if (argc == 2 && 
            (strcmp(mode, "--version") == 0 || strcmp(mode, "-v") == 0)) 
    {
        version();
        return EXIT_SUCCESS;
    }

    // === Case 0: auto-detect ===
    if (strcmp(mode, "auto-detect") == 0 || 
        strcmp(mode, "auto")        == 0 || 
        strcmp(mode, "detect")      == 0 || 
        strcmp(mode, "d")           == 0) {

        if (argc == 3) {
            // user provided value
            ctx = (Interactive){ 
                .interactive = false, 
                .arg = argv[2]
            };
        } 
        else if (!isatty(STDIN_FILENO)) {
            // piped input
            size_t len;
            ctx = (Interactive){ 
                .interactive = false, 
                .arg = read_all_stdin(&len, 128, STOP_ON_EOF),
                .len = len
            };
        } 
        else {
            // no input, error out
            log_err(ERROR, "no input provided\n");
            log_err(USAGE, 
                    "\n  cyfer %s <value>\n  echo <value> | cyfer %s\n", 
                    mode, mode);
            return EXIT_FAILURE;
        }
    }
    // === Case 1: user provided explicit value -> quiet mode ===
    // cli: cyfer <mode> <value>
    else if (argc == 3 && !is_quiet_flag) {
        ctx = (Interactive){ 
            .interactive = false, 
            .arg = argv[2] 
        };
    }
    // === Case 2: user provided NO argument (argc == 2) ===
    else {
        // == non-interactive mode ==
        // pipe: echo <value> > | cyfer <mode>
        if (!isatty(STDIN_FILENO)) {
            size_t len;
            ctx = (Interactive){ 
                .interactive = false, 
                .arg = read_all_stdin(&len, 128, STOP_ON_EOF),
                .len = len
            };
        }
        // == interactive mode ==
        // prompt: cyfer <mode>
        else {
            ctx = (Interactive){
                .verbose = !is_quiet_flag,
                .interactive = true,
                .arg = NULL, // explicitly set as NULL
                .len = 0
            };
        }
    }

    for (size_t k = 0; k < cmds_count; k++) {
        if (canonicalize(&cmds[k], mode)) {
            cmds[k].func(&ctx, &cmds[k]); // runs the command
            return 0;
        }
    }

    // if no matched cmd, exit
    log_err(ERROR, 
            "unknown command: %s\nTry " BLUE("cyfer --help") ".\n", 
            mode);
    return EXIT_FAILURE;
}

int canonicalize(const Canonical *c, const char *arg)
{
    // if use complete canonical name
    if (strcmp(c->name, arg) == 0) {
        return 1;
    }

    // else use aliases
    size_t alias_num = sizeof(c->aliases) / sizeof(c->aliases[0]);

    for (size_t i = 0; i < alias_num; i++) {
        const char *a = c->aliases[i];
        if (!a)
            break; // break if alias = NULL
        if (strcmp(arg, a) == 0)
            return 1;
    }

    return 0;
}

