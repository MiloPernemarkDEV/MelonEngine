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
        glfw3
)

add_library(Engine STATIC
        graphics/Renderer.h
        graphics/Renderer.cpp
        graphics/Device.h
        graphics/Device.cpp
        graphics/RenderDefines.h
        graphics/RendUtil.h
        graphics/RendUtil.cpp
        pch.h
        graphics/RenderTypes.h
        graphics/Descriptors.h
        graphics/Pipelines.h
        ui/MelonImGui.h
        ui/MelonImgui.cpp
        ui/UIConfig.h
        graphics/PipelineBuilder.h
        graphics/PipelineBuilder.cpp
        graphics/Buffer.cpp
        ui/TilemapEditorUi.cpp
        ui/TilemapEditorUi.h
        core/Tilemap.cpp
        core/Tilemap.h
        core/Keycode.h
        ui/ProjectSettingsUi.cpp
        core/Input.cpp
)

# todo: add WindowSystem

target_sources(Engine PRIVATE
        core/ArenaAlloc.cpp
        core/Debug.cpp
        core/WindowSystem.cpp
        core/AppIcon.cpp
        core/RuntimeAssert.cpp
        graphics/Device.cpp
        graphics/Renderer.cpp
        graphics/Descriptors.cpp
        graphics/Pipelines.cpp
        ui/MelonImgui.cpp
)

target_include_directories(Engine PUBLIC
        ${CMAKE_SOURCE_DIR}/Engine/core
        ${CMAKE_SOURCE_DIR}/Engine/graphics
        ${CMAKE_SOURCE_DIR}/Engine/ui
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
        glfw3
        glad
        assimp-vc145-mt.lib
        vk-bootstrap::vk-bootstrap
        imgui
        fastgltf::fastgltf
)

target_precompile_headers(Engine PUBLIC
        pch.h
)