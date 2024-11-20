#include <riv.h>
#define SEQT_IMPL

#include "color.h"
#include "file.h"
#include "game.h"
#include "seqt.h"

int main(const int argc, const char **argv)
{
    riv->width = 256;
    riv->height = 256;
    riv->target_fps = 60;

    // customize palette
    color_setup();

    // initialize music
    seqt_init();

    // load levels from file
    if (argc < 2)
    {
        riv_printf("missing levels file\n");
        return -1;
    }

    // load levels file
    struct File levels;
    file_load(&levels, argv[1]);

    // Game game = Game(levels);
    // game.Start();
    struct Game game = game_create(&levels);
    game_start(&game);

    file_free(&levels);
}
