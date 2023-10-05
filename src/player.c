#include "player.h"

Character *create_character(char *name)
{
    Character *player = malloc(sizeof *player);
    if (!player)
    {
        fprintf(stderr, "ERROR: player.c: create_character: player: malloc failed\n");
        exit(EXIT_FAILURE);
    }

    player->name = name;
    player->health = 100;
    player->max_health = 100;
    player->mana = 100;
    player->max_mana = 100;
    player->gold = 0;
    player->weapon = NULL;
    player->armor = NULL;
    player->spells = NULL;
    player->inventory = NULL;

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
