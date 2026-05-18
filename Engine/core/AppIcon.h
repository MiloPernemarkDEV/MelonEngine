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

class AppIcon {
public:
    static auto load_icon_data(Icon& icon, const char* filename) -> void;

    static auto create_icon(const char* filename) -> Icon;


private:
};

#endif //MELONENGINE_IMAGEUTIL_H
