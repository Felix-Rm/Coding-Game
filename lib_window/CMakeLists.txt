cmake_minimum_required(VERSION 2.8)

add_library(window "window.cpp" "button.cpp")
target_link_libraries(window helpers)