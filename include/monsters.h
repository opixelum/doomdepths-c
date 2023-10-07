#ifndef MONSTERS_H
#define MONSTERS_H

#include <string.h>
#include <time.h>

#include "character.h"

/**
 * @brief Linked list of all alive monsters
 */
typedef struct Monsters
{
    Character *monster;
    struct Monsters *next;
}
Monsters;

/**
 * @brief Add an monster to the end of an monsters list
 * @param head Pointer to a node of the monsters list
 * @param monster Pointer to the monster to be added
 * @return Pointer to the given monsters list node or the new monsters list node if the given monsters list node was NULL
 */
Monsters *add_monster_to_list(Monsters *node, Character *monster);

/**
 * @brief Remove an monster from an monsters list
 * @param head Pointer to the head of the monsters list
 * @param monster Pointer to the monster to be removed
 * @return Pointer to the removed monster, or NULL if the monster was not found
 * @warning This function does not free the monster from memory, it only removes it from the monsters list
 * @warning In case you're not passing the head of the monsters list, the monster to remove can be skipped if it is before the
 * given monsters list node
 */
Character *remove_monster_from_list(Monsters *head, Character *monster);

/**
 * @brief Free a linked list of monsters from memory
 * @param head A pointer to the head of the linked list of monsters to be freed
 * @warning Every monster in the linked list will be freed from memory
 * @warning In case you're not passing the head of the monsters list, only the following nodes will be freed
 */
void free_monsters(Monsters *head);

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
