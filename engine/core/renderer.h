
#pragma once

#include <cstdint>

struct RendererContext;

extern "C" {
    RendererContext* renderer_init(uint32_t width, uint32_t height);

    void renderer_draw(RendererContext* context);

    void renderer_terminate(RendererContext* context);
}