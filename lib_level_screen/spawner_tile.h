#pragma once

#include <iostream>

#include "bot.h"
#include "lib_level_screen/level_screen.h"
#include "tile.h"

class Bot;

class SpawnerTile : public Tile {
   private:
    Bot* bot;

   public:
    SpawnerTile(LevelScreen* window, sf::Vector2f pos, sf::Vector2u tile_pos);
    ~SpawnerTile();
};