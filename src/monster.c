#include "monster.h"

Monsters *add_monster_to_list(Monsters *node, Character *monster_to_add)
{
    Monsters *new_monster_entry = malloc(sizeof *new_monster_entry);
    if (!new_monster_entry)
    {
        fprintf
        (
            stderr,
            "ERROR: stuff.c: add_monster_to_list(): new_monster_entry: malloc()"
            " failed\n"
        );
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

    if (random_monster_type < 30) // 30% chance
    {
        name = "Ghost";
        health = (unsigned short) (50.0 + 50.0 * (level / 100.0));
        max_health = health;
        mana = health;
        max_mana = health;
    }
    else if (random_monster_type < 55) // 25% chance
    {
        name = "Skeleton";
        health = (unsigned short) (80.0 + 80.0 * (level / 100.0));
        max_health = health;
        mana = health;
        max_mana = health;
    }
    else if (random_monster_type < 80) // 25% chance
    {
        name = "Centaur";
        health = (unsigned short) (90.0 + 90.0 * (level / 100.0));
        max_health = health;
        mana = health;
        max_mana = health;
    }
    else if (random_monster_type < 95) // 15% chance
    {
        name = "Grim Reaper";
        health = (unsigned short) (120.0 + 120.0 * (level / 100.0));
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
    int monster_count = rand() % 3 + 1;

    for (int i = 0; i < monster_count; i++)
    {
        Character *monster = generate_random_monster(i);
        head = add_monster_to_list(head, monster);
    }

    return head;
}

const char *get_monster_art_line
(
    const char *monster_name,
    unsigned char line_number
) {
    if (strcmp(monster_name, "Dragon") == 0) return dragon[line_number];
    if (strcmp(monster_name, "Grim Reaper") == 0) return grim_reaper[line_number];
    if (strcmp(monster_name, "Centaur") == 0) return centaur[line_number];
    if (strcmp(monster_name, "Skeleton") == 0) return skeleton[line_number];
    if (strcmp(monster_name, "Ghost") == 0) return ghost[line_number];

    return NULL;
}

void print_monsters(Monsters *head, Character *targeted_monster)
{
    if (!head) return;
    Monsters *current;

    for (int line_number = 0; dragon[line_number] != NULL; line_number++)
    {
        current = head;
        while (current)
        {
            unsigned int hex_color = current->monster == targeted_monster ?
                0xff0000 : 0xffffff;

            color_printf
            (
                hex_color,
                "%s  ",
                get_monster_art_line(current->monster->name, line_number)
            );
            current = current->next;
        }
        printf("\n");
    }

    printf("\n");

    current = head;
    while (current)
    {
        size_t line_length = strlen(get_monster_art_line(current->monster->name, 0));
        size_t padding = line_length / 2 - strlen(current->monster->name) / 2;
        
        for (int space = 0; space < padding; space++) printf(" ");
        printf("%s", current->monster->name);
        for (int space = 0; space < padding; space++) printf(" ");

        printf("  ");
        
        current = current->next;
    }
    printf("\n");
}

unsigned char get_number_of_monsters(Monsters *head)
{
    if (!head) return 0;

    unsigned char number_of_monsters = 0;
    Monsters *current = head;

    while (current)
    {
        number_of_monsters++;
        current = current->next;
    }

    return number_of_monsters;
}

Monsters *update_monsters_list(Monsters *head)
{
    if (!head) return NULL;

    Monsters *node = head;
    Monsters *prev = NULL;

    while (node)
    {
        if (node->monster->health == 0)
        {
            if (prev) prev->next = node->next;
            else head = node->next;

            Monsters *tmp = node;
            node = node->next;
            free_character(tmp->monster);
            free(tmp);
        }
        else
        {
            prev = node;
            node = node->next;
        }
    }

    return head;
}
