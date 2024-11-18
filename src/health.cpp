#include "health.h"

Health::Health(
    uint64_t border_color,
    uint64_t fill_color)
    : border_color(border_color),
      fill_color(fill_color)
{
}

void Health::Draw(uint64_t x0,
                  uint64_t y0,
                  uint64_t width,
                  uint64_t height,
                  float percent)
{
    // draw border
    riv_draw_rect_line(x0, y0, width, height, border_color);

    // draw fill
    uint64_t fill_width = percent * (width - 2);
    riv_draw_rect_fill(x0 + 1, y0 + 1, fill_width, height - 2, fill_color);
}
