#include <math.h>

#include "board.h"
#include "color.h"
#include "draw.h"
#include "game.h"
#include "level.h"
#include "seqt.h"
#include "sfx.h"

// Ease in-out function using sine
float ease(float x)
{
    return (1.0f - cos(x * M_PI)) / 2.0f;
}

int64_t interpolate(int64_t a, int64_t b, float t)
{
    return a + (b - a) * t;
}

struct Game game_create(struct Levels *levels, uint64_t time_per_move)
{
    struct Game g = {
        .score = 0,
        .levels = levels,
        .time_per_move = time_per_move};
    return g;
}

void game_write_score(uint64_t score, uint64_t level)
{
    riv->outcard_len = riv_snprintf(
        (char *)riv->outcard, RIV_SIZE_OUTCARD,
        "JSON{\"score\":%d,\"level\":%d}",
        score,
        level);
}

void game_title(struct Game *game)
{
    const int STEPS = riv->width / 6;
    uint64_t img = riv_make_image("sprite.png", RUSH_COLOR_WHITE);

    for (int step = 0; step <= STEPS; step++)
    {
        riv_clear(RUSH_COLOR_BACKGROUND);

        // Calculate smooth progress (0.0 to 1.0)
        float progress = (float)step / STEPS;
        float smoothProgress = ease(progress);

        riv_clear(RUSH_COLOR_BACKGROUND);

        riv_draw_image_rect(img, interpolate(-256, 32, smoothProgress), 80, 160, 48, 0, 0, 1, 1);
        riv_draw_image_rect(img, interpolate(356, 64, smoothProgress), 128 - 8, 160, 48, 0, 96, 1, 1);
        riv_draw_rect_fill(0, 0, 256, interpolate(0, 74, smoothProgress), RUSH_COLOR_TEAL_5);
        riv_draw_rect_fill(0, interpolate(256, 256 - 74, smoothProgress), 256, interpolate(0, 84, smoothProgress), RUSH_COLOR_RED_5);
        riv_draw_line(0, interpolate(-2, 72, smoothProgress), 256, interpolate(-2, 72, smoothProgress), RUSH_COLOR_WHITE);
        riv_draw_line(0, interpolate(256 + 1, 256 - 73, smoothProgress), 256, interpolate(256 + 1, 256 - 73, smoothProgress), RUSH_COLOR_WHITE);

        riv_present();
        seqt_poll();
    }

    do
    {
        riv_clear(RUSH_COLOR_BACKGROUND);
        riv_draw_image_rect(img, 32, 80, 160, 48, 0, 0, 1, 1);
        riv_draw_image_rect(img, 64, 128 - 8, 160, 48, 0, 96, 1, 1);
        riv_draw_rect_fill(0, 0, 256, 74, RUSH_COLOR_TEAL_5);
        riv_draw_rect_fill(0, 256 - 74, 256, 84, RUSH_COLOR_RED_5);
        riv_draw_line(0, 72, 256, 72, RUSH_COLOR_WHITE);
        riv_draw_line(0, 256 - 73, 256, 256 - 73, RUSH_COLOR_WHITE);

        riv_draw_text("START",
                      RIV_SPRITESHEET_FONT_5X7,
                      RIV_CENTER,
                      128,
                      256 - 48,
                      1,
                      riv->frame % 8 ? RUSH_COLOR_WHITE : RUSH_COLOR_RED_5);

        if (riv->keys[RIV_GAMEPAD_A1].press ||
            riv->keys[RIV_GAMEPAD_A2].press ||
            riv->keys[RIV_GAMEPAD_A3].press ||
            riv->keys[RIV_GAMEPAD_A4].press ||
            riv->keys[RIV_GAMEPAD_L1].press ||
            riv->keys[RIV_GAMEPAD_L2].press ||
            riv->keys[RIV_GAMEPAD_L3].press ||
            riv->keys[RIV_GAMEPAD_R1].press ||
            riv->keys[RIV_GAMEPAD_R2].press ||
            riv->keys[RIV_GAMEPAD_R3].press ||
            riv->keys[RIV_GAMEPAD_SELECT].press ||
            riv->keys[RIV_GAMEPAD_START].press)
        {
            sfx_start();
            break;
        }
        seqt_poll();
    } while (riv_present());

    for (int step = 0; step <= STEPS; step++)
    {
        riv_clear(RUSH_COLOR_BACKGROUND);

        // Calculate smooth progress (0.0 to 1.0)
        float progress = (float)step / STEPS;
        float smoothProgress = ease(progress);

        riv_clear(RUSH_COLOR_BACKGROUND);

        riv_draw_image_rect(img, interpolate(32, -256, smoothProgress), 80, 160, 48, 0, 0, 1, 1);
        riv_draw_image_rect(img, interpolate(64, 356, smoothProgress), 128 - 8, 160, 48, 0, 96, 1, 1);
        riv_draw_rect_fill(0, 0, 256, interpolate(74, 0, smoothProgress), RUSH_COLOR_TEAL_5);
        riv_draw_rect_fill(0, interpolate(256 - 74, 256, smoothProgress), 256, interpolate(84, 0, smoothProgress), RUSH_COLOR_RED_5);
        riv_draw_line(0, interpolate(72, -2, smoothProgress), 256, interpolate(72, -2, smoothProgress), RUSH_COLOR_WHITE);
        riv_draw_line(0, interpolate(256 - 73, 256 + 1, smoothProgress), 256, interpolate(256 - 73, 256 + 1, smoothProgress), RUSH_COLOR_WHITE);

        seqt_poll();
        riv_present();
    }
}

uint64_t game_play(struct Game *game, struct Board *board)
{
    // how much time we give the player to complete the level
    uint64_t max_time = game->time_per_move * board->moves;

    uint64_t start_time = riv->time_ms;
    uint64_t timeout = start_time + max_time;

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
        if (board_is_solved(board))
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
            board_select_previous(board);
        }
        if (riv->keys[RIV_GAMEPAD_A3].press ||
            riv->keys[RIV_GAMEPAD_A4].press ||
            riv->keys[RIV_GAMEPAD_R1].press ||
            riv->keys[RIV_GAMEPAD_R2].press ||
            riv->keys[RIV_GAMEPAD_R3].press ||
            riv->keys[RIV_GAMEPAD_START].press)
        {
            board_select_next(board);
        }
        if (riv->keys[RIV_GAMEPAD_UP].press ||
            riv->keys[RIV_GAMEPAD_LEFT].press)
        {
            board_move_selected_backward(board);
            sfx_move();
        }
        if (riv->keys[RIV_GAMEPAD_DOWN].press ||
            riv->keys[RIV_GAMEPAD_RIGHT].press)
        {
            board_move_selected_forward(board);
            sfx_move();
        }

        // clear screen
        riv_clear(RUSH_COLOR_BACKGROUND);

        // draw board
        draw_board(board, 32, 32, 6 * 32, 6 * 32, 0,
                   RUSH_DRAW_PRIMARY_PIECE | RUSH_DRAW_PIECES | RUSH_DRAW_WALLS | RUSH_DRAW_EXIT | RUSH_DRAW_DIM_UNMOVEABLE);

        // draw score
        riv_recti text_size = draw_score(256 - 32, 256 - 16, game->score);

        // draw timer
        draw_health(32 - 3,
                    256 - 16 - (text_size.height / 2),
                    256 - 64 - text_size.width - 4,
                    text_size.height,
                    (float)time_left / max_time);

        // time running out sfx
        if ((time_left < 2000 && riv->frame % 8 == 0) ||
            (time_left < 4000 && riv->frame % 16 == 0) ||
            (time_left < 6000 && riv->frame % 32 == 0))
        {
            sfx_time();
        }

        seqt_poll();
    } while (riv_present());
    return 0;
}

void game_transition(struct Game *game, struct Board *current, struct Board *next, uint64_t old_score, uint64_t new_score)
{
    const struct Piece *current_piece = &current->pieces[0];
    const struct Piece *next_piece = &next->pieces[0];
    int delta = current_piece->position - next_piece->position;
    const int STEPS = riv->width / 4;

    // how much time we give the player to complete the level
    uint64_t max_time = game->time_per_move * current->moves;

    uint64_t diff_score = new_score - old_score;

    // draw transition from one board to the next
    for (int step = 0; step <= STEPS; step++)
    {
        // clear screen
        riv_clear(RUSH_COLOR_BACKGROUND);

        // Calculate smooth progress (0.0 to 1.0)
        float progress = (float)step / STEPS;
        float smoothProgress = ease(progress);

        // Calculate interpolated offset
        int offset = (int)(smoothProgress * riv->width);
        bool drawPrimary = offset < ((int)riv->width - delta * 32);

        // Draw boards with interpolated positions
        draw_board(current, 32 - offset, 32, 6 * 32, 6 * 32, 0,
                   RUSH_DRAW_PIECES | RUSH_DRAW_WALLS | RUSH_DRAW_EXIT);
        draw_board(next, 32 + riv->width - offset, 32, 6 * 32, 6 * 32, 0,
                   RUSH_DRAW_EXIT | RUSH_DRAW_ENTRY | (drawPrimary ? 0 : RUSH_DRAW_PRIMARY_PIECE));

        // Draw fixed primary piece
        if (drawPrimary)
        {
            draw_piece(current_piece, 32, 32, 6 * 32, 6 * 32, RUSH_COLOR_RED_4, true, true);
        }

        // draw score
        uint64_t sc = old_score + (new_score - old_score) * smoothProgress;
        riv_recti text_size = draw_score(256 - 32, 256 - 16, sc);
        if (riv->frame % 4 == 0)
        {
            sfx_score();
        }

        // draw timer
        draw_health(32 - 3,
                    256 - 16 - (text_size.height / 2),
                    256 - 64 - text_size.width - 4,
                    text_size.height,
                    (float)diff_score / max_time - (smoothProgress * diff_score / max_time));

        // present
        seqt_poll();
        riv_present();
    }

    // draw final state with interpolated alpha for pieces
    for (int colorOffset = -5; colorOffset <= 0; colorOffset++)
    {
        // clear screen
        riv_clear(RUSH_COLOR_BACKGROUND);

        // Draw next board
        draw_board(next, 32, 32, 6 * 32, 6 * 32, colorOffset,
                   RUSH_DRAW_PRIMARY_PIECE | RUSH_DRAW_PIECES | RUSH_DRAW_WALLS | RUSH_DRAW_EXIT);
        draw_piece(&next->pieces[0], 32, 32, 6 * 32, 6 * 32, RUSH_COLOR_RED_4, true, true);

        // draw score
        riv_recti text_size = draw_score(256 - 32, 256 - 16, new_score);

        // draw timer
        float p = (float)(colorOffset + 5) / 5;
        draw_health(32 - 3,
                    256 - 16 - (text_size.height / 2),
                    256 - 64 - text_size.width - 4,
                    text_size.height,
                    p);

        // present
        seqt_poll();
        riv_present();
    }
}

void game_initial_transition(struct Game *game, struct Board *next)
{
    const int STEPS = riv->width / 4;
    int can_move = board_can_move(next, 0);

    // draw transition from one board to the next
    for (int step = 0; step <= STEPS; step++)
    {
        // clear screen
        riv_clear(RUSH_COLOR_BACKGROUND);

        // Calculate smooth progress (0.0 to 1.0)
        float progress = (float)step / STEPS;
        float smoothProgress = ease(progress);

        // Calculate interpolated offset
        int offset = (int)(smoothProgress * riv->width);

        // Draw boards with interpolated positions
        draw_board(next, 32 + riv->width - offset, 32, 6 * 32, 6 * 32, 0,
                   RUSH_DRAW_EXIT | RUSH_DRAW_ENTRY);

        // Draw fixed primary piece
        draw_piece(&next->pieces[0], 32 - (256 - offset), 32, 6 * 32, 6 * 32, RUSH_COLOR_RED_4, true, can_move);

        // present
        seqt_poll();
        riv_present();
    }

    // draw final state with interpolated alpha for pieces
    for (int colorOffset = -5; colorOffset <= 0; colorOffset++)
    {
        // clear screen
        riv_clear(RUSH_COLOR_BACKGROUND);

        // Draw next board
        draw_board(next, 32, 32, 6 * 32, 6 * 32, colorOffset,
                   RUSH_DRAW_PRIMARY_PIECE | RUSH_DRAW_PIECES | RUSH_DRAW_WALLS | RUSH_DRAW_EXIT);
        draw_piece(&next->pieces[0], 32, 32, 6 * 32, 6 * 32, RUSH_COLOR_RED_4, true, can_move);

        // draw score
        draw_score(256 - 32, 256 - 16, 0);

        // present
        seqt_poll();
        riv_present();
    }
}

void game_over(struct Game *game, struct Board *board)
{
    // quit after 3 seconds
    riv->quit_frame = riv->frame + riv->target_fps * 3;

    sfx_game_over();
    do
    {
        riv_clear(RUSH_COLOR_BACKGROUND);

        // draw board
        draw_board(board, 32, 32, 6 * 32, 6 * 32, -3,
                   RUSH_DRAW_PRIMARY_PIECE | RUSH_DRAW_PIECES | RUSH_DRAW_WALLS | RUSH_DRAW_EXIT);

        // draw score
        draw_score(256 - 32, 256 - 16, game->score);

        riv_draw_text("GAME OVER",
                      RIV_SPRITESHEET_FONT_5X7,
                      RIV_LEFT,
                      32 - 3,
                      256 - 16,
                      1,
                      RIV_COLOR_BLACK);

        seqt_poll();

    } while (riv_present());
}

void game_start(struct Game *game)
{
    uint64_t music = seqt_play(seqt_make_source_from_file("music.rivcard"), -1);

    // title screen
    game_write_score(0, 0);
    game_title(game);

    uint64_t l = 0; // level
    uint64_t result = 0;

    struct Levels *levels = game->levels;
    struct Level entry = levels->entries[l];
    struct Board board = board_create(l, entry.desc, entry.moves);
    game_initial_transition(game, &board);
    do
    {
        result = game_play(game, &board);
        game->score += result;

        // write score and level out
        game_write_score(game->score, l + 1);

        if (l + 1 >= levels->count)
        {
            // gone through all levels
            break;
        }
        if (result == 0)
        {
            // player ran out of time
            break;
        }

        struct Board next = board_create(l + 1, levels->entries[l + 1].desc, levels->entries[l + 1].moves);
        game_transition(game, &board, &next, game->score - result, game->score);
        board = next;
        l++;

    } while (result > 0);
    seqt_stop(music);
    game_over(game, &board);
}
