#pragma once

#include "piece.hpp"

class Knight : public Piece {
public:
    explicit Knight(bool i_am_white);
    Knight(/* args */);

    bool i_am_white;

    std::vector<std::pair<int, int>> possible_moves(Chessboard& board, std::pair<int, int>& position) override;
};