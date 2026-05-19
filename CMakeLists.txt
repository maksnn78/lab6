cmake_minimum_required(VERSION 3.4)
project(solver)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOLVER_VERSION_MAJOR 0)
set(SOLVER_VERSION_MINOR 1)
set(SOLVER_VERSION_PATCH 0)
set(SOLVER_VERSION_TWEAK 0)
set(SOLVER_VERSION
  ${SOLVER_VERSION_MAJOR}.${SOLVER_VERSION_MINOR}.${SOLVER_VERSION_PATCH}.${SOLVER_VERSION_TWEAK})
set(SOLVER_VERSION_STRING "v${SOLVER_VERSION}")

add_subdirectory(formatter_lib)
add_subdirectory(formatter_ex_lib)
add_subdirectory(solver_lib)
add_subdirectory(hello_world_application)
add_subdirectory(solver_application)

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(third-party/gtest)
  add_subdirectory(tests)
endif()

install(TARGETS solver_application DESTINATION bin)

include(CPackConfig.cmake)
