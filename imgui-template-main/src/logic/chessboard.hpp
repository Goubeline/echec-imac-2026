#pragma once

#include <memory>
#include <vector>
#include "piece.hpp"

class Chessboard {
private:
    std::pair<int, int>                              white_king;
    std::pair<int, int>                              black_king;
    std::vector<std::vector<std::unique_ptr<Piece>>> board; // x = A-H   y = 1-8
    bool                                             check;
    std::vector<std::pair<int, int>> piece_movement(std::pair<int, int>& piece);

public:
    // ---> MODIFICATION ICI : white_to_play est maintenant public
    bool                                             white_to_play = true; 

    Chessboard();
    // ---> MODIFICATION ICI : on demande au compilateur de générer le destructeur par défaut
    ~Chessboard() = default; 
    Chessboard(const Chessboard&);
    Chessboard(Chessboard&&) = delete;
    Chessboard& operator=(const Chessboard&);
    Chessboard& operator=(Chessboard&&) = delete;

    std::vector<std::pair<int, int>>                  select_piece(std::pair<int, int>& square);
    bool                                              movement(const std::pair<int, int>& start_position, const std::pair<int, int>& end_position);
    std::vector<std::vector<std::unique_ptr<Piece>>>& get_board();
    void                                              is_check(std::pair<int, int> king);

    bool                                             en_passant_possible;
    std::pair<int, int>                              en_passant_case;
};
// select piece renvoie les mouv possibles a partir de la case donnée
//  movement fait le mouvement (bool si mouv illégal le fait pas)
// is_check pour l'échec
// faire rook.cpp