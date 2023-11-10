#ifndef PRETTY_OUTPUT_H
#define PRETTY_OUTPUT_H

#include <stdarg.h>
#include "character.h"

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

#endif // PRETTY_OUTPUT_H
