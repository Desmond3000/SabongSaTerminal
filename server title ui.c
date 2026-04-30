#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── COLOR CODES ─────────────────────────────────────────── */
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

/*CONSTANTS*/
#define HEAL_AMOUNT 30
#define Max_Breeds   4

/* ── BREED DATA (needed by show_how_to_play) ─────────────── */
typedef enum {
    Breed_Banaba = 0,
    Breed_Jolano = 1,
    Breed_Kelso  = 2,
    Breed_Sweater= 3
} BreedID;

typedef struct {
    BreedID id;
    char name[20];
    char talim_name[40];
    char talim_desc[120];
    int  base_hp;
    int  base_sugod;
} Breed;

Breed breed_list[Max_Breeds] = {
    { Breed_Banaba,  "Banaba",  "Tukaa",       "150% ang dinudulot na Sugod damage",                      100, 25 },
    { Breed_Jolano,  "Jolano",  "Kamros",       "120% ang dinudulot na pinsala, nilalampasan ang ilag",    100, 22 },
    { Breed_Kelso,   "Kelso",   "Banta",        "Binababa ng 50% ang sugod ng kalaban para sa isang turn",  100, 20 },
    { Breed_Sweater, "Sweater", "Pakpak Pakak", "Dalawang beses na pag atake, 65% accuracy, 80% damage kada tama", 100, 23 }
};

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
    printf(C_BYELLOW C_BOLD "                  YOU ARE: HOST (SERVER)\n" C_RESET);
    print_line();
    printf(C_DIM "\n     Waiting for opponent to connect...\n\n" C_RESET);
    print_line();
}

/*HOW TO PLAY*/
void show_how_to_play(void) {
    clear_screen();
    printf("\n");
    print_line();
    printf(C_BYELLOW C_BOLD "                  HOW TO PLAY\n" C_RESET);
    print_line();
    printf("\n");

    printf(C_BYELLOW "     MOVES:\n" C_RESET);
    printf("       " C_BGREEN "[1] SUGOD" C_RESET "  — Basic attack. Dmg = your Sugod stat\n");
    printf("       " C_BBLUE  "[2] TALIM" C_RESET "  — Special move, unique per breed\n");
    printf("       " C_CYAN   "[3] ILAG " C_RESET "  — Defend: halve incoming dmg next turn\n");
    printf("       " C_YELLOW "[4] BAWI " C_RESET "  — Heal +%d HP (cannot exceed max)\n\n", HEAL_AMOUNT);

    printf(C_BYELLOW "     BREEDS & TALIM SPECIALS:\n" C_RESET);
    for (int i = 0; i < Max_Breeds; i++) {
        printf("       " C_BWHITE "%-10s" C_RESET
               " HP:" C_BGREEN  "%-4d" C_RESET
               " Sugod:" C_BYELLOW "%-3d" C_RESET
               "  " C_BBLUE "%-14s" C_RESET "— %s\n",
               breed_list[i].name,
               breed_list[i].base_hp,
               breed_list[i].base_sugod,
               breed_list[i].talim_name,
               breed_list[i].talim_desc);
    }

    printf("\n");
    printf(C_BYELLOW "     RULES:\n" C_RESET);
    printf("       • Ilag blocks 50%% damage the NEXT turn only\n");
    printf("       • Kelso's Banta debuff lasts 1 turn\n");
    printf("       • Fight ends when a player reaches 0 HP\n\n");

    press_enter();
}

/*(testing onlyremove when linking)*/
int main(void) {
    show_title();
    press_enter();
    show_how_to_play();
    return 0;
}