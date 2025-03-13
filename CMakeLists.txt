cmake_minimum_required(VERSION 3.14)
project(json_beautifier VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# download CPM.cmake
file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)


CPMAddPackage(
  NAME CLI11
  GITHUB_REPOSITORY CLIUtils/CLI11
  VERSION 2.5.0
)

CPMAddPackage(
  NAME nlohmann_json
  GITHUB_REPOSITORY nlohmann/json
  VERSION 3.11.2
)

add_executable(json_beautifier main.cpp)
target_link_libraries(json_beautifier PRIVATE CLI11::CLI11 nlohmann_json::nlohmann_json)
