#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

/**
 * @brief Gets the user input from stdin.
 * @return A pointer to the string or character containing user input.
 */
char *get_string(void);

/**
 * @brief Gets a single character from stdin (like `getchar()`) without waiting
 * for [ENTER] key, by temporary setting the terminal to unbuffered mode.
 * @return The entered character.
 * @warning This function is not cross-platform because it uses termios.h, which
 * is only available on Unix systems.
 */
unsigned char getchar_no_enter(void);

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

#endif // INPUT_H
