cmake_minimum_required(VERSION 3.12)
project(untitled2)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/Cellar/tbb)
link_directories(/usr/local/Cellar/tbb)
add_executable(untitled2 main.cpp)
target_link_libraries(untitled2 tbb)