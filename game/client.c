#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

// for breeds and details
#define Max_Breeds 4
typedef enum{
    Breed_Banaba = 0,
    Breed_Jolano = 1,
    Breed_Kelso = 2,
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

// displays HP
void display_hp_client(GameState *state){
    printf("\n--------HP--------\n");
    printf("Server (%s): %d / %d\n", state->p1_breed, state->p1_hp, state->p1_max_hp);
    printf("Client (%s): %d / %d\n", state->p2_breed, state->p2_hp, state->p2_max_hp);
    printf("------------------\n");
}

// function for receiving state (HP and action)
int recv_all(int sock, void *buffer, int length) {
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

int main(int argc, char *argv[]){
    int client_sock, port_no;
    struct sockaddr_in server_addr;

    // check if hostname/IP and port are provided
    if (argc < 3) {
        printf("Usage: %s <IP/hostname> <port>\n", argv[0]);
        exit(1);
    }

    // create TCP socket
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0) 
        die_with_error("Error: socket() Failed.\n");

    //Establish connection to server
    port_no = atoi(argv[2]); // convert port from string to integer
    bzero((char *) &server_addr, sizeof(server_addr)); // clear the struct before using it
    server_addr.sin_family = AF_INET; // set address family to IPv4
    server_addr.sin_port = htons(port_no); // set port (convert to network byte order)

    // Temporary variable for storing IP address
    struct in_addr addr;

    // Try to convert input as direct IP address
    if (inet_pton(AF_INET, argv[1], &addr) == 1) {
        server_addr.sin_addr = addr;
    } 
    else {
        // If not an IP, resolve hostname (e.g., "localhost")
        struct hostent *server = gethostbyname(argv[1]);
        if (server == NULL) {
            die_with_error("Invalid host or IP\n");
        }

        // Copy resolved address into server_addr
        bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
    }

    // connect to server using given IP + port
    if (connect(client_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) 
        die_with_error("Error: connect() Failed.\n");

    printf("Connected to Server (P1)!\n");


    Breed breed_list[Max_Breeds];
    // receive breed list from server
    recv(client_sock, breed_list, sizeof(breed_list), 0);


    // show choices
    int choice;
    printf("\nChoose your breed:\n");
    for(int i = 0; i < Max_Breeds; i++){
        printf("%d = %s\n", i, breed_list[i].name);
    }
    printf("Enter choice: ");
    scanf("%d", &choice);

    // send breed choice to server
    send(client_sock, &choice, sizeof(choice), 0);

    // --- COIN TOSS RESULT ---
    // Receive who goes first: 1 = server first, 2 = client first
    int first_turn;
    recv_all(client_sock, &first_turn, sizeof(first_turn));

    printf("\n=== COIN TOSS RESULT ===\n");
    if (first_turn == 1) {
        printf("Server (P1) won the toss! They go first.\n");
    } else {
        printf("You (Client/P2) won the toss! You go first!\n");
    }

    int action;
    GameState state;

    while (1) {
        // receive current state at start of turn
        if (recv_all(client_sock, &state, sizeof(state)) <= 0) {
            printf("\nConnection lost.\n");
            break;
        }
        display_hp_client(&state);

        // check if someone has 0 hp
        if (state.p1_hp <= 0 || state.p2_hp <= 0)
            break;

        if (first_turn == 1) {
            // ============================
            // SERVER GOES FIRST THIS ROUND
            // ============================

            // receive server action
            if (recv_all(client_sock, &action, sizeof(action)) <= 0) {
                printf("\nConnection lost during server action.\n");
                break;
            }
            printf("\n***Server (P1) played: %d\n", action);

            // receive updated state after server action
            if (recv_all(client_sock, &state, sizeof(state)) <= 0) {
                printf("\nConnection lost during update.\n");
                break;
            }
            display_hp_client(&state);

            // check if someone has 0 hp
            if (state.p1_hp <= 0 || state.p2_hp <= 0)
                break;

            // client turn
            printf("\n1=Sugod | 2=Talim");
            printf("\n3=Ilag  | 4=Bawi");
            printf("\nYour turn: ");
            scanf("%d", &action);
            send(client_sock, &action, sizeof(action), 0);

            // receive updated state after client action
            if (recv_all(client_sock, &state, sizeof(state)) <= 0) {
                printf("\nConnection lost after client action.\n");
                break;
            }

            // check if someone has 0 hp
            if (state.p1_hp <= 0 || state.p2_hp <= 0)
                break;

        } else {
            // ============================
            // CLIENT GOES FIRST THIS ROUND
            // ============================

            // client turn
            printf("\n1=Sugod | 2=Talim");
            printf("\n3=Ilag  | 4=Bawi");
            printf("\nYour turn: ");
            scanf("%d", &action);
            send(client_sock, &action, sizeof(action), 0);

            // receive updated state after client action
            if (recv_all(client_sock, &state, sizeof(state)) <= 0) {
                printf("\nConnection lost after client action.\n");
                break;
            }
            display_hp_client(&state);

            // check if someone has 0 hp
            if (state.p1_hp <= 0 || state.p2_hp <= 0)
                break;

            // receive server action
            if (recv_all(client_sock, &action, sizeof(action)) <= 0) {
                printf("\nConnection lost during server action.\n");
                break;
            }
            printf("\n***Server (P1) played: %d\n", action);

            // receive updated state after server action
            if (recv_all(client_sock, &state, sizeof(state)) <= 0) {
                printf("\nConnection lost during update.\n");
                break;
            }
            display_hp_client(&state);

            // check if someone has 0 hp
            if (state.p1_hp <= 0 || state.p2_hp <= 0)
                break;
        }
    }

    // final result display
    if (state.p2_hp <= 0 && state.p1_hp > 0) {
        printf("\nClient (P2) has been defeated!\n");
        printf("YOU LOST!\n");
        printf("\nADOBO?\n\n");
    } else if (state.p1_hp <= 0 && state.p2_hp > 0) {
        printf("\nServer (P1) has been defeated!\n");
        printf("YOU WON!\n");
        printf("\nADOBO?\n\n");
    } else {
        printf("GAME ENDED.\n");
    }

    close(client_sock);

    return 0;
}
