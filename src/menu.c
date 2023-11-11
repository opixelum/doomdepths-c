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
        player = load_game();
        MapContext *map_context = malloc(sizeof *map_context);
        map_context->player = player;

        get_map(map_context);
        explore_map(map_context);

        free_character(player);
        free(map_context);

        break;

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
        "    2. Drink potion\n"
        "    3. Open inventory\n"
        "    4. Flee\n"
        "\nPress the number of your choice."
    );

    unsigned char choice = get_valid_digit_no_enter(1, 4, 0);
    clear_lines(7);

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

unsigned char attack_selection_menu(Character *player, Character *monster)
{
    if (!player) return 0;

    printf
    (
        "\nWould you rather use your weapon or cast a spell?\n\n"
        "    1. Weapon attack\n"
        "    2. Spell attack\n"
        "    B. Back\n"
        "\nPress the key of your choice on your keyboard.",
        monster->name
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
    // Ask user for his name
    char *user_name = get_user_name_menu();

    Item *fireball = create_item(ATTACK_SPELL, "Fireball", "Man's not hot", 34, 41);
    Item *freeze = create_item(ATTACK_SPELL, "Freeze", "Ice ice baby", 34, 38);
    Inventory *spells = NULL;
    spells = add_item_to_inventory(spells, fireball);
    spells = add_item_to_inventory(spells, freeze);

    Item *weapon1 = create_item(WEAPON, "Sword", "A sword", 100, 100);
    Item *weapon2 = create_item(WEAPON, "Axe", "An axe", 120, 120);
    Item *armor = create_item(ARMOR, "Armor", "An armor", 100, 100);
    Item *health_potion = create_item(HEALTH_POTION, "Chug Jug", "Heals 50 HP", 50, 50);
    Item *mana_potion = create_item(MANA_POTION, "Blue elixir", "Restores 50 MP", 50, 50);
    Inventory *inventory = NULL;
    inventory = add_item_to_inventory(inventory, health_potion);
    inventory = add_item_to_inventory(inventory, mana_potion);
    inventory = add_item_to_inventory(inventory, weapon2);
    inventory = add_item_to_inventory(inventory, armor);

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
        weapon1,
        NULL,
        spells,
        inventory
    );

    create_tables("doomdepths.db");
    save_game(player);

    initialize_map();
    MapContext *map_context = malloc(sizeof *map_context);
    map_context->player = player;
    get_map(map_context);

    printf("\nWelcome %s!\n\n", player->name);
    press_any_key_to_continue();

    explore_map(map_context);
}

Item *item_selection_menu
(
    Character *character,
    ItemType item_type,
    unsigned char inventory_menu
) {
    if (!character)
    {
        fprintf
        (
            stderr,
            "ERROR: menus.c: item_selection_menu: `character` is NULL\n"
        );
        exit(EXIT_FAILURE);
    };

    Inventory *inventory = is_spell(item_type) ?
        character->spells : character->inventory;

    unsigned char items_count = number_of_items(inventory, item_type);

    const char *item_type_string = item_type_to_string(item_type);
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
    else printf("Select an item to use, equip or drop.\n\n");

    Item *items[items_count];
    for (unsigned char i = 0; i < items_count; i++)
    {
        if
        (
            inventory->item->type == item_type
            || item_type == ITEM
            || is_spell(inventory->item->type)
            || is_potion(inventory->item->type)
        ) {
            unsigned int hexcolor;

            if (is_spell(inventory->item->type))
                hexcolor = inventory->item->price > character->mana ?
                    0xff0000 : 0xffffff;
            else hexcolor = 0xffffff;

            if (inventory == character->spells)
                color_printf
                (
                    hexcolor,
                    "    %d. %s\n",
                    i + 1,
                    inventory->item->name
                );
            else
            {
                printf("    %d. ", i + 1);
                print_item_details
                (
                    character,
                    inventory->item,
                    is_potion(inventory->item->type) ? 1 : inventory_menu,
                    0,
                    1,
                    is_spell(inventory->item->type) ? 1 : 0
                );
                printf("\n");
            }

            items[i] = inventory->item;
        }
    inventory = inventory->next;
    }

    printf("\nPress the key of your choice or [B] to go back.");

    unsigned char choice;
    do // Don't allow user to select a spell if he doesn't have enough mana
    {
        choice = get_valid_digit_no_enter(1, items_count, 1);
        if (choice == 'b' || choice == 'B')
        {
            clear_lines(items_count + 4);
            return NULL;
        }
    }
    while (is_spell(item_type) && items[choice - 1]->price > character->mana);

    clear_lines(items_count + 3);

    // -1 because array starts at 0
    return items[choice - 1];
}

void print_attack_result
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

    if (defender->health <= 0) printf("You killed %s!\n\n", defender->name);
    else printf
    (
        "%s dealt %d damage to you.\n\n",
        defender->name,
        damage_taken
    );

    if (player->health <= 0) printf("You died!\n\n");
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

    unsigned char item_count = number_of_items(player->inventory, ITEM);

    unsigned int hex_color;
    if (item_count >= 20) hex_color = 0xffff00;
    else if (item_count == MAX_INVENTORY_SIZE) hex_color = 0xff0000;
    else hex_color = 0xffffff;

    printf("Inventory ");
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

    Item *selected_item = item_selection_menu(player, ITEM, 1);
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
