#pragma once

#include "board.h"
#include "file.h"
#include "spritesheet.h"

class Game
{
public:
    Game(File &file, SpriteSheet &ss);

    void Start();

private:
    File &file;

    SpriteSheet &ss;

    int Play(Board &board);

    void Transition(Board &current, Board &next);
};
