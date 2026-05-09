add_executable(Sandbox
        src/entry.cpp
        src/Application.cpp
)

target_link_libraries(Sandbox PRIVATE Engine)