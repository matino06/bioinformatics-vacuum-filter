cmake_minimum_required(VERSION 3.16)
project(vacuum_filter VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Release build by default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=address")

# Main executable
add_executable(vacuum_filter
    src/main.cpp
    src/vacuum_filter.cpp
)

target_include_directories(vacuum_filter PRIVATE src)
