#include "WindowSystem.h"
#include "ImageUtil.h"

WindowSystem::WindowSystem()
    : window(nullptr), WINDOW_X(1400), WINDOW_Y(980), APP_NAME("MelonEngine | OpenGL")
{

}

// ReSharper disable once CppMemberFunctionMayBeConst
bool WindowSystem::Init() {
    glfwInit();
    setupGLContext();
    window = glfwCreateWindow(WINDOW_X, WINDOW_Y, APP_NAME, nullptr, nullptr);
    createWindowIcon("Assets/Icons/melonengine.png");

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        Debug::Log(LogLevel::ERROR, "Failed to initialize GLAD");
        return false;
    }

    if (!window) { return false; }
    return true;
}

bool WindowSystem::WindowShouldClose() const {
    return glfwWindowShouldClose(window);
}

void WindowSystem::SwapBuffers() const {
    glfwSwapBuffers(window);
}

void WindowSystem::PollForEvents() const {
    glfwPollEvents();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void WindowSystem::Terminate() {
    glfwTerminate();
}

auto WindowSystem::setupGLContext() const -> void {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

auto WindowSystem::createWindowIcon(const char* filename) const -> void {
    auto [data] = ImageUtil::CreateIcon(filename);
    GLFWimage images[1];
    images[0].pixels = data.pixels;
    images[0].width = data.x;
    images[0].height = data.y;
    glfwSetWindowIcon(window, 1, images);
}
