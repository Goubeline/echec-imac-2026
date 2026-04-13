#pragma once

#include <vector>

class Chessboard;

class Piece {
public:
    // Nouveau constructeur qui initialise la variable :
    Piece(bool is_white) : i_am_white(is_white) {}

    virtual ~Piece()               = default;
    Piece(const Piece&)            = delete;
    Piece(Piece&&)                 = delete;
    Piece& operator=(const Piece&) = delete;
    Piece& operator=(Piece&&)      = delete;

    bool i_am_white;

    virtual std::vector<std::pair<int, int>> possible_moves(Chessboard& board, std::pair<int, int>& position) = 0;
};