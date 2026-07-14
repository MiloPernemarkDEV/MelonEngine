#include "platform.h"

#ifdef _WIN32
#include "logger.h"
#include <vulkan/vulkan_win32.h>
#include <shobjidl.h>
#include <wrl/client.h>
#include "string_util.h"

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
    ME_LOG(Info, "Terminating platform layer");
}

void Platform::poll_for_events() {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

std::string Platform::file_open_dialog() {
    // WIN32 COM initialization
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) {
        ME_LOG(Error, "Failed to initialize COM");
        return {};
    }

    Microsoft::WRL::ComPtr<IFileOpenDialog> pFileOpen;
    Microsoft::WRL::ComPtr<IShellItem> pItem;
    std::string resultPath{""};

    // Get file open dialog COM object from os factory
    hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&pFileOpen));

    if (FAILED(hr)) {
        CoUninitialize();
        ME_LOG(Error, "Failed to get FIleOpenDialog COM object from the operating system");
        return {};
    }

    if (SUCCEEDED(pFileOpen->Show(nullptr))) {
        if (SUCCEEDED(pFileOpen->GetResult(&pItem))) {
            PWSTR pszFilePath = nullptr;

            if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
                resultPath = StringUtil::wide_to_multibyte(pszFilePath);
                CoTaskMemFree(pszFilePath);
            }
        }
    }
    CoUninitialize();
    return resultPath;
}

std::vector<const char*> Platform::get_vk_extensions() {

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


