#ifndef FIGHT_H
#define FIGHT_H

#include "string.h"

#include "monsters.h"

/**
 * @brief Attack a defender with an attacker
 *
 * Damage is calculated with the following formula:
 * damage = attacker_weapon_damage - defender_armor_value
 *
 * If the attacker doesn't have a weapon, it depends on who is the attacker:
 *  - Player: 5;
 *  - Goblin: 5;
 *  - Orc: 20;
 *  - Troll: 30;
 *  - Ogre: 40;
 *  - Giant: 50;
 *  - Dragon: 70.
 *
 * @param attacker Pointer to the attacker
 * @param defender Pointer to the defender
 * @return The damage dealt to the defender
 */
unsigned short attack(Character *attacker, Character *defender);

/**
 * @brief Update monsters list if a monster is dead
 * @param head Pointer to the head of the monsters list
 * @return Pointer to the head of the monsters list
 */
Monsters *update_monsters_list(Monsters *head);

/**
 * @brief Battle between the player and the monsters
 * @param player Pointer to the player
 * @param monsters Pointer to the monsters list head (will be updated)
 * @return 0 if the player is dead, 1 otherwise
 */
unsigned char battle(Character *player, Monsters *monsters);

#endif // FIGHT_H
