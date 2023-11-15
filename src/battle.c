#include "battle.h"

unsigned short attack(Character *attacker, Character *defender, Item *spell)
{
    if (!attacker || !defender) return 0;
    if (spell && spell->type != ATTACK_SPELL)
    {
        fprintf(stderr, "Error: attack(): spell: invalid item type\n");
        exit(EXIT_FAILURE);
    }
    if (defender->health == 0) return 0;

    unsigned short damage;

    if (spell)
    {
        cast_spell(attacker, defender, spell);
        damage = spell->value;
    }
    else
    {
        if (!attacker->weapon)
        {
            if (strcmp(attacker->name, "Ghost") == 0) damage = 5;
            else if (strcmp(attacker->name, "Skeleton") == 0) damage = 20;
            else if (strcmp(attacker->name, "Centaur") == 0) damage = 40;
            else if (strcmp(attacker->name, "Grim Reaper") == 0) damage = 50;
            else if (strcmp(attacker->name, "Dragon") == 0) damage = 70;
            else damage = 10;
        }
        else damage = attacker->weapon->value;

        if (defender->armor)
        {
            if (defender->armor->value > damage) damage = 0;
            else damage -= defender->armor->value;
        }

        if (defender->health < damage) defender->health = 0;
        else defender->health -= damage;
    }

    return damage;
}

void battle(Character *player)
{
    if (!player)
    {
        fprintf(stderr, "Error: battle(): player: NULL pointer\n");
        exit(EXIT_FAILURE);
    }

    Monsters *monsters = generate_random_monsters_list();

    while (monsters)
    {
        clear_screen();
        print_character_stats(player);
        printf("\n");
        print_monsters(monsters, NULL);

        unsigned char action_choice = battle_actions_menu(player, monsters);

        Character *targeted_monster;
        Item *item;

        switch (action_choice)
        {
        case 1:
            targeted_monster = monster_selection_menu(player, monsters);
            if (!targeted_monster) continue; // Case when user goes back

            // Update monsters on screen
            clear_lines(22);
            print_monsters(monsters, targeted_monster);

            monsters = perform_attack(player, targeted_monster, monsters);
            if (player->health <= 0) return;
            break;

        case 2:
            item = item_selection_menu(player, player, HEAL_SPELL, 0, 0);
            if (!item) continue; // Case when user goes back
            cast_spell(player, player, item);
            break;

        case 3:
            item = item_selection_menu(player, player, POTION, 0, 0);
            if (!item) continue; // Case when user goes back
            drink_potion(player, item);
            break;

        case 4:
            inventory_menu(player);
            break;

        case 5:
            monsters = flee(monsters, player);
            if (!monsters) return;
            break;
        }
        save_game(player);
    }

    player->health = player->max_health;
    player->mana = player->max_mana;
}

void random_battle_trigger(Character *player)
{
    srand(time(NULL));

    if (rand() % 100 < 30)
    {
        clear_screen();
        printf("You're in trouble! You must fight!\n\n");
        press_enter_to_continue();
        battle(player);
    }
}

Monsters *perform_attack
(
    Character *attacker,
    Character *defender,
    Monsters *monsters
) {
    if (!attacker || !defender) return monsters;

    unsigned short damage_dealt, damage_taken = 0;
    Item *spell = NULL;

    unsigned char choice;
    do
    {
        choice = attack_selection_menu(attacker);

        if (choice == 'B' || choice == 'b') return monsters;
        else if (choice == 1) break;

        spell = item_selection_menu
        (
            attacker,
            attacker,
            ATTACK_SPELL,
            0,
            0
        );
    }
    while (choice == 2 && !spell);

    damage_dealt = attack(attacker, defender, spell);

    unsigned char is_defender_dead = defender->health <= 0;

    if (!is_defender_dead)
        damage_taken = attack(defender, attacker, NULL);

    unsigned char has_looted = print_attack_result(attacker, defender, monsters, damage_dealt, damage_taken, spell);

    if (attacker->health <= 0)
    {
        color_printf
        (
            0xff0000,
            "%s killed you... See you up there!\n\n",
            defender->name
        );
        press_enter_to_continue();

        Monsters *current_monster = monsters;
        while (current_monster)
        {
            Monsters *next_monster = current_monster->next;
            free_character(current_monster->monster);
            free(current_monster);
            current_monster = next_monster;
        }

        return NULL;
    }
    restore_mana(attacker, 10);
    if (!has_looted) press_any_key_to_continue();

    return update_monsters_list(monsters);
}

Monsters *flee(Monsters *monsters, Character *player)
{
    int chances_to_flee = rand() % 100 + 1;
    if (chances_to_flee <= 30)
    {
        free(monsters);
        clear_screen();
        printf("You fled!\n\n");
        press_enter_to_continue();
        return NULL;
    }
    else
    {
        unsigned short damage_taken = attack(monsters->monster, player, NULL);

        clear_screen();
        print_character_stats(player);
        printf("\n");
        print_monsters(monsters, monsters->monster);

        printf
        (
            "\nYou failed to flee...\n%s dealt %d damage to you.\n\n",
            monsters->monster->name,
            damage_taken
        );

        if (player->health <= 0)
        {
            color_printf
            (
                0xff0000,
                "%s killed you... See you up there!\n\n",
                monsters->monster->name
            );

            Monsters *current_monster = monsters;
            while (current_monster)
            {
                Monsters *next_monster = current_monster->next;
                free_character(current_monster->monster);
                free(current_monster);
                current_monster = next_monster;
            }

            press_any_key_to_continue();
            return NULL;
        }

        restore_mana(player, 10);
        press_any_key_to_continue();

        return monsters;
    }
}
