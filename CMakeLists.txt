cmake_minimum_required(VERSION 3.10)
project(nonogram_solver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(nonogram_solver src/main.cpp)

# Enable threading support
find_package(Threads REQUIRED)
target_link_libraries(nonogram_solver Threads::Threads)

# Compiler flags for optimization and warnings
target_compile_options(nonogram_solver PRIVATE
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:Debug>:-g -O0>
    -Wall -Wextra
)