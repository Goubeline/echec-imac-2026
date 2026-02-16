#pragma once

#include "piece.hpp"

class Queen : public Piece {
public:
    Queen(/* args */);
    ~Queen() override;
    Queen(const Queen&)            = delete;
    Queen(Queen&&)                 = delete;
    Queen& operator=(const Queen&) = delete;
    Queen& operator=(Queen&&)      = delete;

    bool i_am_white;

    std::vector<std::pair<int, int>> possible_moves(Chessboard& board, std::pair<int, int>& position) override;
};