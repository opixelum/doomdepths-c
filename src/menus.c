#include "menus.h"

void clear_screen(void)
{
    // printf("\e[1;1H\e[3J\e[2J");

    // Not cross-platform, but faster
    system("clear");
}

void clear_lines(unsigned int number_of_lines)
{
    printf("\x1b[2K");
    for (int i = 0; i < number_of_lines; ++i) printf("\x1b[1F\x1b[2K");
}

void clear_stdin(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void press_any_key_to_continue(void)
{
    printf("Press any key to continue...");
    getchar_no_enter();
}

void press_enter_to_continue(void)
{
    printf("Press [ENTER] to continue...");
    while (getchar_no_enter() != '\n') {}
}

char *get_string(void)
{
    // Allocate memory for the string with the maximum size for 255 characters
    char *input = malloc(256 * sizeof input);
    if (!input)
    {
        fprintf(stderr, "ERROR: menus.c: get_string: `input` malloc failed\n");
        exit(EXIT_FAILURE);
    }

    // Get user input from stdin buffer
    fgets(input, 256, stdin);

    // Remove trailing newline & return carriage characters
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
        fprintf(stderr, "ERROR: menus.c: get_string: `input` realloc failed\n");
        exit(EXIT_FAILURE);
    }

    return input;
}

unsigned char getchar_no_enter(void)
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
    clear_screen();
    printf
    (
        "Welcome to Doomdepths C! What do you want to do?\n\n"
        "    1. Start a new game\n"
        "    2. Continue my last game\n"
        "    3. Quit\n"
        "\nPress the number of your choice on your keyboard."
    );

    Character *player;

    switch (get_valid_digit_no_enter(1, 3))
    {
    case 1:
        new_game();
        break;

    case 2:
        player = load_game();
        MapContext *map_context = malloc(sizeof *map_context);
        map_context->player = player;

        get_map(map_context);
        explore_map(map_context);

        free_character(player);
        free(map_context);

        break;

    case 3:
        clear_screen();
        *is_running = 0;
        break;
    }
}

unsigned char battle_actions_menu(Character *player, Monsters *head)
{
    if (!player || !head) return 0;

    printf
    (
        "\nWhat do you want to do now?\n\n"
        "    1. Attack\n"
        "    2. Drink potion\n"
        "    3. Flee\n"
        "\nPress the number of your choice on your keyboard."
    );

    unsigned char choice = get_valid_digit_no_enter(1, 3);
    clear_lines(7);

    return choice;
}

Character *monster_selection_menu(Character *character, Monsters *head)
{
    if (!character || !head) return NULL;

    printf("\nWhich monster do you want to attack now?\n\n");

    Character *monsters[3];
    unsigned char number_of_monsters = 0;
    while (head)
    {
        printf("    %d. %s\n", ++number_of_monsters, head->monster->name);
        monsters[number_of_monsters - 1] = head->monster;
        head = head->next;
    }

    printf("\nPress the number of your choice on your keyboard.");

    unsigned char choice = get_valid_digit_no_enter(1, number_of_monsters);
    clear_lines(number_of_monsters + 4); // +4 for other menu lines

    // -1 because array starts at 0
    return monsters[choice - 1];
}

unsigned char attack_selection_menu(Character *player, Character *monster)
{
    if (!player) return 0;

    printf
    (
        "\nYou are attacking %s.\n"
        "\nWould you rather use your weapon or cast a spell?\n\n"
        "    1. Weapon attack\n"
        "    2. Spell attack\n"
        "\nPress the number of your choice on your keyboard.",
        monster->name
    );

    unsigned char choice = get_valid_digit_no_enter(1, 2);
    clear_lines(8);

    return choice;
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
    for (unsigned char i = 0; i < 92; ++i)
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
}

void print_character_gold(Character *character)
{
    // Print gold
    printf("  Gold ");
    color_printf(0xFFD700, "%d\n", character->gold);
}

Item *type_spell_selection_menu
(
    Character *player,
    Character *monster,
    ItemType spell_type
) {
    if (!player) return NULL;

    printf("\nYou are attacking %s.\n", monster->name);

    switch (spell_type)
    {
    case ATTACK_SPELL:
        printf("\nWhich attack spell do you want to cast?\n\n");
        break;

    case HEAL_SPELL:
        printf("\nWhich heal spell do you want to cast?\n\n");
        break;

    default:
        return NULL;
    }

    Inventory *spells_list = player->spells;

    Item *spells[9]; // For simplicity when getting user input, 9 spells max
    unsigned char number_of_spells = 0;
    while (spells_list)
    {
        if (spells_list->item->type == spell_type)
        {
            printf
            (
                "    %d. %s (%d mana)\n",
                ++number_of_spells,
                spells_list->item->name,
                spells_list->item->price
            );
            spells[number_of_spells - 1] = spells_list->item;
        }
        spells_list = spells_list->next;
    }

    printf("\nPress the number of your choice on your keyboard.");

    unsigned char choice = get_valid_digit_no_enter(1, number_of_spells);
    clear_lines(number_of_spells + 6);

    // -1 because array starts at 0
    return spells[choice - 1];
}

unsigned char get_valid_digit_no_enter(unsigned char min, unsigned char max)
{
    unsigned char input;
    do input = getchar_no_enter() - '0'; // Convert ASCII to integer
    while (input < min || input > max);
    return input;
}

char *get_user_name_menu(void)
{
    clear_screen();
    printf("Before we start, enter your name: ");

    char *user_name = NULL;

    do user_name = get_string();
    while (!strlen(user_name));

    return user_name;
}

void new_game(void)
{
    // Ask user for his name
    char *user_name = get_user_name_menu();

    Item *fireball = create_item(ATTACK_SPELL, "Fireball", "Man's not hot", 34, 41);
    Item *freeze = create_item(ATTACK_SPELL, "Freeze", "Ice ice baby", 34, 38);
    Inventory *spells = NULL;
    spells = add_item_to_inventory(spells, fireball);
    spells = add_item_to_inventory(spells, freeze);

    // Create a new character
    Character *player = create_character
    (
        user_name,
        1,
        0,
        1000,
        100,
        100,
        100,
        100,
        0,
        NULL,
        NULL,
        spells,
        NULL
    );

    create_tables("doomdepths.db");
    save_game(player);

    initialize_map();
    MapContext *map_context = malloc(sizeof *map_context);
    map_context->player = player;
    get_map(map_context);

    printf("\nWelcome %s!\n\n", player->name);
    press_any_key_to_continue();

    explore_map(map_context);
}

void print_attack_result
(
    Character *player,
    Character *defender,
    Monsters *monsters,
    unsigned short damage_dealt,
    unsigned short damage_taken,
    Item *spell
) {
    clear_screen();
    print_character_stats(player);
    print_monsters(monsters);

    if (spell) printf
    (
        "\nYou dealt %d damage to %s by casting the %s spell.\n",
        damage_dealt,
        defender->name,
        spell->name
    );
    else printf
    (
        "\nYou dealt %d damage to %s using your %s.\n",
        damage_dealt,
        defender->name,
        player->weapon ? player->weapon->name : "fists"
    );

    if (defender->health <= 0) printf("You killed %s!\n\n", defender->name);
    else printf
    (
        "%s dealt %d damage to you.\n\n",
        defender->name,
        damage_taken
    );

    if (player->health <= 0) printf("You died!\n\n");
}
