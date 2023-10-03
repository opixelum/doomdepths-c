#ifndef PLAYER_H
#define PLAYER_H

#include "spells.h"
#include "stuff.h"

typedef struct Player
{
    char *name;
    unsigned short health;
    unsigned short max_health;
    unsigned short mana;
    unsigned short max_mana;
    unsigned long gold;
    Item *weapon;
    Item *armor;
    Spells *spells; // Head of the linked list
    Inventory *inventory; // Head of the linked list
}
Player;

#endif // PLAYER_H
