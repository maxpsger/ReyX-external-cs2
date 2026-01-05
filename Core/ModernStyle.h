#pragma once
#include "../OS-ImGui/imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../OS-ImGui/imgui/imgui_internal.h"
#include <unordered_map>

namespace ModernStyle
{
    inline ImVec4 HexToRGBA(int hex, float alpha = 1.0f)
    {
        return ImVec4(
            ((hex >> 16) & 0xFF) / 255.0f,
            ((hex >> 8) & 0xFF) / 255.0f,
            (hex & 0xFF) / 255.0f,
            alpha
        );
    }

    namespace Colors
    {
        constexpr int Background = 0x0A0A0F;
        constexpr int ChildBackground = 0x141419;
        constexpr int Border = 0x2A2A35;
        constexpr int Text = 0xFFFFFF;
        constexpr int TextDim = 0x8A8F9A;

        // Primary ersetzt Lila durch Rot
        constexpr int Primary = 0xFF4757;
        constexpr int PrimaryHover = 0xFF6A76;
        constexpr int PrimaryActive = 0xE63C48;

        constexpr int Secondary = 0x00D4FF;
        constexpr int Success = 0x00FF88;
        constexpr int Warning = 0xFFB800;
        constexpr int Danger = 0xFF4757;

        constexpr int FrameBg = 0x1F1F28;
        constexpr int FrameBgHover = 0x2A2A35;
        constexpr int FrameBgActive = 0x35354A;
        constexpr int Header = 0x2A2A35;
        constexpr int HeaderHover = 0x35354A;
        constexpr int HeaderActive = 0x404060;
    }

    inline void ApplyModernStyle()
    {
        ImGuiStyle& style = ImGui::GetStyle();

        style.WindowPadding = ImVec2(0, 0);
        style.WindowRounding = 10.0f;
        style.WindowBorderSize = 0.0f;
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

        style.ChildRounding = 8.0f;
        style.ChildBorderSize = 1.0f;

        style.PopupRounding = 6.0f;
        style.PopupBorderSize = 1.0f;

        style.FramePadding = ImVec2(14, 6);
        style.FrameRounding = 4.0f;
        style.FrameBorderSize = 0.0f;

        style.ItemSpacing = ImVec2(12, 6);
        style.ItemInnerSpacing = ImVec2(8, 4);
        style.IndentSpacing = 20.0f;

        style.ScrollbarSize = 12.0f;
        style.ScrollbarRounding = 12.0f;

        style.GrabMinSize = 10.0f;
        style.GrabRounding = 4.0f;

        style.TabRounding = 6.0f;
        style.TabBorderSize = 0.0f;

        style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
        style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

        ImVec4* colors = style.Colors;

        colors[ImGuiCol_Text] = HexToRGBA(Colors::Text);
        colors[ImGuiCol_TextDisabled] = HexToRGBA(Colors::TextDim, 0.6f);
        colors[ImGuiCol_WindowBg] = HexToRGBA(Colors::Background);
        colors[ImGuiCol_ChildBg] = HexToRGBA(Colors::ChildBackground);
        colors[ImGuiCol_PopupBg] = HexToRGBA(Colors::Background, 0.95f);
        colors[ImGuiCol_Border] = HexToRGBA(Colors::Border, 0.3f);
        colors[ImGuiCol_BorderShadow] = HexToRGBA(0x000000, 0.0f);
        colors[ImGuiCol_FrameBg] = HexToRGBA(Colors::FrameBg);
        colors[ImGuiCol_FrameBgHovered] = HexToRGBA(Colors::FrameBgHover);
        colors[ImGuiCol_FrameBgActive] = HexToRGBA(Colors::FrameBgActive);
        colors[ImGuiCol_TitleBg] = HexToRGBA(Colors::ChildBackground);
        colors[ImGuiCol_TitleBgActive] = HexToRGBA(Colors::ChildBackground);
        colors[ImGuiCol_TitleBgCollapsed] = HexToRGBA(Colors::ChildBackground, 0.5f);
        colors[ImGuiCol_MenuBarBg] = HexToRGBA(Colors::ChildBackground);
        colors[ImGuiCol_ScrollbarBg] = HexToRGBA(Colors::ChildBackground, 0.0f);
        colors[ImGuiCol_ScrollbarGrab] = HexToRGBA(Colors::Border);
        colors[ImGuiCol_ScrollbarGrabHovered] = HexToRGBA(Colors::HeaderHover);
        colors[ImGuiCol_ScrollbarGrabActive] = HexToRGBA(Colors::HeaderActive);
        colors[ImGuiCol_CheckMark] = HexToRGBA(Colors::Primary);
        colors[ImGuiCol_SliderGrab] = HexToRGBA(Colors::Primary);
        colors[ImGuiCol_SliderGrabActive] = HexToRGBA(Colors::PrimaryActive);
        colors[ImGuiCol_Button] = HexToRGBA(Colors::FrameBg);
        colors[ImGuiCol_ButtonHovered] = HexToRGBA(Colors::Primary, 0.8f);
        colors[ImGuiCol_ButtonActive] = HexToRGBA(Colors::PrimaryActive);
        colors[ImGuiCol_Header] = HexToRGBA(Colors::Header, 0.7f);
        colors[ImGuiCol_HeaderHovered] = HexToRGBA(Colors::HeaderHover, 0.9f);
        colors[ImGuiCol_HeaderActive] = HexToRGBA(Colors::HeaderActive);
        colors[ImGuiCol_Separator] = HexToRGBA(Colors::Border, 0.2f);
        colors[ImGuiCol_SeparatorHovered] = HexToRGBA(Colors::Primary, 0.5f);
        colors[ImGuiCol_SeparatorActive] = HexToRGBA(Colors::Primary, 0.8f);
        colors[ImGuiCol_ResizeGrip] = HexToRGBA(Colors::Border, 0.2f);
        colors[ImGuiCol_ResizeGripHovered] = HexToRGBA(Colors::Primary, 0.5f);
        colors[ImGuiCol_ResizeGripActive] = HexToRGBA(Colors::Primary, 0.8f);
        colors[ImGuiCol_Tab] = HexToRGBA(Colors::Header, 0.7f);
        colors[ImGuiCol_TabHovered] = HexToRGBA(Colors::HeaderHover);
        colors[ImGuiCol_TabActive] = HexToRGBA(Colors::Primary);
        colors[ImGuiCol_TabUnfocused] = HexToRGBA(Colors::Header, 0.5f);
        colors[ImGuiCol_TabUnfocusedActive] = HexToRGBA(Colors::HeaderActive, 0.7f);
        colors[ImGuiCol_PlotLines] = HexToRGBA(Colors::Primary);
        colors[ImGuiCol_PlotLinesHovered] = HexToRGBA(Colors::PrimaryHover);
        colors[ImGuiCol_PlotHistogram] = HexToRGBA(Colors::Primary);
        colors[ImGuiCol_PlotHistogramHovered] = HexToRGBA(Colors::PrimaryHover);
        colors[ImGuiCol_TableHeaderBg] = HexToRGBA(Colors::Header, 0.7f);
        colors[ImGuiCol_TableBorderStrong] = HexToRGBA(Colors::Border, 0.8f);
        colors[ImGuiCol_TableBorderLight] = HexToRGBA(Colors::Border, 0.4f);
        colors[ImGuiCol_TableRowBg] = HexToRGBA(0x000000, 0.0f);
        colors[ImGuiCol_TableRowBgAlt] = HexToRGBA(Colors::Text, 0.03f);
        colors[ImGuiCol_TextSelectedBg] = HexToRGBA(Colors::Primary, 0.35f);
        colors[ImGuiCol_DragDropTarget] = HexToRGBA(Colors::Secondary);
        colors[ImGuiCol_NavHighlight] = HexToRGBA(Colors::Primary);
        colors[ImGuiCol_NavWindowingHighlight] = HexToRGBA(Colors::Primary, 0.7f);
        colors[ImGuiCol_NavWindowingDimBg] = HexToRGBA(0x000000, 0.2f);
        colors[ImGuiCol_ModalWindowDimBg] = HexToRGBA(0x000000, 0.35f);
    }

    inline void RenderTabButton(const char* label, int tab_id, int* current_tab)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size = ImVec2(label_size.x + 40, 35);  // Dynamic width based on text

        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ImGui::ItemSize(size, style.FramePadding.y);
        if (!ImGui::ItemAdd(bb, id))
            return;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
        if (pressed)
            *current_tab = tab_id;

        bool is_active = (*current_tab == tab_id);

        ImDrawList* draw_list = window->DrawList;

        // Background
        if (is_active)
        {
            draw_list->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(HexToRGBA(Colors::Primary)), 4.0f);
        }
        else if (hovered)
        {
            draw_list->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(HexToRGBA(Colors::FrameBgHover)), 4.0f);
        }
        else
        {
            draw_list->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(HexToRGBA(Colors::FrameBg)), 4.0f);
        }

        // Text
        const ImU32 text_col = ImGui::GetColorU32(is_active ? ImGuiCol_Text : ImGuiCol_TextDisabled);
        window->DrawList->AddText(ImVec2(bb.Min.x + size.x * 0.5f - label_size.x * 0.5f, bb.Min.y + size.y * 0.5f - label_size.y * 0.5f), 
                                  text_col, label);
    }

    inline void BeginSection(const char* label)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
        ImGui::Spacing();
        
        // Section header
        ImVec2 text_size = ImGui::CalcTextSize(label);
        ImVec2 pos = ImGui::GetCursorScreenPos();
        
        window->DrawList->AddText(ImVec2(pos.x, pos.y), ImGui::GetColorU32(HexToRGBA(Colors::Primary)), label);
        
        ImGui::Dummy(ImVec2(0, text_size.y + 8));
        
        // Separator line
        ImVec2 line_start = ImVec2(pos.x, pos.y + text_size.y + 4);
        ImVec2 line_end = ImVec2(pos.x + ImGui::GetContentRegionAvail().x, pos.y + text_size.y + 4);
        window->DrawList->AddLine(line_start, line_end, ImGui::GetColorU32(HexToRGBA(Colors::Border, 0.3f)), 1.0f);
        
        ImGui::Spacing();
    }

    inline void EndSection()
    {
        ImGui::PopStyleVar();
        ImGui::Spacing();
        ImGui::Spacing();
    }

    inline bool ToggleSwitch(const char* label, bool* v)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

        const float height = 20.0f;
        const float width = 40.0f;

        const ImVec2 pos = window->DC.CursorPos;
        
        // Label on the left
        window->DrawList->AddText(pos, ImGui::GetColorU32(ImGuiCol_Text), label);
        
        // Switch on the right
        float switch_x = pos.x + ImGui::GetContentRegionAvail().x - width;
        const ImRect total_bb(ImVec2(switch_x, pos.y), ImVec2(switch_x + width, pos.y + height));

        ImGui::ItemSize(ImRect(pos, ImVec2(pos.x + ImGui::GetContentRegionAvail().x, pos.y + height)), style.FramePadding.y);
        if (!ImGui::ItemAdd(total_bb, id))
            return false;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
        if (pressed)
            *v = !*v;

        // Animation
        static std::unordered_map<ImGuiID, float> anim_map;
        auto it = anim_map.find(id);
        if (it == anim_map.end())
        {
            anim_map.insert({ id, *v ? 1.0f : 0.0f });
            it = anim_map.find(id);
        }
        
        float& t = it->second;
        if (*v)
            t += g.IO.DeltaTime * 6.0f;
        else
            t -= g.IO.DeltaTime * 6.0f;
        t = ImClamp(t, 0.0f, 1.0f);
        
        ImU32 col_bg = ImGui::GetColorU32(HexToRGBA(hovered ? Colors::FrameBgHover : Colors::FrameBg));
        ImU32 col_active = ImGui::GetColorU32(HexToRGBA(Colors::Primary));
        
        window->DrawList->AddRectFilled(total_bb.Min, total_bb.Max, col_bg, height * 0.5f);
        
        ImVec2 knob_pos = ImVec2(total_bb.Min.x + 2 + (width - height + 4) * t, total_bb.Min.y + 2);
        ImVec2 knob_size = ImVec2(height - 4, height - 4);
        
        if (t > 0.0f)
        {
            ImVec2 fill_size = ImVec2((width - 4) * t, height - 4);
            window->DrawList->AddRectFilled(ImVec2(total_bb.Min.x + 2, total_bb.Min.y + 2), 
                                           ImVec2(total_bb.Min.x + 2 + fill_size.x, total_bb.Min.y + 2 + fill_size.y), 
                                           col_active, (height - 4) * 0.5f);
        }
        
        window->DrawList->AddRectFilled(knob_pos, ImVec2(knob_pos.x + knob_size.x, knob_pos.y + knob_size.y), 
                                       IM_COL32(255, 255, 255, 255), knob_size.y * 0.5f);

        return pressed;
    }

    inline bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.1f")
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);

        const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
        const ImVec2 pos = window->DC.CursorPos;
        
        // Label
        window->DrawList->AddText(pos, ImGui::GetColorU32(ImGuiCol_Text), label);
        
        // Value text
        char value_text[64];
        ImFormatString(value_text, IM_ARRAYSIZE(value_text), format, *v);
        ImVec2 value_size = ImGui::CalcTextSize(value_text);
        window->DrawList->AddText(ImVec2(pos.x + ImGui::GetContentRegionAvail().x - value_size.x, pos.y), 
                                 ImGui::GetColorU32(HexToRGBA(Colors::TextDim)), value_text);
        
        // Slider
        ImVec2 slider_pos = ImVec2(pos.x, pos.y + label_size.y + 4);
        ImVec2 slider_size = ImVec2(ImGui::GetContentRegionAvail().x, 6);
        const ImRect bb(slider_pos, ImVec2(slider_pos.x + slider_size.x, slider_pos.y + slider_size.y));
        
        ImGui::ItemSize(ImRect(pos, ImVec2(pos.x + ImGui::GetContentRegionAvail().x, pos.y + label_size.y + slider_size.y + 8)));
        if (!ImGui::ItemAdd(bb, id))
            return false;

        const bool hovered = ImGui::ItemHoverable(bb, id);
        bool temp_input_start = false;
        bool value_changed = false;

        if (hovered && g.IO.MouseClicked[0])
        {
            ImGui::SetActiveID(id, window);
            ImGui::FocusWindow(window);
            temp_input_start = true;
        }

        if (g.ActiveId == id)
        {
            if (g.IO.MouseDown[0])
            {
                float mouse_x = g.IO.MousePos.x - bb.Min.x;
                float t = ImClamp(mouse_x / bb.GetWidth(), 0.0f, 1.0f);
                float new_value = ImLerp(v_min, v_max, t);
                if (*v != new_value)
                {
                    *v = new_value;
                    value_changed = true;
                }
            }
            else
            {
                ImGui::ClearActiveID();
            }
        }

        // Render slider
        float grab_t = (*v - v_min) / (v_max - v_min);
        float grab_pos = ImLerp(bb.Min.x, bb.Max.x, grab_t);

        window->DrawList->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(HexToRGBA(Colors::FrameBg)), 3.0f);
        
        if (grab_t > 0.0f)
        {
            window->DrawList->AddRectFilled(bb.Min, ImVec2(grab_pos, bb.Max.y), ImGui::GetColorU32(HexToRGBA(Colors::Primary)), 3.0f);
        }
        
        window->DrawList->AddCircleFilled(ImVec2(grab_pos, bb.GetCenter().y), 8.0f, 
                                         ImGui::GetColorU32(g.ActiveId == id ? HexToRGBA(Colors::PrimaryActive) : HexToRGBA(Colors::Primary)));

        return value_changed;
    }

    inline bool SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d")
    {
        float v_f = (float)*v;
        bool changed = SliderFloat(label, &v_f, (float)v_min, (float)v_max, "%.0f");
        *v = (int)v_f;
        return changed;
    }

    inline void DrawGradientText(const char* text, ImVec2 pos, ImU32 col1, ImU32 col2)
    {
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
    }

    inline void RenderTab(const char* label, bool* selected)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size = ImVec2(label_size.x + style.FramePadding.x * 3.0f, label_size.y + style.FramePadding.y * 2.0f);

        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ImGui::ItemSize(size, style.FramePadding.y);
        if (!ImGui::ItemAdd(bb, id))
            return;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
        if (pressed)
            *selected = true;

        ImDrawList* draw_list = window->DrawList;

        if (*selected)
        {
            draw_list->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_TabActive), style.TabRounding);
            draw_list->AddLine(ImVec2(bb.Min.x, bb.Max.y), ImVec2(bb.Max.x, bb.Max.y), ImGui::GetColorU32(ImGuiCol_TabActive), 3.0f);
        }
        else if (hovered)
        {
            draw_list->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_TabHovered), style.TabRounding);
        }

        const ImU32 text_col = ImGui::GetColorU32(*selected ? ImGuiCol_Text : ImGuiCol_TextDisabled);
        ImGui::RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x, bb.Min.y + style.FramePadding.y), 
                                ImVec2(bb.Max.x - style.FramePadding.x, bb.Max.y - style.FramePadding.y), 
                                label, NULL, &label_size, ImVec2(0.5f, 0.5f), &bb);
    }

    inline void BeginGroupBox(const char* label, const ImVec2& size = ImVec2(0, 0))
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        
        ImGui::BeginGroup();
        
        ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size_arg = ImGui::CalcItemSize(size, 100, label_size.y + style.FramePadding.y * 2);
        
        ImRect bb(pos, ImVec2(pos.x + size_arg.x, pos.y + size_arg.y));
        ImGui::ItemSize(bb);
        
        ImDrawList* draw_list = window->DrawList;
        
        float border_size = 1.0f;
        draw_list->AddRect(ImVec2(bb.Min.x + 1, bb.Min.y + label_size.y * 0.5f), bb.Max, ImGui::GetColorU32(ImGuiCol_Border), style.FrameRounding, 0, border_size);
        
        ImVec2 text_pos = ImVec2(pos.x + style.FramePadding.x + 8, pos.y);
        ImRect text_bb(ImVec2(text_pos.x - 4, text_pos.y), ImVec2(text_pos.x + label_size.x + 4, text_pos.y + label_size.y));
        draw_list->AddRectFilled(text_bb.Min, text_bb.Max, ImGui::GetColorU32(ImGuiCol_WindowBg));
        
        ImGui::RenderText(text_pos, label);
        
        window->DC.CursorPos = ImVec2(pos.x + style.FramePadding.x, pos.y + label_size.y + style.ItemSpacing.y);
        ImGui::PushID(label);
    }

    inline void EndGroupBox()
    {
        ImGui::PopID();
        ImGui::EndGroup();
    }
}