#include <riv.h>
#include <stdio.h>

#include "board.h"
#include "color.h"
#include "draw.h"
#include "piece.h"

void draw_palette()
{
    for (int i = 0; i < 32; i++)
    {
        riv_draw_rect_fill(i * 8, 0, 8, 8, i);
    }
}

void draw_health(uint64_t x0, uint64_t y0, uint64_t w, uint64_t h, float percent)
{
    // draw border
    riv_draw_rect_line(x0, y0, w, h, RUSH_COLOR_GRID_LINE);

    // draw fill
    uint64_t fw = percent * (w - 2);
    riv_draw_rect_fill(x0 + 1, y0 + 1, fw, h - 2, RUSH_COLOR_PIECE);
}

void draw_piece(const struct Piece *p, int64_t x0, int64_t y0, int64_t w, int64_t h, uint32_t color, int selected, int can_move, int clicks)
{
    int64_t padding = 2; // padding
    int64_t cw = w / 6;  // cell width
    int64_t ch = h / 6;  // cell height

    int64_t i0 = p->position;
    int64_t i1 = i0 + p->stride * (p->size - 1);
    int64_t px0 = (i0 % 6) * cw;
    int64_t py0 = (i0 / 6) * ch;
    int64_t px1 = (i1 % 6) * cw;
    int64_t py1 = (i1 / 6) * ch;
    int64_t pw = px1 - px0 + cw - padding;
    int64_t ph = py1 - py0 + ch - padding;
    uint32_t c = can_move ? (selected ? color + 1 : color) : color - 2;
    riv_draw_rect_fill(x0 + px0, y0 + py0, pw, ph, c);

    if (piece_is_fixed(p))
    {
        int64_t fx0 = x0 + px0 + 2;
        int64_t fy0 = y0 + py0 + 2;
        int64_t fx1 = fx0 + pw - 4;
        int64_t fy1 = fy0 + ph - 4;
        riv_draw_point(fx0, fy0, RUSH_COLOR_BOARD);
        riv_draw_point(fx0, fy1, RUSH_COLOR_BOARD);
        riv_draw_point(fx1, fy0, RUSH_COLOR_BOARD);
        riv_draw_point(fx1, fy1, RUSH_COLOR_BOARD);
    }
    if (selected)
    {
        riv_draw_rect_line(x0 + px0, y0 + py0, pw, ph, RUSH_COLOR_PIECE_OUTLINE);
    }
    if (clicks)
    {
        char c[8];
        snprintf(c, 8, "%d", clicks);
        riv_draw_text(c, RIV_SPRITESHEET_FONT_3X5, RIV_TOPLEFT, x0 + px0 + 2, y0 + py0 + 2, 1, RUSH_COLOR_PIECE_OUTLINE);
    }
}

riv_recti draw_title(int64_t x0, int64_t y0, int64_t w, uint64_t level)
{
    char title[256];
    char l[256];
    snprintf(title, 256, "Level %lu", level);
    snprintf(l, 256, "%lu", level);
    riv_recti r = riv_draw_text(title, RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, x0 + w / 2, y0 - 16, 1, RUSH_COLOR_GRID_LINE);
    riv_draw_text(l, RIV_SPRITESHEET_FONT_5X7, RIV_TOPRIGHT, r.x + r.width - 1, r.y, 1, RUSH_COLOR_BLACK);
    return r;
}

void draw_board(const struct Board *b, int64_t x0, int64_t y0, int64_t w, int64_t h, uint32_t colorOffset, uint16_t flags)
{
    int64_t ch = riv->width / 8;

    // draw background
    riv_draw_rect_fill(x0 - 2, y0 - 2, w + 2, h + 2, RUSH_COLOR_BOARD);

    // draw border
    riv_draw_rect_line(x0 - 3, y0 - 3, w + 4, h + 4, RUSH_COLOR_GRID_LINE);

    // draw exit
    uint64_t xExit = x0 + w;
    uint64_t yExit0 = y0 + (ch * 2) - 2;
    uint64_t yExit1 = y0 + (ch * 3) - 1;
    if (flags & RUSH_DRAW_EXIT)
    {
        riv_draw_line(xExit, yExit0, xExit, yExit1, RUSH_COLOR_BACKGROUND);
        riv_draw_line(xExit, yExit0, xExit + 4, yExit0, RUSH_COLOR_GRID_LINE);
        riv_draw_line(xExit, yExit1, xExit + 4, yExit1, RUSH_COLOR_GRID_LINE);
    }

    // draw entry
    uint64_t xEntry = x0 - 3;
    if (flags & RUSH_DRAW_ENTRY)
    {
        riv_draw_line(xEntry, yExit0, xEntry, yExit1, RUSH_COLOR_BACKGROUND);
    }

    // draw pieces
    if (flags & RUSH_DRAW_PRIMARY_PIECE)
    {
        int can_move = flags & RUSH_DRAW_DIM_UNMOVEABLE ? board_can_move(b, 0) : true;
        int clicks = flags & RUSH_DRAW_CLICKS && b->moveable_count > 2 ? board_select_diff(b, 0) : 0;
        uint64_t selected = b->moveable_pieces[b->selected];
        draw_piece(&b->pieces[0], x0, y0, w, h, RUSH_COLOR_RED_4 + colorOffset, 0 == selected, can_move, clicks);
    }
    for (size_t i = 1; i < b->piece_count; i++)
    {
        const struct Piece *p = &b->pieces[i];
        if (piece_is_fixed(p) && (flags & RUSH_DRAW_WALLS))
        {
            uint64_t selected = b->moveable_pieces[b->selected];
            draw_piece(p, x0, y0, w, h, RUSH_COLOR_GREY_4 + colorOffset, i == selected, false, 0);
        }
        else if (flags & RUSH_DRAW_PIECES)
        {
            int can_move = flags & RUSH_DRAW_DIM_UNMOVEABLE ? board_can_move(b, i) : true;
            int clicks = flags & RUSH_DRAW_CLICKS && b->moveable_count > 2 ? board_select_diff(b, i) : 0;
            uint64_t selected = b->moveable_pieces[b->selected];
            draw_piece(p, x0, y0, w, h, RUSH_COLOR_TEAL_4 + colorOffset, i == selected, can_move, clicks);
        }
    }

    // draw title
    draw_title(x0, y0, w, b->index + 1);
}

riv_recti draw_score(int64_t x0, int64_t y0, uint64_t score)
{
    char text[256];
    snprintf(text, 256, "%lu", score);
    riv_recti rn = riv_draw_text(text, RIV_SPRITESHEET_FONT_5X7, RIV_RIGHT, x0, y0, 1, RUSH_COLOR_BLACK);
    riv_recti rs = riv_draw_text("Score ", RIV_SPRITESHEET_FONT_5X7, RIV_RIGHT, x0 - rn.width, y0, 1, RUSH_COLOR_GRID_LINE);
    riv_recti r = {.x = x0 - (rn.width + rs.width), .y = y0, .width = rn.width + rs.width, .height = rn.height};
    return r;
}
