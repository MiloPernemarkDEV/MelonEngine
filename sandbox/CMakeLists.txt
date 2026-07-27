add_executable(Sandbox
        src/entry.cpp
        src/application.cpp
)

target_link_libraries(Sandbox PRIVATE Engine)
