//
// Created by milos on 2026-05-09.
//

#include <ImageUtil.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

auto ImageUtil::LoadTextureData(Texture& texture, const char* filename) -> void {
    texture.data.filename = filename;
    texture.data.pixels = stbi_load(texture.data.filename, &texture.data.x, &texture.data.y, &texture.data.colorChannels , 0);
}

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
