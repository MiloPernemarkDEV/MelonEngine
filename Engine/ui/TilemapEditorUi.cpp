#include "TilemapEditorUi.h"
#include "MelonImGui.h"
#include "uicfg.h"

// the tilemap editor should display the spritesheet and the available assets
// functionality include: moving assets into a tile, see a spritesheet, preview of assets
// menu features: save, load,
void ui::draw_tilemap_editor_ui(const char *title) {
    bool& isOpen = MelonImGui::window_open(title);
    if (!isOpen) {
        return;
    }

    const int chrome_colors = ui::push_window_chrome_style();
    if (const ui::window tilemap(title, &isOpen, ImGuiWindowFlags_MenuBar); tilemap) {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Edit"))
                {
                    if (ImGui::MenuItem("Save Tilemap")) {

                    }
                    if (ImGui::MenuItem("Load Tilemap")) {

                    }

                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }
    }
    ui::pop_window_chrome_style(chrome_colors);
}
