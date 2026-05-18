#ifndef MELONENGINE_TILEMAP_H
#define MELONENGINE_TILEMAP_H

#include "Keycode.h"

class Tilemap {
public:
    Tilemap();
    ~Tilemap();
    inline static Keycode trigKey { Keycode::KEY_LEFT_ALT };
    inline static Keycode initKey { Keycode::KEY_F1 };
private:
};

#endif //MELONENGINE_TILEMAP_H
