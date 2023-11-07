#ifndef MENUS_H
#define MENUS_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "character.h"
#include "map.h"
#include "monsters.h"
#include "save.h"

/**
 * @brief Cross-platform function to clear console screen.
 *
 * Prints a regex where:
 * - "\e[1;1H" moves cursor to top left corner of terminal screen;
 * - "\e[2J" replaces all characters of the terminal screen with spaces.
 */
void clear_screen(void);

/**
 * @brief Cross-platform function to clear a given number of console lines.
 *
 * Prints a regex where:
 * - "\x1b[1F" moves cursor to beginning of previous line;
 * - "\x1b[2K" replaces all characters of this line with spaces.
 *
 * @param number_of_lines Number of lines to clear
 */
void clear_lines(unsigned int number_of_lines);

/**
 * @brief Clears stdin buffer (useful before getting user input)
 *
 * When calling getchar(), it clears a character from stdin buffer.
 * While it's not a newline character, or EOF, it keeps clearing characters.
 */
void clear_stdin(void);

/**
 * @brief Waits for user to press any key.
 */
void press_any_key_to_continue(void);

/**
 * @brief Waits for user to press [ENTER] key.
 */
void press_enter_to_continue(void);

/**
 * @brief Get the user input from stdin
 *
 * @return Pointer to the string or character containing user input
 */
char *get_string(void);

/**
 * @brief Get a single character from stdin (like `getchar()`) without waiting
 * for [ENTER] key, by temporary setting the terminal to unbuffered mode.
 * @return The character.
 * @warning This function is not cross-platform because it uses termios.h, which
 * is only available on Unix systems.
 */
unsigned char getchar_no_enter(void);

/**
 * @brief Prints main menu.
 *
 * 3 options:
 * 1. New Game;
 * 2. Continue Game;
 * 3. Quit.
 *
 * @param is_running Pointer to the variable that controls the main loop
 */
void main_menu(unsigned char *is_running);

/**
 * @brief In battle, ask user to choose between:
 *     1. Attack;
 *     2. Drink potion;
 *     3. Flee.
 * @param player Pointer to the player
 * @param head Pointer to the head of the monsters list
 * @return Selected action, 0 on error
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
unsigned char attack_selection_menu(Character *player, Character *monster);

/**
 * @brief `printf()`, but in color
 * @param hexcolor Color to print in hexadecimal format
 * @param format Format string
 * @param ... Arguments to format string
 */
void color_printf(unsigned int hexcolor, const char *format, ...);

/**
 * @brief Print a bar with current and max values in percentage
 * @param label String to print before the bar
 * @param current Current value
 * @param max Max value
 * @param color Color of the bar in hexadecimal, or -1 for adaptive color
 * (red if < 10%, yellow if < 30%, green otherwise)
 */
void print_stat_bar
(
    const char *label,
    unsigned int current,
    unsigned int max,
    int color
);

/**
 * @brief Prints health & mana bars
 * @param character Pointer to the character
 */
void print_character_stats(Character *character);

/**
 * @brief Prints character's amount of gold.
 * @param character A pointer to the character.
 */
void print_character_gold(Character *character);

/**
 * @brief Prompts user to choose a spell among a player's spells list.
 * @param player A pointer to the player who has the spells list.
 * @param monster A pointer to the target monster.
 * @param spell_type The type of the spell to select (ATTACK_SPELL or HEAL_SPELL).
 * @return A pointer to the selected spell or NULL if player is NULL, or if
 * the player doesn't have any spell of the given type, or if a wrong item
 * type is given.
 */
Item *spell_selection_menu
(
    Character *player,
    Character *monster,
    ItemType spell_type
);

/**
 * @brief Gets a digit between min and max from user input without waiting for
 * [ENTER] key. It will keep reading input until a valid digit is entered.
 * @param min The minimum valid number (included).
 * @param max The maximum valid number (included).
 * @param is_cancelable 1 if the user can go back to previous menu by pressing
 * [B], 0 otherwise.
 * @return The number read from user input.
 * @warning This function is not cross-platform because it uses termios.h, which
 * is only available on Unix systems. It can only read one character, so only
 * positive digits between 0 and 9 included are valid.
 */
unsigned char get_valid_digit_no_enter
(
    unsigned char min,
    unsigned char max,
    unsigned char is_cancelable
);

/**
 * @brief Prompts user to write his user name.
 * @return A pointer to the user name.
 * @warning The user name must be freed with free() when it is no longer needed.
 */
char *get_user_name_menu(void);

/**
 * @brief Initialize a new game
 */
void new_game();

/**
 * @brief Display the attacker stats, battle monsters & attack result.
 * @param attacker A pointer to the attacker.
 * @param defender A pointer to the targeted monster.
 * @param monsters A pointer to the head of the monsters list.
 * @param damage_dealt The damage dealt to the defender.
 * @param damage_taken The damage taken by the attacker in return.
 * @param spell A pointer to the spell used to attack the monster, NULL if
 * weapon was used.
 */
void print_attack_result
(
    Character *attacker,
    Character *defender,
    Monsters *monsters,
    unsigned short damage_dealt,
    unsigned short damage_taken,
    Item *spell
);

#endif // MENUS_H
