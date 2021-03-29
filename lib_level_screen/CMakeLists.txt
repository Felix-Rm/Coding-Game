cmake_minimum_required(VERSION 2.8)

add_library(level_screen "level_screen.cpp" "tile.cpp" "spawner_tile.cpp" "bot.cpp" "top_bar.cpp")
target_link_libraries(level_screen window)