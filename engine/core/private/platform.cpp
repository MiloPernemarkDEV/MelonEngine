#include "platform.h"

#ifdef _WIN32
#include "logger.h"
#include <vulkan/vulkan_win32.h>

Platform::Platform()
    : window_title("MelonEngine"),
      window_height(1480),
      window_width(1080)
{

}

bool Platform::init() {
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = window_title.c_str();

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        0,
        window_title.c_str(),
        window_title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (hwnd == nullptr) {
        ME_LOG(Error, "Failed to create window, hwnd is nullptr");
        return false;
    }

    constexpr int showDefault = 1;
    ShowWindow(hwnd, showDefault);

    return true;
}

bool Platform::window_should_close() {
    return bWindowShouldClose;
}

void Platform::terminate() {
}

void Platform::poll_for_events() {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

std::vector<const char*> Platform::EXTENSIONS() const {

    return {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };

}

LRESULT CALLBACK Platform::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            bWindowShouldClose = true;
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}


#endif // _WIN32


