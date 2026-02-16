#pragma once

#include "piece.hpp"

class King : public Piece {
public:
    explicit King(bool i_am_white);
    King(/* args */);

    bool i_am_white;

    std::vector<std::pair<int, int>> possible_moves(Chessboard& board, std::pair<int, int>& position) override;
};