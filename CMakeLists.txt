cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_NAME RaylibGames)
project(${PROJECT_NAME})

set(RAYLIB_PATH ${PROJECT_SOURCE_DIR}/raylib)
include_directories(${RAYLIB_PATH}/include include)

add_executable(${PROJECT_NAME} src/main.cpp
        src/PingPong.cpp
)

if (WIN32)
    link_directories(${RAYLIB_PATH}/lib/win)
    target_link_libraries(${PROJECT_NAME} ${RAYLIB_PATH}/lib/win/libraylib.a)

    target_link_libraries(${PROJECT_NAME} opengl32 gdi32 winmm)
elseif (APPLE)
    link_directories(${RAYLIB_PATH}/lib/macos)
    target_link_libraries(${PROJECT_NAME} ${RAYLIB_PATH}/lib/macos/libraylib.a)

    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)

    target_link_libraries(${PROJECT_NAME}
            ${COCOA_LIBRARY}
            ${IOKIT_LIBRARY}
            ${COREVIDEO_LIBRARY}
            "-framework OpenGL"
    )
endif ()
