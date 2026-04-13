#include "king.hpp"
#include "basic_move_check.hpp"
#include "chessboard.hpp"

King::King(bool i_am_white) : Piece(i_am_white) {}

std::vector<std::pair<int, int>> King::possible_moves(Chessboard& board, std::pair<int, int>& position)
{
    std::vector<std::pair<int, int>> moves = {};
    std::vector<std::pair<int, int>> eat   = {};
    king_moves(position, board.get_board(), i_am_white, moves, eat);
    return moves;
}