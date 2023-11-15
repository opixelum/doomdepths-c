#ifndef MENUS_H
#define MENUS_H

#include "database.h"
#include "input.h"
#include "map.h"
#include "monster.h"
#include "pretty-output.h"
#include "terminal.h"

/**
 * @brief Prints main menu. 3 options:
 *     1. New Game;
 *     2. Continue Game;
 *     3. Quit.
 * @param is_running A Pointer to the variable that controls the main loop.
 */
void main_menu(unsigned char *is_running);

/**
 * @brief In battle, asks user to choose between:
 *     1. Attack;
 *     2. Drink potion;
 *     3. Flee.
 * @param player A pointer to the player.
 * @param head A pointer to the head of the monsters list.
 * @return Selected action or 0 on error.
 * @warning It's important to pass the head of the monsters list, otherwise
 * monsters can be skipped.
 */
unsigned char battle_actions_menu(Character *player, Monsters *head);

/**
 * @brief Prompts user to choose a monster to attack among the monsters list.
 * @param character A pointer to the character for printing his stats.
 * @param head A pointer to the head of the monsters list.
 * @return A pointer to the selected monster, NULL on error.
 */
Character *monster_selection_menu(Character *character, Monsters *head);

/**
 * @brief Prompts user to choose between an weapon or a spell.
 * @param player A pointer to the player for printing his stats.
 * @param monster A pointer to the target monster.
 * @return 1 if weapon, 2 if spell, 0 on error.
 */
unsigned char attack_selection_menu(Character *player);

/**
 * @brief Prompts user to write his user name.
 * @return A pointer to the user name.
 * @warning The user name must be freed with free() when it is no longer needed.
 */
char *get_user_name_menu(void);

/**
 * @brief Initializes a new game.
 */
void new_game();

/**
 * @brief Prints a menu for selecting a player's item (either a spell or a
 * potion).
 * @param character A pointer to the character who has the items.
 * @param item_type The type of the item to select (see ItemType enum).
 * @param inventory_menu 1 if it is used in the inventory menu, 0 otherwise.
 * @param loot_menu 1 if it is used in the loot menu, 0 otherwise.
 * @return A pointer to the selected item or NULL if user chooses to come back
 * or if he has no item of the given type.
 */
Item *item_selection_menu
(
    Character *character,
    ItemType item_type,
    unsigned char inventory_menu,
    unsigned char loot_menu
);

/**
 * @brief Display the attacker stats, battle monsters & attack result.
 * @param attacker A pointer to the attacker.
 * @param defender A pointer to the targeted monster.
 * @param monsters A pointer to the head of the monsters list.
 * @param damage_dealt The damage dealt to the defender.
 * @param damage_taken The damage taken by the attacker in return.
 * @param spell A pointer to the spell used to attack the monster, NULL if
 * weapon was used.
 * @return 1 if player looted the monster, 0 otherwise.
 */
unsigned char print_attack_result
(
    Character *attacker,
    Character *defender,
    Monsters *monsters,
    unsigned short damage_dealt,
    unsigned short damage_taken,
    Item *spell
);

/**
 * @brief Prints the inventory of a character.
 * @param player A pointer to the character.
 */
void inventory_menu(Character *player);

/** Displays the inventory of the looted character and asks the looter to choose
 * one or more items to loot.
 * @param looter A pointer to the character that will receive the items.
 * @param looted A pointer to the character that will give the items.
 */
void loot_character_menu(Character *looter, Character *looted);

#endif // MENUS_H
