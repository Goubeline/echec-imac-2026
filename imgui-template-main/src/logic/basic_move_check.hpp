#pragma once

#include <memory>
#include <vector>
#include "piece.hpp"

bool in_range(int x, int y);
void line_moves(std::pair<int, int>& position, std::vector<std::vector<std::unique_ptr<Piece>>> const& board, bool is_piece_white, std::vector<std::pair<int, int>>& moves, std::vector<std::pair<int, int>>& eat);
void column_moves(std::pair<int, int>& position, std::vector<std::vector<std::unique_ptr<Piece>>> const& board, bool is_piece_white, std::vector<std::pair<int, int>>& moves, std::vector<std::pair<int, int>>& eat);
void diag_up_left_moves(std::pair<int, int>& position, std::vector<std::vector<std::unique_ptr<Piece>>> const& board, bool is_piece_white, std::vector<std::pair<int, int>>& moves, std::vector<std::pair<int, int>>& eat);
void diag_up_right_moves(std::pair<int, int>& position, std::vector<std::vector<std::unique_ptr<Piece>>> const& board, bool is_piece_white, std::vector<std::pair<int, int>>& moves, std::vector<std::pair<int, int>>& eat);
void knight_moves(std::pair<int, int>& position, std::vector<std::vector<std::unique_ptr<Piece>>> const& board, bool is_piece_white, std::vector<std::pair<int, int>>& moves, std::vector<std::pair<int, int>>& eat);
void pawn_moves(std::pair<int, int>& position, std::vector<std::vector<std::unique_ptr<Piece>>> const& board, bool is_piece_white, std::vector<std::pair<int, int>>& moves, std::vector<std::pair<int, int>>& eat);
void king_moves(std::pair<int, int>& position, std::vector<std::vector<std::unique_ptr<Piece>>> const& board, bool is_piece_white, std::vector<std::pair<int, int>>& moves, std::vector<std::pair<int, int>>& eat);