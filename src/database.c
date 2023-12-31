#include "database.h"

sqlite3 *open_database(const char *db_path)
{
    sqlite3 *db;

    if (sqlite3_open(db_path, &db) != SQLITE_OK)
    {
        fprintf
        (
            stderr,
            "ERROR: database.c: open_database(): sqlite3_open(): %s\n",
            sqlite3_errmsg(db)
        );
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    return db;
}

void create_tables(const char *db_path)
{
    sqlite3 *db = open_database(db_path);

    const char *sql_stmts[] =
    {
        "DROP TABLE IF EXISTS characters;",

        "CREATE TABLE IF NOT EXISTS characters("
        "id INTEGER PRIMARY KEY, "
        "name VARCHAR(255) NOT NULL, "
        "health INT NOT NULL DEFAULT 100, "
        "max_health INT NOT NULL DEFAULT 100, "
        "mana INT NOT NULL DEFAULT 100, "
        "max_mana INT NOT NULL DEFAULT 100, "
        "weapon_id INT, "
        "armor_id INT, "
        "gold INT NOT NULL DEFAULT 0, "
        "xp INT NOT NULL DEFAULT 0, "
        "xp_to_next_level INT NOT NULL DEFAULT 1000);",

        "DROP TABLE IF EXISTS item_types;",

        "CREATE TABLE IF NOT EXISTS item_types("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "label VARCHAR(255) NOT NULL);",

        "INSERT INTO item_types (label) "
        "VALUES ('ARMOR'), ('WEAPON'), ('ITEM'), "
        "('POTION'), ('HEALTH_POTION'), ('MANA_POTION'),"
        "('SPELL'), ('ATTACK_SPELL'), ('HEAL_SPELL');",

        "DROP TABLE IF EXISTS inventory;",

        "CREATE TABLE IF NOT EXISTS inventory("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "type_id INT NOT NULL, "
        "name VARCHAR(255) NOT NULL, "
        "description VARCHAR(255) NOT NULL, "
        "value INT NOT NULL, "
        "price INT NOT NULL);",

        "DROP TABLE IF EXISTS spells;",

        "CREATE TABLE IF NOT EXISTS spells("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "type_id INT NOT NULL, "
        "name VARCHAR(255) NOT NULL, "
        "description VARCHAR(255) NOT NULL, "
        "value INT NOT NULL, "
        "price INT NOT NULL);",

        "DROP TABLE IF EXISTS items_list;",

        "CREATE TABLE IF NOT EXISTS items_list("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "type_id INT NOT NULL, "
        "name VARCHAR(255) NOT NULL, "
        "description VARCHAR(255) NOT NULL, "
        "value INT NOT NULL, "
        "price INT NOT NULL);",
      
        "DROP TABLE IF EXISTS map_context;",
      
        "CREATE TABLE IF NOT EXISTS map_context("
        "pos_x INT,"
        "pos_y INT);"
    };

    char *sqlite_err_msg = 0;
    unsigned char sql_commands_count = sizeof(sql_stmts) / sizeof(sql_stmts[0]);

    for (size_t i = 0; i < sql_commands_count; i++)
    {
        int return_value = sqlite3_exec
        (
            db,
            sql_stmts[i],
            0,
            0,
            &sqlite_err_msg
        );

        if (return_value != SQLITE_OK)
        {
            fprintf
            (
                stderr,
                "ERROR: database.c: create_tables(): %s\n",
                sqlite_err_msg
            );
            sqlite3_free(sqlite_err_msg);
            sqlite3_close(db);
            exit(1);
        }
    }

    insert_item_into_items_list(
        db,
        WEAPON,
        "Steel Sword",
        "A sharp steel sword.",
        20,
        100
    );
    insert_item_into_items_list(
        db,
        WEAPON,
        "Long Bow",
        "A long bow for shooting arrows.",
        15,
        80
                               );
    insert_item_into_items_list(
        db,
        WEAPON,
        "Poisoned Dagger",
        "A small dagger with poison.",
        25,
        120
                               );
    insert_item_into_items_list(
        db,
        WEAPON,
        "War Hammer",
        "A massive war hammer.",
        30,
        150
                               );

    insert_item_into_items_list(
        db,
        HEAL_SPELL,
        "Light Heal",
        "A spell for light healing.",
        15,
        50
                               );
    insert_item_into_items_list(
        db,
        HEAL_SPELL,
        "Medium Heal",
        "A spell for medium healing.",
        30,
        100
                               );
    insert_item_into_items_list(
        db,
        HEAL_SPELL,
        "Powerful Heal",
        "A spell for powerful healing.",
        50,
        150
                               );

    insert_item_into_items_list(
        db,
        ATTACK_SPELL,
        "Fireball",
        "A magical fireball.",
        40,
        120
                               );
    insert_item_into_items_list(
        db,
        ATTACK_SPELL,
        "Lightning Bolt",
        "A devastating lightning bolt.",
        60,
        180
                               );
    insert_item_into_items_list(
        db,
        ATTACK_SPELL,
        "Paralyzing Freeze",
        "A spell of paralyzing freeze.",
        25,
        80
                               );

    insert_item_into_items_list(
        db,
        ARMOR,
        "Light Armor",
        "A light armor for protection.",
        10,
        60
    );
    insert_item_into_items_list(
        db,
        ARMOR,
        "Heavy Armor",
        "A heavy armor for great protection.",
        15,
        90
    );
    insert_item_into_items_list(
        db,
        ARMOR,
        "Wooden Shield",
        "A wooden shield for defense.",
        5,
        40
    );
    insert_item_into_items_list(
        db,
        ARMOR,
        "Iron Helmet",
        "An iron helmet to protect the head.",
        7,
        50
    );

    insert_item_into_items_list(
        db,
        HEALTH_POTION,
        "Minor Healing Potion",
        "A small potion to restore health.",
        10,
        40
    );
    insert_item_into_items_list(
        db,
        HEALTH_POTION,
        "Healing Potion",
        "A potion for moderate healing.",
        20,
        80
    );
    insert_item_into_items_list(
        db,
        HEALTH_POTION,
        "Major Healing Potion",
        "A powerful healing potion.",
        30,
        120
    );

    insert_item_into_items_list(
        db,
        MANA_POTION,
        "Minor Mana Potion",
        "A small potion to restore mana.",
        10,
        40
    );
    insert_item_into_items_list(
        db,
        MANA_POTION,
        "Mana Potion",
        "A potion for moderate mana restoration.",
        20,
        80
    );
    insert_item_into_items_list(
        db,
        MANA_POTION,
        "Major Mana Potion",
        "A powerful mana potion.",
        30,
        120
    );

    sqlite3_close(db);
}

Item *get_equipped_item_from_db(sqlite3 *db, int item_id)
{
    Item *item = NULL;
    char query[256];
    sqlite3_stmt *stmt_item;

    snprintf
    (
        query,
        sizeof(query),
        "SELECT type_id, name, description, value, price FROM items_list "
        "WHERE id = %d;",
        item_id
    );

    int return_value = sqlite3_prepare_v2
    (
        db,
        query,
        -1,
        &stmt_item,
        0
    );
    if (return_value != SQLITE_OK)
    {
        fprintf
        (
            stderr,
            "ERROR: database.c: get_equipped_item_from_db(): sqlite3_prepare_v2(): "
            "%s\n",
            sqlite3_errmsg(db)
        );
        exit(EXIT_FAILURE);
    }

    if (sqlite3_step(stmt_item) == SQLITE_ROW)
    {
        item = malloc(sizeof(Item));

        int type_id = sqlite3_column_int(stmt_item, 0) - 1;
        if (type_id >= 0 && type_id <= 8) item->type = type_id;
        else
        {
            fprintf
            (
                stderr,
                "ERROR: database.c: get_equipped_item_from_db(): Invalid item "
                "type ID: %d\n",
                type_id
            );
            exit(EXIT_FAILURE);
        }

        item->name =
            strdup((const char *) sqlite3_column_text(stmt_item, 1));
        item->description = strdup(
            (const char *) sqlite3_column_text(stmt_item, 2));
        item->value = sqlite3_column_int(stmt_item, 3);
        item->price = sqlite3_column_int(stmt_item, 4);
    }

    sqlite3_finalize(stmt_item);

    return item;
}

Item *get_item_from_db(sqlite3 *db, int item_id)
{
    Item *item = NULL;
    char query[256];
    sqlite3_stmt *stmt_item;

    snprintf
    (
        query,
        sizeof(query),
        "SELECT ity.id, name, description, value, price "
        "FROM inventory inner join item_types ity on ity.id = inventory.type_id "
        "WHERE inventory.id = %d;",
        item_id
    );

    int return_value = sqlite3_prepare_v2
    (
        db,
        query,
        -1,
        &stmt_item,
        0
    );
    if (return_value != SQLITE_OK)
    {
        fprintf
        (
            stderr,
            "ERROR: database.c: get_item_from_db(): sqlite3_prepare_v2(): "
            "%s\n",
            sqlite3_errmsg(db)
        );
        exit(EXIT_FAILURE);
    }

    if (sqlite3_step(stmt_item) == SQLITE_ROW)
    {
        item = malloc(sizeof(Item));
        ItemType item_type;

        int type_id = sqlite3_column_int(stmt_item, 1);
        if (type_id >= 0 && type_id <= 8) item_type = type_id;
        else
        {
            fprintf
            (
                stderr,
                "ERROR: database.c: get_item_from_db(): Invalid item "
                "type ID: %d\n",
                type_id
            );
            exit(EXIT_FAILURE);
        }

        item->type = item_type;
        item->name =
            strdup((const char *) sqlite3_column_text(stmt_item, 1));
        item->description = strdup(
            (const char *) sqlite3_column_text(stmt_item, 2));
        item->value = sqlite3_column_int(stmt_item, 3);
        item->price = sqlite3_column_int(stmt_item, 4);
    }

    sqlite3_finalize(stmt_item);

    return item;
}

Inventory* get_inventory_from_db(sqlite3 *db)
{
    sqlite3_stmt *stmt;
    const char *sql =
        "SELECT i.id, i.type_id, i.name, i.description, i.value, i.price "
        "FROM inventory i "
        "LEFT JOIN characters c ON (i.id = c.weapon_id OR i.id = c.armor_id) "
        "WHERE c.id IS NULL";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf
        (
            stderr,
            "ERROR: database.c: get_inventory_from_db(): sqlite3_prepare_v2(): %s\n",
            sqlite3_errmsg(db)
        );
        return NULL;
    }

    Inventory *head = NULL;
    Inventory *current = NULL;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Inventory *new_node = malloc(sizeof(Inventory));
        if (!new_node)
        {
            fprintf
            (
                stderr,
                "ERROR: database.c: get_inventory_from_db(): new_node: malloc() failed\n"
            );
            sqlite3_finalize(stmt);
            // Free any already allocated nodes
            while (head)
            {
                Inventory *temp = head;
                head = head->next;
                free(temp->item->name);
                free(temp->item->description);
                free(temp->item);
                free(temp);
            }
            return NULL;
        }

        new_node->item = malloc(sizeof(Item));
        if (!new_node->item)
        {
            fprintf
            (
                stderr,
                "ERROR: database.c: get_inventory_from_db(): new_node->item: malloc() failed\n"
            );
            free(new_node);
            sqlite3_finalize(stmt);
            return NULL;
        }

        new_node->item->type = sqlite3_column_int(stmt, 1) - 1;
        new_node->item->name = strdup((char *)sqlite3_column_text(stmt, 2));
        new_node->item->description = strdup((char *)sqlite3_column_text(stmt, 3));
        new_node->item->value = (unsigned short)sqlite3_column_int(stmt, 4);
        new_node->item->price = (unsigned short)sqlite3_column_int(stmt, 5);
        new_node->next = NULL;

        if (!head) head = new_node;
        else current->next = new_node;

        current = new_node;
    }

    sqlite3_finalize(stmt);
    return head;
}

Inventory* get_spells_from_db(sqlite3 *db)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM spells;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf
            (
                stderr,
                "ERROR: database.c: get_inventory_from_db(): sqlite3_prepare_v2(): %s\n",
                sqlite3_errmsg(db)
            );
        return NULL;
    }

    Inventory *head = NULL;
    Inventory *current = NULL;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Inventory *new_node = malloc(sizeof(Inventory));
        if (!new_node)
        {
            fprintf
                (
                    stderr,
                    "ERROR: database.c: get_inventory_from_db(): new_node: malloc() failed\n"
                );
            sqlite3_finalize(stmt);
            // Free any already allocated nodes
            while (head)
            {
                Inventory *temp = head;
                head = head->next;
                free(temp->item->name);
                free(temp->item->description);
                free(temp->item);
                free(temp);
            }
            return NULL;
        }

        new_node->item = malloc(sizeof(Item));
        if (!new_node->item)
        {
            fprintf
                (
                    stderr,
                    "ERROR: database.c: get_inventory_from_db(): new_node->item: malloc() failed\n"
                );
            free(new_node);
            sqlite3_finalize(stmt);
            return NULL;
        }

        new_node->item->type = sqlite3_column_int(stmt, 1) - 1;
        new_node->item->name = strdup((char *)sqlite3_column_text(stmt, 2));
        new_node->item->description = strdup((char *)sqlite3_column_text(stmt, 3));
        new_node->item->value = (unsigned short)sqlite3_column_int(stmt, 4);
        new_node->item->price = (unsigned short)sqlite3_column_int(stmt, 5);
        new_node->next = NULL;

        if (!head) head = new_node;
        else current->next = new_node;

        current = new_node;
    }

    sqlite3_finalize(stmt);
    return head;
}

Character *get_character_from_db(sqlite3 *db)
{
    Character *character = malloc(sizeof *character);
    sqlite3_stmt *result;

    const char *select_character_stmt =
        "SELECT id, name, health, max_health, mana, max_mana, weapon_id, "
        "armor_id, gold, xp, xp_to_next_level FROM characters;";

    int return_value = sqlite3_prepare_v2
    (
        db,
        select_character_stmt,
        -1,
        &result,
        0
    );
    if (return_value != SQLITE_OK)
    {
        fprintf
        (
            stderr,
            "ERROR: database.c: get_character_from_db(): "
            "sqlite3_prepare_v2(): %s\n",
            sqlite3_errmsg(db)
        );
        exit(EXIT_FAILURE);
    }

    if (sqlite3_step(result) == SQLITE_ROW)
    {
        character->name = strdup(
            (const char *) sqlite3_column_text(result, 1));
        character->health = sqlite3_column_int(result, 2);
        character->max_health = sqlite3_column_int(result, 3);
        character->mana = sqlite3_column_int(result, 4);
        character->max_mana = sqlite3_column_int(result, 5);
        character->gold = sqlite3_column_int(result, 8);
        character->xp = sqlite3_column_int(result, 9);
        character->xp_to_next_level = sqlite3_column_int(result, 10);

        int weapon_id = sqlite3_column_int(result, 6);
        character->weapon = get_equipped_item_from_db(db, weapon_id);

        int armor_id = sqlite3_column_int(result, 7);
        character->armor = get_equipped_item_from_db(db, armor_id);

        character->inventory = get_inventory_from_db(db);

        character->spells = get_spells_from_db(db);
    } else memset(character, 0, sizeof *character);

    sqlite3_finalize(result);

    return character;
}

Character *load_game()
{
    sqlite3 *db = open_database("doomdepths.db");

    Character *player = get_character_from_db(db);
    sqlite3_close(db);

    return player;
}

int insert_weapon(sqlite3 *db, Item *weapon)
{
    sqlite3_stmt *stmt_insert_weapon;
    const char *sql_insert_weapon =
        "INSERT INTO inventory (type_id, name, description, value, price)"
        "VALUES (?, ?, ?, ?, ?);";

    int return_value = sqlite3_prepare_v2
    (
        db,
        sql_insert_weapon,
        -1,
        &stmt_insert_weapon,
        0
    );
    if (return_value !=SQLITE_OK)
    {
        fprintf
        (
            stderr,
            "ERROR: database.c: insert_weapon(): sqlite3_prepare_v2(): %s\n",
            sqlite3_errmsg(db)
        );
        exit(EXIT_FAILURE);
    }

    sqlite3_bind_int(stmt_insert_weapon, 1, (int) weapon->type + 1);
    sqlite3_bind_text(stmt_insert_weapon, 2, weapon->name, -1, SQLITE_STATIC);
    sqlite3_bind_text
    (
        stmt_insert_weapon,
        3,
        weapon->description,
        -1,
        SQLITE_STATIC
    );
    sqlite3_bind_int(stmt_insert_weapon, 4, weapon->value);
    sqlite3_bind_int(stmt_insert_weapon, 5, weapon->price);

    if (sqlite3_step(stmt_insert_weapon) != SQLITE_DONE)
    {
        fprintf
        (
            stderr,
            "ERROR: database.c: insert_weapon(): sqlite3_step(): %s\n",
            sqlite3_errmsg(db)
        );
        exit(EXIT_FAILURE);
    }

    int weapon_id = (int) sqlite3_last_insert_rowid(db);

    sqlite3_finalize(stmt_insert_weapon);

    return weapon_id;
}

int insert_armor(sqlite3 *db, Item *armor)
{
    sqlite3_stmt *stmt_insert_armor;
    const char *sql_insert_armor =
        "INSERT INTO inventory (type_id, name, description, value, price)"
        "VALUES (?, ?, ?, ?, ?);";

    int return_value = sqlite3_prepare_v2
    (
        db,
        sql_insert_armor,
        -1,
        &stmt_insert_armor,
        0
    );
    if (return_value != SQLITE_OK)
    {
        fprintf
        (
            stderr,
            "ERROR: database.c: insert_armor(): sqlite3_prepare_v2(): %s\n",
            sqlite3_errmsg(db)
        );
        exit(EXIT_FAILURE);
    }

    sqlite3_bind_int(stmt_insert_armor, 1, (int) armor->type + 1);
    sqlite3_bind_text(stmt_insert_armor, 2, armor->name, -1, SQLITE_STATIC);
    sqlite3_bind_text
    (
        stmt_insert_armor,
        3,
        armor->description,
        -1,
        SQLITE_STATIC
    );
    sqlite3_bind_int(stmt_insert_armor, 4, armor->value);
    sqlite3_bind_int(stmt_insert_armor, 5, armor->price);

    if (sqlite3_step(stmt_insert_armor) != SQLITE_DONE)
    {
        fprintf
        (
            stderr,
            "ERROR: database.c: insert_armor(): sqlite3_step(): %s\n",
            sqlite3_errmsg(db)
        );
        exit(EXIT_FAILURE);
    }

    int armor_id = (int) sqlite3_last_insert_rowid(db);

    sqlite3_finalize(stmt_insert_armor);

    return armor_id;
}


void save_game(Character *player)
{
    sqlite3 *db = open_database("doomdepths.db");

    const char *sql_delete_player = "DELETE FROM characters;";
    const char *sql_delete_inventory = "DELETE FROM inventory;";
    const char *sql_delete_spells = "DELETE FROM spells;";

    char *err_msg = 0;

    int return_value = sqlite3_exec
    (
        db,
        sql_delete_player,
        0,
        0,
        &err_msg
    );
    if (return_value != SQLITE_OK)
    {
        fprintf
        (
            stderr,
            "ERROR: database.c: save_game(): sqlite3_exec(): %s\n",
            err_msg
        );
        sqlite3_free(err_msg);
        exit(EXIT_FAILURE);
    }

    return_value = sqlite3_exec
    (
        db,
        sql_delete_inventory,
        0,
        0,
        &err_msg
    );
    if (return_value != SQLITE_OK)
    {
        fprintf
        (
            stderr,
            "ERROR: database.c: save_game(): sqlite3_exec(): %s\n",
            err_msg
        );
        sqlite3_free(err_msg);
        exit(EXIT_FAILURE);
    }

    return_value = sqlite3_exec
    (
        db,
        sql_delete_spells,
        0,
        0,
        &err_msg
    );
    if (return_value != SQLITE_OK)
    {
        fprintf
        (
            stderr,
            "ERROR: database.c: save_game(): sqlite3_exec(): %s\n",
            err_msg
        );
        sqlite3_free(err_msg);
        exit(EXIT_FAILURE);
    }

    sqlite3_stmt *stmt_insert_player;
    const char *sql_insert_player =
        "INSERT INTO characters (name, health, max_health, mana, max_mana, "
        "gold, xp, xp_to_next_level ) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

    return_value = sqlite3_prepare_v2
    (
        db,
        sql_insert_player,
        -1,
        &stmt_insert_player,
        0
    );
    if (return_value != SQLITE_OK)
    {
        fprintf
        (
            stderr,
            "ERROR: database.c: save_game(): sqlite3_prepare_v2(): %s\n",
            sqlite3_errmsg(db)
        );
        exit(EXIT_FAILURE);
    }

    sqlite3_bind_text(stmt_insert_player, 1, player->name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt_insert_player, 2, player->health);
    sqlite3_bind_int(stmt_insert_player, 3, player->max_health);
    sqlite3_bind_int(stmt_insert_player, 4, player->mana);
    sqlite3_bind_int(stmt_insert_player, 5, player->max_mana);
    sqlite3_bind_int(stmt_insert_player, 6, (int) player->gold);
    sqlite3_bind_int(stmt_insert_player, 7, (int) player->xp);
    sqlite3_bind_int(stmt_insert_player, 8, (int) player->xp_to_next_level);

    if (sqlite3_step(stmt_insert_player) != SQLITE_DONE)
    {
        fprintf
        (
            stderr,
            "ERROR: database.c: save_game(): sqlite3_step(): %s\n",
            sqlite3_errmsg(db)
        );
        exit(EXIT_FAILURE);
    }

    sqlite3_finalize(stmt_insert_player);

    if (player->weapon)
    {
        int weapon_id = get_item_id(db, player->weapon->name);
        sqlite3_stmt *stmt_update_weapon;
        const char *sql_update_weapon =
            "UPDATE characters SET weapon_id = ? WHERE id = 1;";
        return_value = sqlite3_prepare_v2
        (
            db,
            sql_update_weapon,
            -1,
            &stmt_update_weapon,
            0
        );
        if (return_value != SQLITE_OK)
        {
            fprintf
            (
                stderr,
                "ERROR: database.c: save_game(): sqlite3_prepare_v2(): %s\n",
                sqlite3_errmsg(db)
            );
            exit(EXIT_FAILURE);
        }

        sqlite3_bind_int(stmt_update_weapon, 1, weapon_id);

        if (sqlite3_step(stmt_update_weapon) != SQLITE_DONE)
        {
            fprintf
            (
                stderr,
                "ERROR: database.c: save_game(): sqlite3_step(): %s\n",
                sqlite3_errmsg(db)
            );
            exit(EXIT_FAILURE);
        }

        sqlite3_finalize(stmt_update_weapon);
    }
    if (player->armor)
    {
        int armor_id = get_item_id(db, player->armor->name);

        sqlite3_stmt *stmt_update_armor;
        const char *sql_update_armor =
            "UPDATE characters SET armor_id = ? WHERE id = 1;";
        return_value = sqlite3_prepare_v2
        (
            db,
            sql_update_armor,
            -1,
            &stmt_update_armor,
            0
        );
        if (return_value != SQLITE_OK)
        {
            fprintf
            (
                stderr,
                "ERROR: database.c: save_game(): sqlite3_prepare_v2(): %s\n",
                sqlite3_errmsg(db)
            );
            exit(EXIT_FAILURE);
        }

        sqlite3_bind_int(stmt_update_armor, 1, armor_id);

        if (sqlite3_step(stmt_update_armor) != SQLITE_DONE)
        {
            fprintf
            (
                stderr,
                "ERROR: database.c: save_game(): sqlite3_step(): %s\n",
                sqlite3_errmsg(db)
            );
            exit(EXIT_FAILURE);
        }

        sqlite3_finalize(stmt_update_armor);
    }

    Inventory *inventory = player->inventory;
    while (inventory)
    {
        const char *sql_insert_inventory =
            "INSERT INTO inventory (type_id, name, description, value, price) "
            "VALUES (?, ?, ?, ?, ?);";

        sqlite3_stmt *stmt_insert_inventory;
        return_value = sqlite3_prepare_v2
        (
            db,
            sql_insert_inventory,
            -1,
            &stmt_insert_inventory,
            0
        );
        if (return_value != SQLITE_OK)
        {
            fprintf
            (
                stderr,
                "ERROR: database.c: save_game(): sqlite3_prepare_v2(): %s\n",
                sqlite3_errmsg(db)
            );
            exit(EXIT_FAILURE);
        }

        sqlite3_bind_int
        (
            stmt_insert_inventory,
            1,
            (int) inventory->item->type + 1
        );
        sqlite3_bind_text
        (
            stmt_insert_inventory,
            2,
            inventory->item->name,
            -1,
            SQLITE_STATIC
        );
        sqlite3_bind_text
        (
            stmt_insert_inventory,
            3,
            inventory->item->description,
            -1,
            SQLITE_STATIC
        );
        sqlite3_bind_int(stmt_insert_inventory, 4, inventory->item->value);
        sqlite3_bind_int(stmt_insert_inventory, 5, inventory->item->price);

        if (sqlite3_step(stmt_insert_inventory) != SQLITE_DONE)
        {
            fprintf
            (
                stderr,
                "ERROR: database.c: save_game(): sqlite3_step(): %s\n",
                sqlite3_errmsg(db)
            );
            exit(EXIT_FAILURE);
        }

        sqlite3_finalize(stmt_insert_inventory);

        inventory = inventory->next;
    }

    Inventory *spells = player->spells;
    while (spells)
    {
        sqlite3_stmt *stmt_insert_spells;
        const char *sql_insert_spells =
            "INSERT INTO spells(type_id, name, description, value, price)"
            "VALUES (?, ?, ?, ?, ?);";

        return_value = sqlite3_prepare_v2
        (
            db,
            sql_insert_spells,
            -1,
            &stmt_insert_spells,
            0
        );
        if (return_value != SQLITE_OK)
        {
            fprintf
            (
                stderr,
                "ERROR: database.c: save_game(): sqlite3_prepare_v2(): %s\n",
                sqlite3_errmsg(db)
            );
            exit(EXIT_FAILURE);
        }

        sqlite3_bind_int(stmt_insert_spells, 1, (int) spells->item->type + 1);
        sqlite3_bind_text
        (
            stmt_insert_spells,
            2,
            spells->item->name,
            -1,
            SQLITE_STATIC
        );
        sqlite3_bind_text
        (
            stmt_insert_spells,
            3,
            spells->item->description,
            -1,
            SQLITE_STATIC
        );
        sqlite3_bind_int(stmt_insert_spells, 4, spells->item->value);
        sqlite3_bind_int(stmt_insert_spells, 5, spells->item->price);

        if (sqlite3_step(stmt_insert_spells) != SQLITE_DONE)
        {
            fprintf
            (
                stderr,
                "ERROR: database.c: save_game(): sqlite3_step(): %s\n",
                sqlite3_errmsg(db)
            );
            exit(EXIT_FAILURE);
        }

        sqlite3_finalize(stmt_insert_spells);

        spells = spells->next;
    }
    
    sqlite3_close(db);
}

unsigned char insert_item_into_inventory(const char *db_path, Item *item)
{
    sqlite3 *db = open_database(db_path);

    sqlite3_stmt *stmt_insert_item_into_inventory;
    const char *sql_insert_item_into_inventory =
        "INSERT INTO inventory (type_id, name, description, value, price) "
        "VALUES (?, ?, ?, ?, ?);";

    int return_value = sqlite3_prepare_v2
    (
        db,
        sql_insert_item_into_inventory,
        -1,
        &stmt_insert_item_into_inventory,
        0
    );
    if (return_value != SQLITE_OK)
    {
        fprintf
        (
            stderr,
            "ERROR: database.c: insert_item_into_inventory(): sqlite3_prepare_v2(): %s\n",
            sqlite3_errmsg(db)
        );
        return EXIT_FAILURE;
    }

    sqlite3_bind_int(stmt_insert_item_into_inventory, 1, (int) item->type + 1);
    sqlite3_bind_text(stmt_insert_item_into_inventory, 2, item->name, -1, SQLITE_STATIC);
    sqlite3_bind_text
    (
        stmt_insert_item_into_inventory,
        3,
        item->description,
        -1,
        SQLITE_STATIC
    );
    sqlite3_bind_int(stmt_insert_item_into_inventory, 4, item->value);
    sqlite3_bind_int(stmt_insert_item_into_inventory, 5, item->price);

    if (sqlite3_step(stmt_insert_item_into_inventory) != SQLITE_DONE)
    {
        fprintf
        (
            stderr,
            "ERROR: database.c: insert_item_into_inventory(): sqlite3_step(): %s\n",
            sqlite3_errmsg(db)
        );
        return EXIT_FAILURE;
    }

    sqlite3_finalize(stmt_insert_item_into_inventory);

    sqlite3_close(db);

    return EXIT_SUCCESS;
}

void insert_item_into_items_list(sqlite3 *db, ItemType type, const char *name, const char *description, unsigned short value, unsigned short price) {
    sqlite3_stmt *stmt;
    const char *sql_insert_item =
        "INSERT INTO items_list (type_id, name, description, value, price) "
        "VALUES (?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql_insert_item, -1, &stmt, 0) != SQLITE_OK)
    {
        fprintf
        (
            stderr,
            "ERROR: database.c: insert_item_into_items_list(): sqlite3_prepare_v2(): %s\n",
            sqlite3_errmsg(db)
        );
        exit(1);
    }

    sqlite3_bind_int(stmt, 1, (int) type + 1);
    sqlite3_bind_text(stmt, 2, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, description, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4,value);
    sqlite3_bind_int(stmt, 5, price);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf
        (
            stderr,
            "ERROR: database.c: insert_item_into_items_list(): sqlite3_step(): %s\n",
            sqlite3_errmsg(db)
        );
        exit(1);
    }

    sqlite3_finalize(stmt);
}

Item* get_random_item_from_database(sqlite3 *db) {
    Item* item = NULL;
    char query[256];
    snprintf(query, sizeof(query), "SELECT type_id, name, description, value, price FROM items_list ORDER BY RANDOM() LIMIT 1;");

    sqlite3_stmt *stmt_item;
    if (sqlite3_prepare_v2(db, query, -1, &stmt_item, 0) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête SELECT item : %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    if (sqlite3_step(stmt_item) == SQLITE_ROW)
    {
        item = malloc(sizeof(Item));

        int type_id = sqlite3_column_int(stmt_item, 0);
        ItemType item_type;
        if (type_id >= 1 && type_id <= 9) item_type = (ItemType) (type_id) - 1;
        else
        {
            fprintf
            (
                stderr,
                "ERROR: database.c: get_item_from_db(): Invalid item type ID: %d\n",
                type_id
            );
            exit(EXIT_FAILURE);
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

void save_map_context(const char *db_path, struct MapContext *mapcontext) {
    
    sqlite3 *db = open_database(db_path);
    const char *delete_from_map_context_stmt = "DELETE FROM map_context;";

    if (sqlite3_exec(db, delete_from_map_context_stmt, 0, 0, 0) != SQLITE_OK)
    {
        fprintf
        (
            stderr,
            "ERROR: database.c: save_map_context(): sqlite3_exec(): %s\n",
            sqlite3_errmsg(db)
        );
        exit(EXIT_FAILURE);
    }

    const char *sql_insert_mapcontext =
        "INSERT INTO map_context (pos_x, pos_y) VALUES (?, ?);";

    sqlite3_stmt *stmt_insert_mapcontext;

    if (sqlite3_prepare_v2(db, sql_insert_mapcontext, -1, &stmt_insert_mapcontext, 0) != SQLITE_OK)
    {
        fprintf
        (
            stderr,
            "ERROR: database.c: save_map_context(): sqlite3_prepare_v2(): %s\n",
            sqlite3_errmsg(db)
        );
        exit(EXIT_FAILURE);
    }
    sqlite3_bind_int(stmt_insert_mapcontext, 1, mapcontext->pos_x);
    sqlite3_bind_int(stmt_insert_mapcontext, 2, mapcontext->pos_y);

    if (sqlite3_step(stmt_insert_mapcontext) != SQLITE_DONE) {
        fprintf(stderr, "ERROR: Failed to insert datas in map_context tabel\n");
        exit(EXIT_FAILURE);
    }

    sqlite3_finalize(stmt_insert_mapcontext);
}

MapContext *get_map_context(const char *db_path) {
    sqlite3 *db = open_database(db_path);
    const char *sql_select_mapcontext = "SELECT pos_x, pos_y FROM map_context;";

    sqlite3_stmt *stmt_select_mapcontext;
    if (sqlite3_prepare_v2(db, sql_select_mapcontext, -1, &stmt_select_mapcontext, 0) != SQLITE_OK) {
        fprintf(stderr, "ERROR: Failed to prepare map_context selection statement\n");
        exit(EXIT_FAILURE);
    }

    if (sqlite3_step(stmt_select_mapcontext) != SQLITE_ROW) {
        fprintf(stderr, "CAAC\n");
        press_any_key_to_continue();
        return NULL;
    }

    MapContext *mapcontext = (MapContext *)malloc(sizeof(MapContext));

    mapcontext->pos_x = sqlite3_column_int(stmt_select_mapcontext, 0);
    mapcontext->pos_y = sqlite3_column_int(stmt_select_mapcontext, 1);
    sqlite3_finalize(stmt_select_mapcontext);


    return mapcontext;
}

Inventory *generate_random_inventory(void)
{
    Inventory *inventory = NULL;

    sqlite3 *db = open_database("doomdepths.db");

    unsigned char number_of_items = rand() % 5;
    for (unsigned char i = 0; i < number_of_items; i++)
    {
        Item *item = get_random_item_from_database(db);
        if (is_spell(item->type))
        {
            free(item);
            i--;
            continue;
        }
        inventory = add_item_to_inventory(inventory, item);
    }

    return inventory;
}

int get_item_id(sqlite3 *db, const char *item_name)
{
    if (!item_name) return -1;

    sqlite3_stmt *stmt;
    const char *sql = "SELECT id FROM items_list WHERE name = ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf
        (
            stderr,
            "ERROR: database.c: get_item_id(): sqlite3_prepare_v2(): %s\n",
            sqlite3_errmsg(db)
        );

        return -1;
    }

    sqlite3_bind_text(stmt, 1, item_name, -1, SQLITE_TRANSIENT);

    int id = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW) id = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    return id;
};
