#include "character.h"

Character *create_character
(
    const char *name,
    unsigned char level,
    unsigned int xp,
    unsigned int xp_to_next_level,
    unsigned short health,
    unsigned short max_health,
    unsigned short mana,
    unsigned short max_mana,
    unsigned long gold,
    Item *weapon,
    Item *armor,
    Inventory *spells,
    Inventory *inventory
) {
    Character *character = malloc(sizeof *character);
    if (!character)
    {
        fprintf(stderr, "ERROR: character.c: create_character: character: malloc failed\n");
        exit(EXIT_FAILURE);
    }

    character->name = name;
    character->level = level;
    character->xp = xp;
    character->xp_to_next_level = xp_to_next_level;
    character->health = health;
    character->max_health = max_health;
    character->mana = mana;
    character->max_mana = max_mana;
    character->gold = gold;
    character->weapon = weapon;
    character->armor = armor;
    character->spells = spells;
    character->inventory = inventory;

    return character;
}

void free_character(Character *character)
{
    if (!character) return;
    free(character->weapon);
    free(character->armor);
    free_inventory(character->spells);
    free_inventory(character->inventory);
    free(character);
}
