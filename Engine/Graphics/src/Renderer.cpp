#include "Renderer.h"


Renderer::Renderer(GLFWwindow *window)
    : _window(window), _device(window) {
}

void Renderer::Init() {
    _device.Init();
}

void Renderer::Terminate() {
    _device.Cleanup();
}
