#include "renderer.h"
#include <utility>
#include <iostream>
#include "logger.h"
#include "arena_memory.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#if defined (USE_RUST_RENDERER)

Renderer::Renderer(uint32_t width, uint32_t height)
    : _width(width), _height(height)
{
}

Renderer::~Renderer() {
    terminate();
}

Renderer::Renderer(Renderer&& other) noexcept
    : _context(other._context), _width(other._width), _height(other._height)
{
    other._context = nullptr;
}

Renderer& Renderer::operator=(Renderer&& other) noexcept {
    if (this != &other) {
        terminate();
        _context = other._context;
        _width = other._width;
        _height = other._height;
        other._context = nullptr;
    }
    return *this;
}

bool Renderer::init(void* windowHandle) {
    if (!windowHandle) {
        ME_LOG(Error, "Cannot initialize renderer: Provided window handle is null.");
        return false;
    }

    GLFWwindow* window = static_cast<GLFWwindow*>(windowHandle);

    void* hwnd = glfwGetWin32Window(window);
    void* hinstance = GetModuleHandle(nullptr); // Standard Win32 call to get the active process module instance

    _context = renderer_init(_width, _height, hwnd, hinstance);

    if (!_context) {
        ME_LOG(Error, "Failed to initialize Rust Renderer Context.");
    }

    return true;
}

void Renderer::draw() {
    if (_context) {
        renderer_draw(_context);
    }
}

void Renderer::terminate() {
    if (_context) {
        renderer_terminate(_context);
        _context = nullptr;
    }
}

#else

Renderer::Renderer(uint32_t width, uint32_t height)
    : _width(width), _height(height)
{
    _vk_renderer = g_GlobalArena.add<VkRenderer>();
}

Renderer::~Renderer() {
    _vk_renderer->terminate();
}

bool Renderer::init(void* glfwWindowHandle) {
    if (!glfwWindowHandle) {
        ME_LOG(Error, "Cannot initialize renderer: Provided GLFW window handle is null.");
        return false;
    }

    GLFWwindow* window = static_cast<GLFWwindow*>(glfwWindowHandle);

    if (!_vk_renderer->init()) {
        ME_LOG(Error, "Failed to initialize Rust Renderer Context.");
    }

    return true;
}

void Renderer::draw() {
    _vk_renderer->draw();
}

void Renderer::terminate() {
    _vk_renderer->terminate();
}
#endif
