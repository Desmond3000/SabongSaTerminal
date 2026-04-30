#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>



/***************************
CONSTANT -  Max number of breads (currently listed breeds) and 
            Heal Amount (+30)

            Since this cannot be changed
*/
#define Max_Breeds 4
#define Am_Heal 30



/***************************
ENUMS -     For the actions and Breeds of the game
            Makes names as number for easier reading
*/
//Player actions each 
typedef enum{
    Act_Sugod = 1,
    Act_Talim = 2,
    Act_Ilag = 3, 
    Act_Bawi = 4
} Action;

//Breed identifiers 
typedef enum{
    Breed_Banaba = 0,
    Breed_Jolano = 1,
    Breed_Kelso = 2,
    Breed_Sweater = 3
} BreedID;



/***************************
Structs -   Under is the breed template, the player state 
            during the game, and the result of the action made
            by the player
*/
//Breed 
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
//Player
typedef struct{
    char name[20];
    Breed breed;
    int hp;
    int max_hp;
    int sugod;
    int defended;
    int sugod_debuffed;
} Player;
//Result
typedef struct{
    int damage_dealt;
    int healed;
    int hit1;
    int hit2;
    int debuff_applied;
    int ilag_blocked;
    int sugod_w_debuffed;
} ActResult;
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

Breed breed_list[Max_Breeds]={
    {
        Breed_Banaba,
        "Banaba",
        "Tukaa",
        "150% ang dinudulot na Sugod damage",
        100, 25
    },
    {
        Breed_Jolano,
        "Jolano",
        "Kamros",
        "120% ang dinudulot na pinsala, nilalampasan ang ilag",
        100,22
    },
    {
        Breed_Kelso,
        "Kelso",
        "Banta",
        "Binababa ng 50% ang sugod ng kalaban para sa isang turn",
        100, 20
    },
    {
        Breed_Sweater,
        "Sweater",
        "Pakpak Pakak",
        "Dalawang beses na pag atake, 65% na accuracy and 80% damage kada tama",
        100,23
    }
};



/***************************
Utility Functions -     Helper Functions that bigger combat functions use repeatedly so that we dont need
                        to rewrite the same code over and over again
*/
//get_breed - Returns a pointer to a breed by its breedID
Breed *get_breed(BreedID id){
    return &breed_list[id];
}

//roll_percent() - returns random number (1-100) for accuracy checks
int roll_percent(){
    return(rand()%100) + 1;
}

//clamp_hp() - Make sure hp stays within 0 and max health bar
void clamp_hp(Player *p){
    if(p->hp > p->max_hp){
        p->hp = p->max_hp;
    }
    if(p->hp < 0){
        p->hp = 0;
    }
}



/***************************
Player Setup
init_player() -     Sets player with their chosen breed.
                    Call this after the player picks a breed.
*/
void init_player(Player *p, const char *name, BreedID breed_id){
    strncpy(p->name, name, sizeof(p->name) - 1);
    p->name[sizeof(p->name) - 1] = '\0';
    p->breed = breed_list[breed_id];
    p->hp = p->breed.base_hp;
    p->max_hp = p->breed.base_hp;
    p->sugod = p->breed.base_sugod;
    p->defended = 0;
    p->sugod_debuffed = 0;
}



/***************************
Combat Functions -  Here we get the functions for combat
*/
ActResult do_talim(Player *attacker, Player *defender){
    ActResult result;
    memset(&result, 0, sizeof(result));
    
    int atk = attacker->sugod;

    //To check if na debuff ng banta ang attacker
    if (attacker->sugod_debuffed){
        atk = atk / 2;
        result.sugod_w_debuffed = 1;
        attacker->sugod_debuffed = 0;
    }

    switch (attacker->breed.id) {
        case Breed_Banaba:{
            int dmg = (atk * 150)/100;
            if(defender->defended){
                dmg = dmg-(dmg/2);
                result.ilag_blocked = 1;
            }
            if (dmg < 0) dmg = 0;
            defender->hp -= dmg;
            result.damage_dealt = dmg;
            break;
        }
        case Breed_Jolano:{
            int dmg = (atk * 120) / 100;
            if(dmg < 0) dmg = 0;
            defender->hp -= dmg;
            result.damage_dealt = dmg;
            break;
        }
        case Breed_Kelso:{
            defender->sugod_debuffed = 1;
            result.debuff_applied = 1;
            result.damage_dealt = 0;
            break;
        }
        case Breed_Sweater:{
            int per_hit = (atk * 80) /100;
            result.hit1 = roll_percent() <= 65;
            result.hit2 = roll_percent() <= 65;

            int total = 0;
            if(result.hit1) {
                int d = per_hit;
                if(defender->defended){
                    d -= d / 2;
                    result.ilag_blocked = 1;
                }
                total += d;
            }
            if(result.hit2) {
                int d = per_hit;
                if(defender->defended){
                    d -= d / 2;
                    result.ilag_blocked = 1;
                }
                total += d;
            }
            if (total < 0) total = 0;
            defender->hp -= total;
            result.damage_dealt = total;
            break;
        }
    }
    clamp_hp(defender);
    return result;
}

ActResult do_sugod(Player *attacker, Player *defender){
    ActResult result;
    memset(&result, 0, sizeof(result));
    int atk = attacker->sugod;

    //Chinecheck if na debuff ng banta ang attacker
    if(attacker->sugod_debuffed){
        atk = atk/2;
        result.sugod_w_debuffed = 1;
        attacker->sugod_debuffed = 0;
    }

    int dmg = atk;

    if(defender->defended){
        dmg = atk - (atk/2);
        result.ilag_blocked = 1;
    }

    if(dmg < 0) dmg = 0;

    defender->hp -= dmg;
    result.damage_dealt = dmg;

    clamp_hp(defender);
    return result;
}

ActResult do_ilag(Player *p){
    ActResult result;
    memset(&result, 0, sizeof(result));
    p->defended = 1;
    return result;
}

ActResult do_bawi(Player *p){
    ActResult result;
    memset(&result, 0, sizeof(result));
    int before = p->hp;
    p->hp += Am_Heal;

    clamp_hp(p);
    result.healed = p->hp - before;
    return result;
}



/***************************
resolve_action -    taking the players' chosen action then calling 
                    the action that is appropriate
*/
ActResult resolve_action(Action action, Player *attacker, Player *defender) {
    ActResult result;
    memset(&result, 0, sizeof(result));
 
    switch (action) {
        case Act_Sugod: result = do_sugod(attacker, defender); break;
        case Act_Talim: result = do_talim(attacker, defender); break;
        case Act_Ilag:  result = do_ilag(attacker);            break;
        case Act_Bawi:  result = do_bawi(attacker);            break;
        default: break;
    }
 
    return result;
}



/***************************
Clear_ilag: clears the defender's ilag flag after the offensive action.
            This is called right after the resolve_action() when action != Action_ilag
*/
void clear_ilag(Player *defender){
    defender->defended = 0;
}

// function for when player hp is 0
int is_dead(Player *p){
    return p->hp <= 0;
}

// displays HP
void display_hp(Player *p1, Player *p2){
    printf("\n--------HP--------\n");
    printf("Server (%s): %d / %d\n", p1->breed.name, p1->hp, p1->max_hp);
    printf("Client (%s): %d / %d\n", p2->breed.name, p2->hp, p2->max_hp);
    printf("------------------\n");
}



/***************************
getting winner(): pointer to winner unless still ongoing
*/
Player *get_winner(Player *p1, Player *p2) {
    if (is_dead(p2)) return p1;
    if (is_dead(p1)) return p2;
    return NULL;
}



/***************************
coin_flip():    Server picks a side (0=Heads, 1=Tails).
                Client sends their call.
                Returns 1 if server wins, 2 if client wins.
*/
int coin_flip(int client_sock) {
    srand(time(NULL));
    int result = rand() % 2; // 0 = Heads, 1 = Tails

    // server chooses between heads or tails
    int server_call;
    printf("\ncoin flip\n");
    printf("0 = Heads | 1 = Tails\n");
    printf("Your call: ");
    scanf("%d", &server_call);

    // show the result
    printf("Coin landed on: %s\n", result == 1 ? "Tails" : "Heads");

    int server_wins = (server_call == result) ? 1 : 0;

    // send result to client: 1 = server goes first, 2 = client goes first
    int first_turn = server_wins ? 1 : 2;
    send(client_sock, &first_turn, sizeof(first_turn), 0);

    if (server_wins) {
        printf("Server (P1) won the toss! \nYou go first.\n");
    }
    else {
        printf("Client (P2) won the toss! \nClient goes first.\n");
    }

    return first_turn;
}



/***************************
Sending and Receiving State (HP and Action)
*/
// receiving state and action
int recv_all(int sock, void *buffer, int length) {
    int total = 0;
    while (total < length) {
        int n = recv(sock, (char*)buffer + total, length - total, 0);
        if (n <= 0) return n;
        total += n;
    }
    return total;
}

// sending state
void send_state(int sock, Player *p1, Player *p2) {
    GameState state;
    state.p1_hp = p1->hp;
    state.p2_hp = p2->hp;
    state.p1_max_hp = p1->max_hp;
    state.p2_max_hp = p2->max_hp;
    strncpy(state.p1_breed, p1->breed.name, sizeof(state.p1_breed) - 1);
    state.p1_breed[19] = '\0';
    strncpy(state.p2_breed, p2->breed.name, sizeof(state.p2_breed) - 1);
    state.p2_breed[19] = '\0';
    send(sock, &state, sizeof(state), 0);
}



/*
ETO NA INT MAIN AAARRRRRGGGHHH
*/



// function for displaying error message and terminating program
void die_with_error(char *error_msg){
    printf("%s", error_msg);
    exit(-1);
}

int main(int argc, char *argv[]){
    int server_sock, client_sock, port_no, client_size;
    struct sockaddr_in server_addr, client_addr;

    // check if port number is provided
    if (argc < 2) {
        printf("Usage: %s port_no\n", argv[0]);
        exit(1);
    }

    // Create a socket for incoming connections
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) 
       die_with_error("Error: socket() Failed.\n");

    // Bind socket to a port
    bzero((char *) &server_addr, sizeof(server_addr));
    port_no = atoi(argv[1]);
    server_addr.sin_family = AF_INET; // Internet address 
    server_addr.sin_addr.s_addr = INADDR_ANY; // Any incoming interface
    server_addr.sin_port = htons(port_no); // Local port

    //bind socket to the specific port
    if (bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) 
       die_with_error("Error: bind() Failed.\n");

    // Mark the socket so it will listen for incoming connections
    listen(server_sock, 5);

    // Accept client connection
    client_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_size);
    if (client_sock < 0) 
        die_with_error("Error: accept() Failed.");

    printf("Client (P2) connected!\n");

    // send breed list to client
    send(client_sock, breed_list, sizeof(breed_list), 0);

    
    // GAME SETUP SECTION
    // breed choice
    Player p1, p2;

    int p1_choice, p2_choice;

    // Server picks breed
    printf("\nChoose your breed:\n");
    for(int i = 0; i < Max_Breeds; i++){
        printf("%d = %s\n", i, breed_list[i].name);
    }
    printf("Enter choice: ");
    scanf("%d", &p1_choice);

    if (p1_choice < 0 || p1_choice >= Max_Breeds) {
        die_with_error("Invalid breed choice.\n");
    }

    // Client picks breed
    recv(client_sock, &p2_choice, sizeof(p2_choice), 0);

    if (p2_choice < 0 || p2_choice >= Max_Breeds) {
        die_with_error("Client chose invalid breed.\n");
    }

    // initialize players with breed
    init_player(&p1, "Server", p1_choice);
    init_player(&p2, "Client", p2_choice);

    // pointers used to control turns
//    Player *attacker = &p1;
//    Player *defender = &p2;


    // coin flip for who takes first turn
    // first_turn: 1 = server, 2 = client
    int first_turn = coin_flip(client_sock);


    // game loop, turn systeem
    while (!is_dead(&p1) && !is_dead(&p2)) {
        int action;

        display_hp(&p1, &p2);

        // send current state to client
        send_state(client_sock, &p1, &p2);

        // server wins coin flip
        if (first_turn == 1){
            // phase 1: server turn
            printf("\n1=Sugod | 2=Talim");
            printf("\n3=Ilag  | 4=Bawi");
            printf("\nYour turn: ");
            scanf("%d", &action);

            resolve_action(action, &p1, &p2);
            if (action != Act_Ilag)
                clear_ilag(&p2);

            // send server action to client
            send(client_sock, &action, sizeof(action), 0);

            // send updated state after server action
            send_state(client_sock, &p1, &p2);

            display_hp(&p1, &p2);

            // if client is dead, announce winner
            if (is_dead(&p2)) {
                printf("\nClient (P2) has been defeated!\n");
                printf("YOU WON!\n");
                break;
            }

            // phase 2: client turn
            recv_all(client_sock, &action, sizeof(action));
            printf("\n***Client (P2) played: %d\n", action);

            resolve_action(action, &p2, &p1);
            if (action != Act_Ilag)
                clear_ilag(&p1);

            // send updated state after client action
            send_state(client_sock, &p1, &p2);

    //        display_hp(&p1, &p2);

            // if server is dead, announce winner
            if (is_dead(&p1)) {
                printf("\nServer (P1) has been defeated!\n");
                printf("YOU LOST!\n");
                break;
            }
        }
        else{ // client wins coin flip
            // phase 1: client turn
            recv_all(client_sock, &action, sizeof(action));
            printf("\n***Client (P2) played: %d\n", action);

            resolve_action(action, &p2, &p1);
            if (action != Act_Ilag)
                clear_ilag(&p1);

            // send updated state after client action
            send_state(client_sock, &p1, &p2);

            display_hp(&p1, &p2);

            // if server is dead, end round
            if (is_dead(&p1)) {
                printf("\nServer (P1) has been defeated!\n");
                printf("YOU LOST!\n");
                break;
            }

            // phase 2: server turn
            printf("\n1=Sugod | 2=Talim");
            printf("\n3=Ilag  | 4=Bawi");
            printf("\nYour turn: ");
            scanf("%d", &action);

            resolve_action(action, &p1, &p2);
            if (action != Act_Ilag)
                clear_ilag(&p2);

            // send server action to client
            send(client_sock, &action, sizeof(action), 0);

            // send updated state after server action
            send_state(client_sock, &p1, &p2);

            // if client is dead, end round
            if (is_dead(&p2)) {
                printf("\nClient (P2) has been defeated!\n");
                printf("YOU WON!\n");
                break;
            }
        }
    }

    // GAME END
    printf("\nTARA I-TINOLA NA YAN\n\n");

    close(client_sock);
    close(server_sock);

    return 0;
}