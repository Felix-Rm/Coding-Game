#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "../lib_window/drawable.h"
#include "tile.h"
#include "level_screen.h"

class LevelScreen;

class Bot : public Drawable
{
private:
    enum movement_state
    {

        DONE,
        MOVING,
        JUST_DONE
    };

    static std::string path;
    static bool textures_loaded;

    static constexpr int num_textures = 1;
    static sf::Texture textures[num_textures];

    static constexpr int updates_per_movement = 100;

    static constexpr float delta_rotation_per_update = 45.0 / updates_per_movement;
    static sf::Vector2f delta_tile_position_per_update;

    float target_rotation = 0;
    sf::Vector2f target_position;

    sf::Sprite body;

    movement_state movement_complete = DONE;

    sf::Vector2f tile_position;
    float current_rotation = 0;

    LevelScreen *level;

    void applyOffset();

public:
    enum option
    {
        clockwise,
        counterclockwise,
        forward,
        backward
    };

    Bot(sf::Vector2f pos, sf::Vector2f tile_pos, std::ifstream &data, LevelScreen *lvl);
    void setPosition(float x, float y) override;
    void shiftPosition(float dx, float dy) override;
    void setScale(float s);

    void updatePosition();

    bool rotate(option dir);
    bool drive(option dir);

    void render(Window *window) override;

    void update();

    static void loadTextures();
};
