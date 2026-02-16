#pragma once

#include <memory>
#include <vector>
#include "piece.hpp"

class Chessboard {
private:
    std::pair<int, int>                              white_king;
    std::pair<int, int>                              black_king;
    std::vector<std::vector<std::unique_ptr<Piece>>> board; // x = A-H   y = 1-8
    bool                                             white_to_play;
    bool                                             check;

    std::vector<std::pair<int, int>> piece_movement(std::pair<int, int>& piece);

public:
    Chessboard();
    ~Chessboard();
    Chessboard(const Chessboard&);
    Chessboard(Chessboard&&) = delete;
    Chessboard& operator=(const Chessboard&);
    Chessboard& operator=(Chessboard&&) = delete;

    std::vector<std::pair<int, int>>                  select_piece(const std::pair<int, int>& square) const;
    bool                                              movement(const std::pair<int, int>& start_position, const std::pair<int, int>& end_position);
    std::vector<std::vector<std::unique_ptr<Piece>>>& get_board();
    void                                              is_check(std::pair<int, int> king);
};
