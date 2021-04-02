#pragma once

#include <iostream>

#include "../lib_level_screen/level_screen.h"
#include "bot.h"
#include "tile.h"
#include "win_condition.h"

class Bot;

class FinishTile : public Tile, public WinCondition {
   public:
    FinishTile(LevelScreen* window, sf::Vector2f pos, sf::Vector2u tile_pos);
    ~FinishTile();

    void update() override;
    bool isMet() override { return bot_on_tile; };

   private:
    bool bot_on_tile = false;
};