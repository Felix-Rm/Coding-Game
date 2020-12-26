cmake_minimum_required(VERSION 2.8)

add_library(title_screen "title_screen.cpp" )
target_link_libraries(title_screen window)