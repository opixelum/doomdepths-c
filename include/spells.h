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

/**
 * @brief Get the number of spells of a given type in a character's spells list.
 * @param character A pointer to the character to check.
 * @param type A type of spell (`ATTACK_SPELL` or `HEAL_SPELL`).
 * @return The number of spells of the given type in the character's spells list.
 */
unsigned char number_of_type_spells(Character *character, ItemType type);

/**
 * @brief Get the first (or only) spell of a given type in a character's spells
 * @param character A pointer to the character to check.
 * @param type A type of spell (`ATTACK_SPELL` or `HEAL_SPELL`).
 * @return A pointer to the first (or only) spell of the given type in the
 * character's spells list or NULL if the character doesn't have any or if the
 * character is NULL.
 */
Item *get_type_spell(Character *character, ItemType type);

#endif // SPELLS_H
