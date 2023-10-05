# Doomdepths C

Simple doomdepths-like game in C.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md).

## Level system

- Starting level: **1** with **0** xp (1000 xp to level up to level 2));
- Maximum level: 100;
- Level formula: `xp_level_n = xp_level_(n-1) + xp_level_(n-1) * 0.1`;
- When level up, every stat is increased by **10%**;
- When killing a monster, the player receives **10%** of the monster's xp;
