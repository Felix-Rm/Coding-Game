cmake_minimum_required (VERSION 3.1)
project (game)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

add_executable (game main.cpp)
target_link_libraries (game sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
