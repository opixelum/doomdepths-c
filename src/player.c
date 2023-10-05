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
    Character *player = malloc(sizeof *player);
    if (!player)
    {
        fprintf(stderr, "ERROR: player.c: create_character: player: malloc failed\n");
        exit(EXIT_FAILURE);
    }

    player->name = name;
    player->health = health;
    player->max_health = max_health;
    player->mana = mana;
    player->max_mana = max_mana;
    player->gold = gold;
    player->weapon = weapon;
    player->armor = armor;
    player->spells = spells;
    player->inventory = inventory;

    return player;
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
