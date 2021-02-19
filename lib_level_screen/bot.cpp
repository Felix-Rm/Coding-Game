#include "bot.h"

std::string Bot::path = "assets/textures/bots/";
bool Bot::textures_loaded = false;
sf::Texture Bot::textures[Bot::num_textures];

Bot::Bot(sf::Vector2f pos, sf::Vector2i tile_pos, std::ifstream &data) : Drawable(pos, Tile::tex_size)
{
    if (!textures_loaded)
        throw std::runtime_error("Textures have to be loaded before Bot can be constructed!");

    this->applyOffset();

    this->body = sf::Sprite(textures[0]);
    this->body.setOrigin(Tile::tex_size.x / 2, Tile::tex_size.y / 2);
    this->body.setPosition(this->pos);

    this->body.setRotation(90);

    this->tile_position = tile_pos;
}

void Bot::setPosition(float x, float y)
{
    this->pos = {x, y};
    this->applyOffset();

    this->body.setPosition(this->pos);
};

void Bot::shiftPosition(float dx, float dy)
{
    this->pos.x += dx;
    this->pos.y += dy;

    this->body.setPosition(this->pos);
};

void Bot::applyOffset()
{
    auto scale = this->body.getScale();
    this->pos.x += Tile::tex_size.x / 2 * scale.x;
    this->pos.y += Tile::tex_size.y / 2 * scale.y;
}

void Bot::setScale(float s)
{
    this->body.setScale(s, s);
}

void Bot::render(Window *window)
{
    this->body.rotate(1);
    window->draw(this->body);
}

void Bot::loadTextures()
{
    if (textures_loaded)
    {
        return;
    }

    for (int i = 0; i < num_textures; i++)
    {
        textures[i].loadFromFile(path + std::to_string(i) + ".png");
    }

    textures_loaded = true;
}