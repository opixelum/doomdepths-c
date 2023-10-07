#include "monsters.h"

Monsters *add_monster_to_inventory(Monsters *node, Character *monster_to_add)
{
    Monsters *new_monster_entry = malloc(sizeof *new_monster_entry);
    if (!new_monster_entry)
    {
        fprintf(stderr, "ERROR: stuff.c: add_monster_to_inventory: new_monster_entry: malloc failed\n");
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

Character *remove_monster_from_inventory(Monsters *head, Character *monster_to_remove)
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
        // TODO: Give random item to monsters for player to loot
        NULL,
        NULL
    );
}

Monsters *generate_random_monsters_list()
{
    srand(time(NULL));

    Monsters *head = NULL;

    for (unsigned char i = 0; i < 20; i++)
    {
        Character *monster = generate_random_monster(i);
        head = add_monster_to_inventory(head, monster);
    }

    return head;
}