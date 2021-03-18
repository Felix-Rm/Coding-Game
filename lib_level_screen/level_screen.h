#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../lib_window/window.h"
#include "../lib_window/button.h"
#include "../lib_window/style.h"

#include "top_bar.h"
#include "tile.h"
#include "bot.h"

class Bot;
class TopBar;

class LevelScreen : public Window {
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

    
    float elapsed_time = 0;

    TopBar *top_bar;

    void setup() override;
    void render() override;

    void updatePosition();

    static bool onMouseMove(sf::Event &, void *);
    static bool onMouseButton(sf::Event &, void *);
    static bool onScroll(sf::Event &, void *);

   public:
    LevelScreen(sf::VideoMode video_mode, std::string title, sf::Uint32 style, int level_num, std::string &path);

    friend Bot;
    friend TopBar;
};
