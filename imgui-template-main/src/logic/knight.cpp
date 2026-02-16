#include "knight.hpp"
#include <utility>
#include "basic_move_check.hpp"
#include "chessboard.hpp"

Knight::Knight(bool i_am_white)
    : i_am_white(i_am_white)
{}

std::vector<std::pair<int, int>> Knight::possible_moves(Chessboard& chessboard, std::pair<int, int>& position)
{
    int                              x     = position.first;
    int                              y     = position.second;
    std::vector<std::pair<int, int>> moves = {};

    x -= 2;
    y -= 1;

    std::vector<std::vector<std::unique_ptr<Piece>>>& board = chessboard.get_board(); // x = A-H   y = 1-8

    if (in_range(x, y) && (board[x][y] == nullptr || board[x][y]->i_am_white != i_am_white))
    {
        moves.emplace_back(x, y);
    }

    y += 2;
    if (in_range(x, y) && (board[x][y] == nullptr || board[x][y]->i_am_white != i_am_white))
    {
        moves.emplace_back(x, y);
    }

    x++;
    y -= 3;
    if (in_range(x, y) && (board[x][y] == nullptr || board[x][y]->i_am_white != i_am_white))
    {
        moves.emplace_back(x, y);
    }

    y += 4;
    if (in_range(x, y) && (board[x][y] == nullptr || board[x][y]->i_am_white != i_am_white))
    {
        moves.emplace_back(x, y);
    }

    x += 2;
    if (in_range(x, y) && (board[x][y] == nullptr || board[x][y]->i_am_white != i_am_white))
    {
        moves.emplace_back(x, y);
    }

    y -= 4;
    if (in_range(x, y) && (board[x][y] == nullptr || board[x][y]->i_am_white != i_am_white))
    {
        moves.emplace_back(x, y);
    }

    x++;
    y++;
    if (in_range(x, y) && (board[x][y] == nullptr || board[x][y]->i_am_white != i_am_white))
    {
        moves.emplace_back(x, y);
    }

    y += 2;
    if (in_range(x, y) && (board[x][y] == nullptr || board[x][y]->i_am_white != i_am_white))
    {
        moves.emplace_back(x, y);
    }

    return moves;
}