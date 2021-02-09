
#pragma once

#include <cassert>
#include "../lib_window/drawable.h"

enum tile_types
{
    AIR,
    FLOOR,
    WALL,
    _COUNT
};

class Tile : public Drawable
{
private:
    static bool textures_loaded;
    static sf::Texture textures[tile_types::_COUNT];
    static sf::Vector2f tex_size;

    const static std::string path;

    sf::Sprite background;

public:
    Tile(bool is_drivable, int tex_id, sf::Vector2f pos);

    void setPosition(int x, int y) override;
    void shiftPosition(int dx, int dy) override;

    void render(Window *window) override;

    static void loadTextures();
};
