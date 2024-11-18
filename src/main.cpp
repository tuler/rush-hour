#include <stdexcept>

extern "C"
{
#include "riv.h"
}

#include "board.h"
#include "config.h"
#include "file.h"
#include "game.h"
#include "piece.h"

int main(const int argc, const char **argv)
{
    riv->width = SCREEN_WIDTH;
    riv->height = SCREEN_HEIGHT;
    riv->target_fps = TARGET_FPS;

    // customize palette
    riv->palette[RUSH_COLOR_BACKGROUND] = 0xFFFFFF;
    riv->palette[RUSH_COLOR_BOARD] = 0xCDEAF2;
    riv->palette[RUSH_COLOR_BLOCKED] = 0x606DD9;
    riv->palette[RUSH_COLOR_GRID_LINE] = 0xAAAAAA;
    riv->palette[RUSH_COLOR_PRIMARY_PIECE] = 0x3333CC;
    riv->palette[RUSH_COLOR_PIECE] = 0x998833;
    riv->palette[RUSH_COLOR_PIECE_OUTLINE] = 0x222222;
    riv->palette[RUSH_COLOR_LABEL] = 0x222222;
    riv->palette[RUSH_COLOR_WALL] = 0x222222;

    // load levels from file
    if (argc < 2)
    {
        throw std::runtime_error("missing levels file");
    }
    File levels(argv[1]);
    Game game = Game(levels);
    game.Start();
}
