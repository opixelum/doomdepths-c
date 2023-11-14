#include "input.h"

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

unsigned char get_valid_digit_no_enter
(
    unsigned char min,
    unsigned char max,
    unsigned char is_cancelable
) {
    unsigned char input;
    do
    {
        input = getchar_no_enter();
        if (is_cancelable && (input == 'b' || input == 'B')) break;
        input -= '0'; // Convert ASCII character to digit
    }
    while (input < min || input > max);
    return input;
}

unsigned char yes_no_input(void)
{
    unsigned char input;
    do
    {
        input = getchar_no_enter();
        if (input >= 'A' && input <= 'Z') input += 32;
    }
    while (input != 'y' && input != 'n');

    return input == 'y' ? 1 : 0;
}
