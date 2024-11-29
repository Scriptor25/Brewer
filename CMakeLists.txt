cmake_minimum_required(VERSION 3.25)
project(Brewer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE sources "src/*.cpp" "include/*.hpp")
add_library(brewer ${sources})
target_include_directories(brewer PUBLIC "include")

add_executable(example example/main.cpp)
target_link_libraries(example PRIVATE brewer)
