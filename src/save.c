#include "save.h"

sqlite3 *open_database(const char *db_path)
{
    sqlite3 *db;

    if (sqlite3_open(db_path, &db) != SQLITE_OK)
    {
        fprintf
        (
            stderr,
            "ERROR: save.c: open_database(): sqlite3_open(): %s\n",
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

        "CREATE TABLE IF NOT EXISTS item_types("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "label VARCHAR(255) NOT NULL);",

        "INSERT INTO item_types (label) "
        "VALUES ('WEAPON'), ('ARMOR'), ('HEALTH_POTION'), "
        "('MANA_POTION'), ('ATTACK_SPELL'), ('HEAL_SPELL');",

        "CREATE TABLE IF NOT EXISTS items("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "type_id INT NOT NULL, "
        "name VARCHAR(255) NOT NULL, "
        "description VARCHAR(255) NOT NULL, "
        "value INT NOT NULL, "
        "price INT NOT NULL);"
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
                "ERROR: save.c: create_tables(): %s\n",
                sqlite_err_msg
            );
            sqlite3_free(sqlite_err_msg);
            sqlite3_close(db);
            exit(1);
        }
    }

    sqlite3_close(db);
}

Item *get_item_from_db(sqlite3 *db, int itemId)
{
    Item *item = NULL;
    char query[256];
    sqlite3_stmt *stmt_item;

    snprintf
    (
        query,
        sizeof(query),
        "SELECT ity.id, name, description, value, price "
        "FROM items inner join item_type ity on ity.id = items.type_id "
        "WHERE items.id = %d;",
        itemId
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
            "ERROR: save.c: get_item_from_db(): sqlite3_prepare_v2(): "
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
        if (type_id > 0 && type_id < 7) item_type = (ItemType) type_id - 1;
        else
        {
            fprintf
            (
                stderr,
                "ERROR: save.c: get_item_from_db(): Invalid item type "
                "ID: %d\n",
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

Inventory *get_inventory_from_db(sqlite3 *db)
{
    Inventory *inventory = NULL;
    sqlite3_stmt *stmt_items;

    const char *sql_select_items =
        "SELECT items.id, type_id, name, description, value, price "
        "FROM items WHERE type_id NOT IN (5, 6) "
        "AND id NOT IN (SELECT weapon_id FROM player) "
        "AND id NOT IN (SELECT armor_id FROM player);";

    int return_value = sqlite3_prepare_v2
    (
        db,
        sql_select_items,
        -1,
        &stmt_items,
        0
    );
    if (return_value != SQLITE_OK)
    {
        fprintf
        (
            stderr,
            "ERROR: save.c: get_inventory_from_db(): "
            "sqlite3_prepare_v2(): %s\n",
            sqlite3_errmsg(db)
        );
        exit(EXIT_FAILURE);
    }

    while (sqlite3_step(stmt_items) == SQLITE_ROW)
    {
        Inventory *itemNode = malloc(sizeof(Inventory));
        ItemType item_type;

        int type_id = sqlite3_column_int(stmt_items, 1);
        if (type_id > 0 && type_id < 7) item_type = (ItemType) type_id - 1;
        else
        {
            fprintf
            (
                stderr,
                "ERROR: save.c: get_inventory_from_db(): Invalid item "
                "type ID: %d\n",
                type_id
            );
            exit(EXIT_FAILURE);
        }

        itemNode->item = malloc(sizeof(Item));
        itemNode->item->type = item_type;
        itemNode->item->name = strdup(
            (const char *) sqlite3_column_text(stmt_items, 2));
        itemNode->item->description = strdup(
            (const char *) sqlite3_column_text(stmt_items, 3));
        itemNode->item->value = sqlite3_column_int(stmt_items, 4);
        itemNode->item->price = sqlite3_column_int(stmt_items, 5);

        itemNode->next = inventory;
        inventory = itemNode;
    }

    sqlite3_finalize(stmt_items);

    return inventory;
}

Inventory* get_spells_from_db(sqlite3 *db)
{
    Inventory *inventory = NULL;

    const char *sql_select_items =
        "SELECT item.id, idType, name, description, value, price "
        "FROM item WHERE idType IN (5, 6) "
        "AND id NOT IN (SELECT id_weapon FROM player) "
        "AND id NOT IN (SELECT id_armor FROM player);";

    sqlite3_stmt *stmt_items;

    int return_value = sqlite3_prepare_v2
    (
        db,
        sql_select_items,
        -1,
        &stmt_items,
        0
    );
    if (return_value != SQLITE_OK)
    {
        fprintf
        (
            stderr,
            "ERROR: save.c: get_spells_from_db(): sqlite3_prepare_v2(): "
            "%s\n",
            sqlite3_errmsg(db)
        );
        exit(1);
    }

    while (sqlite3_step(stmt_items) == SQLITE_ROW)
    {
        Inventory *itemNode = malloc(sizeof(Inventory));
        ItemType item_type;

        int type_id = sqlite3_column_int(stmt_items, 1);
        if (type_id > 0 && type_id < 7) item_type = (ItemType)type_id-1;
        else
        {
            fprintf
            (
                stderr,
                "ERROR: save.c: get_spells_from_db(): Invalid item type "
                "ID: %d\n",
                type_id
            );
            exit(EXIT_FAILURE);
        }

        itemNode->item = malloc(sizeof(Item));
        itemNode->item->type = item_type;
        itemNode->item->name =
            strdup((const char *)sqlite3_column_text(stmt_items, 2));
        itemNode->item->description =
            strdup((const char *)sqlite3_column_text(stmt_items, 3));
        itemNode->item->value = sqlite3_column_int(stmt_items, 4);
        itemNode->item->price = sqlite3_column_int(stmt_items, 5);

        itemNode->next = inventory;
        inventory = itemNode;
    }

    sqlite3_finalize(stmt_items);

    return inventory;
}

Character *get_character_from_db(sqlite3 *db)
{
    Character *player = malloc(sizeof *player);
    sqlite3_stmt *stmt_player;

    const char *sql_select_player =
        "SELECT id, name, health, max_health, mana, max_mana, weapon_id, "
        "armor_id, gold, xp, xp_to_next_level FROM player;";

    int return_value = sqlite3_prepare_v2
    (
        db,
        sql_select_player,
        -1,
        &stmt_player,
        0
    );
    if (return_value != SQLITE_OK)
    {
        fprintf
        (
            stderr,
            "ERROR: save.c: get_character_from_db(): "
            "sqlite3_prepare_v2(): %s\n",
            sqlite3_errmsg(db)
        );
        exit(EXIT_FAILURE);
    }

    if (sqlite3_step(stmt_player) == SQLITE_ROW)
    {
        player->name = strdup(
            (const char *) sqlite3_column_text(stmt_player, 1));
        player->health = sqlite3_column_int(stmt_player, 2);
        player->max_health = sqlite3_column_int(stmt_player, 3);
        player->mana = sqlite3_column_int(stmt_player, 4);
        player->max_mana = sqlite3_column_int(stmt_player, 5);
        player->gold = sqlite3_column_int(stmt_player, 8);
        player->xp = sqlite3_column_int(stmt_player, 9);
        player->xp_to_next_level = sqlite3_column_int(stmt_player, 10);
        int weapon_id = sqlite3_column_int(stmt_player, 6);

        player->weapon = get_item_from_db(db, weapon_id);

        int armor_id = sqlite3_column_int(stmt_player, 7);
        player->armor = get_item_from_db(db, armor_id);

        player->inventory = get_inventory_from_db(db);

        player->spells = get_spells_from_db(db);
    } else memset(player, 0, sizeof *player);

    sqlite3_finalize(stmt_player);

    return player;
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
        "INSERT INTO items (type_id, name, description, value, price)"
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
            "ERROR: save.c: insert_weapon(): sqlite3_prepare_v2(): %s\n",
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
            "ERROR: save.c: insert_weapon(): sqlite3_step(): %s\n",
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
        "INSERT INTO items (type_id, name, description, value, price)"
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
            "ERROR: save.c: insert_armor(): sqlite3_prepare_v2(): %s\n",
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
            "ERROR: save.c: insert_armor(): sqlite3_step(): %s\n",
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

    const char *sql_delete_player = "DELETE FROM player;";
    const char *sql_delete_inventory = "DELETE FROM items;";

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
            "ERROR: save.c: save_game(): sqlite3_exec(): %s\n",
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
            "ERROR: save.c: save_game(): sqlite3_exec(): %s\n",
            err_msg
        );
        sqlite3_free(err_msg);
        exit(EXIT_FAILURE);
    }

    int weapon_id, armor_id;
    if (player->weapon) weapon_id = insert_weapon(db, player->weapon);
    if (player->armor) armor_id = insert_armor(db, player->armor);

    sqlite3_stmt *stmt_insert_player;
    const char *sql_insert_player =
        "INSERT INTO player (name, health, max_health, mana, max_mana, "
        "weapon_id, armor_id, gold, xp, xp_to_next_level ) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

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
            "ERROR: save.c: save_game(): sqlite3_prepare_v2(): %s\n",
            sqlite3_errmsg(db)
        );
        exit(EXIT_FAILURE);
    }

    sqlite3_bind_text(stmt_insert_player, 1, player->name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt_insert_player, 2, player->health);
    sqlite3_bind_int(stmt_insert_player, 3, player->max_health);
    sqlite3_bind_int(stmt_insert_player, 4, player->mana);
    sqlite3_bind_int(stmt_insert_player, 5, player->max_mana);
    sqlite3_bind_int(stmt_insert_player, 6, weapon_id);
    sqlite3_bind_int(stmt_insert_player, 7, armor_id);
    sqlite3_bind_int(stmt_insert_player, 8, (int) player->gold);
    sqlite3_bind_int(stmt_insert_player, 9, (int) player->xp);
    sqlite3_bind_int(stmt_insert_player, 10, (int) player->xp_to_next_level);

    if (sqlite3_step(stmt_insert_player) != SQLITE_DONE)
    {
        fprintf
        (
            stderr,
            "ERROR: save.c: save_game(): sqlite3_step(): %s\n",
            sqlite3_errmsg(db)
        );
        exit(EXIT_FAILURE);
    }

    sqlite3_finalize(stmt_insert_player);

    Inventory *inventory = player->inventory;
    while (inventory)
    {
        const char *sql_insert_inventory =
            "INSERT INTO items (type_id, name, description, value, price) "
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
                "ERROR: save.c: save_game(): sqlite3_prepare_v2(): %s\n",
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
                "ERROR: save.c: save_game(): sqlite3_step(): %s\n",
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
            "INSERT INTO items (type_id, name, description, value, price)"
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
                "ERROR: save.c: save_game(): sqlite3_prepare_v2(): %s\n",
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
                "ERROR: save.c: save_game(): sqlite3_step(): %s\n",
                sqlite3_errmsg(db)
            );
            exit(EXIT_FAILURE);
        }

        sqlite3_finalize(stmt_insert_spells);

        spells = spells->next;
    }

    sqlite3_close(db);
}

unsigned char insert_item(const char *db_path, Item *item)
{
    sqlite3 *db = open_database(db_path);

    sqlite3_stmt *stmt_insert_item;
    const char *sql_insert_item =
        "INSERT INTO items (type_id, name, description, value, price) "
        "VALUES (?, ?, ?, ?, ?);";

    int return_value = sqlite3_prepare_v2
    (
        db,
        sql_insert_item,
        -1,
        &stmt_insert_item,
        0
    );
    if (return_value != SQLITE_OK)
    {
        fprintf
        (
            stderr,
            "ERROR: save.c: insert_item(): sqlite3_prepare_v2(): %s\n",
            sqlite3_errmsg(db)
        );
        return EXIT_FAILURE;
    }

    sqlite3_bind_int(stmt_insert_item, 1, (int) item->type + 1);
    sqlite3_bind_text(stmt_insert_item, 2, item->name, -1, SQLITE_STATIC);
    sqlite3_bind_text
    (
        stmt_insert_item,
        3,
        item->description,
        -1,
        SQLITE_STATIC
    );
    sqlite3_bind_int(stmt_insert_item, 4, item->value);
    sqlite3_bind_int(stmt_insert_item, 5, item->price);

    if (sqlite3_step(stmt_insert_item) != SQLITE_DONE)
    {
        fprintf
        (
            stderr,
            "ERROR: save.c: insert_item(): sqlite3_step(): %s\n",
            sqlite3_errmsg(db)
        );
        return EXIT_FAILURE;
    }

    sqlite3_finalize(stmt_insert_item);

    sqlite3_close(db);

    return EXIT_SUCCESS;
}

unsigned char init_db_items(const char *db_path)
{
    unsigned char return_value;

    // Health potions
    Item *heartbloom_draught = create_item
    (
        HEALTH_POTION,
        "Heartbloom Draught",
        "A draught made from the heartbloom flower. Restores 9999 HP.",
        9999,
        1000
    );
    return_value = insert_item(db_path, heartbloom_draught);
    free(heartbloom_draught);
    if (return_value == EXIT_FAILURE)
    {
        fprintf
        (
            stderr,
            "ERROR: stuff.c: init_db_items(): "
            "insert_item(heartbloom_draught) failed\n"
        );
        return EXIT_FAILURE;
    }

    Item *crimson_elixir_of_regeneration = create_item
    (
        HEALTH_POTION,
        "Crimson Elixir of Regeneration",
        "A crimson elixir that restores 100 HP.",
        100,
        100
    );
    return_value = insert_item(db_path, crimson_elixir_of_regeneration);
    free(crimson_elixir_of_regeneration);
    if (return_value == EXIT_FAILURE)
    {
        fprintf
        (
            stderr,
            "ERROR: stuff.c: init_db_items(): "
            "insert_item(crimson_elixir_of_regeneration) failed\n"
        );
        return EXIT_FAILURE;
    }

    Item *vitality_essence = create_item
    (
        HEALTH_POTION,
        "Vitality's Essence",
        "A green essence that restores 25 HP.",
        25,
        30
    );
    return_value = insert_item(db_path, vitality_essence);
    free(vitality_essence);
    if (return_value == EXIT_FAILURE)
    {
        fprintf
        (
            stderr,
            "ERROR: stuff.c: init_db_items(): "
            "insert_item(vitality_essence) failed\n"
        );
        return EXIT_FAILURE;
    }

    // Mana potions
    Item *mindfrost_decoction = create_item
    (
        MANA_POTION,
        "Mindfrost Decoction",
        "A decoction made from the mindfrost flower. Restores 9999 MP.",
        9999,
        1000
    );
    return_value = insert_item(db_path, mindfrost_decoction);
    free(mindfrost_decoction);
    if (return_value == EXIT_FAILURE)
    {
        fprintf
        (
            stderr,
            "ERROR: stuff.c: init_db_items(): "
            "insert_item(mindfrost_decoction) failed\n"
        );
        return EXIT_FAILURE;
    }

    Item *sorcerer_siphon_brew = create_item
    (
        MANA_POTION,
        "Sorcerer's Siphon Brew",
        "A blue brew that restores 100 MP.",
        100,
        100
    );
    return_value = insert_item(db_path, sorcerer_siphon_brew);
    free(sorcerer_siphon_brew);
    if (return_value == EXIT_FAILURE)
    {
        fprintf
        (
            stderr,
            "ERROR: stuff.c: init_db_items(): "
            "insert_item(sorcerer_siphon_brew) failed\n"
        );
        return EXIT_FAILURE;
    }

    Item *azure_mystique_vial = create_item
    (
        MANA_POTION,
        "Azure Mystique Vial",
        "A blue vial that restores 25 MP.",
        25,
        30
    );
    return_value = insert_item(db_path, azure_mystique_vial);
    free(azure_mystique_vial);
    if (return_value == EXIT_FAILURE)
    {
        fprintf
        (
            stderr,
            "ERROR: stuff.c: init_db_items(): "
            "insert_item(azure_mystique_vial) failed\n"
        );
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
