cmake_minimum_required (VERSION 3.1)
project (game)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")
set(EXECUTABLE_OUTPUT_PATH ../)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})

link_directories(${PROJECT_SOURCE_DIR}/lib_helpers)
add_subdirectory(lib_helpers)

link_directories(${PROJECT_SOURCE_DIR}/lib_window)
add_subdirectory(lib_window)

link_directories(${PROJECT_SOURCE_DIR}/lib_title_screen)
add_subdirectory(lib_title_screen)

link_directories(${PROJECT_SOURCE_DIR}/lib_stage_screen)
add_subdirectory(lib_stage_screen)

link_directories(${PROJECT_SOURCE_DIR}/lib_level_screen)
add_subdirectory(lib_level_screen)

add_executable (game main.cpp)

target_link_libraries(game helpers window title_screen stage_screen level_screen)
target_link_libraries (game sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
