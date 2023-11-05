#include "fight.h"

unsigned short attack(Character *attacker, Character *defender, Item *spell)
{
    if (!attacker || !defender) return 0;
    if (defender->health == 0) return 0;

    unsigned short damage;

    if (spell && spell->type == ATTACK_SPELL) damage = spell->value;
    else
    {
        if (!attacker->weapon)
        {
            if (strcmp(attacker->name, "Orc") == 0) damage = 20;
            else if (strcmp(attacker->name, "Troll") == 0) damage = 30;
            else if (strcmp(attacker->name, "Ogre") == 0) damage = 40;
            else if (strcmp(attacker->name, "Giant") == 0) damage = 50;
            else if (strcmp(attacker->name, "Dragon") == 0) damage = 70;
            else damage = 5;
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

unsigned char battle(Character *player)
{
    if (!player)
    {
        fprintf(stderr, "Error: battle(): player: NULL pointer\n");
        exit(EXIT_FAILURE);
    }

    Monsters *head = generate_random_monsters_list();
    Character *monster;

    // Get number of monsters in the list
    unsigned char number_of_monsters = 0;
    Monsters *current = head;
    while (current)
    {
        number_of_monsters++;
        current = current->next;
    }

    while (head)
    {
        clear_screen();
        print_character_stats(player);
        print_monsters(head);

        switch (battle_actions_menu(player, head))
        {
        case 1:
            // If there is only one monster, select it automatically
            if (number_of_monsters == 1) monster = head->monster;
            else monster = monster_selection_menu(player, head);

            // If player has no attack spell, don't ask him to choose
            if (!number_of_attack_spells(player))
                printf
                (
                    "\n\nYou dealt %d damage to %s using your %s.\n",
                    attack(player, monster, NULL),
                    monster->name,
                    player->weapon ? player->weapon->name : "fists"
                );
            else
                switch (attack_selection_menu(player, monster))
                {
                case 1:
                    printf
                    (
                        "\n\nYou dealt %d damage to %s using your %s.\n",
                        attack(player, monster, NULL),
                        monster->name,
                        player->weapon ? player->weapon->name : "fists"
                    );
                    break;

                case 2:
                    // If there's only one attack spell, select it automatically
                    if (number_of_type_spells(player, ATTACK_SPELL) == 1)
                    {
                        Inventory *attack_spells = get_type_spells(player, ATTACK_SPELL);
                        Item *spell = attack_spells->item;
                        free_inventory(attack_spells);
                        printf
                        (
                            "\n\nYou dealt %d damage to %s by casting %s spell.\n",
                            attack
                            (
                                player,
                                monster,
                                spell
                            ),
                            monster->name,
                            spell->name
                        );
                    }
                    else
                    {
                        Item *spell = type_spell_selection_menu
                        (
                            player,
                            monster,
                            ATTACK_SPELL
                        );
                        printf
                        (
                            "\n\nYou dealt %d damage to %s by casting %s spell.\n",
                            attack
                            (
                                player,
                                monster,
                                spell
                            ),
                            monster->name,
                            spell->name
                        );
                    }
                    break;
                }
            break;

        case 2:
            // TODO: Implement drink potion
            printf("\nDrink potion to implement\n");
            press_any_key_to_continue();
            break;

        case 3:
            free_monsters_list(head);
            printf("\nYou fled!\n");
            return 0;
        }

        if (monster->health > 0) printf
        (
            "%s dealt %d damage to you.\n\n",
            monster->name,
            attack(monster, player, NULL)
        );
        else
        {
            printf("You killed %s!\n\n", monster->name);
            // Free dead monster & target next monster if any
            head = update_monsters_list(head);
            monster = head ? head->monster : NULL;
            number_of_monsters--;
        }

        if (player->health == 0)
        {
            free_monsters_list(head);
            return 0;
        }

        press_any_key_to_continue();
    }

    return 1;
}

void random_battle_trigger(Character *player)
{
    srand(time(NULL));

    if (rand() % 100 < 10)
    {
        clear_screen();
        printf("You're in trouble! You must fight!\n\n");
        press_enter_to_continue();
        battle(player);
    }
}
