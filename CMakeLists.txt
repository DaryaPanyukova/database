cmake_minimum_required(VERSION 3.0.0)
project(lab12 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib)

enable_testing()
add_subdirectory(tests)