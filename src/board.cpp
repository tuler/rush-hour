#include <algorithm>
#include <map>
#include <string>

#include "board.h"
#include "color.h"

Board::Board(uint64_t index, std::string desc, uint64_t moves) : index(index),
                                                                 moves(moves),
                                                                 selected(0)
{
    // build a list of positions for each label
    std::map<char, std::vector<int>> positions;
    for (size_t i = 0; i < desc.length(); i++)
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
        if (label == 'x')
        {
            // process walls on another pass
            continue;
        }
        const auto &ps = positions[label];
        // XXX: validate positions
        const int stride = ps[1] - ps[0];
        pieces.push_back(Piece(ps[0], ps.size(), stride));
        riv_printf("piece %c position %02d size %d stride %d\n", label, ps[0], ps.size(), stride);
    }

    // create walls
    for (const auto &p : positions['x'])
    {
        pieces.push_back(Piece(p, 1, 1));
        riv_printf("wall position %02d\n", p);
    }
}

int Board::PieceAt(uint64_t position) const
{
    for (size_t i = 0; i < pieces.size(); i++)
    {
        const Piece &piece = pieces[i];
        uint64_t p = piece.Position();
        for (uint64_t j = 0; j < piece.Size(); j++)
        {
            if (p == position)
            {
                return i;
            }
            p += piece.Stride();
        }
    }
    return -1;
}

const Piece &Board::PrimaryPiece() const
{
    return pieces[0];
}

bool Board::IsOccupied(uint64_t position) const
{
    return PieceAt(position) >= 0;
}

bool Board::CanMoveForward(uint64_t index) const
{
    const Piece &piece = pieces[index];
    return piece.CanMoveForward() && !IsOccupied(piece.EndPosition() + piece.Stride());
}

bool Board::CanMoveBackward(uint64_t index) const
{
    const Piece &piece = pieces[index];
    return piece.CanMoveBackward() && !IsOccupied(piece.Position() - piece.Stride());
}

bool Board::CanMove(uint64_t index) const
{
    return CanMoveBackward(index) || CanMoveForward(index);
}

bool Board::SelectNext()
{
    uint64_t s = selected;
    do
    {
        s = (s + 1) % pieces.size();
    } while (s != selected && !CanMove(s));
    selected = s;
    return true;
}

bool Board::SelectPrevious()
{
    uint64_t s = selected;
    do
    {
        s = (s - 1 + pieces.size()) % pieces.size();
    } while (s != selected && !CanMove(s));
    selected = s;
    return true;
}

bool Board::MoveSelectedBackward()
{
    if (!CanMoveBackward(selected))
    {
        return false;
    }
    Piece &piece = pieces[selected];
    piece.Move(-1);
    return true;
}

bool Board::MoveSelectedForward()
{
    if (!CanMoveForward(selected))
    {
        return false;
    }
    Piece &piece = pieces[selected];
    piece.Move(1);
    return true;
}

void Board::Draw(int64_t x0, int64_t y0, int64_t w, int64_t h, uint32_t colorOffset, uint16_t flags) const
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
        pieces[0].Draw(x0, y0, w, h, RUSH_COLOR_RED_5 + colorOffset, 0 == selected);
    }
    for (size_t i = 1; i < pieces.size(); i++)
    {
        if (pieces[i].Fixed() && (flags & RUSH_DRAW_WALLS))
        {
            pieces[i].Draw(x0, y0, w, h, RUSH_COLOR_GREY_5 + colorOffset, i == selected);
        }
        else if (flags & RUSH_DRAW_PIECES)
        {
            pieces[i].Draw(x0, y0, w, h, RUSH_COLOR_TEAL_5 + colorOffset, i == selected);
        }
    }

    // draw title
    std::string title = "Level " + std::to_string(index + 1);
    riv_draw_text(title.c_str(), RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, x0 + w / 2, y0 - 16, 1, RIV_COLOR_BLACK);
}

bool Board::Solved() const
{
    if (pieces.size() > 0)
    {
        // primary piece is always the first in the list
        Piece primaryPiece = pieces[0];

        // calculate exit position
        uint64_t exitPosition = (6 * 6 / 2 - 2);

        // check primary piece position
        return primaryPiece.Position() == exitPosition;
    }
    return false;
}
