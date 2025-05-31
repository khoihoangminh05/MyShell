cmake_minimum_required(VERSION 3.10)
project(MyShell)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Tìm tất cả file nguồn trong thư mục src và các thư mục con
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.c"
    "src/*.cc"
)

# Hiển thị danh sách file nguồn tìm được (debug)
message(STATUS "Source files found: ${SOURCES}")

# Tạo executable từ tất cả file nguồn
add_executable(MyShell main.cpp ${SOURCES})

# Chỉ định thư mục include - QUAN TRỌNG!
target_include_directories(MyShell PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
)

# Thêm thư viện nếu cần (ví dụ cho Windows)
if(WIN32)
    target_link_libraries(MyShell PRIVATE shell32)
endif()