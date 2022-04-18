cmake_minimum_required(VERSION 3.10)
project(Dilara_Karakas_171044010)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

set(SOURCE_FILES main.cpp Timer.cpp)
add_executable(exe ${SOURCE_FILES})

find_package(Threads)
target_link_libraries(exe Threads::Threads)