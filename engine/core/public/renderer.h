#pragma once
#include <cstdint>

//#define USE_RUST_RENDERER
#if !defined(USE_RUST_RENDERER)
#include "vk_renderer.h"
#include "arena_memory.h"
#endif

#if defined(USE_RUST_RENDERER)
struct RendererContext;

extern "C" {
    RendererContext* renderer_init(uint32_t width, uint32_t height, void* hwnd, void* hinstance);
    void renderer_draw(RendererContext* context);
    void renderer_terminate(RendererContext* context);
}
#endif

class Renderer {
public:
    Renderer(uint32_t width, uint32_t height);
    ~Renderer();

#if defined(USE_RUST_RENDERER)
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    Renderer(Renderer&& other) noexcept;
    Renderer& operator=(Renderer&& other) noexcept;
#endif

    bool init(void* windowHandle);
    void draw();
    void terminate();

private:

#if defined(USE_RUST_RENDERER)
    RendererContext* _context = nullptr;
#else
    VkRenderer* _vk_renderer = nullptr;
#endif
    uint32_t _width = 0;
    uint32_t _height = 0;
};
