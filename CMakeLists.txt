
cmake_minimum_required(VERSION 3.15)
project(Engine)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wno-c99-designator)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL3 REQUIRED)
find_package(Bullet REQUIRED)

# Compile shaders
set(SHADER_OUTPUT_DIR "${CMAKE_BINARY_DIR}/shaders")
file(MAKE_DIRECTORY "${SHADER_OUTPUT_DIR}")

file(GLOB SHADERS "${CMAKE_SOURCE_DIR}/assets/shaders/*"
)

foreach(SHADER ${SHADERS})
    if(IS_DIRECTORY "${SHADER}")
        continue()
    endif()
    get_filename_component(NAME "${SHADER}" NAME)

    string(REGEX REPLACE "\\.(hlsl|glsl)$" "" SHADER_NAME "${NAME}")
    set(OUTPUT "${SHADER_OUTPUT_DIR}/${SHADER_NAME}.spv")

    if(NAME MATCHES "\\.([^.]+)\\.hlsl$")
        set(STAGE "${CMAKE_MATCH_1}")

        if(STAGE STREQUAL "vert")
            set(TARGET "vs_6_2")
        elseif(STAGE STREQUAL "frag")
            set(TARGET "ps_6_2")
        elseif(STAGE STREQUAL "comp")
            set(TARGET "cs_6_2")
        else()
            message(FATAL_ERROR "Unknown HLSL shader stage: ${NAME}")
        endif()

        add_custom_command(
            OUTPUT "${OUTPUT}"
            COMMAND dxc
                -spirv
                -T ${TARGET}
                -E main
                "${SHADER}"
                -Fo "${OUTPUT}"
            DEPENDS "${SHADER}"
            COMMENT "Compiling HLSL shader ${NAME}"
        )

    elseif(NAME MATCHES "\\.([^.]+)\\.glsl$")
        set(STAGE "${CMAKE_MATCH_1}")

        if(NOT STAGE MATCHES "^(vert|frag|comp)$")
            message(FATAL_ERROR "Unknown GLSL shader stage: ${NAME}")
        endif()

        add_custom_command(
            OUTPUT "${OUTPUT}"
            COMMAND glslc
                -fshader-stage=${STAGE}
                "${SHADER}"
                -o "${OUTPUT}"
            DEPENDS "${SHADER}"
            COMMENT "Compiling GLSL shader ${NAME}"
        )

    else()
        message(FATAL_ERROR "Unknown shader format: ${NAME}")
    endif()

    list(APPEND COMPILED_SHADERS "${OUTPUT}")
endforeach()

add_custom_target(Shaders ALL DEPENDS ${COMPILED_SHADERS})


# Collect all your source files
file(GLOB_RECURSE SOURCES
    "main.cpp"
    "lib/*.cpp"
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    SDL3::SDL3
    ${BULLET_LIBRARIES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${BULLET_INCLUDE_DIRS}
)