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

/**
 * @brief Useful function for getting the first (or only) attack spell of a
 * character, avoiding to display the spell menu when the player is fighting.
 * @param character A pointer to the character.
 * @return A pointer to an attack spell or NULL if the character doesn't have
 * any or if the character is NULL.
 */
Item *get_attack_spell(Character *character);

#endif // SPELLS_H
