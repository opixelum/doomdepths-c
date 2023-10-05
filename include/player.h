#ifndef PLAYER_H
#define PLAYER_H

#include "spells.h"
#include "stuff.h"

typedef struct Character
{
    char *name;
    unsigned short health;
    unsigned short max_health;
    unsigned short mana;
    unsigned short max_mana;
    unsigned long gold;
    Item *weapon; // Equipped weapon isn't in the inventory
    Item *armor; // Equipped armor isn't in the inventory
    Inventory *spells; // Head of the linked list
    Inventory *inventory; // Head of the linked list
}
Character;

/**
 * @brief Initialize a new player
 * @param name Pointer to the name of the player
 * @return Pointer to the new player
 * @warning The player must be freed with free_character() when it is no longer needed
 */
Character *create_character(char *name);

/**
 * @brief Free a player from memory
 * @param player Pointer to the player to be freed
 */
void free_character1(Character *player);

#endif // PLAYER_H
