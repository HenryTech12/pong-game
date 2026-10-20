cmake_minimum_required(VERSION 3.14)
project(pong-game C CXX)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

add_executable(pong-game main.cpp)
target_link_libraries(pong-game raylib)

if (WIN32)
    target_link_libraries(pong-game opengl32 gdi32 winmm)
endif()