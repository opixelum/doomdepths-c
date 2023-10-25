#ifndef SPELLS_H
#define SPELLS_H

#include "character.h"

/**
 * @brief Useful function for knowing if we need to display the spell menu or
 * not when the player is fighting.
 * @param character A point to the character.
 * @return The number of attack spells the character has.
 */
unsigned char number_of_attack_spells(Character *character);

#endif // SPELLS_H
