#include "fight.h"

unsigned short attack(Character *attacker, Character *defender)
{
    if (!attacker || !defender) return 0;
    if (defender->health == 0) return 0;

    unsigned short damage = 0;

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

    unsigned char result = 1;
    Monsters *head = generate_random_monsters_list();
    Character *monster = head->monster;

    while (monster)
    {
        attack(player, monster);
        if (monster->health > 0) attack(monster, player);
        else
        {
            // Free dead monster & target next monster if any
            head = update_monsters_list(head);
            monster = head ? head->monster : NULL;
        }

        if (player->health == 0)
        {
            result = 0;
            free_monsters_list(head);
            break;
        }
    }

    return result;
}