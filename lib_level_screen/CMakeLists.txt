cmake_minimum_required(VERSION 2.8)

add_library(level_screen "level_screen.cpp" "tile.cpp" "bonus_tile.cpp" "spawner_tile.cpp" "finish_tile.cpp" "bot.cpp" "top_bar.cpp")
target_link_libraries(level_screen window)