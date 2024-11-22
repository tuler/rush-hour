#include <getopt.h>
#include <riv.h>
#include <stdlib.h>
#include <stdio.h>

#define SEQT_IMPL

#include "color.h"
#include "game.h"
#include "level.h"
#include "seqt.h"

struct Options
{
    uint64_t time_per_move; // how many miliseconds per move based on puzzle optimal number of moves
    uint64_t level;         // number of the level to start (0-based)
    const char *file;       // levels file to load
};

static struct option long_options[] = {
    {"time-per-move", required_argument, 0, 't'},
    {"level", required_argument, 0, 'l'},
    {"file", required_argument, 0, 'f'},
    {0, 0, 0, 0}};

struct Options parse_args(int argc, char **argv)
{
    struct Options opts = {
        .time_per_move = 3000, // default 3000ms
        .level = 0,
        .file = "levels.txt"};

    int opt;
    while ((opt = getopt_long(argc, (char *const *)argv, "t:l:f:", long_options, NULL)) != -1)
    {
        switch (opt)
        {
        case 't':
            opts.time_per_move = atoi(optarg);
            break;
        case 'l':
            opts.level = atoi(optarg);
            break;
        case 'f':
            opts.file = optarg;
            break;
        default:
            fprintf(stderr, "Usage: %s [--time-per-move ms] [--level n] [--file path]\n", argv[0]);
            exit(1);
        }
    }
    return opts;
}

int main(int argc, char *argv[])
{
    riv->width = 256;
    riv->height = 256;
    riv->target_fps = 60;

    // customize palette
    color_setup();

    // initialize music
    seqt_init();

    // parse options
    struct Options opts = parse_args(argc, argv);

    // load levels file
    struct Levels levels;
    if (riv->incard_len > 0)
    {
        level_load_incard(&levels, opts.level);
    }
    else
    {
        level_load_file(&levels, opts.file, opts.level);
    }

    // create and start the game
    struct Game game = game_create(&levels, opts.time_per_move);
    game_start(&game);

    // free up dynamic allocated resources
    level_free(&levels);
}
