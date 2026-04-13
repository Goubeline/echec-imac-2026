#include "pawn.hpp"
#include <utility>
#include "basic_move_check.hpp"
#include "chessboard.hpp"

Pawn::Pawn(bool i_am_white) : Piece(i_am_white) {}

std::vector<std::pair<int, int>> Pawn::possible_moves(Chessboard& chessboard, std::pair<int, int>& position)
{
    std::vector<std::pair<int, int>> moves = {};
    std::vector<std::pair<int, int>> eat   = {};
    if (chessboard.en_passant_possible)
    {
        pawn_moves(position, chessboard.get_board(), i_am_white, moves, eat, chessboard.en_passant_case);
    }
    else
    {
        std::pair<int, int> no_passant = std::make_pair<int>(-1, -1); //en passant impossible
        pawn_moves(position, chessboard.get_board(), i_am_white, moves, eat, no_passant);
    }
    
    return moves;
}