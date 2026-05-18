# Лабораторная работа №5
## Аксентьев Максим ИУ8-22
## CI Status
![CI](https://github.com/maksnn78/lab5/actions/workflows/ci.yml/badge.svg)
## Подготовка репозитория
```bash
git clone https://github.com/maksnn78/lab5.git
cd lab5
cp -r ~/lab4/* .
rm -rf build
git add .
git commit -m "initial import from lab4"
```
```bash
[main (root-commit) a9182d6] initial import from lab4
 17 files changed, 371 insertions(+)
 create mode 100644 .github/workflows/ci.yml
 create mode 100644 CMakeLists.txt
 create mode 100644 LICENSE
 create mode 100644 formatter_ex_lib/CMakeLists.txt
 create mode 100644 formatter_ex_lib/formatter_ex.cpp
 create mode 100644 formatter_ex_lib/formatter_ex.h
 create mode 100644 formatter_lib/CMakeLists.txt
 create mode 100644 formatter_lib/formatter.cpp
 create mode 100644 formatter_lib/formatter.h
 create mode 100644 hello_world_application/CMakeLists.txt
 create mode 100644 hello_world_application/hello_world.cpp
 create mode 100644 preview.png
 create mode 100644 solver_application/CMakeLists.txt
 create mode 100644 solver_application/equation.cpp
 create mode 100644 solver_lib/CMakeLists.txt
 create mode 100644 solver_lib/solver.cpp
 create mode 100644 solver_lib/solver.h
```
```bash
git push -u origin main
```
```bash
Enumerating objects: 26, done.
Counting objects: 100% (26/26), done.
Delta compression using up to 4 threads
Compressing objects: 100% (24/24), done.
Writing objects: 100% (26/26), 1.01 MiB | 3.87 MiB/s, done.
Total 26 (delta 3), reused 0 (delta 0), pack-reused 0
remote: Resolving deltas: 100% (3/3), done.
To https://github.com/maksnn78/lab5.git
 * [new branch]      main -> main
branch 'main' set up to track 'origin/main'.
```
## Подключение GoogleTest
```bash
mkdir third-party
git submodule add https://github.com/google/googletest third-party/gtest
```
```bash
Cloning into '/home/vboxuser/lab5/third-party/gtest'...
remote: Enumerating objects: 28637, done.
remote: Counting objects: 100% (64/64), done.
remote: Compressing objects: 100% (49/49), done.
remote: Total 28637 (delta 34), reused 15 (delta 15), pack-reused 28573 (from 3)
Receiving objects: 100% (28637/28637), 13.76 MiB | 14.08 MiB/s, done.
Resolving deltas: 100% (21275/21275), done.
```
```bash
git add .gitmodules third-party/gtest
git commit -m "add googletest submodule"
```
```bash
[main 3f68fa4] add googletest submodule
 2 files changed, 4 insertions(+)
 create mode 100644 .gitmodules
 create mode 160000 third-party/gtest
```
```bash
git push origin main
```
```bash
Enumerating objects: 5, done.
Counting objects: 100% (5/5), done.
Delta compression using up to 4 threads
Compressing objects: 100% (3/3), done.
Writing objects: 100% (4/4), 429 bytes | 214.00 KiB/s, done.
Total 4 (delta 1), reused 0 (delta 0), pack-reused 0
remote: Resolving deltas: 100% (1/1), completed with 1 local object.
To https://github.com/maksnn78/lab5.git
   a9182d6..3f68fa4  main -> main
```
## Создание каталога tests и настройка корневого CMakeLists.txt
```bash
mkdir tests
nano tests/test_solver.cpp
```
```cpp
#include <gtest/gtest.h>
#include "solver.h"

TEST(Solver, SolvesEquation)
{
    float x1 = 0;
    float x2 = 0;

    solve(1, -5, 6, x1, x2);

    EXPECT_FLOAT_EQ(x1, 2);
    EXPECT_FLOAT_EQ(x2, 3);
}

TEST(Solver, DoubleRoot)
{
    float x1 = 0;
    float x2 = 0;

    solve(1, -2, 1, x1, x2);

    EXPECT_FLOAT_EQ(x1, 1);
    EXPECT_FLOAT_EQ(x2, 1);
}
```
```bash
nano tests/CMakeLists.txt
```
```cmake
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
```
```bash
nano CMakeLists.txt
```
```cmake
cmake_minimum_required(VERSION 3.4)
project(lab3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
```
## Сборка и запуск тестов
```bash
cmake -S . -B build -DBUILD_TESTS=ON
```
```bash
CMake Deprecation Warning at CMakeLists.txt:1 (cmake_minimum_required):
  Compatibility with CMake < 3.5 will be removed from a future version of
  CMake.

  Update the VERSION argument <min> value or use a ...<max> suffix to tell
  CMake that the project does not need compatibility with older versions.


-- The C compiler identification is GNU 13.3.0
-- The CXX compiler identification is GNU 13.3.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
CMake Deprecation Warning at formatter_lib/CMakeLists.txt:1 (cmake_minimum_required):
  Compatibility with CMake < 3.5 will be removed from a future version of
  CMake.

  Update the VERSION argument <min> value or use a ...<max> suffix to tell
  CMake that the project does not need compatibility with older versions.


CMake Deprecation Warning at formatter_ex_lib/CMakeLists.txt:1 (cmake_minimum_required):
  Compatibility with CMake < 3.5 will be removed from a future version of
  CMake.

  Update the VERSION argument <min> value or use a ...<max> suffix to tell
  CMake that the project does not need compatibility with older versions.


CMake Deprecation Warning at solver_lib/CMakeLists.txt:1 (cmake_minimum_required):
  Compatibility with CMake < 3.5 will be removed from a future version of
  CMake.

  Update the VERSION argument <min> value or use a ...<max> suffix to tell
  CMake that the project does not need compatibility with older versions.


CMake Deprecation Warning at hello_world_application/CMakeLists.txt:1 (cmake_minimum_required):
  Compatibility with CMake < 3.5 will be removed from a future version of
  CMake.

  Update the VERSION argument <min> value or use a ...<max> suffix to tell
  CMake that the project does not need compatibility with older versions.


CMake Deprecation Warning at solver_application/CMakeLists.txt:1 (cmake_minimum_required):
  Compatibility with CMake < 3.5 will be removed from a future version of
  CMake.

  Update the VERSION argument <min> value or use a ...<max> suffix to tell
  CMake that the project does not need compatibility with older versions.


-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE  
-- Configuring done (0.7s)
-- Generating done (0.0s)
-- Build files have been written to: /home/vboxuser/lab5/build
```
```bash
cmake --build build
```
```bash
[  5%] Building CXX object formatter_lib/CMakeFiles/formatter.dir/formatter.cpp.o
[ 10%] Linking CXX static library libformatter.a
[ 10%] Built target formatter
[ 15%] Building CXX object formatter_ex_lib/CMakeFiles/formatter_ex.dir/formatter_ex.cpp.o
[ 20%] Linking CXX static library libformatter_ex.a
[ 20%] Built target formatter_ex
[ 25%] Building CXX object solver_lib/CMakeFiles/solver_lib.dir/solver.cpp.o
[ 30%] Linking CXX static library libsolver_lib.a
[ 30%] Built target solver_lib
[ 35%] Building CXX object hello_world_application/CMakeFiles/hello_world_application.dir/hello_world.cpp.o
[ 40%] Linking CXX executable hello_world_application
[ 40%] Built target hello_world_application
[ 45%] Building CXX object solver_application/CMakeFiles/solver_application.dir/equation.cpp.o
[ 50%] Linking CXX executable solver_application
[ 50%] Built target solver_application
[ 55%] Building CXX object third-party/gtest/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o
[ 60%] Linking CXX static library ../../../lib/libgtest.a
[ 60%] Built target gtest
[ 65%] Building CXX object third-party/gtest/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o
[ 70%] Linking CXX static library ../../../lib/libgmock.a
[ 70%] Built target gmock
[ 75%] Building CXX object third-party/gtest/googlemock/CMakeFiles/gmock_main.dir/src/gmock_main.cc.o
[ 80%] Linking CXX static library ../../../lib/libgmock_main.a
[ 80%] Built target gmock_main
[ 85%] Building CXX object third-party/gtest/googletest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o
[ 90%] Linking CXX static library ../../../lib/libgtest_main.a
[ 90%] Built target gtest_main
[ 95%] Building CXX object tests/CMakeFiles/check.dir/test_solver.cpp.o
[100%] Linking CXX executable check
[100%] Built target check
```
```bash
ctest --test-dir build --output-on-failure
```
```bash
Internal ctest changing into directory: /home/vboxuser/lab5/build
Test project /home/vboxuser/lab5/build
    Start 1: check
1/1 Test #1: check ............................   Passed    0.01 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.03 sec
```
## Настройка GitHub Actions
```bash
nano .github/workflows/ci.yml
```
```yaml
name: CI

on:
  push:
  pull_request:

jobs:
  build:
    runs-on: ubuntu-latest

    steps:
      - uses: actions/checkout@v4
        with:
          submodules: recursive

      - name: Install dependencies
        run: sudo apt-get update && sudo apt-get install -y cmake g++

      - name: Configure project
        run: cmake -S . -B build -DBUILD_TESTS=ON

      - name: Build project
        run: cmake --build build

      - name: Run tests
        run: ctest --test-dir build --output-on-failure
```
## Добавление тестов и обновление CI
```bash
git add CMakeLists.txt tests
git commit -m "add unit tests"
```
```bash
[main 3e39096] add unit tests
 3 files changed, 46 insertions(+)
 create mode 100644 tests/CMakeLists.txt
 create mode 100644 tests/test_solver.cpp
```
```bash
git push origin main
```
```bash
Enumerating objects: 8, done.
Counting objects: 100% (8/8), done.
Delta compression using up to 4 threads
Compressing objects: 100% (6/6), done.
Writing objects: 100% (6/6), 861 bytes | 287.00 KiB/s, done.
Total 6 (delta 2), reused 0 (delta 0), pack-reused 0
remote: Resolving deltas: 100% (2/2), completed with 2 local objects.
To https://github.com/maksnn78/lab5.git
   3f68fa4..3e39096  main -> main
```
```bash
git add .github/workflows/ci.yml
git commit -m "update CI to run tests"
```
```bash
[main e47bd2b] update CI to run tests
 1 file changed, 9 insertions(+), 5 deletions(-)
```
```bash
git push origin main
```
```bash
Enumerating objects: 9, done.
Counting objects: 100% (9/9), done.
Delta compression using up to 4 threads
Compressing objects: 100% (3/3), done.
Writing objects: 100% (5/5), 546 bytes | 273.00 KiB/s, done.
Total 5 (delta 2), reused 0 (delta 0), pack-reused 0
remote: Resolving deltas: 100% (2/2), completed with 2 local objects.
To https://github.com/maksnn78/lab5.git
   3e39096..e47bd2b  main -> main
```
## История коммитов
```bash
git log --oneline -4
```
```bash
e47bd2b (HEAD -> main, origin/main) update CI to run tests
3e39096 add unit tests
3f68fa4 add googletest submodule
a9182d6 initial import from lab4
```
