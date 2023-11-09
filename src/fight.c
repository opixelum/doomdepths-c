#include "fight.h"

unsigned short attack(Character *attacker, Character *defender, Item *spell)
{
    if (!attacker || !defender) return 0;
    if (defender->health == 0) return 0;

    unsigned short damage;

    if (spell && spell->type == ATTACK_SPELL)
    {
        attacker->mana -= spell->price;
        if (attacker->mana < 0) attacker->mana = 0;
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
    }

    if (defender->armor) damage -= defender->armor->value;
    if (damage > 0)
    {
        if (defender->health < damage) defender->health = 0;
        else defender->health -= damage;
    }

    return damage;
}

Monsters *update_monsters_list(Monsters *head)
{
    if (!head) return NULL;

    Monsters *node = head;
    Monsters *prev = NULL;

    while (node)
    {
        if (node->monster->health == 0)
        {
            if (prev) prev->next = node->next;
            else head = node->next;

            Monsters *tmp = node;
            node = node->next;
            free_character(tmp->monster);
            free(tmp);
        }
        else
        {
            prev = node;
            node = node->next;
        }
    }

    return head;
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
        unsigned char monster_count = get_number_of_monsters(monsters);
        clear_screen();
        print_character_stats(player);
        print_monsters(monsters);

        unsigned char action_choice = battle_actions_menu(player, monsters);

        Character *target_monster = monster_count == 1 ? monsters->monster : NULL;
        Item *potion;

        switch (action_choice)
        {
        case 1:
            target_monster = target_monster ?: monster_selection_menu(player, monsters);
            if (!target_monster) continue; // Case when user goes back
            monsters = perform_attack(player, target_monster, monsters);
            break;

        case 2:
            potion = item_selection_menu(player, POTION);
            if (!potion) continue; // Case when user goes back
            drink_potion(player, potion);
            break;

        case 3:
            printf("\nYou fled!\n\n");
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
        choice = attack_selection_menu(attacker, defender);

        if (choice == 'B' || choice == 'b') return monsters;
        else if (choice == 1) break;

        spell = item_selection_menu
        (
            attacker,
            ATTACK_SPELL
        );
    }
    while (choice == 2 && !spell);

    damage_dealt = attack(attacker, defender, spell);

    unsigned char is_defender_dead = defender->health <= 0;

    if (!is_defender_dead)
        damage_taken = attack(defender, attacker, NULL);

    print_attack_result(attacker, defender, monsters, damage_dealt, damage_taken, spell);
    restore_mana(attacker, 10);
    press_any_key_to_continue();

    return update_monsters_list(monsters);
}
