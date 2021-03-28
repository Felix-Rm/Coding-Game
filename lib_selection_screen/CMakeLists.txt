cmake_minimum_required(VERSION 2.8)

add_library(selection_screen "selection_screen.cpp" "stage.cpp" )
target_link_libraries(selection_screen window)