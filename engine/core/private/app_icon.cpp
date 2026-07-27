#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <app_icon.h>

auto AppIcon::load_icon_data(Icon& icon, const char* filename) -> void {
    icon.data.filename = filename;
    icon.data.pixels = stbi_load(icon.data.filename, &icon.data.x, &icon.data.y, &icon.data.colorChannels , 0);
}

auto AppIcon::create_icon(const char *filename) -> Icon {
    Icon icon{};
    icon.data.filename = filename;
    load_icon_data(icon, filename);
    return icon;
}
