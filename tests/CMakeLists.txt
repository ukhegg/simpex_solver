cmake_minimum_required(VERSION 3.10)
project(simplex_solver_tests)

set(CMAKE_CXX_STANDARD 17)
find_package(Catch2 REQUIRED)

FetchContent_MakeAvailable(fakeit)

set(SOURCE_FILES
        variable_t_tests.cpp
        expression_t_tests.cpp 
        relation_t_tests.cpp
        simplex_table_builder_tests.cpp
        simplex_solver_tests.cpp
        simplex_table_with_data_tests.cpp)
add_executable(simplex_solver_tests ${SOURCE_FILES})
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCE_FILES})

target_include_directories(simplex_solver_tests
        PRIVATE ${simplex_solver_SOURCE_DIR}/include
        )

target_link_libraries(simplex_solver_tests
        PRIVATE simplex_solver Catch2::Catch2WithMain )
