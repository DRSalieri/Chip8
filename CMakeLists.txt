cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)
project(chip8emulator)

# 设置 SDL2 头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/include)
# 设置 SDL2 静态库文件目录
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/lib)
# 链接 SDL2 静态库文件
# link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/lib/libSDL2main.a)
# link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/lib/libSDL2.dll.a)
# link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/lib/libSDL2.a)
# 设置 头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
# 源文件
file(GLOB_RECURSE SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
# 添加可执行文件
add_executable(chip8 ${SRC_LIST})
# 链接SDL2库
target_link_libraries(chip8 mingw32 SDL2main SDL2)