#pragma once

#include "piece.hpp"

class Knight : public Piece {
public:
    Knight(/* args */);
    ~Knight() override;
    Knight(const Knight&)            = delete;
    Knight(Knight&&)                 = delete;
    Knight& operator=(const Knight&) = delete;
    Knight& operator=(Knight&&)      = delete;

    bool i_am_white;

    std::vector<std::pair<int, int>> possible_moves(Chessboard& board, std::pair<int, int>& position) override;
};