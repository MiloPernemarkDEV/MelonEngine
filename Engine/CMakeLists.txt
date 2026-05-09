add_library(Engine STATIC)

# todo: add WindowSystem


target_sources(Engine PRIVATE
        src/ArenaAlloc.cpp
        src/Debug.cpp
        src/WindowSystem.cpp
        src/ImageUtil.cpp
)

target_include_directories(Engine PUBLIC
        include
        ${CMAKE_SOURCE_DIR}/Vendor/include
)

target_link_directories(Engine PUBLIC
        ${CMAKE_SOURCE_DIR}/Vendor/lib
)

target_link_libraries(Engine PUBLIC
        Vulkan::Vulkan
        glfw3
        glad
)