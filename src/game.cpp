#include <algorithm>
#include <map>
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>

#include "board.h"
#include "game.h"
#include "health.h"

Game::Game(File &file) : file(file)
{
    score = 0;
}

void Game::Start()
{
    uint64_t l = 0; // level
    uint64_t result = 0;
    Board board = Board(l, file[l].desc, file[l].moves);
    do
    {
        result = Play(board);
        if (l + 1 >= file.size())
        {
            // gone through all levels
            break;
        }
        if (result == 0)
        {
            // player ran out of time
            break;
        }

        Board next = Board(l + 1, file[l + 1].desc, file[l + 1].moves);
        Transition(board, next, score, score + result);
        board = next;
        score += result;
        l++;
    } while (result > 0);
}

uint64_t Game::Play(Board &board)
{
    // how many miliseconds we give the player per move
    // TODO: maybe make this variable to make it harder as player advances
    uint64_t ms_per_move = 3000;

    // how much time we give the player to complete the level
    uint64_t max_time = ms_per_move * board.Moves();

    uint64_t start_time = riv->time_ms;
    uint64_t timeout = start_time + max_time;

    // health meter
    Health health = Health(RUSH_COLOR_GRID_LINE, RUSH_COLOR_PIECE);

    do
    {
        uint64_t time_ms = riv->time_ms;

        // check if time is over
        if (time_ms >= timeout)
        {
            return 0;
        }
        uint64_t time_left = timeout - time_ms;

        // check if board is solved
        if (board.Solved())
        {
            // return how much time is left (will be added to score)
            return time_left;
        }

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

        // draw score
        riv_recti text_size = riv_draw_text(("Score " + std::to_string(score)).c_str(),
                                            RIV_SPRITESHEET_FONT_5X7, RIV_RIGHT,
                                            256 - 32,
                                            256 - 16,
                                            1,
                                            RIV_COLOR_BLACK);
        // draw timer
        health.Draw(32,
                    256 - 16 - (text_size.height / 2),
                    256 - 64 - text_size.width - 2,
                    text_size.height,
                    (float)time_left / max_time);

    } while (riv_present());
    return 0;
}

void Game::Transition(Board &current, Board &next, uint64_t old_score, uint64_t new_score)
{
    const Piece &piece = current.PrimaryPiece();
    int delta = current.PrimaryPiece().Position() - next.PrimaryPiece().Position();
    const int STEPS = riv->width / 4;

    // Ease in-out function using sine
    auto ease = [](float x) -> float
    {
        return (1.0f - std::cos(x * M_PI)) / 2.0f;
    };

    uint64_t ms_per_move = 3000;

    // how much time we give the player to complete the level
    uint64_t max_time = ms_per_move * current.Moves();

    uint64_t diff_score = new_score - old_score;

    // health meter
    Health health = Health(RUSH_COLOR_GRID_LINE, RUSH_COLOR_PIECE);

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

        // draw score
        uint64_t sc = old_score + (new_score - old_score) * smoothProgress;
        riv_recti text_size = riv_draw_text(("Score " + std::to_string(sc)).c_str(),
                                            RIV_SPRITESHEET_FONT_5X7, RIV_RIGHT,
                                            256 - 32,
                                            256 - 16,
                                            1,
                                            RIV_COLOR_BLACK);
        // draw timer
        health.Draw(32,
                    256 - 16 - (text_size.height / 2),
                    256 - 64 - text_size.width - 2,
                    text_size.height,
                    (float)diff_score / max_time - (smoothProgress * diff_score / max_time));

        // present
        riv_present();
    }
}

uint64_t Game::Score() const
{
    return score;
}
