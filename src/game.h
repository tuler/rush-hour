#ifndef GAME_H
#define GAME_H

#include "level.h"

struct Game
{
    uint64_t score;
    struct Levels *levels;
    uint64_t time_per_move;
};

struct Game game_create(struct Levels *levels, uint64_t time_per_move);

void game_start(struct Game *game);

void game_write_score(uint64_t score, uint64_t level);

#endif
