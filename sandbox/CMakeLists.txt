add_executable(Sandbox
        src/entry.cpp
        src/application.cpp
)



target_link_libraries(Sandbox PRIVATE Engine)

target_include_directories(Sandbox PRIVATE
    "${CMAKE_SOURCE_DIR}/engine/core/public"
)