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

unsigned char number_of_type_spells(Character *character, ItemType type)
{
    if (!character) return 0;
    if (!character->spells) return 0;

    Inventory *node = character->spells;
    unsigned char number_of_type_spell = 0;

    while (node)
    {
        if (node->item->type == type) number_of_type_spell++;
        node = node->next;
    }

    return number_of_type_spell;
}

Inventory *get_type_spells(Character *character, ItemType type)
{
    if (!character) return NULL;
    if (!character->spells) return NULL;

    Inventory *type_spells = NULL;
    Inventory *node = character->spells;

    while (node)
    {
        if (node->item->type == type)
            type_spells = add_item_to_inventory(type_spells, node->item);
        node = node->next;
    }

    return type_spells;
}

unsigned char has_enough_mana(Character *character, ItemType spell_type)
{
    if (!character) return 0;

    Inventory *spells = character->spells;

    while (spells)
    {
        if (spells->item->type == spell_type)
            if (character->mana >= spells->item->price) return 1;

        spells = spells->next;
    }

    return 0;
}
