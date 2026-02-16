#pragma once

#include "piece.hpp"

class King : public Piece {
public:
    King(/* args */);
    ~King() override;
    King(const King&)            = delete;
    King(King&&)                 = delete;
    King& operator=(const King&) = delete;
    King& operator=(King&&)      = delete;

    bool i_am_white;

    std::vector<std::pair<int, int>> possible_moves(Chessboard& board, std::pair<int, int>& position) override;
};