#ifndef MONSTERS_H
#define MONSTERS_H

#include <string.h>
#include <time.h>
#include "pretty-output.h"

static char *dragon[] =
{
    " <>=======()                           ",
    "(/\\___   /|\\\\          ()==========<>_ ",
    "      \\_/ | \\\\        //|\\   ______/ \\)",
    "        \\_|  \\\\      // | \\_/          ",
    "          \\|\\/|\\_   //  /\\/            ",
    "           (oo)\\ \\_//  /               ",
    "          //_/\\_\\/ /  |                ",
    "         @@/  |=\\  \\  |                ",
    "              \\_=\\_ \\ |                ",
    "                \\==\\ \\|\\_              ",
    "             __(\\===\\(  )\\             ",
    "            (((~) __(_/   |            ",
    "                 (((~) \\  /            ",
    "                 ______/ /             ",
    "                 '------'              ",
    "                                       ",
    "                                       ",
    "                                       ",
    NULL
};

static char *grim_reaper[] =
{
    "                       ",
    "                       ",
    "                       ",
    "                       ",
    "                       ",
    "                       ",
    "                 /\\    ",
    "   ____ (((+))) _||_   ",
    "  /.--.\\  .-.  /.||.\\  ",
    " /.,   \\\\(0.0)// || \\\\ ",
    "/;`\";/\\ \\\\|m|//  ||  ;\\",
    "|:   \\ \\__`:`____||__:|",
    "|:    \\__ \\T/ (@~)(~@)|",
    "|:    _/|     |\\_\\/  :|",
    "|'  /   |     |   \\  '|",
    " \\_/    |     |    \\_/ ",
    "        |_____|        ",
    "        |_____|        ",
    NULL
};

static char *centaur[] =
{
    "                  ",
    "                  ",
    "                  ",
    "                  ",
    "                  ",
    "                  ",
    "                  ",
    "                  ",
    "  <=======]}======",
    "    --.   /|      ",
    "   _\\\"/_.'/       ",
    " .'._._,.'        ",
    " :/ \\{}/          ",
    "(L  /--',----._   ",
    "    |          \\\\ ",
    "   : /-\\ .'-'\\ / |",
    "    \\\\, ||    \\|  ",
    "     \\/ ||    ||  ",
    NULL
};

static char *skeleton[] =
{
    "         ",
    "         ",
    "         ",
    "         ",
    "         ",
    "         ",
    "         ",
    "         ",
    "         ",
    "         ",
    "   .-.   ",
    "  (o.o)  ",
    " __|=|__ ",
    "//.=|=.\\\\",
    "\\\\.=|=.//",
    "(:(_=_):)",
    "  || ||  ",
    "  || ||  ",
    NULL
};

static char *ghost[] =
{
    "       ",
    "       ",
    "       ",
    "       ",
    "       ",
    "       ",
    "       ",
    "       ",
    "       ",
    "       ",
    " .-.   ",
    "(o o)  ",
    "| O \\  ",
    " \\   \\ ",
    "  `~~~'",
    "       ",
    "       ",
    "       ",
    NULL
};

/**
 * @brief Linked list of all alive monsters of a battle.
 */
typedef struct Monsters
{
    Character *monster;
    struct Monsters *next;
}
Monsters;

/**
 * @brief Adds an monster to the end of an monsters list.
 * @param head A pointer to a node of the monsters list.
 * @param monster A Pointer to the monster to be added.
 * @return A pointer to the given monsters list node or the new monsters list
 * node if the given monsters list node was NULL.
 */
Monsters *add_monster_to_list(Monsters *node, Character *monster);

/**
 * @brief Removes an monster from an monsters list.
 * @param head A pointer to the head of the monsters list.
 * @param monster A pointer to the monster to be removed.
 * @return A pointer to the removed monster, or NULL if the monster was not
 * found.
 * @warning This function does not free the monster from memory, it only removes
 * it from the monsters list.
 * @warning In case you're not passing the head of the monsters list, the
 * monster to remove can be skipped if it's before the given monsters list node.
 */
Character *remove_monster_from_list(Monsters *head, Character *monster);

/**
 * @brief Frees a linked list of monsters from memory.
 * @param head A pointer to the head of the linked list of monsters to be freed.
 * @warning Every monster in the linked list will be freed from memory.
 * @warning In case you're not passing the head of the monsters list, only the
 * following nodes will be freed.
 */
void free_monsters_list(Monsters *head);

/**
 * @brief Generate a monster with random stats.
 *
 * - 25% chance to be a Goblin;
 * - 20% chance to be an Orc;
 * - 20% chance to be a Troll;
 * - 15% chance to be an Ogre;
 * - 15% chance to be a Giant;
 * - 5% chance to be a Dragon.
 *
 * Level is a random number between 1 and 100.
 * XP is a random number between 0 and 1000.
 * Gold is a random number between 0 and 1000.
 * Health is calculated with the following formula:
 * health = base_health + base_health * (level / 100)
 * Mana is equal to health.
 *
 * @param seed Seed for the random number generator.
 * @return A pointer to the new monster.
 * @warning The monster must be freed with free_character() when it's no longer
 * needed.
 */
Character *generate_random_monster(unsigned int seed);
// TODO: Improve randomness for omitting seed parameter

/**
 * @brief Generates a linked list of a random number of random monsters, with a
 * maximum of 3 monsters.
 * @return A pointer to the head of the linked list of monsters.
 * @warning The monsters list must be freed with free_monsters_list() when it's
 * no longer needed.
 */
Monsters *generate_random_monsters_list(void);

/**
 * @brief Get a line of the art of a monster.
 * @param monster_name A string containing the name of the monster.
 * @param line_number The number of the line to get.
 * @return A string containing the line of the art of the monster.
 */
const char *get_monster_art_line
(
    const char *monster_name,
    unsigned char line_number
);

/**
 * @brief Prints monsters of a battle on the screen.
 * @param head A pointer to the head of the monsters list.
 * @param targeted_monster A pointer to the targeted monster or NULL if no
 * monster is targeted.
 * @warning It's important to pass the head of the monsters list, otherwise
 * monsters can be skipped.
 */
void print_monsters(Monsters *head, Character *targeted_monster);

/**
 * @brief Get the number of monsters in a monsters list.
 * @param head A pointer to the head of the monsters list.
 * @return The number of monsters in the monsters list.
 * @warning It's important to pass the head of the monsters list, otherwise
 * monsters can be skipped.
 */
unsigned char get_number_of_monsters(Monsters *head);

/**
 * @brief Updates monsters list if a monster is dead.
 * @param head A pointer to the head of the monsters list.
 * @return A pointer to the head of the monsters list, NULL if all monsters
 * died.
 * @warning It's important to pass the head of the monsters list, otherwise
 * monsters can be skipped.
 */
Monsters *update_monsters_list(Monsters *head);

#endif // MONSTERS_H
