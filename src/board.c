#include <ctype.h>

#include "board.h"
#include "color.h"
#include "piece.h"

struct Board board_create(uint64_t index, const char *desc, uint64_t moves)
{
    struct Board b = {
        .index = index,
        .moves = moves,
        .piece_count = 0,
        .selected = 0};

    int count[36] = {0};   // number of positions of each label
    int positions[36][36]; // positions of each label

    // build a list of positions for each label
    for (int i = 0; desc[i]; i++)
    {
        if (desc[i] == '.' || desc[i] == 'o')
        {
            continue;
        }
        char label = toupper(desc[i]);
        count[label - 'A']++;
        positions[label - 'A'][count[label - 'A'] - 1] = i;
    }

    // create pieces
    for (char label = 'A'; label <= 'Z'; label++)
    {
        if (label == 'X')
        {
            for (int i = 0; i < count[label - 'A']; i++)
            {
                b.pieces[b.piece_count++] = piece_create(positions[label - 'A'][i], 1, 1);
                // riv_printf("wall position %02d\n", positions[label - 'A'][i]);
            }
        }
        else if (count[label - 'A'] > 1)
        {
            int stride = positions[label - 'A'][1] - positions[label - 'A'][0];
            b.pieces[b.piece_count++] = piece_create(positions[label - 'A'][0], count[label - 'A'], stride);
            // riv_printf("piece %c position %02d size %d stride %d\n", label, positions[label - 'A'][0], count[label - 'A'], stride);
        }
        // XXX: validate positions
    }

    return b;
}

int board_piece_at(struct Board *board, uint64_t position)
{
    for (size_t i = 0; i < board->piece_count; i++)
    {
        const struct Piece *piece = &board->pieces[i];
        uint64_t p = piece->position;
        for (uint64_t j = 0; j < piece->size; j++)
        {
            if (p == position)
            {
                return i;
            }
            p += piece->stride;
        }
    }
    return -1;
}

const struct Piece *board_primary_piece(struct Board *board)
{
    return &board->pieces[0];
}

int board_is_occupied(struct Board *board, uint64_t position)
{
    return board_piece_at(board, position) >= 0;
}

int board_can_move_forward(struct Board *board, uint64_t index)
{
    const struct Piece *piece = &board->pieces[index];
    return piece_can_move_forward(piece) && !board_is_occupied(board, piece_end_position(piece) + piece->stride);
}

int board_can_move_backward(struct Board *board, uint64_t index)
{
    const struct Piece *piece = &board->pieces[index];
    return piece_can_move_backward(piece) && !board_is_occupied(board, piece->position - piece->stride);
}

int board_can_move(struct Board *board, uint64_t index)
{
    return board_can_move_backward(board, index) || board_can_move_forward(board, index);
}

int board_select_next(struct Board *board)
{
    uint64_t s = board->selected;
    do
    {
        s = (s + 1) % board->piece_count;
    } while (s != board->selected && !board_can_move(board, s));
    board->selected = s;
    return 1;
}

int board_select_previous(struct Board *board)
{
    uint64_t s = board->selected;
    do
    {
        s = (s - 1 + board->piece_count) % board->piece_count;
    } while (s != board->selected && !board_can_move(board, s));
    board->selected = s;
    return 1;
}

int board_move_selected_backward(struct Board *board)
{
    if (!board_can_move_backward(board, board->selected))
    {
        return 0;
    }
    struct Piece *piece = &board->pieces[board->selected];
    piece_move(piece, -1);
    return 1;
}

int board_move_selected_forward(struct Board *board)
{
    if (!board_can_move_forward(board, board->selected))
    {
        return 0;
    }
    struct Piece *piece = &board->pieces[board->selected];
    piece_move(piece, 1);
    return 1;
}

int board_is_solved(struct Board *board)
{
    if (board->piece_count > 0)
    {
        // primary piece is always the first in the list
        struct Piece *primaryPiece = &board->pieces[0];

        // calculate exit position
        uint64_t exitPosition = (6 * 6 / 2 - 2);

        // check primary piece position
        return primaryPiece->position == exitPosition;
    }
    return false;
}
