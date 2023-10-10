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
