#include <algorithm>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

extern "C"
{
#include "riv.h"
}

#include "board.h"
#include "config.h"
#include "file.h"
#include "piece.h"

int play(Board &board)
{
    do
    {
        if (riv->keys[RIV_GAMEPAD_A1].press ||
            riv->keys[RIV_GAMEPAD_A2].press ||
            riv->keys[RIV_GAMEPAD_L1].press ||
            riv->keys[RIV_GAMEPAD_L2].press ||
            riv->keys[RIV_GAMEPAD_L3].press ||
            riv->keys[RIV_GAMEPAD_SELECT].press)
        {
            board.SelectPrevious();
        }
        if (riv->keys[RIV_GAMEPAD_A3].press ||
            riv->keys[RIV_GAMEPAD_A4].press ||
            riv->keys[RIV_GAMEPAD_R1].press ||
            riv->keys[RIV_GAMEPAD_R2].press ||
            riv->keys[RIV_GAMEPAD_R3].press ||
            riv->keys[RIV_GAMEPAD_START].press)
        {
            board.SelectNext();
        }
        if (riv->keys[RIV_GAMEPAD_UP].press ||
            riv->keys[RIV_GAMEPAD_LEFT].press)
        {
            board.MoveSelectedBackward();
        }
        if (riv->keys[RIV_GAMEPAD_DOWN].press ||
            riv->keys[RIV_GAMEPAD_RIGHT].press)
        {
            board.MoveSelectedForward();
        }

        // clear screen
        riv_clear(RUSH_COLOR_BACKGROUND);

        // draw board
        board.Draw(32, 32, RUSH_GRID_SIZE * 32, RUSH_GRID_SIZE * 32, true, true);

        if (board.Solved())
        {
            return 0;
        }
        // only exit when solved
        // TODO: timeout = gameover

    } while (riv_present());
    return -1;
}

void transition(Board &current, Board &next)
{
    const Piece &piece = current.PrimaryPiece();
    int delta = current.PrimaryPiece().Position() - next.PrimaryPiece().Position();

    // draw transition from one board to the next
    for (int i = 0; i < (int)riv->width; i += 4)
    {
        // clear screen
        riv_clear(RUSH_COLOR_BACKGROUND);

        bool dp = i < ((int)riv->width - delta * 32); // draw primary piece

        // draw board
        current.Draw(32 - i, 32, RUSH_GRID_SIZE * 32, RUSH_GRID_SIZE * 32, false, true);

        // draw next board
        next.Draw(32 + riv->width - i, 32, RUSH_GRID_SIZE * 32, RUSH_GRID_SIZE * 32, !dp, false);

        // draw "fixed" primary piece
        if (dp)
        {
            piece.Draw(32, 32, RUSH_GRID_SIZE * 32, RUSH_GRID_SIZE * 32, true);
        }

        // present
        riv_present();
    }
}

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

    uint64_t l = 0; // level
    int result = 0;
    Board board = Board(levels[l].desc, levels[l].moves);
    do
    {
        result = play(board);
        if (l + 1 >= levels.size())
        {
            // gone through all levels
            break;
        }

        Board next = Board(levels[l + 1].desc, levels[l + 1].moves);
        transition(board, next);
        board = next;
        l++;
    } while (result >= 0);
}
