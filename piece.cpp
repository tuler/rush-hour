#include "piece.h"

Piece::Piece(int64_t position, int64_t size, int64_t stride) : position(position),
                                                               size(size),
                                                               stride(stride)
{
}

void Piece::Move(int steps)
{
    position += stride * steps;
}

void Piece::Draw(int64_t x0, int64_t y0, int64_t w, int64_t h) const
{
    uint64_t i0 = position;
    uint64_t i1 = i0 + stride * (size - 1);
}
