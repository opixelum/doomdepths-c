#include "menus.h"

void clear_screen()
{
    printf("\e[1;1H\e[2J");
}

void clear_lines(unsigned int number_of_lines)
{
    for (int i = 0; i < number_of_lines; ++i) printf("\x1b[1F\x1b[2K");
}

void clear_stdin()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void wait_for_enter()
{
    printf("\nPress [ENTER] to continue...");
    clear_stdin();
}

char *get_user_input()
{
    // Allocate memory for the string with the maximum size for 255 characters
    char *input = malloc(256 * sizeof input);
    if (!input)
    {
        fprintf(stderr, "ERROR: menus.c: get_user_input: `input` malloc failed\n");
        exit(EXIT_FAILURE);
    }

    // Get user input from stdin buffer
    fgets(input, 256, stdin);

    // Remove trailing newline & retrun carriage characters
    input[strcspn(input, "\n\r")] = '\0';

    // Check if input is a string or a character
    unsigned char input_length = strlen(input);

    // If it's a character, remove the null terminator by reallocating memory
    if (input_length == 1) input = realloc(input, sizeof input);

    // If it's a string, reallocate memory to the actual size of the string
    // & add 1 for the null terminator
    else input = realloc(input, input_length + 1 * sizeof input);

    // Check if memory reallocation failed
    if (!input)
    {
        fprintf(stderr, "ERROR: menus.c: get_user_input: `input` realloc failed\n");
        exit(EXIT_FAILURE);
    }

    return input;
}

unsigned char no_enter_getchar(void)
{
    struct termios old_terminal_settings, new_terminal_settings;
    unsigned char character;

    // Get the current terminal settings
    tcgetattr(STDIN_FILENO, &old_terminal_settings);
    new_terminal_settings = old_terminal_settings;

    // Set the terminal to unbuffered mode, so that characters are available
    // to be read immediately instead of waiting for the user to press [ENTER]
    new_terminal_settings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal_settings);

    // Read a character
    character = getchar();

    // Restore the original terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal_settings);

    return character;
}

void main_menu(unsigned char *is_running)
{
    // First loop for repeating the menu if user enters an invalid choice
    do
    {
        char *input = NULL;
        clear_lines(10); // Main menu is at most 10 lines long
        printf("DoomdepthsC\n\n    1. New Game\n    2. Continue Game\n    3. Quit\n\n");

        // Second loop for repeating the menu if user enters an empty string
        do
        {
            if (input) // Re-print the last line if user entered an empty string
            {
                clear_lines(1);
                free(input);
            }
            printf("Enter your choice (1, 2 or 3): ");
            input = get_user_input();
        }
        while (!strlen(input)); // While length of input is 0

        switch (input[0])
        {
            case '1':
                // TODO: Implement New Game
                printf("New Game\n");
                break;

            case '2':
                // TODO: Implement Continue Game
                printf("Continue Game\n");
                break;

            case '3':
                clear_screen();
                *is_running = 0;
                free(input);
                return;

            default:
                printf("Invalid choice!\n");
                break;
        }

        free(input);
        wait_for_enter();
    } while (1);
}

unsigned char battle_actions_menu(Character *player, Monsters *head)
{
    if (!player || !head) return 0;

    clear_screen();
    print_character_stats(player);
    printf
    (
        "\nWhat do you want to do now?\n\n"
        "    1. Attack\n"
        "    2. Drink potion\n"
        "    3. Flee\n"
        "\nPress the number of your choice on your keyboard."
    );

    return no_enter_get_valid_digit(1, 3);
}

Character *monster_selection_menu(Character *character, Monsters *head)
{
    if (!head) return NULL;

    clear_screen();
    print_character_stats(character);
    printf("\nWhich monster do you want to attack now?\n\n");

    Character *monsters[5];
    unsigned char number_of_monsters = 0;
    while (head)
    {
        printf("    %d. %s\n", ++number_of_monsters, head->monster->name);
        monsters[number_of_monsters - 1] = head->monster;
        head = head->next;
    }

    printf("\nPress the number of your choice on your keyboard.");

    return monsters[no_enter_get_valid_digit(1, number_of_monsters) - 1];
}

unsigned char attack_selection_menu(Character *character)
{
    if (!character) return 0;

    clear_screen();
    print_character_stats(character);
    printf
    (
        "\nWould you rather use your weapon or cast a spell?\n\n"
        "    1. Weapon attack\n"
        "    2. Spell attack\n"
        "\nPress the number of your choice on your keyboard."
    );

    return no_enter_get_valid_digit(1, 2);
}

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
    (
        (float) current / (float) max * 100.0f
    );

    // Set color if adaptive
    if (color == -1)
    {
        if (percentage < 10) color = 0xFF0000; // Red if current < 10%
        else if (percentage < 30) color = 0xFFFF00; // Yellow if current < 30%
        else color = 0x00FF00; // Green otherwise
    }

    // Print stat percentage in color
    for (unsigned char i = 0; i < 100; ++i)
    {
        color_printf
        (
            color,
            "%s",
            i <= percentage ? "█" : "-"
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

    // Print gold
    printf("  Gold ");
    color_printf(0xFFD700, "%d\n", character->gold);
}

Item *spell_selection_menu(Character *character)
{
    if (!character) return NULL;

    clear_screen();
    print_character_stats(character);
    printf("\nWhich attack spell do you want to cast?\n\n");

    Inventory *spells_list = character->spells;

    Item *spells[9]; // For simplicity when getting user input, 9 spells max
    unsigned char number_of_spells = 0;
    while (spells_list)
    {
        printf("    %d. %s\n", ++number_of_spells, spells_list->item->name);
        spells[number_of_spells - 1] = spells_list->item;
        spells_list = spells_list->next;
    }

    printf("\nPress the number of your choice on your keyboard.");

    // -1 because array starts at 0
    return spells[no_enter_get_valid_digit(1, number_of_spells) - 1];
}

unsigned char no_enter_get_valid_digit(unsigned char min, unsigned char max)
{
    unsigned char input;
    do input = no_enter_getchar() - '0'; // Convert ASCII to integer
    while (input < min || input > max);
    return input;
}
