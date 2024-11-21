#ifndef GAME_H
#define GAME_H

#include "file.h"

struct Game
{
    uint64_t score;
    struct File *file;
};

struct Game game_create(struct File *file);

void game_start(struct Game *game);

void game_write_score(uint64_t score, uint64_t level);

#endif
