#ifndef MONSTERS_H
#define MONSTERS_H

#include <string.h>
#include <time.h>

#include "character.h"

/**
 * @brief Generate a monster with random stats.
 *
 * - 25% chance to be a Goblin;
 * - 20% chance to be an Orc;
 * - 20% chance to be a Troll;
 * - 15% chance to be an Ogre;
 * - 15% chance to be a Giant;
 * - 5% chance to be a Dragon.
 *
 * Level is a random number between 1 and 100.
 * XP is a random number between 0 and 1000.
 * Gold is a random number between 0 and 1000.
 * Health is calculated with the following formula:
 * health = base_health + base_health * (level / 100)
 * Mana is equal to health.
 *
 * @return Pointer to the new monster
 * @warning The monster must be freed with free_character() when it is no longer needed
 */
Character *generate_random_monster();

#endif // MONSTERS_H
