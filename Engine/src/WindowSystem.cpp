//
// Created by milos on 2026-05-08.
//

#include "WindowSystem.h"

WindowSystem::WindowSystem()
    : window(nullptr), WINDOW_X(1024), WINDOW_Y(1400), APP_NAME("Sandbox")
{
}

// ReSharper disable once CppMemberFunctionMayBeConst
void WindowSystem::Init() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WINDOW_X, WINDOW_Y, APP_NAME, nullptr, nullptr);

}

// ReSharper disable once CppMemberFunctionMayBeConst
void WindowSystem::Terminate() {
}

