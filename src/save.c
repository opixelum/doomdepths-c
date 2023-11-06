#include "save.h"

static int callback(void *data, int argc, char **argv, char **azColName)
{
    return 0;
}

sqlite3 *open_database(const char *db_path)
{
    sqlite3 *db;

    if (sqlite3_open(db_path, &db) != SQLITE_OK)
    {
        fprintf(stderr, "save.c: open_database(): sqlite3_open(): %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    return db;
}

void create_tables(const char *db_path)
{
    sqlite3 *db = open_database(db_path);

    const char *sql_commands[] =
    {
        "CREATE TABLE IF NOT EXISTS player("
        "id INTEGER PRIMARY KEY, "
        "name VARCHAR(255), "
        "health INT, "
        "max_health INT, "
        "mana INT, "
        "max_mana INT, "
        "id_weapon INT, "
        "id_armor INT, "
        "gold INT, "
        "xp INT, "
        "xp_to_next_level INT);",

        "CREATE TABLE IF NOT EXISTS item_type("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "label VARCHAR(255));",

        "INSERT INTO item_type (label) "
        "VALUES ('WEAPON'), ('ARMOR'), ('HEALTH_POTION'), "
        "('MANA_POTION'), ('ATTACK_SPELL'), ('HEAL_SPELL');",

        "CREATE TABLE IF NOT EXISTS item("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "idType INT, "
        "name VARCHAR(255), "
        "description VARCHAR(255), "
        "value INT, "
        "price INT, "
        "xp INT, "
        "xp_to_next_level INT);"
    };

    int return_code;
    char *sqlite_err_msg = 0;

    for (size_t i = 0; i < sizeof(sql_commands) / sizeof(sql_commands[0]); i++)
    {
        return_code = sqlite3_exec(db, sql_commands[i], 0, 0, &sqlite_err_msg);
        if (return_code != SQLITE_OK)
        {
            fprintf(stderr, "save.c: create_tables(): %s\n", sqlite_err_msg);
            sqlite3_free(sqlite_err_msg);
            sqlite3_close(db);
            exit(1);
        }
    }

    sqlite3_close(db);
}

Item* loadItemFromDatabase(sqlite3 *db, int itemId)
{
    Item* item = NULL;
    char query[256];
    sqlite3_stmt *stmt_item;

    snprintf(query, sizeof(query), "SELECT ity.id, name, description, value, price FROM item inner join item_type ity on ity.id = item.idType WHERE item.id = %d;", itemId);

    if (sqlite3_prepare_v2(db, query, -1, &stmt_item, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de la préparation de la requête SELECT item : %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    if (sqlite3_step(stmt_item) == SQLITE_ROW)
    {
        item = malloc(sizeof(Item));
        ItemType item_type;

        int type_id = sqlite3_column_int(stmt_item, 1);
        if (type_id >= 1 && type_id <= 6) item_type = (ItemType)type_id-1;

        item->type = item_type;
        item->name = strdup((const char *)sqlite3_column_text(stmt_item, 1));
        item->description = strdup((const char *)sqlite3_column_text(stmt_item, 2));
        item->value = sqlite3_column_int(stmt_item, 3);
        item->price = sqlite3_column_int(stmt_item, 4);
    }

    sqlite3_finalize(stmt_item);

    return item;
}

Inventory* loadInventoryFromDatabase(sqlite3 *db)
{
    Inventory *inventory = NULL;
    sqlite3_stmt *stmt_items;

    const char *sql_select_items = "SELECT item.id, idType, name, description, value, price FROM item WHERE idType NOT IN (5, 6) AND id NOT IN (SELECT id_weapon FROM player) AND id NOT IN (SELECT id_armor FROM player);";

    if (sqlite3_prepare_v2(db, sql_select_items, -1, &stmt_items, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de la préparation de la requête SELECT items : %s\n", sqlite3_errmsg(db));
        exit(1);
    }
    

    while (sqlite3_step(stmt_items) == SQLITE_ROW)
    {
        Inventory *itemNode = malloc(sizeof(Inventory));
        ItemType item_type;

        int type_id = sqlite3_column_int(stmt_items, 1);
        if (type_id >= 1 && type_id <= 6) item_type = (ItemType)type_id-1;

        itemNode->item = malloc(sizeof(Item));
        itemNode->item->type = item_type;
        itemNode->item->name = strdup((const char *)sqlite3_column_text(stmt_items, 2));
        itemNode->item->description = strdup((const char *)sqlite3_column_text(stmt_items, 3));
        itemNode->item->value = sqlite3_column_int(stmt_items, 4);
        itemNode->item->price = sqlite3_column_int(stmt_items, 5);

        itemNode->next = inventory;
        inventory = itemNode;
    }

    sqlite3_finalize(stmt_items);

    return inventory;
}

Inventory* loadSpellsFromDatabase(sqlite3 *db)
{
    Inventory *inventory = NULL;

    const char *sql_select_items = "SELECT item.id, idType, name, description, value, price FROM item WHERE idType IN (5, 6) AND id NOT IN (SELECT id_weapon FROM player) AND id NOT IN (SELECT id_armor FROM player);";
    
    sqlite3_stmt *stmt_items;
    if (sqlite3_prepare_v2(db, sql_select_items, -1, &stmt_items, 0) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête SELECT items : %s\n", sqlite3_errmsg(db));
        exit(1);
    }


    while (sqlite3_step(stmt_items) == SQLITE_ROW) {
        Inventory *itemNode = malloc(sizeof(Inventory));
        ItemType item_type;
        int type_id = sqlite3_column_int(stmt_items, 1);
        if (type_id >= 1 && type_id <= 6) {
            item_type = (ItemType)type_id-1;
        }
        itemNode->item = malloc(sizeof(Item));
        itemNode->item->type = item_type;
        itemNode->item->name = strdup((const char *)sqlite3_column_text(stmt_items, 2));
        itemNode->item->description = strdup((const char *)sqlite3_column_text(stmt_items, 3));
        itemNode->item->value = sqlite3_column_int(stmt_items, 4);
        itemNode->item->price = sqlite3_column_int(stmt_items, 5);

        itemNode->next = inventory;
        inventory = itemNode;
    }

    sqlite3_finalize(stmt_items);

    return inventory;
}

Character *loadPlayerFromDatabase(sqlite3 *db)
{
    Character *player = malloc(sizeof *player);
    sqlite3_stmt *stmt_player;

    const char *sql_select_player = "SELECT id, name, health, max_health, mana, max_mana, id_weapon, id_armor, gold, xp, xp_to_next_level FROM player;";

    if (sqlite3_prepare_v2(db, sql_select_player, -1, &stmt_player, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de la requête SELECT player : %s\n", sqlite3_errmsg(db));
        exit(1);
    }
    
    if (sqlite3_step(stmt_player) == SQLITE_ROW)
    {
        player->name = strdup((const char *)sqlite3_column_text(stmt_player, 1));
        player->health = sqlite3_column_int(stmt_player, 2);
        player->max_health = sqlite3_column_int(stmt_player, 3);
        player->mana = sqlite3_column_int(stmt_player, 4);
        player->max_mana = sqlite3_column_int(stmt_player, 5);
        player->gold = sqlite3_column_int(stmt_player, 8);
        player->xp = sqlite3_column_int(stmt_player, 9);
        player->xp_to_next_level =sqlite3_column_int(stmt_player, 10);
        int id_weapon = sqlite3_column_int(stmt_player, 6);
        
        player->weapon = loadItemFromDatabase(db, id_weapon);
         
        int id_armor = sqlite3_column_int(stmt_player, 7);
        player->armor = loadItemFromDatabase(db, id_armor);
        
        player->inventory = loadInventoryFromDatabase(db);
        
        player->spells = loadSpellsFromDatabase(db);
    } else memset(&player, 0, sizeof(Character));

    sqlite3_finalize(stmt_player);

    return player;
}

Character *load_game()
{
    sqlite3 *db = open_database("doomdepths.db");

    Character *player = loadPlayerFromDatabase(db);
    sqlite3_close(db);

    return player;
}

int insertWeapon(sqlite3 *db, Item *weapon)
{
    sqlite3_stmt *stmt_insert_weapon;
    const char *sql_insert_weapon = "INSERT INTO item (idType, name, description, value, price) VALUES (?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql_insert_weapon, -1, &stmt_insert_weapon, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de la préparation de la requête INSERT weapon: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    sqlite3_bind_int(stmt_insert_weapon, 1, weapon->type+1);
    sqlite3_bind_text(stmt_insert_weapon, 2, weapon->name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt_insert_weapon, 3, weapon->description, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt_insert_weapon, 4, weapon->value);
    sqlite3_bind_int(stmt_insert_weapon, 5, weapon->price);

    if (sqlite3_step(stmt_insert_weapon) != SQLITE_DONE)
    {
        fprintf(stderr, "Erreur lors de l'insertion de l'arme : %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    int weaponId = sqlite3_last_insert_rowid(db);

    sqlite3_finalize(stmt_insert_weapon);

    return weaponId;
}

int insertArmor(sqlite3 *db, Item *armor)
{
    sqlite3_stmt *stmt_insert_armor;
    const char *sql_insert_armor = "INSERT INTO item (idType, name, description, value, price) VALUES (?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql_insert_armor, -1, &stmt_insert_armor, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de la préparation de la requête INSERT armor: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    sqlite3_bind_int(stmt_insert_armor, 1, armor->type+1);
    sqlite3_bind_text(stmt_insert_armor, 2, armor->name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt_insert_armor, 3, armor->description, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt_insert_armor, 4, armor->value);
    sqlite3_bind_int(stmt_insert_armor, 5, armor->price);

    if (sqlite3_step(stmt_insert_armor) != SQLITE_DONE)
    {
        fprintf(stderr, "Erreur lors de l'insertion de l'armure : %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    int armorId = sqlite3_last_insert_rowid(db);

    sqlite3_finalize(stmt_insert_armor);

    return armorId;
}


void save_game(Character *player)
{
    sqlite3 *db = open_database("doomdepths.db");

    const char *sql_delete_player = "DELETE FROM player;";
    const char *sql_delete_inventory = "DELETE FROM item;";

    char *errMsg = 0;

    if (sqlite3_exec(db, sql_delete_player, callback, 0, &errMsg) != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de la suppression des données du joueur : %s\n", errMsg);
        sqlite3_free(errMsg);
        exit(1);
    }

    if (sqlite3_exec(db, sql_delete_inventory, callback, 0, &errMsg) != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de la suppression des l'inventaire : %s\n", errMsg);
        sqlite3_free(errMsg);
        exit(1);
    }

    int weapon_id, armor_id;
    if (player->weapon) weapon_id = insertWeapon(db, player->weapon);
    if (player->armor) armor_id = insertArmor(db, player->armor);

    sqlite3_stmt *stmt_insert_player;
    const char *sql_insert_player = "INSERT INTO player (name, health, max_health, mana, max_mana, id_weapon, id_armor, gold, xp, xp_to_next_level ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql_insert_player, -1, &stmt_insert_player, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de la préparation de la requête INSERT player : %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    sqlite3_bind_text(stmt_insert_player, 1, player->name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt_insert_player, 2, player->health);
    sqlite3_bind_int(stmt_insert_player, 3, player->max_health);
    sqlite3_bind_int(stmt_insert_player, 4, player->mana);
    sqlite3_bind_int(stmt_insert_player, 5, player->max_mana);
    sqlite3_bind_int(stmt_insert_player, 6, weapon_id);
    sqlite3_bind_int(stmt_insert_player, 7, armor_id);
    sqlite3_bind_int(stmt_insert_player, 8, player->gold);
    sqlite3_bind_int(stmt_insert_player, 9, player->xp);
    sqlite3_bind_int(stmt_insert_player, 10, player->xp_to_next_level);

    if (sqlite3_step(stmt_insert_player) != SQLITE_DONE)
    {
        fprintf(stderr, "Erreur lors de l'insertion des données du joueur : %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    sqlite3_finalize(stmt_insert_player);

    Inventory *inventory = player->inventory;
    while (inventory)
    {
        const char *sql_insert_inventory = "INSERT INTO item (idType, name, description, value, price) VALUES (?, ?, ?, ?, ?);";

        sqlite3_stmt *stmt_insert_inventory;
        if (sqlite3_prepare_v2(db, sql_insert_inventory, -1, &stmt_insert_inventory, 0) != SQLITE_OK)
        {
            fprintf(stderr, "Erreur lors de la préparation de la requête INSERT inventory : %s\n", sqlite3_errmsg(db));
            exit(1);
        }

        sqlite3_bind_int(stmt_insert_inventory, 1, inventory->item->type+1);
        sqlite3_bind_text(stmt_insert_inventory, 2, inventory->item->name, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt_insert_inventory, 3, inventory->item->description, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt_insert_inventory, 4, inventory->item->value);
        sqlite3_bind_int(stmt_insert_inventory, 5, inventory->item->price);

        if (sqlite3_step(stmt_insert_inventory) != SQLITE_DONE)
        {
            fprintf(stderr, "Erreur lors de l'insertion des données d'inventaire : %s\n", sqlite3_errmsg(db));
            exit(1);
        }

        sqlite3_finalize(stmt_insert_inventory);

        inventory = inventory->next;
    }

    Inventory *spells = player->spells;
    while (spells)
    {
        sqlite3_stmt *stmt_insert_spells;
        const char *sql_insert_spells = "INSERT INTO item (idType, name, description, value, price) VALUES (?, ?, ?, ?, ?);";

        if (sqlite3_prepare_v2(db, sql_insert_spells, -1, &stmt_insert_spells, 0) != SQLITE_OK)
        {
            fprintf(stderr, "Erreur lors de la préparation de la requête INSERT spells : %s\n", sqlite3_errmsg(db));
            exit(1);
        }

        sqlite3_bind_int(stmt_insert_spells, 1, spells->item->type+1);
        sqlite3_bind_text(stmt_insert_spells, 2, spells->item->name, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt_insert_spells, 3, spells->item->description, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt_insert_spells, 4, spells->item->value);
        sqlite3_bind_int(stmt_insert_spells, 5, spells->item->price);

        if (sqlite3_step(stmt_insert_spells) != SQLITE_DONE)
        {
            fprintf(stderr, "Erreur lors de l'insertion des données de sorts : %s\n", sqlite3_errmsg(db));
            exit(1);
        }

        sqlite3_finalize(stmt_insert_spells);

        spells = spells->next;
    }

    sqlite3_close(db);
}


