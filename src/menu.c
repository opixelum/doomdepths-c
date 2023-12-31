#include "menu.h"

void main_menu(unsigned char *is_running)
{
    clear_screen();
    printf
    (
        "Welcome to Doomdepths C! What do you want to do?\n\n"
        "    1. Start a new game\n"
        "    2. Continue my last game\n"
        "    3. Quit\n"
        "\nPress the number of your choice on your keyboard."
    );

    Character *player;

    switch (get_valid_digit_no_enter(1, 3, 0))
    {
    case 1:
        new_game();
        break;

    case 2:
    {
        FILE *file = fopen("doomdepths.db", "r+");
 
        if (!file)
        {
            clear_screen();
            printf("There's no saved game. Start a new one!\n\n");
            press_any_key_to_continue();
        }
        else
        {
            fclose(file);
            player = load_game();
            MapContext *map_context = get_map_context("doomdepths.db");
            if (!map_context) map_context = malloc(sizeof *map_context);
            if (!map_context)
            {
                fprintf
                (
                    stderr,
                    "ERROR: menus.c: main_menu(): malloc failed\n"
                );
                exit(EXIT_FAILURE);
            }

            map_context->player = player;
            get_map(map_context);
            explore_map(map_context);

            free_character(player);
            free(map_context);
        }
        break;
    }

    case 3:
        clear_screen();
        *is_running = 0;
        break;
    }
}

unsigned char battle_actions_menu(Character *player, Monsters *head)
{
    if (!player || !head) return 0;

    printf
    (
        "\nWhat do you want to do now?\n\n"
        "    1. Attack\n"
        "    2. Cast healing spell\n"
        "    3. Drink potion\n"
        "    4. Open inventory\n"
        "    5. Flee\n"
        "\nPress the number of your choice."
    );

    unsigned char choice = get_valid_digit_no_enter(1, 5, 0);
    clear_lines(8);

    return choice;
}

Character *monster_selection_menu(Character *character, Monsters *head)
{
    if (!character || !head) return NULL;

    // Skip menu if there is only one monster
    if (!head->next) return head->monster;

    printf("Which monster do you want to attack now?\n\n");

    Character *monsters[3];
    unsigned char number_of_monsters = 0;
    while (head)
    {
        printf("    %d. %s\n", ++number_of_monsters, head->monster->name);
        monsters[number_of_monsters - 1] = head->monster;
        head = head->next;
    }

    printf("    B. Back\n\nPress the number of your choice on your keyboard.");

    unsigned char choice = get_valid_digit_no_enter(1, number_of_monsters, 1);
    clear_lines(number_of_monsters + 4); // +4 for other menu lines

    // -1 because array starts at 0
    return choice == 'B' || choice == 'b' ? NULL : monsters[choice - 1];
}

unsigned char attack_selection_menu(Character *player)
{
    if (!player) return 0;

    printf
    (
        "\nWould you rather use your weapon or cast a spell?\n\n"
        "    1. Weapon attack\n"
        "    2. Spell attack\n"
        "    B. Back\n"
        "\nPress the key of your choice on your keyboard."
    );

    unsigned char choice = get_valid_digit_no_enter(1, 2, 1);
    clear_lines(6);

    return choice;
}

char *get_user_name_menu(void)
{
    clear_screen();
    printf("Before we start, enter your name: ");

    char *user_name = NULL;

    do user_name = get_string();
    while (!strlen(user_name));

    return user_name;
}

void new_game(void)
{
    create_tables("doomdepths.db");

    // Ask user for his name
    char *user_name = get_user_name_menu();

    Item *fireball = create_item(ATTACK_SPELL, "Fireball", "Man's not hot", 41, 41);
    Item *freeze = create_item(ATTACK_SPELL, "Freeze breeze", "Ice ice baby", 34, 34);
    Item *abrasparadra = create_item(HEAL_SPELL, "Abrasparadra", "Heals 25 HP", 25, 10);
    Item *jouvence = create_item(HEAL_SPELL, "Jouvence", "Heals 50 HP", 50, 20);
    Inventory *spells = NULL;
    spells = add_item_to_inventory(spells, fireball);
    spells = add_item_to_inventory(spells, freeze);
    spells = add_item_to_inventory(spells, abrasparadra);
    spells = add_item_to_inventory(spells, jouvence);

    // Create a new character
    Character *player = create_character
    (
        user_name,
        1,
        0,
        1000,
        100,
        100,
        100,
        100,
        0,
        NULL,
        NULL,
        spells,
        NULL
    );

    initialize_map();
    MapContext *map_context = malloc(sizeof *map_context);
    if (!map_context)
    {
        fprintf
        (
            stderr,
            "ERROR: menus.c: new_game(): malloc() failed\n"
        );
        exit(EXIT_FAILURE);
    }
    map_context->player = player;
    get_map(map_context);

    save_game(player);
    save_map_context("doomdepths.db", map_context);

    printf("\nWelcome %s!\n\n", player->name);
    press_any_key_to_continue();

    explore_map(map_context);
}

Item *item_selection_menu
(
    Character *owner,
    Character *receiver,
    ItemType item_type,
    unsigned char inventory_menu,
    unsigned char loot_menu
) {
    if (!owner)
    {
        fprintf
        (
            stderr,
            "ERROR: menus.c: item_selection_menu: `owner` is NULL\n"
        );
        exit(EXIT_FAILURE);
    }

    Inventory *inventory = is_spell(item_type) ?
                           owner->spells : owner->inventory;

    unsigned char items_count = number_of_items_by_type(inventory, item_type);

    const char *item_type_string = item_type_to_string(item_type, 0);
    if (!item_type_string)
    {
        fprintf
        (
            stderr,
            "ERROR: menus.c: item_selection_menu(): item_type_to_string() "
            "failed\n"
        );
        exit(EXIT_FAILURE);
    }

    if (!items_count)
    {
        printf("You don't have any %s.\n\n", item_type_string);
        press_any_key_to_continue();
        return NULL;
    }

    if (is_spell(item_type))
        printf("Which %s do you want to cast?\n\n", item_type_string);
    else if (is_potion(item_type))
        printf("Which %s do you want to drink?\n\n", item_type_string);
    else if (loot_menu)
        printf("Which item do you want to loot?\n\n");
    else printf("Select an item to use, equip or drop.\n\n");

    // Temporary array to hold the count for each item
    ItemCount temp_items[items_count];
    memset(temp_items, 0, sizeof(temp_items)); // Initialize to zero

    unsigned char i = 0;
    while (inventory)
    {
        if
            (
            inventory->item->type == item_type
            || item_type == ITEM
            || item_type == SPELL && is_spell(inventory->item->type)
            || item_type == POTION && is_potion(inventory->item->type)
            )
        {
            // Check if item already exists in temp_items
            int found = 0;
            for (unsigned char j = 0; j < i; ++j)
            {
                if (strcmp(temp_items[j].item->name, inventory->item->name) ==
                    0)
                {
                    temp_items[j].count++;
                    found = 1;
                    break;
                }
            }

            // If item not found, add to tempItems
            if (!found)
            {
                temp_items[i].item = inventory->item;
                temp_items[i].count = 1;
                i++;
            }
        }
        inventory = inventory->next;
    }

    unsigned char j; // Counter for displayed items
    for (j = 0; j < i; j++)
    {
        printf("    %d. ", j + 1);
        if (temp_items[j].count > 1) printf("(%d) ", temp_items[j].count);

        print_item_details
        (
            receiver,
            temp_items[j].item,
            is_potion(temp_items[j].item->type) ? 1 : inventory_menu,
            0,
            1,
            is_spell(temp_items[j].item->type) ? 1 : 0
        );
        printf("\n");
    }

    printf("\nPress the key of your choice or [B] to go back.");

    unsigned char choice;
    do // Don't allow user to select a spell if he doesn't have enough mana
    {
        choice = get_valid_digit_no_enter(1, j, 1);
        if (choice == 'b' || choice == 'B')
        {
            clear_lines(items_count + 4);
            return NULL;
        }
    }
    while (is_spell(item_type) && temp_items[choice - 1].item->price > owner->mana);

    clear_lines(j + 3);

    // -1 because array starts at 0
    return temp_items[choice - 1].item;
}

unsigned char print_attack_result
(
    Character *player,
    Character *defender,
    Monsters *monsters,
    unsigned short damage_dealt,
    unsigned short damage_taken,
    Item *spell
) {
    clear_screen();
    print_character_stats(player);
    printf("\n");
    print_monsters(monsters, defender);

    if (spell) printf
    (
        "\nYou dealt %d damage to %s by casting the %s spell.\n",
        damage_dealt,
        defender->name,
        spell->name
    );
    else printf
    (
        "\nYou dealt %d damage to %s using your %s.\n",
        damage_dealt,
        defender->name,
        player->weapon ? player->weapon->name : "fists"
    );

    if (defender->health <= 0)
    {
        printf("You killed %s!\n\n", defender->name);

        unsigned char is_attacker_inventory_full =
            number_of_items_by_type(player->inventory, ITEM) >= MAX_INVENTORY_SIZE;

        if (defender->inventory && !is_attacker_inventory_full)
        {
            printf
            (
                "%s has some items in his inventory. Want to loot them? [Y/N]",
                defender->name
            );

            if (yes_no_input())
            {
                clear_lines(25);
                loot_character_menu(player, defender);
            }

            return 1;
        }
    }
    else printf
    (
        "%s dealt %d damage to you.\n\n",
        defender->name,
        damage_taken
    );

    return 0;
}

void inventory_menu(Character *player)
{
    clear_screen();

    if (!player)
    {
        fprintf
        (
            stderr,
            "ERROR: menus.c: inventory_menu(): `player` is NULL\n"
        );
        exit(EXIT_FAILURE);
    }

    unsigned char item_count = number_of_items_by_type(player->inventory, ITEM);

    unsigned int hex_color;
    if (item_count == MAX_INVENTORY_SIZE) hex_color = 0xff0000;
    else if (item_count < 25 && item_count >= 20) hex_color = 0xffff00;
    else hex_color = 0xffffff;

    print_character_stats(player);
    printf("\nInventory ");
    color_printf
    (
        hex_color,
        "(%d/%d)\n\n",
        item_count,
        MAX_INVENTORY_SIZE
    );

    if (player->weapon || player->armor)
    {
        printf("Equipped items:\n\n");

        if (player->weapon)
        {
            printf("    Weapon: ");
            print_item_details(player, player->weapon, 0, 0, 1, 0);
            printf("\n");
        }
        else color_printf(0xff0000, "    No weapon\n");

        if (player->armor)
        {
            printf("    Armor ");
            print_item_details(player, player->armor, 0, 0, 1, 0);
            printf("\n");
        }
        else color_printf(0xff0000, "    No armor\n");

        printf("\n");
    }

    Item *selected_item = item_selection_menu(player, player, ITEM, 1, 0);
    if (!selected_item) return;

    printf("What do you want to do with this %s?\n\n", selected_item->name);

    if (is_potion(selected_item->type)) printf("    1. Drink\n");
    else printf("    1. Equip\n");
    printf("    2. Drop\n\nPress the key of your choice or [B] to go back.");

    unsigned char choice = get_valid_digit_no_enter(1, 2, 1);

    if (choice == 1) is_potion(selected_item->type) ?
        drink_potion(player, selected_item) : equip_item(player, selected_item);
    else if (choice == 2)
        player->inventory =
            remove_item_from_inventory(player->inventory, selected_item);

    clear_screen();
    inventory_menu(player);
}

void loot_character_menu(Character *looter, Character *looted)
{
    if (!looter)
    {
        fprintf
        (
            stderr,
            "ERROR: menu.c: loot_character_menu(): `looter` is NULL\n"
        );
        exit(EXIT_FAILURE);
    }

    if (!looted)
    {
        fprintf
        (
            stderr,
            "ERROR: menu.c: loot_character_menu(): `looted` is NULL\n"
        );
        exit(EXIT_FAILURE);
    }

    Item *selected_item = NULL;
    do
    {
        if (!number_of_items_by_type(looted->inventory, ITEM)) return;

        unsigned char is_attacker_inventory_full =
            number_of_items_by_type(looter->inventory, ITEM) >= MAX_INVENTORY_SIZE;

        if (is_attacker_inventory_full)
        {
            printf
            (
                "That was the last item you could take. Now your inventory "
                "is full.\n\n"
            );
            press_any_key_to_continue();
            return;
        }

        printf("%s's inventory. ", looted->name);
        selected_item = item_selection_menu(looted, looter, ITEM, 1, 1);
        if (!selected_item) return;

        looted->inventory = remove_item_from_inventory(looted->inventory, selected_item);
        looter->inventory = add_item_to_inventory(looter->inventory, selected_item);
    }
    while (1);
}
