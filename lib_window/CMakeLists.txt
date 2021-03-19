cmake_minimum_required(VERSION 2.8)

add_library(window "window.cpp" "button.cpp" "image_button.cpp" "style.cpp")
target_link_libraries(window helpers)