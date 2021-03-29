#include "tile.h"
#include <iostream>

class SpawnerTile : public Tile {
   public:
    SpawnerTile(Window* window, sf::Vector2f pos);
};