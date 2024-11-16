#pragma once

#include "config.h"

extern "C"
{
#include "riv.h"
}

class Piece
{
public:
    explicit Piece(char label, int64_t position, int64_t size, int64_t stride);

    char Label() const
    {
        return label;
    }

    int Position() const
    {
        return position;
    }

    int Size() const
    {
        return size;
    }

    int Stride() const
    {
        return stride;
    }

    bool Fixed() const
    {
        return size == 1;
    }

    bool Horizontal() const
    {
        return stride == 1;
    }

    bool Vertical() const
    {
        return stride == RUSH_GRID_SIZE;
    }

    void Move(int steps);

    void Draw(int64_t x0, int64_t y0, int64_t w, int64_t h) const;

private:
    // just a label. primary is A
    char label;

    // 0 to (RUSH_GRID_SIZE * RUSH_GRID_SIZE - 1) (35 on 6x6)
    int64_t position;

    // 2 or 3 (or 1 for wall)
    int64_t size;

    // 1 for horizontal pieces, RUSH_GRID_SIZE for vertical pieces (6 on 6x6)
    int64_t stride;
};
