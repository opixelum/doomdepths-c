#ifndef SAVE_H
#define SAVE_H

#include <sqlite3.h>
#include <string.h>
#include "character.h"
#include "map.h"
struct MapContext;

void save_mapcontext(const char *db_path, struct MapContext *mapcontext);

struct MapContext *get_mapcontext(const char *db_path);
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
unsigned char insert_item(const char *db_path, Item *item);


/**
 * @brief Adds game's items in the database.
 * @return 0 if success, 1 if failure
 */
unsigned char init_db_items(const char *db_path);

#endif // SAVE_H
