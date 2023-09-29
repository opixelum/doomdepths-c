#include "menus.h"

int main(int argc, char** argv)
{
    clear_screen();
    unsigned char is_running = 1;
    while (is_running) main_menu(&is_running);
    return EXIT_SUCCESS;
}
