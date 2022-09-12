cmake_minimum_required(VERSION 3.10)
project(simplex_solver_tests)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(${CMAKE_SOURCE_DIR}/3rd_party/Catch2-devel ${CMAKE_BINARY_DIR}/Catch2)

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
        PRIVATE ${CMAKE_SOURCE_DIR}/3rd_party/Catch2-devel/single_include)
target_link_libraries(simplex_solver_tests simplex_solver Catch2WithMain )
