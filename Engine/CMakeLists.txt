add_library(Engine STATIC)

target_sources(Engine PRIVATE
        src/Engine.cpp
        src/ArenaAlloc.cpp
        src/Debug.cpp
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
)

