cmake_minimum_required(VERSION 3.10)
project(simplex_solver)

set(CMAKE_CXX_STANDARD 14)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)


add_subdirectory(simplex_solver)
add_subdirectory(tests)
