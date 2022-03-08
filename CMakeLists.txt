cmake_minimum_required(VERSION 3.10)

# Project
project(Chip8 VERSION 1.0 LANGUAGES CXX)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

# Flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add the executable
add_executable(
    main
    src/main.cpp
    src/application.cpp
    src/chip8.cpp
    src/options.cpp
    src/window.cpp
)

target_link_libraries(main ${SDL2_LIBRARIES})
