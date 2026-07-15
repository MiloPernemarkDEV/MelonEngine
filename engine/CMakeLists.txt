set(IMGUI_DIR ${imgui_SOURCE_DIR})

add_library(imgui STATIC
        ${CMAKE_SOURCE_DIR}/Vendor/imgui/imgui.cpp
        ${CMAKE_SOURCE_DIR}/Vendor/imgui/imgui_draw.cpp
        ${CMAKE_SOURCE_DIR}/Vendor/imgui/imgui_tables.cpp
        ${CMAKE_SOURCE_DIR}/Vendor/imgui/imgui_widgets.cpp
        ${CMAKE_SOURCE_DIR}/Vendor/imgui/imgui_demo.cpp
        ${CMAKE_SOURCE_DIR}/Vendor/imgui/backends/imgui_impl_vulkan.cpp
        ${CMAKE_SOURCE_DIR}/Vendor/imgui/backends/imgui_impl_glfw.cpp
        ${CMAKE_SOURCE_DIR}/Vendor/imgui/backends/imgui_impl_win32.cpp
)

if(WIN32)
        target_compile_definitions(imgui PUBLIC IMGUI_IMPL_WIN32_API)
endif()

target_include_directories(imgui PUBLIC
        ${CMAKE_SOURCE_DIR}/Vendor/imgui/
        ${CMAKE_SOURCE_DIR}/Vendor/imgui/backends
        ${CMAKE_SOURCE_DIR}/Vendor/include
        ${CMAKE_SOURCE_DIR}/engine/platform/public
        ${CMAKE_SOURCE_DIR}/engine/core/public
        ${CMAKE_SOURCE_DIR}/engine/math/public
        ${CMAKE_SOURCE_DIR}/engine/renderer/public
        platform/win32/public
)

target_link_libraries(imgui PUBLIC
        Vulkan::Vulkan
)

add_library(Engine STATIC
        pch.h

        # Core Private
        core/private/platform.cpp
        core/private/arena_memory.cpp
        core/private/renderer.cpp
        core/private/file_util.cpp
        core/private/job_system.cpp
        core/Private/string_util.cpp
        core/private/melon_gui.cpp
        core/private/time_util.cpp

        # Core Public
        core/public/file_util.h
        core/public/arena_memory.h
        core/public/logger.h
        core/public/renderer.h
        core/public/job_system.h
        core/public/string_util.h
        core/public/bsp_tree.h
        core/public/melon_gui.h
        core/public/time_util.h


        # Renderer Public
        renderer/public/vk_instance.h
        renderer/public/vk_renderer.h
        renderer/public/vk_device.h

        # Renderer Private
        renderer/private/vk_renderer.cpp
        renderer/private/vk_instance.cpp
        renderer/private/vk_device.cpp
)

target_sources(Engine PRIVATE
        core/private/app_icon.cpp
)

target_include_directories(Engine PUBLIC
        ${CMAKE_SOURCE_DIR}/Engine/core
        ${CMAKE_SOURCE_DIR}/Engine
        ${CMAKE_SOURCE_DIR}/Vendor/include
        ${CMAKE_SOURCE_DIR}/Vendor/assimp/include
        ${vma_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/Vendor/imgui/
        ${CMAKE_SOURCE_DIR}/Vendor/include
)

target_link_directories(Engine PUBLIC
        ${CMAKE_SOURCE_DIR}/Vendor/lib
        ${CMAKE_SOURCE_DIR}/rust/target/debug
)

target_link_libraries(Engine PUBLIC
        Vulkan::Vulkan
        glfw
        assimp
        vk-bootstrap::vk-bootstrap
        imgui
        fastgltf::fastgltf
        renderer
)

# Rust static libraries require native OS system dependencies to link successfully
if(WIN32)
        target_link_libraries(Engine PUBLIC ws2_32 userenv ntdll bcrypt)
elseif(APPLE)
        target_link_libraries(Engine PUBLIC "-framework Security" "-framework CoreFoundation")
else()
        target_link_libraries(Engine PUBLIC pthread dl m)
endif()

target_precompile_headers(Engine PUBLIC
        pch.h
)