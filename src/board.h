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
    explicit Board(std::string desc, int moves);

    int PieceAt(int64_t position) const;

    bool IsOccupied(int64_t position) const;

    void Draw(int64_t x0, int64_t y0, int64_t w, int64_t h) const;

    bool Solved() const;

private:
    // minimal number of moves to solve the board
    int moves;

    // list of pieces on the board (first is the primary)
    std::vector<Piece> pieces;
};
