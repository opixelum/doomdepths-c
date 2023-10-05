#include "player.h"

Character *create_character
(
    char *name,
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

void free_character(Character *player)
{
    if (!player) return;
    free(player->name);
    free_item(player->weapon);
    free_item(player->armor);
    free_inventory(player->spells);
    free_inventory(player->inventory);
    free(player);
}
