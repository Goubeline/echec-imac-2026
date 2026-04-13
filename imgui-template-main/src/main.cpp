#include "quick_imgui/quick_imgui.hpp"
#include "logic/chessboard.hpp" 
#include "design/chessUI.hpp"
#include <optional>
#include <vector>
#include <string>
#include <memory>

// AJOUT DES INCLUDES POUR CRÉER LES NOUVELLES PIÈCES
#include "logic/queen.hpp"
#include "logic/rook.hpp"
#include "logic/bishop.hpp"
#include "logic/knight.hpp"

int main() {
    Chessboard myBoard;    

    std::optional<std::pair<int, int>> selected_square;
    std::vector<std::pair<int, int>> possible_moves;
    std::optional<std::string> winner_message = std::nullopt; 

    // --- VARIABLES DE PROMOTION ---
    bool show_promotion_modal = false;
    std::pair<int, int> promotion_square = {-1, -1};
    // ------------------------------

    quick_imgui::loop(
        "Chess Game",
        {
            .init = [&]() {},
            .loop = [&]() {
                ImGui::Begin("Plateau");
                
                // On ajoute nos deux nouvelles variables d'état
                ChessUI::DrawBoard(myBoard, selected_square, possible_moves, winner_message, show_promotion_modal, promotion_square);

                ImGui::End();

                if (winner_message) {
                    ImGui::Begin("FIN DE PARTIE", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
                    ImGui::Text("%s", winner_message.value().c_str());
                    
                    if (ImGui::Button("Quitter le jeu")) {
                        exit(0);
                    }
                    ImGui::End();
                }

                // --- AFFICHAGE DE LA FENÊTRE DE PROMO ---
                if (show_promotion_modal) {
                    ImGui::OpenPopup("Promotion du Pion");
                }

                // bloque interface
                if (ImGui::BeginPopupModal("Promotion du Pion", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
                    ImGui::Text("Bravo ! Choisissez en quoi votre pion se transforme :");
                    ImGui::Separator();
                    
                    // (On a inversé le tour à la fin du mouvement, donc la pièce est de l'équipe inverse du tour actuel)
                    bool is_white = !myBoard.white_to_play;

                    if (ImGui::Button("Dame (D)", ImVec2(120, 40))) {
                        myBoard.get_board()[promotion_square.first][promotion_square.second] = std::make_unique<Queen>(is_white);
                        show_promotion_modal = false;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Tour (T)", ImVec2(120, 40))) {
                        myBoard.get_board()[promotion_square.first][promotion_square.second] = std::make_unique<Rook>(is_white);
                        show_promotion_modal = false;
                    }
                    
                    if (ImGui::Button("Cavalier (C)", ImVec2(120, 40))) {
                        myBoard.get_board()[promotion_square.first][promotion_square.second] = std::make_unique<Knight>(is_white);
                        show_promotion_modal = false;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Fou (F)", ImVec2(120, 40))) {
                        myBoard.get_board()[promotion_square.first][promotion_square.second] = std::make_unique<Bishop>(is_white);
                        show_promotion_modal = false;
                    }

                    // Si on a cliqué sur un bouton, show_promotion_modal est passé à false, on ferme la popup
                    if (!show_promotion_modal) {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
                // ---------------------------------------------------
            } 
        }
    );
    return 0;
}