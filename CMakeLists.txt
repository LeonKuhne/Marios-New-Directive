cmake_minimum_required(VERSION 3.15)
project(Engine)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wno-c99-designator)

find_package(SDL3 REQUIRED)
find_package(Bullet REQUIRED)

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