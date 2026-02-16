#pragma once

#include "piece.hpp"

class Queen : public Piece {
public:
    explicit Queen(bool i_am_white);
    Queen(/* args */);

    bool i_am_white;

    std::vector<std::pair<int, int>> possible_moves(Chessboard& board, std::pair<int, int>& position) override;
};