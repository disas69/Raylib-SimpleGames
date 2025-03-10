cmake_minimum_required(VERSION 3.30)

set(PROJECT_NAME RaylibGames)
project(${PROJECT_NAME})

set(RAYLIB_PATH ${PROJECT_SOURCE_DIR}/raylib)
include_directories(${RAYLIB_PATH}/include include)
link_directories(${RAYLIB_PATH}/lib)

add_executable(${PROJECT_NAME} src/main.cpp src/PingPong.cpp)
target_link_libraries(${PROJECT_NAME} raylib)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} opengl32 gdi32 winmm)
endif ()