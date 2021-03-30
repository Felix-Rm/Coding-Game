cmake_minimum_required(VERSION 2.8)

add_library(level_editor_screen "level_editor_screen.cpp")
target_link_libraries(level_editor_screen level_screen)