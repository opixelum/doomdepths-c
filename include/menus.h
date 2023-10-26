#ifndef MENUS_H
#define MENUS_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "monsters.h"

/**
 * @brief Cross-platform function to clear console screen.
 *
 * Prints a regex where:
 * - "\e[1;1H" moves cursor to top left corner of terminal screen;
 * - "\e[2J" replaces all characters of the terminal screen with spaces.
 */
void clear_screen();

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
void clear_stdin();

/**
 * @brief Waits for user to press [ENTER] key.
 *
 * Uses getchar() in clearStdin() to wait for [ENTER] key.
 * Then, it clears the stdin buffer (in case user pressed more than one key).
 */
void wait_for_enter();

/**
 * @brief Get the user input from stdin
 *
 * @return Pointer to the string or character containing user input
 */
char *get_user_input();

/**
 * @brief Get a single character from stdin (like `getchar()`) without waiting
 * for [ENTER] key, by temporary setting the terminal to unbuffered mode.
 * @return The character.
 * @warning This function is not cross-platform because it uses termios.h, which
 * is only available on Unix systems.
 */
unsigned char direct_getchar(void);

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
 * @param head Pointer to the head of the monsters list
 * @return Pointer to the selected monster, NULL on error
 */
Character *monster_selection_menu(Monsters *head);

/**
 * @brief Prompts user to choose between an weapon or a spell.
 * @param character A pointer to the character for printing his stats.
 * @return 1 if weapon, 2 if spell, 0 on error.
 */
unsigned char attack_selection_menu(Character *character);

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
 * @brief Prints health & mana bars & their gold
 * @param character Pointer to the character
 */
void print_character_stats(Character *character);

/**
 * @brief Prompts user to choose a spell among a character's spells list.
 * @param character A pointer to the character who has the spells list
 * @return A pointer to the selected spell, NULL on error.
 */
Item *spell_selection_menu(Character *character);

#endif // MENUS_H
