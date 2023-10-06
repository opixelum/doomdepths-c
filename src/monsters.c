#include "monsters.h"

Character *generate_random_monster()
{
    srand(time(NULL));

    char *name = calloc(7, sizeof *name);
    unsigned char level = 0;
    unsigned int xp = 0;
    unsigned int xp_to_next_level = 0;
    unsigned short health = 0;
    unsigned short max_health = 0;
    unsigned short mana = 0;
    unsigned short max_mana = 0;
    unsigned long gold = 0;

    // Assign monster stats based on monster type
    unsigned char random_monster_type = rand() % 100;

    level = rand() % 100 + 1;
    xp = rand() % 1000;
    xp_to_next_level = 0;
    gold = rand() % 1000;

    if (random_monster_type < 25) // 25% chance
    {
        strcpy(name, "Goblin");
        health = (unsigned short) (50.0 + 50.0 * (level / 100.0));
        max_health = health;
        mana = health;
        max_mana = health;
    }
    else if (random_monster_type < 45) // 20% chance
    {
        strcpy(name, "Orc");
        health = (unsigned short) (80.0 + 80.0 * (level / 100.0));
        max_health = health;
        mana = health;
        max_mana = health;
    }
    else if (random_monster_type < 65) // 20% chance
    {
        strcpy(name, "Troll");
        health = (unsigned short) (90.0 + 90.0 * (level / 100.0));
        max_health = health;
        mana = health;
        max_mana = health;
    }
    else if (random_monster_type < 80) // 15% chance
    {
        strcpy(name, "Ogre");
        health = (unsigned short) (120.0 + 120.0 * (level / 100.0));
        max_health = health;
        mana = health;
        max_mana = health;
    }
    else if (random_monster_type < 95) // 15% chance
    {
        strcpy(name, "Giant");
        health = (unsigned short) (130.0 + 130.0 * (level / 100.0));
        max_health = health;
        mana = health;
        max_mana = health;
    }
    else // %5 chance
    {
        strcpy(name, "Dragon");
        health = (unsigned short) (200.0 + 200.0 * (level / 100.0));
        max_health = health;
        mana = health;
        max_mana = health;
    }
    name = realloc(name, (strlen(name) + 1) * sizeof *name);

    return create_character
    (
        name,
        level,
        xp,
        xp_to_next_level,
        health,
        max_health,
        mana,
        max_mana,
        gold,
        NULL,
        NULL,
        // TODO: Give random items to monsters for player to loot
        NULL,
        NULL
    );
}