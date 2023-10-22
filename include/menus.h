#ifndef MENUS_H
#define MENUS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * @brief Prompts user to choose between an weapon or a spell
 * @return 1 if weapon, 2 if spell, 0 on error
 */
unsigned char attack_selection_menu();

#endif // MENUS_H
