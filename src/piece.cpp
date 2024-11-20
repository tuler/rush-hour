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
    if (Fixed())
    {
        return false;
    }
    else if (Horizontal())
    {
        return position % 6 > 0;
    }
    else if (Vertical())
    {
        return position / 6 > 0;
    }
    return false;
}

bool Piece::CanMoveForward() const
{
    if (Fixed())
    {
        return false;
    }
    else if (Horizontal())
    {
        return EndPosition() % 6 < (6 - 1);
    }
    else if (Vertical())
    {
        return EndPosition() / 6 < (6 - 1);
    }
    return false;
}

void Piece::Move(int steps)
{
    position += stride * steps;
}

void Piece::Draw(int64_t x0, int64_t y0, int64_t w, int64_t h, uint32_t color, bool selected) const
{
    int64_t p = 2;      // padding
    int64_t cw = w / 6; // cell width
    int64_t ch = h / 6; // cell height

    int64_t i0 = position;
    int64_t i1 = i0 + stride * (size - 1);
    int64_t px0 = (i0 % 6) * cw;
    int64_t py0 = (i0 / 6) * ch;
    int64_t px1 = (i1 % 6) * cw;
    int64_t py1 = (i1 / 6) * ch;
    int64_t pw = px1 - px0 + cw - p;
    int64_t ph = py1 - py0 + ch - p;
    riv_draw_rect_fill(x0 + px0, y0 + py0, pw, ph, color);

    if (Fixed())
    {
        int64_t fx0 = x0 + px0 + 2;
        int64_t fy0 = y0 + py0 + 2;
        int64_t fx1 = fx0 + pw - 4;
        int64_t fy1 = fy0 + ph - 4;
        riv_draw_point(fx0, fy0, RUSH_COLOR_BOARD);
        riv_draw_point(fx0, fy1, RUSH_COLOR_BOARD);
        riv_draw_point(fx1, fy0, RUSH_COLOR_BOARD);
        riv_draw_point(fx1, fy1, RUSH_COLOR_BOARD);
    }
    if (selected)
    {
        riv_draw_rect_line(x0 + px0, y0 + py0, pw, ph, RUSH_COLOR_PIECE_OUTLINE);
    }
}
