cmake_minimum_required (VERSION 3.1)
project (game)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -Wall")
set(EXECUTABLE_OUTPUT_PATH ../)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})

link_directories(${PROJECT_SOURCE_DIR}/lib_helpers)
add_subdirectory(lib_helpers)

link_directories(${PROJECT_SOURCE_DIR}/lib_window)
add_subdirectory(lib_window)

link_directories(${PROJECT_SOURCE_DIR}/lib_selection_screen)
add_subdirectory(lib_selection_screen)

link_directories(${PROJECT_SOURCE_DIR}/lib_title_screen)
add_subdirectory(lib_title_screen)

link_directories(${PROJECT_SOURCE_DIR}/lib_campaign_screen)
add_subdirectory(lib_campaign_screen)

link_directories(${PROJECT_SOURCE_DIR}/lib_sandbox_screen)
add_subdirectory(lib_sandbox_screen)

link_directories(${PROJECT_SOURCE_DIR}/lib_level_screen)
add_subdirectory(lib_level_screen)

link_directories(${PROJECT_SOURCE_DIR}/lib_level_editor_screen)
add_subdirectory(lib_level_editor_screen)

add_executable (game main.cpp)

target_link_libraries(game title_screen campaign_screen sandbox_screen level_screen level_editor_screen)
target_link_libraries (game sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
