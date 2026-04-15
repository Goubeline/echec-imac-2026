#include "quick_imgui/quick_imgui.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <optional>
#include <memory>

// Logique & Design
#include "logic/chessboard.hpp"
#include "logic/king.hpp"
#include "logic/pawn.hpp"
#include "logic/queen.hpp"
#include "logic/rook.hpp"
#include "logic/bishop.hpp"
#include "logic/knight.hpp"
#include "design/BoardRenderer.hpp"
#include "design/TrackballCamera.hpp"
#include "design/AnimationManager.hpp"

// Transformation clic souris -> grille
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
    std::unique_ptr<BoardRenderer> renderer;
    std::unique_ptr<TrackballCamera> camera;
    std::unique_ptr<Chessboard> myBoard; // Renommé selon Git
    std::unique_ptr<AnimationManager> anim;

    // Variables synchronisées avec le Git
    std::optional<std::pair<int, int>> selected_square;
    std::vector<std::pair<int, int>> possible_moves;
    std::optional<std::string> winner_message;
    
    bool show_promotion_modal = false;
    std::pair<int, int> promotion_square = {-1, -1};

    quick_imgui::loop("Chess 3D", {
        .init = [&]() {
            glEnable(GL_DEPTH_TEST);
            renderer = std::make_unique<BoardRenderer>();
            camera = std::make_unique<TrackballCamera>();
            myBoard = std::make_unique<Chessboard>();
            anim = std::make_unique<AnimationManager>();
        },
        .loop = [&]() {
            glClearColor(0.15f, 0.15f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            ImGuiIO& io = ImGui::GetIO();
            anim->update(io.DeltaTime);

            // 1. CAMERA
            if (!io.WantCaptureMouse) {
                if (ImGui::IsMouseDragging(1)) { camera->rotateLeft(io.MouseDelta.x * 0.5f); camera->rotateUp(io.MouseDelta.y * 0.5f); }
                if (io.MouseWheel != 0.0f) camera->zoom(io.MouseWheel * 1.5f);
            }

            glm::mat4 proj = glm::perspective(glm::radians(70.f), io.DisplaySize.x / io.DisplaySize.y, 0.1f, 100.f);
            glm::mat4 view = camera->getViewMatrix();

            // 2. GAMEPLAY
            if (!winner_message && !show_promotion_modal && ImGui::IsMouseClicked(0) && !io.WantCaptureMouse) {
                int c, r;
                if (!getGridFromMouse(io.MousePos.x, io.MousePos.y, io.DisplaySize.x, io.DisplaySize.y, view, proj, c, r)) {
                    selected_square.reset(); possible_moves.clear();
                } 
                else if (!selected_square) { // SELECTION
                    if (myBoard->get_board()[c][r] && myBoard->get_board()[c][r]->i_am_white == myBoard->white_to_play) {
                        selected_square = {c, r};
                        std::pair<int, int> pos = *selected_square;
                        possible_moves = myBoard->select_piece(pos);
                    }
                } 
                else { // MOUVEMENT
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

            // 3. RENDU
            renderer->render(*myBoard, view, proj, 
                             selected_square ? selected_square->first : -1, 
                             selected_square ? selected_square->second : -1, 
                             possible_moves, *anim);

            // 4. INTERFACE : INFOS
            ImGui::Begin("Infos", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
                ImGui::Text("Tour : %s", myBoard->white_to_play ? "BLANCS" : "NOIRS");
                if (selected_square) ImGui::TextColored({0, 1, 0, 1}, "Piece selectionnee");
            ImGui::End();

            // 5. INTERFACE : PROMOTION (Synchronisé avec la logique Git)
            if (show_promotion_modal) {
                ImGui::OpenPopup("Promotion");
                if (ImGui::BeginPopupModal("Promotion", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                    // On détermine la couleur (l'inverse du tour actuel car le pion vient de bouger)
                    bool is_white = !myBoard->white_to_play;
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

            // 6. INTERFACE : FIN DE PARTIE 
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