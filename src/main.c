#include "menu.h"

int main(int argc, char** argv)
{
    unsigned char is_running = 1;
    while (is_running) main_menu(&is_running);
    return EXIT_SUCCESS;
}
