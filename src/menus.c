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

    // First loop for repeating the menu if user enters an invalid choice
    do
    {
        char *input = NULL;
        clear_lines(12); // Main menu is at most 10 lines long
        printf
        (
            "DoomdepthsC - Battle\n\n"
            "    1. Attack\n"
            "    2. Drink potion\n"
            "    3. Flee\n\n"
        );

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

        // Convert input to integer for direct return
        long choice = strtol(input, NULL, 10);
        free(input);

        if (choice < 1 || choice > 3)
        {
            printf("\nInvalid choice!\n");
            wait_for_enter();
            continue;
        }

        return choice;
    } while (1);
}

Character *monster_selection_menu(Monsters *head)
{
    if (!head) return NULL;

    // First loop for repeating the menu if user enters an invalid choice
    do
    {
        char *input = NULL;
        clear_lines(12); // Main menu is at most 10 lines long
        printf("DoomdepthsC - Battle - Monsters\n\n");

        Character *monsters[5];
        unsigned char number_of_monsters = 0;
        while (head)
        {
            number_of_monsters++;
            printf("    %d. %s\n", number_of_monsters, head->monster->name);
            monsters[number_of_monsters - 1] = head->monster;
            head = head->next;
        }

        // Second loop for repeating the menu if user enters an empty string
        do
        {
            if (input) // Re-print the last line if user entered an empty string
            {
                clear_lines(2);
                free(input);
            }
            printf("\nEnter your choice (");
            for (unsigned char i = 1; i < number_of_monsters; ++i)
            {
                if (i < number_of_monsters - 1) printf("%d, ", i);
                else printf("%d or %d): ", i, i + 1);
            }

            input = get_user_input();
        }
        while (!strlen(input)); // While length of input is 0

        // Convert input to integer for direct return
        long choice = strtol(input, NULL, 10);
        free(input);

        if (choice < 1 || choice > number_of_monsters)
        {
            printf("\nInvalid choice!\n");
            wait_for_enter();
            continue;
        }

        return monsters[choice - 1];
    } while (1);
}

unsigned char attack_selection_menu()
{
    // First loop for repeating the menu if user enters an invalid choice
    do
    {
        char *input = NULL;
        clear_lines(10); // Main menu is at most 10 lines long
        printf
        (
            "DoomdepthsC - Select an attack\n\n"
            "    1. Weapon attack\n"
            "    2. Spell attack\n\n"
        );

        // Second loop for repeating the menu if user enters an empty string
        do
        {
            if (input) // Re-print the last line if user entered an empty string
            {
                clear_lines(1);
                free(input);
            }
            printf("Enter your choice (1 or 2): ");
            input = get_user_input();
        }
        while (!strlen(input)); // While length of input is 0

        // Convert input to integer for direct return
        long choice = strtol(input, NULL, 10);
        free(input);

        if (choice < 1 || choice > 3)
        {
            printf("\nInvalid choice!\n");
            wait_for_enter();
            continue;
        }

        return choice;
    } while (1);
}
