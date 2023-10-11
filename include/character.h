#ifndef PLAYER_H
#define PLAYER_H

#include "spells.h"
#include "stuff.h"

typedef struct Character
{
    char *name;
    unsigned char level;
    unsigned int xp;
    unsigned int xp_to_next_level;
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
 * @param health The health of the player
 * @param max_health The maximum health of the player
 * @param mana The mana of the player
 * @param max_mana The maximum mana of the player
 * @param gold The gold of the player
 * @param weapon Pointer to the weapon of the player
 * @param armor Pointer to the armor of the player
 * @param spells Pointer to the head of the spells linked list of the player
 * @param inventory Pointer to the head of the inventory linked list of the player
 * @return Pointer to the new player
 * @warning The player must be freed with free_character() when it is no longer needed
 */
Character *create_character
(
    const char *name,
    unsigned char level,
    unsigned int xp,
    unsigned int xp_to_next_level,
    unsigned short health,
    unsigned short max_health,
    unsigned short mana,
    unsigned short max_mana,
    unsigned long gold,
    Item *weapon,
    Item *armor,
    Inventory *spells,
    Inventory *inventory
);

/**
 * @brief Free a character from memory
 * @param character Pointer to the character to be freed
 */
void free_character(Character *character);

#endif // PLAYER_H
