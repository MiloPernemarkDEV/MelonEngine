#include "pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <ImageUtil.h>

auto ImageUtil::LoadIconData(Icon& icon, const char* filename) -> void {
    icon.data.filename = filename;
    icon.data.pixels = stbi_load(icon.data.filename, &icon.data.x, &icon.data.y, &icon.data.colorChannels , 0);
}

auto ImageUtil::CreateIcon(const char *filename) -> Icon {
    Icon icon{};
    icon.data.filename = filename;
    LoadIconData(icon, filename);
    return icon;
}
