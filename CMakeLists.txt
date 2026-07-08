cmake_minimum_required(VERSION 3.24)
project(MelonEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimizes FetchContent so it doesn't re-download constantly
set(FETCHCONTENT_QUIET OFF) 

find_package(Vulkan REQUIRED)

# DEPENDENCY FETCHING (Modern & Cached)
include(FetchContent)

# vk_bootstrap
FetchContent_Declare(
    vk_bootstrap
    GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap.git
    GIT_TAG        v1.4.321
)

# Vulkan Memory Allocator (VMA)
FetchContent_Declare(
    VMA
    GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
    GIT_TAG        v3.1.0
)

# Dear ImGui (Docking Branch)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        docking
)

# fastgltf
FetchContent_Declare(
    fastgltf
    GIT_REPOSITORY https://github.com/spnda/fastgltf.git
    GIT_TAG        v0.9.0
)

# GLFW 
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.9
)

# 6. Assimp 
# Disable building assimp tools/tests to drastically speed up compile times
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "")
set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "")
set(ASSIMP_INSTALL OFF CACHE INTERNAL "")
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG        v5.4.1
)

# Populate all dependencies at once 
FetchContent_MakeAvailable(vk_bootstrap VMA imgui glfw fastgltf assimp)
find_package(Vulkan REQUIRED)

# SUBDIRECTORIES
add_subdirectory(Vendor)
add_subdirectory(Engine)
add_subdirectory(Sandbox)