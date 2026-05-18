#ifndef UI_CFG_H
#define UI_CFG_H

#include "imgui.h"
#include "imgui_internal.h"
#include "MelonImGui.h"

namespace ui
{
    constexpr auto g_kChromeBg = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
    constexpr auto g_kChromeActive = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
    constexpr auto kAccent = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    constexpr int g_kWindowChromeColorCount = 7;

    inline void apply_neutral_window_chrome(ImVec4* colors)
    {
        colors[ImGuiCol_TitleBg] = g_kChromeBg;
        colors[ImGuiCol_TitleBgActive] = g_kChromeActive;
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
        colors[ImGuiCol_Tab] = g_kChromeBg;
        colors[ImGuiCol_TabHovered] = g_kChromeActive;
        colors[ImGuiCol_TabSelected] = g_kChromeActive;
        colors[ImGuiCol_TabDimmed] = g_kChromeBg;
        colors[ImGuiCol_TabDimmedSelected] = g_kChromeActive;
        colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
        colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    }

    inline void derive_docking_tab_colors(ImVec4* colors)
    {
        apply_neutral_window_chrome(colors);

        colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
        colors[ImGuiCol_SeparatorHovered] = colors[ImGuiCol_HeaderHovered];
        colors[ImGuiCol_SeparatorActive] = colors[ImGuiCol_HeaderActive];
        colors[ImGuiCol_CheckboxSelectedBg] = ImLerp(colors[ImGuiCol_FrameBg], colors[ImGuiCol_FrameBgHovered], 0.65f);
        colors[ImGuiCol_DockingPreview] = ImVec4(kAccent.x, kAccent.y, kAccent.z, kAccent.w * 0.70f);
        colors[ImGuiCol_DockingEmptyBg] = colors[ImGuiCol_WindowBg];
        colors[ImGuiCol_TreeLines] = colors[ImGuiCol_Border];
        colors[ImGuiCol_DragDropTarget] = ImVec4(kAccent.x, kAccent.y, kAccent.z, 0.95f);
        colors[ImGuiCol_NavCursor] = colors[ImGuiCol_HeaderHovered];
    }


inline void init_imgui_styles(const bool bStyleDark_,const float _alpha) {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    ImGui::StyleColorsLight(&style);

    style.Alpha = 1.0f;
    style.FrameRounding = 3.0f;
    style.WindowBorderSize = 1.0f;
    style.DockingSeparatorSize = 2.0f;

    colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
    colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
    colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
    colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);

    derive_docking_tab_colors(colors);

    if (bStyleDark_)
    {
        for (auto & col : style.Colors)
        {
            float H, S, V;
            ImGui::ColorConvertRGBtoHSV(col.x, col.y, col.z, H, S, V);

            if (S < 0.1f)
                V = 1.0f - V;

            ImGui::ColorConvertHSVtoRGB(H, S, V, col.x, col.y, col.z);

            if (col.w < 1.0f)
                col.w *= _alpha;
        }
    }
    else
    {
        for (auto & col : style.Colors)
        {
            if (col.w < 1.0f)
            {
                col.x *= _alpha;
                col.y *= _alpha;
                col.z *= _alpha;
                col.w *= _alpha;
            }
        }
    }

    ui::apply_neutral_window_chrome(colors);
}

inline int push_window_chrome_style()
{
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ui::g_kChromeBg);
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ui::g_kChromeActive);
    ImGui::PushStyleColor(ImGuiCol_Tab, ui::g_kChromeBg);
    ImGui::PushStyleColor(ImGuiCol_TabHovered, ui::g_kChromeActive);
    ImGui::PushStyleColor(ImGuiCol_TabSelected, ui::g_kChromeActive);
    ImGui::PushStyleColor(ImGuiCol_TabDimmed, ui::g_kChromeBg);
    ImGui::PushStyleColor(ImGuiCol_TabDimmedSelected, ui::g_kChromeActive);
    return ui::g_kWindowChromeColorCount;
}

inline void pop_window_chrome_style(const int color_count)
{
    ImGui::PopStyleColor(color_count);
}

}

#endif // UI_CFG_H