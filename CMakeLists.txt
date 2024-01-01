cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
project(chip8emulator)

# 设置 SDL2 头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/include)
# 设置 SDL2 静态库文件目录
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/lib)
# 链接 SDL2 静态库文件
link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/lib/libSDL2.a)

# 设置 头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
# 源文件
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_LIST)
# 添加可执行文件
add_executable(chip8 ${SRC_LIST})
