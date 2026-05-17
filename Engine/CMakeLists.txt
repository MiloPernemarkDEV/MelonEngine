set(IMGUI_DIR ${imgui_SOURCE_DIR})

add_library(imgui STATIC
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
)

target_include_directories(imgui PUBLIC
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
        ${CMAKE_SOURCE_DIR}/Vendor/include
)

target_link_libraries(imgui PUBLIC
        Vulkan::Vulkan
        glfw3
)

add_library(Engine STATIC
        graphics/include/Renderer.h
        graphics/src/Renderer.cpp
        graphics/include/Device.h
        graphics/src/Device.cpp
        graphics/include/RenderDefines.h
        graphics/include/RendUtil.h
        graphics/src/RendUtil.cpp
        pch.h
        graphics/include/RenderTypes.h
        graphics/include/Descriptors.h
        graphics/include/Pipelines.h
        graphics/include/MelonImgui.h
        graphics/src/MelonImgui.cpp
        graphics/include/PipelineBuilder.h
        graphics/src/PipelineBuilder.cpp
        graphics/src/Buffer.cpp
)

# todo: add WindowSystem

target_sources(Engine PRIVATE
        core/src/ArenaAlloc.cpp
        core/src/Debug.cpp
        core/src/WindowSystem.cpp
        core/src/ImageUtil.cpp
        core/src/AssetSystem.cpp
        core/src/MelonAssert.cpp
        graphics/src/Device.cpp
        graphics/src/Renderer.cpp
        graphics/src/Descriptors.cpp
        graphics/src/Pipelines.cpp
        graphics/src/MelonImgui.cpp
)

target_include_directories(Engine PUBLIC
        ${CMAKE_SOURCE_DIR}/Engine/Core/include
        ${CMAKE_SOURCE_DIR}/Engine/Graphics/include
        ${CMAKE_SOURCE_DIR}/Engine
        ${CMAKE_SOURCE_DIR}/Vendor/include
        ${CMAKE_SOURCE_DIR}/Vendor/assimp/include
        ${vma_SOURCE_DIR}/include
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
)

target_precompile_headers(Engine PUBLIC
        pch.h
)