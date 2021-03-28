cmake_minimum_required(VERSION 2.8)

add_library(sandbox_screen "sandbox_screen.cpp"  )
target_link_libraries(sandbox_screen selection_screen)