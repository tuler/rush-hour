#pragma once

#include "board.h"
#include "file.h"

extern "C"
{
#include "riv.h"
}

class Game
{
public:
    Game(File &file);

    void Start();

    uint64_t Score() const;

private:
    uint64_t score;

    File &file;

    void Title();

    uint64_t Play(Board &board);

    void Transition(Board &current, Board &next, uint64_t old_score, uint64_t new_score);

    void InitialTransition(Board &next);

    void GameOver(Board &board);
};
