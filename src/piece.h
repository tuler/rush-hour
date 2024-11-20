#pragma once

extern "C"
{
#include "riv.h"
}

class Piece
{
public:
    explicit Piece(uint64_t position, uint64_t size, uint64_t stride);

    uint64_t Position() const
    {
        return position;
    }

    uint64_t EndPosition() const
    {
        return position + stride * (size - 1);
    }

    uint64_t Size() const
    {
        return size;
    }

    uint64_t Stride() const
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
        return stride == 6;
    }

    // check if piece can move backward (based on its position and board boundaries only)
    bool CanMoveBackward() const;

    // check if piece can move forward (based on its position and board boundaries only)
    bool CanMoveForward() const;

    void Move(int steps);

    void Draw(int64_t x0, int64_t y0, int64_t w, int64_t h, uint32_t color, bool selected) const;

private:
    // 0 to (6 * 6 - 1)
    uint64_t position;

    // 2 or 3 (or 1 for wall)
    uint64_t size;

    // 1 for horizontal pieces, 6 for vertical pieces
    uint64_t stride;
};
