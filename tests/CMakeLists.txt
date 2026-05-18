file(GLOB TEST_SOURCES *.cpp)

add_executable(check ${TEST_SOURCES})

target_include_directories(check PRIVATE
    ${CMAKE_SOURCE_DIR}/solver_lib
)

target_link_libraries(check
    solver_lib
    gtest_main
)

add_test(NAME check COMMAND check)
