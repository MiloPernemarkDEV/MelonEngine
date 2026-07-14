#ifndef MELON_PLATFORM_WINDOW_H
#define MELON_PLATFORM_WINDOW_H

#include "types.h"
#include <string>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

class Platform
{
public:
    Platform();

    bool init();

    static bool window_should_close();

    void terminate();

    static void poll_for_events();

    static std::string file_open_dialog();

    static std::vector<const char*> get_vk_extensions();

    s32 WIDTH () const { return window_width; };

    s32 HEIGHT () const { return window_height; };

    void* HANDLE() const {return hwnd; };
private:
    std::string window_title;
    s32 window_height;
    s32 window_width;

#ifdef _WIN32
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    HINSTANCE hInstance = nullptr;
    HWND hwnd = nullptr;
#endif
    inline static bool bWindowShouldClose;
};

#endif // MELON_PLATFORM_WINDOW_H