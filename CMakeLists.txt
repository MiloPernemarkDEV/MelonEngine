cmake_minimum_required(VERSION 3.24)
project(MelonEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FETCHCONTENT_QUIET OFF)

find_package(Vulkan REQUIRED)

include(FetchContent)

add_compile_options(-fmacro-prefix-map=${CMAKE_SOURCE_DIR}/=)

FetchContent_Declare(
    VMA
    GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
    GIT_TAG        v3.1.0
)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        docking
)

FetchContent_MakeAvailable(VMA imgui)
find_package(Vulkan REQUIRED)

add_subdirectory(Vendor)
add_subdirectory(Engine)
add_subdirectory(Sandbox)
