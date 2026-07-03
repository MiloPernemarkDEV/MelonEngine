set(IMGUI_DIR ${imgui_SOURCE_DIR})

add_library(imgui STATIC
        ${CMAKE_SOURCE_DIR}/Vendor/imgui/imgui.cpp
        ${CMAKE_SOURCE_DIR}/Vendor/imgui/imgui_draw.cpp
        ${CMAKE_SOURCE_DIR}/Vendor/imgui/imgui_tables.cpp
        ${CMAKE_SOURCE_DIR}/Vendor/imgui/imgui_widgets.cpp
        ${CMAKE_SOURCE_DIR}/Vendor/imgui/imgui_demo.cpp
        ${CMAKE_SOURCE_DIR}/Vendor/imgui/backends/imgui_impl_vulkan.cpp
        ${CMAKE_SOURCE_DIR}/Vendor/imgui/backends/imgui_impl_glfw.cpp
)

target_include_directories(imgui PUBLIC
        ${CMAKE_SOURCE_DIR}/Vendor/imgui/
        ${CMAKE_SOURCE_DIR}/Vendor/imgui/backends
        ${CMAKE_SOURCE_DIR}/Vendor/include
)

target_link_libraries(imgui PUBLIC
        Vulkan::Vulkan
)

add_library(Engine STATIC
        pch.h
        core/platform_window.cpp
        core/arena_memory.cpp
)

# todo: add WindowSystem

target_sources(Engine PRIVATE
        core/app_icon.cpp
)

target_include_directories(Engine PUBLIC
        ${CMAKE_SOURCE_DIR}/Engine/core
        ${CMAKE_SOURCE_DIR}/Engine
        ${CMAKE_SOURCE_DIR}/Vendor/include
        ${CMAKE_SOURCE_DIR}/Vendor/assimp/include
        ${vma_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/Vendor/imgui/
)

target_link_directories(Engine PUBLIC
        ${CMAKE_SOURCE_DIR}/Vendor/lib
)

target_link_libraries(Engine PUBLIC
        Vulkan::Vulkan
        glad
        glfw
        assimp
        vk-bootstrap::vk-bootstrap
        imgui
        fastgltf::fastgltf
)

target_precompile_headers(Engine PUBLIC
        pch.h
)