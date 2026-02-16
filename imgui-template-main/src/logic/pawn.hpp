#pragma once

#include "piece.hpp"

class Pawn : public Piece {
public:
    Pawn(/* args */);
    explicit Pawn(bool i_am_white);

    bool i_am_white;

    std::vector<std::pair<int, int>> possible_moves(Chessboard& board, std::pair<int, int>& position) override;
};