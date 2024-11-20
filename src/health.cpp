#include "color.h"
#include "health.h"

void health_draw(uint64_t x0, uint64_t y0, uint64_t w, uint64_t h, float percent)
{
    // draw border
    riv_draw_rect_line(x0, y0, w, h, RUSH_COLOR_GRID_LINE);

    // draw fill
    uint64_t fw = percent * (w - 2);
    riv_draw_rect_fill(x0 + 1, y0 + 1, fw, h - 2, RUSH_COLOR_PIECE);
}
