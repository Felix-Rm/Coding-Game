#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "../lib_window/dialog.h"
#include "../lib_window/style.h"
#include "../lib_window/text.h"
#include "../lib_window/window.h"
#include "bonus_tile.h"
#include "bot.h"
#include "finish_tile.h"
#include "spawner_tile.h"
#include "tile.h"
#include "top_bar.h"
#include "win_condition.h"

class Bot;
class TopBar;
class Tile;
class SpawnerTile;
class FinishTile;
class BonusTile;

class LevelScreen : public Window {
   public:
    LevelScreen(sf::VideoMode video_mode, std::string title, sf::Uint32 style, int level_num, std::string &path);

    void activateManualControlls();

    friend SpawnerTile;
    friend FinishTile;
    friend BonusTile;
    friend Bot;
    friend TopBar;

    bool next_level_exists = false;
    bool next_level_flag = false;

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
    std::vector<WinCondition *> win_conditions;

    bool boni_collected;

    static constexpr long original_ms_per_unit_movement = 1000;
    int movement_multiplyer = 2;

    long ms_per_unit_movement;

    float elapsed_time = 0;

    TopBar *top_bar;
    Dialog *level_complete_dialog = nullptr;

    Tile *generateTileFromId(int id, sf::Vector2f pos, sf::Vector2u tile_pos);

    friend std::ifstream &operator>>(std::ifstream &data, LevelScreen &obj);
    friend std::ofstream &operator<<(std::ofstream &data, const LevelScreen &obj);

    void setup() override;
    void render() override;
    void updatePosition();

   private:
    static sf::Texture star_textures[2][3];
    static bool textures_loaded;

    void loadTextures();

    static bool onMouseMove(sf::Event &, void *);
    static bool onMouseButton(sf::Event &, void *);
    static bool onScroll(sf::Event &, void *);

    static bool onNextLevel(sf::Event &, void *);
};
