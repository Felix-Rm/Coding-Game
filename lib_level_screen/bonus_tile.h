#pragma once

#include <iostream>

#include "../lib_level_screen/level_screen.h"
#include "../lib_window/image.h"
#include "bot.h"
#include "tile.h"
#include "win_condition.h"

class Bot;

class BonusTile : public Tile {
   public:
    BonusTile(LevelScreen* window, sf::Vector2f pos, sf::Vector2u tile_pos);
    ~BonusTile();

    void update() override;

    void render() override;

    void setPosition(float x, float y) override;
    void shiftPosition(float dx, float dy) override;
    void setScale(float s) override;

   private:
    static sf::Texture bonus_tex;
    static bool texture_loaded;

    Image bonus;
    bool bonus_collected = false;
};