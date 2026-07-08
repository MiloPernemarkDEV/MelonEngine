#ifndef MELON_PLATFORM_WINDOW_H
#define MELON_PLATFORM_WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "types.h"


class PlatformGlfw
{
public:

    PlatformGlfw();
    ~PlatformGlfw() = default;

    PlatformGlfw(PlatformGlfw&) = delete;
    PlatformGlfw& operator=(PlatformGlfw&) = delete;

    PlatformGlfw(PlatformGlfw&&) noexcept = default;

    bool init();

    bool window_should_close() const;
    void swap_buffers() const;
    void poll_for_events() const;

    void set_app_icon(const char* filename) const;

    void terminate();

    GLFWwindow* window;

    const s32 WINDOW_X;
    const s32 WINDOW_Y;


private:

    void set_context() const;

    const char* APP_NAME;
};


class PlatformWin32
{
public:

    PlatformWin32();
    ~PlatformWin32() = default;

    PlatformWin32(PlatformWin32&) = delete;
    PlatformWin32& operator=(PlatformWin32&) = delete;

    PlatformWin32(PlatformWin32&&) noexcept = default;


    bool init();

    bool window_should_close() const;

    void swap_buffers() const;

    void poll_for_events() const;

    void terminate();

    void set_app_icon(const char* filename);


private:

};


class Platform
{
public:

    bool init();

    bool window_should_close() const;

    void terminate();

    void poll_for_events();

    void swap_buffers() const;

    void* get_handle();

    void set_app_icon(const char* filename);


private:

    PlatformGlfw glfw;
    PlatformWin32 win32;
};


#endif // MELON_PLATFORM_WINDOW_H