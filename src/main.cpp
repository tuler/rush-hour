#include <algorithm>
#include <map>
#include <string>
#include <vector>

extern "C"
{
#include "riv.h"
}

#include "board.h"
#include "config.h"
#include "piece.h"

// array of levels, by increasing difficulty
std::string levels[] = {
    "oBBBCCooFoDDooFAAHooFooHooGEEEooGooo", // 05 moves
    "BBCCLoDDoJLoGAAJooGoIEEEHoIKooHFFKoo", // 10 moves
    "FBBCCJFooHIJAAoHIJGDDDxoGooooooooooo", // 10 moves
    "oooooxxoDDJLAAHIJLGoHIEEGooIKoGoFFKo", // 10 moves
    "ooBBBJooooIJAAooIJoCCCDDoEEFFFoGGGHH", // 10 moves
    "EGBBBoEGoooJEGAAIJFCCHIJFooHDDoooooo", // 15 moves
    "BBCCCLDDDoKLAAooKLJoEEFFJoGGGoJoHHII", // 15 moves
    "HBBCCCHDDDKLoIAAKLoIoJEEoooJoMoFFGGM", // 15 moves
    "oBBCCCDDDooKAAoooKEEFFoKoJGGHHoJooII", // 15 moves
    "BBBCCMooooLMAAoKLMIJoKDDIJEEFFooGGHH", // 15 moves
    "BBCCCKoDDDoKoAAJoKooIJEEooIFFFooGGHH", // 15 moves
};

int main()
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

    const Board board = Board(levels[0]);

    do
    {
        // clear screen
        riv_clear(RUSH_COLOR_BACKGROUND);

        // draw board
        board.Draw(32, 32, RUSH_GRID_SIZE * 32, RUSH_GRID_SIZE * 32);

    } while (riv_present());
}
