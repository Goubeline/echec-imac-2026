#include "rook.hpp"
#include "basic_move_check.hpp"
#include "chessboard.hpp"

Rook::Rook(bool i_am_white) : Piece(i_am_white) {}

std::vector<std::pair<int, int>> Rook::possible_moves(Chessboard& board, std::pair<int, int>& position)
{
    std::vector<std::pair<int, int>> moves = {};
    std::vector<std::pair<int, int>> eat   = {};
    line_moves(position, board.get_board(), i_am_white, moves, eat);
    column_moves(position, board.get_board(), i_am_white, moves, eat);
    return moves;
}