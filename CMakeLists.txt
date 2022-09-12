cmake_minimum_required(VERSION 3.10)
project(simplex_solver)

set(CMAKE_CXX_STANDARD 14)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(FetchContent)
FetchContent_Declare(fakeit
        GIT_REPOSITORY https://github.com/eranpeer/FakeIt.git
        GIT_TAG 2.3.0)

add_subdirectory(simplex_solver)
add_subdirectory(tests)
