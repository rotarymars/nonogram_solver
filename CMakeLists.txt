cmake_minimum_required(VERSION 3.14)
project(nonogram_solver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable threading support
find_package(Threads REQUIRED)

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Create a library with the core functions for testing
add_library(nonogram_lib src/main.cpp)
target_link_libraries(nonogram_lib Threads::Threads)
target_compile_definitions(nonogram_lib PRIVATE NO_MAIN TESTING)

# Add executable
add_executable(nonogram_solver src/main.cpp)
target_link_libraries(nonogram_solver Threads::Threads)

# Enable testing
enable_testing()

# Add test executable
add_executable(nonogram_tests test/test_nonogram.cpp)
target_link_libraries(nonogram_tests
    nonogram_lib
    gtest_main
    Threads::Threads
)

include(GoogleTest)
gtest_discover_tests(nonogram_tests)

# Compiler flags for optimization and warnings
target_compile_options(nonogram_solver PRIVATE
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:Debug>:-g -O0>
    -Wall -Wextra
)
target_compile_options(nonogram_lib PRIVATE
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:Debug>:-g -O0>
    -Wall -Wextra
)