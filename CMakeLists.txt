cmake_minimum_required(VERSION 3.16)

project(main C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(main src/main.c)

target_include_directories(main PRIVATE src)

target_link_libraries(main PRIVATE
    SDL2::SDL2
    SDL2_image::SDL2_image
)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})