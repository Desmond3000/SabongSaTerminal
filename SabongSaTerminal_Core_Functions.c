#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
CONSTANT -  Max number of breads (currently listed breeds) and 
            Heal Amount (+30)

            Since this cannot be changed
*/

#define Max_Breeds 4
#define Am_Heal 30

/*
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

/*
Structs -   Under is the breed template, the player state 
            during the game, and the result of the action made
            by the player
*/
//Breed 
typedef struct{
    BreedID id;
    char name[10];
    char talim_name[20];
    char talim_desc[60];
    int base_hp;
    int base_sugod;
} Breed;
//Player
typedef struct{
    char name[10];
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
    int F_hit;
    int S_hit;
    int debuff_app;
    int ilag_blocked;
    int sugod_w_debuffed;
} ActResult;

typedef breed_list[Max_Breeds]={
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
        120,22
    },
    {
        Breed_Kelso,
        "Kelso",
        "Banta",
        "Binababa ng 50% ang sugod ng kalaban para sa isang turn",
        90, 20
    },
    {
        Breed_Sweater,
        "Pakpak Pakak",
        "Dalawang beses na pag atake, 65% na accuracy and 80% damage kada tama",
        85,23
    }
};

/*
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

/*
Player Setup
init_player() -     Sets player with their chosen breed.
                    Call this after the player picks a breed.
*/

void init_player(Player *p, const char *name, BreedID breed_id){
    strncpy(p->name, name, 29);
    p->name[19] = '\0';
    p->breed = breed_list[breed_id];
    p->hp = p->breed.base_hp;
    p->max_hp = p->breed.base_hp;
    p->sugod = p->breed.base_sugod;
    p->defended = 0;
    p->sugod_debuffed = 0;
}

/*
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

    switch (attacker->breed.id): {
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
            if(dmg < 0) dmhg = 0;
            defender->hp -= dmg;
            result.damage_dealt = dmg;
            break;
        }
        case Breed_Kelso:{
            defender->sugod_debuffed = 1;
            result.debuff_app = 1;
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
            if (total < 0) total = 0;
            defender->hp -= total;
            result.damage_dealt = total;
            break
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

/*
resolve_action -    taking the players' chosen action then calling 
                    the action that is appropriate
*/

ActionResult resolve_action(Action action, Player *attacker, Player *defender) {
    ActionResult result;
    memset(&result, 0, sizeof(result));
 
    switch (action) {
        case ACTION_SUGOD: result = do_sugod(attacker, defender); break;
        case ACTION_TALIM: result = do_talim(attacker, defender); break;
        case ACTION_ILAG:  result = do_ilag(attacker);            break;
        case ACTION_BAWI:  result = do_bawi(attacker);            break;
        default: break;
    }
 
    return result;
}

/*
Clear_ilag: clears the defender's ilag flag after the offensive action.
            This is called right after the resolve_action() when action != Action_ilag
*/

void clear_ilag(Player *defender){
    defender->defended = 0;
}

/*
getting winner(): pointer to winner unless still ongoing
*/
Player *get_winner(Player *p1, Player *p2) {
    if (is_dead(p2)) return p1;
    if (is_dead(p1)) return p2;
    return NULL;
}

/*
coin_flip(): for the first turn
*/

int coin_flip(int p1_call) {
    int result = rand() % 2; // 1 = Heads, 0 = Tails
    return (result == p1_call) ? 1 : 2;
}

/*
ETO NA INT MAIN AAARRRRRGGGHHH
*/

int main() {
    srand((unsigned int)time(NULL));
 
    printf("SABONG SA ATONG TERMINAL\n\n");
 
    // Init two players
    Player p1, p2;
    init_player(&p1, "Player1", BREED_BANABA);
    init_player(&p2, "Player2", BREED_SWEATER);
 
    printf("P1: %s | Breed: %s | HP: %d | Sugod: %d\n",
           p1.name, p1.breed.name, p1.hp, p1.sugod);
    printf("P2: %s | Breed: %s | HP: %d | Sugod: %d\n\n",
           p2.name, p2.breed.name, p2.hp, p2.sugod);
 
    // Coin flip
    int first = coin_flip(1); // P1 calls Heads
    printf("Coin flip result: Player %d goes first!\n\n", first);
 
    // Simulate a few turns
    Player *attacker = (first == 1) ? &p1 : &p2;
    Player *defender = (first == 1) ? &p2 : &p1;
 
    int turn = 1;
    while (!is_dead(&p1) && !is_dead(&p2) && turn <= 5) {
        printf("--- Turn %d: %s attacks ---\n", turn, attacker->name);
 
        Action action = (turn % 2 == 0) ? ACTION_TALIM : ACTION_SUGOD;
        ActionResult r = resolve_action(action, attacker, defender);
 
        if (action != ACTION_ILAG) clear_ilag(defender);
 
        printf("  Action: %s\n",     action == ACTION_SUGOD ? "Sugod" : "Talim");
        printf("  Damage dealt: %d\n", r.damage_dealt);
        printf("  Healed: %d\n",       r.healed);
        printf("  Debuff applied: %d\n", r.debuff_applied);
        printf("  Hit1: %d | Hit2: %d\n", r.hit1, r.hit2);
        printf("  %s HP: %d | %s HP: %d\n\n",
               p1.name, p1.hp, p2.name, p2.hp);
 
        // Swap turns
        Player *tmp = attacker;
        attacker    = defender;
        defender    = tmp;
        turn++;
    }
 
    // Check winner
    Player *winner = get_winner(&p1, &p2);
    if (winner)
        printf("PANALO: %s (%s)!\n", winner->name, winner->breed.name);
    else
        printf("Manatling maghintay dahil patuloy pa rin ang laban!\n");
 
    return 0;
}