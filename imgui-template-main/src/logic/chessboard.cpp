#include "chessboard.hpp"
#include <stdexcept>
#include "basic_move_check.hpp"
#include "bishop.hpp"
#include "king.hpp"
#include "knight.hpp"
#include "pawn.hpp"
#include "queen.hpp"
#include "rook.hpp"

Chessboard::Chessboard()
    : white_to_play(true)
{
    board.resize(8);
    for (size_t i = 0; i < 8; i++)
    {
        board[i].resize(8);
    }
    for (size_t i = 0; i < 8; i++)
    {
        board[i][1] = std::make_unique<Pawn>(true);
        board[i][6] = std::make_unique<Pawn>(false);
    }
}

std::vector<std::vector<std::unique_ptr<Piece>>>& Chessboard::get_board()
{
    return board;
}

void Chessboard::is_check(std::pair<int, int> king)
{
    if (!in_range(king.first, king.second))
    {
        throw std::invalid_argument("king not on board");
    }

    if (dynamic_cast<King*>(board[king.first][king.second].get()) == nullptr)
    {
        throw std::invalid_argument("piece given is not king");
    }

    bool                             i_am_white = (board[king.first][king.second])->i_am_white;
    std::vector<std::pair<int, int>> moves      = {};
    std::vector<std::pair<int, int>> eat        = {};

    line_moves(king, board, i_am_white, moves, eat);
    column_moves(king, board, i_am_white, moves, eat);
    for (auto piece = eat.begin(); piece != eat.end();)
    {
        if (dynamic_cast<Queen*>(board[piece->first][piece->second].get()) != nullptr
            || dynamic_cast<Rook*>(board[piece->first][piece->second].get()) != nullptr)
        {
            check = true;
            return;
        }
        piece = eat.erase(piece);
    }

    diag_up_left_moves(king, board, i_am_white, moves, eat);
    diag_up_right_moves(king, board, i_am_white, moves, eat);
    for (auto piece = eat.begin(); piece != eat.end();)
    {
        if (dynamic_cast<Queen*>(board[piece->first][piece->second].get()) != nullptr
            || dynamic_cast<Bishop*>(board[piece->first][piece->second].get()) != nullptr)
        {
            check = true;
            return;
        }
        piece = eat.erase(piece);
    }
    moves.clear();

    knight_moves(king, board, i_am_white, moves, eat);
    for (auto piece = eat.begin(); piece != eat.end();)
    {
        if (dynamic_cast<Knight*>(board[piece->first][piece->second].get()) != nullptr)
        {
            check = true;
            return;
        }
        piece = eat.erase(piece);
    }

    pawn_moves(king, board, i_am_white, moves, eat);
    for (auto piece = eat.begin(); piece != eat.end();)
    {
        if (dynamic_cast<Pawn*>(board[piece->first][piece->second].get()) != nullptr)
        {
            check = true;
            return;
        }
        piece = eat.erase(piece);
    }

    king_moves(king, board, i_am_white, moves, eat);
    for (auto piece = eat.begin(); piece != eat.end();)
    {
        if (dynamic_cast<King*>(board[piece->first][piece->second].get()) != nullptr)
        {
            check = true;
            return;
        }
        piece = eat.erase(piece);
    }

    check = false;
}

std::vector<std::pair<int, int>> Chessboard::piece_movement(std::pair<int, int>& piece)
{
    std::vector<std::pair<int, int>> piece_move = (*board[piece.first][piece.second]).possible_moves(*this, piece);
    if (check)
    {
        /* code */
    }
    return piece_move;
}

std::vector<std::pair<int, int>> Chessboard::select_piece(const std::pair<int, int>& square) const
{}
bool Chessboard::movement(const std::pair<int, int>& start_position, const std::pair<int, int>& end_position)
{
}