#ifndef BOARD_H
#define BOARD_H

#include <riv.h>
#include <stddef.h>

#include "piece.h"

#define MAX_PIECES 36

struct Board
{
    // board level
    uint64_t index;

    // minimal number of moves to solve the board
    uint64_t moves;

    // list of pieces on the board (first is the primary)
    struct Piece pieces[MAX_PIECES];

    size_t piece_count;

    // moveable pieces
    uint64_t moveable_pieces[MAX_PIECES];

    // number of moveable pieces
    size_t moveable_count;

    // selected piece, index in moveable_pieces array, -1 if none is selected
    int64_t selected;
};

struct Board board_create(uint64_t index, const char *desc, uint64_t moves);

int board_can_move(const struct Board *board, uint64_t index);

int board_select_next(struct Board *board);

int board_move_selected_backward(struct Board *board);

int board_move_selected_forward(struct Board *board);

int board_is_solved(const struct Board *board);

int board_select_diff(const struct Board *board, uint64_t index);

#endif
