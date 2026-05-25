# Лабораторная работа №6
## Аксентьев Максим ИУ8-22
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
## Создание CPackConfig.cmake и LICENSE.rtf
```bash
cat > LICENSE.rtf << 'EOF'
{\rtf1\ansi\deff0
{\fonttbl{\f0 Times New Roman;}}
\f0\fs20 Apache License 2.0\par
Copyright 2026 maksnn78\par
}
EOF
nano CPackConfig.cmake
```
```cmake
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME                "solver")
set(CPACK_PACKAGE_CONTACT             "maksnn78@gmail.com")
set(CPACK_PACKAGE_VERSION_MAJOR       ${SOLVER_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR       ${SOLVER_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH       ${SOLVER_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION_TWEAK       ${SOLVER_VERSION_TWEAK})
set(CPACK_PACKAGE_VERSION             ${SOLVER_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_FILE    ${CMAKE_CURRENT_SOURCE_DIR}/DESCRIPTION)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Solver - solves quadratic equations")
set(CPACK_RESOURCE_FILE_README        ${CMAKE_CURRENT_SOURCE_DIR}/README.md)

# --- Linux DEB ---
set(CPACK_DEBIAN_PACKAGE_NAME        "solver")
set(CPACK_DEBIAN_PACKAGE_PREDEPENDS  "libc6 (>= 2.17)")
set(CPACK_DEBIAN_PACKAGE_RELEASE     1)

# --- Linux RPM ---
set(CPACK_RPM_PACKAGE_NAME     "solver")
set(CPACK_RPM_PACKAGE_LICENSE  "Apache-2.0")
set(CPACK_RPM_PACKAGE_GROUP    "Applications/Engineering")
set(CPACK_RPM_CHANGELOG_FILE   ${CMAKE_CURRENT_SOURCE_DIR}/ChangeLog.md)
set(CPACK_RPM_PACKAGE_RELEASE  1)

# --- Windows MSI (WiX) ---
set(CPACK_WIX_UPGRADE_GUID  "A1B2C3D4-E5F6-7890-ABCD-EF1234567890")
set(CPACK_WIX_LICENSE_RTF   ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.rtf)

# --- macOS DMG ---
set(CPACK_DMG_VOLUME_NAME  "solver")
set(CPACK_DMG_FORMAT       "UDZO")

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
* Tue May 19 2026 maksnn78gmail.com  0.1.0.0
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

      - name: Package DEB + TGZ + ZIP
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

      - name: Package RPM
        run: cd _build && cpack -G RPM

      - uses: actions/upload-artifact@v4
        with:
          name: linux-rpm-packages
          path: _build/*.rpm

  build-msi:
    runs-on: windows-latest
    steps:
      - uses: actions/checkout@v4
        with:
          submodules: recursive

      - name: Install WiX Toolset v4
        run: dotnet tool install --global wix --version 4.0.5

      - name: Add WiX extension
        run: wix extension add WixToolset.UI.wixext/4.0.5 --global

      - name: Configure
        run: cmake -S . -B _build -DBUILD_TESTS=OFF

      - name: Build
        run: cmake --build _build --config Release

      - name: Package MSI
        run: |
          cd _build
          cpack -G WIX -C Release

      - uses: actions/upload-artifact@v4
        with:
          name: windows-msi-packages
          path: _build/*.msi

  build-dmg:
    runs-on: macos-latest
    steps:
      - uses: actions/checkout@v4
        with:
          submodules: recursive

      - name: Configure
        run: cmake -S . -B _build -DBUILD_TESTS=OFF -DCMAKE_POLICY_VERSION_MINIMUM=3.5

      - name: Build
        run: cmake --build _build

      - name: Package DMG
        run: cd _build && cpack -G DragNDrop

      - uses: actions/upload-artifact@v4
        with:
          name: macos-dmg-packages
          path: _build/*.dmg

  release:
    needs: [build-deb, build-rpm, build-msi, build-dmg]
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

      - uses: actions/download-artifact@v4
        with:
          name: windows-msi-packages
          path: packages/

      - uses: actions/download-artifact@v4
        with:
          name: macos-dmg-packages
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
## Сборка проекта
```bash
cmake -S . -B _build -DBUILD_TESTS=OFF
cmake --build _build
```
## Создание пакетов локально
```bash
cd _build
cpack -G TGZ
cpack -G ZIP
cpack -G DEB
cpack -G RPM
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
        LICENSE.rtf \
        DESCRIPTION \
        ChangeLog.md \
        .github/workflows/ci.yml \
        .github/workflows/release.yml
git commit -m "add CPack config and release workflow"
git push origin main
```
## Создание тега v0.1.0.0 и запуск релиза
```bash
git tag v0.1.0.0
git push origin v0.1.0.0
```
