//
// Created by milos on 2026-05-09.
//

#ifndef MELONENGINE_COMPONENTS_H
#define MELONENGINE_COMPONENTS_H

#include <cstdint>
#include "ImageUtil.h"

struct Entity {
    uint32_t id;
    bool active = true;
};

struct TransformComponent {
    float x = 0.0f;
    float y = 0.0f;
    float rotation = 0.0f;
};

struct SpriteComponent {
    float x = 0.0f;
    float y = 0.0f;
};

#endif //MELONENGINE_COMPONENTS_H
