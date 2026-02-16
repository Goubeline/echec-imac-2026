#pragma once

#include "piece.hpp"

class Rook : public Piece {
public:
    Rook(/* args */);
    ~Rook() override;
    Rook(const Rook&)            = delete;
    Rook(Rook&&)                 = delete;
    Rook& operator=(const Rook&) = delete;
    Rook& operator=(Rook&&)      = delete;

    bool i_am_white;

    std::vector<std::pair<int, int>> possible_moves(Chessboard& board, std::pair<int, int>& position) override;
};