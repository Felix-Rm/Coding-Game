#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../lib_window/window.h"
#include "../lib_window/button.h"
#include "../lib_window/style.h"

#include "tile.h"

class LevelScreen : public Window
{
private:
    enum tile_types
    {
        AIR,
        FLOOR,
        WALL
    };

    std::string path;
    sf::Vector2u size;

    std::vector<std::vector<Tile *>> map;

    void setup() override;
    void render() override;

public:
    LevelScreen(sf::VideoMode v, std::string title, sf::Uint32 style, int level_num, std::string &path);
};
