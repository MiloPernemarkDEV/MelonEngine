add_library(Engine STATIC)

# todo: add WindowSystem

target_sources(Engine PRIVATE
        core/src/ArenaAlloc.cpp
        core/src/Debug.cpp
        core/src/WindowSystem.cpp
        core/src/ImageUtil.cpp
        core/src/AssetSystem.cpp
        core/src/MeshComponent.cpp
        core/src/MelonAssert.cpp
)

target_include_directories(Engine PUBLIC
        ${CMAKE_SOURCE_DIR}/Engine/Core/include
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
)