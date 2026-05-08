add_library(Engine STATIC)

target_sources(Engine PRIVATE
        src/Engine.cpp
)

target_include_directories(Engine PUBLIC
        include
)