#include <riv.h>

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 256
#define TARGET_FPS 60

#define RUSH_GRID_SIZE 6

#define RUSH_COLOR_BACKGROUND 32
#define RUSH_COLOR_BOARD 33
#define RUSH_COLOR_BLOCKED 34
#define RUSH_COLOR_GRID_LINE 35
#define RUSH_COLOR_PRIMARY_PIECE 36
#define RUSH_COLOR_PIECE 37
#define RUSH_COLOR_PIECE_OUTLINE 38
#define RUSH_COLOR_LABEL 39
#define RUSH_COLOR_WALL 40

int riv_draw_rounded_rect(int64_t x, int64_t y, int64_t w, int64_t h, int64_t r, uint32_t col)
{
    if (w < 2 * r || h < 2 * r)
    {
        return -1;
    }
    riv_draw_circle_fill(x + r, y + r, 2 * r, RIV_COLOR_WHITE);
    riv_draw_circle_fill(x + w - r, y + r, 2 * r, RIV_COLOR_GREY);
    riv_draw_circle_fill(x + r, y + h - r, 2 * r, RIV_COLOR_GOLD);
    riv_draw_circle_fill(x + w - r, y + h - r, 2 * r, RIV_COLOR_BLUE);
    riv_draw_rect_fill(x + r, y, w - 2 * r, h, RIV_COLOR_LIGHTRED);
    riv_draw_rect_fill(x, y + r, r, h - 2 * r, RIV_COLOR_GREEN);
    riv_draw_rect_fill(x + w - r, y + r, r, h - 2 * r, RIV_COLOR_YELLOW);
    return 0;
}

struct Board
{
    int64_t x;
};

int draw_board(int64_t x0, int64_t y0, int64_t w, int64_t h)
{
    int64_t cell_width = w / RUSH_GRID_SIZE;
    int64_t cell_height = h / RUSH_GRID_SIZE;

    riv_draw_rect_fill(x0, y0, w, h, RUSH_COLOR_BOARD);

    // draw grid
    for (int64_t x = x0; x <= x0 + w; x += cell_width)
    {
        riv_draw_line(x, y0, x, y0 + h, RUSH_COLOR_GRID_LINE);
    }
    for (int64_t y = y0; y <= y0 + h; y += cell_height)
    {
        riv_draw_line(x0, y, x0 + w, y, RUSH_COLOR_GRID_LINE);
    }

    // draw exit
    uint64_t xExit = x0 + w;
    uint64_t yExit = y0 + (cell_height / 2) * 5;
    riv_draw_triangle_fill(xExit, yExit - 5, xExit + 5, yExit, xExit, yExit + 5, RUSH_COLOR_GRID_LINE);

    // draw primary piece
    riv_draw_rect_fill(x0 + 3, y0 + 3, cell_width - 5, cell_height - 5, RUSH_COLOR_PRIMARY_PIECE);

    // draw pieces
    riv_draw_rect_fill(x0 + 3, y0 + cell_height + 3, cell_width - 5, cell_height + cell_height - 5, RUSH_COLOR_PIECE);
    return 0;
}

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

    do
    {
        // clear screen
        riv_clear(RUSH_COLOR_BACKGROUND);

        // riv_draw_rounded_rect(0, 0, 192, 128, 32, RIV_COLOR_GREEN);
        draw_board(32, 32, RUSH_GRID_SIZE * 32, RUSH_GRID_SIZE * 32);

    } while (riv_present());
}
