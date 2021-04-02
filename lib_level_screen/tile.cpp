#include "tile.h"

const std::string Tile::path = "_assets/textures/tiles/";
sf::Vector2f Tile::tex_size = {0, 0};
bool Tile::textures_loaded = false;
sf::Texture Tile::textures[tile_type::_COUNT];
bool Tile::type_drivable[tile_type::_COUNT] = {
    false,
    true,
    false,
    true,
    true};

Tile::Tile(LevelScreen* window, tile_type type, sf::Vector2f pos, sf::Vector2u tile_pos) : Drawable((Window*)window, {pos.x, pos.y}, tex_size) {
    if (!textures_loaded)
        throw std::runtime_error("Textures have to be loaded before Tile can be constructed!");

    this->lvl = window;
    this->tile_pos = tile_pos;
    this->type = type;
    this->drivable = type_drivable[type];
    loadBackground();
}

std::ifstream& operator>>(std::ifstream& data, Tile&) {
    return data;
};

std::ofstream& operator<<(std::ofstream& data, const Tile& obj) {
    data << ((int)obj.type);
    return data;
}

void Tile::loadTextures() {
    if (textures_loaded) {
        return;
    }

    for (int i = 0; i < tile_type::_COUNT; i++) {
        textures[i].loadFromFile(path + std::to_string(i) + ".png");
    }

    tex_size = (sf::Vector2f)textures[0].getSize();
    textures_loaded = true;
}

void Tile::loadBackground() {
    this->background = sf::Sprite(textures[this->type]);
    this->background.setPosition(this->pos);
}

void Tile::copyFrom(Tile& other) {
    drivable = other.drivable;
    type = other.type;
}

void Tile::setPosition(float x, float y) {
    this->pos = {x, y};
    this->background.setPosition(this->pos);
};

void Tile::setTilePosition(unsigned int x, unsigned int y) {
    tile_pos.x = x;
    tile_pos.y = y;
}

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
