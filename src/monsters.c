#include "monsters.h"

Monsters *add_monster_to_list(Monsters *node, Character *monster_to_add)
{
    Monsters *new_monster_entry = malloc(sizeof *new_monster_entry);
    if (!new_monster_entry)
    {
        fprintf(stderr, "ERROR: stuff.c: add_monster_to_list: new_monster_entry: malloc failed\n");
        exit(EXIT_FAILURE);
    }

    new_monster_entry->monster = monster_to_add;
    new_monster_entry->next = NULL;

    // If the inventory is empty, the new monster becomes the first monster
    if (!node) return new_monster_entry;

    // Otherwise, add the new monster to the end of the inventory
    Monsters *current = node;
    while (current->next) current = current->next;
    current->next = new_monster_entry;

    return node;
}

Character *remove_monster_from_list(Monsters *head, Character *monster_to_remove)
{
    Monsters *prev = NULL;
    Monsters *current = head;

    while (current)
    {
        if (current->monster == monster_to_remove)
        {
            // If the monster to remove is at the head of the inventory.
            if (!prev)
            {
                Character *removedCharacter = current->monster;
                free(current);
                return removedCharacter;
            }

            // Character found in the middle or end of the inventory.
            Character *removed_monster = current->monster;
            prev->next = current->next;
            free(current);
            return removed_monster;
        }

        prev = current;
        current = current->next;
    }

    return NULL; // Character not found in the inventory.
}

void free_monsters_list(Monsters *head)
{
    while (head)
    {
        Monsters *next = head->next;
        if (head->monster) free_character(head->monster);
        free(head);
        head = next;
    }
}

Character *generate_random_monster(unsigned int seed)
{
    srand(time(NULL) + seed);

    char *name;
    unsigned char level;
    unsigned int xp;
    unsigned int xp_to_next_level;
    unsigned short health;
    unsigned short max_health;
    unsigned short mana;
    unsigned short max_mana;
    unsigned long gold;

    // Assign monster stats based on monster type
    unsigned char random_monster_type = rand() % 100;

    level = rand() % 100 + 1;
    xp = rand() % 1000;
    xp_to_next_level = 0;
    gold = rand() % 1000;

    if (random_monster_type < 25) // 25% chance
    {
        name = "Goblin";
        health = (unsigned short) (50.0 + 50.0 * (level / 100.0));
        max_health = health;
        mana = health;
        max_mana = health;
    }
    else if (random_monster_type < 45) // 20% chance
    {
        name = "Orc";
        health = (unsigned short) (80.0 + 80.0 * (level / 100.0));
        max_health = health;
        mana = health;
        max_mana = health;
    }
    else if (random_monster_type < 65) // 20% chance
    {
        name = "Troll";
        health = (unsigned short) (90.0 + 90.0 * (level / 100.0));
        max_health = health;
        mana = health;
        max_mana = health;
    }
    else if (random_monster_type < 80) // 15% chance
    {
        name = "Ogre";
        health = (unsigned short) (120.0 + 120.0 * (level / 100.0));
        max_health = health;
        mana = health;
        max_mana = health;
    }
    else if (random_monster_type < 95) // 15% chance
    {
        name = "Giant";
        health = (unsigned short) (130.0 + 130.0 * (level / 100.0));
        max_health = health;
        mana = health;
        max_mana = health;
    }
    else // %5 chance
    {
        name = "Dragon";
        health = (unsigned short) (200.0 + 200.0 * (level / 100.0));
        max_health = health;
        mana = health;
        max_mana = health;
    }

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
        // TODO: Give random item to monsters for player to loot
        NULL,
        NULL
    );
}

Monsters *generate_random_monsters_list()
{
    srand(time(NULL));

    Monsters *head = NULL;
    int monster_count = rand() % 4 + 1;

    for (int i = 0; i < monster_count; i++)
    {
        Character *monster = generate_random_monster(i);
        head = add_monster_to_list(head, monster);
    }

    return head;
}