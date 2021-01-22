cmake_minimum_required(VERSION 2.8)

add_library(stage_screen "stage_screen.cpp" "stage.cpp" )
target_link_libraries(stage_screen window)