# Copyright Ohyyp 2026
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.14)
project(EndfieldGachaPlanner CXX)

# Require C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dual Compiler Workflow: Add strict warnings for Clang and GCC
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter)
endif()

include(FetchContent)

# Fetch toml++ for TOML parsing
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)

# Fetch CLI11 for command line parsing
FetchContent_Declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG        v2.4.1
)
FetchContent_MakeAvailable(cli11)

# Main executable target
add_executable(EndfieldGacha 
    src/main.cpp 
    src/MonteCarloEngine.cpp 
    src/DynamicProgrammingEngine.cpp
    src/Banner.cpp
    src/GachaConfig.cpp
    src/GachaMechanics.cpp
    src/SteadyStateMath.cpp
    src/ConfigLoader.cpp
)

target_include_directories(EndfieldGacha PRIVATE include)
target_link_libraries(EndfieldGacha PRIVATE tomlplusplus::tomlplusplus CLI11::CLI11)
