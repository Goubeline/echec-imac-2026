#pragma once

#include "piece.hpp"

class Bishop : public Piece {
public:
    explicit Bishop(bool i_am_white);
    Bishop(/* args */);

    bool i_am_white;

    std::vector<std::pair<int, int>> possible_moves(Chessboard& board, std::pair<int, int>& position) override;
};