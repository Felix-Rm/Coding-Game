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
#include "bot.h"

class Bot;

class LevelScreen : public Window
{
protected:
    std::string path;
    sf::Vector2u size;
    int num_bots = 0;

    float scale = 1;
    sf::Vector2f origin = {0, 0};

    sf::Vector2f mouse_pos = {0, 0};
    sf::Vector2f last_mouse_click_pos = {0, 0};
    sf::Vector2f prev_origin = {0, 0};
    bool mouse_down = false;

    std::vector<std::vector<Tile *>> map;
    std::vector<Bot *> bots;

    sf::Texture top_bar_tex;
    sf::Sprite top_bar;

    void setup() override;
    void render() override;

    void updatePosition();

    static bool onMouseMove(sf::Event &, void *);
    static bool onMouseButton(sf::Event &, void *);
    static bool onScroll(sf::Event &, void *);

public:
    LevelScreen(sf::VideoMode v, std::string title, sf::Uint32 style, int level_num, std::string &path);

    friend Bot;
};
