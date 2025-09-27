cmake_minimum_required(VERSION 3.10)
project(sha3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Main executable
add_executable(sha3
    src/main.cpp
)

# GoogleTest (using submodule)
add_subdirectory(googletest)

enable_testing()

# Test executable
add_executable(sha3_tests
    src/hash_utils.cpp
    test/src/main.cpp
    test/src/bit_management_test.cpp
    # Add other test source files here if needed
)

target_link_libraries(sha3_tests
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(sha3_tests)