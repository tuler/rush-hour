#pragma once

#include "board.h"
#include "file.h"

class Game
{
public:
    Game(File &file);

    void Start();

private:
    File &file;

    int Play(Board &board);

    void Transition(Board &current, Board &next);
};
