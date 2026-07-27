cmake_minimum_required(VERSION 3.20)

project(
  MiniRedBase
  VERSION 0.1.0
  DESCRIPTION "A small educational relational database written in modern C++"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MINIREDBASE_BUILD_TESTS "Build MiniRedBase tests" ON)

add_library(miniredbase_core STATIC src/core.cpp)
target_include_directories(
  miniredbase_core
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_compile_features(miniredbase_core PUBLIC cxx_std_20)

if(MSVC)
  target_compile_options(miniredbase_core PRIVATE /W4 /permissive-)
else()
  target_compile_options(miniredbase_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(miniredbase src/main.cpp)
target_link_libraries(miniredbase PRIVATE miniredbase_core)

if(MINIREDBASE_BUILD_TESTS)
  include(CTest)
  add_subdirectory(tests)
endif()

