#include "config.h"
#include "piece.h"

Piece::Piece(char label, int64_t position, int64_t size, int64_t stride) : label(label),
                                                                           position(position),
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
    int64_t p = 3;                   // padding
    int64_t cw = w / RUSH_GRID_SIZE; // cell width
    int64_t ch = h / RUSH_GRID_SIZE; // cell height

    int64_t i0 = position;
    int64_t i1 = i0 + stride * (size - 1);
    int64_t px0 = (i0 % RUSH_GRID_SIZE) * cw;
    int64_t py0 = (i0 / RUSH_GRID_SIZE) * ch;
    int64_t px1 = (i1 % RUSH_GRID_SIZE) * cw;
    int64_t py1 = (i1 / RUSH_GRID_SIZE) * ch;
    int64_t px = px0 + p;
    int64_t py = py0 + p;
    int64_t pw = px1 - px0 + cw - (p * 2) + 1;
    int64_t ph = py1 - py0 + ch - (p * 2) + 1;
    uint32_t color = label == 'A' ? RUSH_COLOR_PRIMARY_PIECE : RUSH_COLOR_PIECE;
    riv_draw_rect_fill(x0 + px, y0 + py, pw, ph, color);
}
