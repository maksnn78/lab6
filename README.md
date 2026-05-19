# Лабораторная работа №6
## Аксентьев Максим ИУ8-22
## CI Status
![CI](https://github.com/maksnn78/lab6/actions/workflows/ci.yml/badge.svg)
## Release Status
![Release](https://github.com/maksnn78/lab6/actions/workflows/release.yml/badge.svg)
## Подготовка репозитория
```bash
git clone https://github.com/maksnn78/lab5 ~/lab6
cd lab6
git remote remove origin
git remote add origin https://github.com/maksnn78/lab6
git submodule update --init --recursive
git push -u origin main
```
## Замена CMakeLists.txt
```bash
nano CMakeLists.txt
```
```cmake
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
```
## Создание CPackConfig.cmake
```bash
nano CPackConfig.cmake
```
```cmake
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME                "solver")
set(CPACK_PACKAGE_CONTACT             "maksnn78@example.com")
set(CPACK_PACKAGE_VERSION_MAJOR       ${SOLVER_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR       ${SOLVER_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH       ${SOLVER_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION_TWEAK       ${SOLVER_VERSION_TWEAK})
set(CPACK_PACKAGE_VERSION             ${SOLVER_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_FILE    ${CMAKE_CURRENT_SOURCE_DIR}/DESCRIPTION)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Solver - solves quadratic equations")
set(CPACK_RESOURCE_FILE_LICENSE       ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README        ${CMAKE_CURRENT_SOURCE_DIR}/README.md)

set(CPACK_RPM_PACKAGE_NAME     "solver")
set(CPACK_RPM_PACKAGE_LICENSE  "Apache-2.0")
set(CPACK_RPM_PACKAGE_GROUP    "Applications/Engineering")
set(CPACK_RPM_CHANGELOG_FILE   ${CMAKE_CURRENT_SOURCE_DIR}/ChangeLog.md)
set(CPACK_RPM_PACKAGE_RELEASE  1)

set(CPACK_DEBIAN_PACKAGE_NAME        "solver")
set(CPACK_DEBIAN_PACKAGE_PREDEPENDS  "libc6 (>= 2.17)")
set(CPACK_DEBIAN_PACKAGE_RELEASE     1)

include(CPack)
```
## Создание DESCRIPTION и ChangeLog.md
```bash
nano DESCRIPTION
```
```text
Solver is a command-line application that solves quadratic equations.
Built as part of laboratory work at Bauman Moscow State Technical University.
```
```bash
export DATE="$(LANG=en_US date +'%a %b %d %Y')"
nano ChangeLog.md
```
```markdown
* Tue May 19 2026 maksnn78 <maksnn78@gmail.com> 0.1.0.0
- Initial release
```
## Замена ci.yml
```bash
nano .github/workflows/ci.yml
```
```yml
name: CI

on:
  push:
    branches:
      - main
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
## Создание release.yml
```bash
nano .github/workflows/release.yml
```
```yml
name: Release

on:
  push:
    tags:
      - 'v*'

jobs:
  build-deb:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
        with:
          submodules: recursive

      - name: Install dependencies
        run: sudo apt-get update && sudo apt-get install -y cmake g++

      - name: Configure
        run: cmake -S . -B _build -DBUILD_TESTS=OFF

      - name: Build
        run: cmake --build _build

      - name: Package
        run: |
          cd _build
          cpack -G DEB
          cpack -G TGZ
          cpack -G ZIP

      - uses: actions/upload-artifact@v4
        with:
          name: linux-deb-packages
          path: |
            _build/*.deb
            _build/*.tar.gz
            _build/*.zip

  build-rpm:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
        with:
          submodules: recursive

      - name: Install dependencies
        run: sudo apt-get update && sudo apt-get install -y cmake g++ rpm

      - name: Configure
        run: cmake -S . -B _build -DBUILD_TESTS=OFF

      - name: Build
        run: cmake --build _build

      - name: Package
        run: cd _build && cpack -G RPM

      - uses: actions/upload-artifact@v4
        with:
          name: linux-rpm-packages
          path: _build/*.rpm

  release:
    needs: [build-deb, build-rpm]
    runs-on: ubuntu-latest
    permissions:
      contents: write
    steps:
      - uses: actions/download-artifact@v4
        with:
          name: linux-deb-packages
          path: packages/

      - uses: actions/download-artifact@v4
        with:
          name: linux-rpm-packages
          path: packages/

      - name: List packages
        run: ls -la packages/

      - uses: softprops/action-gh-release@v2
        with:
          files: packages/*
          generate_release_notes: true
```
## Установка rpm
```bash
sudo systemctl stop unattended-upgrades
sudo apt-get install -y rpm
```
```bash
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
The following package was automatically installed and is no longer required:
  libwoff1
Use 'sudo apt autoremove' to remove it.
The following additional packages will be installed:
  debugedit libfsverity0 liblua5.3-0 librpm9t64 librpmbuild9t64 librpmio9t64
  librpmsign9t64 rpm-common rpm2cpio
Suggested packages:
  alien elfutils rpmlint rpm-i18n
The following NEW packages will be installed:
  debugedit libfsverity0 liblua5.3-0 librpm9t64 librpmbuild9t64 librpmio9t64
  librpmsign9t64 rpm rpm-common rpm2cpio
0 upgraded, 10 newly installed, 0 to remove and 30 not upgraded.
Need to get 810 kB of archives.
After this operation, 2,854 kB of additional disk space will be used.
Get:1 http://de.archive.ubuntu.com/ubuntu noble/main amd64 debugedit amd64 1:5.0-5build2 [46.1 kB]
Get:2 http://de.archive.ubuntu.com/ubuntu noble/universe amd64 libfsverity0 amd64 1.5-1.1build3 [11.3 kB]
Get:3 http://de.archive.ubuntu.com/ubuntu noble/universe amd64 liblua5.3-0 amd64 5.3.6-2build2 [143 kB]
Get:4 http://de.archive.ubuntu.com/ubuntu noble/universe amd64 librpmio9t64 amd64 4.18.2+dfsg-2.1build2 [95.2 kB]
Get:5 http://de.archive.ubuntu.com/ubuntu noble/universe amd64 librpm9t64 amd64 4.18.2+dfsg-2.1build2 [217 kB]
Get:6 http://de.archive.ubuntu.com/ubuntu noble/universe amd64 librpmbuild9t64 amd64 4.18.2+dfsg-2.1build2 [80.1 kB]
Get:7 http://de.archive.ubuntu.com/ubuntu noble/universe amd64 librpmsign9t64 amd64 4.18.2+dfsg-2.1build2 [12.0 kB]
Get:8 http://de.archive.ubuntu.com/ubuntu noble/universe amd64 rpm-common amd64 4.18.2+dfsg-2.1build2 [34.0 kB]
Get:9 http://de.archive.ubuntu.com/ubuntu noble/universe amd64 rpm2cpio amd64 4.18.2+dfsg-2.1build2 [10.3 kB]
Get:10 http://de.archive.ubuntu.com/ubuntu noble/universe amd64 rpm amd64 4.18.2+dfsg-2.1build2 [161 kB]
Fetched 810 kB in 3s (244 kB/s)       
Selecting previously unselected package debugedit.
(Reading database ... 159512 files and directories currently installed.)
Preparing to unpack .../0-debugedit_1%3a5.0-5build2_amd64.deb ...
Unpacking debugedit (1:5.0-5build2) ...
Selecting previously unselected package libfsverity0:amd64.
Preparing to unpack .../1-libfsverity0_1.5-1.1build3_amd64.deb ...
Unpacking libfsverity0:amd64 (1.5-1.1build3) ...
Selecting previously unselected package liblua5.3-0:amd64.
Preparing to unpack .../2-liblua5.3-0_5.3.6-2build2_amd64.deb ...
Unpacking liblua5.3-0:amd64 (5.3.6-2build2) ...
Selecting previously unselected package librpmio9t64.
Preparing to unpack .../3-librpmio9t64_4.18.2+dfsg-2.1build2_amd64.deb ...
Unpacking librpmio9t64 (4.18.2+dfsg-2.1build2) ...
Selecting previously unselected package librpm9t64.
Preparing to unpack .../4-librpm9t64_4.18.2+dfsg-2.1build2_amd64.deb ...
Unpacking librpm9t64 (4.18.2+dfsg-2.1build2) ...
Selecting previously unselected package librpmbuild9t64.
Preparing to unpack .../5-librpmbuild9t64_4.18.2+dfsg-2.1build2_amd64.deb ...
Unpacking librpmbuild9t64 (4.18.2+dfsg-2.1build2) ...
Selecting previously unselected package librpmsign9t64.
Preparing to unpack .../6-librpmsign9t64_4.18.2+dfsg-2.1build2_amd64.deb ...
Unpacking librpmsign9t64 (4.18.2+dfsg-2.1build2) ...
Selecting previously unselected package rpm-common.
Preparing to unpack .../7-rpm-common_4.18.2+dfsg-2.1build2_amd64.deb ...
Unpacking rpm-common (4.18.2+dfsg-2.1build2) ...
Selecting previously unselected package rpm2cpio.
Preparing to unpack .../8-rpm2cpio_4.18.2+dfsg-2.1build2_amd64.deb ...
Unpacking rpm2cpio (4.18.2+dfsg-2.1build2) ...
Selecting previously unselected package rpm.
Preparing to unpack .../9-rpm_4.18.2+dfsg-2.1build2_amd64.deb ...
Unpacking rpm (4.18.2+dfsg-2.1build2) ...
Setting up libfsverity0:amd64 (1.5-1.1build3) ...
Setting up liblua5.3-0:amd64 (5.3.6-2build2) ...
Setting up debugedit (1:5.0-5build2) ...
Setting up librpmio9t64 (4.18.2+dfsg-2.1build2) ...
Setting up librpm9t64 (4.18.2+dfsg-2.1build2) ...
Setting up librpmbuild9t64 (4.18.2+dfsg-2.1build2) ...
Setting up rpm-common (4.18.2+dfsg-2.1build2) ...
Setting up librpmsign9t64 (4.18.2+dfsg-2.1build2) ...
Setting up rpm2cpio (4.18.2+dfsg-2.1build2) ...
Setting up rpm (4.18.2+dfsg-2.1build2) ...
Processing triggers for dbus (1.14.10-4ubuntu4.1) ...
Processing triggers for libc-bin (2.39-0ubuntu8.7) ...
Processing triggers for man-db (2.12.0-4build2) ...
```
## Сборка проекта
```bash
cmake -S . -B _build -DBUILD_TESTS=OFF
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


-- Configuring done (3.7s)
-- Generating done (0.1s)
-- Build files have been written to: /home/vboxuser/lab6/_build
```
```bash
cmake --build _build
```
```bash
[ 10%] Building CXX object formatter_lib/CMakeFiles/formatter.dir/formatter.cpp.o
[ 20%] Linking CXX static library libformatter.a
[ 20%] Built target formatter
[ 30%] Building CXX object formatter_ex_lib/CMakeFiles/formatter_ex.dir/formatter_ex.cpp.o
[ 40%] Linking CXX static library libformatter_ex.a
[ 40%] Built target formatter_ex
[ 50%] Building CXX object solver_lib/CMakeFiles/solver_lib.dir/solver.cpp.o
[ 60%] Linking CXX static library libsolver_lib.a
[ 60%] Built target solver_lib
[ 70%] Building CXX object hello_world_application/CMakeFiles/hello_world_application.dir/hello_world.cpp.o
[ 80%] Linking CXX executable hello_world_application
[ 80%] Built target hello_world_application
[ 90%] Building CXX object solver_application/CMakeFiles/solver_application.dir/equation.cpp.o
[100%] Linking CXX executable solver_application
[100%] Built target solver_application
```
## Создание пакетов
```bash
cd _build
cpack -G TGZ
```
```bash
CPack: Create package using TGZ
CPack: Install projects
CPack: - Run preinstall target for: solver
CPack: - Install project: solver []
CPack: Create package
CPack: - package: /home/vboxuser/lab6/_build/solver-0.1.0.0-Linux.tar.gz generated.
```
```bash
cpack -G ZIP
```
```bash
CPack: Create package using ZIP
CPack: Install projects
CPack: - Run preinstall target for: solver
CPack: - Install project: solver []
CPack: Create package
CPack: - package: /home/vboxuser/lab6/_build/solver-0.1.0.0-Linux.zip generated.
```
```bash
cpack -G DEB
```
```bash
CPack: Create package using DEB
CPack: Install projects
CPack: - Run preinstall target for: solver
CPack: - Install project: solver []
CPack: Create package
-- CPACK_DEBIAN_PACKAGE_DEPENDS not set, the package will have no dependencies.
CPack: - package: /home/vboxuser/lab6/_build/solver-0.1.0.0-Linux.deb generated.
```
```bash
cpack -G RPM
```
```bash
CPack: Create package using RPM
CPack: Install projects
CPack: - Run preinstall target for: solver
CPack: - Install project: solver []
CPack: Create package
CPackRPM: Will use GENERATED spec file: /home/vboxuser/lab6/_build/_CPack_Packages/Linux/RPM/SPECS/solver.spec
CPack: - package: /home/vboxuser/lab6/_build/solver-0.1.0.0-Linux.rpm generated.
```
```bash
ls *.tar.gz *.zip *.deb *.rpm
```
```bash
solver-0.1.0.0-Linux.deb  solver-0.1.0.0-Linux.tar.gz
solver-0.1.0.0-Linux.rpm  solver-0.1.0.0-Linux.zip
```
## Коммит и пуш
```bash
cd ..
git add CMakeLists.txt \
        CPackConfig.cmake \
        DESCRIPTION \
        ChangeLog.md \
        .github/workflows/ci.yml \
        .github/workflows/release.yml
```
```bash
git commit -m "add CPack config and release workflow"
```
```bash
[main 90995fe] add CPack config and release workflow
 6 files changed, 132 insertions(+), 5 deletions(-)
 create mode 100644 .github/workflows/release.yml
 create mode 100644 CPackConfig.cmake
 create mode 100644 ChangeLog.md
 create mode 100644 DESCRIPTION
```
```bash
git push origin main
```
```bash
Username for 'https://github.com': maksnn78
Password for 'https://maksnn78@github.com': 
Enumerating objects: 15, done.
Counting objects: 100% (15/15), done.
Delta compression using up to 4 threads
Compressing objects: 100% (9/9), done.
Writing objects: 100% (10/10), 2.04 KiB | 261.00 KiB/s, done.
Total 10 (delta 2), reused 0 (delta 0), pack-reused 0
remote: Resolving deltas: 100% (2/2), completed with 2 local objects.
To https://github.com/maksnn78/lab6
   d94a400..90995fe  main -> main
```
## Создание тега и запуск релиза
```bash
git tag v0.1.0.0
git push origin v0.1.0.0
```
```bash
Total 0 (delta 0), reused 0 (delta 0), pack-reused 0
To https://github.com/maksnn78/lab6
 * [new tag]         v0.1.0.0 -> v0.1.0.0
```
