#include <algorithm>
#include <map>
#include <string>

#include "board.h"
#include "config.h"

Board::Board(std::string desc)
{
    // build a list of positions for each label
    std::map<char, std::vector<int>> positions;
    for (int i = 0; i < desc.length(); i++)
    {
        const char label = desc[i];
        if (label == '.' || label == 'o')
        {
            continue;
        }
        positions[label].push_back(i);
    }

    // get list of labels
    std::vector<char> labels;
    labels.reserve(positions.size());
    for (const auto &pair : positions)
    {
        labels.push_back(pair.first);
    }

    // sort labels
    std::sort(labels.begin(), labels.end());

    // create pieces
    pieces.reserve(labels.size());
    for (const char label : labels)
    {
        const auto &ps = positions[label];
        // XXX: validate positions
        const int stride = ps[1] - ps[0];
        pieces.push_back(Piece(ps[0], ps.size(), stride));
    }
}

void Board::Draw(int64_t x0, int64_t y0, int64_t w, int64_t h) const
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

    // draw pieces
    for (const Piece &piece : pieces)
    {
        piece.Draw(x0, y0, w, h);
    }

    // TODO: draw primary piece

    // draw primary piece
    // riv_draw_rect_fill(x0 + 3, y0 + 3, cell_width - 5, cell_height - 5, RUSH_COLOR_PRIMARY_PIECE);

    // draw pieces
    // riv_draw_rect_fill(x0 + 3, y0 + cell_height + 3, cell_width - 5, cell_height + cell_height - 5, RUSH_COLOR_PIECE);
}

bool Board::Solved() const
{
    return false;
}
