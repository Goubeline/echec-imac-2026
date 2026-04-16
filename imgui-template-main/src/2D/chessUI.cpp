#include "chessUI.hpp"
#include <imgui.h>
#include <iostream>
#include <algorithm>
#include <string>

#include "../logic/pawn.hpp"
#include "../logic/rook.hpp"
#include "../logic/knight.hpp"
#include "../logic/bishop.hpp"
#include "../logic/queen.hpp"
#include "../logic/king.hpp"

void ChessUI::DrawBoard(Chessboard& board, 
                        std::optional<std::pair<int, int>>& selected_square, 
                        std::vector<std::pair<int, int>>& possible_moves,
                        std::optional<std::string>& winner_message,
                        bool& show_promotion_modal,
                        std::pair<int, int>& promotion_square) 
{
    ImVec4 color_light    = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    ImVec4 color_dark     = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    ImVec4 color_selected = ImVec4(0.3f, 0.5f, 0.8f, 1.0f); 
    ImVec4 color_move     = ImVec4(0.5f, 0.8f, 0.5f, 1.0f); 

    // --- MODIFICATION : On bloque la sélection si une promotion est en cours ---
    if (winner_message || show_promotion_modal) {
        selected_square = std::nullopt;
        possible_moves.clear();
    } else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        selected_square = std::nullopt;
        possible_moves.clear();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    for (int render_row = 7; render_row >= 0; --render_row) {
        for (int col = 0; col < 8; ++col) {
            
            ImGui::PushID(render_row * 8 + col); 
            std::pair<int, int> current_pos = {col, render_row};

            bool is_light_square = (col + render_row) % 2 != 0; 
            ImVec4 current_color = is_light_square ? color_light : color_dark;

            if (selected_square && selected_square.value() == current_pos) {
                current_color = color_selected;
            } else if (std::find(possible_moves.begin(), possible_moves.end(), current_pos) != possible_moves.end()) {
                current_color = color_move;
            }

            ImGui::PushStyleColor(ImGuiCol_Button, current_color);

            std::string piece_text = " "; 
            Piece* p = board.get_board()[col][render_row].get();
            
            if (p != nullptr) {
                if (dynamic_cast<Pawn*>(p)) piece_text = "P";
                else if (dynamic_cast<Rook*>(p)) piece_text = "T";
                else if (dynamic_cast<Knight*>(p)) piece_text = "C";
                else if (dynamic_cast<Bishop*>(p)) piece_text = "F";
                else if (dynamic_cast<Queen*>(p)) piece_text = "D";
                else if (dynamic_cast<King*>(p)) piece_text = "R";

                if (p->i_am_white) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); 
                } else {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); 
                }
            } else {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); 
            }

            if (ImGui::Button(piece_text.c_str(), ImVec2(50.f, 50.f))) {
                
                // --- MODIFICATION : Clic autorisé seulement si pas de promotion ---
                if (!winner_message && !show_promotion_modal) {
                    if (!selected_square) {
                        if (p != nullptr && p->i_am_white == board.white_to_play) {
                            selected_square = current_pos;
                            possible_moves = board.select_piece(selected_square.value());
                        }
                    } else {
                        if (std::find(possible_moves.begin(), possible_moves.end(), current_pos) != possible_moves.end()) {
                            
                            Piece* dest_piece = board.get_board()[current_pos.first][current_pos.second].get();
                            if (dest_piece != nullptr && dynamic_cast<King*>(dest_piece)) {
                                if (board.white_to_play) winner_message = "Les Blancs gagnent la partie !";
                                else winner_message = "Les Noirs gagnent la partie !";
                            }

                            board.movement(selected_square.value(), current_pos);

                            // --- DÉTECTION DE LA PROMOTION ---
                            Piece* moved_piece = board.get_board()[current_pos.first][current_pos.second].get();
                            // Si c'est un pion ET qu'il est sur la ligne 0 ou 7
                            if (dynamic_cast<Pawn*>(moved_piece) && (current_pos.second == 0 || current_pos.second == 7)) {
                                show_promotion_modal = true;
                                promotion_square = current_pos;
                            }
                            // ---------------------------------
                            
                            board.white_to_play = !board.white_to_play; 
                        }
                        selected_square = std::nullopt;
                        possible_moves.clear();
                    }
                }
            }

            ImGui::PopStyleColor(2); 
            ImGui::PopID();

            if (col < 7) ImGui::SameLine();
        }
    }
    ImGui::PopStyleVar();
}