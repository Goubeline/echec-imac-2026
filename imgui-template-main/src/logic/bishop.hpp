#pragma once

#include "piece.hpp"

class Bishop : public Piece {
public:
    Bishop(/* args */);
    ~Bishop() override;
    Bishop(const Bishop&)            = delete;
    Bishop(Bishop&&)                 = delete;
    Bishop& operator=(const Bishop&) = delete;
    Bishop& operator=(Bishop&&)      = delete;

    bool i_am_white;

    std::vector<std::pair<int, int>> possible_moves(Chessboard& board, std::pair<int, int>& position) override;
};