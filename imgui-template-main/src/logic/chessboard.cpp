#include "chessboard.hpp"
#include <stdexcept>
#include "basic_move_check.hpp"
#include "bishop.hpp"
#include "king.hpp"
#include "knight.hpp"
#include "pawn.hpp"
#include "queen.hpp"
#include "rook.hpp"

#include <vector>

Chessboard::Chessboard()
    : white_to_play(true), check(false), en_passant_possible(false)
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
    board[0][0] = std::make_unique<Rook>(true);
    board[0][7] = std::make_unique<Rook>(false);
    board[1][0] = std::make_unique<Knight>(true);
    board[1][7] = std::make_unique<Knight>(false);
    board[2][0] = std::make_unique<Bishop>(true);
    board[2][7] = std::make_unique<Bishop>(false);
    board[4][0] = std::make_unique<Queen>(true);
    board[4][7] = std::make_unique<Queen>(false);
    board[7][0] = std::make_unique<Rook>(true);
    board[7][7] = std::make_unique<Rook>(false);
    board[6][0] = std::make_unique<Knight>(true);
    board[6][7] = std::make_unique<Knight>(false);
    board[5][0] = std::make_unique<Bishop>(true);
    board[5][7] = std::make_unique<Bishop>(false);
    board[3][0] = std::make_unique<King>(true);
    board[3][7] = std::make_unique<King>(false);

    white_king = std::make_pair<int>(3, 0);
    black_king = std::make_pair<int>(3, 7);
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

    std::pair<int, int> not_a_pawn = std::make_pair<int>(-1, -1); //en passant impossible
    pawn_moves(king, board, i_am_white, moves, eat, not_a_pawn);
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
        //do stuff
    }
    return piece_move;
}

std::vector<std::pair<int, int>> Chessboard::select_piece(std::pair<int, int>& square)
{
    if (board[square.first][square.second] == nullptr)
    {
        std::vector<std::pair<int, int>> moves = {};
        return moves;
    }
    
    return piece_movement(square);
}

bool Chessboard::movement(const std::pair<int, int>& start_position, const std::pair<int, int>& end_position)
{
    if (abs(end_position.second - start_position.second) == 2 && dynamic_cast<Pawn*>(board[start_position.first][start_position.second].get()) != nullptr)
    {
        en_passant_possible = true;
        en_passant_case.first = end_position.first;
        en_passant_case.second = end_position.second;
        if (white_to_play)
        {
            en_passant_case.second--;
        }
        else
        {
            en_passant_case.second++;
        }
    }
    else
    {
        if (en_passant_possible && end_position.first == en_passant_case.first && end_position.second == en_passant_case.second && dynamic_cast<Pawn*>(board[start_position.first][start_position.second].get()) != nullptr)
        {
            board[end_position.first][start_position.second] = nullptr;
            
        }
        en_passant_possible = false;
    }
    
    if (dynamic_cast<King*>(board[start_position.first][start_position.second].get()) != nullptr)
    {
        if (white_to_play)
        {
            white_king.first = end_position.first;
            white_king.second = end_position.second;
        }
        else
        {
            black_king.first = end_position.first;
            black_king.second = end_position.second;
        }
    }
    
    if (white_to_play)
    {
        is_check(black_king);
    }
    else
    {
        is_check(white_king);
    }

    board[end_position.first][end_position.second] = std::move(board[start_position.first][start_position.second]);
    board[start_position.first][start_position.second] = nullptr;  

    if (check)
    {
        check = false;
        return false;
    }
    
    return true;
}