#include "queen.hpp"
#include "basic_move_check.hpp"
#include "chessboard.hpp"

std::vector<std::pair<int, int>> Queen::possible_moves(Chessboard& board, std::pair<int, int>& position)
{
    std::vector<std::pair<int, int>> moves = {};
    std::vector<std::pair<int, int>> eat   = {};
    line_moves(position, board.get_board(), i_am_white, moves, eat);
    column_moves(position, board.get_board(), i_am_white, moves, eat);
    diag_up_left_moves(position, board.get_board(), i_am_white, moves, eat);
    diag_up_right_moves(position, board.get_board(), i_am_white, moves, eat);
    return moves;
}