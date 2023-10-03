#ifndef STUFF_H
#define STUFF_H

#define MAX_INVENTORY_SIZE 25

typedef enum ItemType
{
    WEAPON,
    ARMOR,
    HEALTH_POTION,
    MANA_POTION
} ItemType;

typedef struct Item
{
    ItemType type;
    char *name;
    char *description;
    unsigned short value; // Dealing damage if weapon, reducing damage if armor, healing if health potion, etc.
    unsigned short price; // In gold
} Item;

typedef struct Inventory
{
    Item *item;
    struct Inventory *next;
} Inventory;

#endif // STUFF_H
