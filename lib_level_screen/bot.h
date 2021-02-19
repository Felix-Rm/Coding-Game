#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "../lib_window/drawable.h"
#include "tile.h"

class Bot : public Drawable
{
private:
    static std::string path;
    static bool textures_loaded;

    static constexpr int num_textures = 1;
    static sf::Texture textures[num_textures];

    sf::Sprite body;

    void applyOffset();

public:
    sf::Vector2i tile_position;

    Bot(sf::Vector2f pos, sf::Vector2i tile_pos, std::ifstream &data);
    void setPosition(float x, float y) override;
    void shiftPosition(float dx, float dy) override;
    void setScale(float s);

    void render(Window *window) override;

    static void loadTextures();
};
