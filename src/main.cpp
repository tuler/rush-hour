#include <stdexcept>
extern "C"
{
#include "riv.h"
}

#include "color.h"
#include "config.h"
#include "file.h"
#include "game.h"

void draw_palette()
{
    for (int i = 0; i < 32; i++)
    {
        riv_draw_rect_fill(i * 8, 0, 8, 8, i);
    }
}

int main(const int argc, const char **argv)
{
    riv->width = SCREEN_WIDTH;
    riv->height = SCREEN_HEIGHT;
    riv->target_fps = TARGET_FPS;

    // customize palette
    color_setup();

    // load levels from file
    if (argc < 2)
    {
        throw std::runtime_error("missing levels file");
    }
    File levels(argv[1]);
    Game game = Game(levels);
    game.Start();
}
