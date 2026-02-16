#include "bishop.hpp"
#include "basic_move_check.hpp"
#include "chessboard.hpp"

Bishop::Bishop(bool i_am_white)
    : i_am_white(i_am_white)
{}

std::vector<std::pair<int, int>> Bishop::possible_moves(Chessboard& board, std::pair<int, int>& position)
{
    std::vector<std::pair<int, int>> moves = {};
    std::vector<std::pair<int, int>> eat   = {};
    diag_up_left_moves(position, board.get_board(), i_am_white, moves, eat);
    diag_up_right_moves(position, board.get_board(), i_am_white, moves, eat);
    return moves;
}