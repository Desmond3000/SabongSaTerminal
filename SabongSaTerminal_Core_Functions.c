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
        "Dalawang beses na pag atake, 65% na accuracy and 80% damagae kada tama",
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

}

ActResult do_sugod(Player *attacker, Player *defender){

}

ActResult do_ilag(Player *p){

}

ActResult do_bawi(Player *p){

}

