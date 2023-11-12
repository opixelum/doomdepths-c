#ifndef SAVE_H
#define SAVE_H

#include <sqlite3.h>
#include <string.h>
#include "character.h"

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

#endif // SAVE_H
