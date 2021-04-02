#include "bonus_tile.h"

sf::Texture BonusTile::bonus_tex;
bool BonusTile::texture_loaded = false;

BonusTile::BonusTile(LevelScreen* window, sf::Vector2f pos, sf::Vector2u tile_pos) : Tile(window, Tile::tile_type::BONUS, pos, tile_pos) {
    if (!texture_loaded) {
        if (!bonus_tex.loadFromFile("_assets/textures/miscellaneous/bonus.png"))
            throw std::runtime_error("Could nt load bonus texture");
        else
            texture_loaded = true;
    }

    bonus = Image(this->window, this->pos, this->size, &(this->bonus_tex));
}

BonusTile::~BonusTile() {
}

void BonusTile::setPosition(float x, float y) {
    Tile::setPosition(x, y);
    bonus.setPosition(x, y);
}

void BonusTile::shiftPosition(float dx, float dy) {
    Tile::shiftPosition(dx, dy);
    bonus.shiftPosition(dx, dy);
}

void BonusTile::setScale(float s) {
    Tile::setScale(s);
    bonus.setScale(s);
}

void BonusTile::update() {
    lvl->boni_collected &= bonus_collected;

    if (bonus_collected) return;

    for (auto& bot : lvl->bots) {
        if (bot->getState() == Bot::movement_state::IDLE && bot->getTilePosition() == this->tile_pos && !bonus_collected) {
            bonus_collected = true;
            return;
        }
    }
}

void BonusTile::render() {
    Tile::render();
    if (!this->bonus_collected)
        this->bonus.render();
}