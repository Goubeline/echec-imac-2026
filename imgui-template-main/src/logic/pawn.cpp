#include "pawn.hpp"
#include <utility>
#include "basic_move_check.hpp"
#include "chessboard.hpp"

Pawn::Pawn(bool i_am_white)
    : i_am_white(i_am_white)
{}

std::vector<std::pair<int, int>> Pawn::possible_moves(Chessboard& chessboard, std::pair<int, int>& position)
{
    std::vector<std::pair<int, int>> moves = {};
    std::vector<std::pair<int, int>> eat   = {};
    pawn_moves(position, chessboard.get_board(), i_am_white, moves, eat);
    return moves;
}