cmake_minimum_required(VERSION 3.10)
project(simplex_solver_lib)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DSIMPLEX_SOLVER_DLL_EXPORTS)
if(BUILD_SHARED_LIBS)
    add_definitions(-DSIMPLEX_SOLVER_DLL)
endif()

set(SOURCE_FILES
        src/variable_t.cpp
        include/simplex_solver/variable_t.hpp 
        src/expression_t.cpp 
        include/simplex_solver/expression_t.hpp 
        src/relation_t.cpp 
        include/simplex_solver/relation_t.hpp 
        src/simplex_solver_t.cpp 
        include/simplex_solver/simplex_solver_t.hpp
        src/simplex_table_t.cpp
        include/simplex_solver/simplex_table_t.hpp 
        src/simplex_table_builder.cpp 
        include/simplex_solver/simplex_table_builder.hpp 
        include/simplex_solver/simple_table_with_data_t.hpp
        include/simplex_solver/simplex_solver_api.hpp)

add_library(simplex_solver ${SOURCE_FILES})
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCE_FILES})

target_include_directories(simplex_solver
        PUBLIC ${PROJECT_SOURCE_DIR}/include)
