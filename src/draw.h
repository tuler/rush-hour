#ifndef DRAW_H
#define DRAW_H

#include <riv.h>

#include "board.h"
#include "piece.h"

#define RUSH_DRAW_PRIMARY_PIECE 0x01
#define RUSH_DRAW_PIECES 0x02
#define RUSH_DRAW_WALLS 0x04
#define RUSH_DRAW_ENTRY 0x08
#define RUSH_DRAW_EXIT 0x10
#define RUSH_DRAW_DIM_UNMOVEABLE 0x20
#define RUSH_DRAW_CLICKS 0x40

void draw_palette();

void draw_health(uint64_t x0, uint64_t y0, uint64_t w, uint64_t h, float percent);

void draw_piece(const struct Piece *p, int64_t x0, int64_t y0, int64_t w, int64_t h, uint32_t color, int selected, int can_move, int clicks);

void draw_board(const struct Board *b, int64_t x0, int64_t y0, int64_t w, int64_t h, uint32_t colorOffset, uint16_t flags);

riv_recti draw_score(int64_t x0, int64_t y0, uint64_t score);

#endif
