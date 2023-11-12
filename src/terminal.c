#include "terminal.h"

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
