#include "save.h"



static int callback(void *data, int argc, char **argv, char **azColName) {
    return 0;
}
void insertItem(sqlite3 *db, ItemType type, const char *name, const char *description, unsigned short value, unsigned short price) {
    sqlite3_stmt *stmt;
    const char *sql_insert_item = "INSERT INTO itemList (idType, name, description, value, price) VALUES (?, ?, ?, ?, ?)";

    if (sqlite3_prepare_v2(db, sql_insert_item,-1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n",sqlite3_errmsg(db));
        exit(1);
    }

    sqlite3_bind_int(stmt, 1, type +1);
    sqlite3_bind_text(stmt, 2, name, -1,SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, description, -1,SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4,value);
    sqlite3_bind_int(stmt, 5, price);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Erreur lors de l'insertion de l'item : %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    sqlite3_finalize(stmt);
}


void createDatabaseAndTable(sqlite3 *db) {
    char *errMsg = 0;
     const char *sql_drop_table_item_type = "DROP TABLE IF EXISTS item_type;";
    const char *sql_create_table_player = "CREATE TABLE IF NOT EXISTS player(id INTEGER PRIMARY KEY, name VARCHAR(255), health INT, max_health INT, mana INT, max_mana INT, id_weapon INT, id_armor INT, gold INT, xp INT, xp_to_next_level INT);";
    const char *sql_create_table_item_type = "CREATE TABLE IF NOT EXISTS item_type(id INTEGER PRIMARY KEY AUTOINCREMENT, label VARCHAR(255));";
    const char *sql_insert_item_types = "INSERT INTO item_type (label) VALUES ('WEAPON'), ('ARMOR'), ('HEALTH_POTION'), ('MANA_POTION'), ('ATTACK_SPELL'), ('HEAL_SPELL');";
    const char *sql_create_table_item = "CREATE TABLE IF NOT EXISTS item(id INTEGER PRIMARY KEY AUTOINCREMENT, idType INT, name VARCHAR(255), description VARCHAR(255), value INT, price INT, xp INT, xp_to_next_level INT);";

    const char *sql_create_table_itemList = "CREATE TABLE IF NOT EXISTS itemList(id INTEGER PRIMARY KEY AUTOINCREMENT, idType INT, name VARCHAR(255), description VARCHAR(255), value INT, price INT, xp INT, xp_to_next_level INT);";
    const char *sql_delete_from_table_itemList = "DELETE FROM itemList";

if (sqlite3_exec(db, sql_drop_table_item_type, callback, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la suppression de la table item_type : %s\n", errMsg);
        sqlite3_free(errMsg);
        exit(1);
    }

    if (sqlite3_exec(db, sql_create_table_player, callback, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la création de la table player : %s\n", errMsg);
        sqlite3_free(errMsg);
        exit(1);
    }
        
    if (sqlite3_exec(db, sql_create_table_item_type, callback, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la création de la table item_type : %s\n", errMsg);
        sqlite3_free(errMsg);
        exit(1);
    }

    if (sqlite3_exec(db, sql_insert_item_types, callback, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'insertion des types d'item : %s\n", errMsg);
        sqlite3_free(errMsg);
        exit(1);
    }

    if (sqlite3_exec(db, sql_create_table_item, callback, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la création de la table item : %s\n", errMsg);
        sqlite3_free(errMsg);
        exit(1);
    }
    if (sqlite3_exec(db, sql_create_table_itemList, callback, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la création de la table itemList : %s\n", errMsg);
        sqlite3_free(errMsg);
        exit(1);
    }
if (sqlite3_exec(db, sql_delete_from_table_itemList, callback, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la supression de la table itemList : %s\n", errMsg);
        sqlite3_free(errMsg);
        exit(1);
    }
    insertItem(db, WEAPON, "Epée en acier", "Une épée en acier tranchante.", 20, 100);
    insertItem(db, WEAPON, "Arc long", "Un arc long pour tirer des flèches.", 15, 80);
    insertItem(db, WEAPON, "Dague empoisonnée", "Une petite dague avec du poison.", 25, 120);
    insertItem(db, WEAPON, "Marteau de guerre", "Un marteau de guerre massif.", 30, 150);

    insertItem(db, HEAL_SPELL, "Soin léger", "Un sort de soin léger.", 15, 50);
    insertItem(db, HEAL_SPELL, "Soin moyen", "Un sort de soin moyen.", 30, 100);
    insertItem(db, HEAL_SPELL, "Soin puissant", "Un sort de soin puissant.", 50, 150);

    insertItem(db, ATTACK_SPELL, "Boule de feu", "Une boule de feu magique.", 40, 120);
    insertItem(db, ATTACK_SPELL, "Éclair de foudre", "Un éclair de foudre dévastateur.", 60, 180);
    insertItem(db, ATTACK_SPELL, "Gel paralysant", "Un sort de gel paralysant.", 25, 80);


    insertItem(db, ARMOR, "Armure légère", "Une armure légère pour la protection.", 10, 60);
    insertItem(db, ARMOR, "Armure lourde", "Une armure lourde pour une grande protection.", 15, 90);
    insertItem(db, ARMOR, "Bouclier en bois", "Un bouclier en bois pour la défense.", 5, 40);
    insertItem(db, ARMOR, "Casque en fer", "Un casque en fer pour protéger la tête.", 7, 50);

    insertItem(db, HEALTH_POTION, "Potion de guérison mineure", "Une petite potion pour restaurer la santé.", 10, 40);
    insertItem(db, HEALTH_POTION, "Potion de guérison", "Une potion pour une guérison modérée.", 20, 80);
    insertItem(db, HEALTH_POTION, "Potion de guérison majeure", "Une puissante potion de guérison.", 30, 120);

    insertItem(db, MANA_POTION, "Potion de mana mineure", "Une petite potion pour restaurer le mana.", 10, 40);
    insertItem(db, MANA_POTION, "Potion de mana", "Une potion pour une restauration modérée du mana.", 20, 80);
    insertItem(db, MANA_POTION, "Potion de mana majeure", "Une puissante potion de mana.", 30, 120);
}

Item* getRandomItemFromDatabase(sqlite3 *db) {
    Item* item = NULL;
    char query[256];
    snprintf(query, sizeof(query), "SELECT id, name, description, value, price FROM itemList ORDER BY RANDOM() LIMIT 1;");

    sqlite3_stmt *stmt_item;
    if (sqlite3_prepare_v2(db, query, -1, &stmt_item, 0) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête SELECT item : %s\n", sqlite3_errmsg(db));
        exit(1);
    }
    
    if (sqlite3_step(stmt_item) == SQLITE_ROW) {
        item = malloc(sizeof(Item));
        ItemType item_type;
        int type_id = sqlite3_column_int(stmt_item, 0);
        if (type_id >= 1 && type_id <= 6) {
            item_type = (ItemType)(type_id - 1);
        }
        fprintf(stderr,strdup((const char *)sqlite3_column_text(stmt_item, 1)));
        item->type = item_type;
        item->name = strdup((const char *)sqlite3_column_text(stmt_item, 1));
        item->description = strdup((const char *)sqlite3_column_text(stmt_item, 2));
        item->value = sqlite3_column_int(stmt_item, 3);
        item->price = sqlite3_column_int(stmt_item, 4);
    }

    sqlite3_finalize(stmt_item);

    return item;
}

Item* loadItemFromDatabase(sqlite3 *db, int itemId) {
    Item* item = NULL;
    char query[256];
    snprintf(query, sizeof(query), "SELECT ity.id, name, description, value, price FROM item inner join item_type ity on ity.id = item.idType WHERE item.id = %d;", itemId);

    sqlite3_stmt *stmt_item;
    if (sqlite3_prepare_v2(db, query, -1, &stmt_item, 0) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête SELECT item : %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    if (sqlite3_step(stmt_item) == SQLITE_ROW) {
        item = malloc(sizeof(Item));
        ItemType item_type;
        int type_id = sqlite3_column_int(stmt_item, 1);
        if (type_id >= 1 && type_id <= 6) {
            item_type = (ItemType)type_id-1;
        }

        item->type = item_type;
        item->name = strdup((const char *)sqlite3_column_text(stmt_item, 1));
        item->description = strdup((const char *)sqlite3_column_text(stmt_item, 2));
        item->value = sqlite3_column_int(stmt_item, 3);
        item->price = sqlite3_column_int(stmt_item, 4);
    }

    sqlite3_finalize(stmt_item);

    return item;
}
Inventory* loadInventoryFromDatabase(sqlite3 *db) {
    Inventory *inventory = NULL;
    
    const char *sql_select_items = "SELECT item.id, idType, name, description, value, price FROM item WHERE idType NOT IN (5, 6) AND id NOT IN (SELECT id_weapon FROM player) AND id NOT IN (SELECT id_armor FROM player);";
    
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
Inventory* loadSpellsFromDatabase(sqlite3 *db) {
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

Character loadPlayerFromDatabase(sqlite3 *db) {
    Character player;

    const char *sql_select_player = "SELECT id, name, health, max_health, mana, max_mana, id_weapon, id_armor, gold, xp, xp_to_next_level FROM player;";

    sqlite3_stmt *stmt_player;
    if (sqlite3_prepare_v2(db, sql_select_player, -1, &stmt_player, 0) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la requête SELECT player : %s\n", sqlite3_errmsg(db));
        exit(1);
    }
    
    if (sqlite3_step(stmt_player) == SQLITE_ROW) {
        player.name = strdup((const char *)sqlite3_column_text(stmt_player, 1));
        player.health = sqlite3_column_int(stmt_player, 2);
        player.max_health = sqlite3_column_int(stmt_player, 3);
        player.mana = sqlite3_column_int(stmt_player, 4);
        player.max_mana = sqlite3_column_int(stmt_player, 5);
        player.gold =sqlite3_column_int(stmt_player, 8);
        player.xp =sqlite3_column_int(stmt_player, 9);
        player.xp_to_next_level =sqlite3_column_int(stmt_player, 10);
        int id_weapon = sqlite3_column_int(stmt_player, 6);
        
        player.weapon = loadItemFromDatabase(db, id_weapon);
         
        int id_armor = sqlite3_column_int(stmt_player, 7);
        player.armor = loadItemFromDatabase(db, id_armor);
        
        player.inventory = loadInventoryFromDatabase(db);
        
        player.spells = loadSpellsFromDatabase(db);
    } else {
        memset(&player, 0, sizeof(Character));
    }

    sqlite3_finalize(stmt_player);

    return player;
}



Character continueGame() {
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("doomdepth.db", &db);
    
    if (rc) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    
    Character player = loadPlayerFromDatabase(db);
    sqlite3_close(db);

    return player;
}

int insertWeapon(sqlite3 *db, Item *weapon) {
    const char *sql_insert_weapon = "INSERT INTO item (idType, name, description, value, price) VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt_insert_weapon;
    if (sqlite3_prepare_v2(db, sql_insert_weapon, -1, &stmt_insert_weapon, 0) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête INSERT weapon: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    sqlite3_bind_int(stmt_insert_weapon, 1, weapon->type+1);
    sqlite3_bind_text(stmt_insert_weapon, 2, weapon->name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt_insert_weapon, 3, weapon->description, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt_insert_weapon, 4, weapon->value);
    sqlite3_bind_int(stmt_insert_weapon, 5, weapon->price);

    if (sqlite3_step(stmt_insert_weapon) != SQLITE_DONE) {
        fprintf(stderr, "Erreur lors de l'insertion de l'arme : %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    int weaponId = sqlite3_last_insert_rowid(db);

    sqlite3_finalize(stmt_insert_weapon);

    return weaponId;
}

int insertArmor(sqlite3 *db, Item *armor) {
    const char *sql_insert_armor = "INSERT INTO item (idType, name, description, value, price) VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt_insert_armor;
    if (sqlite3_prepare_v2(db, sql_insert_armor, -1, &stmt_insert_armor, 0) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête INSERT armor: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    sqlite3_bind_int(stmt_insert_armor, 1, armor->type+1);
    sqlite3_bind_text(stmt_insert_armor, 2, armor->name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt_insert_armor, 3, armor->description, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt_insert_armor, 4, armor->value);
    sqlite3_bind_int(stmt_insert_armor, 5, armor->price);

    if (sqlite3_step(stmt_insert_armor) != SQLITE_DONE) {
        fprintf(stderr, "Erreur lors de l'insertion de l'armure : %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    int armorId = sqlite3_last_insert_rowid(db);

    sqlite3_finalize(stmt_insert_armor);

    return armorId;
}


void saveGame(Character player) {
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("doomdepth.db", &db);
    if (rc) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    
    const char *sql_delete_player = "DELETE FROM player;";
    const char *sql_delete_inventory = "DELETE FROM item;";

    char *errMsg = 0;

    if (sqlite3_exec(db, sql_delete_player, callback, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la suppression des données du joueur : %s\n", errMsg);
        sqlite3_free(errMsg);
        exit(1);
    }

    if (sqlite3_exec(db, sql_delete_inventory, callback, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la suppression des l'inventaire : %s\n", errMsg);
        sqlite3_free(errMsg);
        exit(1);
    }

    int weaponId = insertWeapon(db, player.weapon);

    int armorId = insertArmor(db, player.armor);

    const char *sql_insert_player = "INSERT INTO player (name, health, max_health, mana, max_mana, id_weapon, id_armor, gold, xp_to_next_level ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt_insert_player;
    if (sqlite3_prepare_v2(db, sql_insert_player, -1, &stmt_insert_player, 0) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête INSERT player : %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    sqlite3_bind_text(stmt_insert_player, 1, player.name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt_insert_player, 2, player.health);
    sqlite3_bind_int(stmt_insert_player, 3, player.max_health);
    sqlite3_bind_int(stmt_insert_player, 4, player.mana);
    sqlite3_bind_int(stmt_insert_player, 5, player.max_mana);
    sqlite3_bind_int(stmt_insert_player, 6, weaponId);
    sqlite3_bind_int(stmt_insert_player, 7, armorId);
    sqlite3_bind_int(stmt_insert_player, 8, player.gold);
    sqlite3_bind_int(stmt_insert_player, 9, player.xp);
    sqlite3_bind_int(stmt_insert_player, 10, player.xp_to_next_level);
    if (sqlite3_step(stmt_insert_player) != SQLITE_DONE) {
        fprintf(stderr, "Erreur lors de l'insertion des données du joueur : %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    sqlite3_finalize(stmt_insert_player);

    Inventory *inventory = player.inventory;

    while (inventory != NULL) {
        const char *sql_insert_inventory = "INSERT INTO item (idType, name, description, value, price) VALUES (?, ?, ?, ?, ?);";

        sqlite3_stmt *stmt_insert_inventory;
        if (sqlite3_prepare_v2(db, sql_insert_inventory, -1, &stmt_insert_inventory, 0) != SQLITE_OK) {
            fprintf(stderr, "Erreur lors de la préparation de la requête INSERT inventory : %s\n", sqlite3_errmsg(db));
            exit(1);
        }

        sqlite3_bind_int(stmt_insert_inventory, 1, inventory->item->type+1);
        sqlite3_bind_text(stmt_insert_inventory, 2, inventory->item->name, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt_insert_inventory, 3, inventory->item->description, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt_insert_inventory, 4, inventory->item->value);
        sqlite3_bind_int(stmt_insert_inventory, 5, inventory->item->price);

        if (sqlite3_step(stmt_insert_inventory) != SQLITE_DONE) {
            fprintf(stderr, "Erreur lors de l'insertion des données d'inventaire : %s\n", sqlite3_errmsg(db));
            exit(1);
        }

        sqlite3_finalize(stmt_insert_inventory);

        inventory = inventory->next;

    }

    Inventory *spells = player.spells;

    while (spells != NULL) {
        const char *sql_insert_spells = "INSERT INTO item (idType, name, description, value, price) VALUES (?, ?, ?, ?, ?);";

        sqlite3_stmt *stmt_insert_spells;
        if (sqlite3_prepare_v2(db, sql_insert_spells, -1, &stmt_insert_spells, 0) != SQLITE_OK) {
            fprintf(stderr, "Erreur lors de la préparation de la requête INSERT spells : %s\n", sqlite3_errmsg(db));
            exit(1);
        }

        sqlite3_bind_int(stmt_insert_spells, 1, spells->item->type+1);
        sqlite3_bind_text(stmt_insert_spells, 2, spells->item->name, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt_insert_spells, 3, spells->item->description, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt_insert_spells, 4, spells->item->value);
        sqlite3_bind_int(stmt_insert_spells, 5, spells->item->price);

        if (sqlite3_step(stmt_insert_spells) != SQLITE_DONE) {
            fprintf(stderr, "Erreur lors de l'insertion des données de sorts : %s\n", sqlite3_errmsg(db));
            exit(1);
        }

        sqlite3_finalize(stmt_insert_spells);

        spells = spells->next;
    }
    sqlite3_close(db);
}


