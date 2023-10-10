#ifndef FIGHT_H
#define FIGHT_H

#include "character.h"

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
 */
void attack(Character *attacker, Character *defender);

#endif // FIGHT_H
