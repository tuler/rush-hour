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
    explicit Board(uint64_t index, std::string desc, uint64_t moves);

    int PieceAt(uint64_t position) const;

    bool IsOccupied(uint64_t position) const;

    bool CanMoveForward(uint64_t index) const;

    bool CanMoveBackward(uint64_t index) const;

    bool CanMove(uint64_t index) const;

    const Piece &PrimaryPiece() const;

    bool SelectNext();

    bool SelectPrevious();

    bool MoveSelectedBackward();

    bool MoveSelectedForward();

    void Draw(int64_t x0, int64_t y0, int64_t w, int64_t h, bool drawPrimaryPiece, bool drawPieces, uint32_t colorOffset, bool drawEntry) const;

    bool Solved() const;

    uint64_t Moves() const
    {
        return moves;
    }

    uint64_t TimePerMove() const
    {
        // how many miliseconds we give the player per move
        // TODO: maybe make this variable to make it harder as player advances
        return 3000;
    }

    uint64_t MaxTime() const
    {
        return Moves() * TimePerMove();
    }

private:
    // board level
    uint64_t index;

    // minimal number of moves to solve the board
    uint64_t moves;

    // list of pieces on the board (first is the primary)
    std::vector<Piece> pieces;

    // selected piece
    uint64_t selected;
};
