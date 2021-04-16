cmake_minimum_required(VERSION 2.8)

add_library(campaign_screen "campaign_screen.cpp" )
target_link_libraries(campaign_screen selection_screen)