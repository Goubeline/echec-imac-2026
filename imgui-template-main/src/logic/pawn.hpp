#pragma once

#include "piece.hpp"

class Pawn : public Piece {
public:
    Pawn(/* args */);
    ~Pawn() override;
    Pawn(const Pawn&)            = delete;
    Pawn(Pawn&&)                 = delete;
    Pawn& operator=(const Pawn&) = delete;
    Pawn& operator=(Pawn&&)      = delete;

    bool i_am_white;

    std::vector<std::pair<int, int>> possible_moves(Chessboard& board, std::pair<int, int>& position) override;
};