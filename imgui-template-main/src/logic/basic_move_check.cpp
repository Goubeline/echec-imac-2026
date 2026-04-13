#include "basic_move_check.hpp"
#include "piece.hpp"

bool in_range(int x, int y)
{
    return (x >= 0 && x < 8 && y >= 0 && y < 8);
}

void column_moves(std::pair<int, int>& position, std::vector<std::vector<std::unique_ptr<Piece>>> const& board, bool is_piece_white, std::vector<std::pair<int, int>>& moves, std::vector<std::pair<int, int>>& eat)
{
    for (int i = position.second + 1; i < 8; i++)
    {
        if (board[position.first][i] == nullptr)
        {
            moves.emplace_back(position.first, i);
        }
        else
        {
            if (board[position.first][i]->i_am_white != is_piece_white)
            {
                moves.emplace_back(position.first, i);
                eat.emplace_back(position.first, i);
            }
            break;
        }
    }

    for (int i = position.second - 1; i >= 0; i--)
    {
        if (board[position.first][i] == nullptr)
        {
            moves.emplace_back(position.first, i);
        }
        else
        {
            if (board[position.first][i]->i_am_white != is_piece_white)
            {
                moves.emplace_back(position.first, i);
                eat.emplace_back(position.first, i);
            }
            break;
        }
    }
}

void line_moves(std::pair<int, int>& position, std::vector<std::vector<std::unique_ptr<Piece>>> const& board, bool is_piece_white, std::vector<std::pair<int, int>>& moves, std::vector<std::pair<int, int>>& eat)
{
    for (int i = position.first + 1; i < 8; i++)
    {
        if (board[i][position.second] == nullptr)
        {
            moves.emplace_back(i, position.second);
        }
        else
        {
            if (board[i][position.second]->i_am_white != is_piece_white)
            {
                moves.emplace_back(i, position.second);
                eat.emplace_back(position.first, i);
            }
            break;
        }
    }

    for (int i = position.first - 1; i >= 0; i--)
    {
        if (board[i][position.second] == nullptr)
        {
            moves.emplace_back(i, position.second);
        }
        else
        {
            if (board[i][position.second]->i_am_white != is_piece_white)
            {
                moves.emplace_back(i, position.second);
                eat.emplace_back(position.first, i);
            }
            break;
        }
    }
}

void diag_up_left_moves(std::pair<int, int>& position, std::vector<std::vector<std::unique_ptr<Piece>>> const& board, bool is_piece_white, std::vector<std::pair<int, int>>& moves, std::vector<std::pair<int, int>>& eat)
{
    for (int i = position.first + 1, j = position.second - 1; i < 8 && j >= 0; i++, j--)
    {
        if (board[i][j] == nullptr)
        {
            moves.emplace_back(i, j);
        }
        else
        {
            if (board[i][j]->i_am_white != is_piece_white)
            {
                moves.emplace_back(i, j);
                eat.emplace_back(position.first, i);
            }
            break;
        }
    }

    for (int i = position.first - 1, j = position.second + 1; i >= 0 && j < 8; i--, j++)
    {
        if (board[i][j] == nullptr)
        {
            moves.emplace_back(i, j);
        }
        else
        {
            if (board[i][j]->i_am_white != is_piece_white)
            {
                moves.emplace_back(i, j);
                eat.emplace_back(position.first, i);
            }
            break;
        }
    }
}

void diag_up_right_moves(std::pair<int, int>& position, std::vector<std::vector<std::unique_ptr<Piece>>> const& board, bool is_piece_white, std::vector<std::pair<int, int>>& moves, std::vector<std::pair<int, int>>& eat)
{
    for (int i = position.first + 1, j = position.second + 1; i < 8 && j < 8; i++, j++)
    {
        if (board[i][j] == nullptr)
        {
            moves.emplace_back(i, j);
        }
        else
        {
            if (board[i][j]->i_am_white != is_piece_white)
            {
                moves.emplace_back(i, j);
                eat.emplace_back(position.first, i);
            }
            break;
        }
    }

    for (int i = position.first - 1, j = position.second - 1; i >= 0 && j >= 0; i--, j--)
    {
        if (board[i][j] == nullptr)
        {
            moves.emplace_back(i, j);
        }
        else
        {
            if (board[i][j]->i_am_white != is_piece_white)
            {
                moves.emplace_back(i, j);
                eat.emplace_back(position.first, i);
            }
            break;
        }
    }
}

void knight_move_case(int x, int y, std::vector<std::vector<std::unique_ptr<Piece>>> const& board, bool is_piece_white, std::vector<std::pair<int, int>>& moves, std::vector<std::pair<int, int>>& eat)
{
    if (in_range(x, y))
    {
        if (board[x][y] == nullptr)
        {
            moves.emplace_back(x, y);
        }
        else if (board[x][y]->i_am_white != is_piece_white)
        {
            moves.emplace_back(x, y);
            eat.emplace_back(x, y);
        }
    }
}

void knight_moves(std::pair<int, int>& position, std::vector<std::vector<std::unique_ptr<Piece>>> const& board, bool is_piece_white, std::vector<std::pair<int, int>>& moves, std::vector<std::pair<int, int>>& eat)
{
    int x = position.first;
    int y = position.second;
    x -= 2;
    y -= 1;
    knight_move_case(x, y, board, is_piece_white, moves, eat);

    y += 2;
    knight_move_case(x, y, board, is_piece_white, moves, eat);

    x++;
    y -= 3;
    knight_move_case(x, y, board, is_piece_white, moves, eat);

    y += 4;
    knight_move_case(x, y, board, is_piece_white, moves, eat);

    x += 2;
    knight_move_case(x, y, board, is_piece_white, moves, eat);

    y -= 4;
    knight_move_case(x, y, board, is_piece_white, moves, eat);

    x++;
    y++;
    knight_move_case(x, y, board, is_piece_white, moves, eat);

    y += 2;
    knight_move_case(x, y, board, is_piece_white, moves, eat);
}

void pawn_moves(std::pair<int, int>& position, std::vector<std::vector<std::unique_ptr<Piece>>> const& board, bool is_piece_white, std::vector<std::pair<int, int>>& moves, std::vector<std::pair<int, int>>& eat, std::pair<int, int>& en_passant)
{
    int x = position.first;
    int y = position.second;

    if (is_piece_white)
    {
        // --- PIONS BLANCS (Montent : y + 1) ---
        if (y + 1 < 8 && board[x][y + 1] == nullptr)
        {
            moves.emplace_back(x, y + 1);
            if (y == 1 && board[x][y + 2] == nullptr) // Double saut
            {
                moves.emplace_back(x, y + 2);
            }
        }

        // Manger à droite (x + 1)
        if (x + 1 < 8 && y + 1 < 8)
        {
            if (board[x + 1][y + 1] != nullptr && !board[x + 1][y + 1]->i_am_white) {
                moves.emplace_back(x + 1, y + 1);
                eat.emplace_back(x + 1, y + 1);
            } else if (en_passant.first == x + 1 && en_passant.second == y + 1) {
                moves.emplace_back(x + 1, y + 1);
                eat.emplace_back(x + 1, y + 1);
            }
        }
        // Manger à gauche (x - 1)
        if (x - 1 >= 0 && y + 1 < 8)
        {
            if (board[x - 1][y + 1] != nullptr && !board[x - 1][y + 1]->i_am_white) {
                moves.emplace_back(x - 1, y + 1);
                eat.emplace_back(x - 1, y + 1);
            } else if (en_passant.first == x - 1 && en_passant.second == y + 1) {
                moves.emplace_back(x - 1, y + 1);
                eat.emplace_back(x - 1, y + 1);
            }
        }
    }
    else
    {
        // --- PIONS NOIRS (Descendent : y - 1) ---
        if (y - 1 >= 0 && board[x][y - 1] == nullptr)
        {
            moves.emplace_back(x, y - 1);
            if (y == 6 && board[x][y - 2] == nullptr) // Double saut
            {
                moves.emplace_back(x, y - 2);
            }
        }

        // Manger à droite (x + 1)
        if (x + 1 < 8 && y - 1 >= 0)
        {
            if (board[x + 1][y - 1] != nullptr && board[x + 1][y - 1]->i_am_white) {
                moves.emplace_back(x + 1, y - 1);
                eat.emplace_back(x + 1, y - 1);
            } else if (en_passant.first == x + 1 && en_passant.second == y - 1) {
                moves.emplace_back(x + 1, y - 1);
                eat.emplace_back(x + 1, y - 1);
            }
        }
        // Manger à gauche (x - 1)
        if (x - 1 >= 0 && y - 1 >= 0)
        {
            if (board[x - 1][y - 1] != nullptr && board[x - 1][y - 1]->i_am_white) {
                moves.emplace_back(x - 1, y - 1);
                eat.emplace_back(x - 1, y - 1);
            } else if (en_passant.first == x - 1 && en_passant.second == y - 1) {
                moves.emplace_back(x - 1, y - 1);
                eat.emplace_back(x - 1, y - 1);
            }
        }
    }
}

void king_moves(std::pair<int, int>& position, std::vector<std::vector<std::unique_ptr<Piece>>> const& board, bool is_piece_white, std::vector<std::pair<int, int>>& moves, std::vector<std::pair<int, int>>& eat)
{
    for (int column = position.first - 1; column <= position.first + 1; column++)
    {
        for (int line = position.second - 1; line <= position.second + 1; line++)
        {
            if (!in_range(column, line))
            {
                continue;
            }

            if (board[column][line] == nullptr)
            {
                moves.emplace_back(column, line);
            }
            else if (board[column][line]->i_am_white != is_piece_white)
            {
                moves.emplace_back(column, line);
                eat.emplace_back(column, line);
            }
        }
    }
}