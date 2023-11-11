#ifndef FIGHT_H
#define FIGHT_H

#include "menu.h"
#include "monster.h"
#include "pretty-output.h"
#include "spell.h"
#include "terminal.h"

/**
 * @brief Attack a defender with an attacker.
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
 * @param attacker A pointer to the attacker.
 * @param defender A pointer to the defender.
 * @param spell A pointer to the spell used by the attacker or NULL if the
 * attacker uses his weapon or his fists.
 * @return The damage dealt to the defender.
 */
unsigned short attack(Character *attacker, Character *defender, Item *spell);

/**
 * @brief Battle between the player and a random number of monsters. Monsters
 * are generated randomly & free after the battle.
 * @param player A pointer to the player.
 */
void battle(Character *player);

/**
 * @brief Randomly trigger a battle (1/10 chance)
 * @param A pointer to the player
 */
void random_battle_trigger(Character *player);
Monsters *flee(Monsters *monsters, Character *player);
/**
 * @brief Prompts user to choose an attack type, then executes the attack &
 * prints the turn summary.
 * @param attacker A pointer to the character who attacks.
 * @param defender A pointer to the targeted character.
 * @param monsters A pointer to the head of the monsters list.
 * @return A pointer to the head of the updated monsters list, NULL if all
 * monsters died.
 * @warning It's important to pass the head of the monsters list, otherwise
 * monsters can be skipped.
 */
Monsters *perform_attack
(
    Character *attacker,
    Character *defender,
    Monsters *monsters
);

#endif // FIGHT_H
