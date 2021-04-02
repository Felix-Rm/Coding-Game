#include "finish_tile.h"

FinishTile::FinishTile(LevelScreen* window, sf::Vector2f pos, sf::Vector2u tile_pos) : Tile(window, Tile::tile_type::FINISH, pos, tile_pos) {
    lvl->win_conditions.push_back(this);
}

FinishTile::~FinishTile() {
    auto it = std::find(lvl->win_conditions.begin(), lvl->win_conditions.end(), this);
    lvl->win_conditions.erase(it);
}

void FinishTile::update() {
    for (auto& bot : lvl->bots) {
        if (bot->getState() == Bot::movement_state::IDLE && bot->getTilePosition() == this->tile_pos) {
            bot_on_tile = true;
            return;
        }
    }

    bot_on_tile = false;
}