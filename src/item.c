#include "item.h"

Item *create_item
(
    ItemType type,
    char *name,
    char *description,
    unsigned short value,
    unsigned short price
) {
    Item *item = malloc(sizeof *item);
    if (!item)
    {
        fprintf
        (
            stderr,
            "ERROR: stuff.c: create_item(): item: malloc() failed\n"
        );
        exit(EXIT_FAILURE);
    }
    item->type = type;
    item->name = name;
    item->description = description;
    item->value = value;
    item->price = price;
    return item;
}

Inventory *add_item_to_inventory(Inventory *head, Item *item_to_add)
{
    if (number_of_items_by_type(head, ITEM) >= MAX_INVENTORY_SIZE)
    {
        printf("Your inventory is full.\n\n");
        press_any_key_to_continue();
        return head;
    }

    Inventory *new_item_entry = malloc(sizeof *new_item_entry);
    if (!new_item_entry)
    {
        fprintf
        (
            stderr,
            "ERROR: stuff.c: add_item_to_inventory(): new_item_entry: malloc() "
            "failed\n"
        );
        exit(EXIT_FAILURE);
    }

    new_item_entry->item = item_to_add;
    new_item_entry->next = NULL;

    // If the inventory is empty, the new item becomes the first item
    if (!head) return new_item_entry;

    // Otherwise, add the new item to the end of the inventory
    Inventory *current = head;
    while (current->next) current = current->next;
    current->next = new_item_entry;

    return head;
}

Inventory *remove_item_from_inventory(Inventory *head, Item *item_to_remove)
{
    Inventory *prev = NULL;
    Inventory *current = head;

    while (current)
    {
        if (current->item == item_to_remove)
        {
            // If the item to remove is at the head of the inventory.
            if (!prev)
            {
                Inventory *new_head = current->next;
                free(current);
                return new_head;
            }

            // Item found in the middle or end of the inventory.
            prev->next = current->next;
            free(current);
            return head;
        }

        prev = current;
        current = current->next;
    }

    return head; // Item not found in the inventory.
}

void free_inventory(Inventory *head)
{
    while (head)
    {
        Inventory *next = head->next;
        free(head);
        head = next;
    }
}

char *item_type_to_string(ItemType item_type, unsigned char capitalize)
{
    char *item_type_string;

    switch (item_type)
    {
        case ARMOR:
            item_type_string = capitalize ? "Armor" : "armor";
            break;

        case ATTACK_SPELL:
            item_type_string = capitalize ? "Attack spell" : "attack spell";
            break;

        case HEAL_SPELL:
            item_type_string = capitalize ? "Heal spell" : "heal spell";
            break;

        case HEALTH_POTION:
            item_type_string = capitalize ? "Health potion" : "health potion";
            break;

        case ITEM:
            item_type_string = capitalize ? "Item" : "item";
            break;

        case MANA_POTION:
            item_type_string = capitalize ? "Mana potion" : "mana potion";
            break;

        case POTION:
            item_type_string = capitalize ? "Potion" : "potion";
            break;

        case SPELL:
            item_type_string = capitalize ? "Spell" : "spell";
            break;

        case WEAPON:
            item_type_string = capitalize ? "Weapon" : "weapon";
            break;

        default:
            fprintf
            (
                stderr,
                "ERROR: stuff.c: item_type_to_string(): wrong item type\n"
            );
            exit(EXIT_FAILURE);
    }

    return item_type_string;
}

unsigned char number_of_items_by_type(Inventory *head, ItemType item_type)
{
    Inventory *current = head;
    unsigned char number_of_items = 0;
    while (current)
    {
        if
        (
            current->item->type == item_type
            || item_type == ITEM
            || item_type == SPELL && is_spell(item_type)
            || item_type == POTION && is_potion(item_type)
        )
            number_of_items++;

        current = current->next;
    }
    return number_of_items;
}

unsigned char number_of_items(Inventory *head, Item *item)
{
    if (!head)
    {
        fprintf
        (
            stderr,
            "ERROR: stuff.c: number_of_items(): head: NULL pointer\n"
        );
        exit(EXIT_FAILURE);
    }

    if (!item)
    {
        fprintf
        (
            stderr,
            "ERROR: stuff.c: number_of_items(): item: NULL pointer\n"
        );
        exit(EXIT_FAILURE);
    }

    Inventory *current = head;
    unsigned char number_of_items = 0;
    while (current)
    {
        if (current->item == item) number_of_items++;
        current = current->next;
    }

    return number_of_items;
}

unsigned char is_spell(ItemType type)
{
    return type == ATTACK_SPELL || type == HEAL_SPELL || type == SPELL;
}

unsigned char is_potion(ItemType type)
{
    return type == HEALTH_POTION || type == MANA_POTION || type == POTION;
}
