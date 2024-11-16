#pragma once
#include <vector>
#include "piece.h"

extern "C"
{
#include "riv.h"
}

class Board
{
public:
    explicit Board(std::string desc);

    void Draw(int64_t x0, int64_t y0, int64_t w, int64_t h) const;

    bool Solved() const;

private:
    std::vector<Piece> pieces;
};