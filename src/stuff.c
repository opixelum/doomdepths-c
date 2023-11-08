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

unsigned char init_db_items(void)
{
    unsigned char return_value;

    // Health potions
    Item *heartbloom_draught = create_item
    (
        HEALTH_POTION,
        "Heartbloom Draught",
        "A draught made from the heartbloom flower. Restores 9999 HP.",
        9999,
        1000
    );
    return_value = insert_item("doomdepths.db", heartbloom_draught);
    free(heartbloom_draught);
    if (return_value == EXIT_FAILURE)
    {
        fprintf
        (
            stderr,
            "ERROR: stuff.c: init_db_items(): insert_item(heartbloom_draught) failed\n"
        );
        return EXIT_FAILURE;
    }

    Item *crimson_elixir_of_regeneration = create_item
    (
        HEALTH_POTION,
        "Crimson Elixir of Regeneration",
        "A crimson elixir that restores 100 HP.",
        100,
        100
    );
    return_value = insert_item("doomdepths.db", crimson_elixir_of_regeneration);
    free(crimson_elixir_of_regeneration);
    if (return_value == EXIT_FAILURE)
    {
        fprintf
        (
            stderr,
            "ERROR: stuff.c: init_db_items(): insert_item(crimson_elixir_of_regeneration) failed\n"
        );
        return EXIT_FAILURE;
    }

    Item *vitality_essence = create_item
    (
        HEALTH_POTION,
        "Vitality's Essence",
        "A green essence that restores 25 HP.",
        25,
        30
    );
    return_value = insert_item("doomdepths.db", vitality_essence);
    free(vitality_essence);
    if (return_value == EXIT_FAILURE)
    {
        fprintf
        (
            stderr,
            "ERROR: stuff.c: init_db_items(): insert_item(vitality_essence) failed\n"
        );
        return EXIT_FAILURE;
    }

    // Mana potions
    Item *mindfrost_decoction = create_item
    (
        MANA_POTION,
        "Mindfrost Decoction",
        "A decoction made from the mindfrost flower. Restores 9999 MP.",
        9999,
        1000
    );
    return_value = insert_item("doomdepths.db", mindfrost_decoction);
    free(mindfrost_decoction);
    if (return_value == EXIT_FAILURE)
    {
        fprintf
        (
            stderr,
            "ERROR: stuff.c: init_db_items(): insert_item(mindfrost_decoction) failed\n"
        );
        return EXIT_FAILURE;
    }

    Item *sorcerer_siphon_brew = create_item
    (
        MANA_POTION,
        "Sorcerer's Siphon Brew",
        "A blue brew that restores 100 MP.",
        100,
        100
    );
    return_value = insert_item("doomdepths.db", sorcerer_siphon_brew);
    free(sorcerer_siphon_brew);
    if (return_value == EXIT_FAILURE)
    {
        fprintf
        (
            stderr,
            "ERROR: stuff.c: init_db_items(): insert_item(sorcerer_siphon_brew) failed\n"
        );
        return EXIT_FAILURE;
    }

    Item *azure_mystique_vial = create_item
    (
        MANA_POTION,
        "Azure Mystique Vial",
        "A blue vial that restores 25 MP.",
        25,
        30
    );
    return_value = insert_item("doomdepths.db", azure_mystique_vial);
    free(azure_mystique_vial);
    if (return_value == EXIT_FAILURE)
    {
        fprintf
        (
            stderr,
            "ERROR: stuff.c: init_db_items(): insert_item(azure_mystique_vial) failed\n"
        );
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
