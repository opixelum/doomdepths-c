#ifndef STUFF_H
#define STUFF_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_INVENTORY_SIZE 25

typedef enum ItemType
{
    WEAPON,
    ARMOR,
    HEALTH_POTION,
    MANA_POTION,
    ATTACK_SPELL,
    HEAL_SPELL
}
ItemType;

typedef struct Item
{
    ItemType type;
    char *name;
    char *description;
    unsigned short value; // Dealing damage if weapon, reducing damage if armor, healing if health potion, etc.
    unsigned short price; // In gold, or mana if spell
}
Item;

typedef struct Inventory
{
    Item *item;
    struct Inventory *next;
}
Inventory;

/**
 * @brief Initialize a new item
 * @param type The type of the item (WEAPON, ARMOR, HEALTH_POTION, MANA_POTION
 * @param name Pointer to the name of the item
 * @param description Pointer to the description of the item
 * @param value The value of the item (see Item.value)
 * @param price The price of the item in gold
 * @return A pointer to the new item
 * @warning The item must be freed with free_item() when it is no longer needed
 */
Item *create_item
(
    ItemType type,
    char *name,
    char *description,
    unsigned short value,
    unsigned short price
);

/**
 * @brief Get the size of an inventory
 * @param head Pointer to the head of the inventory
 * @return The size of the inventory
 * @warning In case you're not passing the head of the inventory, the size check will may not work properly
 */
unsigned char inventory_size(Inventory *head);

/**
 * @brief Add an item to the end of an inventory. If inventory is full, the item is not added
 * @param head Pointer to the head of the inventory
 * @param item Pointer to the item to be added
 * @return Pointer to the given inventory head or the new inventory head if the given inventory head was NULL
 * or NULL if the inventory is full
 * @warning In case you're not passing the head of the inventory, the size check will may not work properly
 */
Inventory *add_item_to_inventory(Inventory *head, Item *item);

/**
 * @brief Remove an item from an inventory
 * @param head Pointer to the head of the inventory
 * @param item Pointer to the item to be removed
 * @return Pointer to the removed item, or NULL if the item was not found
 * @warning This function does not free the item from memory, it only removes it from the inventory
 * @warning In case you're not passing the head of the inventory, the item to remove can be skipped if it is before the
 * given inventory node
 */
Item *remove_item_from_inventory(Inventory *head, Item *item);

/**
 * @brief Frees a linked list of items from memory.
 * @param head A pointer to the head of the linked list of items to be freed.
 * @warning No item in the linked list will be freed from memory. It only frees
 * the linked list. You'll have to free each item manually if needed.
 * @warning In case you're not passing the head of the inventory, only the
 * following nodes will be freed.
 */
void free_inventory(Inventory *head);

/**
 * @brief Get the item type but in string format.
 * @param item_type The item type.
 * @return A string literal containing the item type.
 */
char *item_type_to_string(ItemType item_type);

#endif // STUFF_H
