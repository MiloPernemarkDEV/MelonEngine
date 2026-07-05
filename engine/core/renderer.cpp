#include "renderer.h"
#include <utility>
#include <iostream>
#include "logger.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

Renderer::Renderer(uint32_t width, uint32_t height)
    : _context(nullptr), _width(width), _height(height)
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

void Renderer::init(void* glfwWindowHandle) {
    if (!glfwWindowHandle) {
        ME_LOG(Error, "Cannot initialize renderer: Provided GLFW window handle is null.");
        return;
    }

    GLFWwindow* window = static_cast<GLFWwindow*>(glfwWindowHandle);

    void* hwnd = glfwGetWin32Window(window);
    void* hinstance = GetModuleHandle(nullptr); // Standard Win32 call to get the active process module instance

    _context = renderer_init(_width, _height, hwnd, hinstance);

    if (!_context) {
        ME_LOG(Error, "Failed to initialize Rust Renderer Context.");
    }
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