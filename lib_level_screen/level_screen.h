#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "../lib_window/style.h"
#include "../lib_window/window.h"
#include "bot.h"
#include "spawner_tile.h"
#include "tile.h"
#include "top_bar.h"

class Bot;
class TopBar;
class Tile;
class SpawnerTile;

class LevelScreen : public Window {
   public:
    LevelScreen(sf::VideoMode video_mode, std::string title, sf::Uint32 style, int level_num, std::string &path);

    void activateManualControlls();

    friend SpawnerTile;
    friend Bot;
    friend TopBar;

   protected:
    std::string path;
    sf::Vector2u size;

    float scale = 1;
    sf::Vector2f origin = {0, 0};

    sf::Vector2f mouse_pos = {0, 0};
    sf::Vector2f last_mouse_click_pos = {0, 0};
    sf::Vector2f prev_origin = {0, 0};
    bool mouse_down[3] = {false};

    std::vector<std::vector<Tile *>> map;
    std::vector<Bot *> bots;

    static constexpr long original_ms_per_unit_movement = 1000;
    int movement_multiplyer = 1;

    long ms_per_unit_movement;

    float elapsed_time = 0;

    TopBar *top_bar;

    Tile *generateTileFromId(int id, sf::Vector2f pos, sf::Vector2u tile_pos);

    friend std::ifstream &operator>>(std::ifstream &data, LevelScreen &obj);
    friend std::ofstream &operator<<(std::ofstream &data, const LevelScreen &obj);

    void setup() override;
    void render() override;
    void updatePosition();

   private:
    static bool onMouseMove(sf::Event &, void *);
    static bool onMouseButton(sf::Event &, void *);
    static bool onScroll(sf::Event &, void *);
};
