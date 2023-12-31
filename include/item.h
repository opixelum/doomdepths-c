#ifndef STUFF_H
#define STUFF_H

#include <stdio.h>
#include <stdlib.h>

#include "terminal.h"

#define MAX_INVENTORY_SIZE 25

typedef enum ItemType
{
    ARMOR,
    WEAPON,
    ITEM, // Any item
    POTION, // Health or mana potion
    HEALTH_POTION,
    MANA_POTION,
    SPELL, // Attack or heal spell
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

typedef struct ItemCount
{
    Item *item;
    unsigned char count;
}
ItemCount;

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
 * @brief Adds an item to the end of an inventory. If inventory is full, the item
 * is not added.
 * @param head A pointer to the head of the inventory.
 * @param item A pointer to the item to be added.
 * @return A pointer to the given inventory head or the new inventory head if
 * the given inventory head was NULL or NULL if the inventory is full.
 * @warning In case you're not passing the head of the inventory, the length
 * check will may not work properly.
 */
Inventory *add_item_to_inventory(Inventory *head, Item *item);

/**
 * @brief Removes an item from an inventory.
 * @param head A pointer to the head of the inventory.
 * @param item A pointer to the item to be removed.
 * @return A pointer to the head of the updated inventory.
 * @warning This function does not free the item from memory, it only removes it
 * from the inventory by freeing the inventory node.
 * @warning In case you're not passing the head of the inventory, the item to
 * remove can be skipped if it is before the given inventory node.
 */
Inventory *remove_item_from_inventory(Inventory *head, Item *item);

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
 * @param capitalize 1 if the first letter must be capitalized, 0 otherwise.
 * @return A string literal containing the item type.
 */
char *item_type_to_string(ItemType item_type, unsigned char capitalize);

/**
 * @brief Get the number of given item type in an inventory (normal one or
 * spells list).
 * @param head A pointer to the head of the inventory.
 * @param item_type The type of the item to count (see ItemType enum). Pass
 * `Item` to count all items in inventory.
 * @return The number of items of the given type in the inventory.
 * @warning If passed inventory is not the head of the inventory, some items
 * may be skipped.
 */
unsigned char number_of_items_by_type(Inventory *head, ItemType item_type);

/**
 * @brief Get the number of given item in the inventory.
 * @param head A pointer to the head of the inventory.
 * @param item The item to count.
 * @return The number of occurrences of the given item in the inventory.
 * @warning If passed inventory is not the head of the inventory, some items
 * may be skipped.
 */
unsigned char number_of_items(Inventory *head, Item *item);

/**
 * @brief Check if an item is a spell.
 * @param type The type of the item to check.
 * @return 1 if the item is a spell, 0 otherwise.
 */
unsigned char is_spell(ItemType type);

/**
 * @brief Check if an item is a potion.
 * @param type The type of the item to check.
 * @return 1 if the item is a potion, 0 otherwise.
 */
unsigned char is_potion(ItemType type);

#endif // STUFF_H
