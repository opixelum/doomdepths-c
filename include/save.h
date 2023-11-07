#ifndef SAVE_H
#define SAVE_H

#include "character.h"
#include <stdio.h>
#include <sqlite3.h>
#include "stuff.h"
#include <string.h>
void saveGame(Character player);
Character continueGame();
Item* getRandomItemFromDatabase(sqlite3 *db);
void createDatabaseAndTable(sqlite3 *db);

#endif // SAVE_H
