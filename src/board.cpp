#include <algorithm>
#include <map>
#include <string>

#include "board.h"
#include "config.h"

Board::Board(std::string desc, int moves) : moves(moves), selected(0)
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
        const auto &ps = positions[label];
        // XXX: validate positions
        const int stride = ps[1] - ps[0];
        pieces.push_back(Piece(label, ps[0], ps.size(), stride));
        riv_printf("piece %c position %02d size %d stride %d\n", label, ps[0], ps.size(), stride);
    }

    // select moveable piece
    SelectNext();
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

void Board::Draw(int64_t x0, int64_t y0, int64_t w, int64_t h, bool drawPrimaryPiece, bool drawPieces) const
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
    if (drawPrimaryPiece)
    {
        pieces[0].Draw(x0, y0, w, h, 0 == selected);
    }
    if (drawPieces)
    {
        for (size_t i = 1; i < pieces.size(); i++)
        {
            pieces[i].Draw(x0, y0, w, h, i == selected);
        }
    }
}

bool Board::Solved() const
{
    if (pieces.size() > 0)
    {
        // primary piece is always the first in the list
        Piece primaryPiece = pieces[0];

        // calculate exit position
        uint64_t exitPosition = (RUSH_GRID_SIZE * RUSH_GRID_SIZE / 2 - 2);

        // check primary piece position
        return primaryPiece.Position() == exitPosition;
    }
    return false;
}
