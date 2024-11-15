#pragma once

extern "C"
{
#include "riv.h"
}

class Piece
{
public:
    explicit Piece(int64_t position, int64_t size, int64_t stride);

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

    void Move(int steps);

    void Draw(int64_t x0, int64_t y0, int64_t w, int64_t h) const;

private:
    int64_t position;
    int64_t size;
    int64_t stride;
};
