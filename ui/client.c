#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

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

/*CONSTANTS*/
#define Max_Breeds 4

// for breeds and details
typedef enum{
    Breed_Banaba  = 0,
    Breed_Jolano  = 1,
    Breed_Kelso   = 2,
    Breed_Sweater = 3
} BreedID;

#pragma pack(push, 1)
typedef struct{
    BreedID id;
    char name[20];
    char talim_name[40];
    char talim_desc[120];
    int base_hp;
    int base_sugod;
} Breed;
#pragma pack(pop)

// displaying breed type in interface
#pragma pack(push, 1)
typedef struct {
    int p1_hp;
    int p2_hp;
    int p1_max_hp;
    int p2_max_hp;
    char p1_breed[20];
    char p2_breed[20];
} GameState;
#pragma pack(pop)

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
    printf(C_BBLUE C_BOLD "                  YOU ARE: CLIENT\n" C_RESET);
    print_line();
    printf(C_DIM "\n     Connecting to host...\n\n" C_RESET);
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
    printf("       " C_BGREEN "[1] SUGOD" C_RESET "  — Basic attack\n");
    printf("       " C_BBLUE  "[2] TALIM" C_RESET "  — Special move, unique per breed\n");
    printf("       " C_CYAN   "[3] ILAG " C_RESET "  — Defend: halve incoming dmg next turn\n");
    printf("       " C_YELLOW "[4] BAWI " C_RESET "  — Heal +30 HP (cannot exceed max)\n\n");

    printf(C_BYELLOW "     BREEDS:\n" C_RESET);
    printf("       " C_BWHITE "Banaba " C_RESET "  HP:100  Sugod:25  " C_BBLUE "Tukaa"        C_RESET " — 150%% sugod dmg\n");
    printf("       " C_BWHITE "Jolano " C_RESET "  HP:100  Sugod:22  " C_BBLUE "Kamros"       C_RESET " — 120%% dmg, ignores ilag\n");
    printf("       " C_BWHITE "Kelso  " C_RESET "  HP:100  Sugod:20  " C_BBLUE "Banta"        C_RESET " — debuffs enemy sugod 50%% (1 turn)\n");
    printf("       " C_BWHITE "Sweater" C_RESET "  HP:100  Sugod:23  " C_BBLUE "Pakpak Pakak" C_RESET " — 2 hits, 65%% acc, 80%% dmg each\n");

    printf("\n");
    printf(C_BYELLOW "     RULES:\n" C_RESET);
    printf("       • Ilag blocks 50%% damage the NEXT turn only\n");
    printf("       • Kelso's Banta debuff lasts 1 turn\n");
    printf("       • Fight ends when a player reaches 0 HP\n\n");

    press_enter();
}

/*DISPLAY & NETWORK HELPERS*/
// displays HP
void display_hp_client(GameState *state){
    printf("\n");
    print_line();
    printf("     " C_BYELLOW "SERVER" C_RESET " (%s): " C_BGREEN "%d" C_RESET " / %d\n",
           state->p1_breed, state->p1_hp, state->p1_max_hp);
    printf("     " C_BBLUE   "CLIENT" C_RESET " (%s): " C_BGREEN "%d" C_RESET " / %d\n",
           state->p2_breed, state->p2_hp, state->p2_max_hp);
    print_line();
}

// function for receiving state (HP and action)
int recv_all(int sock, void *buffer, int length){
    int total = 0;
    while (total < length) {
        int n = recv(sock, (char*)buffer + total, length - total, 0);
        if (n <= 0) return n;
        total += n;
    }
    return total;
}

// function for displaying error message and terminating program
void die_with_error(char *error_msg){
    printf("%s", error_msg);
    exit(-1);
}

/*MAIN*/
int main(int argc, char *argv[]){
    int client_sock, port_no;
    struct sockaddr_in server_addr;

    // check if hostname/IP and port are provided
    if (argc < 3) {
        printf("Usage: %s <IP/hostname> <port>\n", argv[0]);
        exit(1);
    }

    // Title screen shown before connecting
    show_title();

    // create TCP socket
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0)
        die_with_error("Error: socket() Failed.\n");

    // Establish connection to server
    port_no = atoi(argv[2]); // convert port from string to integer
    bzero((char *)&server_addr, sizeof(server_addr)); // clear the struct before using it
    server_addr.sin_family = AF_INET;        // set address family to IPv4
    server_addr.sin_port   = htons(port_no); // set port (convert to network byte order)

    // Temporary variable for storing IP address
    struct in_addr addr;

    // Try to convert input as direct IP address
    if (inet_pton(AF_INET, argv[1], &addr) == 1) {
        server_addr.sin_addr = addr;
    } else {
        // If not an IP, resolve hostname (e.g., "localhost")
        struct hostent *server = gethostbyname(argv[1]);
        if (server == NULL)
            die_with_error("Invalid host or IP\n");

        // Copy resolved address into server_addr
        bcopy((char *)server->h_addr,
              (char *)&server_addr.sin_addr.s_addr,
              server->h_length);
    }

    // connect to server using given IP + port
    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        die_with_error("Error: connect() Failed.\n");

    // Connected: show HTP then start
    printf(C_BGREEN "\n     Connected to Server (P1)!\n" C_RESET);
    press_enter();

    show_how_to_play();

    Breed breed_list[Max_Breeds];
    // receive breed list from server (only once, before the loop)
    recv(client_sock, breed_list, sizeof(breed_list), 0);

    int play_again_flag = 1;

    do {
        // breed choice
        clear_screen();
        printf("\n");
        print_line();
        printf(C_BBLUE C_BOLD "                  CHOOSE YOUR BREED\n" C_RESET);
        print_line();
        printf("\n");
        for (int i = 0; i < Max_Breeds; i++){
            printf("       " C_CYAN "[%d]" C_RESET " %-10s"
                   " HP:" C_BGREEN "%-4d" C_RESET
                   " Sugod:" C_BYELLOW "%d\n" C_RESET,
                   i, breed_list[i].name,
                   breed_list[i].base_hp,
                   breed_list[i].base_sugod);
        }
        printf("\n");
        print_line();
        printf(C_BBLUE "\n     > " C_RESET);

        int choice;
        scanf("%d", &choice);

        // send breed choice to server
        send(client_sock, &choice, sizeof(choice), 0);

        // coin flip result
        int first_turn;
        recv_all(client_sock, &first_turn, sizeof(first_turn));

        printf("\n");
        print_line();
        printf(C_BYELLOW C_BOLD "                   COIN FLIP\n" C_RESET);
        print_line();
        if (first_turn == 1)
            printf(C_BYELLOW "\n     Server (P1) won the toss! Server goes first.\n" C_RESET);
        else
            printf(C_BGREEN  "\n     Client (P2) won the toss! You go first!\n" C_RESET);
        press_enter();

        int action;
        GameState state;

        while (1) {
            // receive initial state at start of turn, if fail print error message
            if (recv_all(client_sock, &state, sizeof(state)) <= 0) {
                printf("\n     Connection lost.\n");
                goto cleanup;
            }
            display_hp_client(&state);

            // checks if someone has 0 hp
            if (state.p1_hp <= 0 || state.p2_hp <= 0) break;

            if (first_turn == 1) {
                // receive server action, if fail print error message
                if (recv_all(client_sock, &action, sizeof(action)) <= 0) {
                    printf("\n     Connection lost during server action.\n");
                    goto cleanup;
                }
                printf(C_BYELLOW "\n     *** Server (P1) played: %d\n" C_RESET, action);

                // receive updated state after server action, if fail print error message
                if (recv_all(client_sock, &state, sizeof(state)) <= 0) {
                    printf("\n     Connection lost during update.\n");
                    goto cleanup;
                }
                display_hp_client(&state);

                // checks if someone has 0 hp
                if (state.p1_hp <= 0 || state.p2_hp <= 0) break;

                // client turn
                printf("\n     " C_BGREEN "[1] SUGOD" C_RESET
                         "  |  " C_BBLUE "[2] TALIM\n" C_RESET);
                printf("     " C_CYAN   "[3] ILAG " C_RESET
                         "  |  " C_YELLOW "[4] BAWI\n" C_RESET);
                printf(C_BBLUE "\n     Your turn: " C_RESET);
                scanf("%d", &action);
                send(client_sock, &action, sizeof(action), 0);

                // receive updated state after client action, if fail print error message
                if (recv_all(client_sock, &state, sizeof(state)) <= 0) {
                    printf("\n     Connection lost after client action.\n");
                    goto cleanup;
                }

                // checks if someone has 0 hp
                if (state.p1_hp <= 0 || state.p2_hp <= 0) break;
            }
            else {
                // client turn
                printf("\n     " C_BGREEN "[1] SUGOD" C_RESET
                         "  |  " C_BBLUE "[2] TALIM\n" C_RESET);
                printf("     " C_CYAN   "[3] ILAG " C_RESET
                         "  |  " C_YELLOW "[4] BAWI\n" C_RESET);
                printf(C_BBLUE "\n     Your turn: " C_RESET);
                scanf("%d", &action);
                send(client_sock, &action, sizeof(action), 0);

                // receive updated state after client action
                if (recv_all(client_sock, &state, sizeof(state)) <= 0) {
                    printf("\n     Connection lost after client action.\n");
                    goto cleanup;
                }
                display_hp_client(&state);

                // check if someone has 0 hp
                if (state.p1_hp <= 0 || state.p2_hp <= 0) break;

                // receive server action
                if (recv_all(client_sock, &action, sizeof(action)) <= 0) {
                    printf("\n     Connection lost during server action.\n");
                    goto cleanup;
                }
                printf(C_BYELLOW "\n     *** Server (P1) played: %d\n" C_RESET, action);

                // receive updated state after server action
                if (recv_all(client_sock, &state, sizeof(state)) <= 0) {
                    printf("\n     Connection lost during update.\n");
                    goto cleanup;
                }
                display_hp_client(&state);

                // check if someone has 0 hp
                if (state.p1_hp <= 0 || state.p2_hp <= 0) break;
            }
        }

        // final result display
        printf("\n");
        print_line();
        if (state.p2_hp <= 0 && state.p1_hp > 0) {
            printf(C_RED C_BOLD "     YOU LOST! Client (P2) has been defeated!\n" C_RESET);
            printf(C_DIM "     ADODO?\n" C_RESET);
        } else if (state.p1_hp <= 0 && state.p2_hp > 0) {
            printf(C_BGREEN C_BOLD "     YOU WON! Server (P1) has been defeated!\n" C_RESET);
            printf(C_DIM "     ADODO?\n" C_RESET);
        } else {
            printf(C_DIM "     GAME OVER.\n" C_RESET);
        }
        print_line();

        // play again
        // server sends choice first, then client sends
        // if both says no, game over
        int server_wants, client_wants;
        char client_ans;

        if (recv_all(client_sock, &server_wants, sizeof(server_wants)) <= 0) {
            printf("\n     Connection lost.\n");
            break;
        }

        printf(C_BYELLOW "\n     Play again? (y/n): " C_RESET);
        scanf(" %c", &client_ans);
        client_wants = (client_ans == 'y' || client_ans == 'Y') ? 1 : 0;

        send(client_sock, &client_wants, sizeof(client_wants), 0);

        play_again_flag = server_wants && client_wants;

        if (!play_again_flag) {
            printf(C_DIM "\n     %s\n" C_RESET,
                   !client_wants ? "ayaw na ni P2" : "ayaw na ni P1");
        } else {
            printf(C_BGREEN "\n     REMATCH!!\n" C_RESET);
            press_enter();
        }

    } while (play_again_flag);

cleanup:
    close(client_sock);
    return 0;
}
