#pragma once
#include "../OS-ImGui/imgui/imgui.h"

namespace ImGui
{
    static inline void GradientText(const char* text)
    {
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImU32 col1 = IM_COL32(123, 92, 255, 255);  // Purple
        ImU32 col2 = IM_COL32(0, 212, 255, 255);   // Cyan
        
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 textSize = ImGui::CalcTextSize(text);
        
        for (int i = 0; i < strlen(text); i++)
        {
            char c[2] = { text[i], '\0' };
            float t = (float)i / (float)strlen(text);
            ImU32 col = IM_COL32(
                (int)((1 - t) * ((col1 >> 16) & 0xFF) + t * ((col2 >> 16) & 0xFF)),
                (int)((1 - t) * ((col1 >> 8) & 0xFF) + t * ((col2 >> 8) & 0xFF)),
                (int)((1 - t) * (col1 & 0xFF) + t * (col2 & 0xFF)),
                255
            );
            
            drawList->AddText(pos, col, c);
            pos.x += ImGui::CalcTextSize(c).x;
        }
        
        ImGui::Dummy(textSize);
    }
}
