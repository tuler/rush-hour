#include <riv.h>

#include "color.h"
#include "piece.h"

struct Piece piece_create(uint64_t position, uint64_t size, uint64_t stride)
{
    struct Piece p;
    p.position = position;
    p.size = size;
    p.stride = stride;
    return p;
}

int piece_is_horizontal(const struct Piece *p)
{
    return p->stride == 1;
}

int piece_is_vertical(const struct Piece *p)
{
    return p->stride == 6;
}

int piece_is_fixed(const struct Piece *p)
{
    return p->size == 1;
}

uint64_t piece_end_position(const struct Piece *p)
{
    return p->position + (p->size - 1) * p->stride;
}

int piece_can_move_backward(const struct Piece *p)
{
    if (piece_is_fixed(p))
    {
        return 0;
    }
    else if (piece_is_horizontal(p))
    {
        return p->position % 6 > 0;
    }
    else if (piece_is_vertical(p))
    {
        return p->position / 6 > 0;
    }
    return 0;
}

int piece_can_move_forward(const struct Piece *p)
{
    if (piece_is_fixed(p))
    {
        return 0;
    }
    else if (piece_is_horizontal(p))
    {
        return piece_end_position(p) % 6 < (6 - 1);
    }
    else if (piece_is_vertical(p))
    {
        return piece_end_position(p) / 6 < (6 - 1);
    }
    return 0;
}

void piece_move(struct Piece *p, int steps)
{
    p->position += p->stride * steps;
}
