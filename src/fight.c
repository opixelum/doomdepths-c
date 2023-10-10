#include "fight.h"

unsigned short attack(Character *attacker, Character *defender)
{
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
    if (damage > 0) defender->health -= damage;
    if (defender->health < 0) defender->health = 0;

    return damage;
}
