//
// Created by milos on 2026-05-09.
//

#ifndef MELONENGINE_IMAGEUTIL_H
#define MELONENGINE_IMAGEUTIL_H

struct ImageData {
    int x = 0;
    int y = 0;
    int colorChannels = 0;
    unsigned char* pixels = nullptr;
    const char* filename;
};

struct Icon {
    ImageData data;
};

class ImageUtil {
public:
    static auto LoadIconData(Icon& icon, const char* filename) -> void;

    static auto CreateIcon(const char* filename) -> Icon;


private:
};

#endif //MELONENGINE_IMAGEUTIL_H
