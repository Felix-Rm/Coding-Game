#include "tile.h"

const std::string Tile::path = "_assets/textures/tiles/";
sf::Vector2f Tile::tex_size = {0, 0};
bool Tile::textures_loaded = false;
sf::Texture Tile::textures[tile_types::_COUNT];

Tile::Tile(Window* window, bool is_drivable, int tex_id, sf::Vector2f pos) : Drawable(window, {pos.x, pos.y}, tex_size) {
    if (!textures_loaded)
        throw std::runtime_error("Textures have to be loaded before Tile can be constructed!");

    this->background = sf::Sprite(textures[tex_id]);
    this->background.setPosition(this->pos);
}

void Tile::loadTextures() {
    if (textures_loaded) {
        return;
    }

    for (int i = 0; i < tile_types::_COUNT; i++) {
        textures[i].loadFromFile(path + std::to_string(i) + ".png");
    }

    tex_size = (sf::Vector2f)textures[0].getSize();
    textures_loaded = true;
}

void Tile::setPosition(float x, float y) {
    this->pos = {x, y};
    this->background.setPosition(this->pos);
};

void Tile::shiftPosition(float dx, float dy) {
    this->pos.x += dx;
    this->pos.y += dy;

    this->background.setPosition(this->pos);
};

void Tile::setScale(float s) {
    this->background.setScale(s, s);
}

void Tile::render() {
    this->window->draw(this->background);
};
