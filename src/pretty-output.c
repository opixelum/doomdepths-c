#include "pretty-output.h"

void color_printf(unsigned int hexcolor, const char *format, ...)
{
    // Extract RGB components from the hexadecimal color
    unsigned int r = (hexcolor >> 16) & 0xFF;
    unsigned int g = (hexcolor >> 8) & 0xFF;
    unsigned int b = hexcolor & 0xFF;

    // Print the beginning of the 24-bit ANSI color escape sequence
    printf("\033[38;2;%d;%d;%dm", r, g, b);

    // Print the formatted string
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    // Reset the color
    printf("\033[0m");
}

void print_stat_bar
(
    const char *label,
    unsigned int current,
    unsigned int max,
    int color
) {
    printf("%s ", label);

    // Convert stat to a percentage
    unsigned char percentage = (unsigned char)
        ((float) current / (float) max * 92.0f);

    // Set color if adaptive
    if (color == -1)
    {
        if (percentage < 10) color = 0xFF0000; // Red if current < 10%
        else if (percentage < 30) color = 0xFFFF00; // Yellow if current < 30%
        else color = 0x00FF00; // Green otherwise
    }

    // Print stat percentage in color
    for (unsigned char i = 0; i < 92; ++i)
    {
        color_printf
        (
            color,
            "%s",
            i < percentage ? "█" : "-"
        );
    }

    // Print stat in number
    printf(" %d / %d\n", current, max);
}

void print_character_stats(Character *character)
{
    if (!character) return;

    // Print health bar
    print_stat_bar
    (
        "Health",
        character->health,
        character->max_health,
        -1
    );

    // Print mana bar
    print_stat_bar
    (
        "  Mana",
        character->mana,
        character->max_mana,
        0x2596be // Eastern Blue
    );
}

void print_character_gold(Character *character)
{
    printf("  Gold ");
    color_printf(0xFFD700, "%d\n", character->gold);
}

char *center_string(const char *string, unsigned char total_width)
{
    int input_length = strlen(string);

    // Calculate the padding on each side
    int padding_left = (total_width - input_length) / 2;
    int padding_right = total_width - input_length - padding_left;

    // Allocate memory for the centered string (+1 for the null terminator)
    char* centered = malloc(total_width + 1);
    if (centered == NULL) {
        perror("Unable to allocate memory for the centered string.");
        exit(EXIT_FAILURE);
    }

    // Set memory to spaces
    memset(centered, ' ', total_width);

    // Copy the input string into the centered string at the right offset
    memcpy(centered + padding_left, string, input_length);

    // Null-terminate the centered string
    centered[total_width] = '\0';

    return centered;
}
