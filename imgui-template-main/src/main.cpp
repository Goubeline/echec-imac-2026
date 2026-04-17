#include "quick_imgui/quick_imgui.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <optional>
#include <memory>



// logic
#include "logic/chessboard.hpp"
#include "logic/king.hpp"
#include "logic/pawn.hpp"
#include "logic/queen.hpp"
#include "logic/rook.hpp"
#include "logic/bishop.hpp"
#include "logic/knight.hpp"

// design
#include "design/renderer/BoardRenderer.hpp"
#include "design/renderer/PieceRenderer.hpp" 
#include "design/renderer/SkyboxRenderer.hpp"
#include "design/renderer/AnimationManager.hpp"

#include "design/core/TrackballCamera.hpp"
#include "design/core/PieceCamera.hpp" 




bool getGridFromMouse(float mx, float my, float sw, float sh, const glm::mat4& v, const glm::mat4& p, int& c, int& r) {// [3.6] Calcul d'intersection rayon/plan pour la sélection
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
    std::unique_ptr<SkyboxRenderer> skyboxRenderer;
    std::unique_ptr<TrackballCamera> camera;
    std::unique_ptr<PieceCamera> pieceCamera; 
    std::unique_ptr<Chessboard> myBoard;
    std::unique_ptr<AnimationManager> anim;

    std::optional<std::pair<int, int>> selected_square;
    std::vector<std::pair<int, int>> possible_moves;
    std::optional<std::string> winner_message;
    
    bool is_check = false;
    bool show_promotion_modal = false;
    std::pair<int, int> promotion_square = {-1, -1};

    float dayNightFactor = 0.0f; 
    bool use_piece_camera = false; 

    quick_imgui::loop("Chess 3D Chaos", {
        .init = [&]() {
            glEnable(GL_DEPTH_TEST);
            
            boardRenderer = std::make_unique<BoardRenderer>();
            pieceRenderer = std::make_unique<PieceRenderer>();
            skyboxRenderer = std::make_unique<SkyboxRenderer>();
            
            camera = std::make_unique<TrackballCamera>();
            pieceCamera = std::make_unique<PieceCamera>(); 
            myBoard = std::make_unique<Chessboard>();
            anim = std::make_unique<AnimationManager>();
        },
        .loop = [&]() {
            ImGuiIO& io = ImGui::GetIO();
            float dt = io.DeltaTime;

            // --- GESTION DU JOUR / NUIT ---
            if (myBoard->white_to_play) dayNightFactor -= dt * 0.6f; 
            else dayNightFactor += dt * 0.6f; 
            dayNightFactor = std::clamp(dayNightFactor, 0.0f, 1.0f);

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            anim->update(dt);

            // --- [3.5] CONTRÔLES DES CAMÉRAS ---
            if (!io.WantCaptureMouse) {
                if (ImGui::IsMouseDragging(1)) { // Clic droit
                    if (use_piece_camera) {
                        pieceCamera->rotateLeft(io.MouseDelta.x * 0.3f); 
                        pieceCamera->rotateUp(io.MouseDelta.y * 0.3f); 
                    } else {
                        camera->rotateLeft(io.MouseDelta.x * 0.5f); 
                        camera->rotateUp(io.MouseDelta.y * 0.5f); 
                    }
                }
                if (!use_piece_camera && io.MouseWheel != 0.0f) {
                    camera->zoom(io.MouseWheel * 1.5f);
                }
            }

            // Calcul des matrices
            glm::mat4 proj = glm::perspective(glm::radians(70.f), io.DisplaySize.x / io.DisplaySize.y, 0.1f, 200.f);
            
            // --- VUE PDV PIÈCE ---
            if (use_piece_camera && selected_square) {
                pieceCamera->setPosition(glm::vec3(selected_square->first, 1.3f, selected_square->second));
            }
            
            glm::mat4 view = use_piece_camera ? pieceCamera->getViewMatrix() : camera->getViewMatrix();

            // --- RENDU 3D ---
            // 1. Skybox (doit utiliser la même matrice de vue pour tourner avec la pièce)
           skyboxRenderer->render(view, proj, dayNightFactor);

            // 2. Plateau
            int hoverC = -1, hoverR = -1;
            getGridFromMouse(io.MousePos.x, io.MousePos.y, io.DisplaySize.x, io.DisplaySize.y, view, proj, hoverC, hoverR);
            
            boardRenderer->setLighting(view, dayNightFactor);
            boardRenderer->render(view, proj, 
                                  selected_square ? selected_square->first : -1, 
                                  selected_square ? selected_square->second : -1, 
                                  possible_moves, hoverC, hoverR);

            // 3. Pièces
            pieceRenderer->setLighting(view, dayNightFactor);
            pieceRenderer->render(*myBoard, view, proj, *anim);

            // --- LOGIQUE DE SÉLECTION ---
            if (!winner_message && !show_promotion_modal && ImGui::IsMouseClicked(0) && !io.WantCaptureMouse) {
                int c, r;
                if (getGridFromMouse(io.MousePos.x, io.MousePos.y, io.DisplaySize.x, io.DisplaySize.y, view, proj, c, r)) {
                    if (!selected_square) {
                        if (myBoard->get_board()[c][r] && myBoard->get_board()[c][r]->i_am_white == myBoard->white_to_play) {
                            selected_square = {c, r};
                            possible_moves = myBoard->select_piece(*selected_square);
                        }
                    } else { 
                        if (std::any_of(possible_moves.begin(), possible_moves.end(), [&](auto& m) { return m.first == c && m.second == r; })) {
                            auto& grid = myBoard->get_board();
                            std::unique_ptr<Piece> capturedPiece = nullptr;
                            if (grid[c][r] != nullptr) capturedPiece = std::move(grid[c][r]);

                            anim->start(glm::vec3(selected_square->first, 0.0f, selected_square->second), 
                                        glm::vec3(c, 0.0f, r), c, r, std::move(capturedPiece));
                            
                            if(!myBoard->movement(*selected_square, {c, r}))
                            {
                                is_check = true;
                            }
                            else{
                                is_check = false;
                            }
                            
                            if (dynamic_cast<Pawn*>(grid[c][r].get()) && (r == 0 || r == 7)) {
                                promotion_square = {c, r};
                                show_promotion_modal = true;
                            } else {
                                myBoard->white_to_play = !myBoard->white_to_play;
                            }
                        }
                        selected_square.reset(); 
                        possible_moves.clear();
                        use_piece_camera = false; // On quitte la vue FPS après le mouvement
                    }
                }
            }

            // --- INTERFACE IMGUI ("Infos") ---
            ImGui::Begin("Infos", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
                ImGui::Text("Tour : %s", myBoard->white_to_play ? "BLANCS" : "NOIRS");
                if (is_check)
                {
                    ImGui::TextColored({1, 0, 0, 1}, "Roi en echec");
                }
                if (selected_square) {
                    ImGui::TextColored({0, 1, 0, 1}, "Piece selectionnee");
                    ImGui::Separator();
                    ImGui::Checkbox("Vue depuis la piece (FPS)", &use_piece_camera);
                } else {
                    use_piece_camera = false; 
                }
            ImGui::End();

            // Promotion Modal
            if (show_promotion_modal) {
                ImGui::OpenPopup("Promotion");
                if (ImGui::BeginPopupModal("Promotion", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                    bool is_white = myBoard->white_to_play; 
                    auto& p = myBoard->get_board()[promotion_square.first][promotion_square.second];
                    if (ImGui::Button("Reine")) { p = std::make_unique<Queen>(is_white);  show_promotion_modal = false; }
                    if (ImGui::Button("Tour"))  { p = std::make_unique<Rook>(is_white);   show_promotion_modal = false; }
                    if (ImGui::Button("Cavalier"))  { p = std::make_unique<Bishop>(is_white);   show_promotion_modal = false; }
                    if (!show_promotion_modal) { 
                        myBoard->white_to_play = !myBoard->white_to_play; 
                        ImGui::CloseCurrentPopup(); 
                    }
                    ImGui::EndPopup();
                }
            }
        }
    });
    return 0;
}