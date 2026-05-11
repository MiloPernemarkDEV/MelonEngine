#include "Renderer.h"


Renderer::Renderer(GLFWwindow *window)
    : _window(window), _device(window) {
}

bool Renderer::Init() {
    _device.Init();

    return true;
}

void Renderer::Terminate() {
    _device.Cleanup();
}
