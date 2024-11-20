#ifndef PIECE_H
#define PIECE_H

#include <riv.h>

struct Piece
{
    // 0 to (6 * 6 - 1)
    uint64_t position;

    // 2 or 3 (or 1 for wall)
    uint64_t size;

    // 1 for horizontal pieces, 6 for vertical pieces
    uint64_t stride;
};

struct Piece piece_create(uint64_t position, uint64_t size, uint64_t stride);

int piece_is_fixed(const struct Piece *p);

uint64_t piece_end_position(const struct Piece *p);

int piece_can_move_backward(const struct Piece *p);

int piece_can_move_forward(const struct Piece *p);

void piece_move(struct Piece *p, int steps);

#endif
