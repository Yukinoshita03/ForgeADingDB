cmake_minimum_required(VERSION 3.20)

project(
  ForgeADingDB
  VERSION 0.1.0
  DESCRIPTION "A small educational relational database written in modern C++"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Export compile commands" FORCE)

option(FORGEADINGDB_BUILD_TESTS "Build ForgeADingDB tests" ON)

add_library(forgeadingdb_core STATIC src/core.cpp src/storage/page.cpp)
target_include_directories(
  forgeadingdb_core
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_compile_features(forgeadingdb_core PUBLIC cxx_std_20)

if(MSVC)
  target_compile_options(forgeadingdb_core PRIVATE /W4 /permissive-)
else()
  target_compile_options(forgeadingdb_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(forgeadingdb src/main.cpp)
target_link_libraries(forgeadingdb PRIVATE forgeadingdb_core)

if(FORGEADINGDB_BUILD_TESTS)
  include(CTest)
  add_subdirectory(tests)
endif()

