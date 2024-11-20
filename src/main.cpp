#include <stdexcept>
extern "C"
{
#include "riv.h"
}

#include "color.h"
#include "file.h"
#include "game.h"

int main(const int argc, const char **argv)
{
    riv->width = 256;
    riv->height = 256;
    riv->target_fps = 60;

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
