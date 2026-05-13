add_library(Engine STATIC
        graphics/include/Renderer.h
        graphics/src/Renderer.cpp
        graphics/include/Device.h
        graphics/src/Device.cpp
        graphics/include/RenderDefines.h
        graphics/include/RendUtil.h
        graphics/src/RendUtil.cpp
        pch.h
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
)

target_include_directories(Engine PUBLIC
        ${CMAKE_SOURCE_DIR}/Engine/Core/include
        ${CMAKE_SOURCE_DIR}/Engine/Graphics/include
        ${CMAKE_SOURCE_DIR}/Engine
        ${CMAKE_SOURCE_DIR}/Vendor/include
        ${CMAKE_SOURCE_DIR}/Vendor/assimp/include
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
)

target_precompile_headers(Engine PUBLIC
        pch.h
)

