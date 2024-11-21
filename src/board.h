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

    // selected piece
    uint64_t selected;
};

struct Board board_create(uint64_t index, const char *desc, uint64_t moves);

int board_select_next(struct Board *board);

int board_select_previous(struct Board *board);

int board_move_selected_backward(struct Board *board);

int board_move_selected_forward(struct Board *board);

int board_is_solved(struct Board *board);

#endif
