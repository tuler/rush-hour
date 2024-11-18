#include <algorithm>
#include <map>
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>

#include "board.h"
#include "game.h"

Game::Game(File &file) : file(file)
{
}

void Game::Start()
{
    uint64_t l = 0; // level
    int result = 0;
    Board board = Board(file[l].desc, file[l].moves);
    do
    {
        result = Play(board);
        if (l + 1 >= file.size())
        {
            // gone through all levels
            break;
        }

        Board next = Board(file[l + 1].desc, file[l + 1].moves);
        Transition(board, next);
        board = next;
        l++;
    } while (result >= 0);
}

int Game::Play(Board &board)
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

void Game::Transition(Board &current, Board &next)
{
    const Piece &piece = current.PrimaryPiece();
    int delta = current.PrimaryPiece().Position() - next.PrimaryPiece().Position();
    const int STEPS = riv->width / 4;

    // Ease in-out function using sine
    auto ease = [](float x) -> float
    {
        return (1.0f - std::cos(x * M_PI)) / 2.0f;
    };

    // draw transition from one board to the next
    for (int step = 0; step <= STEPS; step++)
    {
        // clear screen
        riv_clear(RUSH_COLOR_BACKGROUND);

        // Calculate smooth progress (0.0 to 1.0)
        float progress = static_cast<float>(step) / STEPS;
        float smoothProgress = ease(progress);

        // Calculate interpolated offset
        int offset = static_cast<int>(smoothProgress * riv->width);
        bool drawPrimary = offset < ((int)riv->width - delta * 32);

        // Draw boards with interpolated positions
        current.Draw(32 - offset, 32, RUSH_GRID_SIZE * 32, RUSH_GRID_SIZE * 32, false, true);
        next.Draw(32 + riv->width - offset, 32, RUSH_GRID_SIZE * 32, RUSH_GRID_SIZE * 32, !drawPrimary, false);

        // Draw fixed primary piece
        if (drawPrimary)
        {
            piece.Draw(32, 32, RUSH_GRID_SIZE * 32, RUSH_GRID_SIZE * 32, true);
        }

        // present
        riv_present();
    }
}
