#ifndef SAVE_H
#define SAVE_H

#include "character.h"
#include <stdio.h>
#include <sqlite3.h>
#include "stuff.h"
#include <string.h>

void create_tables(const char *db_path);
void save_game(Character player);
Character continueGame();

#endif // SAVE_H
