cmake_minimum_required(VERSION 3.20)
project(MelonEngine)

set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED)

include(FetchContent)

FetchContent_Declare(
        vk_bootstrap
        GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap.git
        GIT_TAG v1.4.321
)

FetchContent_MakeAvailable(vk_bootstrap)

add_subdirectory(Vendor)
add_subdirectory(Engine)
add_subdirectory(Sandbox)