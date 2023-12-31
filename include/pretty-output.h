#ifndef PRETTY_OUTPUT_H
#define PRETTY_OUTPUT_H

#include <stdarg.h>
#include <string.h>
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

/**
 * @brief Centers a string in a given total_width.
 * @param string The string to center.
 * @param total_width The total_width to center the string in.
 * @return A pointer to the allocated centered string.
 * @warning The returned string must be freed with `free()`.
 */
char *center_string(const char *string, unsigned char total_width);

/**
 * @brief Prints a string containing the details of an item.
 * @param character A pointer to the character. Used to know if the item is
 * better than the equipped one.
 * @param item A pointer to the item.
 * @param show_type 1 to show the type of the item, 0 otherwise.
 * @param show_description 1 to show the description of the item, 0 otherwise.
 * @param show_value 1 to show the value of the item, 0 otherwise.
 * @param show_price 1 to show the price of the item, 0 otherwise.
 */
void *print_item_details
(
    Character *character,
    Item *item,
    unsigned char show_type,
    unsigned char show_description,
    unsigned char show_value,
    unsigned char show_price
);

#endif // PRETTY_OUTPUT_H
