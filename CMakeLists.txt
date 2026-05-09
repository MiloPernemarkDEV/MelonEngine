cmake_minimum_required(VERSION 3.20)
project(MelonEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

add_subdirectory(Engine)
add_subdirectory(Sandbox)
add_subdirectory(Vendor)
