#pragma once
#include "../logic/chessboard.hpp"
#include <optional>
#include <vector>
#include <string>

class ChessUI {
public:
    static void DrawBoard(Chessboard& board, 
                          std::optional<std::pair<int, int>>& selected_square, 
                          std::vector<std::pair<int, int>>& possible_moves,
                          std::optional<std::string>& winner_message,
                          bool& show_promotion_modal,             // <-- NOUVEAU
                          std::pair<int, int>& promotion_square); // <-- NOUVEAU
};