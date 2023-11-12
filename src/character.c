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

void drink_potion(Character *character, Item *potion)
{
    if (!character)
    {
        fprintf
        (
            stderr,
            "ERROR: character.c: drink_potion(): character: NULL pointer\n"
        );
        exit(EXIT_FAILURE);
    }

    if (!potion)
    {
        fprintf
        (
            stderr,
            "ERROR: character.c: drink_potion(): potion: NULL pointer\n"
        );
        exit(EXIT_FAILURE);
    }

    switch (potion->type)
    {
    case HEALTH_POTION:
        if (character->health + potion->value > character->max_health)
            character->health = character->max_health;
        else character->health += potion->value;
        break;

    case MANA_POTION:
        if (character->mana + potion->value > character->max_mana)
            character->mana = character->max_mana;
        else character->mana += potion->value;
        break;

    default:
        fprintf
        (
            stderr,
            "ERROR: character.c: drink_potion(): potion: invalid item type\n"
        );
        exit(EXIT_FAILURE);
    }

    character->inventory = remove_item_from_inventory
    (
        character->inventory,
        potion
    );
    free(potion);
}

void equip_item(Character *character, Item *item)
{
    if (!character)
    {
        fprintf
        (
            stderr,
            "ERROR: character.c: equip_item(): character: NULL pointer\n"
        );
        exit(EXIT_FAILURE);
    }

    if (!item)
    {
        fprintf
        (
            stderr,
            "ERROR: character.c: equip_item(): item: NULL pointer\n"
        );
        exit(EXIT_FAILURE);
    }

    if (item->type != WEAPON && item->type != ARMOR)
    {
        fprintf
        (
            stderr,
            "ERROR: character.c: equip_item(): item: invalid item type\n"
        );
        exit(EXIT_FAILURE);
    }

    character->inventory = remove_item_from_inventory(character->inventory, item);

    if (item->type == WEAPON)
    {
        if (character->weapon)
            character->inventory = add_item_to_inventory
            (
                character->inventory,
                character->weapon
            );
        character->weapon = item;
    }
    else
    {
        if (character->armor)
            character->inventory = add_item_to_inventory
            (
                character->inventory,
                character->armor
            );
        character->armor = item;
    }
}

void cast_spell(Character *caster, Character *target, Item *spell)
{
    if (!caster)
    {
        fprintf
        (
            stderr,
            "ERROR: character.c: cast_spell(): caster: NULL pointer\n"
        );
        exit(EXIT_FAILURE);
    }

    if (!target)
    {
        fprintf
        (
            stderr,
            "ERROR: character.c: cast_spell(): target: NULL pointer\n"
        );
        exit(EXIT_FAILURE);
    }

    if (!spell)
    {
        fprintf
        (
            stderr,
            "ERROR: character.c: cast_spell(): spell: NULL pointer\n"
        );
        exit(EXIT_FAILURE);
    }

    if (spell->type != ATTACK_SPELL && spell->type != HEAL_SPELL)
    {
        fprintf
        (
            stderr,
            "ERROR: character.c: cast_spell(): spell: invalid item type\n"
        );
        exit(EXIT_FAILURE);
    }

    caster->mana -= spell->price;
    if (caster->mana < 0) caster->mana = 0;

    if (spell->type == ATTACK_SPELL)
    {
        if (target->health < spell->value) target->health = 0;
        else target->health -= spell->value;
    }
    else
    {
        if (target->health + spell->value > target->max_health)
            target->health = target->max_health;
        else target->health += spell->value;
    }
}
