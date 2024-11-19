#include "color.h"
#include "piece.h"

Piece::Piece(uint64_t position,
             uint64_t size,
             uint64_t stride)
    : position(position),
      size(size),
      stride(stride)
{
}

bool Piece::CanMoveBackward() const
{
    if (Horizontal())
    {
        return position % RUSH_GRID_SIZE > 0;
    }
    else if (Vertical())
    {
        return position / RUSH_GRID_SIZE > 0;
    }
    return false;
}

bool Piece::CanMoveForward() const
{
    if (Horizontal())
    {
        return EndPosition() % RUSH_GRID_SIZE < (RUSH_GRID_SIZE - 1);
    }
    else if (Vertical())
    {
        return EndPosition() / RUSH_GRID_SIZE < (RUSH_GRID_SIZE - 1);
    }
    return false;
}

void Piece::Move(int steps)
{
    position += stride * steps;
}

void Piece::Draw(int64_t x0, int64_t y0, int64_t w, int64_t h, uint32_t color, bool selected) const
{
    int64_t p = 2;                   // padding
    int64_t cw = w / RUSH_GRID_SIZE; // cell width
    int64_t ch = h / RUSH_GRID_SIZE; // cell height

    int64_t i0 = position;
    int64_t i1 = i0 + stride * (size - 1);
    int64_t px0 = (i0 % RUSH_GRID_SIZE) * cw;
    int64_t py0 = (i0 / RUSH_GRID_SIZE) * ch;
    int64_t px1 = (i1 % RUSH_GRID_SIZE) * cw;
    int64_t py1 = (i1 / RUSH_GRID_SIZE) * ch;
    int64_t pw = px1 - px0 + cw - p;
    int64_t ph = py1 - py0 + ch - p;
    riv_draw_rect_fill(x0 + px0, y0 + py0, pw, ph, color);
    if (selected)
    {
        riv_draw_rect_line(x0 + px0, y0 + py0, pw, ph, RUSH_COLOR_PIECE_OUTLINE);
    }
}
