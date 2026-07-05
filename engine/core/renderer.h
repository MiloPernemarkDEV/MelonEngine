#pragma once
#include <cstdint>

struct RendererContext;

extern "C" {
    RendererContext* renderer_init(uint32_t width, uint32_t height, void* hwnd, void* hinstance);
    void renderer_draw(RendererContext* context);
    void renderer_terminate(RendererContext* context);
}

class Renderer {
public:
    Renderer(uint32_t width, uint32_t height);
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    Renderer(Renderer&& other) noexcept;
    Renderer& operator=(Renderer&& other) noexcept;

    void init(void* glfwWindowHandle);
    void draw();
    void terminate();

private:
    RendererContext* _context = nullptr;
    uint32_t _width = 0;
    uint32_t _height = 0;
};