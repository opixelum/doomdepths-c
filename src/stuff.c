#include "stuff.h"

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
        fprintf(stderr, "ERROR: stuff.c: create_item: item: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    item->type = type;
    item->name = name;
    item->description = description;
    item->value = value;
    item->price = price;
    return item;
}

unsigned char inventory_size(Inventory *head)
{
    unsigned char size = 0;
    Inventory *current = head;
    while (current)
    {
        size++;
        current = current->next;
    }
    return size;
}

Inventory *add_item_to_inventory(Inventory *head, Item *item_to_add)
{
    if (inventory_size(head) >= MAX_INVENTORY_SIZE) return NULL;

    Inventory *new_item_entry = malloc(sizeof *new_item_entry);
    if (!new_item_entry)
    {
        fprintf(stderr, "ERROR: stuff.c: add_item_to_inventory: new_item_entry: malloc failed\n");
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

Item *remove_item_from_inventory(Inventory *head, Item *item_to_remove)
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
                Item *removed_item = current->item;
                free(current);
                return removed_item;
            }

            // Item found in the middle or end of the inventory.
            Item *removed_item = current->item;
            prev->next = current->next;
            free(current);
            return removed_item;
        }

        prev = current;
        current = current->next;
    }

    return NULL; // Item not found in the inventory.
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
