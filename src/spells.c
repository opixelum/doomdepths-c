#include "spells.h"

unsigned char number_of_attack_spells(Character *character)
{
    if (!character) return 0;
    if (!character->spells) return 0;

    Inventory *node = character->spells;
    unsigned char number_of_attack_spell = 0;

    while (node)
    {
        if (node->item->type == ATTACK_SPELL) number_of_attack_spell++;
        node = node->next;
    }

    return number_of_attack_spell;
}

Item *get_attack_spell(Character *character)
{
    if (!character) return NULL;
    if (!character->spells) return NULL;

    Inventory *node = character->spells;

    while (node)
    {
        if (node->item->type == ATTACK_SPELL) return node->item;
        node = node->next;
    }

    return NULL;
}
