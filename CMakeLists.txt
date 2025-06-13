cmake_minimum_required(VERSION 3.10)

project(MyShell)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Gộp toàn bộ source từ src/ và các thư mục con
file(GLOB_RECURSE SOURCES
    src/*.cpp
)

add_subdirectory(programs)

# Include thư mục headers
include_directories(src)

# Tạo executable từ tất cả các file cpp
add_executable(MyShell main.cpp ${SOURCES})
