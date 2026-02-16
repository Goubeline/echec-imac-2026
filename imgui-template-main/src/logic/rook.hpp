#pragma once

#include "piece.hpp"

class Rook : public Piece {
public:
    explicit Rook(bool i_am_white);
    Rook(/* args */);

    bool i_am_white;

    std::vector<std::pair<int, int>> possible_moves(Chessboard& board, std::pair<int, int>& position) override;
};