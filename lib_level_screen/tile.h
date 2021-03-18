
#pragma once

#include <cassert>
#include "../lib_window/drawable.h"

enum tile_types {
    AIR,
    FLOOR,
    WALL,
    _COUNT
};

class Tile : public Drawable {
   private:
    static bool textures_loaded;
    static sf::Texture textures[tile_types::_COUNT];

    const static std::string path;

    sf::Sprite background;

   public:
    Tile(Window* window, bool is_drivable, int tex_id, sf::Vector2f pos);

    static sf::Vector2f tex_size;

    void setPosition(float x, float y) override;
    void shiftPosition(float dx, float dy) override;
    void setScale(float s);

    void render() override;

    static void loadTextures();
};
