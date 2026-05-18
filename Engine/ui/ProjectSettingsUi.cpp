#include "ProjectSettingsUi.h"

void ui::draw_project_settings_ui(const char *title) {
    bool& isOpen = MelonImGui::window_open(title);
    if (!isOpen) {
        return;
    }

    // project settings:
    // VSYNC
    // FPS LIMIT
    // Dark / Light Mode
    if (const ui::window projectSettings(title, &isOpen); projectSettings) {

    }
}
