#ifndef SPELLS_H
#define SPELLS_H

typedef struct Spell
{
    char *name;
    char *description;
    unsigned short value; // Dealing damage if attack spell, reducing damage if defense spell, etc.
    unsigned short mana_cost;
} Spell;

typedef struct Spells {
    Spell *spell;
    struct Spells *next;
} Spells;

#endif // SPELLS_H
