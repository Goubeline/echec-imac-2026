#include "chessUI.hpp"
#include <imgui.h>
#include <iostream>

void ChessUI::DrawBoard(const Board& board) {
    // les colors 
    ImVec4 color_light = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    ImVec4 color_dark  = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            
            ImGui::PushID(row * 8 + col); 

            bool is_light_square = (row + col) % 2 == 0;
            ImGui::PushStyleColor(ImGuiCol_Button, is_light_square ? color_light : color_dark);

            // ici pour les pièces
            if (ImGui::Button("Carré", ImVec2(50.f, 50.f))) {
                std::cout << "Click case: " << row << ", " << col << "\n";
            }

            ImGui::PopStyleColor();
            ImGui::PopID();

            if (col < 7) ImGui::SameLine();
        }
    }
    ImGui::PopStyleVar();
}