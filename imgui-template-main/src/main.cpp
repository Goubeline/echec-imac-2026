#include "quick_imgui/quick_imgui.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <optional>
#include <memory>

#include "logic/chessboard.hpp"
#include "logic/king.hpp"
#include "logic/pawn.hpp"
#include "logic/queen.hpp"
#include "logic/rook.hpp"
#include "logic/bishop.hpp"
#include "logic/knight.hpp"

#include "design/BoardRenderer.hpp"
#include "design/PieceRenderer.hpp" 
#include "design/TrackballCamera.hpp"
#include "design/AnimationManager.hpp"

// [SUJET 3.6] 1. RayCasting
bool getGridFromMouse(float mx, float my, float sw, float sh, const glm::mat4& v, const glm::mat4& p, int& c, int& r) {
    float x = (2.0f * mx) / sw - 1.0f;
    float y = 1.0f - (2.0f * my) / sh;
    glm::vec4 eye = glm::inverse(p) * glm::vec4(x, y, -1.0f, 1.0f);
    glm::vec3 dir = glm::normalize(glm::vec3(glm::inverse(v) * glm::vec4(eye.x, eye.y, -1.0f, 0.0f)));
    glm::vec3 pos = glm::vec3(glm::inverse(v)[3]);
    if (dir.y >= 0.0f) return false;
    glm::vec3 inter = pos + dir * (-pos.y / dir.y);
    c = (int)std::round(inter.x); r = (int)std::round(inter.z);
    return (c >= 0 && c < 8 && r >= 0 && r < 8);
}

int main() {
    
    std::unique_ptr<BoardRenderer> boardRenderer;
    std::unique_ptr<PieceRenderer> pieceRenderer;
    std::unique_ptr<TrackballCamera> camera;
    std::unique_ptr<Chessboard> myBoard;
    std::unique_ptr<AnimationManager> anim;

    std::optional<std::pair<int, int>> selected_square;
    std::vector<std::pair<int, int>> possible_moves;
    std::optional<std::string> winner_message;
    
    bool show_promotion_modal = false;
    std::pair<int, int> promotion_square = {-1, -1};

    quick_imgui::loop("Chess 3D", {
        .init = [&]() {
            glEnable(GL_DEPTH_TEST);
            
            // 2. INITIALISATIONS
            boardRenderer = std::make_unique<BoardRenderer>();
            pieceRenderer = std::make_unique<PieceRenderer>();
            camera = std::make_unique<TrackballCamera>();
            myBoard = std::make_unique<Chessboard>();
            anim = std::make_unique<AnimationManager>();
        },
        .loop = [&]() {
            // Couleur de fond de l'univers 3D
            glClearColor(0.15f, 0.15f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            ImGuiIO& io = ImGui::GetIO();
            
            // Mise à jour du temps pour les déplacements progressifs
            anim->update(io.DeltaTime);

            
            if (!io.WantCaptureMouse) {
                if (ImGui::IsMouseDragging(1)) { 
                    camera->rotateLeft(io.MouseDelta.x * 0.5f); 
                    camera->rotateUp(io.MouseDelta.y * 0.5f); 
                }
                if (io.MouseWheel != 0.0f) camera->zoom(io.MouseWheel * 1.5f);
            }

            glm::mat4 proj = glm::perspective(glm::radians(70.f), io.DisplaySize.x / io.DisplaySize.y, 0.1f, 100.f);
            glm::mat4 view = camera->getViewMatrix();

            // --- DÉTECTION DU SURVOL ---
            int hoverC = -1, hoverR = -1;
            if (!io.WantCaptureMouse) {
                getGridFromMouse(io.MousePos.x, io.MousePos.y, io.DisplaySize.x, io.DisplaySize.y, view, proj, hoverC, hoverR);
            }

            if (!winner_message && !show_promotion_modal && ImGui::IsMouseClicked(0) && !io.WantCaptureMouse) {
                int c, r;
                if (getGridFromMouse(io.MousePos.x, io.MousePos.y, io.DisplaySize.x, io.DisplaySize.y, view, proj, c, r)) {
                    
                    
                    if (!selected_square) { 
                        if (myBoard->get_board()[c][r] && myBoard->get_board()[c][r]->i_am_white == myBoard->white_to_play) {
                            selected_square = {c, r};
                            std::pair<int, int> pos = *selected_square;
                            possible_moves = myBoard->select_piece(pos);
                        }
                    } 
                    else { // Mouvement quand la pièce est déplacée
                        if (std::any_of(possible_moves.begin(), possible_moves.end(), [&](auto& m) { return m.first == c && m.second == r; })) {
                            auto& grid = myBoard->get_board();
                            if (dynamic_cast<King*>(grid[c][r].get())) 
                                winner_message = myBoard->white_to_play ? "Les Blancs gagnent !" : "Les Noirs gagnent !";

                            bool is_pawn = dynamic_cast<Pawn*>(grid[selected_square->first][selected_square->second].get()) != nullptr;
                            
                            anim->start(glm::vec3(selected_square->first, 0.5f, selected_square->second), glm::vec3(c, 0.5f, r), c, r);
                            myBoard->movement(*selected_square, {c, r});
                            
                            if (is_pawn && (r == 0 || r == 7)) {
                                promotion_square = {c, r};
                                show_promotion_modal = true;
                            }
                            else {
                                myBoard->white_to_play = !myBoard->white_to_play;
                            }
                        }
                        selected_square.reset(); possible_moves.clear();
                    }
                }
            }

            // --- 3. RENDU SÉPARÉ (Plateau puis Pièces) ---
            boardRenderer->render(view, proj, 
                                  selected_square ? selected_square->first : -1, 
                                  selected_square ? selected_square->second : -1, 
                                  possible_moves, hoverC, hoverR);

            pieceRenderer->render(*myBoard, view, proj, *anim);


            //--- 4. Interface utilisateur ---
            ImGui::Begin("Infos", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
                ImGui::Text("Tour : %s", myBoard->white_to_play ? "BLANCS" : "NOIRS");
                if (selected_square) ImGui::TextColored({0, 1, 0, 1}, "Piece selectionnee");
            ImGui::End();

            // Menu de promotion 
            if (show_promotion_modal) {
                ImGui::OpenPopup("Promotion");
                if (ImGui::BeginPopupModal("Promotion", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                    bool is_white = myBoard->white_to_play; 
                    auto& p = myBoard->get_board()[promotion_square.first][promotion_square.second];

                    if (ImGui::Button("Reine (Q)", {100, 40})) { p = std::make_unique<Queen>(is_white);  show_promotion_modal = false; } ImGui::SameLine();
                    if (ImGui::Button("Tour (R)", {100, 40}))  { p = std::make_unique<Rook>(is_white);   show_promotion_modal = false; } ImGui::SameLine();
                    if (ImGui::Button("Fou (F)", {100, 40}))   { p = std::make_unique<Bishop>(is_white); show_promotion_modal = false; } ImGui::SameLine();
                    if (ImGui::Button("Cavalier (N)", {100, 40})) { p = std::make_unique<Knight>(is_white); show_promotion_modal = false; }

                    if (!show_promotion_modal) { 
                        myBoard->white_to_play = !myBoard->white_to_play;
                        ImGui::CloseCurrentPopup(); 
                    }
                    ImGui::EndPopup();
                }
            }

            // Écran de fin
            if (winner_message) {
                ImGui::Begin("FIN DE PARTIE", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
                    ImGui::Text("%s", winner_message->c_str());
                    ImGui::Spacing();
                    if (ImGui::Button("Quitter le jeu", {200, 50})) exit(0);
                ImGui::End();
            }
        }
    });
    return 0;
}