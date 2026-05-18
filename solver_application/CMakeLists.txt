cmake_minimum_required(VERSION 3.4)
project(solver_application)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(solver_application equation.cpp)

target_link_libraries(solver_application formatter_ex solver_lib)

target_include_directories(solver_application PUBLIC
${CMAKE_SOURCE_DIR}/solver_lib)
