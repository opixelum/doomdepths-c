#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdio.h>
#include <stdlib.h>
#include "input.h"

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
 * @brief Waits for a given number of milliseconds. Cross-platform.
 * @param milliseconds Number of milliseconds to wait.
 */
void wait_milliseconds(int milliseconds);

#endif // TERMINAL_H
