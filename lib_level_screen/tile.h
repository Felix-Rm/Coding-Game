
#pragma once

#include <iostream>
#include <fstream>
#include "lib_window/drawable.h"

class Tile : public Drawable {
   public:
    enum tile_type {
        AIR,
        FLOOR,
        WALL,
        SPAWNER,
        _COUNT
    };
    Tile(Window* window, tile_type type, sf::Vector2f pos);

    Tile(Tile&& other) : Drawable(other) {
        copyFrom(other);
        loadBackground();
    }

    Tile(Tile& other) : Drawable(other) {
        copyFrom(other);
        loadBackground();
    }

    Tile& operator=(Tile&& other) {
        Drawable::operator=(std::move(other));
        copyFrom(other);
        loadBackground();
        return *this;
    }

    friend std::ifstream& operator>>(std::ifstream& data, Tile&);

    friend std::ofstream& operator<<(std::ofstream& data, const Tile& obj);

    static sf::Vector2f tex_size;

    void setPosition(float x, float y) override;
    void shiftPosition(float dx, float dy) override;
    void setScale(float s);

    tile_type getType() { return type; };
    bool isDrivable() { return drivable; };

    void render() override;

    static void loadTextures();

   private:
    static bool textures_loaded;
    static sf::Texture textures[tile_type::_COUNT];
    static bool type_drivable[tile_type::_COUNT];

    const static std::string path;

    void loadBackground();
    void copyFrom(Tile& other);

    sf::Sprite background;
    tile_type type;
    bool drivable;
};
