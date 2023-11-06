#ifndef SAVE_H
#define SAVE_H

#include "character.h"
#include <stdio.h>
#include <sqlite3.h>
#include "stuff.h"
#include <string.h>

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

#endif // SAVE_H
