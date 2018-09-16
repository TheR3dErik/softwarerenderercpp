cmake_minimum_required(VERSION 3.12)
project(SDLsoftwarerenderer)

set(CMAKE_CXX_STANDARD 14)
find_package(SDL2 REQUIRED)

add_executable(SDLsoftwarerenderer main.cpp)
target_link_libraries(SDLsoftwarerenderer ${SDL2_LIBRARIES})