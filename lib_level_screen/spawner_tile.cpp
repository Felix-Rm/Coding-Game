#include "spawner_tile.h"

SpawnerTile::SpawnerTile(LevelScreen* window, sf::Vector2f pos, sf::Vector2u tile_pos) : Tile(window, Tile::tile_type::SPAWNER, pos, tile_pos) {
    this->bot = new Bot(window, this->pos, this->tile_pos);
    this->lvl->bots.push_back(this->bot);
}

SpawnerTile::~SpawnerTile() {
    auto bot_it = std::find(this->lvl->bots.begin(), this->lvl->bots.end(), bot);
    this->lvl->bots.erase(bot_it);

    delete bot;
}
