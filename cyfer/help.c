#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "commands.h"
#include "cyfer.h"
#include "cyfer_utils.h"
#include "help.h"
#include "logging.h"

typedef enum {
    LEFT_COL,
    RIGHT_COL
} Column;

// help & alias box size
const int grp_box_total_width = 28;
const int als_box_total_width = 44;

static void print_grps(const Canonical *c, const char *grp);
static void print_als (const Canonical *c, const char *grp);

static void print_box_header (const char *grp,  int box_width, Column col);
static void print_box_tail                     (int box_width, Column col);

// help
void help_cmd(Interactive *i, const Canonical *c)
{
    (void)i;
    (void)c;

    const char *help_str = 
        "cyfer - fast, pipe-friendly converter CLI\n"
        "\n"
        "Usage: cyfer <MODE> [OPTION] [ARGS]\n"
        "  Interactive: cyfer <MODE> [OPTION]\n"
        "  CLI:         cyfer <MODE> [OPTION] <ARGS>\n"
        "  Pipe:        cat <FILE> | cyfer <MODE> [OPTION]\n"
        "\n"
        "Options:\n"
        "  --quiet, -q            suppress banner in interactive mode\n"
        "  --delimiters, -D <str> custom token delimiters (default: \" ,;\")\n"
        "                         for Decimal/Bits/Hex in non-interactive modes\n"
        "  --help, -h             display this help text and exit\n"
        "  --converters, -c       list all converters (table format)\n"
        "  --aliases, -a          list short aliases\n"
        "  --version, -v          display version information and exit\n"
        "\n"
        "Convert Modes:\n"
        "\n"
        "  Bytes modes (pipe-only, no interactive):\n"
        "    bytes2ascii, by2a      Bytes → ASCII\n"
        "    bytes2decimal, by2d    Bytes → Decimal\n"
        "    bytes2bits, by2bi      Bytes → Bits\n"
        "    bytes2hex, by2h        Bytes → Hex\n"
        "    bytes2base64, by2b6    Bytes → Base64\n"
        "\n"
        "  From ASCII:\n"
        "    ascii2bytes, a2by      ASCII → Bytes (output binary)\n"
        "    ascii2decimal, a2d     ASCII → Decimal\n"
        "    ascii2bits, a2bi       ASCII → Bits\n"
        "    ascii2hex, a2h         ASCII → Hex\n"
        "    ascii2base64, a2b6     ASCII → Base64\n"
        "\n"
        "  From Decimal (tokenizable input):\n"
        "    decimal2bytes, d2by    Decimal → Bytes (output binary)\n"
        "    decimal2ascii, d2a     Decimal → ASCII\n"
        "    decimal2bits, d2bi     Decimal → Bits\n"
        "    decimal2hex, d2h       Decimal → Hex\n"
        "    decimal2base64, d2b6   Decimal → Base64\n"
        "\n"
        "  From Hex (tokenizable input):\n"
        "    hex2bytes, h2by        Hex → Bytes (output binary)\n"
        "    hex2ascii, h2a         Hex → ASCII\n"
        "    hex2decimal, h2d       Hex → Decimal\n"
        "    hex2bits, h2bi         Hex → Bits\n"
        "    hex2base64, h2b6       Hex → Base64\n"
        "\n"
        "  From Bits (tokenizable input):\n"
        "    bits2bytes, bi2by      Bits → Bytes (output binary)\n"
        "    bits2ascii, bi2a       Bits → ASCII\n"
        "    bits2decimal, bi2d     Bits → Decimal\n"
        "    bits2hex, bi2h         Bits → Hex\n"
        "    bits2base64, bi2ba     Bits → Base64\n"
        "\n"
        "  From Base64:\n"
        "    base642bytes, b62by    Base64 → Bytes (output binary)\n"
        "    base642ascii, b62a     Base64 → ASCII\n"
        "    base642decimal, b62d   Base64 → Decimal\n"
        "    base642bits, ba2bi     Base64 → Bits\n"
        "    base642hex, b62h       Base64 → Hex\n"
        "\n"
        "  Special:\n"
        "    auto-detect, d         detect format and show all representations\n"
        "                           displays confidence score and alternatives\n"
        "\n"
        "Note: Use --delimiters to customize token separators for Decimal/Hex/Bits\n";

    printf("%s", help_str);
}

// === converters & aliases ===

void list_converters_cmd(Interactive *i, const Canonical *c)
{
    (void)i;
    (void)c;

    // print banner
    const char *cvt_banner = 
        "         " 
        C256(63, "╭─") C256(51, " cyfer <MODE> [OPTION] [ARGS] ") C256(63, "───────────────────╮") "\n"
        "         " 
        C256(63, "│") "             " C256(117, "Available convert modes") "              " C256(63, "│") "\n"
        "         " 
        C256(63, "╰──────") C256(50, " Auto detect: cyfer auto-detect [ARGS] ") C256(63, "─────╯") "\n";

    printf("%s\n", cvt_banner);

    char *cvt_grps[] = {"Bytes", "ASCII", "Decimal", "Bits", "Hex", "Base64", NULL};
    // print mode groups
    for (int g = 0; cvt_grps[g]; g += 2) {
        print_box_header(cvt_grps[g], grp_box_total_width, LEFT_COL);
        print_box_header(cvt_grps[g+1], grp_box_total_width, RIGHT_COL);

        print_grps(cmds, cvt_grps[g]);

        print_box_tail(grp_box_total_width, LEFT_COL);
        print_box_tail(grp_box_total_width, RIGHT_COL);
    }
}

void list_aliases_cmd(Interactive *i, const Canonical *c)
{
    (void)c;
    (void)i;

    const char *alias_cheatsheet_banner = 
        "                            " 
        C256(63, "╭─") C256(51, " cyfer alias cheatsheet ") C256(63, "──────────────╮") "\n"
        "                            " 
        C256(63, "│") " " C256(117, "Short aliases for each converter mode") " " C256(63, "│") "\n"
        "                            " 
        C256(63, "╰───────────────────────────────────────╯") "\n";

    printf("%s\n", alias_cheatsheet_banner);

    char *all_grps[] = {"Bytes", "ASCII", "Decimal", "Bits", "Hex", "Base64", "detect", "help", "converter", "alias", NULL};

    // print alias groups
    for (int g = 0; all_grps[g]; g += 2) {
        print_box_header(all_grps[g], als_box_total_width, LEFT_COL);
        print_box_header(all_grps[g+1], als_box_total_width, RIGHT_COL);

        // print_als(cmds, 0, 10);
        print_als(cmds, all_grps[g]);

        print_box_tail(als_box_total_width, LEFT_COL);
        print_box_tail(als_box_total_width, RIGHT_COL);
    }
}

// === version ===
void version(void)
{
    printf("cyfer v%s\n", VERSION);  // VERSION comes from -DVERSION="0.1.0" from ../compile_flags.txt
}

// === list_converters_cmd & list_aliases_cmd helpers BEGINS ===
static void print_grps(const Canonical *c, const char *grp)
{
	char *dup_grp = dup_str(grp);

    // converters per grp is 5 so far
    size_t idx_a = 0;

    for (size_t n = 0; n < converters_count; n++) {
        if (strcmp(dup_grp, c[n].from) == 0) {
            // the 1st converter index of the 1st grp
            idx_a = n;
            // printf("a = %zu\n", a);
            break;
            }
    }
    // the 1st converter index of the 2nd grp
    size_t idx_b = idx_a + 5;

    // print 5 times
    for (int i = 0; i < 5; i++) {
        printf( C256(105, "│ ") BLUE("%-14s ")
                C256(69, "→") "   %-9s" 
                C256(105, " │  ") 
                C256(105, "│ ") BLUE("%-14s ")
                C256(69, "→") "   %-9s"
                C256(105, " │") "\n",
               c[idx_a + i].name, c[idx_a + i].to, c[idx_b + i].name, c[idx_b + i].to);
    }

    free(dup_grp);
}

static void print_als(const Canonical *c, const char *grp)
{
	char *dup_grp = dup_str(grp);

    // converters per grp is 5 so far
    size_t idx_a = 0;
    size_t idx_c = 0;

    for (size_t n = 0; n < converters_count; n++) {
        if (strcmp(dup_grp, c[n].from) == 0 && 
            strcmp(dup_grp, "detect") != 0 &&
            strcmp(dup_grp, "help") != 0 &&
            strcmp(dup_grp, "converter") != 0 && 
            strcmp(dup_grp, "alias") != 0) {
            // the 1st converter index of the 1st grp
            idx_a = n;
            // printf("a = %zu\n", a);
            break;
            }
        else if (strcmp(dup_grp, "detect") == 0) {
            // index of auto-detect
            idx_c = cmds_count - 4;
        }
        else if (strcmp(dup_grp, "converter") == 0) {
            // index of --converters
            idx_c = cmds_count - 2;
        }
    }
    // the 1st converter index of the 2nd grp
    size_t idx_b = idx_a + 5;

    if (strcmp(dup_grp, "detect") != 0 &&
        strcmp(dup_grp, "help") != 0 &&
        strcmp(dup_grp, "converter") != 0 && 
        strcmp(dup_grp, "alias") != 0) {
        // print 5 times
        for (int i = 0; i < 5; i++) {
            // no checking, make sure we give every grp 3 aliases
            printf( C256(105, "│ ") BLUE("%-14s")
                    C256(69, " : ")  "%-9s " 
                    C256(69, "| ")  "%-7s" 
                    C256(69, "| ")  "%-5s " 
                    C256(105, " │  ") 

                    C256(105, "│ ") BLUE("%-14s")
                    C256(69, " : ")  "%-9s" 
                    C256(69, " | ")  "%-7s" 
                    C256(69, " | ")  "%-5s " 
                    C256(105, "│ ") "\n", 
                    c[idx_a + i].name, 
                    c[idx_a + i].aliases[0], 
                    c[idx_a + i].aliases[1], 
                    c[idx_a + i].aliases[2],

                    c[idx_b + i].name, 
                    c[idx_b + i].aliases[0], 
                    c[idx_b + i].aliases[1], 
                    c[idx_b + i].aliases[2]);
        }
    }
    else {
            printf( C256(105, "│ ") BLUE("%-14s")
                    C256(69, " : ")  "%-9s " 
                    C256(69, "| ")  "%-7s" 
                    C256(69, "| ")  "%-5s " 
                    C256(105, " │  ") 

                    C256(105, "│ ") BLUE("%-14s")
                    C256(69, " : ")  "%-9s" 
                    C256(69, " | ")  "%-7s" 
                    C256(69, " | ")  "%-5s " 
                    C256(105, "│ ") "\n", 
                c[idx_c].name, 
                c[idx_c].aliases[0], 
                c[idx_c].aliases[1], 
                c[idx_c].aliases[2],

                c[idx_c + 1].name, 
                c[idx_c + 1].aliases[0], 
                c[idx_c + 1].aliases[1], 
                c[idx_c + 1].aliases[2]);
    }

    free(dup_grp);
}

// == shared utils to draw the box outline ==

static void print_box_header(const char *grp, int box_width, Column col)
{
    // build " From <grp> "
    char left[64];
    snprintf(left, sizeof(left), " From %s ", grp);

    // clac how many "─" should we fill
    int fill_line = box_width - strlen(left);
    
    // print
    if (col == LEFT_COL) {
        printf( C256(105, "╭──") 
                C256(87, "%s"), left);
        for (int i = 0; i < fill_line; i++) {
            printf(C256(105, "─"));
        }
        printf(C256(105, "╮"));
    }
    else {
        printf(C256(105, "  ╭──") 
                C256(87, "%s"), left);
        for (int i = 0; i < fill_line; i++) {
            printf(C256(105, "─"));
        }
        printf(C256(105, "╮") "\n");
    }
}

static void print_box_tail(int box_width, Column col)
{
    if (col == LEFT_COL) {
        printf(C256(105, "╰"));
        for (int i = 0; i < box_width + 2; i++) {
            printf(C256(105, "─"));
        }
        printf(C256(105, "╯") );
    }
    else {
        printf(C256(105, "  ╰"));
        for (int i = 0; i < box_width + 2; i++) {
            printf(C256(105, "─"));
        }
        printf(C256(105, "╯") "\n");
    }
}
