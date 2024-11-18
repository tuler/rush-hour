#pragma once

extern "C"
{
#include "riv.h"
}

class Health
{
public:
    Health(uint64_t border_color, uint64_t fill_color);

    void Draw(uint64_t x0, uint64_t y0, uint64_t width, uint64_t height, float percent);

private:
    uint64_t border_color;
    uint64_t fill_color;
};
