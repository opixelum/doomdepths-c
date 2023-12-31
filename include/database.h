#ifndef SAVE_H
#define SAVE_H

#include <sqlite3.h>
#include <string.h>
#include "character.h"
#include "map.h"
struct MapContext;

void save_map_context(const char *db_path, struct MapContext *mapcontext);

struct MapContext *get_map_context(const char *db_path);
/**
 * @brief Creates a database if it doesn't exist already and opens a connection
 * to it.
 * @param db_path A string containing the path to the database.
 * @return A pointer to the database connection.
 * @warning The caller is responsible for closing the connection with
 * `sqlite3_close()`.
 */
sqlite3 *open_database(const char *db_path);

/**
 * @brief Creates the tables in the database.
 * @param db_path A string containing the path to the database.
 */
void create_tables(const char *db_path);

/**
 * @brief Saves the game to the database.
 * @param player A pointer to the player character.
 */
void save_game(Character *player);

/**
 * @brief Loads the game from the database.
 * @return A pointer to the player character.
 */
Character *load_game();

/**
 * @brief Insert an item into the database.
 * @param db_path A string containing the path to the database.
 * @param item A pointer to the item to insert.
 * @return 0 if the item was inserted successfully, 1 otherwise.
 */
unsigned char insert_item_into_inventory(const char *db_path, Item *item);

void insert_item_into_items_list(sqlite3 *db, ItemType type, const char *name, const char *description, unsigned short value, unsigned short price);

Item* get_random_item_from_database(sqlite3 *db);

/**
 * @brief Fills an inventory with random items. Useful for monsters and chests.
 * @return A pointer to the head of the inventory.
 * @warning The inventory must be freed with free_inventory() when it's no
 * longer needed.
 */
Inventory *generate_random_inventory(void);

/**
 * @brief Gets the id of an item from its name.
 * @param db A pointer to the database connection.
 * @param item_name A string containing the name of the item.
 * @return The id of the item, or -1 if the item doesn't exist.
 */
int get_item_id(sqlite3 *db, const char *item_name);

#endif // SAVE_H
