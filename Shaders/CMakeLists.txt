set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/Shaders)
file(MAKE_DIRECTORY ${OUTPUT_DIR})

file(GLOB_RECURSE SHADER_FILES
        ${SHADER_DIR}/GLSL/*.vert
        ${SHADER_DIR}/GLSL/*.frag
        ${SHADER_DIR}/GLSL/*.comp
        ${SHADER_DIR}/HLSL/*.hlsl
)

set(SPIRV_OUTPUTS)

foreach(SHADER ${SHADER_FILES})

    get_filename_component(FILE_NAME ${SHADER} NAME)
    get_filename_component(FILE_EXT ${SHADER} EXT)
    set(SPIRV_FILE ${OUTPUT_DIR}/${FILE_NAME}.spv)

    if(FILE_EXT MATCHES "vert|frag|comp")

        add_custom_command(
                OUTPUT ${SPIRV_FILE}
                COMMAND glslangValidator -V ${SHADER} -o ${SPIRV_FILE}
                DEPENDS ${SHADER}
                COMMENT "Compiling GLSL: ${FILE_NAME}"
        )

    endif()

    if(FILE_EXT STREQUAL ".hlsl")

        # default assume compute for now (we improve later)
        add_custom_command(
                OUTPUT ${SPIRV_FILE}
                COMMAND dxc -T cs_6_0 -E main -spirv ${SHADER} -Fo ${SPIRV_FILE}
                DEPENDS ${SHADER}
                COMMENT "Compiling HLSL: ${FILE_NAME}"
        )

    endif()

    list(APPEND SPIRV_OUTPUTS ${SPIRV_FILE})

endforeach()

add_custom_target(Shaders ALL DEPENDS ${SPIRV_OUTPUTS})