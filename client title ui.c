#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*COLOR CODES*/
#define C_GREEN   "\033[32m"
#define C_YELLOW  "\033[33m"
#define C_BLUE    "\033[34m"
#define C_MAGENTA "\033[35m"
#define C_CYAN    "\033[36m"
#define C_WHITE   "\033[37m"
#define C_BRED    "\033[91m"
#define C_BGREEN  "\033[92m"
#define C_BYELLOW "\033[93m"
#define C_BBLUE   "\033[94m"
#define C_BWHITE  "\033[97m"
#define C_RESET   "\033[0m"
#define C_BOLD    "\033[1m"
#define C_DEFAULT "\033[0m"
#define C_RED     "\033[31m"
#define C_DIM     "\033[2m"
#define YL        C_BYELLOW C_BOLD
#define RD        C_RED     C_BOLD
#define RS        C_RESET

/*UI HELPERS*/
void clear_screen(void) { printf("\033[2J\033[H"); }

void print_line(void) {
    printf(C_DIM "     ──────────────────────────────────────────────\n" C_RESET);
}

void press_enter(void) {
    printf(C_DIM "\n     [ Press ENTER to continue... ]" C_RESET);
    int c; while ((c = getchar()) != '\n' && c != EOF);
}

/*TITLE SCREEN*/

void show_title(void) {
    clear_screen();
    printf("\n\n");

    printf("  "
        YL"██████"RD"╗ "RS  YL" █████"RD"╗  "RS
        YL"██████"RD"╗  "RS  YL"██████"RD"╗ "RS
        YL"███"RD"╗"YL"   ██"RD"╗ "RS  YL"██████"RD"╗ "RS "\n");

    printf("  "
        YL"██"RD"╔═══╝  "RS  YL"██"RD"╔══"YL"██"RD"╗"RS
        YL"██"RD"╔══"YL"██"RD"╗"RS  YL"██"RD"╔═══"YL"██"RD"╗"RS
        YL"████"RD"╗  "YL"██"RD"║"RS  YL"██"RD"╔════╝ "RS "\n");

    printf("  "
        RD"╚"YL"█████"RD"╗  "RS  YL"███████"RD"║"RS
        YL"██████"RD"╔╝"RS  YL"██"RD"║   "YL"██"RD"║"RS
        YL"██"RD"╔"YL"██"RD"╗ "YL"██"RD"║"RS  YL"██"RD"║  "YL"███"RD"╗"RS "\n");

    printf("  "
        " "RD"╚═══"YL"██"RD"╗ "RS  YL"██"RD"╔══"YL"██"RD"║"RS
        YL"██"RD"╔══"YL"██"RD"╗"RS  YL"██"RD"║   "YL"██"RD"║"RS
        YL"██"RD"║╚"YL"██"RD"╗"YL"██"RD"║"RS  YL"██"RD"║   "YL"██"RD"║"RS "\n");

    printf("  "
        YL"██████"RD"╔╝ "RS  YL"██"RD"║  "YL"██"RD"║"RS
        YL"██████"RD"╔╝"RS  RD"╚"YL"██████"RD"╔╝"RS
        YL"██"RD"║ ╚"YL"████"RD"║"RS  RD"╚"YL"██████"RD"╔╝"RS "\n");

    printf(C_DEFAULT);
    printf("  "RD"╚═════╝  ╚═╝  ╚═╝╚═════╝  ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝"RS"\n");

    printf(C_RED C_BOLD "\n                ─────  SA TERMINAL  ─────\n" C_RESET);
    printf(C_DIM "          A Filipino Cockfighting Terminal Game\n\n" C_RESET);
    print_line();
    printf(C_BBLUE C_BOLD "                  YOU ARE: CLIENT\n" C_RESET);  /*client label*/
    print_line();
    printf(C_DIM "\n     Connecting to host...\n\n" C_RESET);              /*client status*/
    print_line();
}


void show_how_to_play(void) {
    clear_screen();
    printf("\n");
    print_line();
    printf(C_BYELLOW C_BOLD "                  HOW TO PLAY\n" C_RESET);
    print_line();
    printf("\n");

    printf(C_BYELLOW "     MOVES:\n" C_RESET);
    printf("       " C_BGREEN "[1] SUGOD" C_RESET "  — Basic attack\n");
    printf("       " C_BBLUE  "[2] TALIM" C_RESET "  — Special move, unique per breed\n");
    printf("       " C_CYAN   "[3] ILAG " C_RESET "  — Defend: halve incoming dmg next turn\n");
    printf("       " C_YELLOW "[4] BAWI " C_RESET "  — Heal +30 HP (cannot exceed max)\n\n");

    printf(C_BYELLOW "     BREEDS:\n" C_RESET);
    printf("       " C_BWHITE "Banaba " C_RESET "  HP:100  Sugod:25  " C_BBLUE "Tukaa"        C_RESET " — 150%% sugod dmg\n");
    printf("       " C_BWHITE "Jolano " C_RESET "  HP:120  Sugod:22  " C_BBLUE "Kamros"       C_RESET " — 120%% dmg, ignores ilag\n");
    printf("       " C_BWHITE "Kelso  " C_RESET "  HP: 90  Sugod:20  " C_BBLUE "Banta"        C_RESET " — debuffs enemy sugod 50%% (1 turn)\n");
    printf("       " C_BWHITE "Sweater" C_RESET "  HP: 85  Sugod:23  " C_BBLUE "Pakpak Pakak" C_RESET " — 2 hits, 65%% acc, 80%% dmg each\n");

    printf("\n");
    printf(C_BYELLOW "     RULES:\n" C_RESET);
    printf("       • Ilag blocks 50%% damage the NEXT turn only\n");
    printf("       • Kelso's Banta debuff lasts 1 turn\n");
    printf("       • Fight ends when a player reaches 0 HP\n\n");

    press_enter();
}

/*MAIN (testing onlyremove when linking)*/
int main(void) {
    show_title();
    press_enter();
    show_how_to_play();
    return 0;
}